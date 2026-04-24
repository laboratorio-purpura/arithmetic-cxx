// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
#include <cassert>
#include <span>
#include <tuple>

export module purple.arithmetics:multiply;

import :word_concept;

namespace purple::arithmetics
{
    using std::ignore;
    using std::ranges::max;
    using std::ranges::min;
    using std::span;
    using std::size;
    using std::tie;

    /// Computes the product of two integers.
    ///
    /// Multiply stores into product the size(product) least significant words of the result.
    ///
    /// This implementation applies the "school" method described in Knuth, section 4.3.1.

    export
    template <Word W>
    auto multiply ( span<W> product, span<W const> x, W y ) noexcept -> W;

    template <Word W>
    auto multiply ( span<W> r, span<W const> x, W y ) noexcept -> W
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

    /// Computes the product of two integers.
    ///
    /// Multiply stores into r the size(r) least significant words of the result.
    ///
    /// This implementation applies the "convolution" method.

    export
    template <Word W>
    void multiply ( span<W> r, span<W const> x, span<W const> y )
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
                ignore = add<W>( sigma, sigma, p );
            }
            // store low word, propagate high word
            r[k] = sigma[0];
            excess = { sigma[1], sigma[2] };
        }
    }

    /// Computes the product of two integers.
    ///
    /// Multiply stores into r the size(r) least significant words of the result.
    ///
    /// This implementation applies the "school" method described in Knuth, section 4.3.1.

    export
    template <Word W>
    void multiply_school ( span<W> r, span<W const> x, span<W const> y ) noexcept
    {
        auto const rz = size(r);
        auto const xz = size(x);
        auto const yz = size(y);

        // multiply x and y, word by word, propagating excess
        for (auto j = 0uz, j_ = min(rz,yz); j < j_; ++j)
        {
            auto excess = W{0};
            // x × y[j]
            for (auto i = 0uz, i_ = min(rz,xz); i < i_ && i+j < rz; ++i)
            {
                auto carry = W{0};
                // x[i] × y[j] + excess
                auto p = product( x[i], y[j] );
                tie( p[0], carry ) = sum( p[0], excess, W{0} );
                tie( p[1], ignore ) = sum( p[1], W{0}, carry );
                // store low word, propagate high word
                tie( r[i+j], carry ) = sum( r[i+j], p[0], W{0} );
                tie( excess, ignore ) = sum( p[1], W{0}, carry );
            }
            // transfer excess
            if (xz+j < rz) {
                tie( r[xz+j], ignore ) = sum( r[xz+j], excess, W{0} );
            }
        }
    }
}
