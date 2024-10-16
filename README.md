# **rfloat**

**rfloat** is an experimental header-only library implementing reproducible floating point types with no overhead. Convert existing code with single letter changes.

## Table of Contents
- [Overview](#overview)
- [Usage](#usage)
- [Platform Support](#platforms)
- [Project Goals](#goals)
- [Limitations](#limitations)
- [Issues](#issues)
- [Benchmarks](#benchmarks)
- [License](#license)
- [Credit](#credit)

## Overview

The IEEE-754 standard that defines floating point arithmetic is deterministic under certain conditions. However, floating point code is rarely deterministic and reproducible  in the real world due to:
- compiler flags
- compiler optimizations
- nondeterministic implementations
- The [Tablemaker's Dilemma](https://perso.ens-lyon.fr/jean-michel.muller/Intro-to-TMD.htm)

This non-reproducibility creates problems for simulations and high-assurance code. Libraries like [dmath](https://github.com/sixitbb/sixit-dmath) implement reproducible
floating point through a variety of strategies. These strategies come with significant tradeoffs and performance implications. **rfloat** eliminates these performance overheads by tricking the compiler into generating the same instructions that would be generated for normal floating point code, while also preventing non-deterministic compiler optimizations.

## Design

The key insight behind this library comes from [Sherry Ignatchenko's talk](https://github.com/CppCon/CppCon2024/blob/main/Presentations/Cross-Platform_Floating-Point_Determinism_Out_of_the_Box.pdf) on floating point determinism, which observed that C++ can be made deterministic if we can ensure
sequencing between subsequent expressions with semicolons ';'. In practice, Clang and GCC
may optimize across lines, for example converting:
```
float foo = a * b;
float bar = foo + c;
```


into something equivalent to:
`float bar = fma(a, b, c);`

This will result in a more precise result that is ultimately non-deterministic because
this optimization not guaranteed for all combinations of compiler flags, source code, and platforms.

**rfloat** prevents Clang and GCC from optimizing between expressions by inserting an empty assembly block between subsequent expressions that forces the compiler to spill intermediate results into registers. On MSVC, /fp:fast is simply disabled for the wrapper class. This results in an additional call per operation when using reproducible types on MSVC if /fp:fast is enabled.

> [!NOTE]
> MSVC overhead is not present when using the default setting of /fp:precise.

## Usage

Use **rfloat** by including the `<rfloat>` header and replacing usages
of `float` & `double` with `rfloat` & `rdouble`. The library has been tested with C++17 through C++23 compilers.

```
#include <rfloat>
rfloat sum(rfloat a, rfloat b) {
    return a + b;
}
```

For convenience, you may mix regular floating point types and reproducible types. The result will be a reproducible type.

```
float a;
rfloat b;

auto c = a + b;
static_assert(std::is_same<decltype(c), rfloat>::value);
```

Reproducible types prohibit usage with types of different sizes, as well as types
with different rounding modes if the results would be ambiguous.

```
rfloat a;
rdouble b;
auto c = a + b; // compile error
```

Expressions that compile with reproducible types should return the same results under any combination of compiler flags. There should be little to no performance cost beyond the operations themselves.

Reproducible types also allow unwrapping values to interact with existing code.

```
rfloat a;
float b = a.underlying_value();

rfloat c;
rdouble d = c.fp64(); // Casts are allowed as long as they don't lose precision

rdouble e;
rfloat f = e.fp32(); // Compile error to prevent accidental narrowing
float f = e.underlying_value(); // Escape hatches exist
```

Users who need to specific rounding modes should call `rstd::SetRoundingMode<T>()` manually to ensure the environment is initialized to the correct rounding mode.

> [!NOTE]
> Ensuring the environment has the correct rounding mode at any given time is left up to the user.

`<stdfloat>` is supported by defining the `ENABLE_STDFLOAT` macro.

**rfloat** also provides overloads for all of the `<cmath>` functions. Only deterministic overloads are enabled by default. Non-deterministic overloads can be enabled by defining `RSTD_NONDETERMINISM`.

```
rdouble loan_cost(rdouble principal, rdouble interest_rate) {
    constexpr rdouble term = 5;
    rdouble rate = annual_rate / 100.0;
    return principal * pow(1 + rate, term);
}
```

Overloads are only as deterministic as the underlying standard library. Users who need guaranteed determinism should evaluate dedicated implementations like [dmath](https://github.com/sixitbb/sixit-dmath), [crlibm](https://github.com/taschini/crlibm), and [rlibm](https://github.com/rutgers-apl/rlibm).


> [!NOTE]
> If you want to evaluate standard library determinism on your platform, the reproducibility tests can check them by defining `RSTD_DETERMINISM` and `ENABLE_NONDETERMINISTIC_TESTS` when
> building the reproducibility tests, or by using `--preset debug` during CMake configuration with Clang or GCC.

## Platforms
### Tested Platforms
| Support | Windows x64 | MacOS M1 | Linux x64 |
|---------|---------|-------|-------|
| Clang 14/15/16 | Untested | :heavy_check_mark: | :heavy_check_mark: |
| GCC 11   | Untested | Untested | :heavy_check_mark: |
| MSVC     | :heavy_check_mark: | Untested | Untested |


## Goals
**rfloat** aims to be
- Easy to integrate with existing code. Converting most code involves adding a single 'r' prefix
- Support the full range of standard library functionality, including `<cmath>` and `std::numeric_limits`
- Add zero unnecessary overhead
- Be deterministic by default. If it compiles, it should be safe unless the user explicitly opts out
- Be deterministic in all configurations. **rfloat** safely supports dangerous compiler flags like `-ffast-math` and `-funsafe-math-optimizations`, as well as LTO
- Support modern, IEEE-754 compliant architectures. Supporting platforms without IEEE-754 floats is explicitly a non-goal

Other libraries like [streflop](https://github.com/abma/streflop) are sensitive to compiler flags or impose additional overheads.

## Limitations

- This library prohibits some compiler optimizations on code using reproducible types. If you write a loop adding `tmp += a;` 10 times, you'll get 10 separate additions at runtime.
- Ensuring the environment has the correct rounding mode is left up to the user.

## Issues
- MSVC with /fp:fast results in additional overhead because the compiler is forced to
convert every operation into a function call. Suggestions for improvement are welcome.
- Clang produces unnecessary moves on x64.
- Due to GCC Issue #71246, there may be issues with certain combinations of compiler flags and platforms that have not been detected by testing.

Platform non-determinism and reproducibility issues are considered bugs. Please report them.

## Benchmarks

A whetstone benchmark is provided as a basic example and can be built by enabling the `RFLOAT_BENCHMARKS` option in CMake.

> [!NOTE]
> **rfloat** is inherently sensitive to source code, toolchain and platform support for performance.
> Measurements are indicative only, and may not be valid on your source code, with your toolchain,
> on your hardware.

### Ansibench whetstone on x64:

No performance differential observed on `-O2` at 1000000 iterations.

| Compiler | double | rdouble |
|----------|--------|---------|
| Clang 16 | 5882.4 MWIPS | 5882.4 MWIPS |
| GCC 11   | 5882.4 MWIPS | 5882.4 MWIPS |

6% performance differential observed on `-O3 -ffast-math -funsafe-math-optimizations` at 1000000 iterations. Note that the performance of **rfloat** has not decreased, but additional optimizations
have benefited the baseline implementation. No attempt has been made to ensure that the results computed by the baseline implementation are correct.

| Compiler | double | rdouble |
|----------|--------|---------|
| Clang 16 | 6250 MWIPS | 5882.4 MWIPS |
| GCC 11   | 6250 MWIPS | 5882.4 MWIPS |

### Ansibench linpack on x64:
All numbers in GFLOPS, rounded to 2 decimal digits.

#### GCC-11 -O2
| Size | double | rdouble | Slowdown relative to double |
|----------|--------|---------|----|
| 32k | 5.48 | 5.47 | 0.18% |
| 64k | 5.47 | 5.48 | -0.10% |
| 128k | 5.48 | 5.52 | -0.64% |
| 256k | 5.47 | 5.51 | -0.66% |

#### Clang-16 -O2

| Size | double | rdouble | Slowdown relative to double |
|----------|--------|---------|----|
| 32k | 7.69  | 4.25 | 44.77% |
| 64k | 7.71 | 4.25 | 44.82% |
| 128k | 7.68 | 4.24| 44.81% |
| 256k | 7.70 | 4.24 | 44.91% |

The clang slowdown results almost entirely from Clang emitting unnecessary memory stores
after every floating point operation. Eliminating these stores closes the performance gap, but also emits non-deterministic code.

## License

This project is licensed under the [MIT License](LICENSE).

## Credit

This library inspired by Guy Davidson's [P3375R0](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p3375r0.html) proposal and Sherry Ignatchenko's talk on[`Cross-Platform Floating-Point Determinism Out of the Box`](https://github.com/CppCon/CppCon2024/blob/main/Presentations/Cross-Platform_Floating-Point_Determinism_Out_of_the_Box.pdf).



