#pragma once

#include <ftxui/dom/elements.hpp>
#include <memory>
#include <monitor/monitor_controller.hpp>

#include "monitor_panel_view.hpp"

namespace ui
{
    class MonitorTabsView
    {
    public:
        explicit MonitorTabsView(monitor::MonitorController& controller);

        ftxui::Component Component() const;

    private:
        void Init();
        void RefreshTabs();

    private:
        monitor::MonitorController& controller_;

        int                      active_tab_;
        std::vector<std::string> tab_lables_;

        std::vector<std::unique_ptr<MonitorPanelView>> panels_;
        ftxui::Component                               container_;
        ftxui::Component                               root_;
    };

}  // namespace ui
