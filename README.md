## Introduction

CAIO is a server-client communication system for WoW AddOns. It is an extension of [AIO](https://github.com/Rochet2/AIO) to support C++ server side handling.
CAIO is designed for sending lua addons and data to player from server and data from player to server.
CAIO is currently only implemented on TrinityCore 3.3.5 branch.


## Install

+ Clone this repository or merge with your TrinityCore 3.3.5 branch
+ Copy `AIO_Client` folder from [AIO](https://github.com/Rochet2/AIO) repository to `WoW_Installation_Dir/Interface/AddOns`
+ [Install AIO scripts](#api-reference)
+ Build/Install TrinityCore
+ Copy your client side addons to `TrinityCore_Installation_Dir/lua_client_scripts`


## API reference

### Creating an AIO script

```
class ExampleAIOScript : public AIOScript
{
public:
	ExampleAIOScript()
		: AIOScript("ExampleScriptName")
	{
		// Loads addon files to addons list and sends them on AIO client initialization
		// Looks for the file in path config AIO.ClientScriptPath
		AddAddon("ExampleAddon", "example_addon.lua");
		AddAddon("AnotherAddon", "extras/another_addon.lua");

		// Handler function signature: void HandlerFunction(Player *sender, const LuaVal &args)
		AddHandler("Print", std::bind(&ExampleAIOScript::HandlePrint, this, std::placeholders::_1, std::placeholders::_2));
		AddHandler("Save", std::bind(&ExampleAIOScript::HandleSave, this, std::placeholders::_1, std::placeholders::_2));

		// Initialization handler and arguments
		AddInitArgs("ExampleScriptName", "Init", std::bind(&ExampleAIOScript::InitArg, this, std::placeholders::_1), std::bind(&ExampleAIOScript::InitArg, this, std::placeholders::_1));
		//Adds additional argument to send to handler
		AddInitArgs("ExampleScriptName", "Init", std::bind(&ExampleAIOScript::InitArg2, this, std::placeholders::_1));
		AddInitArgs("AnotherScript", "InitB"); //Arguments are not required
	}

	void HandlePrint(Player *sender, const LuaVal &args)
	{
	    //LuaVal args in a handler function is always a table
		//Handler arguments index starts from 4
		LuaVal &InputVal = args.get(4);
		LuaVal &SliderVal = args.get(5);

		//MUST check if the value type is valid or else smallfolk_cpp will
		//throw on obtaining that type
		if(!InputVal.isstring() || !SliderVal.isnumber())
		{
			return;
		}

		sender->GetSession()->SendNotification("HandlePrint -> Stored String: %s, Input: %s, Slider Value: %f",
			storedString.c_str(), InputVal.str().c_str(), SliderVal.num());
	}

	void HandleSave(Player *sender, const LuaVal &args)
	{
	    //LuaVal args in a handler function is always a table
		//Handler arguments index starts from 4
		LuaVal &SaveVal = args.get(4);

		//MUST check if the value type is valid or else smallfolk_cpp will
		//throw on obtaining that type
		if(!SaveVal.isstring())
		{
			return;
		}

		storedString = SaveVal.str();
		sender->GetSession()->SendNotification("Saved");
	}

	LuaVal InitArg(Player *sender)
	{
		LuaVal arg = LuaVal(TTABLE);
		arg.set("key", 12.3);
		arg.set("key2", false);

		return arg;
	}

	LuaVal InitArg2(Player *sender)
	{
		return "LuaVal will implicitly create a string LuaVal for this arg";
	}

private:
	std::string storedString;
};
```

### smallfolk_cpp LuaVal reference

https://github.com/Rochet2/smallfolk_cpp

### AIO functions

```
class AIOScript
{
protected:
    // Registers an AIO Handler script of scriptName
	AIOScript(const char *scriptName);

	// Registers a handler function to call when handling
	// handlerName of this script.
	void AddHandler(const char *handlerName, HandlerFunc function); //!!!!!!!!!LuaVal of FuncType is always table value

	// Adds a client side handler to call and adds arguments
	// to sends with it for AIO client addon initialization.
		
	// You can add additional arguments to the handler by
	// calling this function again
	void AddInitArgs(const std::string &scriptName, const std::string &handlerName,
		ArgFunc a1 = ArgFunc(), ArgFunc a2 = ArgFunc(), ArgFunc a3 = ArgFunc(),
		ArgFunc a4 = ArgFunc(), ArgFunc a5 = ArgFunc(), ArgFunc a6 = ArgFunc());

	// Adds a WoW addon file to the list of addons with a unique
	// addon name to send on AIO client addon initialization.
	//
	// It is required to call World::ForceReloadPlayerAddons()
	// if addons are added after server is fully initialized
	// for online players to load the added addons.
	void AddAddon(const std::string &addonName, const std::string &fileName);

	// Adds WoW addon code to the list of addons with a unique
	// addon name to send on AIO client addon initialization.
	//
	// It is required to call World::ForceReloadPlayerAddons()
	// if addons are added after server is fully initialized
	// for online players to load the added addons.
	void AddAddonCode(const std::string &addonName, const std::string &code);
}
```

AIOMsg.h
```
class AIOMsg
{
public:
	//Creates an empty AIOMsg
	AIOMsg();

	//Creates a AIO message and adds one block
	AIOMsg(const std::string &scriptName, const std::string &handlerName,
		const LuaVal &a1 = LuaVal::nil(), const LuaVal &a2 = LuaVal::nil(), const LuaVal &a3 = LuaVal::nil(),
		const LuaVal &a4 = LuaVal::nil(), const LuaVal &a5 = LuaVal::nil(), const LuaVal &a6 = LuaVal::nil())
	{
		Add(scriptName, handlerName, a1, a2, a3, a4, a5);
	}

	//Adds another block
	//Another block will call another handler in one message
	AIOMsg &Add(const std::string &scriptName, const std::string &handlerName,
		const LuaVal &a1 = LuaVal::nil(), const LuaVal &a2 = LuaVal::nil(), const LuaVal &a3 = LuaVal::nil(),
		const LuaVal &a4 = LuaVal::nil(), const LuaVal &a5 = LuaVal::nil(), const LuaVal &a6 = LuaVal::nil());

	//Appends the last block
	//You can add additional arguments to the last block
	AIOMsg &AppendLast(const LuaVal &a1 = LuaVal::nil(), const LuaVal &a2 = LuaVal::nil(), const LuaVal &a3 = LuaVal::nil(),
		const LuaVal &a4 = LuaVal::nil(), const LuaVal &a5 = LuaVal::nil(), const LuaVal &a6 = LuaVal::nil());
};
```

Player.h

```
//Returns whether AIO client has been initialized
bool AIOInitialized() const { return m_aioinitialized; }

// Sends an AIO message to the player
// See: class AIOMsg
void Player::AIOMessage(AIOMsg &msg);

// Triggers an AIO script handler on the client
// To trigger multiple handlers in one message or add more arguments
// use Player::AIOMessage
void Player::AIOHandle(const std::string &scriptName, const std::string &handlerName,
	const LuaVal &a1 = LuaVal::nil(), const LuaVal &a2 = LuaVal::nil(), const LuaVal &a3 = LuaVal::nil(),
	const LuaVal &a4 = LuaVal::nil(), const LuaVal &a5 = LuaVal::nil(), const LuaVal &a6 = LuaVal::nil());


// AIO can only understand smallfolk LuaVal::dumps() format
// Handler functions are called by creating a table as below
// {
//     {n, ScriptName, HandlerName(optional), Arg1..N(optional) },
//     {n, AnotherScriptName, AnotherHandlerName(optional), Arg1..N(optional) }
// }
// Where n is number of arguments including handler name as a argument
void Player::SendSimpleAIOMessage(const std::string &message);

// Forces a reload on the player addons
// Syncs player addons with addons in addon list
void Player::ForceReloadAddons();

// Forces a reset on the player addons
// Player addons and addon data is deleted and downloaded again
void Player::ForceResetAddons();
```

World.h

```
// AIO prefix configured in worldserver.conf
std::string World::GetAIOPrefix() const { return m_aioprefix; }

// AIO client LUA files path configured in worldserver.conf
std::string World::GetAIOClientScriptPath() const { return m_aioclientpath; }

// Forces a reload on all player addons
// Syncs player addons with addons in addon list
void World::ForceReloadPlayerAddons();

// Forces a reset on all player addons
// Player addons and addon data is deleted and downloaded again
void World::ForceResetPlayerAddons();

// Sends an AIO message to all players
// See: class AIOMsg
void World::AIOMessageAll(AIOMsg &msg);

// Sends a simple string message to all players

// AIO can only understand smallfolk LuaVal::dumps() format
// Handler functions are called by creating a table as below
// {
//     {n, ScriptName, HandlerName(optional), Arg1..N(optional) },
//     {n, AnotherScriptName, AnotherHandlerName(optional), Arg1..N(optional) }
// }
// Where n is number of arguments including handler name as a argument
void World::SendAllSimpleAIOMessage(const std::string &message);

// Reloads client side addon files in the list of addons
// and force reloads all player addons
void World::ReloadAddons();

// Adds a WoW addon file to the list of addons with a unique
// addon name to send on AIO client addon initialization.
//
// It is required to call World::ForceReloadPlayerAddons()
// if addons are added after server is fully initialized
// for online players to load the added addons.// Adds addon to the list of 
void World::AddAddon(const std::string &name, const std::string &fileName, bool formatPath);

// Adds WoW addon code to the list of addons with a unique
// addon name to send on AIO client addon initialization.
//
// It is required to call World::ForceReloadPlayerAddons()
// if addons are added after server is fully initialized
// for online players to load the added addons.
void AddAddonCode(const std::string &name, const std::string &code, const std::string &file = "");
```

## Reporting issues and submitting fixes

Issues can be reported via the [Github issue tracker](https://github.com/SaiFi0102/CAIO-For-Trinity/issues). Fixes can be submitted as pull requests via Github.


## Copyright

TODO


## Authors, Contributors &amp; Thanks

+ Saif
  + CAIO
+ Rochet2
    + [AIO](https://github.com/Rochet2/AIO) 
    + [smallfolk_cpp](https://github.com/Rochet2/smallfolk_cpp) to handle and transmit Lua data in C++
