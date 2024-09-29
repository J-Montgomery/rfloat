#include "testfile_loader.hh"
#include <iostream>
#include <rfloat.hh>

int main() {
    try {
        std::vector<std::array<float, 3>> dataToWrite = {{1.0f, 2.1f, 3.4},
                                                         {3.0f, 4.0f, 3.4},
                                                         {5.0f, 6.0f, 3.4},
                                                         {7.0f, 8.0f, 4.0},
                                                         {9.0f, 10.0f, 5.0}};

        BinaryFileLoader<float, 3> loader("data.bin", 5);
        loader.write(dataToWrite);
        std::cout << "Data written successfully." << std::endl;

        auto dataRead = loader.read();

        std::cout << "Data read from file:" << std::endl;
        for (const auto &[first, second, third] : dataRead) {
            std::cout << "(" << first << ", " << second << ", " << third << ")"
                      << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}