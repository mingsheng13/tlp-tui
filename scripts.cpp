#include "scripts.h"


std::string getOutputFromStream(noshell::istream& stream)
{
    std::string buff;
    std::string line;
    while(getline(stream, buff))
    {
        line += buff + "\n";
    }
    stream.close();
    return line;
}

TLP getTLPMode()
{
    noshell::istream is;
    noshell::Exit e= "sudo"_C("tlp-stat", "-s") | "grep"_C("Mode") | is;
    std::string output = getOutputFromStream(is);

    return output.find("AC") != std::string::npos ? AC : BAT;
}

int getCurrentThreshold()
{
    noshell::istream is;
    noshell::Exit e = "cat"_C("/sys/class/power_supply/BAT0/charge_control_end_threshold") | is;
    std::string threshold = getOutputFromStream(is);
    return std::stoi(threshold);
}

void setTLPMode(TLP mode)
{
    if(mode == AC)
        noshell::Exit e = "sudo"_C("tlp", "start") > "/dev/null";
    else
        noshell::Exit e = "sudo"_C("tlp", "bat") > "/dev/null";
}

void setThreshold(int val)
{
    noshell::Exit e = "sudo"_C("tlp", "setcharge", "0", std::to_string(val), "BAT0") > "/dev/null";
}