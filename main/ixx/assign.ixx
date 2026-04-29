// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
#include <span>

export module purple.arithmetics:assign;

import :word_concept;

namespace purple::arithmetics
{
    using std::size;
    using std::ranges::min;

    export
    template <Word W>
    void assign ( span<W> r, W x ) noexcept
    {
        auto const rz = size(r);

        if (0 < rz)
            r[0] = x;

        for (auto i = 1uz; i < rz; ++i)
            r[i] = W{0};
    }

    export
    template <Word W>
    auto assign ( span<W> r, span<W const> x ) noexcept
    {
        auto const rz = size(r);
        auto const xz = size(x);

        auto const z = min({ rz, xz });

        for (auto i = 0uz; i != z; ++i)
            r[i] = x[i];

        for (auto i = z; i < rz; ++i)
            r[i] = W{0};
    }
}
