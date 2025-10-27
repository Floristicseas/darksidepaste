#pragma once

#include <array>

#include "../../utils/utils.hpp"
#include "../../valve/modules/modules.hpp"
#include "../../valve/classes/c_murmurhash.hpp"
#include "../../sdk/vfunc/vfunc.hpp"

class c_game_event;
class c_cs_player_controller;

class c_utl_string_token
{
public:
	std::uint32_t m_nHashCode;

	c_utl_string_token(const char* szString)
	{
		this->SetHashCode(this->MakeStringToken(szString));
	}

	bool operator==(const c_utl_string_token& other) const
	{
		return (other.m_nHashCode == m_nHashCode);
	}

	bool operator!=(const c_utl_string_token& other) const
	{
		return (other.m_nHashCode != m_nHashCode);
	}

	bool operator<(const c_utl_string_token& other) const
	{
		return (m_nHashCode < other.m_nHashCode);
	}

	uint32_t GetHashCode(void) const
	{
		return m_nHashCode;
	}

	void SetHashCode(uint32_t nCode)
	{
		m_nHashCode = nCode;
	}

	__forceinline std::uint32_t MakeStringToken(const char* szString, int nLen)
	{
		std::uint32_t nHashCode = murmurhash2_lowercase2(szString, nLen, STRINGTOKEN_MURMURHASH_SEED);
		return nHashCode;
	}

	__forceinline std::uint32_t MakeStringToken(const char* szString)
	{
		return MakeStringToken(szString, (int)strlen(szString));
	}

	c_utl_string_token()
	{
		m_nHashCode = 0;
	}
};

class c_buffer
{
public:
	char Pad[0x8];
	const char* name;
};

class c_game_event_helper
{
public:
	c_game_event_helper(c_game_event* Event) :
		Event{ std::move(Event) } {
	};

public:
	c_cs_player_controller* GetPlayerController();
	c_cs_player_controller* GetAttackerController();

	int GetDamage();
	int GetHealth();

protected:
	c_game_event* Event;
};

class c_game_event
{
public:
	virtual ~c_game_event() = 0;
	virtual const char* get_name() const = 0;

	virtual void Unk1() = 0;
	virtual void Unk2() = 0;
	virtual void Unk3() = 0;
	virtual void Unk4() = 0;

	virtual bool get_bool(const char* KeyName = nullptr, bool DefaultValue = false) = 0;
	virtual int get_int(c_buffer* KeyName = nullptr, bool DefaultValue = false) = 0;
	virtual uint64_t get_uint64(const char* KeyName = nullptr, uint64_t DefaultValue = 0) = 0;
	virtual float get_float(const char* KeyName = nullptr, float DefaultValue = 0.f) = 0;
	virtual const char* GetString(const char* KeyName = nullptr, const char* DefaultValue = "") = 0;
	virtual void Unk5() = 0;
	virtual void Unk6() = 0;
	virtual void Unk7() = 0;
	virtual void Unk8() = 0;
	virtual void GetControllerId(int& ControllerId, c_buffer* Buffer = nullptr) = 0;

public:
	c_game_event_helper get_event_helper()
	{
		c_game_event_helper EventHelper(this);
		return EventHelper;
	}

	int get_int2(const char* Name, bool Unk);
	float get_float2(const char* Name, bool Unk);

	[[nodiscard]] float get_float_new2(const c_utl_string_token& keyToken, const float flDefault = 0.0f) const
	{
		using function_t = float(__thiscall*)(const void*, const c_utl_string_token&, float);
		auto vtable = *reinterpret_cast<function_t**>(const_cast<void*>(static_cast<const void*>(this)));
		return vtable[9](this, keyToken, flDefault);
	}
};

class i_game_event_listener
{
public:
	virtual ~i_game_event_listener() {}

	virtual void fire_game_event(c_game_event* Event) = 0;
};

class i_game_event_manager
{
public:
	int load_file(const char* szFileName)
	{
		return vmt::call_virtual< int >(this, 1, szFileName);
	}

	void reset()
	{
		vmt::call_virtual< void >(this, 2);
	}

	bool add(i_game_event_listener* pListener, const char* szName, bool bServerSide = false)
	{
		return vmt::call_virtual< bool >(this, 3, pListener, szName, bServerSide);
	}

	bool find(i_game_event_listener* pListener, const char* szName)
	{
		return vmt::call_virtual< bool >(this, 4, pListener, szName);
	}

	void remove(i_game_event_listener* pListener)
	{
		vmt::call_virtual< void >(this, 5, pListener);
	}
};

class c_events : public i_game_event_listener
{
public:
	void on_bullet_tracer(c_game_event* event);
	void fire_game_event(c_game_event* event);
	void on_player_hurt(c_game_event* event);
	void player_death(c_game_event* event);

	bool initialize();
};

inline c_events* g_events = new c_events();