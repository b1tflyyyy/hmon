#include "monitor_panel_view.hpp"

#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>

#include "custom_events.hpp"
#include "monitor_box_preview.hpp"
#include "monitor_info_view.hpp"

namespace ui
{
    MonitorPanelView::MonitorPanelView(std::uint64_t monitor_id, monitor::MonitorController& controller)
        : monitor_id_(monitor_id), controller_(controller)
    {
        Init();
    }

    ftxui::Component MonitorPanelView::Component() const { return root_; }

    void MonitorPanelView::Init()
    {
        tweaks_panel_ = std::make_unique<TweaksPanelView>(monitor_id_, controller_);
        auto layout   = ftxui::Container::Vertical({tweaks_panel_->Component()});

        // clang-format off
        auto event_handler = ftxui::CatchEvent(layout, [](ftxui::Event event) {
            return event == custom_events::kMonitorInfoChanged;
        });

        root_ = ftxui::Renderer(event_handler, [this] -> ftxui::Element {
            monitor::Monitor monitor = controller_.GetMonitor(monitor_id_);

            MonitorInfoView info_view(monitor);
            MonitorPreviewBox preview_box(monitor.physical_width_, monitor.physical_height_);

            return ftxui::vbox({
                ftxui::hbox({ 
                    preview_box.Render(), 
                    info_view.Render() | ftxui::flex  
                }),
                tweaks_panel_->Component()->Render() | ftxui::flex
            });
        });
        // clang-format on
    }
}  // namespace ui
