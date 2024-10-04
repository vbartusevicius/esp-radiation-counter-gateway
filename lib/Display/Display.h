#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>
#include "Stats.h"

class Display
{
    private:
        int cursorOffset = 0;
        int displayWidth;
        int displayHeight;
        int headerHeight = 16;

    public:
        Display();
        void run(Stats* stats);
        void displayFirstStep(const char* appName);
        void displaySecondStep(const char* ipAddress);
    
    private:
        void renderGraph(Stats* stats);
        void renderNetwork(Stats* stats);
        void renderBoolStatus(String name, bool status);
        void renderUptime(Stats* stats);
};

#endif