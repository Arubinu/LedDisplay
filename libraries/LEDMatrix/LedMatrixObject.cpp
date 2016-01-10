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


#include "LedMatrixObject.h"

LedMatrixObject::LedMatrixObject(
			byte LEDARRAY_D, byte LEDARRAY_C, 
			byte LEDARRAY_B, byte LEDARRAY_A, 
			byte LEDARRAY_G, byte LEDARRAY_DI, 
			byte LEDARRAY_CLK, byte LEDARRAY_LAT){

	this->LEDARRAY_D = LEDARRAY_D;
	this->LEDARRAY_C = LEDARRAY_C;
	this->LEDARRAY_B = LEDARRAY_B;
	this->LEDARRAY_A = LEDARRAY_A;
	this->LEDARRAY_G = LEDARRAY_G;
	this->LEDARRAY_DI = LEDARRAY_DI;
	this->LEDARRAY_CLK = LEDARRAY_CLK;
	this->LEDARRAY_LAT = LEDARRAY_LAT;

	this->Mapping = false;

	configure();
}


void LedMatrixObject::configure(){
	pinMode(LEDARRAY_D, OUTPUT); 
	pinMode(LEDARRAY_C, OUTPUT);
	pinMode(LEDARRAY_B, OUTPUT);
	pinMode(LEDARRAY_A, OUTPUT);
	pinMode(LEDARRAY_G, OUTPUT);
	pinMode(LEDARRAY_DI, OUTPUT);
	pinMode(LEDARRAY_CLK, OUTPUT);
	pinMode(LEDARRAY_LAT, OUTPUT);
}

void LedMatrixObject::clear(){
	for(int i = 0; i < 32; i++)
		Word1[i] = 0;
}

void LedMatrixObject::clearScene(){
	for(byte i = 0; i < 16; i++)
		for(byte k = 0; k < 16; k++)
			this->Scene[i][k] = 1;
}

void LedMatrixObject::sceneToWord(){
	//clear word
	clear();

	int i, k, key = 0;
	unsigned int value;
	for(i = 0; i < 16; i++){
		for(k = 0; k < 16; k++){
			if(i < 8){
				value = Scene[i][k] << (7 - i);
				Word1[15 - k] += value;
			} else {
				value = Scene[i][k] << (15 - i);
				Word1[31 - k] += value;
			}
		}
	}
}

void LedMatrixObject::draw(){
	sceneToWord();
	display();
	clearScene();
}

void LedMatrixObject::display(){
	unsigned char i;

	for( i = 0 ; i < 16 ; i++ )
	{
		this->digitalWrite(LEDARRAY_G, HIGH);		
		
		Display_Buffer[0] = Word1[i];		
		Display_Buffer[1] = Word1[i+16];

		send(Display_Buffer[1]);
		send(Display_Buffer[0]);

		this->digitalWrite(LEDARRAY_LAT, HIGH);					 
		delayMicroseconds(1);
	
		this->digitalWrite(LEDARRAY_LAT, LOW);
		delayMicroseconds(1);

		scan_Line(i);

		this->digitalWrite(LEDARRAY_G, LOW);
		
		delayMicroseconds(100);		
	}	
}

void LedMatrixObject::send(unsigned int dat) {
	unsigned char i;
	this->digitalWrite(LEDARRAY_CLK, LOW);
	delayMicroseconds(1);;	
	this->digitalWrite(LEDARRAY_LAT, LOW);
	delayMicroseconds(1);;

	for( i = 0 ; i < 8 ; i++ )
	{
		if( dat&0x01 )
		{
			this->digitalWrite(LEDARRAY_DI, HIGH);	
		}
		else
		{
			this->digitalWrite(LEDARRAY_DI, LOW);
		}

		delayMicroseconds(1);
		this->digitalWrite(LEDARRAY_CLK, HIGH);				  
			delayMicroseconds(1);
		this->digitalWrite(LEDARRAY_CLK, LOW);
			delayMicroseconds(1);		
		dat >>= 1;
			
	}			
}


void LedMatrixObject::scan_Line(unsigned int m) {	
	switch(m)
	{
		case 0:			
			this->digitalWrite(LEDARRAY_D, LOW);this->digitalWrite(LEDARRAY_C, LOW);this->digitalWrite(LEDARRAY_B, LOW);this->digitalWrite(LEDARRAY_A, LOW); 					
			break;
		case 1:					
			this->digitalWrite(LEDARRAY_D, LOW);this->digitalWrite(LEDARRAY_C, LOW);this->digitalWrite(LEDARRAY_B, LOW);this->digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 2:					
			this->digitalWrite(LEDARRAY_D, LOW);this->digitalWrite(LEDARRAY_C, LOW);this->digitalWrite(LEDARRAY_B, HIGH);this->digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 3:					
			this->digitalWrite(LEDARRAY_D, LOW);this->digitalWrite(LEDARRAY_C, LOW);this->digitalWrite(LEDARRAY_B, HIGH);this->digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 4:
			this->digitalWrite(LEDARRAY_D, LOW);this->digitalWrite(LEDARRAY_C, HIGH);this->digitalWrite(LEDARRAY_B, LOW);this->digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 5:
			this->digitalWrite(LEDARRAY_D, LOW);this->digitalWrite(LEDARRAY_C, HIGH);this->digitalWrite(LEDARRAY_B, LOW);this->digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 6:
			this->digitalWrite(LEDARRAY_D, LOW);this->digitalWrite(LEDARRAY_C, HIGH);this->digitalWrite(LEDARRAY_B, HIGH);this->digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 7:
			this->digitalWrite(LEDARRAY_D, LOW);this->digitalWrite(LEDARRAY_C, HIGH);this->digitalWrite(LEDARRAY_B, HIGH);this->digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 8:
			this->digitalWrite(LEDARRAY_D, HIGH);this->digitalWrite(LEDARRAY_C, LOW);this->digitalWrite(LEDARRAY_B, LOW);this->digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 9:
			this->digitalWrite(LEDARRAY_D, HIGH);this->digitalWrite(LEDARRAY_C, LOW);this->digitalWrite(LEDARRAY_B, LOW);this->digitalWrite(LEDARRAY_A, HIGH); 		
			break;	
		case 10:
			this->digitalWrite(LEDARRAY_D, HIGH);this->digitalWrite(LEDARRAY_C, LOW);this->digitalWrite(LEDARRAY_B, HIGH);this->digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 11:
			this->digitalWrite(LEDARRAY_D, HIGH);this->digitalWrite(LEDARRAY_C, LOW);this->digitalWrite(LEDARRAY_B, HIGH);this->digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 12:
			this->digitalWrite(LEDARRAY_D, HIGH);this->digitalWrite(LEDARRAY_C, HIGH);this->digitalWrite(LEDARRAY_B, LOW);this->digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 13:
			this->digitalWrite(LEDARRAY_D, HIGH);this->digitalWrite(LEDARRAY_C, HIGH);this->digitalWrite(LEDARRAY_B, LOW);this->digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 14:
			this->digitalWrite(LEDARRAY_D, HIGH);this->digitalWrite(LEDARRAY_C, HIGH);this->digitalWrite(LEDARRAY_B, HIGH);this->digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 15:
			this->digitalWrite(LEDARRAY_D, HIGH);this->digitalWrite(LEDARRAY_C, HIGH);this->digitalWrite(LEDARRAY_B, HIGH);this->digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		default : break;	
	}
}

void LedMatrixObject::setScene(unsigned char  Scene[16][16]){
	for(byte i = 0; i < 16; i++)
		for(byte k = 0; k < 16; k++)
			this->Scene[i][k] = Scene[i][k];
}

void LedMatrixObject::setLedOn(int x, int y){
	Scene[x][y] = 0;
}

void LedMatrixObject::setLedOff(int x, int y){
	Scene[x][y] = 1;
}

void LedMatrixObject::toggleLed(int x, int y){
	Scene[x][y] = !Scene[x][y];
}

void LedMatrixObject::digitalWrite(int pin, int state){
	if ( !this->Mapping )
	{
		digitalWrite( pin, state );
		return ;
	}

#if defined(__AVR_ATmega168__) ||defined(__AVR_ATmega168P__) ||defined(__AVR_ATmega328P__)
	if ( pin >= 0 && pin <= 7 )
	{
		if ( state )
			PORTD |= ( 1 << pin );
		else
			PORTD &= ~( 1 << pin );
	}
	else if ( pin >= 8 && pin <= 13 )
	{
		pin -= 8;
		if ( state )
			PORTB |= ( 1 << pin );
		else
			PORTB &= ~( 1 << pin );
	}
	else if ( pin >= 14 && pin <= 21 )
	{
		pin -= 14;
		if ( state )
			PORTC |= ( 1 << pin );
		else
			PORTC &= ~( 1 << pin );
	}
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	if ( ( pin >= 0 && pin <= 3 ) || pin == 5 )
	{
		pin -= ( pin == 5 ) ? 2 : 0;
		pin += ( pin >= 2 && pin <= 3 ) ? 2 : 0;
		if ( state )
			PORTE |= ( 1 << pin );
		else
			PORTE &= ~( 1 << pin );
	}
	else if ( pin == 4 )
	{
		if ( state )
			PORTG |= ( 1 << 5 );
		else
			PORTG &= ~( 1 << 5 );
	}
	else if ( pin >= 6 && pin <= 9 )
	{
		pin -= 3;
		if ( state )
			PORTH |= ( 1 << pin );
		else
			PORTH &= ~( 1 << pin );
	}
	else if ( pin >= 10 && pin <= 13 )
	{
		pin -= 6;
		if ( state )
			PORTB |= ( 1 << pin );
		else
			PORTB &= ~( 1 << pin );
	}
	else if ( pin >= 14 && pin <= 15 )
	{
		pin -= 14;
		pin = 1 - pin;
		if ( state )
			PORTJ |= ( 1 << pin );
		else
			PORTJ &= ~( 1 << pin );
	}
	else if ( pin >= 16 && pin <= 17 )
	{
		pin -= 16;
		pin = 1 - pin;
		if ( state )
			PORTH |= ( 1 << pin );
		else
			PORTH &= ~( 1 << pin );
	}
	else if ( pin >= 18 && pin <= 21 )
	{
		pin -= 18;
		pin = 3 - pin;
		if ( state )
			PORTD |= ( 1 << pin );
		else
			PORTD &= ~( 1 << pin );
	}
	else if ( pin >= 22 && pin <= 29 )
	{
		pin -= 22;
		if ( state )
			PORTA |= ( 1 << pin );
		else
			PORTA &= ~( 1 << pin );
	}
	else if ( pin >= 30 && pin <= 37 )
	{
		pin -= 30;
		pin = 7 - pin;
		if ( state )
			PORTC |= ( 1 << pin );
		else
			PORTC &= ~( 1 << pin );
	}
	else if ( pin == 38 )
	{
		if ( state )
			PORTD |= ( 1 << 7 );
		else
			PORTD &= ~( 1 << 7 );
	}
	else if ( pin >= 39 && pin <= 41 )
	{
		pin -= 39;
		pin = 2 - pin;
		if ( state )
			PORTG |= ( 1 << pin );
		else
			PORTG &= ~( 1 << pin );
	}
	else if ( pin >= 42 && pin <= 49 )
	{
		pin -= 42;
		pin = 7 - pin;
		if ( state )
			PORTL |= ( 1 << pin );
		else
			PORTL &= ~( 1 << pin );
	}
	else if ( pin >= 50 && pin <= 53 )
	{
		pin -= 50;
		pin = 3 - pin;
		if ( state )
			PORTB |= ( 1 << pin );
		else
			PORTB &= ~( 1 << pin );
	}
	else if ( pin >= 54 && pin <= 61 )
	{
		pin -= 54;
		if ( state )
			PORTF |= ( 1 << pin );
		else
			PORTF &= ~( 1 << pin );
	}
	else if ( pin >= 62 && pin <= 69 )
	{
		pin -= 62;
		if ( state )
			PORTK |= ( 1 << pin );
		else
			PORTK &= ~( 1 << pin );
	}
#endif
}