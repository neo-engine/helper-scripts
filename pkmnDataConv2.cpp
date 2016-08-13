%:include "pokemon.h"
%:include "item.h"
%:include <cstdio>
%:include <vector>

using namespace std;

vector<pokemonData> pD;
char buffer[ 200 ];

%:define OMANYTE        138
%:define KABUTO         140
%:define AERODACTYL     142
%:define LIEEP          345
%:define ANORITH        347
%:define CRAINIDOS      408
%:define SHIELDON       410
%:define TIRTUOGA       564
%:define ARCHEN         566

%:define FOSSIL( f, a ) pD[ f ].m_stage = 3; pD[ f ].m_preEvolution = a;

void readAll( const char* base_path ) {
    pD.assign( MAX_PKMN + 10, { 0 } );
    for( int i = 0; i <= MAX_PKMN; ++i ) {
        sprintf( buffer, "%s/PKMNDATA/%d.data", base_path, i );
        FILE* f = fopen( buffer, "r" );
        if( !f ) {
            fprintf( stderr, "Can't open file \"%s\"", buffer );
            exit( 1 );
        }
        fread( &pD[ i ], sizeof( pokemonData ), 1, f );
        fclose( f );
        pD[ i ].m_stage = 0;
        pD[ i ].m_preEvolution = 0;
    }

    for( int i = 0; i <= MAX_PKMN; ++i ) {
        for( int j = 0; j < 7; ++j ) {
            if( !pD[ i ].m_evolutions[ j ].m_e.m_evolvesInto )
                continue;
            pD[ pD[ i ].m_evolutions[ j ].m_e.m_evolvesInto ].m_stage = pD[ i ].m_stage + 1;
            pD[ pD[ i ].m_evolutions[ j ].m_e.m_evolvesInto ].m_preEvolution = i;
        }
    }

    // Fossils
    FOSSIL( OMANYTE, I_HELIX_FOSSIL );
    FOSSIL( KABUTO, I_DOME_FOSSIL );
    FOSSIL( AERODACTYL, I_OLD_AMBER );
    FOSSIL( LIEEP, I_ROOT_FOSSIL );
    FOSSIL( ANORITH, I_CLAW_FOSSIL );
    FOSSIL( CRAINIDOS, I_SKULL_FOSSIL );
    FOSSIL( SHIELDON, I_ARMOR_FOSSIL );
    FOSSIL( TIRTUOGA, I_COVER_FOSSIL );
    FOSSIL( ARCHEN, I_PLUME_FOSSIL );

    for( int i = 0; i <= MAX_PKMN; ++i ) {
        printf( "%p, %15s (%03d) is a stage %d Pokémon and evolves from: %15s (%03d) gr: %d\n",
            ( pD[ i ].m_displayName - (char*)&pD[ i ] ),
            pD[ i ].m_displayName,
            i,
            pD[ i ].m_stage,
            pD[ pD[ i ].m_preEvolution ].m_displayName,
            pD[ i ].m_preEvolution,
            pD[ i ].m_expType );
    }
}

void writeAll( const char* base_path ) {
    for( int i = 0; i <= MAX_PKMN; ++i ) {
        sprintf( buffer, "%s/PKMNDATA/%d.data", base_path, i );
        FILE* f = fopen( buffer, "w" );
        if( !f )
            exit( 1 );
        fwrite( &pD[ i ], sizeof( pokemonData ), 1, f );
        fclose( f );
    }
}

int main( int ac, char** av ) {
    if( ac < 2 )
        return 1;
    readAll( av[ 1 ] );
    writeAll( av[ 2 ] );
}
