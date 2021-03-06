/*
* ============================================================================
* PRINTING.C
*
* Actual print data
* Devise from PMENU.C
*
* Written 11-11-1988
* Update 15-11-1988
* Update 08-04-1989
*
* Update Suttipong Kanakakorn
* Mon  08-07-1989  02:23:55
*  - Add function prototype
* Tue  08-15-1989  00:29:02
*  - make printer font loading better
* ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dir.h>
#include <ctype.h>

#include "..\common\cwtype.h"
#include "..\common\cscrn.h"
#include "..\common\ekbd.h"
#include "..\common\common.h"

#include "cuptype.h"
#include "global.h"
#include "24pins.h"
#include "cupsetup.h"
#include "dot.h"
#include "preproc.h"
#include "prt.h"
#include "prtbuf24.h"
#include "prncntrl.h"
#include "prtt.h"
#include "prtutil.h"

#include "printing.h"

/* local function prototype use internal module only */

/**  */
void initializebufferandfont( void );

/**  */
void strfilter( char *p_s, char p_cutout );

/**  */
void seekbeginline( void );

/**  */
void printtitle( char *p_str, int p_loc );

/**  */
char *adjust( char *p_c, int p_k );

/**  */
void pagewait( register int p_i );

/**  */
void linewait( int *p_printquit );

/**  */
void preprinterloadline( char *p_s );

/* Modified: Suttipong Kanakakorn Tue  08-15-1989  00:28:34 */
void initializebufferandfont( void ) {
	extern int printer24pin;
	extern int printer24pininit;
	extern int printer9pininit;

	if ( printer24pin == YES ) {
		if ( printer24pininit == NO ) {
			AllocateBuffer9pin( );
			cp_init_textbuf( );
			cp_init_grpbuf( );
			cp_lqnorm = cp_loadfont( "NORMAL.P24", CP_LQ_TABSIZE );
			cp_lqitalic = cp_loadfont( "ITALIC.P24", CP_LQ_TABSIZE );
			cp_lqscr = cp_loadfont( "NORMALS.P24", CP_LQSCR_TABSIZE );
			cp_lqscr_italic = cp_loadfont( "ITALICS.P24", CP_LQSCR_TABSIZE );
			cp_lqnorm_comb = cp_create_lqcombine( cp_lqnorm );
			cp_lqitalic_comb = cp_create_lqcombine( cp_lqitalic );
			cp_lqscr_comb = cp_create_lqscrcombine( cp_lqscr );
			cp_lqscr_italic_comb = cp_create_lqscrcombine( cp_lqscr_italic );
			printer24pininit = YES;
		}
	} else {
		if ( printer9pininit == NO ) {
			AllocateBuffer9pin( );
			PrinterLoadFont9pin( );
			printer9pininit = YES;
		}
	}
}

/* modify by Suttipong Kanakakorn Tue  08-29-1989  23:53:53 */
void printing( void ) {
	extern int blankskip( );
#ifdef WANT_TO_USE_GRAPH
	extern int pic_print;
#endif
	int i;
	int j;
	int printquit;
	char st[5];
	char s[500];
	char p[500];
	char scol;
	if ( graphicprint == YES ) {							/* if print in graphic mode */
		initializebufferandfont( );							/* allocate graphic buffer and load font */
	}
	scol = 7;
	dispstrhgc( "���ѧ����� COPY��� :     ", 10 - scol, 12, NORMALATTR );
	dispstrhgc( "  ˹�ҷ�� :     ", 30 - scol, 12, NORMALATTR );
	dispstrhgc( "  ��÷Ѵ��� :     ", 43 - scol, 12, NORMALATTR );
	dispstrhgc( "  ��ش���Ǥ��� <P> ��ԡ����� <Q>", 58 - scol, 12, BOLDATTR );
	printquit = NO;
	PrinterInitialize( );									/* initialize printer */
	PrinterMasterSelectMode( 0 );							/* Set to 10 Cpi */
	for ( i = 1; i <= copytoprint; i++ ) {
		linespace = find_line_space( );
		PrinterSetFormLenghtInch( pagelength );
		seekbeginline( );
		dispprintf( 25 - scol, 12, REVERSEATTR, "%5d", i );
		curpage = pagenumberoffset;
		dispprintf( 38 - scol, 12, NORMALATTR, "%5d", curpage );
		curline = 0;
#ifdef WANT_TO_USE_GRAPH
		pic_print = NO;
#endif
		while ( ( fgets( s, 500, fp ) != NULL ) && ( printquit == NO ) ) {
			strfilter( s, 0x8d );							/* delete 0x8d */
			strcpy( p, s );
			j = 0;
			while ( ( p[j]<32 ) && ( p[j] != NULL ) ) j++;
			if ( p[j] == '.' ) {							/* -to allow free format. */
				dotcommand( &p[j] );						/* do dotcommand then read next */
				if ( newpage == YES ) {						/* page break dotcmd (.pa ,.cp) found */
					newpage = NO;							/* reset pagebreak status */
															/*
															PrinterSkipLine( ( ( 29 + 1 ) - ( locpagetitle != 0 ) - ( locheading != 0 )
															- ( locfooting != 0 ) - curline ) * 2 );
															*/
					skip_line( ( userlineperpage + 1 ) - curline );
					curline = 0;
					printtitle( footing, locfooting );
					PrinterFormFeed( );
					if ( curpage >= pageend - pagebegin + pagenumberoffset ) {
						break;
					}
					pagewait( 0 );							/* finish each page */
					curpage++;
					dispprintf( 38 - scol, 12, NORMALATTR, "%5d", curpage );
				}
			} else {										/* Not dot commands */
				if ( mailmergeflag == YES ) {
					mailmerge( s, p );
					strcpy( s, p );
				}
				if ( stdcode == NO ) {
					contostd( s );							/* convert from ku to so-mo-or */
				}
				marginset( s );								/* set left-right margin */
				if ( curline == 0 ) {
					printtitle( heading, locheading );
					sprintf( p, pageformat, curpage );
					printtitle( p, locpagetitle );
					curline++;								/* begin curline = 1  */
				}

				dispprintf( 53 - scol, 12, NORMALATTR, "%5d", curline );
				dispprintf( 2, 13, NORMALATTR, "%77s", " " );	/* Clear Line */
				dispprintf( 2, 13, NORMALATTR, "%-77.77s", s );		/* Disp CurLine */

				preprinterloadline( s );
				curline++;
				if ( curline > userlineperpage ) {
					curline = 0;
					printtitle( footing, locfooting );
					PrinterFormFeed( );
					if ( curpage >= pageend - pagebegin + pagenumberoffset ) {
						break;
					}
					pagewait( 0 );							/* finish each page */
					curpage++;
					itoa( curpage, st, 10 );
					dispstrhgc( "     ", 38 - scol, 12, 0 );
					dispstrhgc( st, 43 - strlen( st ) - scol, 12, 2 );
				}
				linewait( &printquit );
			}
		}
		if ( curline != 0 ) {
			/*
			PrinterSkipLine( ( ( 29 + 1 ) - ( locpagetitle != 0 ) - ( locheading != 0 )
			- ( locfooting != 0 ) - curline ) * 2 ); */ /* skip to bottom */
			skip_line( ( userlineperpage + 1 ) - curline );
			printtitle( footing, locfooting );
			PrinterFormFeed( );
		}
		fseek( fp, 0L, SEEK_SET );							/* rewind file pointer */
		if ( printquit == YES ) {
			break;											/* exit for loop */
		}
		pagewait( 1 );										/* finish each copy */
	}
	dispprintf( 2, 12, NORMALATTR, "%77s", " " );			/* clear massage */
	fclose( mfp );											/* close merge file */
	mailmergeflag = NO;										/* each field content ready flag Reset  */
	mergefileexist = NO;									/* .df successful           flag Reset  */
	fieldnameexist = NO;									/* .rv successful           flag Reset  */
}

void marginset( char *p_t ) {
	int cw = 1;											/* 2 for enlarge attr, 1 for another */
	char p[500];
	int left;
	int i;
	int k;
	k = ( leftmargin == 1 ) ? 0 : leftmargin;				/* user remind start at 1 not 0 */
	left = k;												/* k is pointer , left is counter */
	for ( i = 0; i < 500; i++ ) { p[i] = ' '; }				/* set all blank  */
	i = 0;
	while ( p_t[i] != '\0' ) {
		if ( p_t[i] < 0x20 ) {
			if ( p_t[i] == ENLARGE ) {
				cw = ( cw == 2 ) ? 1 : 2;
			}
		} else {
			if ( whatlevel( p_t[i] ) == 0 ) {
				left += cw;									/* increment if normal level */
				if ( left > rightmargin ) {
					break;
				}
			}
		}
		p[k++] = p_t[i++];
	}
	p[k] = '\0';
	strcpy( p_t, p );
}

/* modified by Suttipong Kanakakorn */
void contostd( char *p_t ) {
	while ( *p_t ) {
		*p_t = kutostd( *p_t );
		p_t++;
	}
}

/* modified by Suttipong Kanakakorn */
void strfilter( char *p_s, char p_cutout ) {
	char *r;

	r = p_s;
	while ( *p_s ) {
		if ( *p_s != p_cutout ) {
			*r++ = *p_s;
		}
		p_s++;
	}
	*r = '\0';
}

void seekbeginline( void ) {
	int cdtpgbrk;
	int pagecount = 1;
	int linecount = 0;
	char s[500];
	char temp[500];
	if ( pagebegin > 1 ) {
		while ( fgets( s, 500, fp ) != NULL ) {
			if ( s[0] == '.' ) {   /* dot command */
				if ( tolower( s[1] ) == 'p'&&tolower( s[2] == 'a' ) ) { /* page break */
					pagecount++;
					linecount = 0;
				} else {
					if ( tolower( s[1] ) == 'c'&&tolower( s[2] == 'p' ) ) {/*cond pg-brk*/
						strcpy( temp, &s[3] );
						strcpy( temp, &temp[blankskip( temp )] );
						temp[strlen( temp ) - 1] = '\0';
						cdtpgbrk = ( atoi( temp ) > userlineperpage ) ?
							userlineperpage : atoi( temp );
						if ( linecount + cdtpgbrk > userlineperpage ) {
							pagecount++;
							linecount = 0;
						}
					}
				}
			} else {
				linecount++;
				if ( linecount == userlineperpage ) {
					pagecount++;
					linecount = 0;
				}
			}
			if ( pagecount >= pagebegin ) {
				break;
			}
		}
	}
}

/* modify by Suttipong Kanakakorn Wed  08-30-1989  00:32:38 */
void printtitle( char *p_str, int p_loc ) {
	int newloc;
	char s[500];
	if ( p_loc != 0 ) {
		switch ( p_loc ) {
		case 4:												/* odd <-> right  even <-> left  */
			newloc = ( curpage % 2 == 0 ) ? 1 : 3;
			break;
		case 5:												/* odd <-> left   even <-> right */
			newloc = ( curpage % 2 == 0 ) ? 3 : 1;
			break;
		default:
			newloc = p_loc;
			break;
		}
		strcpy( s, adjust( p_str, newloc ) );					/* adjust location LEFT,RIGHT,MIDDLE */
		dispprintf( 2, 13, REVERSEATTR, "%-77.77s", s );
		preprinterloadline( s );
	}
}

char *setpageformat( char *p_form, int p_max ) {
	char s[500];
	int i = 0;
	int j = 0;

	while ( p_max-- > 0 ) {
		if ( p_form[i] == '\0' ) {
			s[j++] = ' ';
			s[j++] = '%';
			s[j++] = 'd';
			s[j] = '\0';
			return ( s );
		} else {
			if ( p_form[i] == '%' ) {
				s[j++] = p_form[i++];
				s[j++] = 'd';
				while ( p_form[i] != '\0' ) {
					if ( p_form[i] == '%' ) {
						s[j++] = ' ';  /* ignore the %  */
						i++;
					} else {
						s[j++] = p_form[i++];
					}
				}
				s[j] = '\0';
				return ( s );
			} else {
				s[j++] = p_form[i++];
			}
		}
	}
	s[j] = '\0';
	return ( s );
}

char *adjust( char *p_c, int p_k ) {
	int maxpaper;
	int len = 0;
	int i = 0;
	int j;
	char s[500];
	maxpaper = ( smallpaper ) ? 80 : 136;			/* find actual thai string length */
	while ( p_c[i] != '\0' ) {
		len = ( whatlevel( p_c[i++] ) == 0 ) ? len + 1 : len;
	}
	for ( i = 0; i < maxpaper; i++ ) {
		s[i] = ' ';
	}
	s[maxpaper] = '\0';
	switch ( p_k ) {
	case 1:											/* left   justify */
		return( p_c );
	case 2:											/* center justify */
		j = ( maxpaper - 1 - len ) / 2;
		i = 0;
		while ( p_c[i] != '\0' ) {
			s[j++] = p_c[i++];
		}
		s[j] = '\0';
		return( s );
	case 3:											/* right  justify */
		j = ( maxpaper - 1 - len );
		i = 0;
		while ( p_c[i] != '\0' ) {
			s[j++] = p_c[i++];
		}
		s[j] = '\0';
		return ( s );
	default:
		return ( p_c );
	}
}

/* modify by Suttipong Kanakakorn Mon  08-28-1989  23:36:26 */
void pagewait( register int p_i ) {
	if ( pagebreak == YES ) {
		if ( p_i == 0 || p_i == 1 ) {
			clear_message( );
			if ( p_i == 0 ) {
				dispstrhgc( "������� � �����˹�ҡ�д�ɾ���� ...", 30, 13, 2 );
			} else {
				dispstrhgc( "����ش��þ�������Ъش ������� � ...", 30, 13, 2 );
			}
			while ( !keypressed( ) );
			while ( keypressed( ) ) { ebioskey( 0 ); }			/* clear KBD buffer */
			clear_message( );
		}
	}
}

/* modify by Suttipong Kanakakorn Mon  08-28-1989  23:36:41 */
void linewait( int *p_printquit ) {
	char i;
	if ( keypressed( ) ) {
		while ( keypressed( ) ) {
			i = tolower( ebioskey( 0 ) & 0x00ff );
			switch ( i ) {
			case 'p':
				while ( keypressed( ) ) {
					ebioskey( 0 );							/* clear KBD buffer */
				}
				clear_message( );
				dispstrhgc( "������ < P > ���;������, < Q > ��ԡ�����...",
					20, 13, REVERSEATTR );
				do {
					i = tolower( ebioskey( 0 ) & 0x00ff );
				} while ( i != 'p' && i != 'q' );
				clear_message( );
				if ( i == 'q' ) {
					*p_printquit = YES;
				}
				break;
			case 'q':
				*p_printquit = YES;							/* quit printing */
				while ( keypressed( ) ) { ebioskey( 0 ); }	/* clear KBD buffer */
				break;
			}
		}
	}
}

/*
void findthreeindex( char *p_s, int *p_uindex, int *p_mindex, int *p_lindex ) {
	int i;
	int j;
	int cadjust = 1;

	i = *p_uindex = *p_mindex = *p_lindex = 0;
	while ( p_s[i] != '\0' ) {
		if ( p_s[i] < ' ' ) {
			if ( p_s[i] == ENLARGE ) { // if enlarge col to add = 2 //
				if ( cadjust == 1 ) {
					cadjust = 2;
				} else {
					cadjust = 1;
				}
			}
		} else {
			j = p_s[i];
			switch ( islevel( &j ) ) {
			case 1:
				*p_uindex = *p_mindex;
				break;
			case 2:
				*p_mindex += cadjust;
				break;
			case 3:
				*p_lindex = *p_mindex;
				break;
			}
		}
		i++;
	}
}
*/

void preprinterloadline( char *p_s ) {
#ifdef WANT_TO_USE_GRAPH
	extern int pic_print;
#endif
	extern int printer24pin;
	int mindex;
	if ( graphicprint == YES ) {
		if ( printer24pin == YES ) {
			/* findthreeindex( p_s, &uindex, &mindex, &lindex ); */
			cp_clearbuf( );
			cp_split4level( p_s );
			mindex = cp_printlq( );
			PrintThree24pin( mindex, mindex, mindex );
		} else {
			PrinterLoadLine9pin( p_s );
		}
	} else {
#ifdef WANT_TO_USE_GRAPH
		if ( pic_print == NO ) {
#endif
			PrinterLoadLineText( p_s );
#ifdef WANT_TO_USE_GRAPH
		} else {
			if ( printer24pin == YES ) {
				/* findthreeindex( p_s, &uindex, &mindex, &lindex ); */
				cp_clearbuf( );
				cp_split4level( p_s );
				mindex = cp_printlq( );
				PrintThree24pin( mindex, mindex, mindex );
			} else {
				PrinterLoadLine9pin( p_s );
			}
		}
#endif
	}
}

/* Written by Suttipong Kanakakorn Wed  08-30-1989  00:40:21 */
void clear_message( void ) {
	dispprintf( 2, 13, NORMALATTR, "%77s", " " );
}
