#pragma once

#include <ftxui/component/app.hpp>
#include <monitor/monitor_controller.hpp>

namespace ui
{
    class TweaksPanelView
    {
    public:
        explicit TweaksPanelView(std::uint64_t monitor_id, monitor::MonitorController& controller);

        ftxui::Component Component() const;

    private:
        void Init();

    private:
        std::uint64_t               monitor_id_;
        monitor::MonitorController& controller_;

        int                      selected_mode_;
        std::vector<std::string> available_modes_;

        bool          onoff_state_;
        std::uint32_t scale_;

        ftxui::Component root_;
    };
}  // namespace ui
