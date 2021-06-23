#include "Unused09.h"
#include "StringHelper.h"

Unused09::Unused09(ZFile* nParent) : ZRoomCommand(nParent)
{
}

std::string Unused09::GetBodySourceCode() const
{
	return "SCENE_CMD_UNK_09()";
}

std::string Unused09::GetCommandCName() const
{
	return "SCmdBase";
}

RoomCommand Unused09::GetRoomCommand() const
{
	return RoomCommand::Unused09;
}
