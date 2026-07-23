add_library(monitor_model STATIC monitor.hpp monitor.cpp)
target_link_libraries(monitor_model PUBLIC nlohmann_json)

add_library(monitor_controller STATIC monitor_controller.hpp monitor_controller.cpp)
target_link_libraries(monitor_controller PUBLIC monitor_model hyprctl)
