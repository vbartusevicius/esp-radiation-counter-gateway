#include "Display.h"
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 22, 21, U8X8_PIN_NONE);

Display::Display()
{
    u8g2.begin();

    this->displayWidth = u8g2.getDisplayWidth();
    this->displayHeight = u8g2.getDisplayHeight();
}

void Display::run(Stats* stats, int page)
{
    if (page == 0) {
        return this->pageOne(stats);
    }
    if (page == 1) {
        return this->pageTwo(stats);
    }
}

void Display::pageOne(Stats* stats)
{
    float max = 0.0;
    float min = 1000;
    for (auto &element : stats->buffer) {
        if (element > max) {
            max = element;
        }
        if (element < min) {
            min = element;
        }
    }

    u8g2.firstPage();
    do {
        this->renderRadiationMetrics(stats);
        this->renderGraph(min, max, stats);
        this->renderAxis(min, max, stats);
    } while (u8g2.nextPage());
}

void Display::pageTwo(Stats* stats)
{

}

void Display::displayFirstStep(const char* appName)
{
    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_6x10_tr);
        u8g2.drawStr(
            0, 
            this->cursorOffset + 14,
            "Welcome! (1/2)"
        );
        u8g2.setFont(u8g2_font_5x7_tr);

        u8g2.drawStr(
            0, 
            this->cursorOffset + 28,
            "Connect to AP:"
        );
        u8g2.drawStr(
            0, 
            this->cursorOffset + 38,
            appName
        );
    } while (u8g2.nextPage());
}

void Display::displaySecondStep(const char* ipAddress)
{
    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_6x10_tr);
        u8g2.drawStr(
            0, 
            this->cursorOffset + 14,
            "Welcome! (2/2)"
        );
        u8g2.setFont(u8g2_font_5x7_tr);

        u8g2.drawStr(
            0, 
            this->cursorOffset + 28,
            "Configure device on:"
        );
        u8g2.drawStr(
            0, 
            this->cursorOffset + 38,
            ipAddress
        );
    } while (u8g2.nextPage());
}

void Display::renderRadiationMetrics(Stats* stats)
{
    u8g2.setDrawColor(1);
    u8g2.setFontMode(1);
    u8g2.setFont(u8g2_font_6x12_mf);

    auto hMiddle = ceil(this->headerHeight - u8g2.getAscent() - u8g2.getDescent());

    auto cpm = String(stats->cpm) + String(" CPM");
    u8g2.drawStr(1, hMiddle, cpm.c_str());

    auto dose = String(stats->dose, 2) + String(" µSv/h");
    auto doseTextWidth = u8g2.getStrWidth(dose.c_str());
    u8g2.drawUTF8(this->displayWidth - doseTextWidth, hMiddle, dose.c_str());
}

void Display::renderGraph(float min, float max, Stats* stats)
{
    u8g2.setDrawColor(1);
    u8g2.setFontMode(1);

    int chartHeight = this->displayHeight - this->headerHeight;
    float pixelRatio = (float) chartHeight / max;

    for (int i = 0; i < stats->buffer.size(); i++) {
        int lineHeight = floor(stats->buffer[i] * pixelRatio);
        u8g2.drawVLine(
            i + (this->displayWidth - stats->buffer.size()),
            this->displayHeight - lineHeight,
            lineHeight
        );
    }

}

void Display::renderAxis(float min, float max, Stats* stats)
{
    // Y axis
    
    for (int i = 0; i < this->displayWidth; i++) {
        if (i % 4 == 0) {
            u8g2.drawPixel(i, this->headerHeight);
        }
    }

    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.setDrawColor(0);

    auto maxText = String(max, 2);
    auto maxTextWidth = u8g2.getStrWidth(maxText.c_str());

    u8g2.drawBox(0, this->headerHeight + 1, maxTextWidth + 1, u8g2.getAscent() + 4);
    u8g2.setDrawColor(1);
    u8g2.drawStr(0, this->headerHeight + u8g2.getAscent() + 3, maxText.c_str());

    // X axis

    String duration = "";
    float span = 0.0;
    float maxSpanInSeconds = (stats->spanSize * this->displayWidth) * 1.0;
    if ((maxSpanInSeconds / 60) < 60) {
        duration = "min";
        span = maxSpanInSeconds / 60;
    } else if ((maxSpanInSeconds / (60 * 60)) < 24) {
        duration = "h";
        span = maxSpanInSeconds / (60 * 60);
    } else {
        duration = "d";
        span = maxSpanInSeconds / (60 * 60 * 24);
    }

    u8g2.setDrawColor(2);
    auto textZero = String("-0") + duration;
    auto textZeroWidth = u8g2.getStrWidth(textZero.c_str());
    u8g2.drawStr(this->displayWidth - textZeroWidth - 1, this->displayHeight - abs(u8g2.getDescent()), textZero.c_str());

    for (int i = this->headerHeight + 1; i < this->displayHeight - u8g2.getAscent() - 2; i++) {
        if (i % 4 == 0) {
            u8g2.drawPixel(floor(this->displayWidth / 2), i);
        }
    }

    auto textHalf = String("-") + String(span / 2, 0) + duration;
    auto textHalfWidth = u8g2.getStrWidth(textHalf.c_str());
    u8g2.drawStr(floor((this->displayWidth - textHalfWidth) / 2), this->displayHeight - abs(u8g2.getDescent()), textHalf.c_str());

    auto textFull = String("-") + String(span, 0) + duration;
    u8g2.drawStr(1, this->displayHeight - abs(u8g2.getDescent()), textFull.c_str());
}

void Display::renderNetwork(Stats* stats)
{
    auto signalStrength = atoi(stats->wifiSignal.c_str());
    // no signal
    unsigned int signalGlyph = 57887;
    // poor
    if (signalStrength >= -95) {
        signalGlyph = 57888;
    }
    // good
    if (signalStrength >= -85) {
        signalGlyph = 57889;
    }
    // very good
    if (signalStrength >= -75) {
        signalGlyph = 57890;
    }

    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(
        this->displayWidth - 10,
        this->cursorOffset + 14,
        signalGlyph
    );

    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.drawStr(
        0, 
        this->cursorOffset + 7, 
        ("SSID: " + stats->network).c_str()
    );
    u8g2.drawStr(
        0, 
        this->cursorOffset + 14 + 1, 
        ("IP: " + stats->ipAddress).c_str()
    );
    u8g2.drawHLine(0, this->cursorOffset + 16, this->displayWidth);

    this->cursorOffset += 17;
}

void Display::renderBoolStatus(String name, bool status)
{
    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.drawStr(0, this->cursorOffset + 8, (name + ":").c_str());

    const char* statusGlyph = "[ ]";
    if (status) {
        statusGlyph = "[+]";
    }

    auto glyphLength = u8g2.getStrWidth(statusGlyph);
    u8g2.drawStr(
        (int) (this->displayWidth / 2) - glyphLength,
        this->cursorOffset + 8,
        statusGlyph
    );

    this->cursorOffset += 8;
}

void Display::renderUptime(Stats* stats)
{
    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.drawStr(0, this->cursorOffset + 8, "Uptime:");

    u8g2.drawStr(
        49,
        this->cursorOffset + 8,
        stats->uptime.c_str()
    );

    this->cursorOffset += 8;
}