//Maker: No idea
//Updated & thanks go to: Rochet

#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "ScriptedCreature.h"
#include "Player.h"
#include "Unit.h"
#include "Pet.h"
#include "WorldSession.h"
#include "Log.h"
#include "SpellInfo.h"
#include "Define.h"
#include "GossipDef.h"
#include "Chat.h"
#include "Map.h"
#include "ObjectMgr.h"
#include "SpellMgr.h"
#include "Language.h"

class Npc_Beastmaster : public CreatureScript
{
public:
    Npc_Beastmaster() : CreatureScript("Npc_Beastmaster") { }

    void static CreatePet(Player *player, Creature * m_creature, uint32 entry) {

        if (player->GetClass() != CLASS_HUNTER)
        {
            m_creature->Whisper("You are not a Hunter!", LANG_UNIVERSAL, player, false);
            player->GetSession()->SendNotification("You are not a Hunter!");
            player->PlayerTalkClass->SendCloseGossip();
            return;
        }

        if (player->GetPet() || player->GetPetGUID())
        {
            m_creature->Whisper("You must disband your current pet to tame a new one!", LANG_UNIVERSAL, player, false);
            player->GetSession()->SendNotification("You must disband your current pet to tame a new one!");
            player->PlayerTalkClass->SendCloseGossip();
            return;
        }

        Creature* newPet = m_creature->SummonCreature(entry, player->GetPositionX(), player->GetPositionY() + 2, player->GetPositionZ(), player->GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 500);
        if (!newPet)
            return;

        Pet* pet = player->CreateTamedPetFrom(newPet, 0);
        if (!pet)
            return;

        newPet->setDeathState(JUST_DIED);
        newPet->RemoveCorpse();
        newPet->SetHealth(0);

        pet->SetPower(POWER_HAPPINESS, 1048000);
        pet->SetUInt64Value(UNIT_FIELD_CREATEDBY, player->GetGUID());
        pet->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, player->GetFaction());
        pet->SetUInt32Value(UNIT_FIELD_LEVEL, player->GetLevel());
        pet->SetFullHealth();
        pet->GetMap()->AddToMap(pet->ToCreature());

        pet->GetCharmInfo()->SetPetNumber(sObjectMgr->GeneratePetNumber(), true);
        if (!pet->InitStatsForLevel(player->GetLevel()))
            pet->UpdateAllStats();

        player->SetMinion(pet, true);
        pet->SavePetToDB(PET_SAVE_AS_CURRENT);
        pet->InitTalentForLevel();
        player->PetSpellInitialize();

        player->PlayerTalkClass->SendCloseGossip();
        m_creature->Whisper("Pet added. You might want to feed it and name it somehow.", LANG_UNIVERSAL, player, false);
        player->GetSession()->SendNotification("Pet added. You might want to feed it and name it somehow.");
    }

    static bool OnGossipHello(Player *player, Creature * m_creature)
    {

        if (player->GetClass() != CLASS_HUNTER)
        {
            m_creature->Whisper("You are not a Hunter!", LANG_UNIVERSAL, player, false);
            player->GetSession()->SendNotification("You are not a Hunter!");
            CloseGossipMenuFor(player);
            return true;
        }

        //    Ability_hunter_pet_sporebat           Ability_hunter_cobrastrikes
        AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_beasttaming:30:30:-18:0|t Get a New Pet.", GOSSIP_SENDER_MAIN, 30);
        if (player->CanTameExoticPets())
        {
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_beastmastery:30:30:-18:0|t Get a New Exotic Pet.", GOSSIP_SENDER_MAIN, 50);
        }
        AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_beastcall:30:30:-18:0|t Take me to the Stable.", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_STABLEPET);
        if (m_creature->IsVendor())
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_beasttraining:30:30:-18:0|t Sell me some Food for my Pet.", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_VENDOR);
        if(player->GetPet())
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_beastcall02:30:30:-18:0|t Reset Pet Talent", GOSSIP_SENDER_MAIN, 200);

        AddGossipItemFor(player, 6, "|cffff0000|TInterface/ICONS/Ability_spy:30:30:-18:0|t Close Beastmaster Window.|r", GOSSIP_SENDER_MAIN, 150);
        SendGossipMenuFor(player, player->GetGossipTextId(m_creature), m_creature->GetGUID());
        //CloseGossipMenuFor(player);

        return true;
    }

    static bool OnGossipSelect(Player *player, Creature * m_creature, uint32 sender, uint32 action)
    {

        player->PlayerTalkClass->ClearMenus();
        switch (action)
        {

        case 100:
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_beasttaming:30:30:-18:0|t Get a New Pet.", GOSSIP_SENDER_MAIN, 30);
            if (player->CanTameExoticPets())
            {
                AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_beastmastery:30:30:-18:0|t Get a New Exotic Pet.", GOSSIP_SENDER_MAIN, 50);
            }
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_beastcall:30:30:-18:0|t Take me to the Stable.", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_STABLEPET);
            if (m_creature->IsVendor())
                AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_beasttraining:30:30:-18:0|t Sell me some Food for my Pet.", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_VENDOR);
            if(player->GetPet())
                AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_beastcall02:30:30:-18:0|t Reset Pet Talent", GOSSIP_SENDER_MAIN, 200);

            AddGossipItemFor(player, 6, "|cffff0000|TInterface/ICONS/Ability_spy:30:30:-18:0|t Close Beastmaster Window.|r", GOSSIP_SENDER_MAIN, 150);
            SendGossipMenuFor(player, player->GetGossipTextId(m_creature), m_creature->GetGUID());
            //CloseGossipMenuFor(player);
            break;

        case 150:
            CloseGossipMenuFor(player);
            player->GetSession()->SendNotification("Bye...");
            m_creature->Whisper("Bye...", LANG_UNIVERSAL, player, false);
            break;

        case 200:
            if(!player->GetPet())
            {
                player->GetSession()->SendNotification("First call pet.");
                m_creature->Whisper("First call pet.", LANG_UNIVERSAL, player, false);
            }
            else
            {
                player->ResetPetTalents();
                player->GetSession()->SendNotification("Pet talent reset.");
                m_creature->Whisper("Pet talent reset.", LANG_UNIVERSAL, player, false);
            }
            CloseGossipMenuFor(player);
            break;

        case 30: // |TInterface/ICONS/Misc_arrowleft:30:30:-18:0|t
            AddGossipItemFor(player, 2, " <- Back to Main Menu.", GOSSIP_SENDER_MAIN, 100);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Spell_chargepositive:30:30:-18:0|t Next Page. ->", GOSSIP_SENDER_MAIN, 31);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_bat:30:30:-18:0|t Bat.", GOSSIP_SENDER_MAIN, 18);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_bear:30:30:-18:0|t Bear.", GOSSIP_SENDER_MAIN, 1);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_boar:30:30:-18:0|t Boar.", GOSSIP_SENDER_MAIN, 2);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_cat:30:30:-18:0|t Cat.", GOSSIP_SENDER_MAIN, 4);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_vulture:30:30:-18:0|t Carrion Bird.", GOSSIP_SENDER_MAIN, 5);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_crab:30:30:-18:0|t Crab.", GOSSIP_SENDER_MAIN, 6);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_crocolisk:30:30:-18:0|t Crocolisk.", GOSSIP_SENDER_MAIN, 7);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_dragonhawk:30:30:-18:0|t Dragonhawk.", GOSSIP_SENDER_MAIN, 17);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_gorilla:30:30:-18:0|t Gorilla.", GOSSIP_SENDER_MAIN, 8);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_mount_blackdirewolf:30:30:-18:0|t Hound.", GOSSIP_SENDER_MAIN, 9);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_hyena:30:30:-18:0|t Hyena.", GOSSIP_SENDER_MAIN, 10);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_moth:30:30:-18:0|t Moth.", GOSSIP_SENDER_MAIN, 11);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_owl:30:30:-18:0|t Owl.", GOSSIP_SENDER_MAIN, 12);
            SendGossipMenuFor(player, player->GetGossipTextId(m_creature), m_creature->GetGUID());
            //CloseGossipMenuFor(player);
            break;

        case 31:
            AddGossipItemFor(player, 2, " <- Back to Main Menu.", GOSSIP_SENDER_MAIN, 30);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Spell_chargenegative:30:30:-18:0|t <- Previous Page.", GOSSIP_SENDER_MAIN, 30);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_raptor:30:30:-18:0|t Raptor.", GOSSIP_SENDER_MAIN, 20);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_ravager:30:30:-18:0|t Ravager.", GOSSIP_SENDER_MAIN, 19);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_tallstrider:30:30:-18:0|t Strider.", GOSSIP_SENDER_MAIN, 13);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_scorpid:30:30:-18:0|t Scorpid.", GOSSIP_SENDER_MAIN, 414);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_spider:30:30:-18:0|t Spider.", GOSSIP_SENDER_MAIN, 16);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_windserpent:30:30:-18:0|t Serpent.", GOSSIP_SENDER_MAIN, 21);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_turtle:30:30:-18:0|t Turtle.", GOSSIP_SENDER_MAIN, 15);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_wasp:30:30:-18:0|t Wasp.", GOSSIP_SENDER_MAIN, 93);
            SendGossipMenuFor(player, player->GetGossipTextId(m_creature), m_creature->GetGUID());
            //CloseGossipMenuFor(player);
            break;

        case 50:
            AddGossipItemFor(player, 2, " <- Back to Main Menu.", GOSSIP_SENDER_MAIN, 100);
            AddGossipItemFor(player, 2, "|TInterface/ICONS/Ability_hunter_beasttaming:30:30:-18:0|t <- Get a New Normal Pet.", GOSSIP_SENDER_MAIN, 100);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_chimera:30:30:-18:0|t Chimaera.", GOSSIP_SENDER_MAIN, 51);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_corehound:30:30:-18:0|t Core Hound.", GOSSIP_SENDER_MAIN, 52);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_devilsaur:30:30:-18:0|t Devilsaur.", GOSSIP_SENDER_MAIN, 53);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_rhino:30:30:-18:0|t Rhino.", GOSSIP_SENDER_MAIN, 54);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_silithid:30:30:-18:0|t Silithid.", GOSSIP_SENDER_MAIN, 55);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_worm:30:30:-18:0|t Worm.", GOSSIP_SENDER_MAIN, 56);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_druid_primalprecision:30:30:-18:0|t Loque'nahak.", GOSSIP_SENDER_MAIN, 57);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_hunter_pet_wolf:30:30:-18:0|t Skoll.", GOSSIP_SENDER_MAIN, 58);
            AddGossipItemFor(player, 6, "|TInterface/ICONS/Ability_druid_primalprecision:30:30:-18:0|t Gondria.", GOSSIP_SENDER_MAIN, 59);
            SendGossipMenuFor(player, player->GetGossipTextId(m_creature), m_creature->GetGUID());
            //CloseGossipMenuFor(player);
            break;

        case GOSSIP_OPTION_STABLEPET:
            player->GetSession()->SendStablePet(m_creature->GetGUID());
            break;
        case GOSSIP_OPTION_VENDOR:
            player->GetSession()->SendListInventory(m_creature->GetGUID());
            break;
        case 51: //chimera
            CreatePet(player, m_creature, 21879);
            break;
        case 52: //core hound
            CreatePet(player, m_creature, 21108);
            break;
        case 53: //Devilsaur
            CreatePet(player, m_creature, 20931);
            break;
        case 54: //rhino
            CreatePet(player, m_creature, 30445);
            break;
        case 55: //silithid
            CreatePet(player, m_creature, 5460);
            break;
        case 56: //Worm
            CreatePet(player, m_creature, 30148);
            break;
        case 57: //Loque'nahak
            CreatePet(player, m_creature, 32517);
            break;
        case 58: //Skoll
            CreatePet(player, m_creature, 35189);
            break;
        case 59: //Gondria
            CreatePet(player, m_creature, 33776);
            break;
        case 16: //Spider
            CreatePet(player, m_creature, 2349);
            break;
        case 17: //Dragonhawk
            CreatePet(player, m_creature, 27946);
            break;
        case 18: //Bat
            CreatePet(player, m_creature, 28233);
            break;
        case 19: //Ravager
            CreatePet(player, m_creature, 17199);
            break;
        case 20: //Raptor
            CreatePet(player, m_creature, 14821);
            break;
        case 21: //Serpent
            CreatePet(player, m_creature, 28358);
            break;
        case 1: //bear
            CreatePet(player, m_creature, 29319);
            break;
        case 2: //Boar
            CreatePet(player, m_creature, 29996);
            break;
        case 93: //Bug
            CreatePet(player, m_creature, 28085);
            break;
        case 4: //cat
            CreatePet(player, m_creature, 28097);
            break;
        case 5: //carrion
            CreatePet(player, m_creature, 26838);
            break;
        case 6: //crab
            CreatePet(player, m_creature, 24478);
            break;
        case 7: //crocolisk
            CreatePet(player, m_creature, 1417);
            break;
        case 8: //gorila
            CreatePet(player, m_creature, 28213);
            break;
        case 9: //hound
            CreatePet(player, m_creature, 29452);
            break;
        case 10: //hyena
            CreatePet(player, m_creature, 13036);
            break;
        case 11: //Moth
            CreatePet(player, m_creature, 27421);
            break;
        case 12: //owl
            CreatePet(player, m_creature, 23136);
            break;
        case 13: //strider
            CreatePet(player, m_creature, 22807);
            break;
        case 414: //scorpid
            CreatePet(player, m_creature, 9698);
            break;
        case 15: //turtle
            CreatePet(player, m_creature, 25482);
            break;
        }
        // CloseGossipMenuFor(player);
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

void AddSC_Npc_Beastmaster()
{
    new Npc_Beastmaster;
}
