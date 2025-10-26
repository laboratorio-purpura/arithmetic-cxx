// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <cassert>
#include <span>
#include <tuple>
#include <vector>

export module purple.arithmetic.alternative:poly;

import purple.arithmetic;

import :duo;

using std::array;
using std::ignore;
using std::size_t;
using std::span;
using std::tie;
using std::vector;

/// Multi-degree nonnegative integer arithmetics.

export namespace purple
{
    /// Expansion procedures.

    /// Product.
    ///
    /// Computes v = v + ( x × y ).
    ///
    /// Computes by definition: successive addition.

    template <typename Integer>
    void product_v0 ( span<Integer> v, span<Integer const> x, span<Integer const> y )
    // requires degree(v) ≥ degree(x) + degree(y) + 1
    {
        auto carry = Integer(0);
        // t ← 0
        auto t = vector(y.begin(),y.end());
        // while t ≠ 0:
        while ( not_zero<unsigned>( t ) ) {
            // x ← x + x
            carry = sum_assign<unsigned>( v, x, carry );
            // t ← t - 1
            ignore = previous_assign<unsigned>( t );
        }
        // terminate:
        // v = v + ( x × y )
    }

    /// Reduction procedures.

    /// Division, quotient and remainder.
    ///
    /// Computes q = x ÷ y, r = x % y.
    ///
    /// Computes by definition: successive subtraction.

    template <typename Integer>
    void division_v0 ( span<Integer> q, span<Integer> r, span<Integer const> x, span<Integer const> y )
    // requires degree(x) ≥ degree(y) ≥ 1
    // requires degree(q) ≥ degree(x)
    // requires degree(r) ≥ degree(x)
    // requires not_zero(y)
    {
        // q ← 0
        for (auto i = 0uz; i != q.size(); ++i) q[i] = 0u;
        // r ← x
        for (auto i = 0uz; i != x.size(); ++i) r[i] = x[i];
        for (auto i = x.size(); i != r.size(); ++i) r[i] = 0u;
        // while r ≥ y:
        while ( not_smaller<Integer>( r, y ) ) {
            // q ← q + 1
            ignore = next_assign<Integer>( q );
            // r ← r - y
            ignore = difference_assign<Integer>( r, y );
        }
        // terminate:
        // q = x ÷ y
        // r = x % y
    }

    /// Restricted division with remainder.
    ///
    /// Requirements:
    /// degree(x) = degree(y) + 1
    /// degree(y) ≥ 1
    /// x ÷ B < y
    /// y is normalized
    /// iy = reciprocal_normalized(y)

    template <typename Integer>
    void division_v1_restricted_accumulate ( Integer & q, span<Integer> x, span<Integer const> y, Integer iy )
    {
        assert( x.size() == y.size() + 1 );
        assert( is_normalized<Integer>(y) == 1 );
        assert( is_smaller<Integer>( x.subspan(1), y ) == 1 );

        // guess the quotient.
        // we guess by dividing 2-by-1 the most significant words.
        // the restriction on the operands guarantee a strict bound on the error.
        // we detect the error and fix the quotient.

        // let N = degree(y)
        auto N = y.size();

        // compute tentative quotient q'
        auto q_ = Integer(0);
        auto r_ = Integer(0);
        {
            // let x' = { x[N-1}, x[N] }
            auto x_ = array { x[N-1], x[N] };
            // let y' = y[N-1]
            auto y_ = y[N-1];
            // let q' = x' ÷ y'
            // let r' = x' % y'
            tie( q_, r_ ) = division_normalized<Integer,2>( x_, y_, iy );
        }
        // invariant: q' - 2 ≤ q ≤ q'

        // fix q' > q
        {
            auto carry = Integer(0);
            while (
                // r_ < B
                (carry == 0) &&
                // q' × y[N-2] > { x[N-2], r' }
                is_greater<Integer>( product( q_, y[N-2] ), array { x[N-2], r_ } )
            ) {
                // q_ ← q_ - 1
                q_ = q_ - 1;
                // r_ ← r_ + y[N-1]
                carry = sum_assign( r_, y[N-1] );
                // carry = 1 ⇒ r_ ≥ B
            }
        }
        // invariant: q' - 1 ≤ q ≤ q'

        // compute remainder
        // r ← x - q' × y
        auto borrow = Integer(0);
        {
            // let t = q' × y
            Integer t_ [ y.size() + 1 ];
            auto t = span<Integer>( t_, y.size() + 1 );
            assign( t, y );
            ignore = product_assign<Integer>( t, q_ );
            // r ← x - t
            borrow = difference_assign<Integer>( x, t );
        }
        // borrow = 1 ⇒ r < 0

        // fix r < 0 ⇒ q' > q
        if ( borrow == 1 ) {
            // q ← q - 1
            q_ = q_ - 1;
            // r ← r + y
            ignore = sum_assign<Integer>( x, y );
        }
        // invariant: q' = q

        // terminate
        q += q_;
    }

    /// Normalized division with remainder.
    ///
    /// Requirements:
    /// degree(x) > degree(y)
    /// x ÷ B < y
    /// y is normalized

    template <typename Integer>
    void division_v1_normalized_accumulate ( span<Integer> q, span<Integer> x, span<Integer const> y )
    {
        assert( q.size() >= y.size() );
        assert( x.size() > y.size() );
        assert( y.size() > 1 );
        assert( is_normalized<Integer>(y) );

        // compute each quotient word one by one,
        // computing each by division of N + 1 dividend words by divisor.
        // normalization of operands is key.

        // let m <- degree(x) - degree(y)
        auto M = x.size() - y.size();
        // let n < degree(y)
        auto N = y.size();
        // invariant: m ≥ 1

        auto iy = reciprocal_normalized( y[N-1] );

        // for j from m to 0 excluding:
        for (auto j = M; j > 0; --j)
        {
            assert( is_smaller<Integer>( x.subspan( j,N ), y ) );

            // let x' = { x[j+n], x[j+n-1}, ..., x[j] }
            auto x_ = x.subspan( j-1, N+1 );

            division_v1_restricted_accumulate<Integer>( q[j-1], x_, y, iy );
        }
    }

    /// Division with remainder.
    ///
    /// Requirements:
    /// degree(x) ≥ degree(y)
    /// y is nonzero

    template <typename Integer>
    void division_v1 ( span<Integer> q, span<Integer> r, span<Integer const> x, span<Integer const> y )
    {
        assert( x.size() >= y.size() );
        assert( y.size() > 1 );
        assert( not_zero<Integer>(y) );
        assert( q.size() >= x.size() );
        assert( r.size() >= x.size() + 1 );

        // normalize the operands, then divide with the normalized algorithm.

        // let N = degree(y)
        auto const N = y.size();

        // 1. find normalization factor.
        auto factor = leading_zero_bits( y[N-1] );

        // 2. normalize dividend and divisor.
        Integer ny_ [N];
        auto ny = span<Integer>( ny_, N );
        assign( ny, y );
        ignore = twice_assign<Integer>( ny, factor );

        assign( r, x );
        ignore = twice_assign<Integer>( r, factor );

        auto r_ = r.subspan( r.size() - N, N );
        if ( not_smaller<Integer>( r_, y ) ) {
            q[N] += Integer(1);
            ignore = difference_assign<Integer>( r_, y );
        }

        // 3. divide with normalized operands.
        division_v1_normalized_accumulate<Integer>( q, r, ny );

        // 4. denormalize remainder.
        ignore = half_assign( r, factor );
    }
}