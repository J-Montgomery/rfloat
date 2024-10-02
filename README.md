# **rfloat**

**rfloat** is an experimental library implementing reproducible floating point types
on existing compilers at minimal performance cost.

## Table of Contents
- [Overview](#overview)
- [Usage](#usage)
- [Limitations](#limitations)
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
this optimization not guaranteed for all combinations of compiler flags, source code, and platforms. MSVC does not appear to optimize between expressions in testing.

**rfloat** prevents Clang and GCC from optimizing between expressions by inserting an empty assembly block between subsequent expressions that forces the compiler to spill intermediate results into registers: `__asm__ volatile("" ::"X"(param) :)`

## Usage

Use **rfloat** by including the header and replacing usages
of `float` & `double` with `rfloat` & `rdouble`. The library has been tested with C++17 through C++23 compilers.

```
#include <rfloat/rfloat.hh>
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

Users who need to specific rounding modes should call `rmath::SetRoundingMode<T>()` manually to ensure the environment is initialized to the correct rounding mode. Ensuring the environment has the correct rounding mode at any given time is left up to the user.

## Limitations

- `<cmath>` is not supported. The `<rfloat/rcmath.hh>` header is provided with overloads
for a safe subset of cmath. If this isn't sufficient, [dmath](https://github.com/sixitbb/sixit-dmath), [crlibm](https://github.com/taschini/crlibm), and [rlibm](https://github.com/rutgers-apl/rlibm) are recommended.
- Unwrapping a reproducible type with `.fp64()` and related functions will allow you to
write non-deterministic code with the result. Use these functions with caution.
- MSVC has been observed adding a small number of unnecessary `MOV` instructions in rfloat-using code.
- Clang occasionally produces suboptimal register allocations.
- This library prohibits many meaningful compiler optimizations on your code. If you write a loop adding `tmp += a;` 10 times, you'll get 10 separate additions at runtime. Be aware of what your code does.
- Ensuring the environment has the correct rounding mode is left up to the user.

Platform non-determinism and reproducibility issues are considered bugs. Please report them.

## License

This project is licensed under the [MIT License](LICENSE).

## Credit

This library inspired by Guy Davidson's [P3375R0](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p3375r0.html) proposal and Sherry Ignatchenko's talk on[`Cross-Platform Floating-Point Determinism Out of the Box`](https://github.com/CppCon/CppCon2024/blob/main/Presentations/Cross-Platform_Floating-Point_Determinism_Out_of_the_Box.pdf).



