#include <cstdio>
#include <filesystem>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include <png.h>
#include "bitmap.h"
#define conv( a ) ( (u8) ( (a) *31 / 255 ) )
#define revconv( a ) ( ( (a) *255 / 31 ) )

#define green( a ) ( revconv( ( ( a ) >> 10 ) & 31 ) )
#define blue( a ) ( revconv( ( ( a ) >> 5 ) & 31 ) )
#define red( a ) ( revconv( (a) &31 ) )
using namespace std;
namespace fs = filesystem;

u8             image_data[ 256 * 256 * 20 / 4 + 100 ] = { 0 };
unsigned short pal[ 300 ]                             = { 0 };

typedef tuple<u8, u8, u8> t3;
map<unsigned short, u8>   palidx;
typedef unsigned short    u16;

void print_tiled( FILE* p_f, u8* p_image_data, u8 p_width, u8 p_height ) {
    // Tile the given sprite into 8x8 blocks
    for( size_t y = 0; y < p_height; y += 8 ) {
        for( size_t x = 0; x < p_width; x += 4 ) {
            for( size_t by = 0; by < 8; ++by ) {
                //                printf( "%u -> %u\n", ( y + by ) * 16 + x, ( y + by ) * 16 + x +
                //                4);
                fwrite( p_image_data + ( y + by ) * 16 + x, 1, 4, p_f );
            }
        }
    }
}

void print( u16 p_color, u8 p_msg ) {
    printf( "\x1b[48;2;%u;%u;%um%2hx\x1b[0;00m", red( p_color ), blue( p_color ), green( p_color ),
            p_msg );
}

int main( int p_argc, char** p_argv ) {
    if( p_argc < 2 ) {
        printf( "Too few arguments.\n" );
        return 1;
    }

    u16 pal[ 16 ];

    u8 frames = 1, sx = 32, sy = 32;
    if( p_argc >= 3 ) { sscanf( p_argv[ 2 ], "%hhu", &sx ); }
    if( p_argc >= 4 ) { sscanf( p_argv[ 3 ], "%hhu", &sy ); }
    if( p_argc >= 5 ) { sscanf( p_argv[ 4 ], "%hhu", &frames ); }

    u8 image[ 128 * 128 / 2 ];
    u8 image_data[ 128 * 128 / 2 ];

    FILE* fin = fopen( p_argv[ 1 ], "r" );
    fread( pal, sizeof( unsigned short int ), 16, fin );

    for( size_t fr = 0; fr < frames; ++fr ) {
        fread( image, sizeof( u8 ), sx * sy / 2, fin );
        size_t i = 0;
        for( size_t tiley = 0; tiley < sy / 8; ++tiley ) {
            for( size_t tilex = 0; tilex < sx / 8; ++tilex ) {
                int shift = tilex * ( -28 );
                for( u8 y = 0; y < 8; ++y ) {
                    for( u8 x = 0; x < 8; x += 2, ++i ) {
                        u16 cur                 = image[ i ];
                        image_data[ i + shift ] = cur;
                    }
                    shift += 4 * ( sx / 8 - 1 );
                }
            }
        }

        for( size_t x = 0; x < sx * sy / 2; ++x ) {
            print( pal[ image_data[ x ] & 0xF ], image_data[ x ] & 0xF );
            print( pal[ image_data[ x ] >> 4 ], image_data[ x ] >> 4 );
            if( ( x & ( sx / 2 - 1 ) ) == sx / 2 - 1 ) printf( "\n" );
        }
        printf( "\n" );
    }

    printf( "Palette:\n" );
    for( u8 i = 0; i < 16; ++i ) { print( pal[ i ], i ); }
    printf( "\n\n" );

    fclose( fin );
}
