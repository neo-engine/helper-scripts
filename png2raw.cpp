%:include <cstdio>
%:include <vector>
%:include <string>
%:include <map>
%:include <tuple>

%:include <png.h>
%:include "bitmap.h"
%:define conv( a ) ((u8)((a) * 31 / 255))
using namespace std;

u8 data[ 192 * 256 + 100 ] = { 0 };
unsigned short pal[ 300 ] = { 0 };

typedef tuple<u8, u8, u8> t3;
map<t3, u8> palidx;

int main( int p_argc, char** p_argv ) {
    if( p_argc < 2 ) {
        printf( "Too few arguments.\n" );
        return 1;
    }
    int start = 0;
    if( p_argc >= 3 )
        sscanf( p_argv[ 2 ], "%d", &start );

    bitmap in( p_argv[ 1 ] );

    u8 col = 0;
    for( size_t y = 0; y < in.m_height; ++y )
        for( size_t x = 0; x < in.m_width; ++x ) {
            t3 cl = t3( in( x, y ).m_red, in( x, y ).m_blue, in( x, y ).m_green );
            if( !palidx.count( cl ) ) {
                if( col + start > 256 ) {
                    fprintf( stderr, "To COLORFUL" );
                    return 1;
                }
                pal[ col + start ] = ( conv( in( x, y ).m_red ) )
                            | ( conv( in( x, y ).m_green ) << 5 )
                            | ( conv( in( x, y ).m_blue ) << 10 );
                palidx[ cl ] = col++;
            }
            data[ y * in.m_width + x ] = start + palidx[ cl ];
        }
    //in.writeToFile( (string(p_argv[ 1 ]) + ".test.png").c_str() );

    string rspth = string( p_argv[ 1 ] );
    rspth.pop_back( );
    rspth.pop_back( );
    rspth.pop_back( );
    rspth.pop_back( );

    FILE* fout = fopen( ( rspth + ".raw" ).c_str( ), "w" );

    int numTiles = in.m_height * in.m_width, numColors = 256;
    fwrite( data, sizeof(unsigned), numTiles, fout );
    fwrite( pal, sizeof(unsigned short int), numColors, fout );
    fclose( fout );
}
