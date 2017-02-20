// Coding Convention : Camel case

#include "WDDS.h"

int main(int argc, char *argv[]) {
    WDDS wdds("wlan0", 1, "Config.ini");
    wdds.start();
    return 0;
}