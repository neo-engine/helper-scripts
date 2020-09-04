// Converts (and combines) multiple palettes as exported from AM to a single p2l
// palette.

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

#define conv( a ) ( ( uint8_t )( ( (a) &0xff ) * 31 / 255 ) )

constexpr uint16_t _32to16( uint32_t p_in ) {
    // result must be AGGGGGBBBBBRRRRR
    // input is RGBA

    return ( conv( p_in ) ) | ( conv( p_in >> 16 ) << 10 ) | ( conv( p_in >> 8 ) << 5 ) ;
//           | ( 1 << 15 );
}

int main( int p_argc, char *p_argv[] ) {
    if( p_argc < 7 ) {
        fprintf( stderr, "Expecting 6 AdvanceMap pals and a file name to write the output to." );
        return 1;
    }

    uint32_t palin[ 16 ];
    uint16_t palout[ 16 * 8 ] = { 0 };

    for( int i = 0; i < min( 8, p_argc - 2 ); ++i ) {
        FILE *f = fopen( p_argv[ i + 1 ], "r" );
        fread( palin, sizeof( uint32_t ), 16, f );
        for( int j = 0; j < 16; ++j ) { palout[ 16 * i + j ] = _32to16( palin[ j ] ); }
        fclose( f );
    }

    FILE *f = fopen( p_argv[ p_argc - 1 ], "w" );
    fwrite( palout, 16 * 8, sizeof( uint16_t ), f );
    fclose( f );
}
