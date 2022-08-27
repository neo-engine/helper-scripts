// Creates a combined image of the map files. Optionally shrinks the image and marks
// routes, etc

#include <bit>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include <png.h>
#include "bitmap.h"
#include "fsdata.h"
#include "locationNames.h"

constexpr u8  MAX_ANIM_PER_TILE_SET   = 32;
constexpr u16 MAX_TILES_PER_TILE_SET  = 512;
constexpr u16 MAX_BLOCKS_PER_TILE_SET = 512;

std::string   MAP_PATH;
std::string   TILESET_PATH;
std::string   BLOCKSET_PATH;
std::string   PALETTE_PATH;
std::string   MAPDATA_PATH;
constexpr u16 SIZE          = 32;
constexpr s16 dir[ 4 ][ 2 ] = { { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 } };

char TMP_BUFFER[ 100 ];
char TMP_BUFFER_SHORT[ 50 ];

typedef struct {
    u16 m_pal[ 16 ];
} palette;
typedef struct {
    u8 m_tile[ 32 ];
} tile;

typedef struct {
    u16 m_tileidx : 10;
    u8  m_vflip : 1;
    u8  m_hflip : 1;
    u8  m_palno : 4;
} blockAtom;
// typedef u16 blockAtom;

class block {
  public:
    blockAtom m_top[ 2 ][ 2 ];
    u8        m_topbehave;

    blockAtom m_bottom[ 2 ][ 2 ];
    u8        m_bottombehave;
};

struct tileSet {
    struct animation {
        u16  m_tileIdx;
        u8   m_acFrame;
        u8   m_maxFrame;
        u8   m_speed;
        tile m_tiles[ 16 ];
    };
    u8        m_animationCount1, m_animationCount2;
    animation m_animations[ 2 * MAX_ANIM_PER_TILE_SET ];
    tile      m_tiles[ 2 * MAX_TILES_PER_TILE_SET ];
};
struct blockSet {
    block m_blocks[ 2 * MAX_BLOCKS_PER_TILE_SET ];
};

struct mapBlockAtom {
    u16 m_blockidx : 10;
    u8  m_movedata : 6;
};

struct mapSlice {
    palette      m_pals[ 16 * 5 ]; // 16 pals for each time of day
    tileSet      m_tileSet;
    blockSet     m_blockSet;
    mapBlockAtom m_blocks[ SIZE ][ SIZE ]; // [ y ][ x ]
    u8           m_map;
    u16          m_x, m_y;
    u8           m_tIdx1, m_tIdx2;
};

struct position {
    u16 m_posX; // Global
    u16 m_posY; // Global
    u8  m_posZ;

    constexpr auto operator<=>( const position& ) const = default;
};
enum direction : u8 { UP, RIGHT, DOWN, LEFT };

struct movement {
    direction m_direction;
    u8        m_frame;
};

typedef std::pair<u8, position> warpPos;
enum moveMode {
    // Player modes
    WALK       = 0,
    BIKE       = ( 1 << 2 ),
    ACRO_BIKE  = ( 1 << 2 ) | ( 1 << 0 ),
    MACH_BIKE  = ( 1 << 2 ) | ( 1 << 1 ),
    BIKE_JUMP  = ( 1 << 2 ) | ( 1 << 0 ) | ( 1 << 1 ),
    SIT        = ( 1 << 3 ),
    DIVE       = ( 1 << 4 ),
    ROCK_CLIMB = ( 1 << 5 ),
    STRENGTH   = ( 1 << 6 ),
    SURF       = ( 1 << 7 ),
    // NPC modes
    NO_MOVEMENT            = 0,
    LOOK_UP                = 1,
    LOOK_DOWN              = 2,
    LOOK_RIGHT             = 4,
    LOOK_LEFT              = 8,
    WALK_LEFT_RIGHT        = 16,
    WALK_UP_DOWN           = 17,
    WALK_CIRCLE            = 18,
    WALK_AROUND_LEFT_RIGHT = 19,
    WALK_AROUND_UP_DOWN    = 20,
};
enum eventType : u8 {
    EVENT_NONE        = 0,
    EVENT_MESSAGE     = 1,
    EVENT_ITEM        = 2,
    EVENT_TRAINER     = 3,
    EVENT_OW_PKMN     = 4,
    EVENT_NPC         = 5,
    EVENT_WARP        = 6,
    EVENT_GENERIC     = 7,
    EVENT_HMOBJECT    = 8, // cut, rock smash, strength
    EVENT_BERRYTREE   = 9,
    EVENT_NPC_MESSAGE = 10,
};
enum eventTrigger : u8 {
    TRIGGER_NONE           = 0,
    TRIGGER_STEP_ON        = ( 1 << 0 ),
    TRIGGER_INTERACT       = ( 1 << 1 ) | ( 1 << 2 ) | ( 1 << 3 ) | ( 1 << 4 ),
    TRIGGER_INTERACT_DOWN  = ( 1 << 1 ),
    TRIGGER_INTERACT_LEFT  = ( 1 << 2 ),
    TRIGGER_INTERACT_UP    = ( 1 << 3 ),
    TRIGGER_INTERACT_RIGHT = ( 1 << 4 ),
    TRIGGER_ON_MAP_ENTER   = ( 1 << 5 ),
};

enum wildPkmnType : u8 {
    GRASS,
    HIGH_GRASS,
    WATER,
    OLD_ROD,
    GOOD_ROD,
    SUPER_ROD,
    HEADBUTT,
    ROCK_SMASH,
    SWEET_SCENT,
};
enum mapWeather : u8 {
    NOTHING        = 0, // Inside
    SUNNY          = 1,
    REGULAR        = 2,
    RAINY          = 3,
    SNOW           = 4,
    THUNDERSTORM   = 5,
    MIST           = 6,
    BLIZZARD       = 7,
    SANDSTORM      = 8,
    FOG            = 9,
    DENSE_MIST     = 0xa,
    CLOUDY         = 0xb, // Dark Forest clouds
    HEAVY_SUNLIGHT = 0xc,
    HEAVY_RAIN     = 0xd,
    UNDERWATER     = 0xe
};
enum mapType : u8 { OUTSIDE = 0, CAVE = 1, INSIDE = 2, DARK = 4, FLASHABLE = 8 };
enum warpType : u8 {
    NO_SPECIAL,
    CAVE_ENTRY,
    DOOR,
    TELEPORT,
    EMERGE_WATER,
    LAST_VISITED,
    SLIDING_DOOR
};

constexpr u8 MAX_EVENTS_PER_SLICE = 64;
struct mapData {
    mapType    m_mapType;
    mapWeather m_weather;
    u8         m_battleBG;
    u8         m_battlePlat1;
    u8         m_battlePlat2;
    u8         m_surfBattleBG;
    u8         m_surfBattlePlat1;
    u8         m_surfBattlePlat2;

    u16 m_baseLocationId;
    u8  m_extraLocationCount;
    struct locationData {
        u8  m_left;
        u8  m_top;
        u8  m_right;
        u8  m_bottom;
        u16 m_locationId;
    } m_extraLocations[ 4 ];
    u8 m_pokemonDescrCount;
    struct wildPkmnData {
        u16          m_speciesId;
        u8           m_forme;
        wildPkmnType m_encounterType;
        u8           m_slot;
        u8           m_daytime;
        u8           m_encounterRate;
    } m_pokemon[ 30 ];
    u8 m_eventCount;
    struct event {
        u8           m_posX;
        u8           m_posY;
        u8           m_posZ;
        u16          m_activateFlag;
        u16          m_deactivateFlag;
        eventType    m_type;
        eventTrigger m_trigger;
        union data {
            struct {
                u8  m_msgType;
                u16 m_msgId;
            } m_message;
            struct {
                u8  m_itemType;
                u16 m_itemId;
            } m_item;
            struct {
                u8  m_movementType;
                u16 m_spriteId;
                u16 m_trainerId;
                u8  m_sight;
            } m_trainer;
            struct {
                u16 m_speciesId;
                u8  m_level;
                u8  m_forme; // BIT(6) female; BIT(7) genderless
                u8  m_shiny; // BIT(6) hidden ability, BIT(7) fateful
            } m_owPkmn;
            struct {
                u8  m_movementType;
                u16 m_spriteId;
                u16 m_scriptId;
                u8  m_scriptType;
            } m_npc;
            struct {
                warpType m_warpType;
                u8       m_bank;
                u8       m_mapY;
                u8       m_mapX;
                u8       m_posX;
                u8       m_posY;
                u8       m_posZ;
            } m_warp;
            struct {
                u16 m_scriptId;
                u8  m_scriptType;
            } m_generic;
            struct {
                u8 m_hmType;
            } m_hmObject;
            struct {
                u8 m_treeIdx; // internal id of this berry tree
            } m_berryTree;
        } m_data;
    } m_events[ MAX_EVENTS_PER_SLICE ];
};

size_t read( FILE* p_stream, void* p_buffer, size_t p_size, size_t p_count ) {
    if( !p_stream ) return 0;
    return fread( p_buffer, p_size, p_count, p_stream );
}
bool readNop( FILE* p_file, u32 p_cnt ) {
    u8 tmp;
    for( u32 i = 0; i < p_cnt; ++i ) {
        if( !read( p_file, &tmp, sizeof( u8 ), 1 ) ) return false;
    }
    return true;
}

FILE* open( const char* p_path, const char* p_name, const char* p_ext = ".raw",
            const char* p_mode = "rb" ) {
    snprintf( TMP_BUFFER, 99, "%s%s%s", p_path, p_name, p_ext );
    return fopen( TMP_BUFFER, p_mode );
}
FILE* open( const char* p_path, u16 p_value, const char* p_ext = ".raw",
            const char* p_mode = "rb" ) {
    snprintf( TMP_BUFFER, 99, "%s%d%s", p_path, p_value, p_ext );
    return fopen( TMP_BUFFER, p_mode );
}

bool readTiles( FILE* p_file, tile* p_tileSet, u16 p_startIdx = 0, u16 p_size = 512 ) {
    if( p_file == 0 ) return false;
    read( p_file, p_tileSet + p_startIdx, sizeof( tile ) * p_size, 1 );
    return true;
}

bool readPal( FILE* p_file, palette* p_palette, u8 p_count = 6 ) {
    if( p_file == 0 ) return false;
    read( p_file, p_palette, sizeof( u16 ) * 16, p_count );
    return true;
}

bool readBlocks( FILE* p_file, block* p_tileSet, u16 p_startIdx = 0, u16 p_size = 512 ) {
    if( p_file == 0 ) return false;
    readNop( p_file, 4 );
    for( u16 i = 0; i < p_size; ++i ) {
        read( p_file, &( p_tileSet + p_startIdx + i )->m_bottom, 4 * sizeof( blockAtom ), 1 );
        read( p_file, &( p_tileSet + p_startIdx + i )->m_top, 4 * sizeof( blockAtom ), 1 );
    }
    for( u16 i = 0; i < p_size; ++i ) {
        read( p_file, &( p_tileSet + p_startIdx + i )->m_bottombehave, sizeof( u8 ), 1 );
        read( p_file, &( p_tileSet + p_startIdx + i )->m_topbehave, sizeof( u8 ), 1 );
    }
    return true;
}

bool readMapData( FILE* p_file, mapData& p_result ) {
    if( !p_file ) {
        std::memset( &p_result, 0, sizeof( mapData ) );
        return false;
    }
    fread( &p_result, sizeof( mapData ), 1, p_file );
    fclose( p_file );
    return true;
}

void constructMapSlice( FILE* p_mapFile, mapSlice* p_result, u16 p_x = 0, u16 p_y = 0 ) {
    p_result->m_x = p_x;
    p_result->m_y = p_y;

    //    printf( "constructing slice\n" );

    readNop( p_mapFile, 8 ); // map size ignored

    u8 tsidx1, tsidx2;

    read( p_mapFile, &tsidx1, sizeof( u8 ), 1 );
    readNop( p_mapFile, 3 );
    read( p_mapFile, &tsidx2, sizeof( u8 ), 1 );
    readNop( p_mapFile, 3 );

    // printf( "red tsidx %hhu %hhu\n", tsidx1, tsidx2 );

    // border size
    u8 b1, b2;
    //    readNop( p_mapFile, 4 );
    read( p_mapFile, &b1, sizeof( u8 ), 1 );
    read( p_mapFile, &b2, sizeof( u8 ), 1 );
    readNop( p_mapFile, 2 );

    if( b1 && b2 ) {
        read( p_mapFile, p_result->m_blocks, sizeof( mapBlockAtom ), b1 * b2 ); // Border blocks
    }

    read( p_mapFile, p_result->m_blocks, sizeof( mapBlockAtom ), SIZE * SIZE );
    fclose( p_mapFile );

    // read the first tileset

    FILE* f = open( TILESET_PATH.c_str( ), tsidx1, ".ts" );
    readTiles( f, p_result->m_tileSet.m_tiles );
    fclose( f );

    f = open( BLOCKSET_PATH.c_str( ), tsidx1, ".bvd" );
    readBlocks( f, p_result->m_blockSet.m_blocks );
    fclose( f );

    f = open( PALETTE_PATH.c_str( ), tsidx1, ".p2l" );
    for( u8 i = 0; i < 5; ++i ) { readPal( f, p_result->m_pals + i * 16, 8 ); }
    fclose( f );

    p_result->m_tIdx1 = tsidx1;

    f = open( TILESET_PATH.c_str( ), tsidx2, ".ts" );
    if( !f ) {
        fprintf( stderr, "Tileset %hhu does not exist!\n", tsidx2 );
    } else {
        readTiles( f, p_result->m_tileSet.m_tiles, 512 );
        fclose( f );
    }

    f = open( BLOCKSET_PATH.c_str( ), tsidx2, ".bvd" );
    if( !f ) {
        fprintf( stderr, "Blockset %hhu does not exist!\n", tsidx2 );
    } else {
        readBlocks( f, p_result->m_blockSet.m_blocks, 512 );
        fclose( f );
    }

    f = open( PALETTE_PATH.c_str( ), tsidx2, ".p2l" );
    if( !f ) {
        fprintf( stderr, "Palette %hhu does not exist!\n", tsidx2 );
    } else {
        for( u8 i = 0; i < 5; ++i ) { readPal( f, p_result->m_pals + 6 + i * 16, 8 ); }
        fclose( f );
    }

    p_result->m_tIdx2 = tsidx2;
}

#define conv( a ) ( ( u8 )( (a) *31 / 255 ) )
#define revconv( a ) ( ( (a) *255 / 31 ) )

#define green( a ) ( revconv( ( ( a ) >> 5 ) & 31 ) )
#define blue( a ) ( revconv( ( ( a ) >> 10 ) & 31 ) )
#define red( a ) ( revconv( ( ( a ) >> 0 ) & 31 ) )

void renderTile( tile* p_tile, palette* p_pal, bool p_flipX, bool p_flipY, bitmap* p_out,
                 u32 p_x = 0, u32 p_y = 0, u16 p_scale = 1 ) {
    // a tile is a 8 by 8 pixel palette indexed image; each pixel occupies 4 bits

    // TODO: use scale
    for( u8 x = 0; x < 8; ++x ) {
        for( u8 y = 0; y < 8; ++y ) {
            u8 px = x, py = y;
            if( p_flipX ) { px = 7 - x; }
            if( p_flipY ) { py = 7 - y; }

            // get palette idx for this pixel
            u8 idx = p_tile->m_tile[ 4 * py + px / 2 ];
            if( !p_flipX ) {
                if( x & 1 ) {
                    idx >>= 4;
                } else {
                    idx &= 15;
                }
            } else {
                if( x & 1 ) {
                    idx &= 15;
                } else {
                    idx >>= 4;
                }
            }

            if( !idx ) { continue; } // first pal entry is "transparent"

            // get color for the palette idx
            u16 color = p_pal->m_pal[ idx ];

            ( *p_out )( p_x + x / p_scale, p_y + y / p_scale )
                = pixel( red( color ), green( color ), blue( color ) );
        }
    }
}

void renderBlock( block* p_block, tileSet* p_tiles, palette p_pals[ 16 * 5 ], bitmap* p_out,
                  u32 p_x = 0, u32 p_y = 0, u16 p_scale = 1, u8 p_time = 0 ) {
    u16 tilesize = 8 / p_scale;
    for( u16 x = 0; x < 2; ++x ) {
        for( u16 y = 0; y < 2; ++y ) {
            // render the bottom tile first
            blockAtom ba = p_block->m_bottom[ y ][ x ];
            renderTile( &p_tiles->m_tiles[ ba.m_tileidx ], &p_pals[ 16 * p_time + ba.m_palno ],
                        ba.m_vflip, ba.m_hflip, p_out, p_x + x * tilesize, p_y + y * tilesize,
                        p_scale );

            // draw top tile on top of bottom tile
            ba = p_block->m_top[ y ][ x ];
            renderTile( &p_tiles->m_tiles[ ba.m_tileidx ], &p_pals[ 16 * p_time + ba.m_palno ],
                        ba.m_vflip, ba.m_hflip, p_out, p_x + x * tilesize, p_y + y * tilesize,
                        p_scale );
        }
    }
}

#define BLOCK_SIZE 16

/*
 * @brief: renders the given mapSlice and outputs starting at the specified upper left
 * corner of the given bitmap.
 */
void renderMapSlice( mapSlice* p_mapSlice, bitmap* p_out, u32 p_x = 0, u32 p_y = 0, u16 p_scale = 1,
                     u8 p_time = 0 ) {
    u16 blocksize = BLOCK_SIZE / p_scale;

    for( u16 x = 0; x < SIZE; ++x ) {
        for( u16 y = 0; y < SIZE; ++y ) {
            renderBlock(
                &p_mapSlice->m_blockSet.m_blocks[ p_mapSlice->m_blocks[ y ][ x ].m_blockidx ],
                &p_mapSlice->m_tileSet, p_mapSlice->m_pals, p_out, p_x + blocksize * x,
                p_y + blocksize * y, p_scale, p_time );
        }
    }
}

constexpr pixel tint( const pixel& p_target, const pixel& p_tint, u8 p_force ) {
    return p_target & p_tint;
}

mapSlice SLICE;

#define TINT_FORCE 1
void tintRectangle( bitmap& p_out, u32 p_tx, u32 p_ty, u32 p_bx, u32 p_by, pixel p_tint,
                    u32 p_borderStrength = 0, pixel p_border = pixel( 0, 0, 0 ),
                    u8 p_borderSides = 0 << 0 | 0 << 1 | 0 << 2 | 0 << 3 ) {

    u32 topb   = ( p_borderSides & 1 ) ? p_borderStrength : 0;
    u32 leftb  = ( p_borderSides & 2 ) ? p_borderStrength : 0;
    u32 downb  = ( p_borderSides & 4 ) ? p_borderStrength : 0;
    u32 rightb = ( p_borderSides & 8 ) ? p_borderStrength : 0;

    for( u32 y = p_ty + topb; y < p_by - downb; ++y ) {
        for( u32 x = p_tx + leftb; x < p_bx - rightb; ++x ) {
            p_out( x, y ) = tint( p_out( x, y ), p_tint, TINT_FORCE );
        }
    }

    // edges
    for( u32 y = p_ty + topb; y < p_by - downb; ++y ) {
        for( u32 x = p_tx; x < p_tx + leftb; ++x ) {
            p_out( x, y ) = tint( p_out( x, y ), p_border, TINT_FORCE );
        }
    }
    for( u32 y = p_ty + topb; y < p_by - downb; ++y ) {
        for( u32 x = p_bx - rightb; x < p_bx; ++x ) {
            p_out( x, y ) = tint( p_out( x, y ), p_border, TINT_FORCE );
        }
    }
    for( u32 y = p_ty; y < p_ty + topb; ++y ) {
        for( u32 x = p_tx + leftb; x < p_bx - rightb; ++x ) {
            p_out( x, y ) = tint( p_out( x, y ), p_border, TINT_FORCE );
        }
    }
    for( u32 y = p_by - downb; y < p_by; ++y ) {
        for( u32 x = p_tx + leftb; x < p_bx - rightb; ++x ) {
            p_out( x, y ) = tint( p_out( x, y ), p_border, TINT_FORCE );
        }
    }

    // corners
    for( u32 y = p_ty; y < p_ty + topb; ++y ) {
        for( u32 x = p_tx; x < p_tx + leftb; ++x ) {
            if( p_borderStrength > 1 && x - p_tx + y - p_ty <= p_borderStrength ) { continue; }
            p_out( x, y ) = tint( p_out( x, y ), p_border, TINT_FORCE );
        }
    }
    for( u32 y = p_by - downb; y < p_by; ++y ) {
        for( u32 x = p_tx; x < p_tx + leftb; ++x ) {
            if( p_borderStrength > 1 && x - p_tx + p_by - y <= p_borderStrength ) { continue; }
            p_out( x, y ) = tint( p_out( x, y ), p_border, TINT_FORCE );
        }
    }
    for( u32 y = p_ty; y < p_ty + topb; ++y ) {
        for( u32 x = p_bx - rightb; x < p_bx; ++x ) {
            if( p_borderStrength > 1 && p_bx - x + y - p_ty <= p_borderStrength ) { continue; }
            p_out( x, y ) = tint( p_out( x, y ), p_border, TINT_FORCE );
        }
    }
    for( u32 y = p_by - downb; y < p_by; ++y ) {
        for( u32 x = p_bx - rightb; x < p_bx; ++x ) {
            if( p_borderStrength > 1 && p_bx - x + p_by - y <= p_borderStrength ) { continue; }
            p_out( x, y ) = tint( p_out( x, y ), p_border, TINT_FORCE );
        }
    }
}

constexpr pixel colorForLocation( u16 p_loc ) {
    auto routecol    = pixel( 230, 200, 30, 100 );
    auto searoutecol = pixel( 7, 211, 255, 100 );
    auto citycol     = pixel( 255, 68, 119, 100 );
    auto specialcol  = pixel( 0, 255, 0, 100 );

    switch( p_loc ) {
    case L_METEOR_FALLS:
    case L_RUSTURF_TUNNEL:
    case L_DESERT_RUINS:
    case L_GRANITE_CAVE:
    case L_PETALBURG_WOODS:
    case L_MT_CHIMNEY:
    case L_JAGGED_PASS:
    case L_FIERY_PATH:
    case L_MT_PYRE:
    case L_TEAM_AQUA_HIDEOUT:
    case L_SEAFLOOR_CAVERN:
    case L_CAVE_OF_ORIGIN:
    case L_VICTORY_ROAD3:
    case L_SHOAL_CAVE:
    case L_NEW_MAUVILLE:
    case L_ABANDONED_SHIP:
    case L_ISLAND_CAVE:
    case L_ANCIENT_TOMB:
    case L_SEALED_CHAMBER:
    case L_SCORCHED_SLAB:
    case L_TEAM_MAGMA_HIDEOUT:
    case L_SKY_PILLAR:
    case L_BATTLE_FRONTIER:
    case L_SOUTHERN_ISLAND:
    case L_SS_TIDAL:
    case L_MIRAGE_FOREST:
    case L_MIRAGE_CAVE:
    case L_MIRAGE_ISLAND:
    case L_MIRAGE_MOUNTAIN:
    case L_TRACKLESS_FOREST:
    case L_PATHLESS_PLAIN:
    case L_NAMELESS_CAVERN:
    case L_FABLED_CAVE:
    case L_GNARLED_DEN:
    case L_CRESCENT_ISLE:
    case L_SECRET_ISLET:
    case L_SECRET_SHORE:
    case L_SECRET_MEADOW:
    case L_HIDDEN_LAKE:
    case L_POKEMON_LEAGUE2:
    case L_CRYSTAL_CAVERN:
    case L_SAFARI_ZONE:
        return specialcol;
    case L_LITTLEROOT_TOWN:
    case L_OLDALE_TOWN:
    case L_DEWFORD_TOWN:
    case L_LAVARIDGE_TOWN:
    case L_FALLARBOR_TOWN:
    case L_VERDANTURF_TOWN:
    case L_PACIFIDLOG_TOWN:
    case L_PETALBURG_CITY:
    case L_SLATEPORT_CITY:
    case L_MAUVILLE_CITY:
    case L_RUSTBORO_CITY:
    case L_FORTREE_CITY:
    case L_LILYCOVE_CITY:
    case L_MOSSDEEP_CITY:
    case L_SOOTOPOLIS_CITY:
    case L_EVER_GRANDE_CITY:
    case L_NEW_LILYCOVE_CITY:
    case L_BLOSSAND_TOWN:
    case L_CLIFFELTA_CITY:
        return citycol;
    case L_ROUTE_105:
    case L_ROUTE_106:
    case L_ROUTE_107:
    case L_ROUTE_108:
    case L_ROUTE_109:
    case L_ROUTE_122:
    case L_ROUTE_124:
    case L_ROUTE_125:
    case L_ROUTE_126:
    case L_ROUTE_127:
    case L_ROUTE_128:
    case L_ROUTE_129:
    case L_ROUTE_130:
    case L_ROUTE_131:
    case L_ROUTE_132:
    case L_ROUTE_133:
    case L_ROUTE_134:
    case L_ROUTE_137:
        return searoutecol;
    default:
        return routecol;
    }
}

int main( int p_argc, char* p_argv[] ) {
    if( p_argc < 9 ) {
        printf(
            "Too few arguments.\nUsage: %s MAP_BANK_PATH TILE_SETS_PATH BLOCKS_PATH "
            " PALETTES_PATH #MAPS_X #MAPS_Y MAP_BANK OUTPUT_FILENAME [TIME_OF_DAY] [SCALE] [GRID_SIZE] [GRID_SUBDIVS] [MAPDATA_PATH]",
            p_argv[ 0 ] );
    }

    MAP_PATH      = std::string( p_argv[ 1 ] );
    TILESET_PATH  = std::string( p_argv[ 2 ] );
    BLOCKSET_PATH = std::string( p_argv[ 3 ] );
    PALETTE_PATH  = std::string( p_argv[ 4 ] );

    u16 mapsX, mapsY, bank;
    sscanf( p_argv[ 5 ], "%hu", &mapsX );
    sscanf( p_argv[ 6 ], "%hu", &mapsY );
    sscanf( p_argv[ 7 ], "%hu", &bank );

    u8 scale = 1, time = 0, oldscale = 1;
    if( p_argc >= 10 ) { sscanf( p_argv[ 9 ], "%hhu", &time ); }
    if( p_argc >= 11 ) { sscanf( p_argv[ 10 ], "%hhu", &oldscale ); }

    u32 grid = 0, divs = 0;
    if( p_argc >= 12 ) { sscanf( p_argv[ 11 ], "%u", &grid ); }
    if( p_argc >= 13 ) { sscanf( p_argv[ 12 ], "%u", &divs ); }
    bool mapdata = false;
    if( p_argc >= 14 ) {
        mapdata      = true;
        MAPDATA_PATH = std::string( p_argv[ 13 ] );
    }

    u32 sizeX = mapsX * SIZE * BLOCK_SIZE / scale;
    u32 sizeY = mapsY * SIZE * BLOCK_SIZE / scale;

    bitmap out( sizeX, sizeY );
    char   buffer[ 50 ];

    for( u16 x = 0; x < mapsX; ++x ) {
        for( u16 y = 0; y < mapsY; ++y ) {
            // try to open map x|y

            FILE* mapF;

            // fprintf( stderr, "Map %d/%d,%d\n", bank, y, x );

            if( bank == 10 ) {
                snprintf( buffer, 49, "%hhu/%hu/%hu_%hu", bank, y, y, x );
                mapF = open( MAP_PATH.c_str( ), buffer, ".map" );
            } else {
                snprintf( buffer, 49, "%hhu/%hu_%hu", bank, y, x );
                mapF = open( MAP_PATH.c_str( ), buffer, ".map" );
            }
            if( !mapF ) {
                fprintf( stderr, "Map %d/%d,%d does not exist.\n", bank, y, x );
                continue;
            }

            //            printf( "file opened\n" );

            // construct slice
            constructMapSlice( mapF, &SLICE, x, y );

            //            printf( "slice constructed\n" );

            // draw slice
            renderMapSlice( &SLICE, &out, x * SIZE * BLOCK_SIZE / scale,
                            y * SIZE * BLOCK_SIZE / scale, scale, time );

            //            printf( "slice printed\n" );
        }
    }

    if( oldscale > 1 ) {
        printf( "Scaling down with factor %hhu.\n", oldscale );
        bitmap sout( sizeX / oldscale, sizeY / oldscale );

        // combine oldscale * oldscale pixel into a single new one

        for( u32 x = 0; x < sizeX / oldscale; ++x ) {
            for( u32 y = 0; y < sizeY / oldscale; ++y ) {
                pixel res = out( x * oldscale, y * oldscale );
                for( u32 x2 = 0; x2 < oldscale; ++x2 ) {
                    for( u32 y2 = 0; y2 < oldscale; ++y2 ) {
                        res ^= out( x * oldscale + x2, y * oldscale + y2 );
                    }
                }
                sout( x, y ) = res;
            }
        }
        out   = sout;
        scale = oldscale;
    }

    if( mapdata ) {
        mapData                       tmp;
        std::vector<std::vector<u16>> locs
            = std::vector<std::vector<u16>>( divs * sizeY, std::vector<u16>( divs * sizeX, 0 ) );

        auto bordercol = pixel( 255, 255, 255, 20 );

        for( u16 x = 0; x < mapsX; ++x ) {
            for( u16 y = 0; y < mapsY; ++y ) {
                // try to open map x|y

                FILE* mapF;

                // fprintf( stderr, "Map %d/%d,%d\n", bank, y, x );

                snprintf( buffer, 49, "%hhu/%hu/%hu_%hu", bank, y, y, x );
                mapF = open( MAPDATA_PATH.c_str( ), buffer, ".map.data" );
                if( !mapF ) {
                    fprintf( stderr, "Map data for %d/%d,%d does not exist.\n", bank, y, x );
                    continue;
                }

                readMapData( mapF, tmp );

                if( tmp.m_baseLocationId ) {
                    for( u8 x2 = 0; x2 < divs; ++x2 ) {
                        for( u8 y2 = 0; y2 < divs; ++y2 ) {
                            locs[ divs * y + y2 ][ divs * x + x2 ] = tmp.m_baseLocationId;
                        }
                    }
                }
                for( u8 i = 0; i < tmp.m_extraLocationCount; ++i ) {
                    for( u8 x2 = tmp.m_extraLocations[ i ].m_left * divs / SIZE;
                         x2 <= tmp.m_extraLocations[ i ].m_right * divs / SIZE; ++x2 ) {
                        for( u8 y2 = tmp.m_extraLocations[ i ].m_top * divs / SIZE;
                             y2 <= tmp.m_extraLocations[ i ].m_bottom * divs / SIZE; ++y2 ) {
                            locs[ divs * y + y2 ][ divs * x + x2 ]
                                = tmp.m_extraLocations[ i ].m_locationId;
                        }
                    }
                }
            }
        }

        printf( "{\n" );
        for( u16 y = 0; y < divs * mapsY; ++y ) {
            printf( "    {" );
            for( u16 x = 0; x < divs * mapsX; ++x ) { printf( " %hu,", locs[ y ][ x ] ); }
            printf( " },\n" );
        }
        printf( "}\n" );

        for( u16 y = 0; y < divs * mapsY; ++y ) {
            for( u16 x = 0; x < divs * mapsX; ++x ) {
                //                printf( "%04u ", locs[ y ][ x ] );
                if( !locs[ y ][ x ] ) { continue; }
                u8 border = 0;

                if( !y || locs[ y ][ x ] != locs[ y - 1 ][ x ] ) { border |= 1; }
                if( !x || locs[ y ][ x ] != locs[ y ][ x - 1 ] ) { border |= 2; }
                if( y + 1 >= divs * mapsY || locs[ y ][ x ] != locs[ y + 1 ][ x ] ) { border |= 4; }
                if( x + 1 >= divs * mapsX || locs[ y ][ x ] != locs[ y ][ x + 1 ] ) { border |= 8; }

                u8 borsize = std::max( 1, BLOCK_SIZE / scale );
                if( borsize == 1 && std::popcount( border ) >= 3
                    && colorForLocation( locs[ y ][ x ] ) != colorForLocation( L_ROUTE_110 ) ) {
                    borsize = 0;
                }
                if( borsize == 1 && ( border == 0b101 || border == 0b1010 )
                    && colorForLocation( locs[ y ][ x ] ) == colorForLocation( L_ISLAND_CAVE ) ) {
                    borsize = 0;
                }

                tintRectangle( out, x * ( SIZE / divs ) * BLOCK_SIZE / scale,
                               y * ( SIZE / divs ) * BLOCK_SIZE / scale,
                               ( x + 1 ) * ( SIZE / divs ) * BLOCK_SIZE / scale,
                               ( y + 1 ) * ( SIZE / divs ) * BLOCK_SIZE / scale,
                               colorForLocation( locs[ y ][ x ] ), borsize, bordercol, border );
            }
            //          printf( "\n" );
        }
    }

    if( grid ) {
        grid = ( grid * BLOCK_SIZE ) / scale;
        printf( "Drawing grid every %u pixel with %u subdivisions.\n", grid, divs );

        for( u32 x = 0; x < mapsX * SIZE * BLOCK_SIZE / scale; ++x ) {
            for( u32 y = 0; y < mapsY * SIZE * BLOCK_SIZE / scale; ++y ) {
                if( ( ( x % grid ) == 0 || ( y % grid ) == 0 ) ) {
                    out( x, y ) = pixel( 255, 255, 255 );
                    continue;
                }
                if( grid / divs ) {
                    if( ( x % ( grid / divs ) ) == 0 || ( y % ( grid / divs ) ) == 0 ) {
                        out( x, y ) = pixel( 150, 150, 150 );
                        continue;
                    }
                }
            }
        }
    }

    out.writeToFile( p_argv[ 8 ] );
    return 0;
}
