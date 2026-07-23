#pragma once

#include <ftxui/dom/elements.hpp>

namespace ui
{
    class MonitorPreviewBox
    {
    public:
        MonitorPreviewBox(std::uint32_t width, std::uint32_t height);

        ftxui::Element Render() const;

    private:
        std::uint32_t width_;
        std::uint32_t height_;
    };
}  // namespace ui
