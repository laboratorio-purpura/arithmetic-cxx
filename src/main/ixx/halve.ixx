// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
#include <cassert>
#include <span>
#include <tuple>

export module purple.arithmetic:halve;

import :word_concept;

namespace purple
{
    using std::ignore;
    using std::size;
    using std::ranges::min;
    using std::tie;

    /// Computes half (to a power) of an integer.
    ///
    /// Halve stores into quotient the size(quotient) least significant words of the result.
    /// It permits aliasing quotient to x, in which case it becomes "halve accumulate".
    ///
    /// This implementation applies the "binary shift" method.

    export
    template <Word W>
    auto halve ( span<W> quotient, span<W const> x, size_t y ) noexcept -> W
    {
        constexpr auto Bits = sizeof(W) * 8;

        auto remainder = W{0};

        auto const pz = size(quotient);
        auto const xz = size(x);

        // TODO: lift this restriction
        assert(y < Bits);

        // count of result words to compute
        auto const z = min({ pz, xz });

        // halve word by word,
        // from most to least significant,
        // propagating remainder<
        for (auto i = z; i > 0; --i)
        {
    		// x[i] ÷ 2^y
            auto [ q, r ] = half( x[i-1], y );
            // store quotient, propagate remainder
            tie( quotient[i-1], ignore ) = sum( q, remainder, W{0} );
            tie( remainder, ignore ) = twice( r, Bits - y );
        }

        return remainder;
    }
}