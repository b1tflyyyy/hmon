#include "hyprctl.hpp"

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <cstdlib>
#include <format>

namespace hyprctl
{
    std::string GetSocketPath()
    {
        const char* hypr_inst_sig = std::getenv("HYPRLAND_INSTANCE_SIGNATURE");
        const char* rt_dir        = std::getenv("XDG_RUNTIME_DIR");

        if (hypr_inst_sig == nullptr || rt_dir == nullptr)
        {
            throw std::runtime_error("You are not in Hyprland session.");
        }

        return std::format("{}/hypr/{}/.socket.sock", rt_dir, hypr_inst_sig);
    }

    // NOLINTNEXTLINE
    std::string SendRequest(std::string_view request, std::string_view path)
    {
        int fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (fd == -1)
        {
            throw std::runtime_error("failed to create socket");
        }

        sockaddr_un addr{};

        addr.sun_family = AF_UNIX;
        std::ranges::copy(path, addr.sun_path);

        if (connect(fd, reinterpret_cast<sockaddr*>(std::addressof(addr)), sizeof(addr)) == -1)
        {
            close(fd);  // todo: remove
            throw std::runtime_error("failed to connect to socket");
        }

        if (write(fd, std::data(request), std::size(request)) == -1)
        {
            close(fd);  // todo:
            throw std::runtime_error("failed to write to socket");
        }

        std::array<char, 4096> buffer{};
        if (read(fd, std::data(buffer), std::size(buffer)) == -1)
        {
            throw std::runtime_error("failed to read response from socket");
        }

        close(fd);
        return std::string(std::from_range, buffer);
    }
}  // namespace hyprctl
