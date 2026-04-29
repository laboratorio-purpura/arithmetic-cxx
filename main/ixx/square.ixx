// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
#include <array>
#include <cassert>
#include <span>
#include <tuple>

export module purple.arithmetics:square;

import :word_concept;

import :assign;
import :product;
import :sum;
import :twice;

namespace purple::arithmetics
{
    using std::array;
    using std::ignore;
    using std::ranges::max;
    using std::ranges::min;
    using std::span;
    using std::size;
    using std::tie;

    export
    template <Word W>
    void square_definition ( span<W> r, span<W const> x ) noexcept
    {
        product( r, x, x );
    }

    export
    template <Word W>
    void square_school ( span<W> r, span<W const> x ) noexcept
    {
        auto const rz = size(r);
        auto const xz = size(x);

        assign<W>( r, W{0} );

        // square x, word by word, propagating excess
        for (auto i = 0uz, i_ = min(rz,xz); i < i_ && i+i < rz; ++i)
        {
            auto excess = array<W,2>{};
            auto carry = W{};
            {
                // x[i] × x[i]
                auto p = product( x[i], x[i] );
                // store low word, carry high word
                tie( r[i+i], carry ) = sum( r[i+i], p[0], W{0} );
                tie( excess[0], ignore ) = sum( excess[0], p[1], carry );
            }
            for (auto j = i+1, j_ = min(rz,xz); j < j_ && i+j < rz; ++j)
            {
                // 2 × x[i] × x[j]
                auto p = product( x[i], x[j] );
                auto p2 = twice<W>( p, p, 1uz );
                // ( 2 × x[i] × x[j] ) + excess
                tie( p[0], carry ) = sum( p[0], excess[0], W{0} );
                tie( p[1], carry ) = sum( p[1], W{0}, carry );
                tie( p2, ignore ) = sum( p2, W{0}, carry );
                // store low word, carry high words
                tie( r[i+j], carry ) = sum( r[i+j], p[0], W{0} );
                tie( excess[0], carry ) = sum( excess[1], p[1], carry );
                tie( excess[1], ignore ) = sum( W{0}, p2, carry );
            }
            // store excess
            if (xz+i < rz) {
                tie( r[xz+i], carry ) = sum( r[xz+i], excess[0], W{0} );
            }
            if (xz+i+1 < rz) {
                tie( r[xz+i+1], ignore ) = sum( r[xz+i+1], excess[1], carry );
            }
        }
    }

    export
    template <Word W>
    void square ( span<W> r, span<W const> x ) noexcept
    {
        square_school( r, x );
    }
}
