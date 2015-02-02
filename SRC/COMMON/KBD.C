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
#include "grphc.h"
#include "var.h"

#include "kbd.h"

void waitkbd( unsigned int x, unsigned int y ) {
	register unsigned int i;

	setcurpos( x, y, thaimode );
	while ( !keypressed( ) ) {
		for ( i = 0; !keypressed( ) && i < 1200; i++ );
		setcurpos( x, y, thaimode );
		for ( i = 0; !keypressed( ) && i < 1200; i++ );
		setcurpos( x, y, thaimode );
	}
	setcurpos( x, y, thaimode );
}

int readkbd( void ) {
	register unsigned int c;
	c = ebioskey( 0 );
	if ( thaimode && ( ( c & 0xff00 ) < 0x4700 ) ) {       /* Ignore Keypad */
		c = thaikey( c );
	}
	return( c );
}