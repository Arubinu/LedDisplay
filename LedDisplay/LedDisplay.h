#ifndef		LED_DISPLAY_H
#define		LED_DISPLAY_H

/*
 * Vous pouvez éditer les paramètres ci-dessous.
 */

#define		ENABLE_DEBUG			false
#define		ENABLE_SERIAL			true
#define		ENABLE_BLUETOOTH		true

#define		SERIAL_SPEED			9600
#define		BLUETOOTH_SPEED			9600
#define		BLUETOOTH_RX			10											// Pin RX du module Bluetooth
#define		BLUETOOTH_TX			11											// Pin TX du module Bluetooth

#define		TIME_ZONE				+1											// Fuseau horaire
#define		TIME_LENGTH				10											// Timestamp UTC composé de 10 chiffres
#define		TIME_HEADER				"date="										// Commande suivi du timestamp UTC

#define		TIMER_TIMEOUT			21											// Vitesse de défilement

#define		DISPLAY_NUMBER			6											// Nombre d'afficheurs 16x16
#define		DISPLAY_DECAL			1											// Points de décalage ( lors du déplacement du texte )
#define		DISPLAY_1				14, 15, 16, 17, 18, 19, 20, 21				// Pins de l'afficheur 1 dans cet ordre : D, C, B, A, G, DI, CLK, LAT
#define		DISPLAY_2				9, 8, 7, 6, 5, 4, 3, 2
#define		DISPLAY_3				23, 25, 27, 29, 31, 33, 35, 37
#define		DISPLAY_4				39, 41, 43, 45, 47, 49, 51, 53
#define		DISPLAY_5				A0, A1, A2, A3, A4, A5, A6, A7
#define		DISPLAY_6				A8, A9, A10, A11, A12, A13, A14, A15

/*
 * Ne pas éditer ce qui suit !
 */

#include	<avr/pgmspace.h>

#include	<Timer.h>
#include	<DateTime.h>
#include	<LedMatrixObject.h>

#include	"ascii.h"

void		dateUpdate( void );
void		decalScreen( void );

char		*toTwo( char *, int );
char		*toTwo( char *, int, int );
void		timeStamp( const char * );
void		timeString( const char * );

int			cstringScene( char * );
void		mstringScene( char *, int, int, bool (*)[ 16 * 16 ] );

void		charScene( bool *, unsigned char, int, int );
void		mcharScene( unsigned char, int, int, bool (*)[ 16 * 16 ] );

int			reversePixel( int, int );
char		charNExtend( unsigned char c, char );

#endif