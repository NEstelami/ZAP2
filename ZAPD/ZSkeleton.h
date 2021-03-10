#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include "ZFile.h"
#include "ZDisplayList.h"

enum class ZLimbType
{
	Standard,
	LOD,
	Skin
};

// TODO: check if more types exists
enum class ZLimbSkinType
{
	SkinType_0, // Segment = 0
	SkinType_4 = 4, // Segment = segmented address // Struct_800A5E28
	SkinType_5 = 5, // Segment = 0
	SkinType_DList = 11,
};


class Struct_800A57C0
{
protected:
	uint32_t offset;

	uint16_t unk_0;
	int16_t unk_2;
	int16_t unk_4;
	int8_t unk_6;
	int8_t unk_7;
	int8_t unk_8;
	uint8_t unk_9;

public:
	Struct_800A57C0(const std::vector<uint8_t>& rawData, uint32_t fileOffset);
	Struct_800A57C0(const std::vector<uint8_t>& rawData, uint32_t fileOffset, size_t index);

	[[nodiscard]]
	std::string GetSourceOutputCode() const;

	static size_t GetRawDataSize();
	static std::string GetSourceTypeName();
};


class Struct_800A598C_2
{
protected:
	uint32_t offset;

	uint8_t unk_0;
	int16_t x;
	int16_t y;
	int16_t z;
	uint8_t unk_8;

public:
	Struct_800A598C_2(const std::vector<uint8_t>& rawData, uint32_t fileOffset);
	Struct_800A598C_2(const std::vector<uint8_t>& rawData, uint32_t fileOffset, size_t index);

	[[nodiscard]]
	std::string GetSourceOutputCode() const;

	static size_t GetRawDataSize();
	static std::string GetSourceTypeName();
};


class Struct_800A598C
{
protected:
	uint32_t offset;
	ZFile* parent;

	uint16_t unk_0; // Length of unk_8
	uint16_t unk_2; // Length of unk_C
	uint16_t unk_4; // 0 or 1
	segptr_t unk_8; // Struct_800A57C0*
	segptr_t unk_C; // Struct_800A598C_2*

	std::vector<Struct_800A57C0> unk_8_arr;
	std::vector<Struct_800A598C_2> unk_C_arr;

public:
	Struct_800A598C(ZFile* parent, const std::vector<uint8_t>& rawData, uint32_t fileOffset);
	Struct_800A598C(ZFile* parent, const std::vector<uint8_t>& rawData, uint32_t fileOffset, size_t index);

	[[nodiscard]]
	std::string GetSourceOutputCode(const std::string& prefix) const;

	static size_t GetRawDataSize();
	static std::string GetSourceTypeName();
};


class Struct_800A5E28
{
protected:
	uint32_t offset;
	ZFile* parent;

	uint16_t unk_0;
	uint16_t unk_2; // Length of unk_4
	segptr_t unk_4; // Struct_800A598C*
	segptr_t unk_8; // Gfx*

	std::vector<Struct_800A598C> unk_4_arr;

public:
	Struct_800A5E28() = default;
	Struct_800A5E28(ZFile* parent, const std::vector<uint8_t>& rawData, uint32_t fileOffset);
	Struct_800A5E28(ZFile* parent, const std::vector<uint8_t>& rawData, uint32_t fileOffset, size_t index);

	[[nodiscard]]
	std::string GetSourceOutputCode(const std::string& prefix) const;

	static size_t GetRawDataSize();
	static std::string GetSourceTypeName();
};


class ZLimb : public ZResource
{
protected:
	segptr_t segAddress;
	ZLimbType type = ZLimbType::Standard;

	int16_t transX, transY, transZ;
	uint8_t childIndex, siblingIndex;
	segptr_t dListPtr = 0;

	std::vector<ZLimb*> children;
	std::vector<ZDisplayList> dLists;

	segptr_t farDListPtr = 0; // LOD only

	ZLimbSkinType skinSegmentType = ZLimbSkinType::SkinType_0; // Skin only
	segptr_t skinSegment = 0; // Skin only
	Struct_800A5E28 segmentStruct; // Skin only


	std::string GetLimbDListSourceOutputCode(const std::string& prefix, const std::string& limbPrefix, segptr_t dListPtr);

	std::string GetSourceOutputCodeSkin(const std::string& prefix);
	std::string GetSourceOutputCodeSkin_Type_4(const std::string& prefix);

	std::string GetSourceOutputCodeSkin_Type_4_StructA5E28_Entry(const std::string& prefix, uint32_t fileOffset, uint16_t index);

public:
	ZLimb(tinyxml2::XMLElement* reader, const std::vector<uint8_t>& nRawData, int nRawDataIndex, ZFile* nParent);
	ZLimb(ZLimbType limbType, const std::string& prefix, const std::vector<uint8_t>& nRawData, int nRawDataIndex, ZFile* nParent);
	virtual ~ZLimb();

	void ParseXML(tinyxml2::XMLElement* reader) override;
	void ParseRawData() override;
	static ZLimb* FromXML(tinyxml2::XMLElement* reader, std::vector<uint8_t> nRawData, int rawDataIndex, std::string nRelPath, ZFile* parent);
	int GetRawDataSize() override;
	std::string GetSourceOutputCode(const std::string& prefix) override;
	std::string GetSourceTypeName() override;
	ZResourceType GetResourceType() override;

	ZLimbType GetLimbType();
	static const char* GetSourceTypeName(ZLimbType limbType);

	uint32_t GetFileAddress();
};


enum ZSkeletonType
{
	Normal,
	Flex
};

class ZSkeleton : public ZResource
{
public:
	ZSkeletonType type = ZSkeletonType::Normal;
	ZLimbType limbType = ZLimbType::Standard;
	std::vector<ZLimb*> limbs;
	segptr_t limbsArrayAddress;
	uint8_t limbCount;
	uint8_t dListCount; // FLEX SKELETON ONLY

	ZSkeleton(tinyxml2::XMLElement* reader, const std::vector<uint8_t>& nRawData, int nRawDataIndex, ZFile* nParent);
	//ZSkeleton(ZSkeletonType nSkelType, ZLimbType nLimbType, const std::string& prefix, const std::vector<uint8_t>& nRawData, int nRawDataIndex, ZFile* nParent);
	~ZSkeleton();
	void ParseXML(tinyxml2::XMLElement* reader) override;
	void ParseRawData() override;
	static ZSkeleton* FromXML(tinyxml2::XMLElement* reader, std::vector<uint8_t> nRawData, int rawDataIndex, std::string nRelPath, ZFile* nParent);
	void Save(const std::string& outFolder) override;
	void GenerateHLIntermediette(HLFileIntermediette& hlFile) override;

	int GetRawDataSize() override;
	std::string GetSourceOutputCode(const std::string& prefix) override;

	std::string GetSourceTypeName() override;
	ZResourceType GetResourceType() override;

	segptr_t GetAddress();
};
