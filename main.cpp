#include <iostream>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

#include "scripts.h"

using namespace ftxui;

Component tlptui()
{
    const int INFOWIDTH = 25;
    class Impl : public ComponentBase
    {
    private:
        int threshold;
        std::string mode;
        Element tlpInfo;
        Element thresholdInfo;
        Component tlpBox;
        std::vector<std::string> tlpSelection = {"AC", "BAT"};
        int selected;
        Component thresholdSlider;
        Component thresholdRenderer;
        std::vector<std::string> tab_values = {"TLP mode", "Threshold"};
        int tabSelected = 0;
        Component menu;
        Component tabContainer;
        Component horizontalContainer;
    public:
        Impl()
        {
            updateTLP();
            updateThreshold();
            threshold = getCurrentThreshold();
            getTLPMode() == AC ? selected = 0 : selected = 1;
            tlpBox = Radiobox(&tlpSelection, &selected);
            thresholdSlider = Slider("Threshold: ", &threshold, 0) | size(WIDTH, EQUAL, 30);
            thresholdRenderer = Renderer(thresholdSlider, [&]
            {
                return vbox(
                {
                    separatorEmpty(),
                    hbox(
                    {
                        thresholdSlider->Render(), text(std::to_string(threshold))
                    })
                });
            });
            menu = Menu(&tab_values, &tabSelected) | size(WIDTH, EQUAL, 15);
            tabContainer = Container::Tab(
            {
                tlpBox,
                thresholdRenderer,
            }, &tabSelected);
            horizontalContainer = Container::Horizontal(
            {
                menu,
                tabContainer
            });
            Add(horizontalContainer);
        }

        bool updateTLP()
        {
            if (getTLPMode() == AC)
            {
                mode = "AC";
            }
            else
            {
                mode = "BAT";
            }
            tlpInfo = text(mode) | color(Color::Red);
            return true;
        }

        bool updateThreshold()
        {
            threshold = getCurrentThreshold();
            thresholdInfo = text(std::to_string(threshold));
            return true;
        }

        bool OnEvent(Event event) override
        {
            if (event.is_character())
            {
                if (event.character() == "a")
                {
                    setThreshold(threshold);
                    if (selected == 0)
                        setTLPMode(AC);
                    else
                        setTLPMode(BAT);
                    updateTLP();
                    updateThreshold();
                    return true;
                }
            }
            return ComponentBase::OnEvent(event);
        }

        Element Render() override
        {
            //info area
            auto infoTitle = text("Info") | center;
            auto navTitle = text("navigation") | center;
            auto controlTitle = text("control") | center;
            return vbox(
            {
                hbox(
                {
                    window(infoTitle, vbox(
                    {
                        hbox({text("TLP mode:       "), tlpInfo}),
                        hbox({text("Bat threshold:  "), thresholdInfo})
                    })) |size(WIDTH, GREATER_THAN, INFOWIDTH),
                    window(navTitle, vbox(
                    {
                        text("Press A to apply settings"),
                        text("Press Q to exit")
                    }))
                }),
                window(controlTitle, hbox(
                {
                    menu->Render(),
                    separator(),
                    tabContainer->Render()
                })) | size(HEIGHT, EQUAL, 5)
            });
        }
        bool Focusable() const override
        {
            return true;
        }
    };
    return Make<Impl>();
}

int main()
{
    if (!isRoot())
    {
        std::cout << "Root required! Please run as sudo.\n";
        return 1;
    }
    auto screen = ScreenInteractive::FitComponent();
    auto comp = tlptui();
    auto component = CatchEvent(comp, [&](Event event) {
        if (event == Event::Character('q')) {
            screen.ExitLoopClosure()();
            return true;
        }
        return false;
    });
    screen.Loop(component);

    return 0;
}





