#ifndef AIO_MESSAGE_H
#define AIO_MESSAGE_H

#include "smallfolk_cpp/smallfolk.h"

class Player;

class AIOMsg
{
	public:
		AIOMsg();

		AIOMsg &Add(const std::string &scriptName, const std::string &handlerName,
			const LuaVal &a1 = LuaVal::nil(), const LuaVal &a2 = LuaVal::nil(), const LuaVal &a3 = LuaVal::nil(),
			const LuaVal &a4 = LuaVal::nil(), const LuaVal &a5 = LuaVal::nil(), const LuaVal &a6 = LuaVal::nil());
		AIOMsg &AppendLast(const LuaVal &a1 = LuaVal::nil(), const LuaVal &a2 = LuaVal::nil(), const LuaVal &a3 = LuaVal::nil(),
			const LuaVal &a4 = LuaVal::nil(), const LuaVal &a5 = LuaVal::nil(), const LuaVal &a6 = LuaVal::nil());

	private:
		LuaVal _val;
		friend class Player;
};

#endif
