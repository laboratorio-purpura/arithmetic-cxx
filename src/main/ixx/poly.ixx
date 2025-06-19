module;

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

    template <typename Integer>
    void sum_accumulate (span<Integer> r, span<const Integer> x, Integer y) noexcept
    // requires is_compact(x)
    // requires r.size() > x.size()
    {
        Integer carry = y;
        auto xz = x.size();
        for (auto i = 0u; i != xz; ++i) {
            tie( r[i], carry ) = sum( x[i], carry );
        }
        r[xz] = carry;
    }

    // --

    template <typename Integer>
    void sum_accumulate (span<Integer> r, span<const Integer> x, span<const Integer> y) noexcept
    // requires is_compact(x) && is_compact(y)
    // requires x.size() == y.size()
    // requires r.size() > x.size()
    {
        Integer carry {};
        auto xz = x.size();
        for (auto i = 0u; i != xz; ++i) {
            tie( r[i], carry ) = sum( x[i], y[i], carry );
        }
        r[xz] = carry;
    }

    // --

    template <typename Integer>
    void twice_accumulate (span<Integer> r, span<const Integer> x) noexcept
    // requires is_compact(x)
    // requires r.size() > x.size()
    {
        Integer carry {};
        auto xz = x.size();
        for (auto i = 0u; i != xz; ++i) {
            tie( r[i], carry ) = twice( x[i], carry );
        }
        r[xz] = carry;
    }

    // --

    template <typename Integer>
    void product_accumulate (span<Integer> r, span<const Integer> x, Integer y) noexcept
    // requires is_compact(x)
    // requires r.size() > x.size()
    {
        Integer carry {};
        auto xz = x.size();
        for (auto xi = 0u; xi != xz; ++xi) {
            tie( r[xi], carry ) = product( x[xi], y, carry );
        }
        r[xz] = carry;
    }

    // --

    template <typename Integer>
    void product_accumulate (span<Integer> r, span<const Integer> x, span<const Integer> y) noexcept
    // requires is_compact(x) && is_compact(y)
    // requires r.size() > x.size() + y.size()
    {
        Integer carry {};
        int xz = x.size();
        int yz = y.size();
        for (int xi = 0; xi != xz; ++xi)
        {
            // xi * yi
            for (int yi = 0; yi != yz; ++yi) {
                auto ri = xi+yi;
                auto xv = x[xi];
                auto yv = y[yi];
                auto rv = r[ri];
                // xi * yi + carry
                auto [p,c0] = product( xv, yv, carry );]
                // store product and propagate carry
                auto [s,c1] = sum( rv, p );
                r[ri] = s;
                carry = c0 + c1;
            }
            // propagate carry
            carry = sum_accumulate( r[xi+yz], carry );
        }
        // propagate carry
        r[xz+yz] = carry; // TODO
    }

    // --

    template <typename Integer>
    void square_accumulate (span<Integer> r, span<const Integer> x) noexcept
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
                auto [r0,c0] = product( x[xi], x[xj], carry );
                // 2 * ( xi * xj + carry )
                tie( r[xi+xj], carry ) = twice( r0, c0 );
            }
            // propagate carry
            carry = sum_accumulate( r[xi+xz], carry );
        }
        // propagate carry
        r[xz+xz] = carry; // TODO
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
