#ifndef NOENGINE
#include "animation_programming.h"
#else
#include "ui_window.h"
#endif

int main()
{
#ifndef NOENGINE
    CSimulation simulation;

    Run(&simulation, 1400, 800);
#else
    UiWindow window;
#endif

    return 0;
}
