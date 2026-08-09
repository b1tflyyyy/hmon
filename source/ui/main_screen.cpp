#include "main_screen.hpp"

#include <chrono>
#include <ftxui/component/component.hpp>
#include <ftxui/screen/screen.hpp>

#include "custom_events.hpp"
#include "monitor_tabs_view.hpp"

namespace ui
{
    MainScreen::MainScreen() : screen_(ftxui::ScreenInteractive::Fullscreen()) {}

    int MainScreen::Run()
    {
        controller_.Refresh();
        MonitorTabsView tabs_view(controller_);

        StartRefreshThread();

        // clang-format off
        auto component = ftxui::CatchEvent(tabs_view.Component(), [&](ftxui::Event event) -> bool {
            if (event == ftxui::Event::Character('q'))
            {
                screen_.ExitLoopClosure()();
                return true;
            } 

            if (event == ftxui::Event::Character('r'))  
            {
                RefreshMonitorsInfo();
                return true;
            }

            return false;
        });
        // clang-format on

        screen_.Loop(component);
        StopRefreshThread();

        return 0;
    }

    void MainScreen::RefreshMonitorsInfo()
    {
        using RefreshStatus = monitor::MonitorController::RefreshStatusT;

        RefreshStatus status = controller_.Refresh();
        if (status.HasFlag(RefreshStatus::kNothingChanged))
        {
            return;
        }

        if (status.HasFlag(RefreshStatus::kMonitorsCountOrNameChanged))
        {
            screen_.PostEvent(custom_events::kMonitorsCountOrNameChanged);
        }
        if (status.HasFlag(RefreshStatus::kMonitorInfoChanged))
        {
            screen_.PostEvent(custom_events::kMonitorInfoChanged);
        }
    }

    void MainScreen::StartRefreshThread()
    {
        refresh_running_.store(true);

        // clang-format off
        refresh_thread_ = std::thread([this] { 
            while(refresh_running_.load()) 
            {
                std::this_thread::sleep_for(std::chrono::seconds(kRefreshInterval));
                RefreshMonitorsInfo();
            }
        });
        // clang-format on
    }

    void MainScreen::StopRefreshThread()
    {
        refresh_running_.store(false);
        refresh_thread_.join();
    }
}  // namespace ui
