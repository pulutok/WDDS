// Coding Convention : Camel case

#include "WDDS.h"

int main(int argc, char *argv[]) {
    WDDS wdds("ens33", 1, "Config.ini");
    wdds.start();
    return 0;
}