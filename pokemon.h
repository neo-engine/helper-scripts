/*
Pokémon Emerald 2 Version
------------------------------

file        : pokemon.h
author      : Philip Wellnitz
description : Header file. Consult the corresponding source file for details.

Copyright (C) 2012 - 2016
Philip Wellnitz

This file is part of Pokémon Emerald 2 Version.

Pokémon Emerald 2 Version is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Pokémon Emerald 2 Version is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Pokémon Emerald 2 Version.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <string>
#include <map>
#include <vector>
#include <cstdint>

#undef RAND_MAX
#define RAND_MAX 4294967295

#define MAX_PKMN 649

extern std::string NatureList[ 25 ];
extern std::string TypeList[ 19 ];
extern std::string PersonalityList[ 30 ];
extern std::string TasteList[ 6 ];

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned u32;
typedef signed char s8;
typedef short s16;
typedef int s32;

enum pkmnLevelUpType {
    MEDIUM_FAST = 0,
    ERRATIC = 1,
    FLUCTUATING = 2,
    MEDIUM_SLOW = 3,
    FAST = 4,
    SLOW = 5
};
enum pkmnColorType {
    RED_ = 0,
    BLUE_ = 1,
    YELLOW_ = 2,
    GREEN_ = 3,
    BLACK_ = 4,
    BROWN_ = 5,
    PURPLE_ = 6,
    GRAY_ = 7,
    WHITE_ = 8,
    PINK_ = 9
};
enum pkmnGenderType {
    GENDERLESS = 255,
    FEMALE = 254,
    //   MALE_125=223,
    MALE_250 = 191,
    MALE_500 = 127,
    MALE_750 = 63,
    MALE_875 = 31,
    MALE = 0
};
enum pkmnEggType {
    NONE_ = 0,
    PFLANZE_ = 8,
    KAEFER_ = 4,
    FLUG_ = 5,
    HUMANOTYP_ = 9,
    MINERAL_ = 11,
    AMORPH_ = 12,
    FELD_ = 6,
    WASSER1_ = 2,
    WASSER2_ = 13,
    WASSER3_ = 10,
    MONSTER_ = 1,
    FEE_ = 7,
    DRACHE_ = 15,
    DITTO_ = 14,
    UNBEKANNT_ = 16
};

enum pkmnNatures {
    ROBUST = 0, EINSAM = 1, MUTIG = 2,
    HART = 3, FRECH = 4, KUEHN = 5,
    SANFT = 6, LOCKER = 7, PFIFFIG = 8,
    LASCH = 9, SCHEU = 10, HASTIG = 11,
    ERNST = 12, FROH = 13, NAIV = 14,
    MAESSIG = 15, MILD = 16, RUHIG = 17,
    ZAGHAFT = 18, HITZIG = 19, STILL = 20,
    ZART = 21, FORSCH = 22, SACHT = 23,
    KAUZIG = 24
};

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
    char            m_displayName[ 15 ];
    char            m_species[ 50 ];
    char            m_dexEntry[ 200 ];

    u16             m_formeIdx[ 30 ];
    char            m_formeName[ 30 ][ 15 ];

    u16             m_preEvolution;
    u8              m_stage; //0: Basic, 1: Stage 1, 2: Stage 2, 3 Restored (m_preEvolution: itemIdx)
};
