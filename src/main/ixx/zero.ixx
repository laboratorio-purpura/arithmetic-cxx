// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <span>

export module purple.arithmetic:zero;

import :word_concept;

export namespace purple
{
    /// Tests if zero.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto is_zero ( span<W const,Bi> x ) noexcept -> bool
    {
        auto r = true;
        r = r && is_zero( x[0] );
        r = r && is_zero( x[1] );
        return r;
    }

    /// Tests if zero.

    template <Word W>
    auto is_zero ( span<W const> x ) noexcept -> bool
    {
        bool r = true;
        auto const xz = x.size();
        for (auto i = 0u; i != xz; ++i)
            r = r && is_zero(x[i]);
        return r;
    }

    /// Tests if *not* zero.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto not_zero ( span<W const,Bi> x ) noexcept -> bool
    {
        auto r = false;
        r = r || not_zero( x[0] );
        r = r || not_zero( x[1] );
        return r;
    }

    /// Tests if *not* zero.

    template <Word W>
    auto not_zero ( span<W const> x ) noexcept -> bool
    {
        bool r = false;
        auto const xz = x.size();
        for (auto i = 0u; i != xz; ++i)
            r = r || not_zero(x[i]);
        return r;
    }
}
