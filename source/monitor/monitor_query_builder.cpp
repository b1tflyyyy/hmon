#include "monitor_query_builder.hpp"

#include <iterator>

namespace monitor
{
    MonitorQueryBuilder::MonitorQueryBuilder(const Monitor& monitor) : query_(""), monitor_(monitor)
    {
        query_.reserve(128);
        std::format_to(std::back_inserter(query_), "eval hl.monitor({{output=\"{}\"", monitor_.name_);
    }

    MonitorQueryBuilder& MonitorQueryBuilder::WithMode(std::optional<std::string_view> mode)
    {
        if (mode.has_value())
        {
            std::format_to(std::back_inserter(query_), ", mode=\"{}\"", mode.value());
        }
        else
        {
            std::format_to(std::back_inserter(query_), ", mode=\"{}x{}@{:.2f}\"", monitor_.width_, monitor_.height_,
                           monitor_.refresh_rate_);
        }

        return *this;
    }

    MonitorQueryBuilder& MonitorQueryBuilder::WithPosition(std::optional<std::uint32_t> x_pos, std::optional<std::uint32_t> y_pos)
    {
        std::uint32_t x = x_pos.value_or(monitor_.x_pos_);
        std::uint32_t y = y_pos.value_or(monitor_.y_pos_);

        std::format_to(std::back_inserter(query_), ", position=\"{}x{}\"", x, y);
        return *this;
    }

    MonitorQueryBuilder& MonitorQueryBuilder::WithScale(std::optional<double> scale)
    {
        double sc = scale.value_or(monitor_.scale_);
        std::format_to(std::back_inserter(query_), ", scale={:.1f}", sc);

        return *this;
    }

    MonitorQueryBuilder& MonitorQueryBuilder::WithDisabled(std::optional<bool> disabled)
    {
        bool dis = disabled.value_or(monitor_.disabled_);
        std::format_to(std::back_inserter(query_), ", disabled={}", dis);

        return *this;
    }

    std::string MonitorQueryBuilder::Build()
    {
        query_ += "})";
        return query_;
    }
}  // namespace monitor
