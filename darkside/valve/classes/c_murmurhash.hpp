#include <Windows.h>
#include <cstdint>

#define STRINGTOKEN_MURMURHASH_SEED 0x31415926

static uint32_t murmurhash2(const void* key, int len, uint32_t seed)
{
	const uint32_t m = 0x5bd1e995;
	const int r = 24;

	uint32_t h = seed ^ len;

	const unsigned char* data = (const unsigned char*)key;

	while (len >= 4)
	{
		uint32_t k = *(uint32_t*)data;

		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		data += 4;
		len -= 4;
	}

	switch (len)
	{
	case 3:
		h ^= data[2] << 16;
	case 2:
		h ^= data[1] << 8;
	case 1:
		h ^= data[0];
		h *= m;
	};

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}

#define TOLOWERU( c ) ( ( uint32_t )( ( ( c >= 'A' ) && ( c <= 'Z' ) ) ? c + 32 : c ) )

static uint32_t murmurhash2_lowercase2(const char* pString, int len, uint32_t nSeed)
{
	char* p = (char*)malloc(len + 1);

	for (int i = 0; i < len; i++)
	{
		p[i] = TOLOWERU(pString[i]);
	}

	return murmurhash2(p, len, nSeed);
}