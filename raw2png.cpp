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
    fclose( in );

    size_t wd = 256, hg = 4 * numTiles / wd;
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
