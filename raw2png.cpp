// Converts a *.raw file to a *.png file.
// Arguments: path #tiles (length of the image) #colors [width] [height]
//
// Tries to guess the dimensions of the original png if they're not given
// (out of 256x192, 64x64, 32x64, 32x32, 16x(#img length / 4))

%:include <cstdio>
%:include <vector>
%:include <string>

%:include <png.h>
%:include "bitmap.h"
using namespace std;

unsigned data[ 12300 ] = { 0 };
unsigned short pal[ 300 ] = { 0 };

int main( int p_argc, char** p_argv ) {
    if( p_argc < 4 ) {
        printf( "Too few arguments.\n" );
        return 1;
    }
    FILE* in = fopen( p_argv[ 1 ], "r" );
    if( !in ) {
        printf( "Input file does not exist.\n" );
        return 2;
    }

    int numTiles, numColors;
    sscanf( p_argv[ 2 ], "%d", &numTiles );
    sscanf( p_argv[ 3 ], "%d", &numColors );
    fread( data, sizeof(unsigned), numTiles, in );
    fread( pal, sizeof(unsigned short int), numColors, in );

    size_t wd, hg;
    if( p_argc >= 5 ) {
        sscanf( p_argv[ 4 ], "%lu", &wd );
    } else {
        wd = 256;
    }
    if( p_argc >= 6 ) {
        sscanf( p_argv[ 5 ], "%lu", &hg );
    } else {
       hg = 4 * numTiles / wd;
    }

    fclose( in );
    if( p_argc < 5 ) {
        if( hg != 192 ) {
            wd = 64; hg = 4 *  numTiles / wd;
            if( hg != 64 ) {
                wd = 32;
                hg = 4 * numTiles / wd;
                if( hg != 64 && hg != 32 ) {
                    wd = 16;
                    hg = 4 * numTiles / wd;
                }
            }
        }
    }


    u8* ptr = reinterpret_cast<u8*>( data );
    bitmap result( wd, hg );
    for( int i = 0; i < 4 * numTiles; ++i ) {
        auto currCol = pal[ ptr[ i ] ];
%:define conv( a ) ((u8)((a) * 255 / 31))
        result( i % wd, i / wd ) = {
            conv( currCol & 31 ),
            conv( ( currCol >> 5 ) & 31 ),
            conv( ( currCol >> 10 ) & 31 )
        };
    }
    result.writeToFile( (string(p_argv[ 1 ]) + ".png").c_str() );
}
