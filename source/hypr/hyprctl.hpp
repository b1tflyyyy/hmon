#pragma once

#include <string>

namespace hyprctl
{
    std::string GetSocketPath();
    std::string SendRequest(std::string_view request, std::string_view path);
}  // namespace hyprctl
