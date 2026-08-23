#pragma once

#include <string>

namespace hypr::hyprctl
{
    std::string GetSocketPath();
    std::string SendRequest(std::string_view request, std::string_view path);
}  // namespace hypr::hyprctl
