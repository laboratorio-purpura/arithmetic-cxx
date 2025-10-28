// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <cassert>
#include <span>
#include <tuple>
#include <vector>

export module purple.arithmetic.alternative:poly;

import purple.arithmetic;

import :duo;

using std::array;
using std::ignore;
using std::size_t;
using std::span;
using std::tie;
using std::vector;

/// Multi-degree nonnegative integer arithmetics.
///
/// Let a *word* be a nonnegative integer in base B.
/// In a binary machine, B = 2 ^ bits.
///
/// By multi-degree integers we mean integers represented with variable quantity of words.
///
/// This module partition defines procedures with multi-degree integer operands.
///
/// Unless otherwise specified, requires:
/// degree(operand) ≥ 1

export namespace purple
{
    /// Expansion procedures.

    /// Product with excess.
    ///
    /// Computes by definition: successive addition.

    template <typename Integer>
    void product_v0 ( span<Integer> r, span<Integer const> x, span<Integer const> y )
    // requires degree(v) ≥ degree(x) + degree(y) + 1
    {
        auto carry = Integer(0);
        // t ← 0
        auto t = vector(y.begin(),y.end());
        // while t ≠ 0:
        while ( not_zero<unsigned>( t ) ) {
            // x ← x + x
            carry = sum_assign<unsigned>( r, x, carry );
            // t ← t - 1
            ignore = previous_assign<unsigned>( t );
        }
        // terminate:
        // v = v + ( x × y )
    }

    /// Reduction procedures.

    /// Division with remainder.
    ///
    /// Requires:
    /// degree(x) ≥ degree(y)
    /// y is nonzero
    ///
    /// Computes by definition: successive subtraction.

    template <typename Integer>
    void division_v0 ( span<Integer> q, span<Integer> r, span<Integer const> x, span<Integer const> y )
    {
        // q ← 0
        for (auto i = 0uz; i != q.size(); ++i) q[i] = 0u;
        // r ← x
        for (auto i = 0uz; i != x.size(); ++i) r[i] = x[i];
        for (auto i = x.size(); i != r.size(); ++i) r[i] = 0u;
        // while r ≥ y:
        while ( not_smaller<Integer>( r, y ) ) {
            // q ← q + 1
            ignore = next_assign<Integer>( q );
            // r ← r - y
            ignore = difference_assign<Integer>( r, y );
        }
        // terminate:
        // q = x ÷ y
        // r = x % y
    }
}