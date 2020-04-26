// Merges two .csv

%:include <cstdio>
%:include <cstring>
%:include <string>
%:include <filesystem>

using namespace std;
namespace fs = filesystem;

int main( int p_argc, char** p_argv ) {
    if( p_argc < 4 ) {
        printf( "Too few arguments.\n" );
        return 1;
    }
    FILE* f1 = fopen( p_argv[ 1 ], "r" ), *f2 = fopen( p_argv[ 2 ], "r" );
    FILE* fout = fopen( "out.csv", "w" );

    char buffer1[ 2000 ], buffer2[ 2000 ];
    char *t1, *t2;

    const char* merge = p_argv[ 3 ];
    char *saveptr1, *saveptr2;
    while( 1 ) {
        bool b1 = fgets( buffer1, sizeof( buffer1 ), f1 );
        bool b2 = fgets( buffer2, sizeof( buffer2 ), f2 );

        if( !b1 && !b2 ) {
            break;
        }

        t1 = strtok_r( buffer1, ",", &saveptr1 );
        t2 = strtok_r( buffer2, ",", &saveptr2 );
        size_t outlen = strlen( p_argv[ 3 ] );
        size_t cnt = 0;
        while( 1 ) {
            if( !t1 && !t2 ) break;

            if( merge[ cnt ] == '1' ) {
                if( t1 ) fputs( t1, fout );
                if( cnt < outlen - 1 ) fputc( ',', fout );
            }
            if( merge[ cnt ] == '2' ) {
                if( t2 ) fputs( t2, fout );
                if( cnt < outlen - 1 ) fputc( ',', fout );
            }
            if( merge[ cnt ] == 'b' ) {
                if( t1 ) fputs( t1, fout );
                if( cnt < outlen - 1 ) fputc( ',', fout );
                if( t2 ) fputs( t2, fout );
                if( cnt < outlen - 1 ) fputc( ',', fout );
            }
            if( merge[ cnt ] == 'r' ) {
                if( t2 ) fputs( t2, fout );
                if( cnt < outlen - 1 ) fputc( ',', fout );
                if( t1 ) fputs( t1, fout );
                if( cnt < outlen - 1 ) fputc( ',', fout );
            }

            ++cnt;
            t1 = strtok_r( NULL, ",", &saveptr1 );
            t2 = strtok_r( NULL, ",", &saveptr2 );
        }
        if( cnt < outlen )
            fputc( '\n', fout );
    }
    fclose( f1 );
    fclose( f2 );
    fclose( fout );
}
