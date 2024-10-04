#include "Display.h"
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 22, 21, U8X8_PIN_NONE);

Display::Display()
{
    u8g2.begin();

    this->displayWidth = u8g2.getDisplayWidth();
    this->displayHeight = u8g2.getDisplayHeight();
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

void Display::run(Stats* stats)
{
    u8g2.firstPage();
    do {
        this->renderGraph(stats);
        // this->renderNetwork(stats);
        // this->renderBoolStatus("MQTT", stats->mqttConnected);
        // this->renderBoolStatus("Sensor", true);
        // this->renderUptime(stats);
    } while (u8g2.nextPage());
}

void Display::renderGraph(Stats* stats)
{
    u8g2.setDrawColor(1);
    u8g2.setFontMode(1);
    float max = 0.0;
    for (auto &element : stats->buffer) {
        if (element > max) {
            max = element;
        }
    }

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

    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.setDrawColor(0);

    auto maxText = String(max, 2);
    auto maxTextWidth = u8g2.getStrWidth(maxText.c_str());

    u8g2.drawBox(0, this->headerHeight, maxTextWidth + 2, u8g2.getAscent() + 2);
    u8g2.setDrawColor(1);
    u8g2.setFontPosTop();
    u8g2.drawStr(1, this->headerHeight - 1, maxText.c_str());
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