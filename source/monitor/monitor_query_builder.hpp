#pragma once

#include <format>
#include <optional>
#include <string>

#include "monitor.hpp"

namespace monitor
{
    class MonitorQueryBuilder
    {
    public:
        explicit MonitorQueryBuilder(const Monitor& monitor);

        MonitorQueryBuilder& WithMode(std::optional<std::string_view> mode = std::nullopt);
        MonitorQueryBuilder& WithPosition(std::optional<std::uint32_t> x_pos = std::nullopt,
                                          std::optional<std::uint32_t> y_pos = std::nullopt);

        MonitorQueryBuilder& WithScale(std::optional<double> scale = std::nullopt);
        MonitorQueryBuilder& WithDisabled(std::optional<bool> disabled = std::nullopt);

        std::string Build();

    private:
        std::string    query_;
        const Monitor& monitor_;
    };
}  // namespace monitor
