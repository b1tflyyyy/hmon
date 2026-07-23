#pragma once

#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>
#include <tuple>
#include <vector>

namespace monitor
{
    struct Monitor
    {
        std::uint64_t            id_;
        std::string              name_;
        std::string              description_;
        std::string              model_;
        std::uint32_t            width_;
        std::uint32_t            height_;
        std::uint32_t            physical_width_;
        std::uint32_t            physical_height_;
        double                   refresh_rate_;
        bool                     vrr_;
        bool                     disabled_;
        std::string              current_format_;
        std::vector<std::string> available_modes_;
        std::string              color_management_preset_;

        /*
         *  Returns resolution string in format WIDTHxHEIGHT@REFRESH_RATE[Hz]
         */
        std::string GetResolutionStr() const;

        static std::vector<Monitor> ParseMonitors(std::string_view raw_json);

        friend bool operator==(const Monitor& lhs, const Monitor& rhs)
        {
            auto l = std::tie(lhs.id_, lhs.name_, lhs.description_, lhs.model_, lhs.width_, lhs.height_, lhs.physical_width_,
                              lhs.physical_height_, lhs.refresh_rate_, lhs.vrr_, lhs.disabled_, lhs.current_format_, lhs.available_modes_,
                              lhs.color_management_preset_);

            auto r = std::tie(rhs.id_, rhs.name_, rhs.description_, rhs.model_, rhs.width_, rhs.height_, rhs.physical_width_,
                              rhs.physical_height_, rhs.refresh_rate_, rhs.vrr_, rhs.disabled_, rhs.current_format_, rhs.available_modes_,
                              rhs.color_management_preset_);
            return l == r;
        }

    private:
        static Monitor ParseMonitor(const nlohmann::json& item);
    };
}  // namespace monitor
