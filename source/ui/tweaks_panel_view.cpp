#include "tweaks_panel_view.hpp"

#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>

namespace ui
{

    TweaksPanelView::TweaksPanelView(std::uint64_t monitor_id, monitor::MonitorController& controller)
        : monitor_id_(monitor_id), controller_(controller), selected_mode_(0), onoff_state_(false)
    {
        Init();
    }

    ftxui::Component TweaksPanelView::Component() const { return root_; }

    void TweaksPanelView::Init()
    {
        monitor::Monitor monitor = controller_.GetMonitor(monitor_id_);

        onoff_state_     = !monitor.disabled_;
        available_modes_ = monitor.available_modes_;

        static ftxui::CheckboxOption onoff_option{};
        onoff_option.on_change = [this] { controller_.Toggle(monitor_id_); };

        ftxui::Component onoff_component = ftxui::Checkbox("Enabled", std::addressof(onoff_state_), onoff_option);

        static ftxui::MenuOption menu_option{};
        menu_option.on_enter = [this] { controller_.SetMode(monitor_id_, available_modes_[selected_mode_]); };

        ftxui::Component mode_menu = ftxui::Menu(std::addressof(available_modes_), std::addressof(selected_mode_), menu_option);
        auto             layout    = ftxui::Container::Vertical({onoff_component, mode_menu});

        // clang-format off
        root_ = ftxui::Renderer(layout, [this, onoff_component, mode_menu] {
            ftxui::Element mode_box = mode_menu->Render() | ftxui::vscroll_indicator
                                                          | ftxui::frame 
                                                          | ftxui::border; 

            ftxui::Elements rows{};
            rows.append_range(std::initializer_list<ftxui::Element>{
                ftxui::text("Monitor status:") | ftxui::dim,        
                onoff_component->Render(),
                ftxui::text(std::format("Select resolution: [current: {}]", controller_.GetMonitor(monitor_id_).GetResolutionStr())) | ftxui::dim,
                mode_box
            });

            return ftxui::window(ftxui::text(" Tweaks "), ftxui::vbox(rows));
        });
        // clang-format on
    }
}  // namespace ui
