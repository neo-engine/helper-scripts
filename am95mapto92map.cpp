#include <cstdio>
#include <cstring>

#include "fsdata.h"

size_t read( FILE* p_stream, void* p_buffer, size_t p_size, size_t p_count ) {
    if( !p_stream ) return 0;
    return fread( p_buffer, p_size, p_count, p_stream );
}

size_t write( FILE* p_stream, void* p_buffer, size_t p_size, size_t p_count ) {
    if( !p_stream ) return 0;
    return fwrite( p_buffer, p_size, p_count, p_stream );
}

bool readwrite( FILE* p_fin, FILE* p_fout, u32 p_cnt ) {
    u8 tmp;
    for( u32 i = 0; i < p_cnt; ++i ) {
        if( !read( p_fin, &tmp, sizeof( u8 ), 1 ) ) return false;
        if( p_fout && !write( p_fout, &tmp, sizeof( u8 ), 1 ) ) return false;
    }
    return true;
}

int main( int p_argc, char** p_argv ) {
    if( p_argc < 3 ) {
        fprintf( stderr, "Too few arguments.\nUsage %s INPUT OUTPUT\n", p_argv[ 0 ] );
        return 1;
    }

    FILE* fin  = fopen( p_argv[ 1 ], "rb" );
    FILE* fout = fopen( p_argv[ 2 ], "wb" );
    if( !fin || !fout ) {
        fprintf( stderr, "Failed to open file. Aborting\n" );
        return 1;
    }

    // map size
    u8 sx, sy;
    read( fin, &sx, sizeof( u8 ), 1 );
    write( fout, &sx, sizeof( u8 ), 1 );
    readwrite( fin, fout, 3 );
    read( fin, &sy, sizeof( u8 ), 1 );
    write( fout, &sy, sizeof( u8 ), 1 );
    readwrite( fin, fout, 3 );

    // tile set idx
    readwrite( fin, fout, 8 );

    // border size
    u8 b1, b2;
    //    readNop( p_mapFile, 4 );
    read( fin, &b1, sizeof( u8 ), 1 );
    write( fout, &b1, sizeof( u8 ), 1 );
    read( fin, &b2, sizeof( u8 ), 1 );
    write( fout, &b2, sizeof( u8 ), 1 );
    readwrite( fin, fout, 2 );

    // am 1.95 now includes some spam that needs to be removed
    readwrite( fin, NULL, 4 ); // game code?
    readwrite( fin, NULL, 28 ); // ????

    // actual border blocks (hopefully)
    if( b1 && b2 ) { readwrite( fin, fout, 2 * b1 * b2 ); }


    // readwrite actual map data
    readwrite( fin, fout, 2 * sx * sy );

    // done
    fclose( fin );
    fclose( fout );
}
