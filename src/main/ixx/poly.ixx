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
    auto is_compact (span<const Integer> x) noexcept -> Integer;

    /// 1 if and only if every term of x is significant, else 0.
    template <typename Integer>
    auto is_compact (span<Integer> x) noexcept -> Integer
    {
        return is_compact( span< const Integer >(x) );
    }

    /// 1 if and only if x is the additive identity, else 0.
    template <typename Integer>
    auto is_zero (span<const Integer> x) noexcept -> Integer
    {
        Integer r { 1 };
        auto const xz = x.size();
        for (auto i = 0u; i != xz; ++i)
            r &= is_zero(x[i]);
        return r;
    }

    /// 1 if and only if x is *not* the additive identity, else 0.
    template <typename Integer>
    auto not_zero (span<const Integer> x) noexcept -> Integer
    {
        Integer r { 0 };
        auto const xz = x.size();
        for (auto i = 0u; i != xz; ++i)
            r |= not_zero(x[i]);
        return r;
    }

    /// poly integer arithmetic

    /// Accumulates r += y, returns carry.
    template <typename Integer>
    auto sum_accumulate (span<Integer> r, Integer y) noexcept -> Integer
    {
        assert( is_compact(r) );
        Integer carry = y;
        auto rz = r.size();
        for (auto i = 0uz; i != rz; ++i) {
            tie( r[i], carry ) = sum( r[i], carry );
        }
        return carry;
    }

    // Accumulates r += y, returns carry.
    template <typename Integer>
    auto sum_accumulate (span<Integer> r, span<const Integer> y) noexcept -> Integer
    {
        assert( is_compact(r) );
        assert( is_compact(y) );
        assert( r.size() >= y.size() );
        Integer carry {};
        auto rz = r.size();
        for (auto i = 0uz; i != rz; ++i) {
            tie( r[i], carry ) = sum( r[i], y[i], carry );
        }
        return carry;
    }

    // Accumulates r *= y, returns carry.
    template <typename Integer>
    auto product_accumulate (span<Integer> r, Integer y) noexcept -> Integer
    {
        assert( is_compact(r) );
        Integer carry {};
        auto rz = r.size();
        for (auto i = 0uz; i != rz; ++i) {
            tie( r[i], carry ) = product_sum( r[i], y, carry );
        }
        return carry;
    }

    // Accumulates r += x * y, returns carry.
    template <typename Integer>
    auto product_sum_accumulate (span<Integer> r, span<const Integer> x, span<const Integer> y) noexcept -> Integer
    {
        assert( is_compact(x) );
        assert( is_compact(y) );
        assert( r.size() >= x.size() + y.size() );
        Integer carry {};
        auto xz = x.size();
        auto yz = y.size();
        for (auto xi = 0uz; xi != xz; ++xi)
        {
            for (auto yi = 0uz; yi != yz; ++yi)
            {
                auto ri = xi+yi;
                auto xv = x[xi];
                auto yv = y[yi];
                auto rv = r[ri];
                auto [ p0, p1 ] = product_sum( xv, yv, carry );
                auto [ s, c ] = sum( rv, p0 );
                r[ri] = s;
                carry = p1 + c;
            }
            // propagate carry
            carry = sum_accumulate( r[xi+yz], carry );
        }
        return carry;
    }

    // --

    template <typename Integer>
    auto twice_accumulate (span<Integer> r, unsigned N) noexcept -> Integer
    {
        assert( is_compact(r) );
        Integer carry {};
        auto rz = r.size();
        for (auto i = 0uz; i != rz; ++i) {
            carry = twice_sum_accumulate( r[i], N, carry );
        }
        return carry;
    }

    // --

    template <typename Integer>
    auto square_accumulate (span<Integer> r, span<const Integer> x) noexcept -> Integer
    // requires is_compact(x)
    // requires r.size() > 2 * x.size()
    {
        Integer carry {};
        auto xz = x.size();
        for (auto xi = 0u; xi != xz; ++xi)
        {
            // xi ^ 2
            tie( r[xi+xi], carry ) = square( x[xi], carry );
            // 2 * xi * xj
            for (auto xj = xi + 1u; xj != xz; ++xj) {
                // xi * xj + carry
                auto [r0,c0] = product_sum( x[xi], x[xj], carry );
                // 2 * ( xi * xj + carry )
                tie( r[xi+xj], carry ) = twice( r0, c0 );
            }
            // propagate carry
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
