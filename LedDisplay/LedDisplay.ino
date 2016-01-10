#include		<avr/pgmspace.h>

#include		"Timer.h"
Timer			*timer					= new Timer( 42 );

#include		"ascii.h"
#include		"LedMatrixObject.h"

LedMatrixObject	*led1					= new LedMatrixObject( 14, 15, 16, 17, 18, 19, 20, 21 );
LedMatrixObject	*led2					= new LedMatrixObject( 9, 8, 7, 6, 5, 4, 3, 2 );
LedMatrixObject	*led3					= new LedMatrixObject( 23, 25, 27, 29, 31, 33, 35, 37 );
LedMatrixObject	*led4					= new LedMatrixObject( 39, 41, 43, 45, 47, 49, 51, 53 );
LedMatrixObject	*led5					= new LedMatrixObject( A0, A1, A2, A3, A4, A5, A6, A7 );
LedMatrixObject	*led6					= new LedMatrixObject( A8, A9, A10, A11, A12, A13, A14, A15 );

bool			debug;

int				pos;
int				length;
char			*str					= NULL;
unsigned char	screen[ 16 ][ 16 ]		= { { 0 } };

void	setup()
{
	Serial.begin( 9600 );

	debug = true;
	length = sizeof( bool ) * ( 16 * 16 );

	led1->Mapping = true;
	led2->Mapping = true;
	led3->Mapping = true;
	led4->Mapping = true;
	led5->Mapping = true;
	led6->Mapping = true;

	timer->setOnTimer( &decalScreen );
	timer->Start();

	return ;
}

unsigned long next = 0;

void	loop()
{
	int		x;

	if ( Serial.available() )
	{
		if ( str )
			free( str );

		String rec = Serial.readString();
		rec.trim();

		pos = 16 * 6;
		str = ( char * )malloc( sizeof( char ) * ( rec.length() + 1 ) );

		memset( str, 0, rec.length() + 1 );
		strcpy( str, rec.c_str() );
	}
	else if ( debug )
	{
		if ( !str )
		{
			pos = 16 * 6;

			char tmp[] = "Nous sommes le Dimanche 10 Janvier 2016 !";
			str = ( char * )malloc( sizeof( char ) * ( strlen( tmp ) + 1 ) );

			memset( str, 0, strlen( tmp ) + 1 );
			strcpy( str, tmp );
		}
	}

	if ( !str )
		return ;

	bool	screen1[ 16 * 16 ]	= { 0 };
	bool	screen2[ 16 * 16 ]	= { 0 };
	bool	screen3[ 16 * 16 ]	= { 0 };
	bool	screen4[ 16 * 16 ]	= { 0 };
	bool	screen5[ 16 * 16 ]	= { 0 };
	bool	screen6[ 16 * 16 ]	= { 0 };

	x = pos;
	do
	{
		timer->Update();
		mstringScene( str, pos, 5, screen1, screen2, screen3, screen4, screen5, screen6 );

		screenScene( ( bool * )screen1 );
		led1->setScene( screen );

		screenScene( ( bool * )screen2 );
		led2->setScene( screen );

		screenScene( ( bool * )screen3 );
		led3->setScene( screen );

		screenScene( ( bool * )screen4 );
		led4->setScene( screen );

		screenScene( ( bool * )screen5 );
		led5->setScene( screen );

		screenScene( ( bool * )screen6 );
		led6->setScene( screen );

		led1->draw();
		led2->draw();
		led3->draw();
		led4->draw();
		led5->draw();
		led6->draw();
	}
	while ( pos == x );

	if ( pos < cstringScene( str ) )
	{
		free( str );
		str = NULL;
	}

	return ;
}

void	decalScreen()
{
	--pos;
}

void	mcharScene( char c, int x, int y, bool *screen1, bool *screen2, bool *screen3, bool *screen4, bool *screen5, bool *screen6 )
{
	charScene( screen1, c, x, y );
	charScene( screen2, c, x - 16, y );
	charScene( screen3, c, x - 32, y );
	charScene( screen4, c, x - 48, y );
	charScene( screen5, c, x - 64, y );
	charScene( screen6, c, x - 80, y );
	return ;
}

int		cstringScene( char *str )
{
	int		len;
	int		decal;

	len = strlen( str );
	decal = len * -6;

	return ( decal );
}

void	mstringScene( char *str, int x, int y, bool *screen1, bool *screen2, bool *screen3, bool *screen4, bool *screen5, bool *screen6 )
{
	int		len;
	int		decal;

	len = strlen( str );
	for ( int i = 0; i < len; ++i )
		mcharScene( str[ i ], ( i * 6 ) + x , y, screen1, screen2, screen3, screen4, screen5, screen6 );

	return ;
}

void	screenScene( bool *scene )
{
	for ( int j = 0; j < 16; ++j )
	{
		for ( int i = 0; i < 16; ++i )
			screen[ j ][ i ] = scene[ ( 16 * j ) + i ] ? 0 : 1;
	}

	return ;
}

void	charScene( bool *scene, char c, int x, int y )
{
	int		d;

	if ( x >= 16 || y >= 16 || c <= 32 || c >= 127 )
		return ;

	if ( isalpha( c ) )
		c = toupper( c );
	else if ( c > 96 )
		c -= 26;

	x = 11 - x;
	d = ( c - 33 ) * 7 * 5;

	for ( int j = 0; j < 7; ++j )
	{
		for ( int i = 0; i < 5; ++i )
		{
			int decal = i + x;
			if ( decal >= 0 && decal < 16 && pgm_read_byte_near( ascii + d + ( j * 5 ) + ( 4 - i ) ) )
				scene[ reversePixel( decal, ( j + y ) ) ] = 1;
		}
	}

	return ;
}

void	reverseScene( bool *scene )
{
	bool	temp[ 16 * 16 ] = { 0 };

	for ( int j = 0; j < 16; ++j )
	{
		for ( int i = 0; i < 16; ++i )
			temp[ ( 16 * i ) + j ] = scene[ ( 16 * j ) + i ];
	}

	memcpy( ( void * )scene, ( const void * )temp, length );
	return ;
}

int		reversePixel( int x, int y )
{
	return ( ( 16 * x ) + y );
}