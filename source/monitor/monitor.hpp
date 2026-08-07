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
        std::uint32_t            x_pos_;
        std::uint32_t            y_pos_;
        double                   scale_;
        bool                     vrr_;
        bool                     disabled_;
        std::string              current_format_;
        std::vector<std::string> available_modes_;
        std::string              color_management_preset_;

        static std::vector<Monitor> ParseMonitors(std::string_view raw_json);

        friend bool operator==(const Monitor& lhs, const Monitor& rhs)
        {
            auto l = std::tie(lhs.id_, lhs.name_, lhs.description_, lhs.model_, lhs.width_, lhs.height_, lhs.physical_width_,
                              lhs.physical_height_, lhs.x_pos_, lhs.y_pos_, lhs.scale_, lhs.refresh_rate_, lhs.vrr_, lhs.disabled_,
                              lhs.current_format_, lhs.available_modes_, lhs.color_management_preset_);

            auto r = std::tie(rhs.id_, rhs.name_, rhs.description_, rhs.model_, rhs.width_, rhs.height_, rhs.physical_width_,
                              rhs.physical_height_, rhs.refresh_rate_, rhs.x_pos_, rhs.y_pos_, rhs.scale_, rhs.vrr_, rhs.disabled_,
                              rhs.current_format_, rhs.available_modes_, rhs.color_management_preset_);
            return l == r;
        }

    private:
        static Monitor ParseMonitor(const nlohmann::json& item);
    };
}  // namespace monitor
