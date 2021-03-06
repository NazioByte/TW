#ifndef SCUP_PRNCNTRL_H_INCLUDED
#define SCUP_PRNCNTRL_H_INCLUDED

/** Put a character to printer. */
void putp( unsigned int p_character );

/**  */
int PrinterReadstatus( void );

/**  */
void PrinterInitialize( void );

/**  */
void PrinterSetFormLenghtInch( register unsigned int p_lenght );

/**  */
void PrinterSkipLine( register int p_lines );

/**  */
void PrinterFormFeed( void );

/** Will feed n/180 inches on 24 pins printer and
*   n/216 inches on 9 pins printer
*
*   modify by Suttipong Kanakakorn Tue  08-29-1989  12:56:39 */
void PrinterLineFeed( register unsigned int p_lines );

/**  */
void PrinterMasterSelectMode( register unsigned int p_mode );

/**  */
void PrinterSetGraphicMode9pinQuadrupleDensity( register unsigned int p_density );

/**  */
void PrinterSetGraphicMode24pinTripleDensity( register unsigned int p_density );

/**  */
void PrinterSet10cpi( void );

#endif /* SCUP_PRNCNTRL_H_INCLUDED */
