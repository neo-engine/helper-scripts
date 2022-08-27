// Converts a type icon to a raw file
// Arguments: *.png, THERSHOLD, MAX_ITEMS_PER_DIR
// otherwise outputs to working dir
//
// The sprite may not contain more than 16 colors. The program automatically combines
// similar colors and will do so very aggressively to enforce this color limit.

#include <cstdio>
#include <filesystem>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include <png.h>
#include "bitmap.h"
#define conv( a ) ( ( u8 )( (a) *31 / 248 ) )
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

void print_tiled( FILE* p_f, u8* p_image_data, uint16_t p_width, uint16_t p_height ) {
    // Tile the given sprite into 8x8 blocks
    for( size_t y = 0; y < p_height; y += 8 ) {
        for( size_t x = 0; x < p_width; x += 4 ) {
            for( size_t by = 0; by < 8; ++by ) {
                //                printf( "%u -> %u\n", ( y + by ) * 16 + x, ( y + by ) * 16 + x +
                //                4);
                fwrite( p_image_data + ( y + by ) * ( p_width ) + x, 1, 4, p_f );
            }
        }
    }
}

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
    int start = 0;

    bitmap in( p_argv[ 1 ] );

    uint16_t       NUM_FRAMES = 1, HEIGHT = 16, WIDTH = 32, THRESHOLD = 0, SCALE = 1;
    if( p_argc >= 3 ) { sscanf( p_argv[ 2 ], "%hu", &NUM_FRAMES ); }
    if( p_argc >= 4 ) { sscanf( p_argv[ 3 ], "%hu", &THRESHOLD ); }

    palidx[ 0xffff ] = 3;
    pal[ 3 ] = 0xffff;

    unsigned short bg_color = ( conv( in( 1 * SCALE, 1 * SCALE ).m_red ) )
        | ( conv( in( 1 * SCALE, 1 * SCALE ).m_green ) << 5 )
        | ( conv( in( 1 * SCALE, 1 * SCALE ).m_blue ) << 10 )
        | ( 1 << 15 );
    palidx[ bg_color ] = 1;
    pal[ 1 ]        = bg_color | ( 1 << 15 );

    unsigned short border_color = ( conv( in( 0, 0 ).m_red ) )
        | ( conv( in( 0, 0 ).m_green ) << 5 )
        | ( conv( in( 0, 0 ).m_blue ) << 10 )
        | ( 1 << 15 );
    palidx[ border_color ] = 2;
    pal[ 2 ]        = border_color | ( 1 << 15 );

    u8   col    = 4;
    bool genraw = true;

    if( in.m_width == 2 * WIDTH && in.m_height == 2 * HEIGHT ) {
        SCALE = 2;
        fprintf( stderr, "[%s]: Scaling down too large image.\n", p_argv[ 1 ] );
    }

    size_t numTiles = HEIGHT * WIDTH * NUM_FRAMES, numColors = 16;

    printf( "%u %u\n", HEIGHT, WIDTH );

    bool clr_err = false;
    for( size_t frame = 0; frame < NUM_FRAMES; ++frame )
        for( size_t y = 0; y < HEIGHT; y++ )
            for( size_t x = 0; x < WIDTH; x++ ) {
                size_t nx = x + WIDTH * frame;

                unsigned short conv_color = ( conv( in( nx * SCALE, y * SCALE ).m_red ) )
                                            | ( conv( in( nx * SCALE, y * SCALE ).m_green ) << 5 )
                                            | ( conv( in( nx * SCALE, y * SCALE ).m_blue ) << 10 )
                                            | ( 1 << 15 );
                //                printf( "\x1b[48;2;%u;%u;%um%u %u %u\x1b[0;00m
                //                ->\x1b[48;2;%u;%u;%um %x\x1b[0;00m\n",
                //                        in( nx, y ).m_red, in( nx, y ).m_green, in(nx, y ).m_blue,
                //                        in( nx, y ).m_red, in( nx, y ).m_green, in(nx, y ).m_blue,
                //                        red( conv_color ), blue( conv_color ), green( conv_color
                //                        ), conv_color );
                if( !palidx.count( conv_color ) ) {
                    /*printf( "(%u,%u) %u Adding color \x1b[48;2;%u;%u;%um%3hx\x1b[0;00m\n",
                            x,y,
                               col + start, red( conv_color ), blue( conv_color ), green( conv_color
                                    ), conv_color );

                                    */
                    // Check if the new color is very close to an existing color
                    u8 min_del = 255, del_p = 0;
                    for( u8 p = 2 + start; p < 16; ++p ) {
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
                    } else if( col + start > 16 ) {
                        clr_err = true;
                        fprintf( stderr, "[%s] Too COLORFUL:", p_argv[ 1 ] );
                        fprintf( stderr,
                                 " replacing \x1b[48;2;%u;%u;%um%3hx\x1b[0;00m"
                                 " with \x1b[48;2;%u;%u;%um%3hx\x1b[0;00m\n",
                                 red( conv_color ), blue( conv_color ), green( conv_color ),
                                 conv_color, red( pal[ del_p ] ), blue( pal[ del_p ] ),
                                 green( pal[ del_p ] ), pal[ del_p ] );
                        palidx[ conv_color ] = del_p;
                    } else {
                        fprintf( stderr, " adding color \x1b[48;2;%u;%u;%um%3hx\x1b[0;00m\n",
                                 red( conv_color ), blue( conv_color ), green( conv_color ),
                                 conv_color & 0x7fff );
                        pal[ col + start ]   = conv_color;
                        palidx[ conv_color ] = start + ( col++ );
                    }
                }
                image_data[ WIDTH * HEIGHT * frame + y * WIDTH + x ] = palidx[ conv_color ];
            }
    //    in.writeToFile( (string(p_argv[ 1 ]) + ".test.png").c_str() );

    // Print if there were too many colors
    if( clr_err ) {
        for( size_t x = 0; x < numTiles; ++x ) {
            printf( "\x1b[48;2;%u;%u;%um%2hx\x1b[0;00m", red( pal[ image_data[ x ] ] ),
                    blue( pal[ image_data[ x ] ] ), green( pal[ image_data[ x ] ] ),
                    image_data[ x ] );
            if( ( x % WIDTH ) == size_t( WIDTH - 1 ) ) printf( "\n" );
        }
    }

    // As we are dealing with sprites here, two neighboring pixels share a single byte.
    for( size_t i = 0; i < numTiles / 2; ++i ) {
        image_data[ i ] = ( image_data[ 2 * i + 1 ] << 4 ) | image_data[ 2 * i ];
    }

    string rspth = string( p_argv[ 1 ] );
    rspth.pop_back( );
    rspth.pop_back( );
    rspth.pop_back( );
    rspth.pop_back( );

    FILE*          fout;
    // Output to cwd
    fout = fopen( ( rspth + ( genraw ? ".raw" : ".rsd" ) ).c_str( ), "wb" );

    for( size_t fr = 0; fr < NUM_FRAMES; ++fr )
        print_tiled( fout, image_data + ( fr * WIDTH * HEIGHT / 2 ), WIDTH / 2, HEIGHT );

    fwrite( pal, sizeof( unsigned short int ), numColors, fout );
    fclose( fout );
}
