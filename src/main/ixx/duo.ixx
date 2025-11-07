// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <span>
#include <tuple>

export module purple.arithmetic:duo;

import :mono;

using std::array;
using std::ignore;
using std::size_t;
using std::span;
using std::tie;
using std::tuple;

/// Bi-degree nonnegative integer arithmetics.
///
/// Let a *word* be a nonnegative integer in base B.
/// In a binary machine, B = 2 ^ bits.
///
/// By bi-degree integers we mean integers represented with two words.
///
/// This module partition defines procedures with bi-degree integer operands.

export namespace purple
{
    /// Tests.

    /// Tests if zero.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto is_zero ( span<Word const,Bi> x ) noexcept -> bool
    {
        auto r = true;
        r = r && is_zero( x[0] );
        r = r && is_zero( x[1] );
        return r;
    }

    /// Tests if *not* zero.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto not_zero ( span<Word const,Bi> x ) noexcept -> bool
    {
        auto r = false;
        r = r || not_zero( x[0] );
        r = r || not_zero( x[1] );
        return r;
    }

    /// Tests if odd.
    ///
    /// In a binary machine, this means the least significant bit is 1.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto is_odd ( span<Word const,Bi> x ) noexcept -> bool
    {
        return is_odd( x[0] );
    }

    /// Tests if *not* odd.
    ///
    /// In a binary machine, this means the least significant bit is 0.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto not_odd ( span<Word const,Bi> x ) noexcept -> bool
    {
        return not_odd( x[0] );
    }

    /// Tests if normalized.
    ///
    /// Normalized means B ÷ 2 ≤ y[1] < B.
    ///
    /// In a binary machine, this means the most significant bit is 1.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto is_normalized ( span<Word const,Bi> y ) noexcept -> bool
    {
        return is_normalized( y[1] );
    }

    /// Tests if *not* normalized.
    ///
    /// Normalized means B ÷ 2 ≤ y[1] < B.
    ///
    /// In a binary machine, this means the most significant bit is 0.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto not_normalized ( span<Word const,Bi> y ) noexcept -> bool
    {
        return not_normalized( y[1] );
    }

    /// Tests if smaller.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto is_smaller ( span<Word const,Bi> x, span<Word const,Bi> y ) noexcept -> bool
    {
        auto carry = Word(0);
        tie( ignore, carry ) = difference( x[0], y[0], carry );
        tie( ignore, carry ) = difference( x[1], y[1], carry );
        return carry == 1;
    }

    /// Tests if *not* smaller.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto not_smaller ( span<Word const,Bi> x, span<Word const,Bi> y ) noexcept -> bool
    {
        auto carry = Word(0);
        tie( ignore, carry ) = difference( x[0], y[0], carry );
        tie( ignore, carry ) = difference( x[1], y[1], carry );
        return carry == 0;
    }

    /// Tests if greater.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto is_greater ( span<Word const,Bi> x, span<Word const,Bi> y ) noexcept -> Word
    {
        auto carry = Word(0);
        tie( ignore, carry ) = difference( y[0], x[0], carry );
        tie( ignore, carry ) = difference( y[1], x[1], carry );
        return carry == 1;
    }

    /// Tests if *not* greater.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto not_greater ( span<Word const,Bi> x, span<Word const,Bi> y ) noexcept -> bool
    {
        auto carry = Word(0);
        tie( ignore, carry ) = difference( y[0], x[0], carry );
        tie( ignore, carry ) = difference( y[1], x[1], carry );
        return carry == 0;
    }

    /// Tests if equal.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto is_equal ( span<Word const,Bi> x, span<Word const,Bi> y ) noexcept -> bool
    {
        bool r = true;
        r = r && is_equal( x[0], y[0] );
        r = r && is_equal( x[1], y[1] );
        return r;
    }

    /// Tests if *not* equal.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto not_equal ( span<Word const,Bi> x, span<Word const,Bi> y ) noexcept -> bool
    {
        bool r = false;
        r = r || not_equal( x[0], y[0] );
        r = r || not_equal( x[1], y[1] );
        return r;
    }

    /// Transform procedures.

    /// Normalized reciprocal approximation.
    ///
    /// Reciprocal is the multiplicative inverse.
    ///
    /// Requires:
    /// y is normalized

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto reciprocal_normalized ( span<Word const,Bi> y ) noexcept -> Word
    {
        auto v = reciprocal_normalized( y[1] );
        // p ← ( y[1] * v ) % B
        auto p = y[1] * v;
        // p ← ( p + y[0] ) % B
        p = p + y[0];
        if (p < y[0]) {
            v = v - 1;
            if (p >= y[1]) {
                v = v - 1;
                p = p - y[1];
            }
            // p ← ( p - y[1] ) % B
            p = p - y[1];
        }
        auto t = product( v, y[0] );
        // p ← ( p + t[1] ) % B
        p = p + t[1];
        if ( p < t[1] ) {
            v = v - 1;
            auto tp = array { t[0], p };
            if ( not_smaller( span<Word const,2>(tp), y ) ) {
                v = v - 1;
            }
        }
        return v;
    }

    /// Expand procedures.
    ///
    /// These procedures increase values, producing a "carry" or an "excess".

    /// Next with carry.
    ///
    /// Permits aliasing r to x.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto next_assign ( span<Word> r, span<Word const,Bi> x, Word carry = Word(0) ) -> Word
    {
        carry = sum_assign( r[0], x[0], Word(1), carry );
        carry = sum_assign( r[1], x[1], Word(0), carry );
        return carry;
    }

    /// Sum with carry.
    ///
    /// Permits aliasing r to x.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto sum_assign ( span<Word> r, span<Word const,Bi> x, Word y, Word carry = Word(0) ) noexcept -> Word
    {
        carry = sum_assign( r[0], x[0], y, carry );
        carry = sum_assign( r[1], x[1], Word(0), carry );
        return carry;
    }

    /// Sum with carry.
    ///
    /// Permits aliasing r to x.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto sum_assign ( span<Word> r, span<Word const,Bi> x, span<Word const,Bi> y, Word carry = Word(0) ) noexcept -> Word
    {
        carry = sum_assign( r[0], x[0], y[0], carry );
        carry = sum_assign( r[1], x[1], y[1], carry );
        return carry;
    }

    /// Twice with excess.
    ///
    /// Permits aliasing r to x.
    ///
    /// Requires:
    /// z < B

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto twice_assign ( span<Word> r, span<Word const,Bi> x, size_t z, Word excess = Word(0) ) noexcept -> Word
    {
        excess = twice_assign( r[0], x[0], z, excess );
        excess = twice_assign( r[1], x[1], z, excess );
        return excess;
    }

    /// Reduce procedures.
    ///
    /// These procedures decrease values, requiring with a "borrow" or leaving a "remainder".

    /// Difference with borrow.
    ///
    /// Permits aliasing r to x.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto difference_assign ( span<Word> r, span<Word const,Bi> x, Word y, Word borrow = Word(0) ) noexcept -> Word
    {
        borrow = difference_assign( r[0], x[0], y, borrow );
        borrow = difference_assign( r[1], x[1], Word(0), borrow );
        return borrow;
    }

    /// Difference with borrow.
    ///
    /// Permits aliasing r to x.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto difference_assign ( span<Word> r, span<Word,Bi> x, span<Word const,Bi> y, Word borrow = Word(0) ) noexcept -> Word
    {
        borrow = difference_assign( r[0], x[0], y[0], borrow );
        borrow = difference_assign( r[1], x[1], y[1], borrow );
        return borrow;
    }

    /// Half N times with remainder.
    ///
    /// Permits aliasing r to x.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto half_assign ( span<Word> r, span<Word const,Bi> x, size_t times = 1 ) noexcept -> Word
    {
        constexpr auto B = sizeof(Word) * 8uz;
        auto t = x[0] & ((1 << times) - 1);
        r[0] = x[0] >> times;
        r[0] |= x[1] << (B - times);
        r[1] = x[1] >> times;
        return t;
    }

    /// Normalized division with remainder.
    ///
    /// Computes by the "Improved division by invariant integers" method.
    ///
    /// Requires:
    /// x ÷ B < y
    /// y is normalized
    /// iy = reciprocal_normalized(y)

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto division_normalized ( span<Word const,Bi> x, Word y, Word iy ) noexcept -> tuple< Word, Word >
    {
        // t = ( r[1] × iy ) + x
        auto t = product( x[1], iy );
        ignore = sum_assign<Word,Bi>( t, t, x );
        // q = ( t[1] + 1 ) mod B
        auto [ q, _ ] = sum( t[1], Word(1) );
        // r = ( x - ( q × y ) ) mod B
        auto qy = product( q, y );
        auto [ r, _ ] = difference( x[0], qy[0] );
        // if r > t[0] : q = ( q - 1 ) mod B; r = ( r + y ) mod B
        if ( is_greater( r, t[0] ) ) {
            ignore = previous_assign( q, q );
            ignore = sum_assign( r, r, y );
        }
        // if r >= y : q = ( q + 1 ) mod B; r = ( r - y ) mod B
        if ( not_smaller( r, y ) ) [[unlikely]] {
            ignore = next_assign( q, q );
            ignore = difference_assign( r, r, y );
        }
        // terminate
        return { q, r };
    }
}