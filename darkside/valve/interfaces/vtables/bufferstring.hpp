#pragma once

#include "../../../darkside.hpp"

enum StringConvertErrorPolicy {
    STRING_CONVERT_FLAG_SKIP = 1,
    STRING_CONVERT_FLAG_FAIL = 2,
    STRING_CONVERT_FLAG_ASSERT = 4,

    STRING_CONVERT_REPLACE = 0,
    STRING_CONVERT_SKIP = STRING_CONVERT_FLAG_SKIP,
    STRING_CONVERT_FAIL = STRING_CONVERT_FLAG_FAIL,

    STRING_CONVERT_ASSERT_REPLACE = STRING_CONVERT_FLAG_ASSERT + STRING_CONVERT_REPLACE,
    STRING_CONVERT_ASSERT_SKIP = STRING_CONVERT_FLAG_ASSERT + STRING_CONVERT_SKIP,
    STRING_CONVERT_ASSERT_FAIL = STRING_CONVERT_FLAG_ASSERT + STRING_CONVERT_FAIL,
};

class CBufferString {
public:
    enum AllocationOption_t {
        UNK1 = -1,
        UNK2 = 0,
        UNK3 = (1 << 1),
        UNK4 = (1 << 8),
        UNK5 = (1 << 9)
    };

    enum AllocationFlags_t {
        iLengthMASK = (1 << 30) - 1,
        FLAGS_MASK = ~iLengthMASK,

        OVERFLOWED_MARKER = (1 << 30),
        FREE_HEAP_MARKER = (1 << 31),

        STACK_ALLOCATED_MARKER = (1 << 30),
        ALLOW_HEAP_ALLOCATION = (1 << 31)
    };

    CBufferString(bool allow_heap_allocation = true) :
        iLength(0),
        iAllocatedSize((allow_heap_allocation* ALLOW_HEAP_ALLOCATION) | STACK_ALLOCATED_MARKER | sizeof(cString)),
        cStringPtr(nullptr) {
    }

    CBufferString(const char* string, bool allow_heap_allocation = true) :
        CBufferString(allow_heap_allocation) {
        insert(0, string);
    }

    CBufferString(const CBufferString& other) : CBufferString() { *this = other; }

    ~CBufferString() { purge(); }

    void SetHeapAllocationState(bool bState) {
        if (bState)
            iAllocatedSize |= ALLOW_HEAP_ALLOCATION;
        else
            iAllocatedSize &= ~ALLOW_HEAP_ALLOCATION;
    }

    int allocated_num() const { return iAllocatedSize & iLengthMASK; }
    int length() const { return iLength & iLengthMASK; }

    bool can_heap_allocate() const { return (iAllocatedSize & ALLOW_HEAP_ALLOCATION) != 0; }
    bool is_stack_allocated() const { return (iAllocatedSize & STACK_ALLOCATED_MARKER) != 0; }
    bool should_free_memory() const { return (iLength & FREE_HEAP_MARKER) != 0; }
    bool is_overflowed() const { return (iLength & OVERFLOWED_MARKER) != 0; }

    bool is_input_string_unsafe(const char* data) const {
        return ((void*)data >= this && (void*)data < &this[1]) ||
            (!is_allocation_empty() && data >= base() && data < (base() + allocated_num()));
    }

    bool is_allocation_empty() const { return allocated_num() == 0; }

protected:
    char* base() { return is_stack_allocated() ? cString : (!is_allocation_empty() ? cStringPtr : nullptr); }
    const char* base() const { return const_cast<CBufferString*>(this)->base(); }

public:
    void clear() {
        if (!is_allocation_empty())
            base()[0] = '\0';

        iLength &= ~iLengthMASK;
    }

public:
    const char* insert(int index, const char* buf, int count = -1, bool ignore_alignment = false) {
        using fn = const char* (__fastcall*)(void*, int, const char*, int, bool);
        static auto func = reinterpret_cast<fn>(g_opcodes->scan_absolute(g_modules->m_modules.tier0_dll.get_name(), xorstr_("40 53 55 56 57 48 83 EC 38 48 63 EA")));

        return func(this, index, buf, count, ignore_alignment);
    }

    void purge(int allocated_bytes_to_preserve = 0) {
        using fn = void(__fastcall*)(void*, int);
        static auto func = reinterpret_cast<fn>(g_opcodes->scan_absolute(g_modules->m_modules.tier0_dll.get_name(), xorstr_("48 89 5C 24 ? 57 48 83 EC 20 8B 41 04 8D 7A 08 48 8B")));

        func(this, allocated_bytes_to_preserve);
    }

private:
    int iLength;
    int iAllocatedSize;

    union {
        char* cStringPtr;
        char cString[8];
    };
};