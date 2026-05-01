// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <span>

export module purple.arithmetics:compact;

import :word_concept;

namespace purple::arithmetics
{
    using std::size;
    using std::span;

    export
    template <Word W>
    auto compact ( span<const W> x ) -> span<const W>
    {
        auto i = size(x);
        while ( i > 1 && is_zero( x[i-1] ) ) --i;
        return x.subspan(0,i);
    }
}
