
#include <stdbool.h>
#include <stdio.h>

#include "engine.h"



int main(int argc, char *argv[])
{
    struct EngineInfo info;

    if (initEngine(&info) != 0)
        return 1;

    while (info.running) {
        stepFrame(&info);
    }

    quitEngine(&info);

    return 0;
}
