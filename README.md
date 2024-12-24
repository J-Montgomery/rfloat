# **rfloat**

**rfloat** is a header-only library that makes your floating point code reproducible with zero overhead.

## Table of Contents
- [Features](#features)
- [Quick Start](#quick-start)
- [Why is Reproduccibility Important](#why-is-reproducibility-important)
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
- [Additional Resources](#resources)

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

3. Replace usages of `float` & `double` with `rfloat` & `rdouble`.

4. **Optional** Replace `cmath` functions with their `rstd` equivalents.

    `std::sqrt` → `rstd::sqrt`

## Why is Reproducibility Important?

IEEE-754 floating point arithmetic is reproducible under certain conditions. However, floating point code is rarely reproducible in real programs due to:

- compiler flags & optimizations
- non-compliant hardware
- non-compliant implementations
- ambiguous standard semantics

Floating point non-reproducibility creates problems for many applications, especially games, robotics, and high-assurance code. Other libraries like [dmath](https://github.com/sixitbb/sixit-dmath) implement reproducible floating point through a variety of strategies, each of which have their own significant tradeoffs and performance implications.

**rfloat** uses a different approach focused on preventing the compiler from performing dangerous optimizations while still allowing it to generate the same instructions it otherwise would. This isn't perfect, but it's an excellent tradeoff that provides reproducibility in practice for almost all applications.

> [!NOTE]
> It's impossible for any hard float library to guarantee complete reproducibility. Users that need such guarantees should consider soft float alternatives like [Berkeley SoftFloat](http://www.jhauser.us/arithmetic/SoftFloat.html) and [GNU MPFR](https://www.mpfr.org/). See the section on [Reproducibility Guarantees](#reproducibility-guarantees) on why this is difficult.

## Examples

```
#include <rfloat>
rfloat sum(rfloat a, rfloat b) {
    return a + b;
}
```

You can mix regular floating point types and reproducible types. The result will be a reproducible type.

```
float a;
rfloat b;

auto c = a + b;
static_assert(std::is_same<decltype(c), rfloat>::value);
```

Reproducible types prohibit usage with types of differing sizes, as well as types
with different rounding modes if the results would be ambiguous.

```
rfloat a;
rdouble b;

rfloat c = a + b; // compile error
```

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

Overloads are only as reproducible as the underlying standard library implementation. Users who need guaranteed reproducibility should evaluate dedicated soft float implementations like [Berkeley SoftFloat](http://www.jhauser.us/arithmetic/SoftFloat.html) and [GNU MPFR](https://www.mpfr.org/), or other libraries with custom implementations like [dmath](https://github.com/sixitbb/sixit-dmath), [crlibm](https://github.com/taschini/crlibm), and [rlibm](https://github.com/rutgers-apl/rlibm).

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

This may compile into something equivalent to
`float bar = fma(a, b, c);`

Compilers do this because it produces a result faster with less rounding error. Compilers are imperfect though and don't apply this optimization consistently. The same code built in different translation units, built by different compilers, or even built by the same compiler for another platform may produce a different result for the same inputs.

**rfloat** prevents Clang and GCC from optimizing between expressions by inserting an empty assembly block between subsequent expressions. The assembly block acts as a no-op forcing the compiler to spill intermediate results into registers, which happens anyway. The presence of the assembly block and the "side-effect" of writing into a register prevents the optimizer from applying optimizations that would affect reproducibility.

MSVC does not support inline assembly blocks and instead, `/fp:fast` is simply disabled for the implementation class. This has no overhead in most cases, but does produce in an additional call per operation when using reproducible types mixed with non-reproducible types within translation units where `/fp:fast` is enabled. Code that does not mix non-reproducible types does not have an additional overhead.

## Supported Platforms
| Support | Windows x64 | MacOS M1 | Linux x64 |
|---------|---------|-------|-------|
| Clang 14/15/16/18 | Untested | :heavy_check_mark: | :heavy_check_mark: |
| GCC 11   | Untested | Untested | :heavy_check_mark: |
| MSVC     | :heavy_check_mark: | Untested | Untested |

The following platforms are all continuously tested via QEMU.

| Support | GCC | Clang |
|---------|---------|---------|
| arm32 | :heavy_check_mark: | :heavy_check_mark: |
| aarch64 | :heavy_check_mark: | :heavy_check_mark: |
| ppc64el | :heavy_check_mark: | :heavy_check_mark:\* |
| s390x | :heavy_check_mark: | :heavy_check_mark: |
| mips64el | :heavy_check_mark: | :heavy_check_mark: |

> [!NOTE]
> Platform combinations with an asterisk have [documented issues](#issues)

## Goals
**rfloat** aims to provide the best tradeoff between performance, reproducibility, and ease of use for most applications.

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

Performance issues and uncertainty:

- MSVC with `/fp:fast` results in additional overhead because the compiler is forced to
convert every operation into a function call. Suggestions for improvement are welcome.
- Clang produces unnecessary moves on x64.
- Due to [GCC Issue #71246](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=71246), there may be issues with certain combinations of compiler flags and platforms that have not been detected despite extensive testing.

The following list is all known reproducibility issues:

- LLVM does not propagate NaN payloads according to IEEE754.
    - This is documented, intended behavior by the LLVM project
- Clang targeting PPC64el with `-ffast-math` generates a non-IEEE754 compliant `sqrt()` implementation. See [`ppc64el.SqrtRoundingBug`](src/compiler_bug_tests.cpp) test case for details.
    - **rfloat** disables `std::sqrt` overloads for PPC64el systems by default.

Unidentified reproducibility issues are considered bugs, please report them.

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
have benefited the baseline implementation, at the cost of non-reproducible outputs.

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

The Clang slowdown results from Clang emitting unnecessary memory stores after every floating point operation and entirely eliding some functions in the benchmark. This is not representative of typical overheads, but is included to illustrate what may occur.

## Reproducibility Guarantees

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

This library inspired by Guy Davidson's [P3375R0](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p3375r0.html) proposal and Sherry Ignatchenko's talk on [`Cross-Platform Floating-Point Determinism Out of the Box`](https://github.com/CppCon/CppCon2024/blob/main/Presentations/Cross-Platform_Floating-Point_Determinism_Out_of_the_Box.pdf).

## Resources

[Agner Fog on NaN Payload Propagation](https://grouper.ieee.org/groups/msc/ANSI_IEEE-Std-754-2019/background/nan-propagation.pdf)


