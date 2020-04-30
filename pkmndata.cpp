// Arguments
// pkmnnames pkmndata pkmndescr pkmnformnames pkmnformdescr abtynames movenames itemnames

%:include <cstdio>
%:include <cstring>
%:include <string>
%:include <map>
%:include <vector>
%:include <filesystem>
%:include <cassert>

%:define NUM_LANGUAGES 2
%:define MAX_ITEMS_PER_DIR 30
%:define MAX_PKMN 890

using namespace std;
namespace fs = filesystem;

int getType( char* p_str ) {
    if( !strcmp( p_str, "Normal" ) ) return 0;
    if( !strcmp( p_str, "Fight" ) ) return 1;
    if( !strcmp( p_str, "Fighting" ) ) return 1;
    if( !strcmp( p_str, "Flying" ) ) return 2;
    if( !strcmp( p_str, "Poison" ) ) return 3;
    if( !strcmp( p_str, "Ground" ) ) return 4;
    if( !strcmp( p_str, "Rock" ) ) return 5;
    if( !strcmp( p_str, "Bug" ) ) return 6;
    if( !strcmp( p_str, "Ghost" ) ) return 7;
    if( !strcmp( p_str, "Steel" ) ) return 8;
    if( !strcmp( p_str, "???" ) ) return 9;
    if( !strcmp( p_str, "Water" ) ) return 10;
    if( !strcmp( p_str, "Fire" ) ) return 11;
    if( !strcmp( p_str, "Grass" ) ) return 12;
    if( !strcmp( p_str, "Electric" ) ) return 13;
    if( !strcmp( p_str, "Electr" ) ) return 13;
    if( !strcmp( p_str, "Psychic" ) ) return 14;
    if( !strcmp( p_str, "Ice" ) ) return 15;
    if( !strcmp( p_str, "Dragon" ) ) return 16;
    if( !strcmp( p_str, "Dark" ) ) return 17;
    if( !strcmp( p_str, "Fairy" ) ) return 18;

    fprintf( stderr, "Found bad type %s.\n", p_str );
    return 9;
}

int getLevelUpType( char* p_str ) {
    if( !strcmp( p_str, "Medium Fast" ) ) return 0;
    if( !strcmp( p_str, "Erratic" ) ) return 1;
    if( !strcmp( p_str, "Fluctuating" ) ) return 2;
    if( !strcmp( p_str, "Medium Slow" ) ) return 3;
    if( !strcmp( p_str, "Fast" ) ) return 4;
    if( !strcmp( p_str, "Slow" ) ) return 5;

    fprintf( stderr, "Found bad level up type %s.\n", p_str );
    return 0;
}

int getEggGroup( char* p_str ) {
    if( !strcmp( p_str, "none" ) ) return 0;
    if( !strcmp( p_str, "Monster" ) ) return 1;
    if( !strcmp( p_str, "Water 1" ) ) return 2;

    if( !strcmp( p_str, "Bug" ) ) return 4;
    if( !strcmp( p_str, "Flying" ) ) return 5;
    if( !strcmp( p_str, "Field" ) ) return 6;
    if( !strcmp( p_str, "Fairy" ) ) return 7;
    if( !strcmp( p_str, "Grass" ) ) return 8;
    if( !strcmp( p_str, "Human-Like" ) ) return 9;
    if( !strcmp( p_str, "Water 3" ) ) return 10;
    if( !strcmp( p_str, "Mineral" ) ) return 11;
    if( !strcmp( p_str, "Amorphous" ) ) return 12;
    if( !strcmp( p_str, "Water 2" ) ) return 13;
    if( !strcmp( p_str, "Ditto" ) ) return 14;
    if( !strcmp( p_str, "Dragon" ) ) return 15;
    if( !strcmp( p_str, "Undiscovered" ) ) return 16;

    fprintf( stderr, "Found bad egg group %s.\n", p_str );
    return 0;
}

unsigned char getExpType( char* p_str ) {
    if( !strcmp( p_str, "Medium Fast" ) ) return 0;
    if( !strcmp( p_str, "Erratic" ) ) return 1;
    if( !strcmp( p_str, "Fluctuating" ) ) return 2;
    if( !strcmp( p_str, "Medium Slow" ) ) return 3;
    if( !strcmp( p_str, "Fast" ) ) return 4;
    if( !strcmp( p_str, "Slow" ) ) return 5;


    fprintf( stderr, "Found bad exp type %s.\n", p_str );
    return 0;
}

unsigned char getGender( char* p_str ) {
    if( !strcmp( p_str, "genderless" ) ) return 255;
    if( !strcmp( p_str, "always female" ) ) return 254;
    if( !strcmp( p_str, "1m7f" ) ) return 223;
    if( !strcmp( p_str, "1m3f" ) ) return 191;
    if( !strcmp( p_str, "1m1f" ) ) return 127;
    if( !strcmp( p_str, "3m1f" ) ) return 63;
    if( !strcmp( p_str, "7m1f" ) ) return 31;
    if( !strcmp( p_str, "always male" ) ) return 0;

    fprintf( stderr, "Found bad gender value %s.\n", p_str );
    return 255;
}

unsigned char getColor( char* p_str ) {
    if( !strcmp( p_str, "Red" ) ) return 0;
    if( !strcmp( p_str, "Blue" ) ) return 1;
    if( !strcmp( p_str, "Yellow" ) ) return 2;
    if( !strcmp( p_str, "Green" ) ) return 3;
    if( !strcmp( p_str, "Black" ) ) return 4;
    if( !strcmp( p_str, "Brown" ) ) return 5;
    if( !strcmp( p_str, "Purple" ) ) return 6;
    if( !strcmp( p_str, "Gray" ) ) return 7;
    if( !strcmp( p_str, "White" ) ) return 8;
    if( !strcmp( p_str, "Pink" ) ) return 9;

    fprintf( stderr, "Found bad color %s.\n", p_str );
    return 0;
}

unsigned char getShape( char* p_str ) {
    if( !strcmp( p_str, "Pomaceous" ) ) return 1;
    if( !strcmp( p_str, "Caudal" ) ) return 2;
    if( !strcmp( p_str, "Ichthyic" ) ) return 3;
    if( !strcmp( p_str, "Brachial" ) ) return 4;
    if( !strcmp( p_str, "Alvine" ) ) return 5;
    if( !strcmp( p_str, "Sciurine" ) ) return 6;
    if( !strcmp( p_str, "Crural" ) ) return 7;
    if( !strcmp( p_str, "Mensal" ) ) return 8;
    if( !strcmp( p_str, "Alar" ) ) return 9;
    if( !strcmp( p_str, "Cilial" ) ) return 10;
    if( !strcmp( p_str, "Polycephalic" ) ) return 11;
    if( !strcmp( p_str, "Anthropomorphic" ) ) return 12;
    if( !strcmp( p_str, "Lepidopterous" ) ) return 13;
    if( !strcmp( p_str, "Chitinous" ) ) return 14;

    fprintf( stderr, "Found bad shape %s.\n", p_str );
    return 0;
}




struct pkmnFormeData {
    unsigned char   m_types[ 2 ];
    unsigned short  m_abilities[ 4 ]; // abilities
    unsigned char   m_bases[ 6 ]; // base values (hp, atk, def, satk, sdef, spd)
    unsigned short  m_expYield;
    unsigned char   m_genderRatio; // from pkmnGenderType
    unsigned char   m_size; // in dm
    unsigned short  m_weight; // in 100g
    unsigned char   m_colorShape; // (color << 4) | shape
    unsigned char   m_evYield[ 6 ]; // (hp, atk, def, satk, sdef, spd)
    unsigned short  m_items[ 4 ]; // possible held items: 1%, 5%, 50%, 100%
    unsigned char   m_eggGroups; // (eg1 << 4) | eg2;
};

struct pkmnData {
    pkmnFormeData   m_baseForme;
    unsigned char   m_expTypeFormeCnt; // (ExpType << 5) | FormeCnt
    unsigned char   m_eggCycles;
    unsigned char   m_catchrate;
    unsigned char   m_baseFriend;
};

struct names {
    char* m_name[ NUM_LANGUAGES ];
};
struct data {
    char m_data[ 200 ][ NUM_LANGUAGES ];
};

map<string, int> abilities;
vector<names> ability_names;

map<string, int> items;
vector<names> item_names;

map<string, int> moves;
vector<names> move_names;

map<string, int> pkmns;
vector<names> pkmn_names;
vector<vector<names>> forme_names;

vector<pkmnData> pkmn_data;
vector<vector<pkmnFormeData>> forme_data;

FILE* getFilePtr( string p_prefix, unsigned short p_index, unsigned char p_digits,
                  string p_ext = ".data", unsigned char p_formeId = 0 ) {
    char buffer[ 50 ];
    char buffer2[ 100 ];
    snprintf( buffer2, 20, "%%0%hhuhhu/", p_digits );
    snprintf( buffer, 40, buffer2, p_index / MAX_ITEMS_PER_DIR );

    fs::create_directories( p_prefix + buffer );
    if( !p_formeId )
        snprintf( buffer2, 80, "%s/%s%hu%s", p_prefix.c_str( ),
                  buffer, p_index, p_ext.c_str( ) );
    else
        snprintf( buffer2, 80, "%s/%s%hu_%hhu%s", p_prefix.c_str( ),
                  buffer, p_index, p_formeId, p_ext.c_str( ) );

    return fopen( buffer2, "wb" );
}

void printPkmnData( ) {
    fs::create_directories( "out" );
    FILE* g = fopen( "out/pkmnNames.h", "w" );
    fprintf( g, "#pragma once\n" );
    for( size_t i = 0; i < pkmn_data.size( ); ++i ) {
        FILE* f = getFilePtr( "out/pkmn_data/", i, 2 );
        assert( f );
        FILE* n = getFilePtr( "out/pkmn/", i, 2, ".str" );
        assert( n );
        assert( fwrite( &pkmn_data[ i ], sizeof( pkmnData ), 1, f ) );
        for( int j = 0; j < NUM_LANGUAGES; ++j ) {
            assert( fwrite( pkmn_names[ i ].m_name[ j ], 1, 15, n ) );
        }
        fclose( f );
        fclose( n );
        for( unsigned char forme = 1; forme <= ( pkmn_data[ i ].m_expTypeFormeCnt & 31 );
                ++forme ) {
            //fprintf( stderr, "write %lu %hhu / %hhu\n", i, forme,
            //         pkmn_data[ i ].m_expTypeFormeCnt & 31);
            f = getFilePtr( "out/pkmn_data/", i, 2, ".data", forme );
            n = getFilePtr( "out/pkmn/", i, 2, ".str", forme );
            if( forme <= forme_data[ i ].size( ) ) {
                fwrite( &forme_data[ i ][ forme - 1 ], sizeof( pkmnFormeData ), 1, f );
                for( int j = 0; j < NUM_LANGUAGES; ++j ) {
                    assert( fwrite( forme_names[ i ][ forme - 1 ].m_name[ j ], 1, 30, n ) );
                }
            } else {
                fwrite( &pkmn_data[ i ], sizeof( pkmnData ), 1, f );
            }
            fclose( f );
            fclose( n );
        }
        if( i ) {
            fprintf( g, "#define PKMN_" );
            char* s = pkmn_names[ i ].m_name[ 0 ];
            while( *s ) {
                if( *s == '\'' || *s == ' ' || *s == '.' || *s == ':' || *s == '-' || *s == '?') {
                    fputc( '_', g );
                    ++s; continue;
                }
                fputc( toupper( *( s++ ) ), g );
            }
            fprintf( g, " %lu\n", i );
        }
    }
    fclose( g );
}

void printItemData( ) {
    FILE* g = fopen( "out/itemNames.h", "w" );
    fprintf( g, "#pragma once\n" );

    for( size_t i = 0; i < item_names.size( ); ++i ) {
        if( !strcmp( item_names[ i ].m_name[ 0 ], "???" ) ) continue;

        // FILE* f = getFilePtr( "out/item/", i, 3 );
        // assert( f );
        FILE* n = getFilePtr( "out/item/", i, 2, ".str" );
        assert( n );
        // assert( fwrite( &item_data[ i ], sizeof( itemData ), 1, f ) );
        for( int j = 0; j < NUM_LANGUAGES; ++j ) {
            assert( fwrite( item_names[ i ].m_name[ j ], 1, 20, n ) );
        }
        // fclose( f );
        fclose( n );

        if( i ) {
            fprintf( g, "#define I_" );
            char* s = item_names[ i ].m_name[ 0 ];
            while( *s ) {
                if( *s == '\'' || *s == ' ' || *s == '.' || *s == ':' || *s == '-' || *s == '?') {
                    fputc( '_', g );
                    ++s; continue;
                }
                fputc( toupper( *( s++ ) ), g );
            }
            fprintf( g, " %lu\n", i );
        }
    }
    fclose( g );
}

void printAbilityData( ) {
    FILE* g = fopen( "out/abilityNames.h", "w" );
    fprintf( g, "#pragma once\n" );

    for( size_t i = 0; i < ability_names.size( ); ++i ) {
        // FILE* f = getFilePtr( "out/abty/", i, 2 );
        // assert( f );
        FILE* n = getFilePtr( "out/abty/", i, 2, ".str" );
        assert( n );
        // assert( fwrite( &item_data[ i ], sizeof( itemData ), 1, f ) );
        for( int j = 0; j < NUM_LANGUAGES; ++j ) {
            assert( fwrite( ability_names[ i ].m_name[ j ], 1, 20, n ) );
        }
        // fclose( f );
        fclose( n );

        fprintf( g, "#define A_" );
        char* s = ability_names[ i ].m_name[ 0 ];
        while( *s ) {
            if( *s == '\'' || *s == ' ' || *s == '.' || *s == ':' || *s == '-' || *s == '?' ) {
                fputc( '_', g );
                ++s; continue;
            }
            fputc( toupper( *( s++ ) ), g );
        }
        fprintf( g, " %lu\n", i );
    }
    fclose( g );
}

void printMoveData( ) {
    FILE* g = fopen( "out/moveNames.h", "w" );
    fprintf( g, "#pragma once\n" );

    for( size_t i = 0; i < move_names.size( ); ++i ) {
        // FILE* f = getFilePtr( "out/move/", i, 2 );
        // assert( f );
        FILE* n = getFilePtr( "out/move/", i, 2, ".str" );
        assert( n );
        // assert( fwrite( &item_data[ i ], sizeof( itemData ), 1, f ) );
        for( int j = 0; j < NUM_LANGUAGES; ++j ) {
            assert( fwrite( move_names[ i ].m_name[ j ], 1, 20, n ) );
        }
        // fclose( f );
        fclose( n );

        if( i ) {
            fprintf( g, "#define M_" );
            char* s = move_names[ i ].m_name[ 0 ];
            while( *s ) {
                if( *s == '\'' || *s == ' ' || *s == '.' || *s == ':' || *s == '-' || *s == '?') {
                    fputc( '_', g );
                    ++s; continue;
                }
                fputc( toupper( *( s++ ) ), g );
            }
            fprintf( g, " %lu\n", i );
        }
    }
    fclose( g );
}

void readNames( char* p_path, vector<names>& p_out ) {
    FILE* f = fopen( p_path, "r" );
    char buffer[ 500 ];
    char* t1;
    while( fgets( buffer, sizeof( buffer ), f ) ) {
        t1 = strtok( buffer, "," );
        names n;
        for( int i = 0; i < NUM_LANGUAGES; ++i ) {
            n.m_name[ i ] = new char[ 30 ];
        }
        int cnt = 0;
        while( cnt < NUM_LANGUAGES && ( t1 = strtok( NULL, "," ) ) ) {
            strncpy( n.m_name[ cnt++ ], t1, 29 );
        }
        p_out.push_back( n );
    }
    fprintf( stderr, "read %lu objects from %s\n", p_out.size( ), p_path );
}

void readItems( char* p_path, vector<names>& p_out ) {
    FILE* f = fopen( p_path, "r" );
    char buffer[ 500 ];
    char* t1;
    while( fgets( buffer, sizeof( buffer ), f ) ) {
        t1 = strtok( buffer, "," );
        names n;
        for( int i = 0; i < NUM_LANGUAGES; ++i ) {
            n.m_name[ i ] = new char[ 30 ];
        }
        t1 = strtok( NULL, "," ); // TODO: properly parse bag type
        int cnt = 0;
        while( cnt < NUM_LANGUAGES && ( t1 = strtok( NULL, "," ) ) ) {
            strncpy( n.m_name[ cnt++ ], t1, 29 );
        }
        p_out.push_back( n );
    }
    fprintf( stderr, "read %lu objects from %s\n", p_out.size( ), p_path );
}


void readForme( char* p_buf, pkmnFormeData& p_out ) {
    char tmp_buf[ 50 ], tmp_buf2[ 50 ], *tmp_buf3[ 3 ], tmp_buf5[ 50 ], tmp_buf6[ 50 ];
    char tmp_buf7[ 50 ], tmp_buf8[ 50 ], tmp_buf9[ 50 ], tmp_buf10[ 50 ], *tmp_buf11[ 50 ];
    for( int j = 0; j < 3; ++j ) tmp_buf3[ j ] = new char[ 50 ];
    for( int j = 0; j < 4; ++j ) tmp_buf11[ j ] = new char[ 50 ];

    float sz, wt;
    assert( sscanf( p_buf, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%hhu,%hhu,"
                "%hhu,%hhu,%hhu,%hhu,%hu,%[^,],%f m,%f kg,%[^,],%[^,],%hhu,%hhu,%hhu,%hhu,%hhu,"
                "%hhu,%[^,],%[^,],%[^,],%[^,],",
                tmp_buf5, tmp_buf, tmp_buf2, tmp_buf3[ 0 ], tmp_buf3[ 1 ], tmp_buf3[ 2 ],
                tmp_buf6, tmp_buf7, &p_out.m_bases[ 0 ], &p_out.m_bases[ 1 ],
                &p_out.m_bases[ 2 ], &p_out.m_bases[ 3 ], &p_out.m_bases[ 4 ], &p_out.m_bases[ 5 ],
                &p_out.m_expYield, tmp_buf8, &sz, &wt, tmp_buf9, tmp_buf10,
                &p_out.m_evYield[ 0 ], &p_out.m_evYield[ 1 ], &p_out.m_evYield[ 2 ],
                &p_out.m_evYield[ 3 ], &p_out.m_evYield[ 4 ], &p_out.m_evYield[ 5 ],
                tmp_buf11[ 0 ], tmp_buf11[ 1 ], tmp_buf11[ 2 ], tmp_buf11[ 3 ] ) );

    p_out.m_types[ 0 ] = getType( tmp_buf );
    p_out.m_types[ 1 ] = getType( tmp_buf2 );
    // printf( "%lu %s %s got %hhu\n", i, tmp_buf, tmp_buf2, cur.m_baseForme.m_types );
    for( size_t j = 0; j < 3; ++j ) {
        if( abilities.count( tmp_buf3[ j ] ) )
            p_out.m_abilities[ j ] = abilities[ tmp_buf3[ j ] ];
        else if( strcmp( tmp_buf3[ j ], "none" ) ) {
            fprintf( stderr, "[%s] ability %lu \"%s\" unknown\n", tmp_buf5, j, tmp_buf3[ j ] );
        }
        delete[] tmp_buf3[ j ];
    }
    p_out.m_eggGroups = ( getEggGroup( tmp_buf6 ) << 4 ) | getEggGroup( tmp_buf7 );
    p_out.m_genderRatio = getGender( tmp_buf8 );
    p_out.m_size = (unsigned char) ( sz * 10 );
    p_out.m_weight = (unsigned short) ( wt * 10 );
    p_out.m_colorShape = ( getColor( tmp_buf9 ) << 4 ) | getShape( tmp_buf10 );

    for( size_t j = 0; j < 3; ++j ) {
        if( items.count( tmp_buf11[ j ] ) )
            p_out.m_items[ j ] = items[ tmp_buf11[ j ] ];
        else if( strcmp( tmp_buf11[ j ], "none" ) )
            fprintf( stderr, "[%s] item %lu \"%s\" unknown\n", tmp_buf5, j, tmp_buf11[ j ] );
        delete[] tmp_buf11[ j ];
    }
}

void readPkmnData( char* p_pkmnData, char* p_pkmnDescr, char* p_pkmnFormeNames,
                   char* p_pkmnFormeDescr, vector<pkmnData>& p_out,
                   vector<vector<names>>& p_formeNameOut,
                   vector<vector<pkmnFormeData>>& p_formeOut ) {
    FILE* pdata = fopen( p_pkmnData, "r" );
    FILE* pdesc = fopen( p_pkmnDescr, "r" );

    FILE* fdata = fopen( p_pkmnFormeNames, "r" );
    FILE* fdesc = fopen( p_pkmnFormeDescr, "r" );
    char fdata_buf[ 1000 ], fdescr_buf[ 1000 ];
    assert( fgets( fdata_buf, sizeof( fdata_buf ), fdata ) );
    assert( fgets( fdescr_buf, sizeof( fdescr_buf ), fdesc ) );

    for( size_t i = 0; i <= MAX_PKMN; ++i ) {
        // fprintf( stderr, "reading data %lu\n", i );
        char data_buf[ 1000 ], descr_buf[ 1000 ];
        assert( fgets( data_buf, sizeof( data_buf ), pdata ) );
        assert( fgets( descr_buf, sizeof( descr_buf ), pdesc ) );

        pkmnData cur = { 0 };
        cur.m_baseForme = { 0 };
        int id_a, id_b;
        char tmp_buf[ 50 ];
        unsigned char tmp1;
        assert( sscanf( data_buf, "%d,%[^,],%hhu,%hhu,%hhu,%hhu",
                &id_a, tmp_buf, &cur.m_eggCycles, &cur.m_catchrate, &cur.m_baseFriend, &tmp1 ) );

        assert( sscanf( descr_buf, "%d", &id_b ) );
        assert( id_a == id_b );
        readForme( descr_buf, cur.m_baseForme );

        // Check for existing forms
        unsigned char forms_found = 0;
        int cur_formbase, cur_formid;
        sscanf( fdescr_buf, "%d_%d", &cur_formbase, &cur_formid );
        vector<names> fnames;
        vector<pkmnFormeData> fdatas;
        while( cur_formbase == id_a ) {
            names n = pkmn_names[ id_a ], n2;
            pkmnFormeData pf = cur.m_baseForme;
            while( cur_formid > forms_found + 1 ) {
                fnames.push_back( n );
                fdatas.push_back( pf );
                forms_found++;
            }
            for( int i = 0; i < NUM_LANGUAGES; ++i ) {
                n2.m_name[ i ] = new char[ 30 ];
            }

            char* t1 = strtok( fdata_buf, "," );
            int cnt = 0;
            while( cnt < NUM_LANGUAGES && ( t1 = strtok( NULL, "," ) ) ) {
                strncpy( n2.m_name[ cnt++ ], t1, 29 );
            }

            readForme( fdescr_buf, pf );
            fnames.push_back( n2 );
            fdatas.push_back( pf );
            if( !fgets( fdata_buf, sizeof( fdata_buf ), fdata ) ) break;
            assert( fgets( fdescr_buf, sizeof( fdescr_buf ), fdesc ) );
            sscanf( fdescr_buf, "%d_%d", &cur_formbase, &cur_formid );
            ++forms_found;
        }
        while( forms_found < tmp1 ) {
            fnames.push_back( pkmn_names[ id_a ] );
            fdatas.push_back( cur.m_baseForme );
            forms_found++;
        }
        p_formeNameOut.push_back( fnames );
        p_formeOut.push_back( fdatas );


        cur.m_expTypeFormeCnt = ( getExpType( tmp_buf ) << 5 )
            |  max( tmp1, forms_found );
        p_out.push_back( cur );
    }
    fprintf( stderr, "read %lu pkmndata\n", p_out.size( ) );
}

int main( int p_argc, char** p_argv ) {
    if( p_argc < 8 ) {
        fprintf( stderr, "too few args." );
        return 1;
    }

    // pkmnnames abtynames movenames itemnames pkmndata pkmndescr pkmnformnames
    // pkmnformdescr

    readNames( p_argv[ 1 ], pkmn_names);
    for( size_t i = 0; i < pkmn_names.size( ); ++i )
        pkmns[ pkmn_names[ i ].m_name[ 0 ] ] = i;
    readNames( p_argv[ 2 ], ability_names);
    for( size_t i = 0; i < ability_names.size( ); ++i )
        abilities[ ability_names[ i ].m_name[ 0 ] ] = i;
    readNames( p_argv[ 3 ], move_names);
    for( size_t i = 0; i < move_names.size( ); ++i )
        moves[ move_names[ i ].m_name[ 0 ] ] = i;
    readItems( p_argv[ 4 ], item_names);
    for( size_t i = 0; i < item_names.size( ); ++i )
        items[ item_names[ i ].m_name[ 0 ] ] = i;

    readPkmnData( p_argv[ 5 ], p_argv[ 6 ], p_argv[ 7 ], p_argv[ 8 ], pkmn_data, forme_names,
                  forme_data );

    printPkmnData( );
    printItemData( );
    printAbilityData( );
    printMoveData( );
}
