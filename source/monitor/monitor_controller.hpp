#pragma once

#include <concepts>
#include <cstdint>
#include <mutex>
#include <optional>

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
            template <std::convertible_to<UintType>... Flags>
                requires(sizeof...(Flags) > 0)
            explicit RefreshStatus(Flags... flags)
            {
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
        void           SetScale(std::uint64_t monitor_id, double scale);

        /*
         *** Thread Safe ***
         */
        Monitor GetMonitor(std::uint64_t monitor_id);

        /*
         *** Thread Safe ***
         */
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
