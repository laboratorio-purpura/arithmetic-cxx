// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
#include <array>
#include <span>
#include <tuple>

export module purple.arithmetics:product;

import :sum;
import :word_concept;

namespace purple::arithmetics
{
    using std::array;
    using std::ignore;
    using std::ranges::max;
    using std::ranges::min;
    using std::span;
    using std::size;
    using std::tie;

    /// Product of nonnegative integers `x` and `y`.
    ///
    /// Stores into `r` the `size(r)` least significant words of the result.
    /// Permits aliasing `r` to `x`, in which case it "accumulates" the result.
    ///
    /// This implementation applies the "school" method described in Knuth, section 4.3.1.

    export
    template <Word W>
    auto product ( span<W> r, span<W const> x, W y ) noexcept -> W
    {
        auto excess = W{0};

        auto const rz = size(r);
        auto const xz = size(x);

        // count of result words to compute
        auto const z = min( rz, xz );

        // multiply x and y, word by word, propagating excess
        for (auto i = 0uz; i < z; ++i)
        {
            // x[i] × y + excess
            auto p = product( x[i], y );
            auto carry = W{0};
            tie( p[0], carry ) = sum( p[0], excess, W{0} );
            tie( p[1], ignore ) = sum( p[1], W{0}, carry );
            // store low word, propagate high word
            r[i] = p[0];
            excess = p[1];
        }

        return excess;
    }

    /// Product of nonnegative integers `x` and `y`.
    ///
    /// Stores into `r` the `size(r)` least significant words of the result.
    /// Permits aliasing `r` to `x`, in which case it "accumulates" the result.
    ///
    /// This implementation applies the "convolution" method.

    export
    template <Word W>
    void product_convolve ( span<W> r, span<W const> x, span<W const> y )
    {
        auto const rz = size(r);
        auto const xz = size(x);
        auto const yz = size(y);

        // compute result, word by word
        array<W,2> excess {};
        for (auto k = 0uz; k < rz; ++k)
        {
            // let i, j that i + j = k
            auto const s = max( 0z, ssize_t(k) - ssize_t(yz) + 1z );
            auto const e = min( ssize_t(k) + 1z, ssize_t(xz) );
            // Σ x[i] × y[k-i]
            array<W,3> sigma { excess[0], excess[1], W{0} };
            for (auto i = s; i < e; ++i)
            {
                // x[i] × y[k-i]
                auto p = product( x[i], y[k-i] );
                ignore = sum<W>( sigma, sigma, p );
            }
            // store low word, propagate high words
            r[k] = sigma[0];
            excess = { sigma[1], sigma[2] };
        }
    }

    /// Product of nonnegative integers `x` and `y`.
    ///
    /// Stores into `r` the `size(r)` least significant words of the result.
    /// Permits aliasing `r` to `x`, in which case it "accumulates" the result.
    ///
    /// This implementation applies the "school" method described in Knuth, section 4.3.1.

    export
    template <Word W>
    void product_school ( span<W> r, span<W const> x, span<W const> y ) noexcept
    {
        auto const rz = size(r);
        auto const xz = size(x);
        auto const yz = size(y);

        assign<W>( r, W{0} );

        // multiply x and y, word by word, propagating excess
        for (auto j = 0uz, j_ = min(rz,yz); j < j_; ++j)
        {
            auto excess = W{0};
            auto carry = W{0};
            // x × y[j]
            for (auto i = 0uz, i_ = min(rz,xz); i < i_ && i+j < rz; ++i)
            {
                // x[i] × y[j]
                auto p = product( x[i], y[j] );
                // x[i] × y[j] + excess
                tie( p[0], carry ) = sum( p[0], excess, W{0} );
                tie( p[1], ignore ) = sum( p[1], W{0}, carry );
                // store low word, carry high word
                tie( r[i+j], carry ) = sum( r[i+j], p[0], W{0} );
                tie( excess, ignore ) = sum( p[1], W{0}, carry );
            }
            // store excess
            if (xz+j < rz) {
                tie( r[xz+j], ignore ) = sum( r[xz+j], excess, W{0} );
            }
        }
    }

    /// Product of nonnegative integers `x` and `y`.
    ///
    /// Stores into `r` the `size(r)` least significant words of the result.
    /// Permits aliasing `r` to `x`, in which case it "accumulates" the result.

    export
    template <Word W>
    void product ( span<W> r, span<W const> x, span<W const> y ) noexcept
    {
        product_convolve( r, x, y );
    }
}
