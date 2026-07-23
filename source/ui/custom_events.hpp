#pragma once

#include <ftxui/component/event.hpp>

namespace ui
{
    namespace custom_events
    {
        static const inline ftxui::Event kMonitorInfoChanged         = ftxui::Event::Special("MonitorInfoChanged");
        static const inline ftxui::Event kMonitorsCountOrNameChanged = ftxui::Event::Special("MonitorsCountOrNameChanged");
    }  // namespace custom_events
}  // namespace ui
