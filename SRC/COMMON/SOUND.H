#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

#define CC 262*2
#define DD 294*2
#define EE 330*2
#define FF 349*2
#define GG 392*2
#define AA 440*2
#define BB 494*2
#define C 262
#define D 294
#define E 330
#define F 349
#define G 392
#define A 440
#define B 494
#define Cc 262/2
#define Dd 294/2
#define Ee 330/2
#define Ff 349/2
#define Gg 392/2
#define Aa 440/2
#define Bb 494/2

/** Play sound.
*  \param[in]  p_freq		frequency of sound.
*  \param[in]  p_duration	duration of sound. */
void soundnoise( unsigned int p_freq, unsigned int p_duration );

/** Play error sound */
void errorsound( void );

#endif /* SOUND_H_INCLUDED */