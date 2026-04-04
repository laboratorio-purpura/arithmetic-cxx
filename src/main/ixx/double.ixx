// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
#include <cassert>
#include <span>
#include <tuple>

export module purple.arithmetic:double_;

import :word_concept;

namespace purple
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
    auto double_ ( span<W> product, span<W const> x, size_t y ) noexcept -> W
    {
        constexpr auto Bits = sizeof(W) * 8;

        auto excess = W{0};

        auto const pz = size(product);
        auto const xz = size(x);

        // TODO: lift this restriction
        assert(y < Bits);

        // count of result words to compute
        auto const z = min({ pz, xz });

        // double word by word,
        // from least to most significant,
        // propagating excess
        for (auto i = 0uz; i != z; ++i)
            tie( product[i], excess ) = twice( x[i], y, excess );

        return excess;
    }

}
