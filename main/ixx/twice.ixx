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
    auto twice ( span<W> r, span<W const> x, size_t y ) noexcept -> W
    {
        auto e = W{0};

        constexpr auto Bits = sizeof(W) * 8;

        auto const pz = size(r);
        auto const xz = size(x);

        // TODO: document this restriction
        y = min(y, Bits-1);

        // count of result words
        auto const z = min({ pz, xz });

        // double x, y times, word by word, propagating excess
        for (auto i = 0uz; i != z; ++i) {
            // x[i] × 2^y
            auto p = twice( x[i], y );
            // store low word with excess, forward high word
            r[i] = binary_or( p[0], e );
            e = p[1];
        }

        return e;
    }
}
