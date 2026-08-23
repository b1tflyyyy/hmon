#pragma once

#include <monitor/monitor.hpp>
#include <string>
#include <vector>

namespace hypr::cfg_autogen
{
    namespace detail
    {
        std::string GetDefaultConfigDir();
        std::string GenerateUniqueFilename(std::string_view extension);
    }  // namespace detail

    // Returns path to the generated config
    std::string GenerateLua(const std::vector<monitor::Monitor>& monitors);
}  // namespace hypr::cfg_autogen
