// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <cassert>
#include <span>
#include <tuple>

export module purple:poly;

import :mono;

using std::span;
using std::tie;
using std::tuple;

export namespace purple
{
    /// poly integer properties

    /// 1 if and only if every term of x is significant, else 0.
    template <typename Integer>
    auto is_compact (span<Integer const> x) noexcept -> Integer;

    /// 1 if and only if every term of x is significant, else 0.
    template <typename Integer>
    auto is_compact (span<Integer> x) noexcept -> Integer
    {
        return is_compact(span<Integer const>(x));
    }

    /// 1 if and only if x is the additive identity, else 0.
    template <typename Integer>
    auto is_zero (span<Integer const> x) noexcept -> Integer
    {
        Integer r { 1 };
        auto const xz = x.size();
        for (auto i = 0u; i != xz; ++i)
            r &= is_zero(x[i]);
        return r;
    }

    /// 1 if and only if x is *not* the additive identity, else 0.
    template <typename Integer>
    auto not_zero (span<Integer const> x) noexcept -> Integer
    {
        Integer r { 0 };
        auto const xz = x.size();
        for (auto i = 0u; i != xz; ++i)
            r |= not_zero(x[i]);
        return r;
    }

    /// poly integer arithmetic

    /// Accumulates r + y, returns carry.
    template <typename Integer>
    auto sum_accumulate (span<Integer> r, Integer y) noexcept -> Integer
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

    // Accumulates r + y, returns carry.
    template <typename Integer>
    auto sum_accumulate (span<Integer> r, span<Integer const> y) noexcept -> Integer
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

    // Accumulates r * y, returns carry.
    template <typename Integer>
    auto product_accumulate (span<Integer> r, Integer y) noexcept -> Integer
    {
        assert( is_compact(r) );
        Integer carry {};
        auto const rz = r.size();
        for (auto i = 0uz; i != rz; ++i) {
            carry = product_sum_accumulate( r[i], y, carry );
        }
        return carry;
    }

    // Accumulates r + (x * y), returns carry.
    template <typename Integer>
    auto product_sum_accumulate (span<Integer> r, span<Integer const> x, span<Integer const> y) noexcept -> Integer
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

    // Accumulates r * 2 * N, returns carry.
    template <typename Integer>
    auto twice_accumulate (span<Integer> r, unsigned N) noexcept -> Integer
    {
        assert( is_compact(r) );
        Integer carry {};
        auto const rz = r.size();
        for (auto i = 0uz; i != rz; ++i) {
            carry = twice_sum_accumulate( r[i], N, carry );
        }
        return carry;
    }

    // Accumulates r + x * x, returns carry.
    template <typename Integer>
    auto square_sum_accumulate (span<Integer> r, span<Integer const> x) noexcept -> Integer
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

    // --

    template <typename Integer>
    void ratio_accumulate (span<Integer> q, span<Integer> r, span<const Integer> x, Integer y)
    // requires is_compact(x)
    // requires not_zero(y)
    // requires q.size() >= x.size()
    // requires r.size() >= x.size()
    {
        auto const xz = x.size();
        for (auto j = xz; j != 0; --j) {
            tie( q[j-1], r[j-1] ) = ratio( x[j-1], y );
        }
    }

    template <typename Integer>
    void quotient_accumulate (span<Integer> q, span<const Integer> x, Integer y)
    // requires is_compact(x)
    // requires not_zero(y)
    // requires q.size() >= x.size()
    {
        auto const xz = x.size();
        for (auto j = xz; j != 0; --j) {
            tie( q[j-1], std::ignore ) = ratio( x[j-1], y );
        }
    }

    template <typename Integer>
    void remainder_accumulate (span<Integer> r, span<const Integer> x, Integer y)
    // requires is_compact(x)
    // requires not_zero(y)
    // requires r.size() >= x.size()
    {
        auto const xz = x.size();
        for (auto j = xz; j != 0; --j) {
            tie( std::ignore, r[j-1] ) = ratio( x[j-1], y );
        }
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
