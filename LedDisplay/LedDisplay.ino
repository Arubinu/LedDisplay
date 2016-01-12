#include		<avr/pgmspace.h>

#include		<SoftwareSerial.h>
SoftwareSerial	BTSerial( 10, 11 );

#include		<DateTime.h>
time_t			timestamp;
#define			TIME_LENGTH				10
#define			TIME_HEADER				"date="

#include		"Timer.h"
Timer			*timer					= new Timer( 42 );

#include		"ascii.h"
#include		"LedMatrixObject.h"

LedMatrixObject	*led1					= new LedMatrixObject( 14, 15, 16, 17, 18, 19, 20, 21 );
LedMatrixObject	*led2					= new LedMatrixObject( 9, 8, 7, 6, 12, 4, 3, 2 );
LedMatrixObject	*led3					= new LedMatrixObject( 23, 25, 27, 29, 31, 33, 35, 37 );
LedMatrixObject	*led4					= new LedMatrixObject( 39, 41, 43, 45, 47, 49, 51, 53 );
LedMatrixObject	*led5					= new LedMatrixObject( A0, A1, A2, A3, A4, A5, A6, A7 );
LedMatrixObject	*led6					= new LedMatrixObject( A8, A9, A10, A11, A12, A13, A14, A15 );

bool			debug;

int				pos;
char			*str					= NULL;
unsigned long	save					= 42;
unsigned char	screen[ 16 ][ 16 ]		= { { 0 } };

void	setup()
{
	Serial.begin( 9600 );
	BTSerial.begin( 9600 );

	debug = false;
	if ( debug )
	{
		timeString( __TIMESTAMP__ );
		//timeString( "Mon Jan 11 23:42:42 2016" );
		//timeStamp( "1452555632" );
	}

	led1->Mapping = true;
	led2->Mapping = true;
	led3->Mapping = true;
	led4->Mapping = true;
	led5->Mapping = true;
	led6->Mapping = true;

	timer->setOnTimer( &decalScreen );
	timer->Start();

	bool screen_void[ 16 * 16 ] = { 0 };

	led1->setScene( &screen_void );
	led2->setScene( &screen_void );
	led3->setScene( &screen_void );
	led4->setScene( &screen_void );
	led5->setScene( &screen_void );
	led6->setScene( &screen_void );

	led1->draw();
	led2->draw();
	led3->draw();
	led4->draw();
	led5->draw();
	led6->draw();

	return ;
}

void	loop()
{
	int		x;
	int		serial			= 0;

	serial = Serial.available() ? 1 : serial;
	serial = BTSerial.available() ? 2 : serial;

	if ( serial )
	{
		String rec = ( serial == 2 ) ? BTSerial.readString() : Serial.readString();
		rec.trim();

		if ( rec.length() == ( TIME_LENGTH + strlen( TIME_HEADER ) ) && !strncmp( TIME_HEADER, rec.c_str(), 5 ) )
			timeStamp( rec.c_str() + strlen( TIME_HEADER ) );
		else
		{
			if ( str )
				free( str );

			pos = 16 * 6;
			str = ( char * )malloc( sizeof( char ) * ( rec.length() + 1 ) );

			memset( str, 0, rec.length() + 1 );
			strcpy( str, rec.c_str() );
		}
	}
	else
		dateUpdate();

	if ( !str )
		return ;

	bool screen1[ 16 * 16 ] = { 0 };
	bool screen2[ 16 * 16 ] = { 0 };
	bool screen3[ 16 * 16 ] = { 0 };
	bool screen4[ 16 * 16 ] = { 0 };
	bool screen5[ 16 * 16 ] = { 0 };
	bool screen6[ 16 * 16 ] = { 0 };

	x = pos;
	mstringScene( str, pos, 5, screen1, screen2, screen3, screen4, screen5, screen6 );

	led1->setScene( &screen1 );
	led2->setScene( &screen2 );
	led3->setScene( &screen3 );
	led4->setScene( &screen4 );
	led5->setScene( &screen5 );
	led6->setScene( &screen6 );

	do
	{
		led1->draw();
		led2->draw();
		led3->draw();
		led4->draw();
		led5->draw();
		led6->draw();

		timer->Update();
	}
	while ( pos == x );

	if ( str && pos < cstringScene( str ) )
	{
		free( str );
		str = NULL;
		save = millis() + 10000;
	}

	return ;
}

char	*toTwo( char *str, int num )
{
	return ( toTwo( str, num, 2 ) );
}

char	*toTwo( char *str, int num, int count )
{
	memset( str, 0, count + 1 );
	--count;

	for ( int i = count; i >= 0; --i )
	{
		if ( count - i == 0 )
			str[ i ] = '0' + ( num % 10 );
		else
			str[ i ] = '0' + ( num / ( int )pow( 10, count - i ) % 10 );
	}

	return ( str );
}

void	dateUpdate( void )
{
	char	two[ 5 ];
	byte	b_time[ 7 ]		= { 0 };
	String	tmp				= "Nous sommes le DAY/MONTH/YEAR et il est HH:MM !";

	if ( !str && DateTime.available() && save < millis() )
	{
		pos = 16 * 6;

		timestamp = DateTime.now();
		DateTime.localTime( &timestamp, &b_time[ 6 ], &b_time[ 5 ], &b_time[ 4 ], &b_time[ 3 ], &b_time[ 2 ], &b_time[ 1 ], &b_time[ 0 ] );

		if ( debug )
		{
			Serial.print( "dateUpdate: " );
			Serial.println( timestamp );

			for ( int i = 0; i <= 6; ++i )
				Serial.println( b_time[ i ] );
		}

		tmp.replace( "HH", toTwo( two, b_time[ 4 ] ) );
		tmp.replace( "MM", toTwo( two, b_time[ 5 ] ) );

		tmp.replace( "DAY", toTwo( two, b_time[ 3 ] ) );
		tmp.replace( "MONTH", toTwo( two, b_time[ 1 ] + 1 ) );

		tmp.replace( "YEAR", toTwo( two, b_time[ 0 ] + 1900, 4 ) );


		if ( str )
			free( str );
		str = ( char * )malloc( sizeof( char ) * ( tmp.length() + 1 ) );

		memset( str, 0, tmp.length() + 1 );
		strcpy( str, tmp.c_str() );
	}

	return ;
}

void	timeString( const char *str )
{
	int		i_time[ 6 ]		= { 0 };
	char	s_month[]		= "JanFebMarAprMaiJunJilAugSepOctNovDec";

	// Année
	for ( int i = 0; i < 4; ++i )
		i_time[ 0 ] += ( str[ 20 + i ] - '0' ) * pow( 10, 3 - i );

	// Mois
	for ( int i = 0; i < 12; ++i )
	{
		if ( !strncmp( &str[ 4 ], &s_month[ i * 3 ], 3 ) )
		{
			i_time[ 1 ] += i;
			break ;
		}
	}

	for ( int i = 0; i < 2; ++i )
	{
		i_time[ 2 ] += ( str[ 8 + i ] - '0' ) * ( i ? 1 : 10 ); // Jours
		i_time[ 3 ] += ( str[ 11 + i ] - '0' ) * ( i ? 1 : 10 ); // Heures
		i_time[ 4 ] += ( str[ 14 + i ] - '0' ) * ( i ? 1 : 10 ); // Minutes
		i_time[ 5 ] += ( str[ 17 + i ] - '0' ) * ( i ? 1 : 10 ); // Secondes
	}

	time_t tmp = DateTime.makeTime( i_time[ 5 ], i_time[ 4 ], i_time[ 3 ], i_time[ 2 ], i_time[ 1 ], i_time[ 0 ] + 1 );
	DateTime.sync( tmp );

	if ( debug )
	{
		Serial.print( "timeString: " );
		Serial.println( tmp );

		for ( int i = 0; i < 6; ++i )
			Serial.println( i_time[ i ] );
	}

	return ;
}

void	timeStamp( const char *str )
{
	timestamp = 0;
	for ( int i = 0; i < TIME_LENGTH; ++i )
	{
		char c = str[ i ];
		if ( c >= '0' && c <= '9' )
			timestamp = ( 10 * timestamp ) + ( c - '0' ) ;
	}

	DateTime.sync( timestamp + 3600 ); // +1 ( fuseau horaire )
	return ;
}

void	decalScreen()
{
	pos -= 2;
	return ;
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
		for ( int i = 5; i < 12; ++i ) // réduit pour les leds de défilement
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

int		reversePixel( int x, int y )
{
	return ( ( 16 * x ) + y );
}