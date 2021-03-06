#include "tctype.h"
#include "thai.h"

#include "global.h"
#include "routine.h"

unsigned char *WoreWaanRtn( register unsigned char *TempIndx ) {
	register unsigned char *plus1 = TempIndx + 1;
	register unsigned char *minus3 = TempIndx - 3;

	if ( istcon( *( TempIndx - 1 ) ) ) {
		if ( istlvwl( *( TempIndx - 2 ) ) ) {
			return( minus3 );
		} else if ( istcon( *( TempIndx - 2 ) ) && istcon( *plus1 ) ) {
			switch ( *( TempIndx + 1 ) ) {
			case OrAng:
				return( FAIL );
			case RoreReo:
				if ( *( TempIndx + 2 ) == RoreReo ) {
					return( FAIL );
				}
			default:
				if ( plus1 <= RightMargin ) {
					if ( *( TempIndx + 3 ) == Karan ) {
						return( FAIL );
					} else {
						return( !istrvwl( *( TempIndx + 2 ) ) ? plus1 : FAIL );
					}
				} else {
					return( FAIL );
				}
			}
		} else if ( isttnl( *( TempIndx - 1 ) ) && istcon( *( TempIndx - 2 ) ) && istcon( *minus3 ) ) {

			if ( plus1 <= RightMargin ) {
				return( plus1 );
			} else if ( !nstrcmp( "��", minus3 ) || !nstrcmp( "��", minus3 ) || !nstrcmp( "˹", minus3 ) || !nstrcmp( "��", minus3 ) ) {
				return( TempIndx - 4 );
			} else {
				return( minus3 );
			}
		}
	}
	return( FAIL );
}
