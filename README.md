# **rfloat**

**rfloat** is a header-only library that makes your floating point code reproducible with zero overhead.

## Table of Contents
- [Features](#features)
- [Quick Start](#quick-start)
- [Why is Reproducibility Important](#why-is-reproducibility-important)
- [Examples](#examples)
- [Design](#design)
- [Supported Platforms](#supported-platforms)
- [Goals](#goals)
- [Limitations](#limitations)
- [Issues](#issues)
- [Benchmarks](#benchmarks)
- [Reproducibility Guarantees](#reproducibility-guarantees)
- [License](#license)
- [Credit](#credit)
- [Additional Resources](#additional-resources)

## Features

- Zero unnecessary overhead
- Header-only
- Convert existing code with an `r` prefix
- Supports all major compilers
- Supports all major platforms
- Zero dependencies
- Full <cmath> support
- Compatible with C++17 and newer

## Quick Start

1. Include the `<rfloat>` header.

2. Replace instances of `float` & `double` with `rfloat` & `rdouble`.

3. **Optional** Replace `<cmath>` includes with `<rcmath>`.

4. **Optional** Replace `cmath` functions with their `rstd` equivalents.

    `std::sqrt` → `rstd::sqrt`

## Why is Reproducibility Important?

IEEE-754 floating point arithmetic is reproducible under certain conditions. However, floating point code is rarely reproducible in real programs due to:

- compiler flags & optimizations
- non-compliant hardware
- non-compliant implementations
- ambiguous standard semantics

Floating point non-reproducibility creates problems for many applications, especially games, robotics, and high-assurance code. Other libraries like [dmath](https://github.com/sixitbb/sixit-dmath) implement reproducible floating point through a variety of strategies, each of which has its own significant tradeoffs.

**rfloat** uses a different approach focused on preventing the compiler from performing dangerous optimizations while still allowing it to generate the same instructions it otherwise would. This provides practical reproducibility to applications without code
changes.

> [!NOTE]
> It's impossible for any hard float library, rfloat included, to guarantee complete reproducibility. Users that need such guarantees should consider soft float alternatives like [Berkeley SoftFloat](http://www.jhauser.us/arithmetic/SoftFloat.html) and [GNU MPFR](https://www.mpfr.org/). See the section on [Reproducibility Guarantees](#reproducibility-guarantees) on why this is difficult. Limitations with the rfloat approach are documented below in [Issues](#issues) and [Limitations](#limitations).

## Examples

```cpp
#include <rfloat>
rfloat sum(rfloat a, rfloat b) {
    return a + b;
}
```

You can mix regular floating point types and reproducible types. The result will be a reproducible type. The `rfloat` and `rdouble` types are provided in the global namespace for ease of conversion.

```cpp
float a;
rfloat b;

auto c = a + b;
static_assert(std::is_same<decltype(c), rfloat>::value);
```

Reproducible types prohibit usage with types of differing sizes, as well as types
with different rounding modes if the results would be ambiguous.

```cpp
rfloat a;
rdouble b;

rfloat c = a + b; // compile error
```

Reproducible types also allow unwrapping values to interact with existing code.

```cpp
rfloat a;
float b = a.underlying_value();

rfloat c;
rdouble d = c.fp64(); // Casts are allowed as long as they don't lose precision

rdouble e;
rfloat x = e.fp32(); // Compile error to prevent accidental narrowing
float y = e.underlying_value(); // Escape hatch returning the raw floating point type
```

Expressions that compile with reproducible types should return the same results under any combination of compiler flags. There should be little to no performance cost beyond the operations themselves.

Code requiring specific rounding modes should call `rstd::SetRoundingMode<T>()` manually to ensure the environment is initialized to the correct rounding mode. Refer to compiler documentation for how this interacts with the runtime environment.

> [!NOTE]
> Ensuring the environment has the correct rounding mode at runtime is left to the user. This is a rare requirement and can introduce significant overhead.

`<stdfloat>` is supported by defining the `ENABLE_STDFLOAT` macro.

**rfloat** also provides overloads for all of the `<cmath>` functions. Only reproducible overloads are enabled by default. This encompasses the `abs`, `fma`, `sqrt()` and other basic operations on most platforms. Certain platforms do not implement all operations in a reproducible way. When this occurs, the affected functions can be enabled by defining `RSTD_NONDETERMINISM`.

```
rdouble loan_cost(rdouble principal, rdouble interest_rate) {
    constexpr rdouble term = 5;
    rdouble rate = annual_rate / 100.0;
    return principal * pow(1 + rate, term);
}
```

Overloads are only as reproducible as the underlying standard library implementation. Users who need guaranteed reproducibility should evaluate dedicated soft float implementations like [Berkeley SoftFloat](http://www.jhauser.us/arithmetic/SoftFloat.html) and [GNU MPFR](https://www.mpfr.org/), or other libraries with custom implementations like [dmath](https://github.com/sixitbb/sixit-dmath), [crlibm](https://github.com/taschini/crlibm), and [rlibm](https://github.com/rutgers-apl/rlibm). LLVM's standard library functions are reproducible for most common functions, and plans to increase reproducibility over time. The current status is available in the [LLVM math.h Implementation Status](https://libc.llvm.org/headers/math/index.html#basic-operations).

> [!NOTE]
> If you want to evaluate standard library reproducibility on your platform, the reproducibility tests can check them by defining `RSTD_DETERMINISM` and `ENABLE_NONDETERMINISTIC_TESTS` when
> building the reproducibility tests, or by using `--preset debug` during CMake configuration with Clang or GCC.

Platform reproducibility issues are documented in the [Issues](#issues) section.

## Design

Inspiration for this library comes from [Sherry Ignatchenko's talk](https://github.com/CppCon/CppCon2024/blob/main/Presentations/Cross-Platform_Floating-Point_Determinism_Out_of_the_Box.pdf) on floating point reproducibility, which observed that C++ can be made practically reproducible if we can ensure sequencing between subsequent expressions with semicolons ';'. In practice, Clang and GCC may optimize across lines, for example converting:

```
float foo = a * b;
float bar = foo + c;
```

Compilers will sometimes optimize this into
`float bar = fma(a, b, c);`

This implementation is faster and has less rounding error. Unfortunately, compilers don't apply this transformation consistently in all circumstances. That means the same code built in different translation units, built by different compilers, or even built by the same compiler for another platform may produce a different result for the same inputs.

**rfloat** prevents compilers automatically applying these kinds of transformations, while still allowing programmers to manually specify them. Since these transformations  usually don't impact performance significantly, the resulting program becomes reproducible for negligible cost in most cases. The library does this with two strategies:

The default strategy inserts a compiler intrinsic between potentially problematic expressions. This prevents some codegen options that might harm reproducibility like fma contraction without additional side effects. This method has negligible overhead at both compile time and runtime.

The second strategy uses inline assembly to prevent the compiler from reordering or fusing operations that could lead to reproducibility issues by forcing the compiler to spill intermediate results into registers. 

MSVC does not support inline assembly blocks or optimization barriers. Instead, `/fp:contract` is simply disabled for the implementation class. This has no overhead in most cases, but does produce in an additional call per operation when mixing reproducible types with non-reproducible types inside a single translation unit. Code that does not mix non-reproducible types does not incur additional overhead.

If `-ffast-math` is set, **rfloat** has to fallback to the inline assembly strategy discussed above to ensure reproducibility.

> [!NOTE]
> **rfloat** may prevent some compiler transformations required for autovectorization. This has not been observed to significantly impact performance in practice, and effort has been made to allow loop unrolling for autovectorization.

### Configuration Macros

| Macro name | Default | Purpose |
|------------|---------|---------|
| ENABLE_STDFLOAT | Off | Enables `<stdfloat>` support |
| RSTD_NONDETERMINISM | Off | Enables math functions that are known to be nondeterministic on some platforms |

## Supported Platforms

| Compiler | OS | Architecture | Tested |
|---------|---------|-------|-------|
| Clang 14+ | MacOS | AARCH64 | :heavy_check_mark: |
| Clang 14+ | Linux | x64 | :heavy_check_mark: |
| GCC 11+ | Linux | x64 | :heavy_check_mark: |
| MSVC | Windows | x64 | :heavy_check_mark: |

The following platforms are all continuously tested via QEMU.

| Support | GCC | Clang |
|---------|---------|---------|
| arm32 | :heavy_check_mark: | :heavy_check_mark: |
| aarch64 | :heavy_check_mark: | :heavy_check_mark: |
| ppc64el | :heavy_check_mark: | :heavy_check_mark:\* |
| s390x | :heavy_check_mark: | :heavy_check_mark: |
| mips64el | :heavy_check_mark: | :heavy_check_mark: |

> [!NOTE]
> Platform combinations with an asterisk have [workarounds](#issues) to ensure reproducibility by default.

## Goals
**rfloat** aims to demonstrate that complete reproducibility is achievable today without severe performance degradations or code modifications.

- Zero unnecessary overhead
- Easy to integrate with existing code
    - Converting most code involves adding a single 'r' prefix
- Supports the full range of standard library functionality, including `<cmath>` and `std::numeric_limits`
- Reproducible by default
    - If it compiles, it is reproducible unless the user explicitly opts out
- Safely supports dangerous compiler flags like `-ffast-math` and `-funsafe-math-optimizations`
- Supports all modern, IEEE-754 compliant architectures.


> [!NOTE]
> Although supporting non-IEEE-754 platforms and runtimes is explicitly not a goal, using **rfloat** will improve reproducibility even on platforms that are not fully compliant.

## Limitations

- **rfloat** prohibits some compiler optimizations on code using reproducible types. Code incrementing a variable `tmp += a;` 10 times will result in 10 floating point additions at runtime.
- Ensuring the environment has the correct rounding mode is left up to the user
- **rfloat** inherently cannot prevent all possible instances of compiler non-reproducibility.
- **rfloat** does not eliminate reproducibility issues caused by buggy or incomplete hardware implementations
- **rfloat** is only as reproducible as the inputs provided
    - The user is responsible for ensuring the same inputs are passed to the same operations in the same order
    - Float serialization can lead to reproducibility issues

## Issues

- MSVC with `/fp:contract` results in additional overhead when reproducible and non-reproducible types are mixed inside a single translation unit because the compiler is forced to convert every operation into a function call. Suggestions for improvement are welcome.
- Due to [GCC Issue #71246](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=71246), rfloat may have issues with certain combinations of compiler flags and platforms that have not been detected despite extensive testing.
- Test code uses infinities and NaNs even under -ffast-math, causing potential undefined behavior and generating diagnostic warnings on Clang-18+.
    - **rfloat** works correctly even in this scenario despite the warnings. These warnings do not affect the library itself.

There are no known correctness issues with **rfloat**. All reproducibility issues are considered bugs, please report them. Future compiler versions that break these guarantees will also be considered bugs.

The following list documents issues in related systems that may impact reproducibility:

- LLVM does not propagate NaN payloads according to IEEE754.
    - This is intentional and reasonable behavior documented by the LLVM project
- Clang targeting PPC64el with `-ffast-math` generates a non-IEEE754 compliant `sqrt()` when inlined into unrolled loops. See [`ppc64el.SqrtRoundingBug`](src/compiler_bug_tests.cpp) test case for details.
    - **rfloat** disables inlining for `std::sqrt` overloads for ppc64.

> [!NOTE]
> Floating point code aiming for reproducibility should typically avoid NaNs regardless of documented compiler issues

## Benchmarks

A whetstone benchmark is provided as a basic example and can be built by enabling the `RFLOAT_BENCHMARKS` option in CMake. "Comparable opts" tests listed below exclude the two optimizations that are prohibited by rfloat, `-fno-associative-math` and `-ffp-contract=off`. Tests results are reported after initial run warmups.

> [!NOTE]
> **rfloat** is inherently sensitive to source code, toolchain and platform support for performance.
> Measurements are indicative only, and may not be valid on your source code, with your toolchain,
> on your hardware.

### Ansibench whetstone on x64:

No performance differential observed on `-O2` at 1000000 iterations. 

| Compiler | double | double comparable ops | rdouble |
|----------|--------|---------|---------|
| Clang 18 | 5882.4 MWIPS | 5882.4 MWIPS | 5882.4 MWIPS |
| GCC 13 | 5882.4 MWIPS | 5882.4 MWIPS | 5882.4 MWIPS |

### Ansibench linpack on x64:

All numbers in GFLOPS, rounded to 2 decimal digits. Note that linpack functions as a worst-case impact benchmark and depends on tight loop whose performance is heavily affected by contraction and reassociation optimizations.

#### GCC-13 -O2
| Size | double | double comparable opts | rdouble | Slowdown relative to double | comparable ops |
|----------|--------|---------|----|----|----|
| 32k | 7.80 | 5.52 | 6.29 | 19.4% | -13.9% |
| 64k | 7.70 | 5.52 | 6.27 | 18.6% | -13.5% |
| 128k | 7.66 | 5.48 | 6.20 | 19.1% | -13.1% |
| 256k | 7.74 | 5.48 | 6.30 | 18.6% | -15.0% |

#### Clang-18 -O2

| Size | double | double comparable opts | rdouble | Slowdown relative to double | comparable opts |
|----------|--------|---------|----|----|----|
| 32k | 7.89 | 7.03 | 7.02 | 11.0% | 1.42% |
| 64k | 7.89 | 7.01 | 6.99 | 11.4% | 0.28% |
| 128k | 7.77 | 6.97 | 7.04 | 9.40% | -1.00% |
| 256k | 7.81 | 7.00 | 7.04 | 9.86% | -0.57% |

Clang produces essentially identical code for rdouble and double with comparable options. I'm not sure why the comparable numbers for g++-13 are lower than rdouble, but my speculation is that it's not unrolling the inner loops.

## Reproducibility Guarantees

Floating point reproducibility is extremely challenging to guarantee because it requires many different layers to work together.

- The IEEE-754 standard must specify a required behavior
- The floating point hardware must perfectly implement that behavior
- The compiler must provide a runtime that uses the hardware properly
- The compiler must not break the floating point semantics of your code when translating it to an executable

All of these layers have reproducibility issues. At the lowest level, the IEEE-754 standard does not specify required behaviors in all situations. For example:
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

This library inspired by Guy Davidson's [P3375R0](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p3375r0.html) proposal and Sherry Ignatchenko's talk on [`Cross-Platform Floating-Point Determinism Out of the Box`](https://github.com/CppCon/CppCon2024/blob/main/Presentations/Cross-Platform_Floating-Point_Determinism_Out_of_the_Box.pdf).

## Additional Resources

[Agner Fog on NaN Payload Propagation](https://grouper.ieee.org/groups/msc/ANSI_IEEE-Std-754-2019/background/nan-propagation.pdf)

