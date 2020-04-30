// Converts a 64x128 pkmn icon to a rsd file (only the first frame is used)
// Arguments: icon###*.png, where ### is the id of the corresponding pkmn
//
// Outputs the file into folder (###)/30 (padded with zeros to a 2 character name)
// (Downscales the icon to 2 32x32 icons by combining 4 pixels into one using majority)
//
// The sprite may not contain more than 16 colors. The program automatically combines
// similar colors and will do so very aggressively to enforce this color limit.

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

u8 image_data[ 64 * 64 * 20 / 4 + 100 ] = { 0 };
unsigned short pal[ 300 ] = { 0 };

typedef tuple<u8, u8, u8> t3;
map<unsigned short, u8> palidx;

t3 major( t3 p_1, t3 p_2, t3 p_3, t3 p_4 ) {
    if( p_1 == p_2 || p_1 == p_3 || p_1 == p_4 ) {
        return p_1;
    }
    if( p_2 == p_3 || p_2 == p_4 ) {
        return p_2;
    }
    if( p_3 == p_4 ) {
        return p_3;
    }
    return p_4;
}

void print_tiled( FILE* p_f, u8* p_image_data, u8 p_width, u8 p_height ) {
    // Tile the given sprite into 8x8 blocks
    for( size_t y = 0; y < p_height; y += 8 ) {
        for( size_t x = 0; x < p_width; x += 4 ) {
            for( size_t by = 0; by < 8; ++by ) {
//                printf( "%u -> %u\n", ( y + by ) * 16 + x, ( y + by ) * 16 + x + 4);
                fwrite( p_image_data + ( y + by ) * p_width + x, 1, 4, p_f );
            }
        }
    }

}

// Computes distance between colors
int col_dis( int p_1, int p_2 ) {
    return abs( red( p_1 ) - red( p_2 ) )
        + abs( green( p_1 ) - green( p_2 ) )
        + abs( blue( p_1 ) - blue( p_2 ) );
}

int main( int p_argc, char** p_argv ) {
    if( p_argc < 2 ) {
        printf( "Too few arguments.\n" );
        return 1;
    }
    int start = 0;

    bitmap in( p_argv[ 1 ] );

    u8 NUM_FRAMES = 1, HEIGHT = 64, WIDTH = 64, THRESHOLD = 10, MAX_ITEMS_PER_DIR = 30;
    if( p_argc >= 3 ) {
        sscanf( p_argv[ 2 ], "%hhu", &HEIGHT );
    }
    if( p_argc >= 4 ) {
        sscanf( p_argv[ 3 ], "%hhu", &WIDTH );
    }
    if( p_argc >= 5 ) {
        sscanf( p_argv[ 4 ], "%hhu", &NUM_FRAMES );
    }
    if( p_argc >= 6 ) {
        sscanf( p_argv[ 5 ], "%hhu", &THRESHOLD );
    }
    if( p_argc >= 7 ) {
        sscanf( p_argv[ 6 ], "%hhu", &MAX_ITEMS_PER_DIR );
    }

    u8 col = 0;

//    if( in.m_height != HEIGHT || in.m_width != NUM_FRAMES * WIDTH ) {
//        fprintf( stderr, "Wrong DIMENSION" );
//        return 1;
//    }
    for( size_t frame = 0; frame < NUM_FRAMES; ++frame )
        for( size_t y = 0; y < HEIGHT; y += 2 )
            for( size_t x = 0; x < WIDTH; x += 2 ) {
                size_t nx = x + WIDTH * frame;

               t3 cl = major(
                        t3( in( nx, y ).m_red, in( nx, y ).m_blue, in( nx, y ).m_green ),
                        t3( in( nx + 1, y ).m_red, in( nx + 1, y ).m_blue, in( nx + 1, y
                                ).m_green ),
                        t3( in( nx, y + 1 ).m_red, in( nx, y + 1 ).m_blue, in( nx, y + 1
                                ).m_green ),
                        t3( in( nx + 1, y + 1 ).m_red, in( nx + 1, y + 1 ).m_blue, in( nx
                                + 1, y + 1 ).m_green )
                        );
                unsigned short conv_color = ( conv( get<0>( cl ) ) )
                    | ( conv( get<2>( cl ) ) << 5 )
                    | ( conv( get<1>( cl ) ) << 10 )
                    | ( 1 << 15 );
//               unsigned short conv_color = ( conv( in( nx, y ).m_red ) )
//                    | ( conv( in( nx, y ).m_green ) << 5 )
//                    | ( conv( in( nx, y ).m_blue ) << 10 )
//                    | ( 1 << 15 );
//                printf( "\x1b[48;2;%u;%u;%um%u %u %u\x1b[0;00m ->\x1b[48;2;%u;%u;%um %x\x1b[0;00m\n",
//                        in( nx, y ).m_red, in( nx, y ).m_green, in(nx, y ).m_blue,
//                        in( nx, y ).m_red, in( nx, y ).m_green, in(nx, y ).m_blue,
//                        red( conv_color ), blue( conv_color ), green( conv_color ),
//                        conv_color );
                if( !palidx.count( conv_color ) ) {
                    // Check if the new color is very close to an existing color
                    u8 min_del = 255, del_p = 0;
                    for( u8 p = 1 + start; p < 16; ++p ) {
                        if( col_dis( conv_color, pal[ p ] ) < min_del ) {
                            min_del = col_dis( conv_color, pal[ p ] );
                            del_p = p;
                        }
                    }

                    if( min_del < THRESHOLD && col + start ) {
                        fprintf( stderr, "[%s] replacing \x1b[48;2;%u;%u;%um%3hx\x1b[0;00m"
                                " with \x1b[48;2;%u;%u;%um%3hx\x1b[0;00m (%hu)\n",
                                p_argv[ 1 ],
                                red( conv_color ), blue( conv_color ), green( conv_color
                                    ), conv_color,
                                red( pal[ del_p ] ), blue( pal[ del_p ] ), green( pal[
                                    del_p ] ), pal[ del_p ], del_p );
                        palidx[ conv_color ] = del_p;
                    } else if( col + start > 16 ) {
                        fprintf( stderr, "[%s] To COLORFUL:", p_argv[ 1 ] );
                        fprintf( stderr, " replacing \x1b[48;2;%u;%u;%um%3hx\x1b[0;00m"
                                " with \x1b[48;2;%u;%u;%um%3hx\x1b[0;00m\n",
                                red( conv_color ), blue( conv_color ), green( conv_color
                                    ), conv_color,
                                red( pal[ del_p ] ), blue( pal[ del_p ] ), green( pal[
                                    del_p ] ), pal[ del_p ] );
                        palidx[ conv_color ] = del_p;
                    } else {
//                        fprintf( stderr, " adding color \x1b[48;2;%u;%u;%um%3hx\x1b[0;00m\n",
//                                red( conv_color ), blue( conv_color ), green( conv_color
//                                    ), conv_color );
                        pal[ col + start ] = conv_color;
                        palidx[ conv_color ] = col++;
                    }
                }
                image_data[ WIDTH * HEIGHT * frame / 4 + y * WIDTH / 4 + x / 2 ] = start +
                    palidx[ conv_color ];
            }
    //    in.writeToFile( (string(p_argv[ 1 ]) + ".test.png").c_str() );

    size_t numTiles = HEIGHT * WIDTH * NUM_FRAMES / 4, numColors = 16;

    // As we are dealing with sprites here, two neighboring pixels share a single byte.
    for( size_t i = 0; i < numTiles / 2; ++i ) {
        image_data[ i ] = ( image_data[ 2 * i + 1 ] << 4 ) | image_data[ 2 * i ];
    }

    /*
    size_t cnt = 0;
    for( size_t y = 0; y < HEIGHT / 4; y += 2 )
        for( size_t x = 0; x < WIDTH / 2; ++x, ++cnt ) {
            image_data[ cnt ] = image_data[ y * HEIGHT / 4 + x ]
                | ( image_data[ ( y + 1 ) * HEIGHT / 4 + x ] << 4 );
        }
    */

    string rspth = string( p_argv[ 1 ] );
    rspth.pop_back( );
    rspth.pop_back( );
    rspth.pop_back( );
    rspth.pop_back( );

    unsigned short pkmnidx;
    if( !sscanf( p_argv[ 1 ], "icon%03hu", &pkmnidx ) ) {
        fprintf( stderr, "argument must start with \"icon\"" );
        return 1;
    }

    char buffer[ 50 ];
    snprintf( buffer, 40, "%02hhu/%hu/", pkmnidx / MAX_ITEMS_PER_DIR, pkmnidx );

    fs::create_directories( buffer );

    FILE* fout = fopen( ( buffer + rspth + ".rsd" ).c_str( ), "wb" );

    u8 meta[3] = { NUM_FRAMES, 32, 32 };

    fwrite( pal, sizeof(unsigned short int), numColors, fout );
    fwrite( meta, sizeof(u8), 3, fout );

    for( size_t fr = 0; fr < NUM_FRAMES; ++fr )
        print_tiled( fout, image_data + fr * 16 * 32, WIDTH / 4, HEIGHT / 2 );

    fwrite( pal, sizeof(unsigned short int), numColors, fout );
    fclose( fout );

    /*
    // DEBUG: Print icon on terminal
    for( size_t x = 0; x < numTiles; ++x ) {
        printf( "\x1b[48;2;%u;%u;%um%3hx\x1b[0;00m", red( pal[ image_data[ x ] ] ),
                blue( pal[ image_data[ x ] ] ), green( pal[ image_data[ x ] ] ), image_data[ x ] );
        if( ( x & 15 ) == 15 )
            printf( "\n" );

        if( x == numTiles / 2 - 1 )
            printf( "\n" );
    }
    */
}
