#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include <rcmath>
#include <rfloat>

/* This file holds minimal reproducibility tests for bugs discovered in
 * compilers and platforms */

/* Clang on PPC64 platforms introduces a rounding error when built with
 * -ffast-math. As -ffast-math explicitly breaks the IEEE standard, this isn't
 * strictly a bug in the compiler. However this is the only platform/compiler
 * combination I know of that actually deviates from the standard like this. The
 * bug is extremely difficult to reproduce in practice though. A single sqrt()
 * is not enough to trigger the issue, only a sequence of sqrt() calls like the
 * below test case does. The result is a single digit off in the resulting
 * float, likely due to different rounding errors in the sqrt() implementation
 * when the function size hits some threshold that prevents inlining.
 * When this happens the sqrt() call will return 15515.856114955872 instead of
 * 15515.856114955874. Tested on Clang++ v14.0.0
 */
TEST_CASE("ppc64el.SqrtRoundingBug") {
    rdouble inputs[] = {974348689.58807015, 94139670.81675984,
                        494620722.88245404, 584298408.20084012,
                        687516480.08111632, 240741790.98001355};

    rdouble expected[] = {31214.558936305188, 9702.5600135613604,
                          22240.070208577446, 24172.265268295399,
                          26220.535465186753, 15515.856114955874};

    size_t size = sizeof(inputs) / sizeof(inputs[0]);

    for (size_t i = 0; i < size; i++) {
        rdouble y = rstd::sqrt(inputs[i]);
        CHECK_EQ(y, expected[i]);
    }
}