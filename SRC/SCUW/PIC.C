/*
* ============================================================================
* GRPH.C
*
* CU-Writer graphic feature.
* ============================================================================
*/

#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>

#include "..\common\cwtype.h"
#include "..\common\cwgrphc.h"
#include "..\common\cscrn.h"
#include "..\common\dir.h"
#include "..\common\ekbd.h"
#include "..\common\fileutil.h"
#include "..\common\sound.h"

#include "var.h"
#include "convert.h"

#include "edit.h"
#include "getstr.h"

#include "pic.h"

#ifdef WANT_TO_USE_GRAPH
char *readgraph( char *filename ) {
	size_t filelen, namelen;
	int handle;
	char *buffer;
	if ( ( handle = open( filename, O_RDONLY || O_BINARY ) ) == -1 ) {
		return( NULL );
	}
	filelen = ( size_t ) filelength( handle );
	namelen = strlen( filename );
	buffer = ( char * ) malloc( filelen + namelen + 2 );
	strcpy( buffer, filename );
	read( handle, buffer + namelen + 1, filelen );
	close( handle );
	buffer[namelen + filelen] = '\0';
	buffer[namelen + filelen + 1] = 0x80;       /* code of end of graph */
	return( buffer );
}

void paintlinegraph( char *graph, unsigned int y ) {
	unsigned int endy, x;
	char tp;

	x = wind.col * 8;
	y = ( y * 20 ) + 84;
	if ( y >= 344 ) {
		return;
	}
	endy = y + 20;
	if ( endy > 344 ) {
		endy = 344;
	}
	while ( ( y != endy ) && ( *graph != 0x80 ) ) {
		tp = *graph;
		if ( tp == 0 ) {
			graph++;
			y++;
			x = wind.col * 8;
		} else {
			if ( tp < 128 ) {
				/*h_line_g( x, x + tp, y ); */ /* invalid function */
			} else {
				tp -= 128;
			}
			x += tp;
			graph++;
		}
	}
}

void notavailable( void ) {
	savepic( );
	blockmsg( 10 );
	dispstrhgc( "����觹�����������ҧ��þѲ�� ! �����������ͷӧҹ���"
		, 25 - CENTER_FACTOR, 10, REVERSEATTR );
	ebioskey( 0 );
	retpic( );
}

void insertgraph( ) {
	int i;
	char graphname[30];

	notavailable( ); /* comment this out */

	storeline( curline );

	framebox( 18 - CENTER_FACTOR, 4, ( 18 - CENTER_FACTOR ) + 53, 6, REVERSEATTR );
	dispstrhgc( "����������ٻ�Ҿ����ͧ�����ҹ :", 21 - CENTER_FACTOR, 5, REVERSEATTR );

	graphname[0] = '*';
	graphname[1] = '.';
	graphname[2] = '*';
	graphname[3] = '\0';
	i = getname( graphname, 41, 5, 22, REVERSEATTR );
	if ( ( i == YES ) && ( graphname[0] != '\0' ) ) {
		if ( havewild( graphname ) ) {
			selectfile( graphname );
		}
		curline->graph = readgraph( graphname );
	}
	changeflag = YES;
	pagecomplete = NO;
	loadtoline( curline->text );
}

void deletegraph( ) {

	notavailable( );  /* comment this out */

	if ( curline->graph != NULL ) {
		free( curline->graph );
		curline->graph = NULL;
		changeflag = YES;
		pagecomplete = NO;
	} else {
		errorsound( );
	}
}
#endif