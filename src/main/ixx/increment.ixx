// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
#include <span>
#include <tuple>

export module purple.arithmetic:increment;

import :word_concept;

namespace purple
{
    using std::size;
    using std::ranges::min;
    using std::tie;

    /// Computes the successor of an integer.
    ///
    /// Decrement stores into successor the size(successor) least significant words of the result.
    /// It permits aliasing successor to x, in which case it becomes "decrement accumulate".
    ///
    /// This implementation applies the "school" method described in Knuth, section 4.3.1.

    export
    template <Word W>
    auto increment ( span<W> successor, span<W const> x ) -> W
    {
        auto carry = W{0};

        auto const rz = size(successor);
        auto const xz = size(x);

        // count of result words to compute
        auto const z = min({ rz, xz });

        // increment x[0],
        // propagating carry
        if (z > 0)
            tie( successor[0], carry ) = next( x[0], carry );

        // propagate carry
        for (auto i = 1uz; i != z; ++i)
            tie( successor[i], carry ) = sum( x[i], W{0}, carry );

        return carry;
    }
}
