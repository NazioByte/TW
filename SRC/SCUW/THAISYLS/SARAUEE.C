#include "tctype.h"
#include "thai.h"

#include "global.h"
#include "routine.h"

/** Written       : Tanat Chirakarn
*   Modified      : Subun Yenjaichon
*   Date Written  : 7 May, 1988
*   Date Modified : 23 May, 1988
*/

unsigned char *Ueer1( unsigned char *TempIndx );
unsigned char *Ueer2( unsigned char *TempIndx );
unsigned char *Ueer3( unsigned char *TempIndx );
unsigned char *Ueer4( unsigned char *TempIndx );
unsigned char *Ueer5( unsigned char *TempIndx );
unsigned char *Ueer6( unsigned char *TempIndx );

unsigned char *SaraUeeFC( unsigned char *TempIndx );

unsigned char *SaraUeeRtn( register unsigned char *TempIndx ) {
	/* if ( findchar( *( TempIndx + 1 ), "����" ) ) {
		return(FAIL);
	} */    /* corrected by Subun */

	if ( isttnl( *( TempIndx + 1 ) ) ) {
		if ( *( TempIndx + 2 ) != OrAng ) {
			if ( TempIndx + 2 <= RightMargin ) {
				return( TempIndx + 2 );
			}
			/* return( ( TempIndx + 2 <= RightMargin ) ? ( TempIndx + 2 ) : FAIL ); */
		} else if ( *( TempIndx - 3 ) == SaraA ) { /* *( TempIndx + 2 ) == OrAng */
			switch ( *( TempIndx + 1 ) ) {
			case MaiEk:
				return( Ueer1( TempIndx ) ); /* �--��� */
			case MaiToe:
				return( Ueer2( TempIndx ) ); /* �--��� */
			default:
				return( TempIndx - 4 );
			}

		} else if ( *( TempIndx - 2 ) == SaraA ) {
			switch ( *( TempIndx + 1 ) ) {
			case MaiEk:
				return( Ueer3( TempIndx ) ); /* �-��� */
			case MaiToe:
				return( Ueer4( TempIndx ) ); /* �-��� */
			default:
				return( TempIndx - 3 );
			}
		}

		if ( TempIndx + 2 <= RightMargin ) {
			return( TempIndx + 2 );
		} else {
			return( SaraUeeFC( TempIndx ) );
		}
		/* return( ( TempIndx + 2 <= RightMargin ) ? TempIndx + 2 : FAIL ); */
	} else if ( *( TempIndx + 1 ) == OrAng ) {
		if ( *( TempIndx - 3 ) == SaraA ) {
			return ( Ueer5( TempIndx ) );   /* �--�� */
		} else if ( *( TempIndx - 2 ) == SaraA ) {
			return ( Ueer6( TempIndx ) );   /* �-�� */
		} else if ( TempIndx + 1 <= RightMargin ) {
			return( TempIndx + 1 );
		}
		/* return( ( TempIndx + 1 <= RightMargin ) ? ( TempIndx + 1 ) : FAIL );  */
	}

	if ( TempIndx + 1 <= RightMargin ) {
		return( TempIndx + 1 );
	} else {
		return( SaraUeeFC( TempIndx ) );
	}

	/* return( ( TempIndx + 1 <= RightMargin ) ? ( TempIndx + 1 ) : FAIL );  */
}

unsigned char *SaraUeeFC( unsigned char *TempIndx ) {
	unsigned char *TempIndxm2 = TempIndx - 2;

	switch ( *( TempIndx - 1 ) ) {
	case NoreNoo:
	case MoreMar:
	case WoreWaan:
		if ( *TempIndxm2 != HorHeeb ) {
			return( TempIndxm2 );		/* cut before NoreNoo */
		} else {
			return( TempIndx - 3 );		/* cut before HorHeeb */
		}
		break;
	case RoreReo:
		if ( !findchar( *TempIndxm2, "���" ) ) {
			return( TempIndxm2 );
		}
		break;
	case LoreLing:
		if ( !findchar( *TempIndxm2, "����" ) ) {
			return( TempIndxm2 );
		}
		break;
	default:
		return( TempIndxm2 );
	}
	return( FAIL );
}

unsigned char *Ueer1( unsigned char *TempIndx ) { /* �--��� */
	if ( TempIndx + 2 <= RightMargin &&
		( !findchar( *( TempIndx + 3 ), "����" ) ||
		!nstrcmp( "��", TempIndx - 2 ) ||
		!nstrcmp( "˧", TempIndx - 2 ) ||
		!nstrcmp( "��", TempIndx - 2 ) ) ) {
		return( TempIndx + 2 );
	} else if ( TempIndx + 3 <= RightMargin ) {
		return( TempIndx + 3 );
	}
	return( TempIndx - 4 );
}

unsigned char *Ueer2( unsigned char *TempIndx ) { /* �--��� */
	if ( TempIndx + 2 <= RightMargin ) {
		if ( !nstrcmp( "��", TempIndx - 2 ) ) {
			if ( *( TempIndx + 3 ) != NoreNoo ) {
				return( TempIndx + 2 );
			}
		} else if ( TempIndx + 3 <= RightMargin ) {
			return( TempIndx + 3 );
		}
	}
	return( TempIndx - 4 );
}

unsigned char *Ueer3( unsigned char *TempIndx ) { /* �-��� */
	if ( findchar( *( TempIndx - 1 ), "����������" ) ) {
		if ( findchar( *( TempIndx + 3 ), "����" ) ) {
			return( TempIndx - 3 );
		} else {
			return( ( TempIndx + 2 <= RightMargin ) ? TempIndx + 2 : TempIndx - 3 );
		}
	} else {
		return( ( TempIndx + 3 <= RightMargin ) ? TempIndx + 3 : TempIndx - 3 );
	}
}

unsigned char *Ueer4( unsigned char *TempIndx ) { /* �-��� */
	if ( TempIndx + 2 <= RightMargin ) {
		if ( findchar( *( TempIndx + 3 ), "����" ) ) {
			return( TempIndx - 3 );
		} else {
			return( TempIndx + 2 );
		}
	}
	return( TempIndx - 3 );
}

unsigned char *Ueer5( unsigned char *TempIndx ) { /* �--�� */
	if ( TempIndx + 2 <= RightMargin &&
		findchar( *( TempIndx + 2 ), "������" ) &&
		nstrcmp( "��", TempIndx - 2 ) &&
		nstrcmp( "��", TempIndx - 2 ) &&
		nstrcmp( "˹", TempIndx - 2 ) &&
		nstrcmp( "��", TempIndx - 2 ) ) {
		return( TempIndx + 2 );
	}
	return( TempIndx - 4 );
}

unsigned char *Ueer6( unsigned char *TempIndx ) { /* �-�� */
	if ( TempIndx + 2 <= RightMargin &&
		!findchar( *( TempIndx - 1 ), "��������" ) &&
		findchar( *( TempIndx + 2 ), "�������" ) ) {
		return( TempIndx + 2 );
	}
	return( TempIndx - 3 );
}
