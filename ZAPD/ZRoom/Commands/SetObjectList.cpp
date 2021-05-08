#include "SetObjectList.h"
#include "../../BitConverter.h"
#include "../../Globals.h"
#include "../../StringHelper.h"
#include "../../ZFile.h"
#include "../ZNames.h"
#include "../ZRoom.h"

SetObjectList::SetObjectList(ZRoom* nZRoom, std::vector<uint8_t> rawData, uint32_t rawDataIndex)
	: ZRoomCommand(nZRoom, rawData, rawDataIndex)
{
	objects = std::vector<uint16_t>();
	uint8_t objectCnt = rawData[rawDataIndex + 1];
	segmentOffset = GETSEGOFFSET(BitConverter::ToInt32BE(rawData, rawDataIndex + 4));
	uint32_t currentPtr = segmentOffset;

	for (uint8_t i = 0; i < objectCnt; i++)
	{
		uint16_t objectIndex = BitConverter::ToInt16BE(rawData, currentPtr);
		objects.push_back(objectIndex);
		currentPtr += 2;
	}

	if (segmentOffset != 0)
		zRoom->parent->AddDeclarationPlaceholder(segmentOffset);
}

std::string SetObjectList::GenerateExterns() const
{
	return StringHelper::Sprintf("s16 %sObjectList0x%06X[];\n", zRoom->GetName().c_str(),
	                             segmentOffset);
}

std::string SetObjectList::GenerateSourceCodePass1(std::string roomName, uint32_t baseAddress)
{
	std::string sourceOutput = "";

	sourceOutput +=
		StringHelper::Sprintf("%s 0x%02X, (u32)%sObjectList0x%06X",
	                          ZRoomCommand::GenerateSourceCodePass1(roomName, baseAddress).c_str(),
	                          objects.size(), zRoom->GetName().c_str(), segmentOffset);

	std::string declaration = "";

	for (size_t i = 0; i < objects.size(); i++)
	{
		uint16_t objectIndex = objects[i];
		declaration += StringHelper::Sprintf("    %s,", ZNames::GetObjectName(objectIndex).c_str());

		if (i < objects.size() - 1)
			declaration += "\n";
	}

	zRoom->parent->AddDeclarationArray(
		segmentOffset, DeclarationAlignment::None, objects.size() * 2, "s16",
		StringHelper::Sprintf("%sObjectList0x%06X", zRoom->GetName().c_str(), segmentOffset),
		objects.size(), declaration);

	return sourceOutput;
}

size_t SetObjectList::GetRawDataSize() const
{
	return ZRoomCommand::GetRawDataSize() + (objects.size() * 2);
}

std::string SetObjectList::GetCommandCName() const
{
	return "SCmdObjectList";
}

RoomCommand SetObjectList::GetRoomCommand() const
{
	return RoomCommand::SetObjectList;
}