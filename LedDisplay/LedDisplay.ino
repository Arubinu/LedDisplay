#include		"LedDisplay.h"
#if ( !ENABLE_DEBUG && !ENABLE_SERIAL && ! ENABLE_BLUETOOTH )
#error			Il vous faut au moins sélectionner un moyen de communication !
#elif ( DISPLAY_NUMBER < 1 || DISPLAY_NUMBER > 6 )
#error			Ce programme est fait pour fonctionner de 1 à 6 afficheurs 16x16
#endif

#if ENABLE_BLUETOOTH
#include		<SoftwareSerial.h>
SoftwareSerial	BTSerial( BLUETOOTH_RX, BLUETOOTH_TX );
#endif

int				pos;
char			*str		= NULL;
unsigned long	save		= 42;

time_t			timestamp;
Timer			*timer		= new Timer( TIMER_TIMEOUT );

LedMatrixObject	*led1		= NULL;
LedMatrixObject	*led2		= NULL;
LedMatrixObject	*led3		= NULL;
LedMatrixObject	*led4		= NULL;
LedMatrixObject	*led5		= NULL;
LedMatrixObject	*led6		= NULL;

void	setup()
{
#if ( ENABLE_DEBUG || ENABLE_SERIAL )
	Serial.begin( SERIAL_SPEED );
#endif
#if ENABLE_BLUETOOTH
	BTSerial.begin( BLUETOOTH_SPEED );
#endif

#if ENABLE_DEBUG
	timeString( __TIMESTAMP__ );
	//timeString( "Mon Jan 11 23:42:42 2016" );
	//timeStamp( "1452555632" );
#endif

	timer->setOnTimer( &decalScreen );
	timer->Start();

	bool screen_void[ 16 * 16 ] = { 0 };

#if ( DISPLAY_NUMBER >= 1 )
	led1 = new LedMatrixObject( DISPLAY_1 );
	led1->Mapping = true;
	led1->setScene( &screen_void );
	led1->draw();
#endif
#if ( DISPLAY_NUMBER >= 2 )
	led2 = new LedMatrixObject( DISPLAY_2 );
	led2->Mapping = true;
	led2->setScene( &screen_void );
	led2->draw();
#endif
#if ( DISPLAY_NUMBER >= 3 )
	led3 = new LedMatrixObject( DISPLAY_3 );
	led3->Mapping = true;
	led3->setScene( &screen_void );
	led3->draw();
#endif
#if ( DISPLAY_NUMBER >= 4 )
	led4 = new LedMatrixObject( DISPLAY_4 );
	led4->Mapping = true;
	led4->setScene( &screen_void );
	led4->draw();
#endif
#if ( DISPLAY_NUMBER >= 5 )
	led5 = new LedMatrixObject( DISPLAY_5 );
	led5->Mapping = true;
	led5->setScene( &screen_void );
	led5->draw();
#endif
#if ( DISPLAY_NUMBER >= 6 )
	led6 = new LedMatrixObject( DISPLAY_6 );
	led6->Mapping = true;
	led6->setScene( &screen_void );
	led6->draw();
#endif

	return ;
}

void	loop()
{
	int		x;
	int		serial			= 0;

#if ( ENABLE_DEBUG || ENABLE_SERIAL )
	serial = Serial.available() ? 1 : serial;
#endif
#if ENABLE_BLUETOOTH
	serial = BTSerial.available() ? 2 : serial;
#endif

	if ( serial )
	{
#if ( ( ENABLE_SERIAL || ENABLE_DEBUG ) && ENABLE_BLUETOOTH )
		String rec = ( serial == 2 ) ? BTSerial.readString() : Serial.readString();
#elif ENABLE_BLUETOOTH
		String rec = BTSerial.readString();
#else
		String rec = Serial.readString();
#endif
		rec.trim();

		if ( rec.length() == ( TIME_LENGTH + strlen( TIME_HEADER ) ) && !strncmp( TIME_HEADER, rec.c_str(), 5 ) )
			timeStamp( rec.c_str() + strlen( TIME_HEADER ) );
		else
		{
			if ( str )
				free( str );

			pos = 16 * DISPLAY_NUMBER;
			str = ( char * )malloc( sizeof( char ) * ( rec.length() + 1 ) );

			memset( str, 0, rec.length() + 1 );
			strcpy( str, rec.c_str() );
		}
	}
	else
		dateUpdate();

	if ( !str )
		return ;

	bool screen[ DISPLAY_NUMBER ][ 16 * 16 ] = { { 0 } };

	x = pos;
	mstringScene( str, pos, 5, screen );

	if ( led1 )
		led1->setScene( &screen[ 0 ] );
	if ( led2 )
		led2->setScene( &screen[ 1 ] );
	if ( led3 )
		led3->setScene( &screen[ 2 ] );
	if ( led4 )
		led4->setScene( &screen[ 3 ] );
	if ( led5 )
		led5->setScene( &screen[ 4 ] );
	if ( led6 )
		led6->setScene( &screen[ 5 ] );

	do
	{
		if ( led1 )
			led1->draw();
		if ( led2 )
			led2->draw();
		if ( led3 )
			led3->draw();
		if ( led4 )
			led4->draw();
		if ( led5 )
			led5->draw();
		if ( led6 )
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

void	dateUpdate( void )
{
	char	two[ 5 ];
	byte	b_time[ 7 ]		= { 0 };
	String	tmp				= "Nous sommes le DAY/MONTH/YEAR et il est HH:MM !";

	if ( !str && DateTime.available() && save < millis() )
	{
		pos = 16 * DISPLAY_NUMBER;

		timestamp = DateTime.now();
		DateTime.localTime( &timestamp, &b_time[ 6 ], &b_time[ 5 ], &b_time[ 4 ], &b_time[ 3 ], &b_time[ 2 ], &b_time[ 1 ], &b_time[ 0 ] );

#if ENABLE_DEBUG
		Serial.print( "dateUpdate: " );
		Serial.println( timestamp );

		for ( int i = 0; i <= 6; ++i )
			Serial.println( b_time[ i ] );
#endif

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

void	decalScreen( void )
{
	pos -= DISPLAY_DECAL;
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

void	timeStamp( const char *str )
{
	timestamp = 0;
	for ( int i = 0; i < TIME_LENGTH; ++i )
	{
		char c = str[ i ];
		if ( c >= '0' && c <= '9' )
			timestamp = ( 10 * timestamp ) + ( c - '0' ) ;
	}

	DateTime.sync( timestamp TIME_ZONE * 3600 );
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

#if ENABLE_DEBUG
	Serial.print( "timeString: " );
	Serial.println( tmp );

	for ( int i = 0; i < 6; ++i )
		Serial.println( i_time[ i ] );
#endif

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

void	mstringScene( char *str, int x, int y, bool ( *screen )[ 16 * 16 ] )
{
	int		len;
	int		decal;

	len = strlen( str );
	for ( int i = 0, j = 0; i < len; ++i, ++j )
	{
		if ( ( unsigned char )str[ i ] == 195 )
		{
			++i;
			str[ i ] = charNExtend( ( unsigned char )str[ i ], 32 );
		}
		mcharScene( str[ i ], ( j * 6 ) + x , y, screen );
	}

	return ;
}

void	charScene( bool *scene, unsigned char c, int x, int y )
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

void	mcharScene( unsigned char c, int x, int y, bool ( *screen )[ 16 * 16 ] )
{
	int		decal;

	for ( int i = 0; i < DISPLAY_NUMBER; ++i )
	{
		decal = x - ( 16 * i );
		if ( decal > -5 && decal < ( 16 * DISPLAY_NUMBER ) )
			charScene( screen[ i ], c, decal, y );
	}

	return ;
}

int		reversePixel( int x, int y )
{
	return ( ( 16 * x ) + y );
}

char	charNExtend( unsigned char c, char diff )
{
	int		x				= c + diff;

	x = ( ( x >= 192 && x <= 198 ) || ( x >= 224 && x <= 230 ) ) ? 'A' : x;
	x = ( ( x >= 200 && x <= 203 ) || ( x >= 232 && x <= 235 ) ) ? 'E' : x;
	x = ( ( x >= 204 && x <= 207 ) || ( x >= 236 && x <= 239 ) ) ? 'I' : x;
	x = ( ( x >= 210 && x <= 214 ) || ( x >= 242 && x <= 246 ) ) ? 'O' : x;
	x = ( ( x >= 217 && x <= 220 ) || ( x >= 249 && x <= 252 ) ) ? 'U' : x;
	x = ( x == 199 || x == 231 ) ? 'C' : x;
	x = ( x == 209 || x == 241 ) ? 'N' : x;

	if ( x == ( c + diff ) )
		return ( ( char )c );

	return ( ( char )x );
}