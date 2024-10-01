#pragma once

#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <cstdint>
#include <type_traits>
#include <cstring>

template<typename T, size_t ArraySize>
class BinaryFileLoader {
public:
    BinaryFileLoader(const std::string& path, size_t numItems)
        : m_path(path), m_record_size(numItems) {
        static_assert(std::is_trivially_copyable<T>::value, "T must be trivially copyable");
        static_assert(sizeof(T) == sizeof(UnsignedType), "Size mismatch in conversion");
    }

        void write(const std::vector<std::array<T, ArraySize>>& data) {
        std::ofstream file(m_path, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Unable to open file for writing");
        }

        uint32_t endianness = 0x01020304;
        file.write(reinterpret_cast<const char*>(&endianness), sizeof(endianness));

        file.write(reinterpret_cast<const char*>(&m_record_size), sizeof(m_record_size));

        for (const auto& arr : data) {
            for (const auto& item : arr) {
                auto bits = serialize_t(item);
                file.write(reinterpret_cast<const char*>(&bits), sizeof(bits));
            }
        }
    }

    std::vector<std::array<T, ArraySize>> read() {
        std::ifstream file(m_path, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Unable to open file for reading");
        }

        uint32_t endianness;
        file.read(reinterpret_cast<char*>(&endianness), sizeof(endianness));
        bool do_swap = (endianness != 0x01020304);

        size_t num_items;
        file.read(reinterpret_cast<char*>(&num_items), sizeof(num_items));
        if (do_swap) {
            num_items = endian_swap(num_items);
        }

        if (num_items != m_record_size) {
            throw std::runtime_error("Unexpected number of items in file");
        }

        std::vector<std::array<T, ArraySize>> result(m_record_size);
        for (auto& arr : result) {
            for (auto& item : arr) {
                UnsignedType bits;
                file.read(reinterpret_cast<char*>(&bits), sizeof(bits));
                if (do_swap) {
                    bits = endian_swap(bits);
                }
                item = deserialize_t(bits);
            }
        }

        return result;
    }


private:
    std::string m_path;
    size_t m_record_size;

    // Define an unsigned integer type with the same size as T
    using UnsignedType = typename std::conditional<sizeof(T) == 8, std::uint64_t,
                      typename std::conditional<sizeof(T) == 4, std::uint32_t,
                      typename std::conditional<sizeof(T) == 2, std::uint16_t,
                      std::uint8_t>::type>::type>::type;

    static UnsignedType serialize_t(T value) {
        UnsignedType result;
        static_assert(sizeof(T) == sizeof(result), "Size mismatch in conversion");
        std::memcpy(&result, &value, sizeof(result));
        return result;
    }

    static T deserialize_t(UnsignedType value) {
        T result;
        static_assert(sizeof(T) == sizeof(result), "Size mismatch in conversion");
        std::memcpy(&result, &value, sizeof(result));
        return result;
    }

    // Unfortunately C++17 doesn't have std::byteswap
    template<typename U>
    static U endian_swap(U value) {
        if constexpr (sizeof(T) == 1) {
            return value;
        } else if constexpr (sizeof(T) == 2) {
            return static_cast<T>(((value & 0xFF) << 8) | ((value >> 8) & 0xFF));
        } else if constexpr (sizeof(T) == 4) {
            return static_cast<T>(
                ((value & 0xFF) << 24) |
                ((value & 0xFF00) << 8) |
                ((value >> 8) & 0xFF00) |
                ((value >> 24) & 0xFF)
            );
        } else if constexpr (sizeof(T) == 8) {
            return static_cast<T>(
                ((value & 0xFF) << 56) |
                ((value & 0xFF00) << 40) |
                ((value & 0xFF0000) << 24) |
                ((value & 0xFF000000) << 8) |
                ((value >> 8) & 0xFF000000) |
                ((value >> 24) & 0xFF0000) |
                ((value >> 40) & 0xFF00) |
                ((value >> 56) & 0xFF)
            );
        } else {
            static_assert(sizeof(T) <= 8, "Not implemented");
            return value;
        }
    }
};
