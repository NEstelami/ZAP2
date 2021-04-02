#pragma once

#include <array>
#include <cstdint>
#include "ZResource.h"

class ZMtx : public ZResource
{
protected:
	std::array<std::array<int32_t, 4>, 4> mtx;

public:
	ZMtx() = default;
	ZMtx(tinyxml2::XMLElement* reader, const std::vector<uint8_t>& nRawData, int nRawDataIndex,
	     ZFile* nParent);
	ZMtx(const std::string& prefix, const std::vector<uint8_t>& nRawData, int nRawDataIndex,
	     ZFile* nParent);
	void ParseRawData() override;
	static ZMtx* ExtractFromXML(tinyxml2::XMLElement* reader, const std::vector<uint8_t>& nRawData,
	                            int nRawDataIndex, ZFile* nParent);

	int GetRawDataSize() override;

	void DeclareVar(const std::string& prefix, const std::string& bodyStr);

	std::string GetBodySourceCode();
	std::string GetSourceOutputCode(const std::string& prefix) override;
	static std::string GetDefaultName(const std::string& prefix, uint32_t address);

	std::string GetSourceTypeName() override;
	ZResourceType GetResourceType() override;
};
