// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>

export module purple.arithmetic:tri.array;

import :tri;

using std::array;

// Tri-degree arithmetic with arrays.

export namespace purple
{
    /// Quotient and ratio by "normalised" operands.
    template <typename Integer, size_t BiDegree, size_t TriDegree>
    requires requires { BiDegree == 2uz; TriDegree == 3uz; }
    auto ratio_normalised ( array<Integer,TriDegree> const & x, array<Integer,BiDegree> const & y, Integer iy )
    {
        return ratio_normalised( span(x), span(y), iy );
    }
}
