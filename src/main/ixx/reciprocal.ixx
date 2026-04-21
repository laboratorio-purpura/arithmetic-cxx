// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <span>
#include <tuple>

export module purple.arithmetics:reciprocal;

import :word_concept;

namespace purple::arithmetics
{
	/// Computes an approximation to the multiplicative inverse of a "normalised" integer.
	///
	/// Requires:
	/// y is "normalised".
	/// Otherwise, the result is undefined.
	///
	/// This implementation applies the "Improved division by invariant integers" method.

	export
	template <Word W>
	auto reciprocal_normalized ( W y ) noexcept -> W
	{
		// y' = ( ( ( β^2 - 1 ) ÷ y ) ÷ β ) - β
		// y' = <β - 1 - y, β - 1> ÷ y

		// t0 ← β - 1
		auto t0 = W(-1);
		// t1 ← t0 - y
		auto [ t1, _ ] = difference( t0, y, W{0} );
		// q, r ← <t1, t0> ÷ y
		auto [ q, _ ] = division( array<W,2>{ t0, t1 }, y );

		return q;
	}

    /// Computes an approximation to the multiplicative inverse of a "normalised" two-word integer.
    ///
    /// Requires:
    /// y is "normalised".
    /// Otherwise, the result is undefined.
    ///
    /// This implementation applies the "Improved division by invariant integers" method.

    export
    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto reciprocal_normalized ( span<W const,Bi> y ) noexcept -> W
    {
        // 1. v ← RECIPROCAL_WORD(d1)
        auto v = reciprocal_normalized( y[1] );
        // We have β^2 − d1 ≤ (β + v).d1 < β^2
        // 2. p ← d1.v mod β
        auto [ p, _ ] = product( y[1], v );
	    // 3. p ← (p + d0) mod β
        tie( p, ignore ) = sum( p, y[0], W{0} );
    	// 4. if p < d0
        if ( is_smaller( p, y[0] ) ) {
	    	// 5. v ← v − 1
            tie( v, ignore ) = previous( v, W{0} );
	    	// 6. if p ≥ d1
            if ( not_smaller( p, y[1] ) ) {
	    		// 7. v ← v − 1
                tie( v, ignore ) = previous( v, W{0} );
	    		// 8. p ← p − d1
                tie( p, ignore ) = difference( p, y[1], W{0} );
            }
    		// 9. p ← (p − d1) mod β
            tie( p, ignore ) = difference( p, y[1], W{0} );
        }
        // We have β^2 − d1 ≤ (β + v) . d1 + d0 < β^2.
        // 10. <t1, t0> ← v.d0
        auto t = product( v, y[0] );
		// 11. p ← (p + t1) mod β
        tie( p, ignore ) = sum( p, t[1], W{0} );
		// 12. if p < t1
        if ( is_smaller( p, t[1] ) ) {
			// 13. v ← v − 1
            tie( v, ignore ) = previous( v, W{0} );
			// 14. if <p, t0> ≥ <d1, d0>
            auto tp = array { t[0], p };
            if ( not_smaller<W>( tp, y ) ) {
				// 15. v ← v − 1
                tie( v, ignore ) = previous( v, W{0} );
            }
        }
        return v;
    }
}
