#pragma once

#include <atomic>
#include <ftxui/component/screen_interactive.hpp>
#include <monitor/monitor_controller.hpp>
#include <thread>

namespace ui
{
    class MainScreen
    {
    public:
        MainScreen();

        int Run();

    private:
        void RefreshMonitorsInfo();

        void StartRefreshThread();
        void StopRefreshThread();

    private:
        ftxui::ScreenInteractive screen_;

        monitor::MonitorController controller_;
        std::atomic<bool>          refresh_running_;

        // refresh interval in seconds
        static constexpr std::uint64_t kRefreshInterval = 1;
        std::thread                    refresh_thread_;
    };
}  // namespace ui
