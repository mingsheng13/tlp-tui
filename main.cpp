#include <iostream>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

#include "scripts.h"

using namespace ftxui;

Component tlptui()
{
    class Impl : public ComponentBase
    {
    private:
        int threshold;
        std::string mode;
        Element tlpInfo;
        Element thresholdInfo;
        Component tlpBox;
        std::vector<std::string> tlpSelection = {"AC", "BAT"};
        int selected = 0;
        Component thresholdSlider;
        Component container;
    public:
        Impl()
        {
            updateTLP();
            updateThreshold();
            tlpBox = Radiobox(&tlpSelection, &selected);
            thresholdSlider = Slider("Threshold", &threshold, 0);
            container = Container::Horizontal({tlpBox, thresholdSlider});
            Add(container);
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
            auto controlTitle = text("control") | center;
            return vbox(
            {
                hbox(
                {
                    window(infoTitle, vbox(
                    {
                        hbox({text("TLP mode:       "), tlpInfo}),
                        hbox({text("Bat threshold:  "), thresholdInfo})
                    })),
                    window(controlTitle, vbox(
                    {
                        text("Press A to apply settings"),
                        text("press Q to exit")
                    }))
                }),
                vbox(
                {
                    container->Render()
                })
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
    screen.Loop(tlptui());

    return 0;
}





