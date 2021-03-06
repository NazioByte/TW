#ifndef SCUW_KBD_H_INCLUDED
#define SCUW_KBD_H_INCLUDED

/** Wait for keyboard press and display cursor at given position.
*   \param[in]  p_xcurpos		Vertical position of cursor.
*   \param[in]  p_ycurpos		Horizontal position of cursor.*/
void waitkbd( int p_xcurpos, int p_ycurpos );

/**  */
int changekey( int p_key );

/** Read keyboard.
*   \return int					*/
int readkbd( void );

/** Remap alternate character.
*   \param[in]  p_key			Keyboard code.
*   \return int					ALT mapped keyboard code, return -1 if fail. */
int alt_char_map( int p_key );

/** Display key combination when press control at top left of screen.
*  \param[in]  p_key		Key code.*/
void dispkey( register unsigned int p_key );

#endif /* SCUW_KBD_H_INCLUDED */
