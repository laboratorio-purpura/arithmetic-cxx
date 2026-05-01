// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
#include <cassert>
#include <span>
#include <tuple>

export module purple.arithmetics:twice;

import :word_concept;
import :sum;

namespace purple::arithmetics
{
    using std::ignore;
    using std::size;
    using std::span;
    using std::ranges::min;
    using std::tie;

    /// Twice of nonnegative integer `x`, `y` times.
    ///
    /// Stores into `r` the `size(r)` least significant words of the result.
    /// Permits aliasing `r` to `x`, in which case it "accumulates" the result.
    /// Returns the "excess" of the top word of the result.
    ///
    /// This implementation applies the "shift" method.

    export
    template <Word W>
    auto twice ( span<W> product, span<W const> x, size_t y ) noexcept -> W
    {
        constexpr auto Bits = sizeof(W) * 8;

        auto excess = W{0};

        auto const pz = size(product);
        auto const xz = size(x);

        // TODO: document this restriction
        y = min(y, 63uz);

        // count of result words
        auto const z = min({ pz, xz });

        // double x, y times, word by word, propagating excess
        for (auto i = 0uz; i != z; ++i) {
            // x[i] × 2^y
            auto p = twice( x[i], y );
			// pull lower excess
            auto carry = W{0};
            tie( p[0], carry ) = sum( p[0], excess, W{0} );
            tie( p[1], ignore ) = sum( p[1], W{0}, carry );
            // store low word, propagate high word
            product[i] = p[0];
            excess = p[1];
        }

        return excess;
    }
}
