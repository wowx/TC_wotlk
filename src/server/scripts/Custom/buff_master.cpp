//Maker: No idea
//Updated & thanks go to: Mireno

#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "ScriptedCreature.h"
#include "Player.h"
#include "Unit.h"
#include "WorldSession.h"
#include "Log.h"
#include "SpellInfo.h"
#include "Define.h"
#include "GossipDef.h"
#include "Chat.h"
#include "ObjectMgr.h"
#include "SpellMgr.h"
#include "Language.h"

#define BUFF_COST1 100000 // 10 golds
#define BUFF_COST2 100000 // 10 golds

enum spells
{
Soulstone_Resurection = 47883,  //'Soulstone Resurection'
Blessing_of_Kings = 20217,      //'Blessing of Kings'
Blessing_of_Might = 48932,      //'Blessing of Might'
Blessing_of_Wisdom = 48936,     //'Blessing of Wisdom'
Power_Word_Fortitude = 48161,   //'Power Word: Fortitude'
Divine_Spirit = 48073,          //'Divine Spirit'
Shadow_Protection = 48169,      //'Shadow Protection'
Mark_of_the_Wild = 48469,       //'Mark of the Wild'
Thorns = 53307,                 //'Thorns'
Horn_of_Winter = 57623,         //'Horn of Winter'
Arcane_Intelect = 42995,        //'Arcane Intelect'
Berserk_100 = 41924,            //'Berserk 100%'
Berserk_500 = 46587,            //'Berserk 500%'
Conjured_Mana_Strudel = 43523	//'Conjured Mana Strudel'
};

uint32 tank_auras[] = { 47883,20217,48161,48073,48169,48469,53307,57623,42995,43523 };
uint32 dmg_auras[] = { 47883,48932,48161,48073,48169,48469,53307,57623,42995,43523 };
uint32 spell_auras[] = { 47883,48936,48161,48073,48169,48469,53307,57623,42995,43523 };
uint32 heal_auras[] = { 47883,48936,48161,48073,48169,48469,53307,57623,42995,43523 };
uint32 zone_id[] = { 206,876,1196,2159,3456,3457,3606,3607,3717,3805,3836,3845,3923,3959,4075,4100,4196,4228,4264,4265,4272,4273,4277,4415,4416,4493,4494,4500,4603,4722,4723,4809,4812,4813,4820,4987};

class Npc_Buff_master : public CreatureScript
{
public:
    Npc_Buff_master() : CreatureScript("Npc_Buff_master") { }

    static bool OnGossipHello(Player *player, Creature * m_creature)
    {
        if(!player)
            return true;

        if(player->GetMoney() < BUFF_COST1 || player->GetMoney() < BUFF_COST2)
        {
           player->GetSession()->SendNotification("You can not do it now! You must have 10 golds.");
           return true;
        }

        // if (player->HasAura(SPELL_AURA_MOUNTED)) player->RemoveAurasByType(SPELL_AURA_MOUNTED);
        if (player->duel || player->GetMap()->IsBattleArena() || player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || player->isDead(), player->IsInCombat() || player->IsInFlight())
        {
            m_creature->Whisper("You can not do it now!", LANG_UNIVERSAL, player, false);
            player->GetSession()->SendNotification("You can not do it now!");
            CloseGossipMenuFor(player);
            return true;
        }

        // Buffs menu
        AddGossipItemFor(player, 6, "|cffff0000Buffs cost 10 golds.|r", GOSSIP_SENDER_MAIN, 0);
        AddGossipItemFor(player, 6, "|TInterface/ICONS/Spell_holy_devotionaura:30:30:-18:0|t TANK buffs", GOSSIP_SENDER_MAIN, 10);
        AddGossipItemFor(player, 6, "|TInterface/ICONS/Spell_holy_fistofjustice:30:30:-18:0|t DMG buffs", GOSSIP_SENDER_MAIN, 20);
        AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_marksmanship:30:30:-18:0|t SPELL buffs", GOSSIP_SENDER_MAIN, 30);
        AddGossipItemFor(player, 6, "|TInterface/ICONS/Spell_nature_healingtouch:30:30:-18:0|t HEAL buffs", GOSSIP_SENDER_MAIN, 40);
        AddGossipItemFor(player, 6, "|cffff0000Berserk cost 10 golds.|r", GOSSIP_SENDER_MAIN, 0);
        AddGossipItemFor(player, 6, "|TInterface/ICONS/Spell_shadow_unholyfrenzy:30:30:-18:0|t Berserk (Just in Raid and Dungeon)", GOSSIP_SENDER_MAIN, 60);
        AddGossipItemFor(player, 6, "|cffff0000|TInterface/ICONS/Ability_spy:30:30:-18:0|t Close Buffmaster Window.|r", GOSSIP_SENDER_MAIN, 50);
        SendGossipMenuFor(player, player->GetGossipTextId(m_creature), m_creature->GetGUID());

        return true;
    }

    static bool OnGossipSelect(Player *player, Creature * m_creature, uint32 sender, uint32 action)
    {

        player->PlayerTalkClass->ClearMenus();

        switch (action)
        {
        case 10: // TANK
            for(int i = 0; i < 11; i++)
                player->CastSpell(player, tank_auras[i], true);
            // player->AddAura(tank_auras[i], player);
            player->GetSession()->SendNotification("You are TANK buffs.");
            m_creature->Whisper("You are TANK buffs.", LANG_UNIVERSAL, player, false);
            player->SetMoney(player->GetMoney() - BUFF_COST1);
            break;
        case 20: // DMG
            for(int i = 0; i < 11; i++)
                player->CastSpell(player, dmg_auras[i], true);
            player->GetSession()->SendNotification("You are DMG buffs.");
            m_creature->Whisper("You are DMG buffs.", LANG_UNIVERSAL, player, false);
            player->SetMoney(player->GetMoney() - BUFF_COST1);
            break;
        case 30: // Spell
            for(int i = 0; i < 11; i++)
                player->CastSpell(player, spell_auras[i], true);
            player->GetSession()->SendNotification("You are SPELL buffs.");
            m_creature->Whisper("You are SPELL buffs.", LANG_UNIVERSAL, player, false);
            player->SetMoney(player->GetMoney() - BUFF_COST1);
            break;
        case 40: // HEAL
            for(int i = 0; i < 11; i++)
                player->CastSpell(player, heal_auras[i], true);
            player->GetSession()->SendNotification("You are HEAL buffs.");
            m_creature->Whisper("You are HEAL buffs.", LANG_UNIVERSAL, player, false);
            player->SetMoney(player->GetMoney() - BUFF_COST1);
            break;
        case 60: // Berserk
            /*for(int i = 0; i < int size_of_array=size(zone_id); i++)
            if (player->GetZoneId() != zone_id[i])
                return true; */
            player->CastSpell(player, 41924, true);
            player->CastSpell(player, 46587, true);
            player->GetSession()->SendNotification("You are Berserk buffs.");
            m_creature->Whisper("You are Berserk buffs.", LANG_UNIVERSAL, player, false);
            player->SetMoney(player->GetMoney() - BUFF_COST2);
            break;

        case 50: // Bye
            player->GetSession()->SendNotification("Bye...");
            m_creature->Whisper("Bye...", LANG_UNIVERSAL, player, false);
            break;

        }
        CloseGossipMenuFor(player);
        return true;
    }

        static bool OnGossipSelectCode(Player* player, Creature* m_creature, uint32 sender, uint32 action, char const* code)
        {
            player->PlayerTalkClass->ClearMenus();
            // code
            return true;
        }

        struct MyAI : public ScriptedAI
        {
            MyAI(Creature* m_creature) : ScriptedAI(m_creature) { }
            bool GossipHello(Player* player) override
            {
                return OnGossipHello(player, me);
            }
            bool GossipSelect(Player* player, uint32 menuId, uint32 gossipListId) override
            {
                uint32 sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
                uint32 action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
                return OnGossipSelect(player, me, sender, action);
            }
            bool GossipSelectCode(Player* player, uint32 menuId, uint32 gossipListId, char const* code) override
            {
                uint32 sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
                uint32 action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
                return OnGossipSelectCode(player, me, sender, action, code);
            }
        };

        CreatureAI* GetAI(Creature* m_creature) const override
        {
            return new MyAI(m_creature);
        }
};

void AddSC_Npc_Buff_master()
{
    new Npc_Buff_master;
}

/*
// SQL
-- --------------------------------------------------------------------------------------
--  BUFFER --
-- --------------------------------------------------------------------------------------
SET
@Entry      := 190146, -- change me
-- Alliance Version
-- @Model       := 4309, -- Human Male Tuxedo
-- @Name        := "Bruce Buffer",
-- @Title       := "Ph.D.",
-- Horde Version
@Model      := 21572, -- Buffer
@Name       := "Buffmaster Hasselhoof",
@Title      := "Entrix Crew",
@Icon       := "Speak",
@GossipMenu := 0,
@MinLevel   := 80,
@MaxLevel   := 80,
@Faction    := 35,
@NPCFlag    := 81,
@Scale      := 1.25,
@Rank       := 0,
@Type       := 7,
@TypeFlags  := 0,
@FlagsExtra := 2,
@AIName     := "SmartAI",
@Script     := "Npc_Buff_master";

-- NPC
DELETE FROM creature_template WHERE entry = @Entry;
INSERT INTO creature_template (entry, modelid1, NAME, subname, IconName, gossip_menu_id, minlevel, maxlevel, faction, npcflag, speed_walk, speed_run, scale, rank, unit_class, unit_flags, TYPE, type_flags, RegenHealth, flags_extra, AIName, ScriptName) VALUES
(@Entry, @Model, @Name, @Title, @Icon, @GossipMenu, @MinLevel, @MaxLevel, @Faction, @NPCFlag, 1, 1.14286, @Scale, @Rank, 1, 2, @Type, @TypeFlags, 1, @FlagsExtra, @AIName, @Script);

-- NPC EQUIPPED
DELETE FROM `creature_equip_template` WHERE `CreatureID`=@Entry AND `ID`=1;
INSERT INTO `creature_equip_template` VALUES (@Entry, 1, 1906, 0, 0, 18019); -- War Axe(14824), Torch
*/
