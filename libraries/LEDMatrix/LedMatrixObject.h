/**
 *     
	This file is part of .PNG Arduino Framework.

    .PNG Arduino Framework is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    .PNG Arduino Framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with .PNG Arduino Framework.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "stdlib.h"
#include "Arduino.h"

#ifndef LED_MATRIX_OBJECT_H
#define LED_MATRIX_OBJECT_H
typedef void ( *CallBackType )( void );


class LedMatrixObject
{

private:
	byte LEDARRAY_D;
	byte LEDARRAY_C;
	byte LEDARRAY_B;
	byte LEDARRAY_A;
	byte LEDARRAY_G;
	byte LEDARRAY_DI;
	byte LEDARRAY_CLK;
	byte LEDARRAY_LAT;

	bool			( *Scene )[ 16 * 16 ];
	unsigned int	Word[ 32 ];

	void			configure( void );
	void			sceneToWord( void );
	void			display( void );
	void			send( unsigned int dat );
	void			scan_Line( unsigned int m );
	void			portWrite( int pin, int state );


public:
	bool			Mapping;

	LedMatrixObject(byte LEDARRAY_D, byte LEDARRAY_C, byte LEDARRAY_B, byte LEDARRAY_A, byte LEDARRAY_G, byte LEDARRAY_DI, byte LEDARRAY_CLK, byte LEDARRAY_LAT);
	void			clear( void );
	void			draw( void );
	void			setScene( bool ( *newScene )[ 16 * 16 ] );
	void			setLedOn( int x, int y );
	void			setLedOff( int x, int y );
	void			toggleLed( int x, int y );
};


#endif // LED_MATRIX_OBJECT_H
