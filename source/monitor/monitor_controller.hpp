#pragma once

#include <concepts>
#include <cstdint>
#include <mutex>

#include "monitor.hpp"

namespace monitor
{
    class MonitorController
    {
    public:
        template <std::unsigned_integral T>
        class RefreshStatus
        {
            friend class MonitorController;

        public:
            using UintType = T;

            RefreshStatus() = default;

            static constexpr UintType kNothingChanged             = 1;
            static constexpr UintType kMonitorInfoChanged         = 2;
            static constexpr UintType kMonitorsCountOrNameChanged = 4;

            bool HasFlag(UintType flag) { return static_cast<bool>(status_ & flag); }

        private:
            // TODO: Rewr
            template <std::convertible_to<UintType> AtLeastOneFlag, std::convertible_to<UintType>... Flags>
            RefreshStatus(AtLeastOneFlag f1, Flags... flags)
            {
                status_ |= f1;
                ((status_ |= flags), ...);
            }

            void SetFlag(UintType flag) { status_ |= flag; }

        private:
            UintType status_{};
        };

        using RefreshStatusT = RefreshStatus<std::uint16_t>;

    public:
        MonitorController();

        RefreshStatusT Refresh();
        void           Toggle(std::uint64_t monitor_id);
        void           SetMode(std::uint64_t monitor_id, std::string_view mode);
        void           SetVrr(std::uint64_t monitor_id, bool value);

        Monitor GetMonitor(std::uint64_t monitor_id);

        std::vector<Monitor> Monitors();

    private:
        static RefreshStatusT CompareMonitors(const std::vector<Monitor>& lhs, const std::vector<Monitor>& rhs);

    private:
        std::string socket_path_;

        // *** Guarded by mut_ ***
        std::vector<Monitor> monitors_;
        mutable std::mutex   mut_;
    };
}  // namespace monitor
