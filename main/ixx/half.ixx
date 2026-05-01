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

    /// Half of nonnegative integer `x`, `y` times.
    ///
    /// Stores into `q` the `size(q)` least significant words of the quotient.
    /// Returns the remainder.
    ///
    /// This implementation applies the "shift" method.

    export
    template <Word W>
    auto half ( span<W> q, span<W const> x, size_t y ) noexcept -> W
    {
        auto r = W{0};

        constexpr auto Bits = sizeof(W) * 8;

        auto const qz = size(q);
        auto const xz = size(x);

        // TODO: document this restriction
        y = min(y, Bits-1);

        // count of result words
        auto const z = min({ qz, xz });

        // halve x, y times, word by word, propagating remainder
        if (xz > z)
        {
            // x[z] ÷ 2^y
            // forward remainder
            tie( ignore, r ) = half( x[z], y );
        }
        for (auto i = z; i > 0; --i)
        {
    		// x[i] ÷ 2^y
            auto [ q_, r_ ] = half( x[i-1], y );
            // store quotient with lower remainder
            auto [ t, _ ] = twice( r, Bits - y );
            q[i-1] = binary_or( q_, t );
            // forward current remainder
            r = r_;
        }

        return r;
    }
}
