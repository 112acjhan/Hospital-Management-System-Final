#include <iostream>

using namespace std;

#pragma once


//ANSI escape codes for text style
const string BOLD = "\x1b[1m";
const string ITALIC = "\x1b[3m";
const string UNDERLINE = "\x1b[4m";
const string SLOW_BLINK = "\x1b[5m";
const string RAPID_BLINK = "\x1b[6m";

// ANSI escape codes for text colors
const string BLACK_TEXT = "\x1b[30m";
const string RED_TEXT = "\x1b[31m";
const string GREEN_TEXT = "\x1b[32m";
const string YELLOW_TEXT = "\x1b[33m";
const string BLUE_TEXT = "\x1b[34m";
const string MAGENTA_TEXT = "\x1b[35m";
const string CYAN_TEXT = "\x1b[36m";
const string WHITE_TEXT = "\x1b[37m";
const string GRAY_TEXT = "\x1b[38;5;8m";
const string BRED_TEXT = "\x1b[91m";
const string BGREEN_TEXT = "\x1b[92m";
const string BYELLOW_TEXT = "\x1b[93m";
const string BBLUE_TEXT = "\x1b[94m";
const string BMAGENTA_TEXT = "\x1b[95m";
const string BCYAN_TEXT = "\x1b[96m";
const string BWHITE_TEXT = "\x1b[97m";

// ANSI escape codes for background colors
const string RED_BG = "\x1b[41m";
const string GREEN_BG = "\x1b[42m";
const string YELLOW_BG = "\x1b[43m";
const string BLUE_BG = "\x1b[44m";
const string MAGENTA_BG = "\x1b[45m";
const string CYAN_BG = "\x1b[46m";
const string WHITE_BG = "\x1b[47m";
const string BBLACK_BG = "\x1b[100m";
const string BRED_BG = "\x1b[101m";
const string BGREEN_BG = "\x1b[102m";
const string BYELLOW_BG = "\x1b[103m";
const string BBLUE_BG = "\x1b[104m";
const string BMAGENTA_BG = "\x1b[105m";
const string BCYAN_BG = "\x1b[106m";
const string RESET = "\x1b[0m"; // Reset
