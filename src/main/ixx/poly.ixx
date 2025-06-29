// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <cassert>
#include <span>
#include <tuple>

export module purple.arithmetic:poly;

import :mono;
import :duo;

using std::array;
using std::ignore;
using std::size_t;
using std::span;
using std::tie;

/// Poly-degree arithmetic.

export namespace purple
{
    /// Properties.

    /// 1 if and only if every term of x is significant, else 0.
    template <typename Integer>
    auto is_compact ( span<Integer const> x ) noexcept -> Integer;

    /// 1 if and only if every term of x is significant, else 0.
    template <typename Integer>
    auto is_compact ( span<Integer> x ) noexcept -> Integer
    {
        return is_compact(span<Integer const>(x));
    }

    /// 1 if and only if x is the additive identity, else 0.
    template <typename Integer>
    auto is_zero ( span<Integer const> x ) noexcept -> Integer
    {
        Integer r { 1 };
        auto const xz = x.size();
        for (auto i = 0u; i != xz; ++i)
            r &= is_zero(x[i]);
        return r;
    }

    /// 1 if and only if x is *not* the additive identity, else 0.
    template <typename Integer>
    auto not_zero ( span<Integer const> x ) noexcept -> Integer
    {
        Integer r { 0 };
        auto const xz = x.size();
        for (auto i = 0u; i != xz; ++i)
            r |= not_zero(x[i]);
        return r;
    }

    /// 1 if and only if x is smaller than y, else 0.
    template <typename Integer>
    auto is_smaller ( span<Integer const> x, span<Integer const> y ) -> Integer
    {
        assert( is_compact(x) );
        assert( is_compact(y) );
        auto const xz = x.size();
        auto const yz = y.size();
        if (xz < yz) return 1;
        Integer r {};
        Integer carry { 0 };
        for (auto i = 0uz; i != xz; ++i) {
            tie( r, carry ) = difference( x[i], y[i], carry );
        }
        return ( is_zero(r) & carry ) | ( not_zero(r) & not_zero(carry) );
    }

    /// 1 if and only if x is not smaller than y, else 0.
    template <typename Integer>
    auto not_smaller ( span<Integer const> x, span<Integer const> y ) -> Integer
    {
        assert( is_compact(x) );
        assert( is_compact(y) );
        auto const xz = x.size();
        auto const yz = y.size();
        if (xz < yz) return 0;
        Integer difference {};
        Integer carry {};
        auto const z = x.size();
        for (auto i = 0uz; i != z; ++i) {
            tie( difference, carry ) = difference( x[i], y[i], carry );
        }
        return carry;
    }

    /// 1 if and only if x is greater than y, else 0.
    template <typename Integer>
    auto is_greater ( span<Integer const> x, span<Integer const> y ) -> Integer
    {
        return 1U - is_smaller(y,x);
    }

    /// 1 if and only if x is not greater than y, else 0.
    template <typename Integer>
    auto not_greater ( span<Integer const> x, span<Integer const> y ) -> Integer
    {
        return 1U - not_smaller(y,x);
    }

    /// 1 if and only if x is equal than y, else 0.
    template <typename Integer>
    auto is_equal ( span<Integer const> x, span<Integer const> y ) -> Integer
    {
        assert( is_compact(x) );
        assert( is_compact(y) );
        auto const xz = x.size();
        auto const yz = y.size();
        if (xz != yz) return 0;
        Integer difference {};
        Integer carry {};
        for (auto i = 0uz; i != xz; ++i) {
            tie( difference, carry ) = difference( x[i], y[i], carry );
        }
        return is_zero(difference) & is_zero(carry);
    }

    /// 1 if and only if x is equal than y, else 0.
    template <typename Integer>
    auto not_equal ( span<Integer const> x, span<Integer const> y ) -> Integer
    {
        assert( is_compact(x) );
        assert( is_compact(y) );
        assert( x.size() == y.size() );
        Integer difference {};
        Integer carry {};
        auto const z = x.size();
        for (auto i = 0uz; i != z; ++i) {
            tie( difference, carry ) = difference( x[i], y[i], carry );
        }
        return not_zero(difference) | carry;
    }

    /// Operators.

    /// Accumulate sum, return carry.
    template <typename Integer>
    auto sum_accumulate ( span<Integer> r, Integer y ) noexcept -> Integer
    {
        assert( is_compact(r) );
        Integer carry {};
        auto const rz = r.size();
        if (rz == 0) return carry;
        carry = sum_accumulate( r[0], y, carry );
        for (auto i = 1uz; i != rz; ++i) {
            carry = sum_accumulate( r[i], 0, carry );
        }
        return carry;
    }

    // Accumulate sum, return carry.
    template <typename Integer>
    auto sum_accumulate ( span<Integer> r, span<Integer const> y ) noexcept -> Integer
    {
        assert( is_compact(r) );
        assert( is_compact(y) );
        assert( r.size() >= y.size() );
        Integer carry {};
        auto const rz = r.size();
        for (auto i = 0uz; i != rz; ++i) {
            carry = sum_accumulate( r[i], y[i], carry );
        }
        return carry;
    }

    /// Accumulate difference, return carry.
    template <typename Integer>
    auto difference_accumulate ( span<Integer> r, Integer y ) noexcept -> Integer
    {
        assert( is_compact(r) );
        Integer carry {};
        auto const z = r.size();
        if (z == 0) return carry;
        carry = difference_accumulate( r[0], y, carry );
        for (auto i = 1uz; i != z; ++i) {
            carry = difference_accumulate( r[i], 0, carry );
        }
        return carry;
    }

    // Accumulate difference, return carry.
    template <typename Integer>
    auto difference_accumulate ( span<Integer> r, span<Integer const> y ) noexcept -> Integer
    {
        assert( is_compact(r) );
        assert( is_compact(y) );
        assert( r.size() >= y.size() );
        Integer carry {};
        auto const z = r.size();
        for (auto i = 0uz; i != z; ++i) {
            carry = difference_accumulate( r[i], y[i], carry );
        }
        return carry;
    }

    // Accumulate product, return carry.
    template <typename Integer>
    auto product_accumulate ( span<Integer> r, Integer y ) noexcept -> Integer
    {
        assert( is_compact(r) );
        Integer carry {};
        auto const rz = r.size();
        for (auto i = 0uz; i != rz; ++i) {
            carry = product_sum_accumulate( r[i], y, carry );
        }
        return carry;
    }

    // Accumulate product and sum, return carry.
    template <typename Integer>
    auto product_sum_accumulate ( span<Integer> r, span<Integer const> x, span<Integer const> y ) noexcept -> Integer
    {
        assert( is_compact(x) );
        assert( is_compact(y) );
        assert( r.size() >= x.size() + y.size() );
        Integer carry {};
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
        return carry;
    }

    // Accumulate twice N times, return carry.
    template <typename Integer>
    auto twice_accumulate ( span<Integer> r, size_t N ) noexcept -> Integer
    {
        assert( is_compact(r) );
        Integer carry {};
        auto const rz = r.size();
        for (auto i = 0uz; i != rz; ++i) {
            carry = twice_sum_accumulate( r[i], N, carry );
        }
        return carry;
    }

    // Accumulate square, return carry.
    template <typename Integer>
    auto square_sum_accumulate ( span<Integer> r, span<Integer const> x ) noexcept -> Integer
    {
        assert( is_compact(x) );
        assert( r.size() >= 2 * x.size() );
        Integer carry {};
        auto const xz = x.size();
        for (auto xi = 0uz; xi != xz; ++xi)
        {
            // xi ^ 2
            {
                auto ri = xi+xi;
                // xi ^ 2 + carry
                auto [ p0, p1 ] = square_sum( x[xi], carry );
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
        return carry;
    }

    // Accumulate half N times, rounded down.
    template <typename Integer>
    auto half_accumulate ( span<Integer> r, size_t N ) noexcept
    {
        constexpr auto B = sizeof(Integer) * 8uz;
        assert( is_compact(r) );
        auto const rz = r.size();
        if (rz == 0) return;
        r[0] >>= N;
        for (auto i = 1uz; i != rz; ++i) {
            r[i-1] |= r[i] << (B - N);
            r[i] >>= N;
        }
    }

    /// Store quotient, return remainder, with "normalised" operands.
    template <typename Integer>
    auto ratio_normalised ( span<Integer> q, span<Integer const> x, Integer y, Integer iy ) -> Integer
    // requires q.size() >= x.size()
    // requires B/2 <= y < B
    // requires iy = ( (B^2 - 1) / y ) - B
    {
        auto const xz = x.size();
        auto r = Integer(0);
        for (auto i = xz; i != 0; --i) {
            auto t = array<unsigned,2> { x[i-1], r };
            tie( q[i-1], r ) = ratio_normalised<Integer>( t, y, iy );
        }
        return r;
    }

    /// Store quotient, return remainder, with "normalised" operands.
    template <typename Integer>
    auto ratio ( span<Integer> q, span<Integer const> x, Integer y ) -> Integer
    // requires q.size() >= x.size()
    // requires B/2 <= y < B
    {
        auto iy = inverse_normalised(y);
        return ratio_normalised( q, x, y, iy );
    }
}

/// Deferred definitions.

namespace purple
{
    template <typename Integer>
    auto is_compact (span<const Integer> x) noexcept -> Integer
    {
        switch (x.size())
        {
            case 0: return 0;
            case 1: return 1;
            default: return not_zero( x[ x.size() - 1 ]);
        }
    }
}
