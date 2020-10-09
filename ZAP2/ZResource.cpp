#include "ZResource.h"

using namespace std;

ZResource::ZResource()
{
	parent = nullptr;
	name = "";
	relativePath = "";
	sourceOutput = "";
	rawData = vector<uint8_t>();
	rawDataIndex = 0;
}

void ZResource::Save(string outFolder)
{

}

string ZResource::GetName()
{
	return name;
}

void ZResource::SetName(string nName)
{
	name = nName;
}

bool ZResource::IsExternalResource()
{
	return false;
}

std::string ZResource::GetExternalExtension()
{
	return "";
}

string ZResource::GetRelativePath()
{
	return relativePath;
}

vector<uint8_t> ZResource::GetRawData()
{
	return rawData;
}

int ZResource::GetRawDataIndex()
{
	return rawDataIndex;
}

int ZResource::GetRawDataSize()
{
	return rawData.size();
}

void ZResource::SetRawDataIndex(int value)
{
	rawDataIndex = value;
}

string ZResource::GetSourceOutputCode(std::string prefix)
{
	return "";
}

string ZResource::GetSourceOutputHeader(std::string prefix)
{
	return "";
}