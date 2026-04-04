// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
#include <span>
#include <tuple>

export module purple.arithmetic:decrement;

import :word_concept;

namespace purple
{
    using std::size;
    using std::ranges::min;
    using std::tie;

    /// Computes the predecessor of an integer.
    ///
    /// Decrement stores into predecessor the size(predecessor) least significant words of the result.
    /// It permits aliasing predecessor to x, in which case it becomes "decrement accumulate".
    ///
    /// This implementation applies the "school" method described in Knuth, section 4.3.1.

    export
    template <Word W>
    auto decrement ( span<W> predecessor, span<W const> x ) -> W
    {
        auto borrow = W{0};

        auto const rz = size(predecessor);
        auto const xz = size(x);

        // count of result words to compute
        auto const z = min({ rz, xz });

        // decrement x[0],
        // propagating borrow
        if (z > 0)
            tie( predecessor[0], borrow ) = previous( x[0], borrow );

        // propagate borrow
        for (auto i = 1uz; i != z; ++i)
            tie( predecessor[i], borrow ) = difference( x[i], W{0}, borrow );

        return borrow;
    }
}
