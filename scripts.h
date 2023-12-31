#ifndef SETTINGS_TUI_SCRIPTS_H
#define SETTINGS_TUI_SCRIPTS_H

#include<iostream>
#include<string>
#include <unistd.h>
#include "noshell.hpp"

enum TLP {AC = 0, BAT = 1};

bool isRoot();

TLP getTLPMode();

int getCurrentThreshold();

void setTLPMode(TLP mode);

void setThreshold(int val);

#endif //SETTINGS_TUI_SCRIPTS_H
