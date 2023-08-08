#include <memory>  // for allocator, __shared_ptr_access
#include <string>  // for string, basic_string, operator+, to_string
#include <vector>  // for vector

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

#include "scripts.h"

using namespace ftxui;

Component wrap(const std::string& name, const Component& component)
{
    return Renderer(component, [=]
    {
        return hbox(
                {
                    text("  "+name) | size(WIDTH, EQUAL, 15),
                    component->Render()
                });
    });
}

Component updateTLP()
{
    Element tlp_stat = getTLPMode() == AC ? text("AC") : text("BAT");
    return wrap("TLP mode:", Renderer([=]{return tlp_stat;}));
}

Component updateThreshold()
{
    int threshold = getCurrentThreshold();
    return wrap("Threshold:", Renderer([=]{return text(std::to_string(threshold)+"%");}));
}

int main() {
    auto screen = ScreenInteractive::TerminalOutput();

    // --TLP-----
    auto tlpInfo = updateTLP();

    std::vector<std::string> tlp_list = {"AC", "BAT"};
    int tlp_selected = getTLPMode();

    auto tlp_toggle = Toggle(&tlp_list, &tlp_selected);
    tlp_toggle = wrap("TLP:", tlp_toggle);

    // --charge-threshold-----
    auto threshold_info = updateThreshold();
    int current_threshold = getCurrentThreshold();

    auto threshold_slider = Slider("", &current_threshold);
    threshold_slider = wrap("Threshold:", threshold_slider);

    //------------------------
    auto container = Container::Vertical(
            {
                    tlpInfo,
                    tlp_toggle,
                    threshold_info,
                    threshold_slider
            });

    auto layout = Renderer(container, [&]
        {
            return vbox(
                    {
                        window(text("INFO") | center | color(Color::Green),
                               vbox(tlpInfo->Render(),
                                    threshold_info->Render())),

                        text(""),

                        window(text("CONTROL") | center | color(Color::Red),
                               vbox(separatorEmpty(),
                                    tlp_toggle->Render(),
                                    hbox(threshold_slider->Render() | flex,
                                         text(std::to_string(current_threshold)+"%")),
                                    separatorEmpty())),

                    }) | size(WIDTH, EQUAL, 50);
        });

    auto event = CatchEvent(layout, [&](const Event& e)
    {
        if (e == Event::Character('q'))
        {
            screen.ExitLoopClosure()();
            return true;
        }
        else if (e == Event::Character('a'))
        {
            tlp_selected == AC ? setTLPMode(AC) : setTLPMode(BAT);
            setThreshold(current_threshold);

            tlpInfo = updateTLP();
            threshold_info = updateThreshold();
            return true;
        }
        return false;
    });

    screen.Loop(event);

    return 0;
}













