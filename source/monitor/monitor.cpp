#include "monitor.hpp"

#include <ranges>
#include <stdexcept>

namespace monitor
{
    Monitor Monitor::ParseMonitor(const nlohmann::json& item)
    {
        Monitor mon;

        mon.id_              = item.value("id", -1);
        mon.name_            = item.value("name", "undefined");
        mon.description_     = item.value("description", "undefined");
        mon.model_           = item.value("model", "undefined");
        mon.width_           = item.value("width", -1);
        mon.height_          = item.value("height", -1);
        mon.physical_width_  = item.value("physicalWidth", -1);
        mon.physical_height_ = item.value("physicalHeight", -1);
        mon.refresh_rate_    = item.value("refreshRate", -1.0);
        mon.x_pos_           = item.value("x", -1);
        mon.y_pos_           = item.value("y", -1);
        mon.scale_           = item.value("scale", -1.0);
        mon.vrr_             = item.value("vrr", false);
        mon.disabled_        = item.value("disabled", false);
        mon.current_format_  = item.value("currentFormat", "undefined");

        // clang-format off
        mon.available_modes_ = item["availableModes"] | std::views::transform([](const nlohmann::json& item) {
            /*
             * take_while ch != 'H' -> because we don't need suffix "Hz" (it's not used in hyprctl query)
            */
            return item.get<std::string>() | std::views::take_while([](char ch) { return ch != 'H'; }) 
                                           | std::ranges::to<std::string>();
        }) | std::ranges::to<std::vector<std::string>>();
        // clang-format on

        mon.color_management_preset_ = item.value("colorManagementPreset", "undefined");
        return mon;
    }

    std::vector<Monitor> Monitor::ParseMonitors(std::string_view raw_json)
    {
        nlohmann::json data;
        try
        {
            data = nlohmann::json::parse(raw_json);
        }
        catch (const nlohmann::json::parse_error& e)
        {
            throw std::runtime_error("failed to parse json");
        }

        // clang-format off
        return data | std::views::transform([](const nlohmann::json& item) {
            return ParseMonitor(item);
        }) | std::ranges::to<std::vector<Monitor>>();
        // clang-format on
    }
}  // namespace monitor
