// Converts a 40x30 pokemon icon to a 32x32 icon
// Arguments pokemonnames.csv icon.png
//
// Outputs the file where it came from
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
%:include "fsdata.h"
%:define conv( a ) ((u8)((a) * 31 / 255))
%:define revconv( a ) (((a) * 255 / 31))

%:define green( a ) (revconv(((a) >> 10) & 31 ))
%:define blue( a ) (revconv(((a) >> 5 ) & 31 ))
%:define red( a ) (revconv( (a) & 31 ))

u8 image_data[ 64 * 64 * 20 / 4 + 100 ] = { 0 };
unsigned short pal[ 300 ] = { 0 };

typedef tuple<u8, u8, u8> t3;
map<unsigned short, u8> palidx;

map<string, int>    pkmns;
vector<names>       pkmn_names;

std::string normalizeName( const char* p_string ) {
    std::string res = "";
    while( *p_string ) {
        if( *p_string == '\xe9' || *p_string == '\xc9' ) {
            res += 'E';
            ++p_string; continue;
        }
        if( *p_string == '{' ) {
            res += "_M";
            ++p_string; continue;
        }
        if( *p_string == '}' ) {
            res += "_F";
            ++p_string; continue;
        }
        if( *p_string == '.' ) {
            ++p_string; continue;
        }
        if( *p_string == '\'' || *p_string == ' ' || *p_string == ':' || *p_string == '-'
                || *p_string == '?') {
            res += "_";
            ++p_string; continue;
        }
        res += toupper( *( p_string++ ) );
    }
    return res;
}

int main( int p_argc, char** p_argv ) {
    if( p_argc < 3 ) {
        printf( "Too few arguments.\n" );
        return 1;
    }
    readNames( p_argv[ 1 ], pkmn_names );
    for( size_t i = 0; i < pkmn_names.size( ); ++i ) {
        pkmns[ normalizeName( pkmn_names[ i ].m_name[ 0 ] ) ] = i;
    }

    bitmap in( p_argv[ 2 ] );

    char bf1[ 50 ], bf2[ 20 ];
    sscanf( p_argv[ 2 ], "%[^.].%s", bf1, bf2 );
    string rspth = string( bf1 );

    if( !pkmns.count( normalizeName( rspth.c_str( ) ) ) ) {
        fprintf( stderr, "[%s] Unknown Pokémon \"%s\".\n", p_argv[ 2 ], rspth.c_str( ) );
        exit( 1 );
    }

    u16 outi = pkmns[ normalizeName( rspth.c_str( ) ) ];
    fprintf( stderr, "[%s] \"%s\" -> %hu.\n", p_argv[ 2 ], rspth.c_str( ), outi );

    u8 HEIGHT = 30, WIDTH = 40, TARGET_WIDTH = 32, TARGET_HEIGHT = 32;
    if( p_argc >= 4 ) {
        sscanf( p_argv[ 3 ], "%hhu", &HEIGHT );
    }
    if( p_argc >= 5 ) {
        sscanf( p_argv[ 4 ], "%hhu", &WIDTH );
    }
    if( p_argc >= 6 ) {
        sscanf( p_argv[ 5 ], "%hhu", &TARGET_HEIGHT );
    }
    if( p_argc >= 7 ) {
        sscanf( p_argv[ 6 ], "%hhu", &TARGET_WIDTH );
    }
    bitmap out( TARGET_WIDTH, TARGET_HEIGHT );

    s32 shift_x = ( s32( TARGET_WIDTH ) - WIDTH ) / 2;
    s32 shift_y = ( s32( TARGET_HEIGHT ) - HEIGHT ) / 2;

    for( s32 y = 0; y < HEIGHT; y++ ) {
        if( y + shift_y < 0 || y + shift_y >= TARGET_HEIGHT ) { continue; }
        for( s32 x = 0; x < WIDTH; x++ ) {
            if( x + shift_x < 0 || x + shift_x >= TARGET_WIDTH ) { continue; }
            s32 nx = x + shift_x;
            s32 ny = y + shift_y;

//            fprintf( stderr, "(%hhi, %hhi) -> (%hhi, %hhi)\n", x, y, nx, ny );

            out( nx, ny ) = in( x, y );
        }
    }

    char BUFFER[ 50 ];
    if( p_argc <= 7 ) {
        snprintf( BUFFER, 49, "icon%03hu.%s", outi, bf2 );
    } else {
        snprintf( BUFFER, 49, "icon%03hu%s.%s", outi, p_argv[ 7 ], bf2 );
    }

    out.writeToFile( BUFFER );


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
