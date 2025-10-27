#pragma once

#include "../../darkside.hpp"

using c_models_data = std::unordered_map< std::string, std::unordered_map< std::string, std::vector< std::string > > >;

class c_skins {
public:
    std::string m_model_path;

    c_models_data custom_models(const std::string& folder);
};

inline const auto g_skins = std::make_unique<c_skins>( );