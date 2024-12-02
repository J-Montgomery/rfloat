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
- [Reproducibility](#reproducibility)
- [License](#license)
- [Credit](#credit)

## Overview

The IEEE-754 standard that defines floating point arithmetic is reproducible under certain conditions. However, floating point code is rarely reproducible  in the real world due to:

- compiler flags & optimizations
- non-compliant hardware
- non-compliant implementations
- ambiguous standard semantics

The lack of floating point reproducibility creates problems for many applications, especially simulations and high-assurance code. Other libraries [dmath](https://github.com/sixitbb/sixit-dmath) implement reproducible floating point through a variety of strategies, which each have their own significant tradeoffs and performance implications.

**rfloat** is a different approach that eliminates these performance overheads and provides practical reproducibility. It accomplishes this by tricking the compiler into generating the same instructions that would be generated for normal floating point code, while also preventing most non-reproducible compiler optimizations. While this does not address all of the potential sources of non-reproducibility, this is an ideal tradeoff for almost all real-world applications.

Users that need to guarantee complete reproducibility are encouraged to look at soft float alternatives like [Berkeley SoftFloat](http://www.jhauser.us/arithmetic/SoftFloat.html) and [GNU MPFR](https://www.mpfr.org/). It is not possible for **rfloat** or any other similar library to guarantee complete reproducibility. See the [Issues](#issues) and [Reproducibility](#reproducibility) sections for further details.

## Design

The key insight behind this library comes from [Sherry Ignatchenko's talk](https://github.com/CppCon/CppCon2024/blob/main/Presentations/Cross-Platform_Floating-Point_Determinism_Out_of_the_Box.pdf) on floating point reproducibility, which observed that C++ can be made practically reproducible if we can ensure
sequencing between subsequent expressions with semicolons ';'. In practice, Clang and GCC
may optimize across lines, for example converting:
```
float foo = a * b;
float bar = foo + c;
```

into something equivalent to:
`float bar = fma(a, b, c);`

This will result in a more precise result that is ultimately non-reproducible because
this optimization not guaranteed for all combinations of compiler flags, source code, and platforms.

**rfloat** prevents Clang and GCC from optimizing between expressions by inserting an empty assembly block between subsequent expressions that forces the compiler to spill intermediate results into registers. On MSVC, `/fp:fast` is simply disabled for the wrapper class. This results in an additional call per operation when using reproducible types on MSVC if `/fp:fast` is enabled.

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

**rfloat** also provides overloads for all of the `<cmath>` functions. Only reproducible overloads are enabled by default. This encompasses the `abs`, `fma`, `sqrt()` and other basic operations on most platforms. Certain platforms do not implement all operations in a reproducible way. When this occurs, the affected functions can be enabled by defining `RSTD_NONDETERMINISM`. 

```
rdouble loan_cost(rdouble principal, rdouble interest_rate) {
    constexpr rdouble term = 5;
    rdouble rate = annual_rate / 100.0;
    return principal * pow(1 + rate, term);
}
```

Overloads are only as reproducible as the underlying standard library implementation. Users who need guaranteed reproducibility should evaluate dedicated implementations like [dmath](https://github.com/sixitbb/sixit-dmath), [crlibm](https://github.com/taschini/crlibm), and [rlibm](https://github.com/rutgers-apl/rlibm).


> [!NOTE]
> If you want to evaluate standard library reproducibility on your platform, the reproducibility tests can check them by defining `RSTD_DETERMINISM` and `ENABLE_NONDETERMINISTIC_TESTS` when
> building the reproducibility tests, or by using `--preset debug` during CMake configuration with Clang or GCC.

All known platform reproducibility issues are documented in the [Issues](#issues) section.

## Platforms
### Tested Platforms
| Support | Windows x64 | MacOS M1 | Linux x64 |
|---------|---------|-------|-------|
| Clang 14/15/16 | Untested | :heavy_check_mark: | :heavy_check_mark: |
| GCC 11   | Untested | Untested | :heavy_check_mark: |
| MSVC     | :heavy_check_mark: | Untested | Untested |

The following platforms are all continuously tested via QEMU.

| Support | GCC |
|---------|---------|
| arm32 | :heavy_check_mark: |
| aarch64 | :heavy_check_mark: |
| ppc64el | :heavy_check_mark: |
| s390x | :heavy_check_mark: |
| mips64el | :heavy_check_mark: |

## Goals
**rfloat** aims to provide the best tradeoff between performance, reproducibility, and ease of use available to typical programs on existing compilers.

- Zero unnecessary overhead
- Easy to integrate with existing code
    - Converting most code involves adding a single 'r' prefix
- Supports the full range of standard library functionality, including `<cmath>` and `std::numeric_limits`
- Reproducible by default
    - If it compiles, it should be safe unless the user explicitly opts out
- Safely supports dangerous compiler flags like `-ffast-math` and `-funsafe-math-optimizations`
- Support all modern, IEEE-754 compliant architectures.
    - Supporting platforms and runtimes without IEEE-754 operations is explicitly a non-goal

Other libraries like [streflop](https://github.com/abma/streflop) are sensitive to compiler flags or impose additional overheads.

## Limitations

- **rfloat** prohibits some compiler optimizations on code using reproducible types. If you write a loop adding `tmp += a;` 10 times, you'll get 10 separate additions at runtime
- Ensuring the environment has the correct rounding mode is left up to the user
- **rfloat** inherently cannot prevent all possible instances of compiler non-reproducibility.
- **rfloat** does not eliminate reproducibility issues caused by buggy or incomplete hardware implementations
- **rfloat** is only as reproducible as the inputs provided
    - The user is responsible for ensuring the same inputs are passed to the same operations in the same order
    - Float serialization can lead to reproducibility issues

## Issues
- MSVC with `/fp:fast` results in additional overhead because the compiler is forced to
convert every operation into a function call. Suggestions for improvement are welcome.
- Clang produces unnecessary moves on x64.
- Due to GCC Issue #71246, there may be issues with certain combinations of compiler flags and platforms that have not been detected by testing despite extensive search.

Unidentified reproducibility issues are considered bugs. Please report them.

The following reproducibility issues are known:
    - LLVM does not propagate NaN payloads according to IEEE754.
    - Clang targeting PPC64el with `-ffast-math` generates non-IEEE754 compliant `sqrt()` implementation. See [`ppc64el.SqrtRoundingBug`](src/compiler_bug_tests.cpp) test case for details.

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

The Clang slowdown results from Clang emitting unnecessary memory stores
after every floating point operation and entirely eliding some functions in the benchmark. This is not a result representative of typical overheads, but is included to illustrate what may occur.

## Reproducibility

Floating point reproducibility is extremely challenging to guarantee because it requires many different layers to work together.

- The IEEE-754 standard must specify a required behavior
- The floating point hardware must perfectly implement that behavior
- The compiler must provide a runtime that uses the hardware properly
- The compiler must not break the floating point semantics of your code when translating it to an executable

All of these layers have reproducibility issues. At the lowest level, The IEEE-754 standard does not specify required behaviors in all situations. For example:
- operations taking multiple inputs do not have a specified result with multiple NaNs
- bounds for transcendental math functions are difficult to specify as a result of the [Tablemaker's Dilemma](https://perso.ens-lyon.fr/jean-michel.muller/Intro-to-TMD.htm)

This leads to implementations choosing their own behaviors or worse, providing indeterminate results. Hardware vendors also do not perfectly implement the parts of IEEE-754 which are well specified for reasons of efficiency and cost. For example:
- ARMv7 FPUs do not support subnormal numbers for performance reasons
- x86 SSE does not implement the `maxss` and `minss` instructions (max and min functions respectively) according to IEEE-754 semantics
    - IEEE-754 requires returning the non-NaN argument if one argument is NaN
    - x86 returns the second argument unconditionally. This allows compilers to implement ternary operators like the `MAX` and `MIN` macros in a single instruction.

Occasionally, compiler runtimes also do not implement the IEEE-754 standard correctly. **rfloat** does not define `rstd::sqrt` when built with clang for the PPC64el target because the optimizer incorrectly optimizes the runtime implementation in certain situations. These situations are rare and generally not encountered in real programs, although **rfloat** maintains [compiler bug tests](src/compiler_bug_tests.cpp) to document discovered examples.

Compilers sometimes simply fail to maintain IEEE-754 semantics while translating code into executables. This can have significant optimization benefits, but in many cases breakages are simply not noticeable for typical applications. For example:
- LLVM does not correctly propagate NaN values between operations even in `strictfp` mode. LLVM also does not commit to a single strategy either, choosing between a number of NaN strategies depending on the situation.

A more common reproducibility issue is an intentional breakage that often benefits application code, floating point contraction. Compilers are not able to guarantee that all possible contractions are optimized across all possible targets, which means that different programs accumulate rounding errors slightly differently when built for new targets or even the same target with a new compiler version. This behavior can usually be disabled with appropriate compiler flags, but disabling the behavior at a more fine-grained level or tracking down where all the differences have occurred is not generally possible.

Many compilers also implement configuration flags that explicitly allow them to break IEEE-754 semantics when translating code. These flags are often specified at the level of an entire translation unit or even an entire project, making it difficult to build applications where only part of the code must ensure reproducibility.

Compiler breakages introduce the vast majority of floating point reproducibility issues. This is also the layer that **rfloat** targets. By resolving these breakages, most programs that anyone would intentionally write can be made reproducible without additional overhead, or significant changes.

## License

This project is licensed under the [MIT License](LICENSE).

## Credit

This library inspired by Guy Davidson's [P3375R0](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p3375r0.html) proposal and Sherry Ignatchenko's talk on[`Cross-Platform Floating-Point Determinism Out of the Box`](https://github.com/CppCon/CppCon2024/blob/main/Presentations/Cross-Platform_Floating-Point_Determinism_Out_of_the_Box.pdf).

## Additional Resources

[Agner Fog on NaN Payload Propagation](https://grouper.ieee.org/groups/msc/ANSI_IEEE-Std-754-2019/background/nan-propagation.pdf)


