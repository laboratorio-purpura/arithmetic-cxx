module;

#include <span>
#include <tuple>
#include <vector>

export module purple;

export import :mono;

using namespace std;

export namespace purple
{
    /// poly integer properties

    /// 1 if and only if every term of x is significant, else 0.
    template <typename Integer>
    auto is_compact (span<const Integer> x) noexcept -> Integer;

    /// 1 if and only if every term of x is significant, else 0.
    template <typename Integer>
    auto is_compact (vector<Integer> const & x) -> Integer
    {
        auto xs = span( x.begin(), x.end() );
        return is_compact(xs);
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

    /// 1 if and only if x is the additive identity, else 0.
    template <typename Integer>
    auto is_zero (vector<Integer> const & x) noexcept
    {
        auto xs = span( x.begin(), x.end() );
        return is_zero(xs);
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

    /// 1 if and only if x is *not* the additive identity, else 0.
    template <typename Integer>
    auto not_zero (vector<Integer> const & x) noexcept
    {
        auto xs = span( x.begin(), x.end() );
        return not_zero(xs);
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

    template <typename Integer>
    void sum_accumulate (vector<Integer> & r, vector<Integer> const & x, Integer y) noexcept
    // requires is_compact(x)
    // requires r.size() > x.size()
    {
        auto rs = span( r.begin(), r.end() );
        auto xs = span( x.begin(), x.end() );
        sum_accumulate(rs,xs,y);
    }

    template <typename Integer>
    auto sum (vector<Integer> const & x, Integer y) -> vector<Integer>
    // requires is_compact(x)
    {
        auto r = vector<Integer>( x.size() + 1 );
        sum_accumulate(r,x,y);
        return std::move(r);
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

    template <typename Integer>
    void sum_accumulate (vector<Integer> & r, vector<Integer> const & x, vector<Integer> const & y) noexcept
    // requires is_compact(x) && is_compact(y)
    // requires x.size() == y.size()
    // requires r.size() > x.size()
    {
        auto rs = span( r.begin(), r.end() );
        auto xs = span( x.begin(), x.end() );
        auto ys = span( y.begin(), y.end() );
        sum_accumulate(rs,xs,ys);
    }

    template <typename Integer>
    auto sum (vector<Integer> const & x, vector<Integer> const & y) -> vector<Integer>
    // requires is_compact(x) && is_compact(y)
    // requires x.size() == y.size()
    {
        auto r = vector<Integer>( x.size() + 1 );
        sum_accumulate(r,x,y);
        return std::move(r);
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

    template <typename Integer>
    void twice_accumulate (vector<Integer> & r, vector<Integer> const & x) noexcept
    // requires is_compact(x)
    // requires r.size() > x.size()
    {
        auto rs = span( r.begin(), r.end() );
        auto xs = span( x.begin(), x.end() );
        twice_accumulate(rs,xs);
    }

    template <typename Integer>
    auto twice (vector<Integer> const & x) -> vector<Integer>
    // requires is_compact(x)
    {
        auto r = vector<Integer>( x.size() + 1 );
        twice_accumulate(r,x);
        return std::move(r);
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

    template <typename Integer>
    void product_accumulate (vector<Integer> & r, vector<Integer> const & x, Integer y) noexcept
    // requires is_compact(x)
    // requires r.size() > x.size()
    {
        auto rs = span( r.begin(), r.end() );
        auto xs = span( x.begin(), x.end() );
        product_accumulate(rs,xs,y);
    }

    template <typename Integer>
    auto product (vector<Integer> const & x, Integer y) -> vector<Integer>
    // requires is_compact(x)
    {
        auto r = vector<Integer>( x.size() + 1 );
        product_accumulate(r,x,y);
        return std::move(r);
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
                tie( r[xi+yi], carry ) = product( x[xi], y[yi], carry );
            }
            // propagate carry
            carry = sum_accumulate( r[xi+yz], carry );
        }
        // propagate carry
        r[xz+yz] = carry; // TODO
    }

    template <typename Integer>
    void product_accumulate (vector<Integer> & r, vector<Integer> const & x, vector<Integer> const & y) noexcept
    // requires is_compact(x) && is_compact(y)
    // requires r.size() > x.size() + y.size()
    {
        auto rs = span( r.begin(), r.end() );
        auto xs = span( x.begin(), x.end() );
        auto ys = span( y.begin(), y.end() );
        product_accumulate(rs,xs,ys);
    }

    template <typename Integer>
    auto product (vector<Integer> const & x, vector<Integer> const & y) -> vector<Integer>
    // requires is_compact(x) && is_compact(y)
    {
        auto r = vector<Integer>( x.size() + y.size() + 1 );
        product_accumulate(r,x,y);
        return std::move(r);
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

    template <typename Integer>
    void square_accumulate (vector<Integer> & r, vector<Integer> const & x) noexcept
    // requires is_compact(x)
    // requires r.size() > x.size() + y.size()
    {
        auto rs = span( r.begin(), r.end() );
        auto xs = span( x.begin(), x.end() );
        square_accumulate(rs,xs);
    }

    template <typename Integer>
    auto square (vector<Integer> const & x) -> vector<Integer>
    // requires is_compact(x)
    {
        auto r = vector<Integer>( (2 * x.size()) + 1 );
        square_accumulate(r,x);
        return std::move(r);
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
