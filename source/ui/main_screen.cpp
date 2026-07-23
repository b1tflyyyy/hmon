#include "main_screen.hpp"

#include <chrono>
#include <ftxui/component/component.hpp>

#include "custom_events.hpp"
#include "monitor_tabs_view.hpp"

namespace ui
{
    int MainScreen::Run()
    {
        controller_.Refresh();
        MonitorTabsView tabs_view(controller_);

        auto screen = ftxui::ScreenInteractive::Fullscreen();
        StartRefreshThread(screen);

        // clang-format off
        auto component = ftxui::CatchEvent(tabs_view.Component(), [&](ftxui::Event event) -> bool {
            if (event == ftxui::Event::Character('q'))
            {
                screen.ExitLoopClosure()();
                return true;
            } 

            if (event == ftxui::Event::Character('r'))  
            {
                // TODO: referesh
            }

            return false;
        });
        // clang-format on

        screen.Loop(component);
        StopRefreshThread();

        return 0;
    }

    void MainScreen::StartRefreshThread(ftxui::ScreenInteractive& screen)
    {
        refresh_running_.store(true);

        // clang-format off
        refresh_thread_ = std::thread([this, &screen]{ // TODO: ...
            using RefreshStatus = monitor::MonitorController::RefreshStatusT;
    
            while(refresh_running_.load()) 
            {
                std::this_thread::sleep_for(std::chrono::seconds(kRefreshInterval));                
                RefreshStatus status = controller_.Refresh();

                if (status.HasFlag(RefreshStatus::kNothingChanged)) 
                {
                    continue;
                } 
                
                if (status.HasFlag(RefreshStatus::kMonitorsCountOrNameChanged)) 
                {
                    screen.PostEvent(custom_events::kMonitorsCountOrNameChanged);
                }
                if (status.HasFlag(RefreshStatus::kMonitorInfoChanged)) 
                {
                    screen.PostEvent(custom_events::kMonitorInfoChanged);
                }
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
