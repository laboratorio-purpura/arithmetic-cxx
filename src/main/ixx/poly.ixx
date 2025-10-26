// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <cassert>
#include <span>
#include <tuple>
#include <vector>

export module purple.arithmetic:poly;

import :mono;
import :duo;

using std::array;
using std::ignore;
using std::size_t;
using std::span;
using std::tie;
using std::vector;

/// Variable degree nonnegative integer arithmetics.
///
/// Let a *word* be a nonnegative integer in base B.
/// In a binary machine, B = 2 ^ bits.
///
/// By variable degree integers we mean integers represented with variable quantity of words.
///
/// This module partition defines procedures with variable degree integer operands.
///
/// Unless otherwise specified, requires:
/// degree(operand) ≥ 1

export namespace purple
{
    /// Representation procedures.

    /// Assigns a value.

    template <typename Integer>
    void assign ( span<Integer> x, Integer y )
    {
        x[0] = y;
        for (auto i = 1uz; i != x.size(); ++i) x[i] = Integer(0);
    }

    /// Assigns a value.
    ///
    /// Requires:
    /// degree(x) ≥ degree(y)

    template <typename Integer>
    void assign ( span<Integer> x, span<Integer const> y )
    {
        for (auto i = 0uz; i != y.size(); ++i) x[i] = y[i];
        for (auto i = y.size(); i != x.size(); ++i) x[i] = Integer(0);
    }

    /// Test procedures.

    /// Tests if zero.

    template <typename Integer>
    auto is_zero ( span<Integer const> x ) noexcept -> bool
    {
        Integer r { 1 };
        auto const xz = x.size();
        for (auto i = 0u; i != xz; ++i)
            r &= is_zero(x[i]);
        return r;
    }

    /// Tests if *not* zero.

    template <typename Integer>
    auto not_zero ( span<Integer const> x ) noexcept -> bool
    {
        Integer r { 0 };
        auto const xz = x.size();
        for (auto i = 0u; i != xz; ++i)
            r |= not_zero(x[i]);
        return r;
    }

    /// Tests if smaller.
    ///
    /// Requires:
    /// degree(x) = degree(y)

    template <typename Integer>
    auto is_smaller_isodegree ( span<Integer const> x, span<Integer const> y ) -> bool
    {
        assert( x.size() == y.size() );

        auto const z = y.size();
        auto b = Integer(0);
        for (auto i = 0uz; i != z; ++i)
            tie( ignore, b ) = difference( x[i], y[i], b );
        return b;
    }

    /// Tests if smaller.

    template <typename Integer>
    auto is_smaller ( span<Integer const> x, span<Integer const> y ) -> bool;
    // deferred definition; see further down.

    /// Tests if *not* smaller.
    ///
    /// Requires:
    /// degree(x) = degree(y)

    template <typename Integer>
    auto not_smaller_isodegree ( span<Integer const> x, span<Integer const> y ) -> bool
    {
        return Integer(1) - is_smaller_isodegree( x, y );
    }

    /// Tests if *not* smaller.

    template <typename Integer>
    auto not_smaller ( span<Integer const> x, span<Integer const> y ) -> bool
    {
        return Integer(1) - is_smaller( x, y );
    }

    /// Tests if greater.
    ///
    /// Requires:
    /// degree(x) = degree(y)

    template <typename Integer>
    auto is_greater_isodegree ( span<Integer const> x, span<Integer const> y ) -> bool
    {
        return is_smaller_isodegree( y, x );
    }

    /// Tests if greater.

    template <typename Integer>
    auto is_greater ( span<Integer const> x, span<Integer const> y ) -> bool;
    // deferred definition; see further down.

    /// Tests if *not* greater.
    ///
    /// Requires:
    /// degree(x) = degree(y)

    template <typename Integer>
    auto not_greater_isodegree ( span<Integer const> x, span<Integer const> y ) -> bool
    {
        return Integer(1) - is_smaller_isodegree( y, x );
    }

    /// Tests if *not* greater.

    template <typename Integer>
    auto not_greater ( span<Integer const> x, span<Integer const> y ) -> bool
    {
        return Integer(1) - is_smaller( y, x );
    }

    /// Tests if equals.
    ///
    /// Requires:
    /// degree(x) = degree(y)

    template <typename Integer>
    auto is_equal_isodegree ( span<Integer const> x, span<Integer const> y ) -> bool
    {
        auto const z = y.size();
        Integer r {};
        Integer b {};
        for (auto i = 0uz; i != z; ++i)
            tie( r, b ) = difference( x[i], y[i], b );
        return is_zero(r) & (1U - b);
    }

    /// Tests if equals.

    template <typename Integer>
    auto is_equal ( span<Integer const> x, span<Integer const> y ) -> bool
    {
        auto const xz = x.size();
        auto const yz = y.size();
        if (xz < yz) return is_equal(y,x);
        auto r = is_equal_isodegree( x, y );
        for (auto i = yz; i != xz; ++i)
            r &= is_zero( x[i] );
        return r;
    }

    /// Tests if *not* equals.
    ///
    /// Requires:
    /// degree(x) = degree(y)

    template <typename Integer>
    auto not_equal_isodegree ( span<Integer const> x, span<Integer const> y ) -> bool
    {
        return Integer(1) - is_equal_isodegree( x, y );
    }

    /// Tests if *not* equals.

    template <typename Integer>
    auto not_equal ( span<Integer const> x, span<Integer const> y ) -> bool
    {
        return Integer(1) - is_equal( x, y );
    }

    /// Tests if normalized.
    ///
    /// Let N = degree(y).
    /// Normalized means B ÷ 2 ≤ y[N-1] < B.
    ///
    /// In a binary machine, this means the most significant bit is 1.

    template <typename Integer>
    auto is_normalized ( span<Integer const> y ) noexcept -> Integer
    {
        return is_normalized( y[ y.size() - 1 ] );
    }

    /// Expand procedures.
    ///
    /// These procedures increase values, producing a "carry" or an "excess".

    /// Next with carry.

    template <typename Integer>
    auto next_assign ( span<Integer> x, Integer carry = Integer(0) ) -> Integer
    {
        carry = sum_assign( x[0], Integer(1), carry );
        for (auto i = 1uz; i != x.size(); ++i)
            carry = sum_assign( x[i], Integer(0), carry );
        return carry;
    }

    /// Sum with carry.

    template <typename Integer>
    auto sum_assign ( span<Integer> x, Integer y, Integer carry = Integer(0) ) noexcept -> Integer
    {
        carry = sum_assign( x[0], y, carry );
        for (auto i = 1uz; i != x.size(); ++i)
            carry = sum_assign( x[i], Integer(0), carry );
        return carry;
    }

    /// Sum with carry.
    ///
    /// Requires:
    /// degree(x) = degree(y)

    template <typename Integer>
    auto sum_assign_isodegree ( span<Integer> x, span<Integer const> y, Integer carry = Integer(0) ) noexcept -> Integer
    {
        for (auto i = 0uz; i != y.size(); ++i)
            carry = sum_assign( x[i], y[i], carry );
        return carry;
    }

    /// Sum with carry.
    ///
    /// Requires:
    /// degree(x) ≥ degree(y)

    template <typename Integer>
    auto sum_assign ( span<Integer> x, span<Integer const> y, Integer carry = Integer(0) ) noexcept -> Integer
    {
        carry = sum_assign_isodegree( x, y, carry );
        for (auto i = y.size(); i != x.size(); ++i)
            carry = sum_assign( x[i], Integer(0), carry );
        return carry;
    }

    /// Twice with excess.

    template <typename Integer>
    auto twice_assign ( span<Integer> x, size_t z, Integer excess = Integer(0) ) noexcept -> Integer
    {
        for (auto i = 0uz; i != x.size(); ++i)
            excess = twice_assign( x[i], z, excess );
        return excess;
    }

    /// Product with excess.

    template <typename Integer>
    auto product_assign ( span<Integer> x, Integer y, Integer excess = Integer(0) ) noexcept -> Integer
    {
        for (auto i = 0uz; i != x.size(); ++i)
            excess = product_assign( x[i], y, excess );
        return excess;
    }

    /// Product and sum.

    template <typename Integer>
    auto product_sum_assign ( span<Integer> r, span<Integer const> x, span<Integer const> y ) noexcept -> Integer
    // requires degree(r) ≥ degree(x) + degree(y) + 1
    {
        auto carry = Integer(0);
        auto const xz = x.size();
        auto const yz = y.size();
        for (auto xi = 0uz; xi != xz; ++xi)
        {
            for (auto yi = 0uz; yi != yz; ++yi)
            {
                auto ri = xi+yi;
                // xi * yi + carry
                auto [ p0, p1 ] = product( x[xi], y[yi], carry );
                // store
                auto c = sum_assign( r[ri], p0 );
                carry = p1 + c;
            }
            // store
            carry = sum_assign( r[xi+yz], carry );
        }
        // store
        return sum_assign( r[xz+yz], carry );
    }

    /// Square and sum.

    template <typename Integer>
    auto square_sum_assign ( span<Integer> r, span<Integer const> x ) noexcept -> Integer
    // requires degree(r) ≥ degree(x) * 2 + 1
    {
        auto carry = Integer(0);
        auto const xz = x.size();
        for (auto xi = 0uz; xi != xz; ++xi)
        {
            // xi ^ 2
            {
                auto ri = xi+xi;
                // xi ^ 2 + carry
                auto [ p0, p1 ] = product( x[xi], x[xi], carry );
                // store
                auto c = sum_assign( r[ri], p0 );
                carry = p1 + c;
            }
            // 2 * xi * xj
            for (auto xj = xi + 1uz; xj != xz; ++xj)
            {
                auto ri = xi+xj;
                // xi * xj
                auto [ p0, p1 ] = product( x[xi], x[xj] );
                // 2 * xi * xj + carry
                auto [ t00, t01 ] = twice( p0, 1uz, carry );
                auto [ t10, t11 ] = twice( p1, 1uz );
                // store
                auto c0 = sum_assign( r[ri+0], t00 );
                auto c1 = sum_assign( r[ri+1], t01, c0 );
                auto c2 = sum_assign( r[ri+1], t10, c1 );
                auto c3 = sum_assign( r[ri+2], t11, c2 );
                carry = c3;
            }
            // store
            carry = sum_assign( r[xi+xz], carry );
        }
        // store
        return sum_assign( r[xz+xz], carry );
    }

    /// Reduce procedures.
    ///
    /// These procedures decrease values, requiring a "borrow" or leaving a "remainder".

    /// Previous with borrow.

    template <typename Integer>
    auto previous_assign ( span<Integer> x, Integer borrow = Integer(0) ) -> Integer
    {
        borrow = difference_assign( x[0], Integer(1), borrow );
        for (auto i = 1uz; i != x.size(); ++i)
            borrow = difference_assign( x[i], Integer(0), borrow );
        return borrow;
    }

    /// Difference with borrow.

    template <typename Integer>
    auto difference_assign ( span<Integer> x, Integer y, Integer borrow = Integer(0) ) noexcept -> Integer
    {
        borrow = difference_assign( x[0], y, borrow );
        for (auto i = 1uz; i != x.size(); ++i) {
            borrow = difference_assign( x[i], Integer(0), borrow );
        }
        return borrow;
    }

    /// Difference with borrow.
    ///
    /// Requires:
    /// degree(x) = degree(y)

    template <typename Integer>
    auto difference_assign_isodegree ( span<Integer> x, span<Integer const> y, Integer borrow = Integer(0) ) noexcept -> Integer
    {
        for (auto i = 0uz; i != y.size(); ++i)
            borrow = difference_assign( x[i], y[i], borrow );
        return borrow;
    }

    /// Difference with borrow.
    ///
    /// Requires:
    /// degree(x) ≥ degree(y)

    template <typename Integer>
    auto difference_assign ( span<Integer> x, span<Integer const> y, Integer borrow = Integer(0) ) noexcept -> Integer
    {
        borrow = difference_assign_isodegree( x, y, borrow );
        for (auto i = y.size(); i != x.size(); ++i)
            borrow = difference_assign( x[i], Integer(0), borrow );
        return borrow;
    }

    /// Half with remainder.

    template <typename Integer>
    auto half_assign ( span<Integer> x, size_t N ) noexcept -> Integer
    {
        constexpr auto B = sizeof(Integer) * 8uz;
        auto const z = x.size();
        if (z == 0) return Integer(0);
        auto r_ = x[0] & ((1 << N) - 1);
        x[0] >>= N;
        for (auto i = 1uz; i != z; ++i) {
            x[i-1] |= x[i] << (B - N);
            x[i] >>= N;
        }
        return r_;
    }

    /// Normalized division with remainder.
    ///
    /// Requires:
    /// x ÷ B < y
    /// y is normalized
    /// iy = reciprocal_normalized(y)

    template <typename Integer>
    auto division_normalized ( span<Integer> q, span<Integer const> x, Integer y, Integer iy ) -> Integer
    {
        auto const xz = x.size();
        auto r = Integer(0);
        for (auto i = xz; i != 0; --i) {
            auto t = array { x[i-1], r };
            tie( q[i-1], r ) = division_normalized( span<Integer const,2>(t), y, iy );
        }
        return r;
    }
}

/// Deferred definitions.

namespace purple
{
    template <typename Integer>
    auto is_smaller ( span<Integer const> x, span<Integer const> y ) -> bool
    {
        auto const xz = x.size();
        auto const yz = y.size();
        if (xz < yz) return is_greater( y, x );
        auto c = is_smaller_isodegree( x, y );
        for (auto i = yz; i != xz; ++i)
            c &= is_zero( x[i] );
        return c;
    }

    template <typename Integer>
    auto is_greater ( span<Integer const> x, span<Integer const> y ) -> bool
    {
        auto const xz = x.size();
        auto const yz = y.size();
        if (xz < yz) return is_smaller( y, x );
        auto c = is_greater_isodegree( x, y );
        for (auto i = yz; i != xz; ++i)
            c |= not_zero( x[i] );
        return c;
    }
}
