#include "monitor_box_preview.hpp"

namespace ui
{
    // NOLINTNEXTLINE
    MonitorPreviewBox::MonitorPreviewBox(std::uint32_t width, std::uint32_t height) : width_(width), height_(height) {}

    ftxui::Element MonitorPreviewBox::Render() const
    {
        static constexpr double kPixelsPerCol = 20.0;

        std::uint32_t cols = static_cast<std::uint32_t>(width_ / kPixelsPerCol);
        std::uint32_t rows = static_cast<std::uint32_t>(height_ / kPixelsPerCol / 2.0);

        std::string label = std::format("{}x{} [mm]", width_, height_);

        // clang-format off
        ftxui::Element inner = ftxui::vbox({ftxui::text(label) | ftxui::center})
            | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, cols)
            | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, rows)
            | ftxui::border;
        // clang-format on

        return ftxui::window(ftxui::text(" Monitor Preview "), inner | ftxui::center);
    }
}  // namespace ui
