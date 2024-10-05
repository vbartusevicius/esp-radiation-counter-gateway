#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>
#include "Stats.h"

class Display
{
    private:
        int displayWidth;
        int displayHeight;
        int headerHeight = 16;

    public:
        Display();
        void run(Stats* stats, int page);
        void configWizardFirstStep(const char* appName);
        void configWizardSecondStep(const char* ipAddress);
    
    private:
        void pageOne(Stats* stats);
        void renderGraph(float min, float max, Stats* stats);
        void renderAxis(float min, float max, Stats* stats);
        void renderRadiationMetrics(Stats* stats);
        
        void pageTwo(Stats* stats);
        int renderNetwork(Stats* stats, int startY);
        int renderBoolStatus(const char* name, bool status, int startY);
        int renderUptime(Stats* stats, int startY);

        void configWizard(const char* header, const char* helpLineOne, const char* helpLineTwo);
};

#endif