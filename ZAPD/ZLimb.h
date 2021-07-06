#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "OtherStructs/SkinLimbStructs.h"
#include "ZDisplayList.h"
#include "ZFile.h"

enum class ZLimbType
{
	Invalid,
	Standard,
	LOD,
	Skin,
	Curve,
	Legacy,
};

class ZLimb : public ZResource
{
public:
	ZLimb(ZFile* nParent);

	void ExtractFromBinary(uint32_t nRawDataIndex, ZLimbType nType);

	void ParseXML(tinyxml2::XMLElement* reader) override;
	void ParseRawData() override;
	void DeclareReferences(const std::string& prefix) override;

	std::string GetBodySourceCode() const override;
	std::string GetDefaultName(const std::string& prefix) const;

	size_t GetRawDataSize() const override;
	std::string GetSourceTypeName() const override;
	ZResourceType GetResourceType() const override;

	ZLimbType GetLimbType();
	void SetLimbType(ZLimbType value);
	static const char* GetSourceTypeName(ZLimbType limbType);
	static ZLimbType GetTypeByAttributeName(const std::string& attrName);

protected:
	ZLimbType type = ZLimbType::Standard;

	int16_t transX, transY, transZ;
	uint8_t childIndex, siblingIndex;

	segptr_t dListPtr = 0;
	segptr_t dList2Ptr = 0;                                     // LOD and Curve Only
	ZLimbSkinType skinSegmentType = ZLimbSkinType::SkinType_0;  // Skin only
	segptr_t skinSegment = 0;                                   // Skin only
	Struct_800A5E28 segmentStruct;                              // Skin only

	// Legacy only
	float legTransX, legTransY, legTransZ;  // Vec3f
	uint16_t rotX, rotY, rotZ;              // Vec3s
	segptr_t childPtr;                      // LegacyLimb*
	segptr_t siblingPtr;                    // LegacyLimb*

	void DeclareDList(segptr_t dListSegmentedPtr, const std::string& prefix,
	                  const std::string& limbSuffix);
};
