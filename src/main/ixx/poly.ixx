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

/// Multi-degree nonnegative integer arithmetics.
///
/// This module partition defines integer arithmetics on multi-degree integers,
/// that is, integers represented with multiple "limbs".
///
/// In the specification of this module partition, let B = 2 ^ bits be the numeric base of a single "limb"
/// and let, for some multi-degree integer x, B(x) = B ^ degree(x).

export namespace purple
{
    /// Representation.

    /// Assigns a value.
    ///
    /// Requirements:
    /// degree(x) > 1
    ///
    /// Effects:
    /// x = y
    template <typename Integer>
    void assign ( span<Integer> x, Integer y )
    {
        x[0] = y;
        for (auto i = 1uz; i != x.size(); ++i) x[i] = Integer(0);
    }

    /// Assigns a value.
    ///
    /// Requirements:
    /// degree(x) >= degree(y)
    ///
    /// Effects:
    /// x = y
    template <typename Integer>
    void assign ( span<Integer> x, span<Integer const> y )
    {
        for (auto i = 0uz; i != y.size(); ++i) x[i] = y[i];
        for (auto i = y.size(); i != x.size(); ++i) x[i] = Integer(0);
    }

    /// Tests.
    ///
    /// These procedures return 1 if the test holds, else return 0.

    /// Tests if normalized.
    ///
    /// Normalized means the most significant bit is 1.
    template <typename Integer>
    auto is_normalized ( span<Integer const> x ) noexcept -> Integer
    {
        return is_normalized( x[ x.size() - 1 ] );
    }

    /// Tests if zero.
    template <typename Integer>
    auto is_zero ( span<Integer const> x ) noexcept -> Integer
    {
        Integer r { 1 };
        auto const xz = x.size();
        for (auto i = 0u; i != xz; ++i)
            r &= is_zero(x[i]);
        return r;
    }

    /// Tests if *not* zero.
    template <typename Integer>
    auto not_zero ( span<Integer const> x ) noexcept -> Integer
    {
        Integer r { 0 };
        auto const xz = x.size();
        for (auto i = 0u; i != xz; ++i)
            r |= not_zero(x[i]);
        return r;
    }

    /// Tests if smaller.
    template <typename Integer>
    auto is_smaller_isodegree ( span<Integer const> x, span<Integer const> y ) -> Integer
    // requires degree(x) = degree(y)
    {
        assert( x.size() == y.size() );

        auto const z = y.size();
        auto c = Integer(0);
        for (auto i = 0uz; i != z; ++i)
            tie( ignore, c ) = difference( x[i], y[i], c );
        return c;
    }

    /// Tests if smaller.
    template <typename Integer>
    auto is_smaller ( span<Integer const> x, span<Integer const> y ) -> Integer;
    // deferred definition; see further down.

    /// Tests if *not* smaller.
    template <typename Integer>
    auto not_smaller_isodegree ( span<Integer const> x, span<Integer const> y ) -> Integer
    // requires degree(x) = degree(y)
    {
        return Integer(1) - is_smaller_isodegree( x, y );
    }

    /// Tests if *not* smaller.
    template <typename Integer>
    auto not_smaller ( span<Integer const> x, span<Integer const> y ) -> Integer
    {
        return Integer(1) - is_smaller( x, y );
    }

    /// Tests if greater.
    template <typename Integer>
    auto is_greater_isodegree ( span<Integer const> x, span<Integer const> y ) -> Integer
    // requires degree(x) = degree(y)
    {
        return is_smaller_isodegree( y, x );
    }

    /// Tests if greater.
    template <typename Integer>
    auto is_greater ( span<Integer const> x, span<Integer const> y ) -> Integer;
    // deferred definition; see further down.

    /// Tests if *not* greater.
    template <typename Integer>
    auto not_greater_isodegree ( span<Integer const> x, span<Integer const> y ) -> Integer
    // requires degree(x) = degree(y)
    {
        return Integer(1) - is_smaller_isodegree( y, x );
    }

    /// Tests if *not* greater.
    template <typename Integer>
    auto not_greater ( span<Integer const> x, span<Integer const> y ) -> Integer
    {
        return Integer(1) - is_smaller( y, x );
    }

    /// Tests if equals.
    template <typename Integer>
    auto is_equal_isodegree ( span<Integer const> x, span<Integer const> y ) -> Integer
    // requires degree(x) = degree(y)
    {
        auto const z = y.size();
        Integer r {};
        Integer c {};
        for (auto i = 0uz; i != z; ++i)
            tie( r, c ) = difference( x[i], y[i], c );
        return is_zero(r) & (1U - c);
    }

    /// Tests if equals.
    template <typename Integer>
    auto is_equal ( span<Integer const> x, span<Integer const> y ) -> Integer
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
    template <typename Integer>
    auto not_equal_isodegree ( span<Integer const> x, span<Integer const> y ) -> Integer
    // requires degree(x) = degree(y)
    {
        return Integer(1) - is_equal_isodegree( x, y );
    }

    /// Tests if *not* equals.
    template <typename Integer>
    auto not_equal ( span<Integer const> x, span<Integer const> y ) -> Integer
    {
        return Integer(1) - is_equal( x, y );
    }

    /// Increase procedures.
    ///
    /// These procedures increase values, maybe with a "carry" or an "excess".

    template <typename Integer>
    auto next_accumulate ( span<Integer> x ) -> Integer
    // requires degree(x) >= 1
    {
        auto carry = Integer(0);
        carry = sum_accumulate( x[0], Integer(1), carry );
        for (auto i = 1uz; i != x.size(); ++i)
            carry = sum_accumulate( x[i], Integer(0), carry );
        return carry;
    }

    /// Sum with carry.
    ///
    /// Computes r = x + y + carry.
    /// If r is higher than B(x), set carry ← 1; else carry ← 0.
    ///
    /// Accumulates x ← r % B(x).
    /// Returns carry.
    template <typename Integer>
    auto sum_accumulate ( span<Integer> x, Integer y, Integer carry = Integer(0) ) noexcept -> Integer
    // requires degree(r) ≥ 1
    {
        carry = sum_accumulate( x[0], y, carry );
        for (auto i = 1uz; i != x.size(); ++i)
            carry = sum_accumulate( x[i], Integer(0), carry );
        return carry;
    }

    /// Sum with carry.
    ///
    /// Computes r = x + y + carry.
    /// If r is higher than B(x), set carry ← 1; else carry ← 0.
    ///
    /// Accumulates x ← r % B(x).
    /// Returns carry.
    template <typename Integer>
    auto sum_accumulate_isodegree ( span<Integer> x, span<Integer const> y, Integer carry = Integer(0) ) noexcept -> Integer
    // requires degree(x) = degree(y)
    {
        for (auto i = 0uz; i != y.size(); ++i)
            carry = sum_accumulate( x[i], y[i], carry );
        return carry;
    }

    /// Sum with carry.
    ///
    /// Computes r = x + y + carry.
    /// If r is higher than B(x), set carry ← 1; else carry ← 0.
    ///
    /// Accumulates x ← r % B(x).
    /// Returns carry.
    template <typename Integer>
    auto sum_accumulate ( span<Integer> x, span<Integer const> y, Integer carry = Integer(0) ) noexcept -> Integer
    // requires degree(x) ≥ degree(y)
    {
        carry = sum_accumulate_isodegree( x, y, carry );
        for (auto i = y.size(); i != x.size(); ++i)
            carry = sum_accumulate( x[i], Integer(0), carry );
        return carry;
    }

    /// Twice N times.
    ///
    /// In a binary machine, twice means shifting bits towards most significant.
    ///
    /// Computes r = x × 2 ^ N + carry.
    ///
    /// Accumulates x ← r % B(x).
    /// Returns r ÷ B(x).
    template <typename Integer>
    auto twice_accumulate ( span<Integer> x, size_t N, Integer carry = Integer(0) ) noexcept -> Integer
    {
        for (auto i = 0uz; i != x.size(); ++i)
            carry = twice_sum_accumulate( x[i], N, carry );
        return carry;
    }

    /// Product.
    ///
    /// Computes r = x × y.
    ///
    /// Accumulates x ← r % B(x).
    /// Returns r ÷ B(x).
    template <typename Integer>
    auto product_accumulate ( span<Integer> x, Integer y ) noexcept -> Integer
    {
        auto carry = Integer(0);
        for (auto i = 0uz; i != x.size(); ++i)
            carry = product_sum_accumulate( x[i], y, carry );
        return carry;
    }

    /// Product and sum.
    ///
    /// Computes r = x × y + r.
    ///
    /// Accumulates r ← r % B(r).
    /// Returns r ÷ B(r).
    template <typename Integer>
    auto product_sum_accumulate ( span<Integer> r, span<Integer const> x, span<Integer const> y ) noexcept -> Integer
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
                auto [ p0, p1 ] = product_sum( x[xi], y[yi], carry );
                // store
                auto c = sum_accumulate( r[ri], p0 );
                carry = p1 + c;
            }
            // store
            carry = sum_accumulate( r[xi+yz], carry );
        }
        // store
        return sum_accumulate( r[xz+yz], carry );
    }

    /// Square and sum.
    ///
    /// Computes r = x ^ 2 + r.
    ///
    /// Accumulates r ← r % B(r).
    /// Returns r ÷ B(r).
    template <typename Integer>
    auto square_sum_accumulate ( span<Integer> r, span<Integer const> x ) noexcept -> Integer
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
                auto [ p0, p1 ] = product_sum( x[xi], x[xi], carry );
                // store
                auto c = sum_accumulate( r[ri], p0 );
                carry = p1 + c;
            }
            // 2 * xi * xj
            for (auto xj = xi + 1uz; xj != xz; ++xj)
            {
                auto ri = xi+xj;
                // xi * xj
                auto [ p0, p1 ] = product( x[xi], x[xj] );
                // 2 * xi * xj + carry
                auto [ t00, t01 ] = twice_sum( p0, 1, carry );
                auto [ t10, t11 ] = twice( p1, 1 );
                // store
                auto c0 = sum_accumulate( r[ri+0], t00 );
                auto c1 = sum_accumulate( r[ri+1], t01, c0 );
                auto c2 = sum_accumulate( r[ri+1], t10, c1 );
                auto c3 = sum_accumulate( r[ri+2], t11, c2 );
                carry = c3;
            }
            // store
            carry = sum_accumulate( r[xi+xz], carry );
        }
        // store
        return sum_accumulate( r[xz+xz], carry );
    }

    /// Decrease procedures.
    ///
    /// These procedures decrease values, maybe with a "borrow" or a "remainder".

    template <typename Integer>
    auto previous_accumulate ( span<Integer> x ) -> Integer
    // requires degree(x) >= 1
    {
        auto borrow = Integer(0);
        borrow = difference_accumulate( x[0], Integer(1), borrow );
        for (auto i = 1uz; i != x.size(); ++i)
            borrow = difference_accumulate( x[i], Integer(0), borrow );
        return borrow;
    }

    /// Difference with borrow.
    ///
    /// Computes r = x - y - borrow.
    /// If r is lower than zero, borrow ← 1; else borrow ← 0.
    ///
    /// Accumulates x ← |r|.
    /// Returns borrow.
    template <typename Integer>
    auto difference_accumulate ( span<Integer> x, Integer y, Integer borrow = Integer(0) ) noexcept -> Integer
    // requires degree(r) ≥ 1
    {
        borrow = difference_accumulate( x[0], y, borrow );
        for (auto i = 1uz; i != x.size(); ++i) {
            borrow = difference_accumulate( x[i], Integer(0), borrow );
        }
        return borrow;
    }

    /// Difference with borrow.
    ///
    /// Computes r = x - y - borrow.
    /// If r is lower than zero, borrow ← 1; else borrow ← 0.
    ///
    /// Accumulates x ← |r|.
    /// Returns borrow.
    template <typename Integer>
    auto difference_accumulate_isodegree ( span<Integer> x, span<Integer const> y, Integer borrow = Integer(0) ) noexcept -> Integer
    // requires degree(r) = degree(y)
    {
        for (auto i = 0uz; i != y.size(); ++i)
            borrow = difference_accumulate( x[i], y[i], borrow );
        return borrow;
    }

    /// Difference with borrow.
    ///
    /// Computes r = x - y - borrow.
    /// If r is lower than zero, borrow ← 1; else borrow ← 0.
    ///
    /// Accumulates x ← |r|.
    /// Returns borrow.
    template <typename Integer>
    auto difference_accumulate ( span<Integer> x, span<Integer const> y, Integer borrow = Integer(0) ) noexcept -> Integer
    // requires degree(r) ≥ degree(y)
    {
        borrow = difference_accumulate_isodegree( x, y, borrow );
        for (auto i = y.size(); i != x.size(); ++i)
            borrow = difference_accumulate( x[i], Integer(0), borrow );
        return borrow;
    }

    /// Half N times with remainder.
    ///
    /// In a binary machine, half means shifting bits towards least significant.
    ///
    /// Computes q = x ÷ 2 ^ N and r = x % 2 ^ N.
    ///
    /// Accumulates x ← q.
    /// Returns r.
    template <typename Integer>
    auto half_accumulate ( span<Integer> x, size_t N ) noexcept -> Integer
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

    /// Division, quotient and remainder, with normalized operands.
    ///
    /// Computes q = x ÷ y and r = x % y.
    ///
    /// Returns r.
    template <typename Integer>
    auto division_normalized ( span<Integer> q, span<Integer const> x, Integer y, Integer iy ) -> Integer
    // requires degree(q) ≥ degree(x) ≥ 1
    // requires is_normalized(y)
    // requires iy = inverse_normalized(y)
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
    auto is_smaller ( span<Integer const> x, span<Integer const> y ) -> Integer
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
    auto is_greater ( span<Integer const> x, span<Integer const> y ) -> Integer
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
