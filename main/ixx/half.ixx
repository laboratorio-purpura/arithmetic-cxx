// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
#include <cassert>
#include <span>
#include <tuple>

export module purple.arithmetics:half;

import :word_concept;
import :sum;

namespace purple::arithmetics
{
    using std::ignore;
    using std::size;
    using std::span;
    using std::ranges::min;
    using std::tie;

    /// Division of nonnegative integers `x` by 2, `y` times.
    ///
    /// Stores into `q` the `size(q)` least significant words of the quotient.
    /// Returns the remainder.
    ///
    /// This implementation applies the "shift" method.

    export
    template <Word W>
    auto half ( span<W> quotient, span<W const> x, size_t y ) noexcept -> W
    {
        constexpr auto Bits = sizeof(W) * 8;

        auto remainder = W{0};

        auto const pz = size(quotient);
        auto const xz = size(x);

        // TODO: lift this restriction
        assert(y < Bits);

        // count of result words to compute
        auto const z = min({ pz, xz });

        // halve x, y times, word by word, propagating remainder
        if (xz > z)
        {
            // x[i] ÷ 2^y
            auto [ _, r ] = half( x[z], y );
            // propagate remainder
            tie( remainder, ignore ) = twice( r, Bits - y );
        }
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
