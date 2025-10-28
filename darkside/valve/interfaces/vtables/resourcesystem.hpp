#pragma once

#include "../../../darkside.hpp"

#include "bufferstring.hpp"

struct ResourceBinding_t;

class IResourceSystem {
public:
	void* QueryInterface(const char* szInterfaceName) {
		return vmt::call_virtual<void*>(this, 3U, szInterfaceName);
	}

	void PreCache(const char* szName) {
		CBufferString pBufferString(szName);
		vmt::call_virtual<void>(this, 49U, &pBufferString, "");
	}
};

class CResourceHandleUtils {
public:
	void DeleteResource(const ResourceBinding_t* pBinding) {
		vmt::call_virtual<void>(this, 2U, pBinding);
	}
};