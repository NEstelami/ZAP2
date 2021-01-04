#pragma once

#include <stdint.h>
#include <vector>

class BitConverter
{
public:
	static inline int8_t ToInt8BE(const uint8_t* data, int offset)
	{
		return (uint8_t)data[offset + 0];
	}

	static inline int8_t ToInt8BE(const std::vector<uint8_t>& data, int offset)
	{
		return (uint8_t)data[offset + 0];
	}

	static inline uint8_t ToUInt8BE(const uint8_t* data, int offset)
	{
		return (uint8_t)data[offset + 0];
	}

	static inline uint8_t ToUInt8BE(const std::vector<uint8_t>& data, int offset)
	{
		return (uint8_t)data[offset + 0];
	}

	static inline int16_t ToInt16BE(const uint8_t* data, int offset)
	{
		return ((uint16_t)data[offset + 0] << 8) + (uint16_t)data[offset + 1];
	}

	static inline int16_t ToInt16BE(const std::vector<uint8_t>& data, int offset)
	{
		return ((uint16_t)data[offset + 0] << 8) + (uint16_t)data[offset + 1];
	}

	static inline uint16_t ToUInt16BE(const uint8_t* data, int offset)
	{
		return ((uint16_t)data[offset + 0] << 8) + (uint16_t)data[offset + 1];
	}

	static inline uint16_t ToUInt16BE(const std::vector<uint8_t>& data, int offset)
	{
		return ((uint16_t)data[offset + 0] << 8) + (uint16_t)data[offset + 1];
	}

	static inline int32_t ToInt32BE(const uint8_t* data, int offset)
	{
		return ((uint32_t)data[offset + 0] << 24) + ((uint32_t)data[offset + 1] << 16) + ((uint32_t)data[offset + 2] << 8) + (uint32_t)data[offset + 3];
	}

	static inline int32_t ToInt32BE(const std::vector<uint8_t>& data, int offset)
	{
		return ((uint32_t)data[offset + 0] << 24) + ((uint32_t)data[offset + 1] << 16) + ((uint32_t)data[offset + 2] << 8) + (uint32_t)data[offset + 3];
	}

	static inline uint32_t ToUInt32BE(const uint8_t* data, int offset)
	{
		return ((uint32_t)data[offset + 0] << 24) + ((uint32_t)data[offset + 1] << 16) + ((uint32_t)data[offset + 2] << 8) + (uint32_t)data[offset + 3];
	}

	static inline uint32_t ToUInt32BE(const std::vector<uint8_t>& data, int offset)
	{
		return ((uint32_t)data[offset + 0] << 24) + ((uint32_t)data[offset + 1] << 16) + ((uint32_t)data[offset + 2] << 8) + (uint32_t)data[offset + 3];
	}

	static inline int64_t ToInt64BE(const uint8_t* data, int offset)
	{
		return ((uint64_t)data[offset + 0] << 56) + ((uint64_t)data[offset + 1] << 48) + ((uint64_t)data[offset + 2] << 40) + ((uint64_t)data[offset + 3] << 32) + ((uint64_t)data[offset + 4] << 24) + ((uint64_t)data[offset + 5] << 16) + ((uint64_t)data[offset + 6] << 8) + ((uint64_t)data[offset + 7]);
	}

	static inline int64_t ToInt64BE(const std::vector<uint8_t>& data, int offset)
	{
		return ((uint64_t)data[offset + 0] << 56) + ((uint64_t)data[offset + 1] << 48) + ((uint64_t)data[offset + 2] << 40) + ((uint64_t)data[offset + 3] << 32) + ((uint64_t)data[offset + 4] << 24) + ((uint64_t)data[offset + 5] << 16) + ((uint64_t)data[offset + 6] << 8) + ((uint64_t)data[offset + 7]);
	}

	static inline uint64_t ToUInt64BE(const uint8_t* data, int offset)
	{
		return ((uint64_t)data[offset + 0] << 56) + ((uint64_t)data[offset + 1] << 48) + ((uint64_t)data[offset + 2] << 40) + ((uint64_t)data[offset + 3] << 32) + ((uint64_t)data[offset + 4] << 24) + ((uint64_t)data[offset + 5] << 16) + ((uint64_t)data[offset + 6] << 8) + ((uint64_t)data[offset + 7]);
	}

	static inline uint64_t ToUInt64BE(const std::vector<uint8_t>& data, int offset)
	{
		return ((uint64_t)data[offset + 0] << 56) + ((uint64_t)data[offset + 1] << 48) + ((uint64_t)data[offset + 2] << 40) + ((uint64_t)data[offset + 3] << 32) + ((uint64_t)data[offset + 4] << 24) + ((uint64_t)data[offset + 5] << 16) + ((uint64_t)data[offset + 6] << 8) + ((uint64_t)data[offset + 7]);
	}

	static inline float ToFloatBE(const uint8_t* data, int offset)
	{
		uint32_t floatData = ((uint32_t)data[offset + 0] << 24) + ((uint32_t)data[offset + 1] << 16) + ((uint32_t)data[offset + 2] << 8) + (uint32_t)data[offset + 3];
		return *(float*)(&floatData);
	}

	static inline float ToFloatBE(const std::vector<uint8_t>& data, int offset)
	{
		uint32_t floatData = ((uint32_t)data[offset + 0] << 24) + ((uint32_t)data[offset + 1] << 16) + ((uint32_t)data[offset + 2] << 8) + (uint32_t)data[offset + 3];
		return *(float*)(&floatData);
	}

	static inline double ToDoubleBE(const uint8_t* data, int offset)
	{
		uint64_t floatData = ((uint64_t)data[offset + 0] << 56) + ((uint64_t)data[offset + 1] << 48) + ((uint64_t)data[offset + 2] << 40) + ((uint64_t)data[offset + 3] << 32) + ((uint64_t)data[offset + 4] << 24) + ((uint64_t)data[offset + 5] << 16) + ((uint64_t)data[offset + 6] << 8) + ((uint64_t)data[offset + 7]);
		return *(double*)(&floatData);
	}

	static inline double ToDoubleBE(const std::vector<uint8_t>& data, int offset)
	{
		uint64_t floatData = ((uint64_t)data[offset + 0] << 56) + ((uint64_t)data[offset + 1] << 48) + ((uint64_t)data[offset + 2] << 40) + ((uint64_t)data[offset + 3] << 32) + ((uint64_t)data[offset + 4] << 24) + ((uint64_t)data[offset + 5] << 16) + ((uint64_t)data[offset + 6] << 8) + ((uint64_t)data[offset + 7]);
		return *(double*)(&floatData);
	}
};