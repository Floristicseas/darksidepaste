#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <array> 

#include "..\..\utils\pe64.h"

namespace parser
{
	static std::string WC2Str(const wchar_t* wszString)
	{
		int iLength = WideCharToMultiByte(CP_UTF8, 0, wszString, -1, nullptr, 0, nullptr, nullptr);
		char* szString = new char[iLength];

		WideCharToMultiByte(CP_UTF8, 0, wszString, -1, szString, iLength, nullptr, nullptr);
		return std::string(szString);
	}

	static std::string GetDirectory()
	{
		const _PEB* pPEB = reinterpret_cast<_PEB*>(__readgsqword(0x60));

		for (LIST_ENTRY* pListEntry = pPEB->Ldr->InMemoryOrderModuleList.Flink; pListEntry != &pPEB->Ldr->InMemoryOrderModuleList; pListEntry = pListEntry->Flink)
		{
			const _LDR_DATA_TABLE_ENTRY* pEntry = CONTAINING_RECORD(pListEntry, _LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

			std::string strzModuleName = WC2Str(pEntry->BaseDllName.Buffer);
			std::string strModuleFullName = WC2Str(pEntry->FullDllName.Buffer);

			if (strcmp(strzModuleName.c_str(), "client.dll") == 0)
			{
				size_t binPos = strModuleFullName.find("csgo");
				size_t clientDllPos = strModuleFullName.find("client.dll", binPos);

				strModuleFullName.erase(binPos, clientDllPos - binPos + std::string("client.dll").length());

				return strModuleFullName;
			}
		}

		return "";
	}
}

struct Model_t
{
	std::string name;
	std::string path;
};

class c_model_changer
{
public:
	std::vector<Model_t> player_models;

	unsigned long long cur_player_model = ~1U;

	uint32_t last_player_model_hash;

	void update_player_models();
	void set_player_model();
};

class c_skins {
public:
	std::string m_model_path;

	struct {
		std::vector< const char* > m_dumped_agent_models;
		std::vector< const char* > m_dumped_agent_name;
	} m_agents;

	struct {
		std::array< int, 20 > m_knife_idxs = { 500, 503, 505, 506, 507, 508, 509, 512, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 525, 526 };
		std::vector< const char* > m_dumped_knife_models;
		std::vector< const char* > m_dumped_knife_name;
	} m_knives;

	void dump_items();

	void agent_changer(int stage);
	void knife_changer(int stage);
};

inline const auto g_skins = std::make_unique< c_skins >();

inline auto g_model_changer = std::make_unique<c_model_changer>();