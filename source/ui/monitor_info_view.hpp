#pragma once

#include <ftxui/dom/elements.hpp>
#include <monitor/monitor.hpp>

namespace ui
{
    class MonitorInfoView
    {
    public:
        explicit MonitorInfoView(const monitor::Monitor& monitor);

        ftxui::Element Render() const;

    private:
        void Init();

    private:
        const monitor::Monitor& monitor_;
        ftxui::Element          root_;
    };
}  // namespace ui
