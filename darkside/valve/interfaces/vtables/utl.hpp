#pragma once

#include "../../../darkside.hpp"

class CUtlBuffer
{
public:
    char pad_0001[ 0x80 ];

    CUtlBuffer(int a1, int nSize, int a3)
    {
        g_interfaces->fnUtlBufferInit(this, a1, nSize, a3);
    }

    void PutsString(const char* szString)
    {
        g_interfaces->fnUtlBufferPutString(this, szString);
    }

    void EnsureCapacity(int nSize)
    {
        g_interfaces->fnUtlBufferEnsureCapacity(this, nSize);
    }
};