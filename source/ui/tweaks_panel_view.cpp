#include "tweaks_panel_view.hpp"

#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>

namespace ui
{
    TweaksPanelView::TweaksPanelView(std::uint64_t monitor_id, monitor::MonitorController& controller)
        : monitor_id_(monitor_id), controller_(controller), selected_mode_(0), onoff_state_(false), scale_(100)
    {
        Init();
    }

    ftxui::Component TweaksPanelView::Component() const { return root_; }

    void TweaksPanelView::Init()
    {
        monitor::Monitor monitor = controller_.GetMonitor(monitor_id_);

        onoff_state_     = !monitor.disabled_;
        available_modes_ = monitor.available_modes_;

        scale_ = static_cast<std::uint32_t>(monitor.scale_ * 100.0);
        if (scale_ < 10) scale_ = 100;

        static ftxui::CheckboxOption onoff_option{};
        onoff_option.on_change = [this] { controller_.Toggle(monitor_id_); };

        ftxui::Component onoff_component = ftxui::Checkbox("Enabled", std::addressof(onoff_state_), onoff_option);

        static ftxui::SliderOption<std::uint32_t> slider_opt{};
        slider_opt.value     = std::addressof(scale_);
        slider_opt.min       = 10;
        slider_opt.max       = 300;
        slider_opt.increment = 10;
        slider_opt.on_change = [this] { controller_.SetScale(monitor_id_, static_cast<double>(scale_) / 100.0); };

        ftxui::Component scale_slider = ftxui::Slider(slider_opt);

        static ftxui::MenuOption menu_option{};
        menu_option.on_enter = [this] { controller_.SetMode(monitor_id_, available_modes_[selected_mode_]); };

        ftxui::Component mode_menu = ftxui::Menu(std::addressof(available_modes_), std::addressof(selected_mode_), menu_option);

        // clang-format off
        auto status_block = ftxui::Renderer(onoff_component, [onoff_component] {
            return ftxui::vbox({
                ftxui::text("Monitor status:") | ftxui::dim,
                onoff_component->Render(),
            });
        });

        auto scale_block = ftxui::Renderer(scale_slider, [this, scale_slider] {
            return ftxui::vbox({
                ftxui::text(std::format("Monitor scale: [current: {:.2f}x]", scale_ / 100.0)) | ftxui::dim,
                scale_slider->Render() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 30),
                }) | ftxui::flex;
        });

        auto spacer = ftxui::Renderer([] { return ftxui::emptyElement() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 4); });

        auto top_row = ftxui::Container::Horizontal({status_block, spacer, scale_block});
        auto layout  = ftxui::Container::Vertical({top_row, mode_menu});

        root_ = ftxui::Renderer(layout, [this, top_row, mode_menu] constexpr {
            ftxui::Element mode_box = mode_menu->Render() | ftxui::vscroll_indicator
                                                          | ftxui::frame 
                                                          | ftxui::border
                                                          | ftxui::flex;  
            
            monitor::Monitor monitor = controller_.GetMonitor(monitor_id_);

            ftxui::Elements rows{};
            rows.append_range(std::initializer_list<ftxui::Element>{
                top_row->Render(),       
                ftxui::text(std::format("Select resolution: [current: {}x{}@{:.2f}Hz]", monitor.width_, monitor.height_, monitor.refresh_rate_)) | ftxui::dim,
                mode_box,
            });

            return ftxui::window(ftxui::text(" Tweaks "), ftxui::vbox(std::move(rows))) | ftxui::flex;
        });
        // clang-format on
    }
}  // namespace ui
