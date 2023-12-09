// Converts a png to a raw file
// Arguments: file [palette offset]
//
// palette offset specifies how many entries of the palette should be left blank before
// writing the actual palette DATA. In total, the palette may not have more than 256
// colors.

%:include <cstring>
%:include <cstdio>
%:include <vector>
%:include <string>
%:include <map>
%:include <tuple>

%:include <png.h>
%:include "bitmap.h"
%:define conv( a ) ( (u8) ( ( a ) * 31 / 255 ) )
%:define revconv( a ) ( ( ( a ) * 255 / 31 ) )

%:define green( a ) ( revconv( ( ( a ) >> 10 ) & 31 ) )
%:define blue( a ) ( revconv( ( ( a ) >> 5 ) & 31 ) )
%:define red( a ) ( revconv( ( a ) & 31 ) )
using namespace std;

u8             DATA[ 192 * 256 + 100 ] = { 0 };
unsigned short pal[ 300 ]              = { 0 };

typedef tuple<u8, u8, u8> t3;
map<unsigned short, u8>   palidx;

// Computes distance between colors
int col_dis( int p_1, int p_2 ) {
    return abs( red( p_1 ) - red( p_2 ) ) + abs( green( p_1 ) - green( p_2 ) )
           + abs( blue( p_1 ) - blue( p_2 ) );
}

int main( int p_argc, char** p_argv ) {
    if( p_argc < 2 ) {
        printf( "Too few arguments.\n" );
        return 1;
    }
    int start = 1, mxclr = 235, THRESHOLD = 5;
    if( p_argc >= 3 ) { sscanf( p_argv[ 2 ], "%d", &start ); }
    if( p_argc >= 4 ) { sscanf( p_argv[ 3 ], "%d", &mxclr ); }
    if( p_argc >= 5 ) { sscanf( p_argv[ 4 ], "%d", &THRESHOLD ); }

    bitmap in( p_argv[ 1 ] );

    u8 col = 0, SCALE = 1;
    memset( pal, 0, sizeof( pal ) );
    for( size_t y = 0; y < 192; ++y )
        for( size_t x = 0; x < 256; ++x ) {
            unsigned short conv_color = ( conv( in( x * SCALE, y * SCALE ).m_red ) )
                                        | ( conv( in( x * SCALE, y * SCALE ).m_green ) << 5 )
                                        | ( conv( in( x * SCALE, y * SCALE ).m_blue ) << 10 )
                                        | ( 1 << 15 );

            if( !palidx.count( conv_color ) ) {
                // Check if the new color is very close to an existing color
                u8 min_del = 255, del_p = 0;
                for( u8 p = start; p < mxclr; ++p ) {
                    if( col_dis( conv_color, pal[ p ] ) < min_del ) {
                        min_del = col_dis( conv_color, pal[ p ] );
                        del_p   = p;
                    }
                }

                if( min_del < THRESHOLD && col + start ) {
                    fprintf( stderr,
                             "[%s] replacing \x1b[48;2;%u;%u;%um%3hx\x1b[0;00m"
                             " with \x1b[48;2;%u;%u;%um%3hx\x1b[0;00m (%hu)\n",
                             p_argv[ 1 ], red( conv_color ), blue( conv_color ),
                             green( conv_color ), conv_color, red( pal[ del_p ] ),
                             blue( pal[ del_p ] ), green( pal[ del_p ] ), pal[ del_p ], del_p );
                    palidx[ conv_color ] = del_p;
                } else if( col + start > mxclr ) {
                    fprintf( stderr, "[%s] Too COLORFUL:", p_argv[ 1 ] );
                    fprintf( stderr,
                             " replacing \x1b[48;2;%u;%u;%um%3hx\x1b[0;00m"
                             " with \x1b[48;2;%u;%u;%um%3hx\x1b[0;00m\n",
                             red( conv_color ), blue( conv_color ), green( conv_color ), conv_color,
                             red( pal[ del_p ] ), blue( pal[ del_p ] ), green( pal[ del_p ] ),
                             pal[ del_p ] );
                    palidx[ conv_color ] = del_p;
                } else {
                    fprintf( stderr, "[%s] Color %d is", p_argv[ 1 ], col + start );
                    fprintf( stderr, " \x1b[48;2;%u;%u;%um%3hx\x1b[0;00m\n", red( conv_color ),
                             blue( conv_color ), green( conv_color ), conv_color );

                    pal[ col + start ]   = conv_color;
                    palidx[ conv_color ] = col++;
                }
            }

            DATA[ y * in.m_width + x ] = start + palidx[ conv_color ];
        }
    // in.writeToFile( ( string( p_argv[ 1 ] ) + ".test.png" ).c_str( ) );

    string rspth = string( p_argv[ 1 ] );
    rspth.pop_back( );
    rspth.pop_back( );
    rspth.pop_back( );
    rspth.pop_back( );

    FILE* fout = fopen( ( rspth + ".raw" ).c_str( ), "w" );

    int numTiles = in.m_height * in.m_width, numColors = 256;
    fwrite( DATA, sizeof( u8 ), numTiles, fout );
    fwrite( pal, sizeof( unsigned short int ), numColors, fout );
    fclose( fout );
}
