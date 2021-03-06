/*
* ============================================================================
* PRTUTIL.C
*
* Written by Suttipong Kanakakorn Tue  08-29-1989  18:25:25
* ============================================================================
*/

#include <stdio.h>
#include <string.h>

#include "..\common\cwtype.h"
#include "..\common\cscrn.h"
#include "..\common\cwgrphc.h"
#include "..\common\ekbd.h"
#include "..\common\kbdcode.h"
#include "..\common\tutil1.h"

#include "const.h"
#include "cuptype.h"

#include "pmenu.h"
#include "prncntrl.h"

#include "prtutil.h"

extern int lineperpage;
extern int linespace;
extern int userlineperpage;
extern int pagelength;
extern int printer24pin;
extern int graphicprint;
extern int locpagetitle;
extern int locheading;
extern int locfooting;
extern int extbarprinting;

int find_line_space( void ) {
	register unsigned int i;
	register unsigned int j;
	register unsigned int k;
	/*
	[ i = Feeding resolution ( Dots / Inch )     ]
	[ j = Minimum number of dots need for 1 line ]
	[ k = linespace                              ]
	*/
	i = ( printer24pin ) ? 180 : 216;
	if ( graphicprint ) {
		j = ( printer24pin ) ? IPL_GRPH_24 : IPL_GRPH_9;
	} else {
		j = ( printer24pin ) ? IPL_TEXT_24 : IPL_TEXT_9;
	}
	k = ( pagelength - 1 ) * i / lineperpage - j;
	extbarprinting = ( k>j / 2 );
	return k;
}

int find_line_perpage( void ) {
	return userlineperpage +
		( ( locpagetitle != 0 ) + ( locheading != 0 ) + ( locfooting != 0 ) );
}

int find_max_line_perpage( void ) {
	register int i;
	if ( graphicprint ) {
		i = ( printer24pin ) ? ML_GRPH_24 : ML_GRPH_9;
	} else {
		i = ( printer24pin ) ? ML_TEXT_24 : ML_TEXT_9;
	}
	return i;
}

int find_max_userline_perpage( void ) {
	return find_max_line_perpage( ) -
		( ( locpagetitle != 0 ) + ( locheading != 0 ) + ( locfooting != 0 ) );
}

void set_all_lineperpage( set_linespace_mode p_mode ) {
	char *find_lineperinch( void );

	static char m1[] = "�ӹǹ��÷Ѵ���˹������ "
		"����ͧ������¹����繨ӹǹ�٧�ش��ҷ��������";
	static char m2[] = "������ ESC ���ͷӧҹ����";

	if ( userlineperpage > find_max_userline_perpage( ) ) {
		userlineperpage = find_max_userline_perpage( );
		if ( p_mode == INTERACTIVE ) {
			savepic( );
			framebox( 10, 8, 10 + thaistrlen( m1 ) + 4, 11, REVERSEATTR );
			dispstrhgc( m1, 10 + 2, 9, REVERSEATTR );
			dispstrhgc( m2, 10 + ( thaistrlen( m1 ) + 4 - thaistrlen( m2 ) ) / 2,
				10, REVERSEATTR );
			while ( ebioskey( 0 ) != ESCKEY );
			retpic( );
			writemenu( 0, 1, NORMALATTR ); /* show new userline per page */
		}
	}
	lineperpage = find_line_perpage( );
	if ( p_mode == INTERACTIVE ) {
		savepic( );        /*"                             "*/
		dispprintf( 19, 12, NORMALATTR,
			"�ӹǹ��÷Ѵ��͡�д�� 1 ���� ��� : %s ��÷Ѵ/����",
			find_lineperinch( ) );
		ebioskey( 0 );
		retpic( );
	}
}

void line_feed( unsigned int p_lines ) {
	register int i;
	register int j;

	j = ( printer24pin ) ? 180 : 216;
	i = p_lines / j;

	/* optimize by feed 1 inch */
	for ( ; i > 0; i-- ) {
		PrinterLineFeed( j );
	}
	PrinterLineFeed( p_lines % j );
}

void skip_line( unsigned int p_lines ) {
	for ( ; p_lines > 0; p_lines-- ) {
		if ( graphicprint ) {
			line_feed( ( printer24pin ) ? IPL_GRPH_24 + linespace
				: IPL_GRPH_9 + linespace );
		} else {
			line_feed( ( printer24pin ) ? IPL_TEXT_24 + linespace
				: IPL_TEXT_9 + linespace );
		}
	}
}

char *find_lineperinch( void ) {
	static char x[7];
	register i;

	sprintf( x, "%4d", ( unsigned int ) lineperpage * 1000 / pagelength );
	for ( i = 0; i < 4; i++ ) {
		if ( x[i] != '0' ) {
			memmove( &x[i + 2], & x[i + 1], 4 - i );
			x[i + 1] = '.';
			break;
		}
	}
	return x;
}
