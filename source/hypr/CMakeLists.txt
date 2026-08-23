add_library(hyprctl STATIC hyprctl.hpp hyprctl.cpp)
add_library(config_generator STATIC config_generator.hpp config_generator.cpp)
target_link_libraries(config_generator PUBLIC monitor_model)
