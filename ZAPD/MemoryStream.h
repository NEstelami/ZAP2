#pragma once

#include "Stream.h"
#include <memory>
#include <vector>

class MemoryStream : public Stream
{
public:
	MemoryStream();
	MemoryStream(char* nBuffer, size_t nBufferSize);
	~MemoryStream();

	uint64_t GetLength() override;

	void Seek(int32_t offset, SeekOffsetType seekType);

	std::unique_ptr<char[]> Read(size_t length);
	int8_t ReadByte();

	void Write(char* srcBuffer, size_t length);
	void WriteByte(int8_t value);

	std::vector<char> ToVector();

	void Flush();
	void Close();

protected:
	std::vector<char> buffer;
	std::size_t bufferSize;
};