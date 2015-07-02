#include "ScriptMgr.h"
#include "Player.h"
#include "smallfolk_cpp/smallfolk.h"

class ExampleWindowScript : public AIOScript
{
public:
	ExampleWindowScript()
		: AIOScript("AIOExample"), counter(0)
	{
		AddAddon("ExampleWindow", "ExampleWindow.lua");
		AddHandler("Print", std::bind(&ExampleWindowScript::HandlePrint, this, std::placeholders::_1, std::placeholders::_2));
		AddInitArgs("AIOExample", "Init", std::bind(&ExampleWindowScript::InitArg, this, std::placeholders::_1), std::bind(&ExampleWindowScript::InitArg, this, std::placeholders::_1));
		AddInitArgs("AIOExample", "Init", std::bind(&ExampleWindowScript::InitArg2, this, std::placeholders::_1));
		AddInitArgs("AIOExample", "InitB");
	}

private:
	void HandlePrint(Player *sender, const LuaVal &args)
	{
		LuaVal &btn = args.get(4);
		LuaVal &inp = args.get(5);
		LuaVal &val = args.get(6);
		if(!btn.isstring() || !inp.isstring() || !val.isnumber())
		{
			return;
		}

		sender->GetSession()->SendNotification("HandlePrint -> Button Name: %s, Input: %s, Slider Value: %f", btn.str().c_str(), inp.str().c_str(), val.num());
	}

	LuaVal InitArg(Player *sender)
	{
		if(counter % 2 == 0)
		{
			return "Inited 1";
		}
		else
		{
			return "Inited 2";
		}
	}
	LuaVal InitArg2(Player *sender)
	{
		if(counter % 2 != 0)
		{
			return "Inited 1";
		}
		else
		{
			return "Inited 2";
		}
	}

	size_t counter;
};

void AddSC_ExampleWindow()
{
	new ExampleWindowScript();
}