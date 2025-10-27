#include "skins.hpp"


c_models_data c_skins::custom_models(const std::string& folder)
{
    c_models_data data;

    if (!std::filesystem::is_directory(folder))
        return data;

    for (const auto& entry : std::filesystem::directory_iterator(folder))
    {
        if (!entry.is_directory())
            continue;

        std::string name = entry.path().filename().string();
        std::unordered_map< std::string, std::vector< std::string > > packs;

        for (const auto& pack : std::filesystem::directory_iterator(entry.path()))
        {
            if (!pack.is_directory())
                continue;

            std::vector< std::string > model_list;

            for (const auto& model : std::filesystem::directory_iterator(pack.path()))
            {
                if (model.is_regular_file() && model.path().extension() == xorstr_(".vmdl_c"))
                {
                    std::string model_name = model.path().filename().string();

                    if (model_name.find(xorstr_("_arms")) == std::string::npos && model_name.find(xorstr_("_arm")) == std::string::npos)
                    {
                        if (model_name.ends_with(xorstr_(".vmdl_c")))
                            model_name.erase(model_name.size() - 2);

                        model_list.push_back(model_name);
                    }
                }
            }

            if (!model_list.empty())
                packs[pack.path().filename().string()] = model_list;
        }

        if (!packs.empty())
            data[name] = packs;
    }

    return data;
}
