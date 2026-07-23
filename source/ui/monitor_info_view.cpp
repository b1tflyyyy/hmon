#include "monitor_info_view.hpp"

#include <format>
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>

namespace ui
{
    MonitorInfoView::MonitorInfoView(const monitor::Monitor& monitor) : monitor_(monitor) { Init(); }

    ftxui::Element MonitorInfoView::Render() const { return root_; }

    void MonitorInfoView::Init()
    {
        // clang-format off
        auto field_row = [](const std::string& param_name, const std::string& param_value) -> ftxui::Element {
            return ftxui::hbox({
                ftxui::text(param_name) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 35) | ftxui::dim,
                ftxui::text(param_value)
            }); 
        };
      
        root_ = ftxui::window(ftxui::text(" Information "),
            ftxui::vbox({
              field_row("ID", std::to_string(monitor_.id_)),
              field_row("Name", monitor_.name_),
              field_row("Description", monitor_.description_),
              field_row("Model", monitor_.model_),
              field_row("Resolution", std::format("{}x{}", monitor_.width_, monitor_.height_)),
              field_row("Physical size in mm (WIDTHxHEIGHT)", std::format("{}x{}", monitor_.physical_width_, monitor_.physical_height_)),
              field_row("Refresh rate", std::to_string(monitor_.refresh_rate_)),
              field_row("Vrr status", std::format("{}", monitor_.vrr_)),
              field_row("Monitor status", monitor_.disabled_ ? "disabled" : "enabled"),
              field_row("Current format", monitor_.current_format_),
              field_row("Color management preset", monitor_.color_management_preset_)
            })
        );
        // clang-format on 
    }  
}  // namespace ui
