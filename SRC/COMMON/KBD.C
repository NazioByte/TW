/*
* ============================================================================
* KBD.C
* ============================================================================
*/

#include <stdio.h>

#include "cwtype.h"
#include "common.h"
#include "cwgrphc.h"
#include "ekbd.h"
#include "var.h"

#include "kbd.h"

void waitkbd( int p_xcurpos, int p_ycurpos ) {
	register unsigned int i;

	setcurpos( p_xcurpos, p_ycurpos, thaimode );
	while ( !keypressed( ) ) {
		for ( i = 0; !keypressed( ) && i < 6000; i++ );
		setcurpos( p_xcurpos, p_ycurpos, thaimode );
		for ( i = 0; !keypressed( ) && i < 6000; i++ );
		setcurpos( p_xcurpos, p_ycurpos, thaimode );
	}
	setcurpos( p_xcurpos, p_ycurpos, thaimode );
}

int readkbd( void ) {
	register unsigned int c;

	c = ebioskey( 0 );
	if ( thaimode && ( ( c & 0xff00 ) < 0x4700 ) ) {       /* Ignore Keypad */
		c = thaikey( c );
	}
	return( c );
}
