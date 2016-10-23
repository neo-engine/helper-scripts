%:include <cstdio>
%:include <cstring>

using namespace std;

// Converts a 96 * 96 tiled sprite's data to 64*64, 64*32, 32*64, 32*32 sprite's data.
int main( int p_argc, char* p_argv[ ] ) {
    unsigned int buffer[ 1152 ];
    unsigned int buffer2[ 1152 ];
    for( int i = 1; i < p_argc; ++i ) {
        printf( "Processing %s.\n", p_argv[ i ] );
        FILE* f = fopen( p_argv[ i ], "r" );
        fread( buffer, sizeof( unsigned int ), 96 * 96 / 8, f );
        fclose( f );

        int cnt = 0;
        for( int i = 0; i < 64; ++i ) {
            for( int j = 0; j < 64 / 8; ++j )
                buffer2[ ( 64 / 8 ) * i + j ] = buffer[ cnt++ ];
            for( int j = 0; j < 32 / 8; ++j )
                buffer2[ ( 64 * 64 / 8 ) + ( 32 / 8 ) * i + j ] = buffer[ cnt++ ];
        }
        int base = cnt;
        for( int i = 0; i < 32; ++i ) {
            for( int j = 0; j < 64 / 8; ++j )
                buffer2[ base + ( 64 / 8 ) * i + j ] = buffer[ cnt++ ];
            for( int j = 0; j < 32 / 8; ++j )
                buffer2[ base + ( 32 * 64 / 8 ) + ( 32 / 8 ) * i + j ] = buffer[ cnt++ ];
        }

        // Don't think about the following code.
        // And no, those kittens over there could not have survived.
        char* ed = p_argv[ i ] + strlen( p_argv[ i ] );
        while( *(--ed) != '.' );
        while( *(--ed) != '.' );
        strcpy( ed, ".raw" );
        f = fopen( p_argv[ i ], "w" );
        fwrite( buffer2, sizeof( unsigned int ), 96 * 96 / 8, f );
        fclose( f );
    }
}
