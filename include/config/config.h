#pragma once
#include <filesystem>

#include "autoboost.h"

// NOLINTNEXTLINE(hicpp-special-member-functions,cppcoreguidelines-special-member-functions)
class Config {
    // TODO : Init on Contrustion while save on desconstruction
    Config() = default;
    ~Config() = default;

    AutoBoost AutoBoost;

    void init(std::filesystem::path path);
    void save(std::filesystem::path path);
};
