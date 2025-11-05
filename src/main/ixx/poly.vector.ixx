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

/// Multi-degree arithmetic.

export namespace purple
{
    /// Expansion operators.

    template <typename Word>
    auto sum (vector<Word> const & x, Word y) -> vector<Word>
    {
        auto const xz = x.size();
        auto r = vector<Word>( xz + 1 );
        for (auto i = 0uz; i != xz; ++i) r[i] = x[i];
        auto rs = span( r.begin(), r.begin() + xz );
        r[xz] = sum_assign<Word>(rs,y);
        return std::move(r);
    }

    template <typename Word>
    auto sum (vector<Word> const & x, vector<Word> const & y) -> vector<Word>
    {
        auto const xz = x.size();
        auto r = vector<Word>( xz + 1 );
        for (auto i = 0uz; i != xz; ++i) r[i] = x[i];
        auto rs = span( r.begin(), r.begin() + xz );
        auto ys = span( y.begin(), y.end() );
        r[xz] = sum_assign<Word>(rs,ys);
        return std::move(r);
    }

    template <typename Word>
    auto twice (vector<Word> const & x, unsigned N = 1) -> vector<Word>
    {
        auto const xz = x.size();
        auto r = vector<Word>( xz + 1 );
        for (auto i = 0uz; i != xz; ++i) r[i] = x[i];
        auto rs = span( r.begin(), r.begin() + xz );
        r[xz] = twice_assign(rs,N);
        return std::move(r);
    }

    template <typename Word>
    auto product (vector<Word> const & x, Word y) -> vector<Word>
    {
        auto const xz = x.size();
        auto r = vector<Word>( xz + 1 );
        for (auto i = 0uz; i != xz; ++i) r[i] = x[i];
        auto rs = span( r.begin(), r.begin() + xz );
        r[xz] = product_assign(rs,y);
        return std::move(r);
    }

    template <typename Word>
    auto product_sum (vector<Word> const & x, vector<Word> const & y, vector<Word> const & z) -> vector<Word>
    {
        auto xz = x.size();
        auto yz = y.size();
        auto zz = z.size();
        auto r = vector<Word>( xz + yz + 1 );
        for (auto i = 0uz; i != zz; ++i) r[i] = z[i];
        auto rs = span( r.begin(), r.begin() + xz + yz );
        auto xs = span( x.begin(), x.end() );
        auto ys = span( y.begin(), y.end() );
        r[xz+yz] = product_sum_assign(rs,xs,ys);
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
        r[xz+yz] = product_accumulate(rs,xs,ys);
        return std::move(r);
    }

    template <typename Integer>
    auto square (vector<Integer> const & x) -> vector<Integer>
    {
        auto xz = x.size();
        auto r = vector<Integer>( xz + xz + 1 );
        auto rs = span( r.begin(), r.begin() + xz + xz );
        auto xs = span( x.begin(), x.end() );
        r[xz+xz] = square_accumulate(rs,xs);
        return std::move(r);
    }

    /// Reduction operators.

    template <typename Integer>
    auto difference (vector<Integer> const & x, vector<Integer> const & y) -> vector<Integer>
    // requires x > y
    {
        auto const xz = x.size();
        auto r = vector<Integer>( xz + 1 );
        for (auto i = 0uz; i != xz; ++i) r[i] = x[i];
        auto rs = span( r.begin(), r.begin() + xz );
        auto ys = span( y.begin(), y.end() );
        ignore = difference_assign<Integer>(rs,ys);
        return std::move(r);
    }
}
