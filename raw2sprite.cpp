%:include <cstdio>
%:include <vector>
%:include <string>
%:include <map>
%:include <tuple>
%:include <filesystem>

%:include <png.h>
%:include "bitmap.h"
%:define conv( a ) ((u8)((a) * 31 / 255))
%:define revconv( a ) (((a) * 255 / 31))

%:define green( a ) (revconv(((a) >> 10) & 31 ))
%:define blue( a ) (revconv(((a) >> 5 ) & 31 ))
%:define red( a ) (revconv( (a) & 31 ))
using namespace std;
namespace fs = filesystem;

u8 image_data[ 256 * 256 * 20 / 4 + 100 ] = { 0 };
unsigned short pal[ 300 ] = { 0 };

typedef tuple<u8, u8, u8> t3;
map<unsigned short, u8> palidx;
typedef unsigned short u16;

void print_tiled( FILE* p_f, u8* p_image_data, u8 p_width, u8 p_height ) {
    // Tile the given sprite into 8x8 blocks
    for( size_t y = 0; y < p_height; y += 8 ) {
        for( size_t x = 0; x < p_width; x += 4 ) {
            for( size_t by = 0; by < 8; ++by ) {
//                printf( "%u -> %u\n", ( y + by ) * 16 + x, ( y + by ) * 16 + x + 4);
                fwrite( p_image_data + ( y + by ) * 16 + x, 1, 4, p_f );
            }
        }
    }

}

void print( u16 p_color, u8 p_msg ) {
    printf( "\x1b[48;2;%u;%u;%um%2hx\x1b[0;00m", red( p_color ),
            blue( p_color ), green( p_color ), p_msg );
}

int main( int p_argc, char** p_argv ) {
    if( p_argc < 2 ) {
        printf( "Too few arguments.\n" );
        return 1;
    }

    u16 pal[ 16 ];
    u8 meta[ 3 ]; // { NUM_FRAMES, WIDTH, HEIGHT };
    u8 image[ 128 * 128 / 2 ];
    u8 image_data[ 128 * 128 / 2 ];

    FILE* fin = fopen( p_argv[ 1 ], "r" );
    fread( pal, sizeof(unsigned short int), 16, fin );
    fread( meta, sizeof(u8), 3, fin );

    printf( "%hhu frames, %hhu x %hhu, Palette:\n", meta[ 0 ], meta[ 1 ], meta[ 2 ] );
    for( u8 i = 0; i < 16; ++i ) {
        print( pal[ i ], i );
    }
    printf( "\n\n" );

    for( size_t fr = 0; fr < meta[ 0 ]; ++fr ) {
        fread( image, sizeof(u8), meta[ 1 ] * meta[ 2 ] / 2, fin );
        size_t i = 0;
        for( size_t tiley = 0; tiley < meta[ 2 ] / 8; ++tiley ) {
            for( size_t tilex = 0; tilex < meta[ 1 ] / 8; ++tilex ) {
                int shift = tilex * ( -28 );
                for( u8 y = 0; y < 8; ++y ) {
                    for( u8 x = 0; x < 8; x += 2, ++i ) {
                        u16 cur = image[ i ];
                        image_data[ i + shift ] = cur;
                    }
                    shift += 4 * ( meta[ 1 ] / 8 - 1 );
                }
            }
        }

        for( size_t x = 0; x < meta[ 1 ] * meta[ 2 ] / 2; ++x ) {
            print( pal[ image_data[ x ] & 0xF ], image_data[ x ] & 0xF );
            print( pal[ image_data[ x ] >> 4 ], image_data[ x ] >> 4 );
            if( ( x & ( meta[ 1 ] / 2 - 1 ) ) == meta[ 1 ] / 2 - 1 )
                printf( "\n" );
        }
        printf( "\n" );
    }
    fclose( fin );
}
