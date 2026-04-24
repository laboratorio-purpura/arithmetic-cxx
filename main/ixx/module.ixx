// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

/// Multi-word nonnegative integer arithmetics.
///
/// Let a *word* be a nonnegative integer in a binary base B.
///
/// Let a *multi-word integer* be an integer represented with a variable quantity of words.
///
/// This module defines procedures with multi-word integer operands.
///
/// Requires, unless otherwise specified:
/// size(operand) ≥ 1

export module purple.arithmetics;

// words.

export import :word_concept;
export import :word_type;

// properties.

export import :odd;
export import :normal;
export import :zero;

// relations.

export import :equal;
export import :greater;
export import :smaller;

// transformations.

export import :reciprocal;

// operations.

export import :add;
export import :multiply;
export import :double_;
export import :square;

export import :subtract;
export import :divide;
export import :halve;
