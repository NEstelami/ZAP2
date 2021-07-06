#include "Declaration.h"

Declaration::Declaration(uint32_t nAddress, DeclarationAlignment nAlignment,
                         DeclarationPadding nPadding, size_t nSize, std::string nText)
{
	address = nAddress;
	alignment = nAlignment;
	padding = nPadding;
	size = nSize;
	text = nText;
}

Declaration::Declaration(uint32_t nAddress, DeclarationAlignment nAlignment, size_t nSize,
                         std::string nVarType, std::string nVarName, bool nIsArray,
                         std::string nText)
	: Declaration(nAddress, nAlignment, DeclarationPadding::None, nSize, nText)
{
	varType = nVarType;
	varName = nVarName;
	isArray = nIsArray;
}

Declaration::Declaration(uint32_t nAddress, DeclarationAlignment nAlignment,
                         DeclarationPadding nPadding, size_t nSize, std::string nVarType,
                         std::string nVarName, bool nIsArray, std::string nText)
	: Declaration(nAddress, nAlignment, nPadding, nSize, nText)
{
	varType = nVarType;
	varName = nVarName;
	isArray = nIsArray;
}

Declaration::Declaration(uint32_t nAddress, DeclarationAlignment nAlignment, size_t nSize,
                         std::string nVarType, std::string nVarName, bool nIsArray,
                         size_t nArrayItemCnt, std::string nText)
	: Declaration(nAddress, nAlignment, DeclarationPadding::None, nSize, nText)
{
	varType = nVarType;
	varName = nVarName;
	isArray = nIsArray;
	arrayItemCnt = nArrayItemCnt;
}

Declaration::Declaration(uint32_t nAddress, DeclarationAlignment nAlignment, size_t nSize,
                         std::string nVarType, std::string nVarName, bool nIsArray,
                         std::string nArrayItemCntStr, std::string nText)
	: Declaration(nAddress, nAlignment, DeclarationPadding::None, nSize, nText)
{
	varType = nVarType;
	varName = nVarName;
	isArray = nIsArray;
	arrayItemCntStr = nArrayItemCntStr;
}

Declaration::Declaration(uint32_t nAddress, DeclarationAlignment nAlignment, size_t nSize,
                         std::string nVarType, std::string nVarName, bool nIsArray,
                         size_t nArrayItemCnt, std::string nText, bool nIsExternal)
	: Declaration(nAddress, nAlignment, nSize, nVarType, nVarName, nIsArray, nArrayItemCnt, nText)
{
	isExternal = nIsExternal;
}

Declaration::Declaration(uint32_t nAddress, DeclarationAlignment nAlignment,
                         DeclarationPadding nPadding, size_t nSize, std::string nVarType,
                         std::string nVarName, bool nIsArray, size_t nArrayItemCnt,
                         std::string nText)
	: Declaration(nAddress, nAlignment, nPadding, nSize, nText)
{
	varType = nVarType;
	varName = nVarName;
	isArray = nIsArray;
	arrayItemCnt = nArrayItemCnt;
}

Declaration::Declaration(uint32_t nAddress, std::string nIncludePath, size_t nSize,
                         std::string nVarType, std::string nVarName)
	: Declaration(nAddress, DeclarationAlignment::None, DeclarationPadding::None, nSize, "")
{
	includePath = nIncludePath;
	varType = nVarType;
	varName = nVarName;
}
