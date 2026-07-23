#pragma once

#include <cstdint>
#include <ftxui/dom/elements.hpp>
#include <memory>
#include <monitor/monitor_controller.hpp>

#include "monitor_info_view.hpp"
#include "tweaks_panel_view.hpp"

namespace ui
{
    class MonitorPanelView
    {
    public:
        explicit MonitorPanelView(std::uint64_t monitor_id, monitor::MonitorController& controller);

        ftxui::Component Component() const;

    private:
        void Init();

    private:
        std::uint64_t monitor_id_;

        std::unique_ptr<MonitorInfoView> monitor_info_;
        std::unique_ptr<TweaksPanelView> tweaks_panel_;

        monitor::MonitorController& controller_;

        ftxui::Component root_;
    };
}  // namespace ui
