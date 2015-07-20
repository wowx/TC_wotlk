#ifndef AIO_MESSAGE_H
#define AIO_MESSAGE_H

#include "smallfolk_cpp/smallfolk.h"

class Player;

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

private:
    LuaVal _val;
    friend class Player;
};

#endif
