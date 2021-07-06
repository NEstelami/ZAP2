#pragma once

#include <map>
#include <string>
#include <vector>
#include "ZFile.h"
#include "ZRoom/ZRoom.h"
#include "ZTexture.h"

enum class VerbosityLevel
{
	VERBOSITY_SILENT,
	VERBOSITY_INFO,
	VERBOSITY_DEBUG
};

struct TexturePoolEntry
{
	fs::path path = "";  // Path to Shared Texture
};

class ExternalFile
{
public:
	fs::path xmlPath, outPath;

	ExternalFile(fs::path nXmlPath, fs::path nOutPath);
};

class GameConfig
{
public:
	std::map<int32_t, ZFile*> segmentRefFiles;
	std::map<uint32_t, std::string> symbolMap;
	std::vector<std::string> actorList;
	std::vector<std::string> objectList;
	std::map<uint32_t, TexturePoolEntry> texturePool;  // Key = CRC

	// ZBackground
	uint32_t bgScreenWidth = 320, bgScreenHeight = 240;

	// ExternalFile
	fs::path externalXmlFolder;
	std::vector<ExternalFile> externalFiles;

	GameConfig() = default;
};

class Globals
{
public:
	static Globals* Instance;

	bool genSourceFile;  // Used for extraction
	bool useExternalResources;
	bool testMode;  // Enables certain experimental features
	bool outputCrc = false;
	bool profile;  // Measure performance of certain operations
	bool useLegacyZDList;
	VerbosityLevel verbosity;  // ZAPD outputs additional information
	ZFileMode fileMode;
	fs::path baseRomPath, inputPath, outputPath, sourceOutputPath, cfgPath;
	TextureType texType;
	ZGame game;
	GameConfig cfg;
	bool warnUnaccounted = false;
	bool verboseUnaccounted = false;

	std::vector<ZFile*> files;
	std::vector<ZFile*> externalFiles;
	std::vector<int32_t> segments;
	std::map<int32_t, std::vector<ZFile*>> segmentRefFiles;
	std::map<uint32_t, std::string> symbolMap;

	Globals();
	~Globals();

	void ReadConfigFile(const std::string& configFilePath);
	void ReadTexturePool(const std::string& texturePoolXmlPath);
	void GenSymbolMap(const std::string& symbolMapPath);
	void AddSegment(int32_t segment, ZFile* file);
	bool HasSegment(int32_t segment);

	/**
	 * Search in every file (and the symbol map) for the `segAddress` passed as parameter.
	 * If the segment of `currentFile` is the same segment of `segAddress`, then that file will be
	 * used only, otherwise, the search will be performed in every other file.
	 * The name of that variable will be stored in the `declName` parameter.
	 * Returns `true` if the address is found. `false` otherwise,
	 * in which case `declName` will be set to the address formatted as a pointer.
	 */
	bool GetSegmentedPtrName(segptr_t segAddress, ZFile* currentFile,
	                         const std::string& expectedType, std::string& declName);

	bool GetSegmentedArrayIndexedName(segptr_t segAddress, size_t elementSize, ZFile* currentFile,
	                                  const std::string& expectedType, std::string& declName);
};

/*
 * Note: In being able to track references across files, there are a few major files that make use
 * of segments...
 * Segment 1: nintendo_rogo_static/title_static
 * Segment 2: parameter_static
 * Segment 4: gameplay_keep
 * Segment 5: gameplay_field_keep, gameplay_dangeon_keep
 * Segment 7: link_animetion
 * Segment 8: icon_item_static
 * Segment 9: icon_item_24_static
 * Segment 12: icon_item_field_static, icon_item_dungeon_static
 * Segment 13: icon_item_nes_static
 *
 * I'm thinking a config file could be usable, but I'll have to experiment...
 */
