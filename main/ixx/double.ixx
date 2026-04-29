// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
#include <cassert>
#include <span>
#include <tuple>

export module purple.arithmetics:double_;

import :word_concept;

namespace purple::arithmetics
{
    using std::size;
    using std::ranges::min;
    using std::tie;

    /// Computes twice (to a power) of an integer.
    ///
    /// Double adds into product the size(product) least significant words of the result.
    /// It permits aliasing product to x, in which case it becomes "double accumulate".
    ///
    /// This implementation applies the "binary shift" method.

    export
    template <Word W>
    auto twice ( span<W> product, span<W const> x, size_t y ) noexcept -> W
    {
        constexpr auto Bits = sizeof(W) * 8;

        auto excess = W{0};

        auto const pz = size(product);
        auto const xz = size(x);

        // TODO: lift this restriction
        assert(y < Bits);

        // count of result words to compute
        auto const z = min({ pz, xz });

        // double x, y times, word by word, propagating excess
        for (auto i = 0uz; i != z; ++i) {
            auto carry = W{0};
            // x[i] × 2^y + excess
            auto p = twice( x[i], y );
            tie( p[0], carry ) = sum( p[0], excess, W{0} );
            tie( p[1], ignore ) = sum( p[1], W{0}, carry );
            // store low word, propagate high word
            product[i] = p[0];
            excess = p[1];
        }

        return excess;
    }
}
