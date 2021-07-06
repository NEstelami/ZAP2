#include "SetAlternateHeaders.h"

#include "BitConverter.h"
#include "Globals.h"
#include "StringHelper.h"
#include "ZFile.h"

SetAlternateHeaders::SetAlternateHeaders(ZFile* nParent) : ZRoomCommand(nParent)
{
}

void SetAlternateHeaders::DeclareReferences(const std::string& prefix)
{
	if (cmdArg2 != 0)
	{
		std::string varName =
			StringHelper::Sprintf("%sAlternateHeaders0x%06X", prefix.c_str(), segmentOffset);
		parent->AddDeclarationPlaceholder(segmentOffset, varName);
	}
}

void SetAlternateHeaders::ParseRawDataLate()
{
	int numHeaders = zRoom->GetDeclarationSizeFromNeighbor(segmentOffset) / 4;

	for (int32_t i = 0; i < numHeaders; i++)
	{
		int32_t address = BitConverter::ToInt32BE(parent->GetRawData(), segmentOffset + (i * 4));
		headers.push_back(address);

		if (address != 0)
			zRoom->commandSets.push_back(CommandSet(address));
	}
}

void SetAlternateHeaders::DeclareReferencesLate(const std::string& prefix)
{
	if (!headers.empty())
	{
		std::string declaration = "";

		for (size_t i = 0; i < headers.size(); i++)
		{
			if (headers.at(i) == 0)
				declaration += StringHelper::Sprintf("\tNULL,");
			else
				declaration +=
					StringHelper::Sprintf("\t%sSet%04X,", prefix.c_str(), GETSEGOFFSET(headers[i]));

			if (i + 1 < headers.size())
				declaration += "\n";
		}

		std::string varName =
			StringHelper::Sprintf("%sAlternateHeaders0x%06X", prefix.c_str(), segmentOffset);
		parent->AddDeclarationArray(segmentOffset, GetDeclarationAlignment(), headers.size() * 4,
		                            "SCmdBase*", varName, 0, declaration);
	}
}

std::string SetAlternateHeaders::GetBodySourceCode() const
{
	std::string listName;
	Globals::Instance->GetSegmentedPtrName(cmdArg2, parent, "SCmdBase*", listName);
	return StringHelper::Sprintf("SCENE_CMD_ALTERNATE_HEADER_LIST(%s)", listName.c_str());
}

std::string SetAlternateHeaders::GetCommandCName() const
{
	return "SCmdAltHeaders";
}

RoomCommand SetAlternateHeaders::GetRoomCommand() const
{
	return RoomCommand::SetAlternateHeaders;
}
