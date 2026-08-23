#include "tweaks_panel_view.hpp"

#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>

namespace ui
{
    TweaksPanelView::TweaksPanelView(std::uint64_t monitor_id, monitor::MonitorController& controller)
        : monitor_id_(monitor_id),
          controller_(controller),
          selected_mode_(0),
          onoff_state_(false),
          scale_(100),
          show_modal_(false),
          cfg_generated_path_("")
    {
        Init();
    }

    ftxui::Component TweaksPanelView::Component() const { return root_; }

    // TODO: refactor / add some comments
    void TweaksPanelView::Init()
    {
        monitor::Monitor monitor = controller_.GetMonitor(monitor_id_);

        onoff_state_     = !monitor.disabled_;
        available_modes_ = monitor.available_modes_;

        scale_ = static_cast<std::uint32_t>(monitor.scale_ * 100.0);
        if (scale_ < 10) scale_ = 100;

        ftxui::CheckboxOption onoff_option{};
        onoff_option.on_change = [this] { controller_.Toggle(monitor_id_); };

        ftxui::Component onoff_component = ftxui::Checkbox("Enabled", std::addressof(onoff_state_), onoff_option);

        ftxui::SliderOption<std::uint32_t> slider_opt{};
        slider_opt.value     = std::addressof(scale_);
        slider_opt.min       = 10;
        slider_opt.max       = 300;
        slider_opt.increment = 10;
        slider_opt.on_change = [this] { controller_.SetScale(monitor_id_, static_cast<double>(scale_) / 100.0); };

        ftxui::Component scale_slider = ftxui::Slider(slider_opt);

        ftxui::MenuOption menu_option{};
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
            }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 40);
        });

        auto spacer = ftxui::Renderer([] { return ftxui::emptyElement() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 4); });

        // clang-format off
        auto generate_cfg_btn = ftxui::Button("Generate Config", [this] {
            cfg_generated_path_ = controller_.GenerateConfig();
            show_modal_ = true;
        });

        auto top_row = ftxui::Container::Horizontal({status_block, spacer, scale_block, generate_cfg_btn});

        auto modal_close_btn = ftxui::Button(" OK ", [this] {
            show_modal_ = false;
        });

        auto modal_dialog = ftxui::Renderer(modal_close_btn, [this, modal_close_btn] {
            return ftxui::window(
                ftxui::text(" Config Generated ") | ftxui::bold | ftxui::color(ftxui::Color::Green),
                ftxui::vbox({
                    ftxui::text("Hyprland config successfully generated at:") | ftxui::dim,
                    ftxui::separator(),
                    ftxui::text(cfg_generated_path_) | ftxui::bold | ftxui::color(ftxui::Color::Cyan),
                    ftxui::separator(),
                    modal_close_btn->Render() | ftxui::center,
                })
            ) | ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, 45)
              | ftxui::clear_under
              | ftxui::center;
        });
        // clang-format on

        auto layout = ftxui::Container::Vertical({top_row, mode_menu});

        // clang-format off
        auto main_view = ftxui::Renderer(layout, [this, top_row, mode_menu, generate_cfg_btn] constexpr {
            ftxui::Element mode_box = mode_menu->Render() | ftxui::vscroll_indicator
                                                          | ftxui::frame
                                                          | ftxui::border
                                                          | ftxui::flex;

            monitor::Monitor monitor = controller_.GetMonitor(monitor_id_);

            ftxui::Elements rows{};
            rows.append_range(std::initializer_list<ftxui::Element>{
                top_row->Render(),
                ftxui::text(std::format("Select resolution: [current: {}x{}@{:.2f}Hz]", monitor.width_, monitor.height_, monitor.refresh_rate_)) | ftxui::dim,
                mode_box
            });

            return ftxui::window(ftxui::text(" Tweaks "), ftxui::vbox(std::move(rows))) | ftxui::flex;
        });
        // clang-format on

        root_ = ftxui::Modal(main_view, modal_dialog, std::addressof(show_modal_));
    }
}  // namespace ui
