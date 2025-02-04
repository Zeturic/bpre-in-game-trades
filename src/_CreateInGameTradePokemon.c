#include "global.h"
#include "mail.h"
#include "pokemon.h"
#include "trade.c.h"
#include "constants/items.h"
#include "constants/region_map_sections.h"

#if INSERT_INGAME_TRADE_HACK

void CreateInGameTradePokemonInternal(u8 whichPlayerMon, u8 whichInGameTrade)
{
    const struct InGameTrade *inGameTrade = &sInGameTrades[whichInGameTrade];
    u8 level = inGameTrade->level ? inGameTrade->level : GetMonData(&gPlayerParty[whichPlayerMon], MON_DATA_LEVEL);

    struct MailStruct mail;
    u8 metLocation = METLOC_IN_GAME_TRADE;
    u8 isMail;
    struct Pokemon *pokemon = &gEnemyParty[0];

    CreateMon(pokemon, inGameTrade->species, level, USE_RANDOM_IVS, TRUE, inGameTrade->personality, OT_ID_PRESET, inGameTrade->otId);

    for (u8 i = 0; i < NUM_STATS; ++i)
    {
        if (inGameTrade->ivs[i] != 0xFF)
            SetMonData(pokemon, MON_DATA_HP_IV + i, &inGameTrade->ivs[i]);
    }

    SetMonData(pokemon, MON_DATA_NICKNAME, inGameTrade->nickname);
    SetMonData(pokemon, MON_DATA_OT_NAME, inGameTrade->otName);
    SetMonData(pokemon, MON_DATA_OT_GENDER, &inGameTrade->otGender);
    SetMonData(pokemon, MON_DATA_ABILITY_NUM, &inGameTrade->abilityNum);
    SetMonData(pokemon, MON_DATA_BEAUTY, &inGameTrade->conditions[1]);
    SetMonData(pokemon, MON_DATA_CUTE, &inGameTrade->conditions[2]);
    SetMonData(pokemon, MON_DATA_COOL, &inGameTrade->conditions[0]);
    SetMonData(pokemon, MON_DATA_SMART, &inGameTrade->conditions[3]);
    SetMonData(pokemon, MON_DATA_TOUGH, &inGameTrade->conditions[4]);
    SetMonData(pokemon, MON_DATA_SHEEN, &inGameTrade->sheen);
    SetMonData(pokemon, MON_DATA_POKERUS, &inGameTrade->pokerus);
    SetMonData(pokemon, MON_DATA_PP_BONUSES, &inGameTrade->ppBonuses);
    SetMonData(pokemon, MON_DATA_MET_LOCATION, &metLocation);

    for (u8 i = 0; i < MAX_MON_MOVES; ++i)
    {
        u16 move;

        if (inGameTrade->moveset != NULL)
        {
            move = inGameTrade->moveset[i];
            SetMonData(pokemon, MON_DATA_MOVE1 + i, &move);
        }
        else
            move = GetMonData(pokemon, MON_DATA_MOVE1 + i);

        u8 pp = CalculatePPWithBonus(move, inGameTrade->ppBonuses, i);
        SetMonData(pokemon, MON_DATA_PP1 + i, &pp);
    }

    if (inGameTrade->ball)
        SetMonData(pokemon, MON_DATA_POKEBALL, &inGameTrade->ball);

    isMail = FALSE;
    if (inGameTrade->heldItem != ITEM_NONE)
    {
        if (ItemIsMail(inGameTrade->heldItem))
        {
            GetInGameTradeMail(&mail, inGameTrade);
            gLinkPartnerMail[0] = mail;
            SetMonData(pokemon, MON_DATA_MAIL, &isMail);
            SetMonData(pokemon, MON_DATA_HELD_ITEM, &inGameTrade->heldItem);
        }
        else
        {
            SetMonData(pokemon, MON_DATA_HELD_ITEM, &inGameTrade->heldItem);
        }
    }
    CalculateMonStats(&gEnemyParty[0]);
}

#endif
