#include "monitor_controller.hpp"

#include <hypr/config_generator.hpp>
#include <hypr/hyprctl.hpp>
#include <mutex>

#include "monitor.hpp"
#include "monitor_query_builder.hpp"

namespace monitor
{
    MonitorController::MonitorController() : socket_path_(hypr::hyprctl::GetSocketPath()) {}

    MonitorController::RefreshStatusT MonitorController::Refresh()
    {
        std::vector<Monitor> new_monitors{};
        new_monitors.reserve(8);

        try
        {
            std::string response = hypr::hyprctl::SendRequest("j/monitors all", socket_path_);
            new_monitors         = Monitor::ParseMonitors(response);
        }
        // TODO
        catch (...)
        {
            // TODO: log...
            throw;
        }

        std::lock_guard lock(mut_);

        RefreshStatusT status = CompareMonitors(new_monitors, monitors_);
        if (!status.HasFlag(RefreshStatusT::kNothingChanged))
        {
            monitors_ = std::move(new_monitors);
        }

        return status;
    }

    void MonitorController::Toggle(std::uint64_t monitor_id)
    {
        Monitor monitor = GetMonitor(monitor_id);

        // clang-format off
        auto query = MonitorQueryBuilder(monitor).WithMode()
                                                 .WithPosition()
                                                 .WithScale()
                                                 .WithDisabled(!monitor.disabled_)
                                                 .Build();
        // clang-format on
        hypr::hyprctl::SendRequest(query, socket_path_);
    }

    void MonitorController::SetMode(std::uint64_t monitor_id, std::string_view mode)
    {
        Monitor monitor = GetMonitor(monitor_id);

        // clang-format off
        auto query = MonitorQueryBuilder(monitor).WithMode(mode)
                                                 .WithPosition()
                                                 .WithScale()
                                                 .WithDisabled()
                                                 .Build();
        // clang-format on
        hypr::hyprctl::SendRequest(query, socket_path_);
    }

    // NOLINTNEXTLINE
    void MonitorController::SetScale(std::uint64_t monitor_id, double scale)
    {
        Monitor monitor = GetMonitor(monitor_id);

        // clang-format off
        auto query = MonitorQueryBuilder(monitor).WithMode()
                                                 .WithPosition()
                                                 .WithScale(scale)
                                                 .WithDisabled()
                                                 .Build();
        // clang-format on
        hypr::hyprctl::SendRequest(query, socket_path_);
    }

    std::string MonitorController::GenerateConfig()
    {
        auto monitors = Monitors();
        return hypr::cfg_autogen::GenerateLua(monitors);
    }

    Monitor MonitorController::GetMonitor(std::uint64_t monitor_id)
    {
        std::lock_guard lock(mut_);

        /*
         * Note: possible UB, if element with monitor_id does not exist.
         */
        auto it = std::ranges::find(monitors_, monitor_id, &Monitor::id_);
        return *it;
    }

    std::vector<Monitor> MonitorController::Monitors()
    {
        std::lock_guard lock(mut_);
        return monitors_;
    }

    MonitorController::RefreshStatusT MonitorController::CompareMonitors(const std::vector<Monitor>& lhs, const std::vector<Monitor>& rhs)
    {
        if (std::size(lhs) != std::size(rhs))
        {
            return RefreshStatusT(RefreshStatusT::kMonitorsCountOrNameChanged, RefreshStatusT::kMonitorInfoChanged);
        }

        RefreshStatusT status;
        for (std::size_t i = 0; i < std::size(lhs); ++i)
        {
            const auto& left  = lhs[i];
            const auto& right = rhs[i];

            bool inf_chng         = status.HasFlag(RefreshStatusT::kMonitorInfoChanged);
            bool cnt_or_name_chng = status.HasFlag(RefreshStatusT::kMonitorsCountOrNameChanged);

            if (inf_chng && cnt_or_name_chng)
            {
                break;
            }

            if (!cnt_or_name_chng && (left.name_ != right.name_))
            {
                status.SetFlag(RefreshStatusT::kMonitorsCountOrNameChanged);
            }
            else if (!inf_chng && (left != right))
            {
                status.SetFlag(RefreshStatusT::kMonitorInfoChanged);
            }
        }

        return status;
    }
}  // namespace monitor
