#include "Globals.h"
#include <algorithm>
#include "File.h"
#include "Path.h"
#include "tinyxml2.h"

using namespace tinyxml2;

Globals* Globals::Instance;

Globals::Globals()
{
	Instance = this;

	game = ZGame::OOT_RETAIL;
	genSourceFile = true;
	testMode = false;
	profile = false;
	useLegacyZDList = false;
	useExternalResources = true;
	verbosity = VerbosityLevel::VERBOSITY_SILENT;
	outputPath = Directory::GetCurrentDirectory();
}

Globals::~Globals()
{
	for (auto& declPair : segmentRefFiles)
	{
		for (auto& file : declPair.second)
		{
			delete file;
		}
	}
}

void Globals::ReadConfigFile(const std::string& configFilePath)
{
	XMLDocument doc;
	XMLError eResult = doc.LoadFile(configFilePath.c_str());

	if (eResult != tinyxml2::XML_SUCCESS)
	{
		throw std::runtime_error("Error: Unable to read config file.");
		return;
	}

	XMLNode* root = doc.FirstChild();

	if (root == nullptr)
		return;

	for (XMLElement* child = root->FirstChildElement(); child != NULL;
	     child = child->NextSiblingElement())
	{
		if (std::string(child->Name()) == "SymbolMap")
		{
			std::string fileName = std::string(child->Attribute("File"));
			GenSymbolMap(Path::GetDirectoryName(configFilePath) + "/" + fileName);
		}
		else if (std::string(child->Name()) == "ActorList")
		{
			std::string fileName = std::string(child->Attribute("File"));
			std::vector<std::string> lines =
				File::ReadAllLines(Path::GetDirectoryName(configFilePath) + "/" + fileName);

			for (std::string line : lines)
				cfg.actorList.push_back(StringHelper::Strip(line, "\r"));
		}
		else if (std::string(child->Name()) == "ObjectList")
		{
			std::string fileName = std::string(child->Attribute("File"));
			std::vector<std::string> lines =
				File::ReadAllLines(Path::GetDirectoryName(configFilePath) + "/" + fileName);

			for (std::string line : lines)
				cfg.objectList.push_back(StringHelper::Strip(line, "\r"));
		}
		else if (std::string(child->Name()) == "TexturePool")
		{
			std::string fileName = std::string(child->Attribute("File"));
			ReadTexturePool(Path::GetDirectoryName(configFilePath) + "/" + fileName);
		}
		else if (std::string(child->Name()) == "BGConfig")
		{
			cfg.bgScreenWidth = child->IntAttribute("ScreenWidth", 320);
			cfg.bgScreenHeight = child->IntAttribute("ScreenHeight", 240);
		}
		else if (std::string(child->Name()) == "ExternalXMLFolder")
		{
			if (cfg.externalXmlFolder == "")
				cfg.externalXmlFolder = child->Attribute("Path");
		}
		else if (std::string(child->Name()) == "ExternalFile")
		{
			const char* xmlPathValue = child->Attribute("XmlPath");
			if (xmlPathValue == nullptr)
			{
				throw std::runtime_error(StringHelper::Sprintf(
					"Parse: Fatal error in configuration file.\n"
					"\t Missing 'XmlPath' attribute in `ExternalFile` element.\n"));
			}
			const char* outPathValue = child->Attribute("OutPath");
			if (outPathValue == nullptr)
			{
				throw std::runtime_error(StringHelper::Sprintf(
					"Parse: Fatal error in configuration file.\n"
					"\t Missing 'OutPath' attribute in `ExternalFile` element.\n"));
			}

			cfg.externalFiles.push_back(
				ExternalFile(fs::path(xmlPathValue), fs::path(outPathValue)));
		}
	}
}

void Globals::ReadTexturePool(const std::string& texturePoolXmlPath)
{
	XMLDocument doc;
	XMLError eResult = doc.LoadFile(texturePoolXmlPath.c_str());

	if (eResult != tinyxml2::XML_SUCCESS)
	{
		fprintf(stderr, "Warning: Unable to read texture pool XML with error code %i\n", eResult);
		return;
	}

	XMLNode* root = doc.FirstChild();

	if (root == nullptr)
		return;

	for (XMLElement* child = root->FirstChildElement(); child != NULL;
	     child = child->NextSiblingElement())
	{
		if (std::string(child->Name()) == "Texture")
		{
			std::string crcStr = std::string(child->Attribute("CRC"));
			fs::path texPath = std::string(child->Attribute("Path"));
			std::string texName = "";

			uint32_t crc = strtoul(crcStr.c_str(), NULL, 16);

			cfg.texturePool[crc].path = texPath;
		}
	}
}

void Globals::GenSymbolMap(const std::string& symbolMapPath)
{
	auto symbolLines = File::ReadAllLines(symbolMapPath);

	for (std::string symbolLine : symbolLines)
	{
		auto split = StringHelper::Split(symbolLine, " ");
		uint32_t addr = strtoul(split[0].c_str(), NULL, 16);
		std::string symbolName = split[1];

		symbolMap[addr] = symbolName;
	}
}

void Globals::AddSegment(int32_t segment, ZFile* file)
{
	if (std::find(segments.begin(), segments.end(), segment) == segments.end())
		segments.push_back(segment);
	if (segmentRefFiles.find(segment) == segmentRefFiles.end())
		segmentRefFiles[segment] = std::vector<ZFile*>();

	segmentRefFiles[segment].push_back(file);
}

bool Globals::HasSegment(int32_t segment)
{
	return std::find(segments.begin(), segments.end(), segment) != segments.end();
}

bool Globals::GetSegmentedPtrName(segptr_t segAddress, ZFile* currentFile,
                                  const std::string& expectedType, std::string& declName)
{
	if (segAddress == 0)
	{
		declName = "NULL";
		return true;
	}

	uint8_t segment = GETSEGNUM(segAddress);
	uint32_t offset = Seg2Filespace(segAddress, currentFile->baseAddress);
	ZSymbol* sym;

	sym = currentFile->GetSymbolResource(offset);
	if (sym != nullptr)
	{
		if (expectedType == "" || expectedType == sym->GetSourceTypeName())
		{
			declName = sym->GetName();
			return true;
		}
	}
	sym = currentFile->GetSymbolResource(segAddress);
	if (sym != nullptr)
	{
		if (expectedType == "" || expectedType == sym->GetSourceTypeName())
		{
			declName = sym->GetName();
			return true;
		}
	}

	if (currentFile->IsSegmentedInFilespaceRange(segAddress))
	{
		if (currentFile->GetDeclarationPtrName(segAddress, expectedType, declName))
			return true;
	}
	else if (HasSegment(segment))
	{
		for (auto file : segmentRefFiles[segment])
		{
			offset = Seg2Filespace(segAddress, file->baseAddress);

			sym = file->GetSymbolResource(offset);
			if (sym != nullptr)
			{
				if (expectedType == "" || expectedType == sym->GetSourceTypeName())
				{
					declName = sym->GetName();
					return true;
				}
			}
			sym = file->GetSymbolResource(segAddress);
			if (sym != nullptr)
			{
				if (expectedType == "" || expectedType == sym->GetSourceTypeName())
				{
					declName = sym->GetName();
					return true;
				}
			}

			if (file->IsSegmentedInFilespaceRange(segAddress))
			{
				if (file->GetDeclarationPtrName(segAddress, expectedType, declName))
					return true;
			}
		}
	}

	const auto& symbolFromMap = Globals::Instance->symbolMap.find(segAddress);
	if (symbolFromMap != Globals::Instance->symbolMap.end())
	{
		declName = "&" + symbolFromMap->second;
		return true;
	}

	declName = StringHelper::Sprintf("0x%08X", segAddress);
	return false;
}

bool Globals::GetSegmentedArrayIndexedName(segptr_t segAddress, size_t elementSize,
                                           ZFile* currentFile, const std::string& expectedType,
                                           std::string& declName)
{
	if (segAddress == 0)
	{
		declName = "NULL";
		return true;
	}

	uint8_t segment = GETSEGNUM(segAddress);

	if (currentFile->IsSegmentedInFilespaceRange(segAddress))
	{
		bool addressFound = currentFile->GetDeclarationArrayIndexedName(segAddress, elementSize,
		                                                                expectedType, declName);
		if (addressFound)
			return true;
	}
	else if (HasSegment(segment))
	{
		for (auto file : segmentRefFiles[segment])
		{
			if (file->IsSegmentedInFilespaceRange(segAddress))
			{
				bool addressFound = file->GetDeclarationArrayIndexedName(segAddress, elementSize,
				                                                         expectedType, declName);
				if (addressFound)
					return true;
			}
		}
	}

	declName = StringHelper::Sprintf("0x%08X", segAddress);
	return false;
}

ExternalFile::ExternalFile(fs::path nXmlPath, fs::path nOutPath)
	: xmlPath{nXmlPath}, outPath{nOutPath}
{
}
