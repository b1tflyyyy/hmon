#include "monitor_tabs_view.hpp"

#include <chrono>
#include <ftxui/component/animation.hpp>
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>

#include "custom_events.hpp"

namespace ui
{
    MonitorTabsView::MonitorTabsView(monitor::MonitorController& controller) : controller_(controller), active_tab_(0) { Init(); }

    ftxui::Component MonitorTabsView::Component() const { return root_; }

    void MonitorTabsView::Init()
    {
        container_ = ftxui::Container::Vertical({});
        RefreshTabs();

        // clang-format off
        auto event_handler = ftxui::CatchEvent(container_, [this](ftxui::Event event) {
            if (event == custom_events::kMonitorsCountOrNameChanged)
            {
                RefreshTabs();
                return true;
            }                                               
    
            return false;
        });

        root_ = ftxui::Renderer(event_handler, [this] -> ftxui::Element { return container_->Render(); });
        // clang-format on
    }

    void MonitorTabsView::RefreshTabs()
    {
        auto monitors = controller_.Monitors();

        container_->DetachAllChildren();
        tab_lables_.clear();
        panels_.clear();

        if (monitors.empty())
        {
            return;
        }

        ftxui::Components panel_components;
        for (const auto& monitor : monitors)
        {
            tab_lables_.push_back(std::format("{} [{}]", monitor.name_, monitor.id_));
            panels_.push_back(std::make_unique<MonitorPanelView>(monitor.id_, controller_));
            panel_components.push_back(panels_.back()->Component());
        }

        if (active_tab_ >= static_cast<int>(std::size(monitors)))
        {
            active_tab_ = 0;
        }

        static ftxui::MenuOption tab_opt = ftxui::MenuOption::Toggle();
        tab_opt.underline.enabled        = true;
        tab_opt.underline.SetAnimation(std::chrono::milliseconds(150), ftxui::animation::easing::QuinticInOut);

        auto tab_menu    = ftxui::Menu(&tab_lables_, &active_tab_, tab_opt);
        auto tab_content = ftxui::Container::Tab(panel_components, &active_tab_);

        container_->Add(tab_menu);
        container_->Add(tab_content);
    }
}  // namespace ui
