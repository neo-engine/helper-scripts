%:include <cstdio>
%:include <vector>

typedef unsigned short u16;

u16 mxlen = 0;

void updateLearnSet( u16 p_pkmnIdx ) {
    std::vector<u16> res;
    char pt[ 150 ];
    sprintf( pt, "FSROOT/PKMNDATA/LEARNSETS/%d.learnset.data", 
            p_pkmnIdx );
    FILE* f = fopen( pt, "r" );
    
    if( !f )
        return;

    for( int i = 0; i < 101; ++i ) {
        u16 z; fscanf( f, "%hu", &z );
        res.push_back( z );
        for( u16 j = 0; j < z; ++j ) {
            u16 g, h;
            fscanf( f, "%hu %hu", &g, &h );
            res.push_back( g );
            res.push_back( h );
        }
    }
    fclose( f );
    f = fopen( pt, "w" );
    fwrite( &res[0], sizeof( u16 ), res.size(), f );
    mxlen = std::max( mxlen,(u16) res.size() );
    fclose( f );
}

int main() {
    for( int i = 0; i < 669; ++i ) {
        updateLearnSet( i );          
    }
    printf( "%d\n", mxlen );
}
