/* ScriptData
Name: aio_commandscript
%Complete : 100
Comment : All AIO related server side commands
Category : commandscripts
EndScriptData */

#include "Player.h"
#include "ScriptMgr.h"
#include "World.h"

class caio_commandscript : public CommandScript
{
	public:
		caio_commandscript() : CommandScript("caio_commandscript") { }

		ChatCommand* GetCommands() const
		{
			static ChatCommand caioCommandTable[] =
			{
				{ "version", rbac::RBAC_PERM_COMMAND_CAIO, true, &HandleVersionCommand, "", NULL },
				{ "send", rbac::RBAC_PERM_COMMAND_CAIO, true, &HandleSendCommand, "", NULL },
				{ "forcereload", rbac::RBAC_PERM_COMMAND_CAIO, true, &HandleReloadCommand, "", NULL },
				{ "forcereset", rbac::RBAC_PERM_COMMAND_CAIO, true, &HandleResetCommand, "", NULL },
				{ "sendall", rbac::RBAC_PERM_COMMAND_CAIO, true, &HandleSendAllCommand, "", NULL },
				{ "forcereloadall", rbac::RBAC_PERM_COMMAND_CAIO, true, &HandleReloadAllCommand, "", NULL },
				{ "forceresetall", rbac::RBAC_PERM_COMMAND_CAIO, true, &HandleResetAllCommand, "", NULL },
				{ "reloadaddons", rbac::RBAC_PERM_COMMAND_CAIO, true, &HandleReloadAddonsCommand, "", NULL },
				{ NULL, 0, false, NULL, "", NULL }
			};
			static ChatCommand commandTable[] =
			{
				{ "caio", SEC_ADMINISTRATOR, true, 0, "", caioCommandTable },
				{ NULL, 0, false, NULL, "", NULL }
			};
			return commandTable;
		}
		
		static bool HandleVersionCommand(ChatHandler* handler, char const* args)
		{
			handler->SendSysMessage("AIO version " AIO_VERSION_STRING ".");
			return true;
		}

		static bool HandleSendCommand(ChatHandler* handler, char const* args)
		{
			// format: name "message text"
			Player *target = 0;
			if(!handler->extractPlayerTarget((char*)args, &target, 0, 0))
				return false;

			char* tailStr = strtok(NULL, " ");
			if(!tailStr)
				return false;

			char const* msg = handler->extractQuotedArg(tailStr);
			if(!msg)
				return false;

			target->SendSimpleAIOMessage(msg);
			handler->SendSysMessage("Message sent.");
			return true;
		};

		static bool HandleReloadCommand(ChatHandler* handler, char const* args)
		{
			Player *target = 0;
			if(!handler->extractPlayerTarget((char*)args, &target, 0, 0))
				return false;

			target->ForceReloadAddons();
			handler->SendSysMessage("Reload message sent.");
			return true;
		};

		static bool HandleResetCommand(ChatHandler* handler, char const* args)
		{
			Player *target = 0;
			if(!handler->extractPlayerTarget((char*)args, &target, 0, 0))
				return false;

			target->ForceResetAddons();
			handler->SendSysMessage("Reset message sent.");
			return true;
		};

		static bool HandleSendAllCommand(ChatHandler* handler, char const* args)
		{
			char* tailStr = strtok((char*)args, " ");
			if(!tailStr)
				return false;

			char const* msg = handler->extractQuotedArg(tailStr);
			if(!msg)
				return false;

			sWorld->SendAllSimpleAIOMessage(msg);
			handler->SendSysMessage("Message sent.");
			return true;
		};

		static bool HandleReloadAllCommand(ChatHandler* handler, char const* args)
		{
			Player *target = 0;
			if(!handler->extractPlayerTarget((char*)args, &target, 0, 0))
				return false;

			sWorld->ForceReloadPlayerAddons();
			handler->SendSysMessage("Reload all message sent.");
			return true;
		};

		static bool HandleResetAllCommand(ChatHandler* handler, char const* args)
		{
			Player *target = 0;
			if(!handler->extractPlayerTarget((char*)args, &target, 0, 0))
				return false;

			sWorld->ForceResetPlayerAddons();
			handler->SendSysMessage("Reset all message sent.");
			return true;
		};

		static bool HandleReloadAddonsCommand(ChatHandler* handler, char const* args)
		{
			sWorld->ReloadAddons();
			sWorld->ForceReloadPlayerAddons();
			return true;
		}
};

void AddSC_caio_commandscript()
{
	new caio_commandscript();
}