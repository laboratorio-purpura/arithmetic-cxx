// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <span>
#include <tuple>
#include <vector>

export module purple.arithmetic:poly.vector;

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
    auto sum (vector<Integer> const & x, Integer y) -> vector<Integer>
    {
        auto const xz = x.size();
        auto r = vector<Integer>( xz + 1 );
        for (auto i = 0uz; i != xz; ++i) r[i] = x[i];
        auto rs = span( r.begin(), r.begin() + xz );
        r[xz] = sum_accumulate<Integer>(rs,y);
        return std::move(r);
    }

    template <typename Integer>
    auto sum (vector<Integer> const & x, vector<Integer> const & y) -> vector<Integer>
    {
        auto const xz = x.size();
        auto r = vector<Integer>( xz + 1 );
        for (auto i = 0uz; i != xz; ++i) r[i] = x[i];
        auto rs = span( r.begin(), r.begin() + xz );
        auto ys = span( y.begin(), y.end() );
        r[xz] = sum_accumulate<Integer>(rs,ys);
        return std::move(r);
    }

    template <typename Integer>
    auto difference (vector<Integer> const & x, vector<Integer> const & y) -> vector<Integer>
    {
        auto const xz = x.size();
        auto r = vector<Integer>( xz + 1 );
        for (auto i = 0uz; i != xz; ++i) r[i] = x[i];
        auto rs = span( r.begin(), r.begin() + xz );
        auto ys = span( y.begin(), y.end() );
        auto carry = difference_accumulate<Integer>(rs,ys);
        // TODO: if carry...
        return std::move(r);
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
    auto square (vector<Integer> const & x) -> vector<Integer>
    {
        auto xz = x.size();
        auto r = vector<Integer>( xz + xz + 1 );
        auto rs = span( r.begin(), r.begin() + xz + xz );
        auto xs = span( x.begin(), x.end() );
        r[xz+xz] = square_sum_accumulate(rs,xs);
        return std::move(r);
    }
}
