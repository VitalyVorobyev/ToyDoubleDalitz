/** Copyright 2016 Vitaly Vorobyev
 **
 **/

#include "./toytools.h"

int main(int argc, char** argv) {
    ProgOpt popt;
    if (popt.Parse(argc, argv)) return -1;

    ToyTools driver(popt);
    driver.Run();

    return 0;
}
