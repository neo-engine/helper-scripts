%:include <cstdio>
%:include <string>
%:include <vector>
%:include <cstring>

%:define PKMN_NAME_LEN 15

typedef unsigned short u16;
typedef short s16;
typedef char u8;

std::string readString( FILE* p_file, bool p_new = true ) {
    std::string ret = "";
    unsigned char ac;

    while( ( ac = fgetc( p_file ) ) == '\n' || ac == '\r' );

    if( ac == '*' ) {
        ret += '\0';
        return ret;
    } else ret += ac;

    while( ( ac = fgetc( p_file ) ) != '*' ) {
        if( !p_new ) {
            if( ac == 228 ) {
                ret += '\x84';
                printf( "Converted 228\n" );
            }
            else if( ac == 195 )
                ret += '\x8E';
            else if( ac == 252 )
                ret += '\x81';
            else if( ac == 220 )
                ret += '\x9A';
            else if( ac == 246 )
                ret += '\x94';
            else if( ac == 214 )
                ret += '\x99';
            else if( ac == 223 )
                ret += '\x9D';
            else if( ac == 233 )
                ret += '\x82';
            else if( ac == '%' )
                ret += ' ';
            else if( ac == '|' )
                ret += (char)136;
            else if( ac == '#' )
                ret += (char)137;
            else if( ac == '\r' )
                ret += "";
            else
                ret += ac;
            continue;
        }
        if( ac == '|' )
            ret += (char)136;
        else if( ac == '#' )
            ret += (char)137;
        else
            ret += ac;
    }
    //ret += '\0';
    return ret;
}

struct pokemonData {
    u8              m_types[ 2 ];
    u16             m_bases[ 6 ];
    u16             m_catchrate;
    u16             m_items[ 4 ];
    u8              m_gender;
    u8              m_eggT[ 2 ];
    u16             m_eggcyc;
    u16             m_baseFriend;
    u16             m_EVYield[ 6 ];
    u16             m_EXPYield;
    u16             m_formecnt;
    u16             m_size;
    u16             m_weight;
    u16             m_expType;
    u16             m_abilities[ 4 ];

    union {
        struct evolvestruct {
            s16 m_evolvesInto;
            s16 m_evolutionTrigger;
            s16 m_evolveItem;
            s16 m_evolveLevel;
            s16 m_evolveGender;
            s16 m_evolveLocation;
            s16 m_evolveHeldItem;
            s16 m_evolveDayTime;
            s16 m_evolveKnownMove;
            s16 m_evolveKnownMoveType;
            s16 m_evolveFriendship;
            s16 m_evolveMinimumBeauty;
            s16 m_evolveAtkDefRelation; //1: >; 2: ==; 3 <
            s16 m_evolveAdditionalPartyMember;
            s16 m_evolveTradeSpecies;
        }m_e;
        s16 m_evolveData[ 15 ];
    }               m_evolutions[ 7 ];

    u8              m_displayName[ PKMN_NAME_LEN ];
    u8              m_species[ 50 ];
    u8              m_dexEntry[ 200 ];

    u16             m_formeIdx[ 30 ];
    u8              m_formeName[ 30 ][ PKMN_NAME_LEN ];
};

void convertPkmnData( u16 p_pkmnIdx ) {
    pokemonData data;
    
    char buffer[ 100 ];
    sprintf( buffer, "P-Emerald_2/FSROOT/PKMNDATA/%d.data", p_pkmnIdx );
    FILE* f = fopen( buffer, "r" );

    for( int i = 0; i < 2; ++i )
        data.m_types[ i ] = ( ( fgetc( f ) ) - 42 );
    for( int i = 2; i < 8; ++i )
        data.m_bases[ i - 2 ] = (short)fgetc( f );
    fscanf( f, "%hi", &data.m_catchrate );
    fgetc( f );

    std::string pkmnName = readString( f );
    strncpy( data.m_displayName, pkmnName.c_str(), PKMN_NAME_LEN - 1 );

    for( int i = 0; i < 4; ++i )
        fscanf( f, "%hi", &data.m_items[ i ] );
    fscanf( f, "%hi", &data.m_gender );
    fgetc( f );
    for( int i = 0; i < 2; ++i )
        data.m_eggT[ i ] = ( ( fgetc( f ) ) - 42 );
    data.m_eggcyc = ( ( fgetc( f ) ) - 42 );
    data.m_baseFriend = ( ( fgetc( f ) ) - 42 );
    for( int i = 0; i < 6; ++i )
        data.m_EVYield[ i ] = ( ( fgetc( f ) ) - 42 );
    fscanf( f, "%hi", &data.m_EXPYield );
    fgetc( f );

    std::string dex = readString( f, false );
    strncpy( data.m_dexEntry, dex.c_str(), 249 );
    
    fscanf( f, "%hi", &data.m_formecnt );
    
    for( int i = 0; i < data.m_formecnt; ++i ) {
        fscanf( f, "%hi", &data.m_formeIdx[ i ] ); 

        std::string formeName = readString( f );
        strncpy( data.m_formeName[ i ],
                formeName.c_str(),
                PKMN_NAME_LEN - 1 );
    }
    fscanf( f, " " );
    
    std::string species = readString( f );
    strncpy( data.m_species, species.c_str(), 49 );
    
    fscanf( f, "%hi", &data.m_size );
    fscanf( f, "%hi", &data.m_weight );
    fscanf( f, "%hi", &data.m_expType );
    for( int i = 0; i < 4; ++i )
        fscanf( f, "%hu ", &data.m_abilities[ i ] );
    for( int i = 0; i < 7; ++i )
        for( int j = 0; j < 15; ++j )
            fscanf( f, "%hi ", &( data.m_evolutions[ i ].m_evolveData[ j ] ) );
    fclose( f );

    FILE* w = fopen( buffer, "w" );
    fwrite( &data, sizeof( pokemonData ), 1, w );
    fclose( w );
        
    return;
}

int main( ) {
    printf( "S(pd)=%d\n", sizeof( pokemonData ) );
    for( u16 i = 0; i < 669; ++i ) {
        fprintf( stderr, "%d\n", i );
        convertPkmnData( i );
    }
}
