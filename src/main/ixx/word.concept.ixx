// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <cstdlib>
#include <tuple>

/// Singular nonnegative integer arithmetics.
///
/// This module partition defines `concept Word` representing words in a binary base.
///
/// The Word interface is intended to represent efficient primitive procedures,
/// appropriate for the definition of "long" arithmetics,
/// such as "carrying" instead of "overflowing".
///
/// We purposefully avoid overloading C++ operators.

export module purple.arithmetic:word_concept;

using namespace std;

namespace purple
{
    export
    template <typename W>
    concept Word = requires (W & r, W w, array<W,2> w2, size_t z)
    {
        /// Construction and assignment.

        requires default_initializable<W>;
        requires copyable<W>;

        w = W(0u);
        w = W{0u};
        w = W(1u);
        w = W{1u};

        /// Query procedures.

        { leading_zero_bits(w) } noexcept -> convertible_to<size_t>;
        { trailing_zero_bits(w) } noexcept -> convertible_to<size_t>;

        /// Test procedures.

        { is_zero(w) } noexcept -> convertible_to<bool>;
        { not_zero(w) } noexcept -> convertible_to<bool>;

        { is_odd(w) } noexcept -> convertible_to<bool>;
        { not_odd(w) } noexcept -> convertible_to<bool>;

        { is_normalized(w) } noexcept -> convertible_to<bool>;
        { not_normalized(w) } noexcept -> convertible_to<bool>;

        /// Comparison procedures.

        { are_equal(w,w) } noexcept -> convertible_to<bool>;
        { not_equal(w,w) } noexcept -> convertible_to<bool>;

        { is_smaller(w,w) } noexcept -> convertible_to<bool>;
        { not_smaller(w,w) } noexcept -> convertible_to<bool>;

        { is_greater(w,w) } noexcept -> convertible_to<bool>;
        { not_greater(w,w) } noexcept -> convertible_to<bool>;

        /// Expand procedures.

        { next(w,w) } noexcept -> same_as< array<W,2> >;
        { sum(w,w,w) } noexcept -> same_as< array<W,2> >;
        { product(w,w,w) } noexcept -> same_as< array<W,2> >;
        { twice(w,z,w) } noexcept -> same_as< array<W,2> >;

        /// Reduce procedures.

        { previous(w,w) } noexcept -> same_as< array<W,2> >;
        { difference(w,w,w) } noexcept -> same_as< array<W,2> >;
        { division(w,w) } noexcept -> same_as< tuple<W,W> >;
        { division(w2,w) } noexcept -> same_as< tuple<array<W,2>,W> >;
        { half(w,z) } noexcept -> same_as< tuple<W,W> >;
    };
}