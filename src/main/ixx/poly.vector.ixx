module;

#include <span>
#include <tuple>
#include <vector>

export module purple:poly.vector;

import :poly;

using std::span;
using std::tuple;
using std::vector;

export namespace purple
{
    /// poly integer properties

    /// 1 if and only if every term of x is significant, else 0.
    template <typename Integer>
    auto is_compact (vector<Integer> const & x) -> Integer
    {
        auto xs = span( x.begin(), x.end() );
        return is_compact(xs);
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
    auto not_zero (vector<Integer> const & x) noexcept
    {
        auto xs = span( x.begin(), x.end() );
        return not_zero(xs);
    }

    /// poly integer arithmetic

    template <typename Integer>
    auto sum_accumulate (vector<Integer> & r, Integer y) noexcept -> Integer
    {
        auto rs = span( r.begin(), r.end() );
        return sum_accumulate(rs,y);
    }

    template <typename Integer>
    auto sum (vector<Integer> const & x, Integer y) -> vector<Integer>
    {
        auto const xz = x.size();
        auto r = vector<Integer>( xz + 1 );
        for (auto i = 0uz; i != xz; ++i) r[i] = x[i];
        auto rs = span( r.begin(), r.begin() + xz );
        r[xz] = sum_accumulate(rs,y);
        return std::move(r);
    }

    // --

    template <typename Integer>
    auto sum_accumulate (vector<Integer> & r, vector<Integer> const & y) noexcept -> Integer
    {
        auto rs = span( r.begin(), r.end() );
        auto ys = span( y.begin(), y.end() );
        return sum_accumulate(rs,ys);
    }

    template <typename Integer>
    auto sum (vector<Integer> const & x, vector<Integer> const & y) -> vector<Integer>
    {
        auto const xz = x.size();
        auto r = vector<Integer>( xz + 1 );
        for (auto i = 0uz; i != xz; ++i) r[i] = x[i];
        auto rs = span( r.begin(), r.begin() + xz );
        auto ys = span( y.begin(), y.end() );
        r[xz] = sum_accumulate(rs,ys);
        return std::move(r);
    }

    // --

    template <typename Integer>
    auto twice_accumulate (vector<Integer> & r, vector<Integer> const & x, unsigned N) noexcept -> Integer
    {
        auto rs = span( r.begin(), r.end() );
        auto xs = span( x.begin(), x.end() );
        twice_accumulate(rs,xs,N);
    }

    template <typename Integer>
    auto twice (vector<Integer> const & x, unsigned N = 1) -> vector<Integer>
    {
        auto const xz = x.size();
        auto r = vector<Integer>( xz + 1 );
        for (auto i = 0uz; i != xz; ++i) r[i] = x[i];
        auto rs = span( r.begin(), r.begin() + xz );
        r[xz] = twice_accumulate(rs,N);
        return std::move(r);
    }

    // --

    template <typename Integer>
    auto product_accumulate (vector<Integer> & r, vector<Integer> const & x, Integer y) noexcept -> Integer
    {
        auto rs = span( r.begin(), r.end() );
        auto xs = span( x.begin(), x.end() );
        return product_accumulate(rs,xs,y);
    }

    template <typename Integer>
    auto product (vector<Integer> const & x, Integer y) -> vector<Integer>
    {
        auto const xz = x.size();
        auto r = vector<Integer>( xz + 1 );
        for (auto i = 0uz; i != xz; ++i) r[i] = x[i];
        auto rs = span( r.begin(), r.begin() + xz );
        r[xz] = product_accumulate(rs,y);
        return std::move(r);
    }

    // --

    template <typename Integer>
    auto product_sum_accumulate (vector<Integer> & r, vector<Integer> const & x, vector<Integer> const & y) noexcept -> Integer
    {
        auto rs = span( r.begin(), r.end() );
        auto xs = span( x.begin(), x.end() );
        auto ys = span( y.begin(), y.end() );
        return product_sum_accumulate(rs,xs,ys);
    }

    template <typename Integer>
    auto product_sum (vector<Integer> const & x, vector<Integer> const & y, vector<Integer> const & z) -> vector<Integer>
    {
        auto xz = x.size();
        auto yz = y.size();
        auto zz = z.size();
        auto r = vector<Integer>( xz + yz + 1 );
        for (auto i = 0uz; i != zz; ++i) r[i] = z[i];
        auto rs = span( r.begin(), r.begin() + xz + yz );
        auto xs = span( x.begin(), x.end() );
        auto ys = span( y.begin(), y.end() );
        r[xz+yz] = product_sum_accumulate(rs,xs,ys);
        return std::move(r);
    }

    template <typename Integer>
    auto product (vector<Integer> const & x, vector<Integer> const & y) -> vector<Integer>
    {
        auto xz = x.size();
        auto yz = y.size();
        auto r = vector<Integer>( xz + yz + 1 );
        auto rs = span( r.begin(), r.begin() + xz + yz );
        auto xs = span( x.begin(), x.end() );
        auto ys = span( y.begin(), y.end() );
        r[xz+yz] = product_sum_accumulate(rs,xs,ys);
        return std::move(r);
    }

    template <typename Integer>
    auto square_sum_accumulate (vector<Integer> & r, vector<Integer> const & x) noexcept -> Integer
    {
        auto rs = span( r.begin(), r.end() );
        auto xs = span( x.begin(), x.end() );
        return square_sum_accumulate(rs,xs);
    }

    template <typename Integer>
    auto square (vector<Integer> const & x) -> vector<Integer>
    {
        auto xz = x.size();
        auto r = vector<Integer>( xz + xz + 1 );
        auto rs = span( r.begin(), r.begin() + xz + xz );
        auto xs = span( x.begin(), x.end() );
        r[xz+xz] = square_sum_accumulate(rs,xs);
        return std::move(r);
    }

    // --

    template <typename Integer>
    void ratio_accumulate (vector<Integer> & q, vector<Integer> & r, vector<Integer> const & x, Integer y) noexcept
    // requires is_compact(x)
    // requires not_zero(y)
    // requires q.size() >= x.size()
    // requires r.size() >= x.size()
    {
        auto qs = span( q.begin(), q.end() );
        auto rs = span( r.begin(), r.end() );
        auto xs = span( x.begin(), x.end() );
        ratio_accumulate(qs,rs,xs,y);
    }

    template <typename Integer>
    void quotient_accumulate (vector<Integer> & q, vector<Integer> const & x, Integer y) noexcept
    // requires is_compact(x)
    // requires not_zero(y)
    // requires q.size() >= x.size()
    {
        auto qs = span( q.begin(), q.end() );
        auto xs = span( x.begin(), x.end() );
        quotient_accumulate(qs,xs,y);
    }

    template <typename Integer>
    auto quotient (vector<Integer> const & x, Integer y) -> vector<Integer>
    // requires is_compact(x)
    // requires not_zero(y)
    {
        auto q = vector<Integer>( x.size() );
        quotient_accumulate(q,x,y);
        return std::move(q);
    }

    template <typename Integer>
    void remainder_accumulate (vector<Integer> & r, vector<Integer> const & x, Integer y) noexcept
    // requires is_compact(x)
    // requires not_zero(y)
    // requires r.size() >= x.size()
    {
        auto rs = span( r.begin(), r.end() );
        auto xs = span( x.begin(), x.end() );
        remainder_accumulate(rs,xs,y);
    }

    template <typename Integer>
    auto remainder (vector<Integer> const & x, Integer y) -> vector<Integer>
    // requires is_compact(x)
    // requires not_zero(y)
    {
        auto r = vector<Integer>( x.size() );
        remainder_accumulate(r,x,y);
        return std::move(r);
    }
}
