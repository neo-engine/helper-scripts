// reads a p2l and adds tinted versions of the colors for different daytimes

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "fsdata.h"

using namespace std;

#define conv( a ) ( ( uint8_t )( ( (a) &0xff ) * 31 / 255 ) )
#define revconv( a ) ( ( (a) *255 / 31 ) )
#define green( a ) ( revconv( ( ( a ) >> 10 ) & 31 ) )
#define blue( a ) ( revconv( ( ( a ) >> 5 ) & 31 ) )
#define red( a ) ( revconv( (a) &31 ) )

constexpr uint8_t DAYTIME_DAY     = 2;
constexpr uint8_t DAYTIME_DUSK    = 3;
constexpr uint8_t DAYTIME_EVENING = 4;
constexpr uint8_t DAYTIME_NIGHT   = 0;
constexpr uint8_t DAYTIME_MORNING = 1;

constexpr uint16_t tints[ 5 ] = {
    0x0000, // day (no tint)
    0x3672, // dusk
    0x24a4, // evening
    0x1c44, // night
    0x3f1c, // morning
};
constexpr uint16_t tint_fac[ 5 ] = { 400, 25, 35, 18, 30 };

constexpr uint16_t _32to16( uint32_t p_in ) {
    // result must be AGGGGGBBBBBRRRRR
    // input is RGBA

    return ( conv( p_in ) ) | ( conv( p_in >> 16 ) << 10 ) | ( conv( p_in >> 8 ) << 5 );
    //           | ( 1 << 15 );
}

constexpr uint16_t _tint( uint16_t p_color, uint16_t p_tint, uint8_t p_tintFactor = 20 ) {
    return ( p_color & ( 1 << 15 ) )
           | ( ( p_color & 31 )
               + ( ( p_tint & 31 ) - ( p_color & 31 ) ) * 10 / ( p_tintFactor - 3 ) )
           | ( ( ( ( p_color >> 5 ) & 31 )
                 + ( ( ( p_tint >> 5 ) & 31 ) - ( ( p_color >> 5 ) & 31 ) ) * 10
                       / ( p_tintFactor + 2 ) )
               << 5 )
           | ( ( ( ( p_color >> 10 ) & 31 )
                 + ( ( ( p_tint >> 10 ) & 31 ) - ( ( p_color >> 10 ) & 31 ) ) * 10 / p_tintFactor )
               << 10 );
}

constexpr uint16_t _swap( uint16_t p_color ) {

    auto mx
        = std::max( ( p_color & 31 ), std::max( ( p_color >> 5 ) & 31, ( p_color >> 10 ) & 31 ) );
    auto mn
        = std::min( ( p_color & 31 ), std::min( ( p_color >> 5 ) & 31, ( p_color >> 10 ) & 31 ) );

    return ( p_color & ( 1 << 15 ) ) | ( mn << 10 ) | mx | ( mn << 5 );
}

int main( int p_argc, char *p_argv[] ) {
    if( p_argc < 2 ) {
        fprintf( stderr, "Usage %s P2L OUTPUT [dusk P2L] [eve P2L] [night P2L] [morning P2L]",
                 p_argv[ 0 ] );
        return 1;
    }

    uint16_t palin[ 16 * 8 * 5 ]  = { 0 };
    uint16_t palout[ 16 * 8 * 5 ] = { 0 };

    FILE *f = fopen( p_argv[ 1 ], "r" );
    fread( palin, sizeof( uint16_t ), 16 * 8, f );
    fclose( f );

    for( u8 i = 0; p_argc >= 4 && i < p_argc - 3 && i < 4; ++i ) {
        f = fopen( p_argv[ 3 + i ], "r" );
        fread( palin + 16 * 8 * ( i + 1 ), sizeof( uint16_t ), 16 * 8, f );
        fclose( f );
    }

    // display pal

    for( uint8_t i = 0; i < 8; ++i ) {
        for( uint8_t j = 0; j < 16; ++j ) {
            unsigned short color = palin[ 16 * i + j ];
            palout[ 16 * i + j ] = color; // daytime, unchanged
            for( uint8_t k = 1; k < 5; ++k ) {
                if( palin[ 16 * i + j + 16 * 8 * k ] ) {
                    // don't overwrite existing pals
                    palout[ 16 * i + j + 16 * 8 * k ] = palin[ 16 * i + j + 16 * 8 * k ];
                    continue;
                }
                palout[ 16 * i + j + 16 * 8 * k ]
                    //                    = k == 2 ? _swap( color ) : _tint( color, tints[ k ],
                    //                    tint_fac[ k ] );
                    = ( k == 1 )
                          ? _tint( _swap( color ), tints[ k ], tint_fac[ k ] )
                          : _tint( color, tints[ k ],
                                   ( j <= 3 ? 5 : 0 ) + ( j == 15 ? -5 : 0 ) + tint_fac[ k ] );
            }
        }
    }

    for( uint8_t k = 0; k < 5; ++k ) {
        for( uint8_t i = 0; i < 8; ++i ) {
            for( uint8_t j = 0; j < 16; ++j ) {
                unsigned short color = palout[ 16 * i + j + 16 * 8 * k ];
                fprintf( stderr, "\x1b[48;2;%u;%u;%um%04hx\x1b[0;00m", red( color ), blue( color ),
                         green( color ), color );
            }
            fprintf( stderr, "\n" );
        }
        fprintf( stderr, "\n" );
    }

    if( p_argc >= 3 ) {
        f = fopen( p_argv[ 2 ], "w" );
        fwrite( palout, 16 * 8 * 5, sizeof( uint16_t ), f );
        fclose( f );
    }
}
