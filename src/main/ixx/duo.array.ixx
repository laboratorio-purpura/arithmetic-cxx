// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>

export module purple.arithmetic:duo.array;

import :duo;

using std::array;

// Duo-degree arithmetic.

export namespace purple
{
    /// Properties.

    /// 1 if and only if x is smaller than y, else 0.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto is_smaller ( array<Integer,Degree> x, array<Integer,Degree> y )
    {
        return is_smaller( span(x), span(y) );
    }

    /// 1 if and only if x is *not* smaller than y, else 0.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto not_smaller ( array<Integer,Degree> x, array<Integer,Degree> y )
    {
        return not_smaller( span(x), span(y) );
    }

    /// Operators.

    // Accumulate sum, return carry.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto sum_accumulate ( array<Integer,Degree> & x, Integer y )
    {
        return sum_accumulate( span(x), y );
    }

    // Accumulate sum, return carry.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto sum_accumulate ( array<Integer,Degree> & x, array<Integer,Degree> const & y )
    {
        return sum_accumulate( span(x), span(y) );
    }

    // Accumulate difference, return carry.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto difference_accumulate ( array<Integer,Degree> & x, Integer y )
    {
        return difference_accumulate( span(x), y );
    }

    // Accumulate difference, return carry.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto difference_accumulate ( array<Integer,Degree> & x, array<Integer,Degree> const & y )
    {
        return difference_accumulate( span(x), span(y) );
    }

    // Accumulate twice N times, return carry.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto twice_accumulate ( array<Integer,Degree> & x, size_t N )
    {
        return twice_accumulate( span(x), N );
    }

    // Accumulate half N times, rounded down.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto half_accumulate ( array<Integer,Degree> & r, size_t N )
    {
        return half_accumulate( span(r), N );
    }

    /// Quotient and remainder with "normalised" operands.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto ratio_normalised ( array<Integer,Degree> const & x, Integer y, Integer iy )
    {
        return ratio_normalised( span(x), y, iy );
    }

    /// Quotient and remainder.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto ratio ( array<Integer,Degree> const & x, Integer y )
    {
        return ratio( span(x), y );
    }

    // Approximate inverse of "normalised" integer.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto inverse_normalised ( array<Integer,Degree> const & y )
    {
        return inverse_normalised( span(y) );
    }
}
