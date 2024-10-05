#include "Display.h"
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, D6, D5);

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
    float min = 1.0;
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
    int yPosition = 0;
    u8g2.firstPage();
    do {
        yPosition = renderNetwork(stats, yPosition);
        yPosition = renderUptime(stats, yPosition);
        yPosition = renderBoolStatus("MQTT", stats->mqttConnected, yPosition);
    } while (u8g2.nextPage());
}

void Display::configWizardFirstStep(const char* appName)
{
    this->configWizard(
        "Welcome! (1/2)",
        "Connect to AP to config:",
        appName
    );
}

void Display::configWizardSecondStep(const char* ipAddress)
{
    this->configWizard(
        "Welcome! (2/2)",
        "Configure device on:",
        ipAddress
    );
}

void Display::configWizard(const char* header, const char* helpLineOne, const char* helpLineTwo)
{
    int cursor = 0;

    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_6x12_mf);
        cursor += ((this->headerHeight + u8g2.getAscent()) / 2);
        u8g2.drawStr(1, cursor, header);

        u8g2.setFont(u8g2_font_5x7_tr);
        int lineHeight = u8g2.getAscent() - u8g2.getDescent();
        cursor += 6;
        cursor += lineHeight;

        u8g2.drawStr(0, cursor, helpLineOne);
        cursor += lineHeight + 2;
        u8g2.drawStr(0, cursor, helpLineTwo);
    } while (u8g2.nextPage());
}

void Display::renderRadiationMetrics(Stats* stats)
{
    u8g2.setDrawColor(1);
    u8g2.setFontMode(1);
    u8g2.setFont(u8g2_font_6x12_mf);

    int ascent = u8g2.getAscent();
    int descent = u8g2.getDescent();
    int headerMiddleY = this->headerHeight / 2 + ascent / 2 - descent / 2;

    auto cpm = String(stats->cpm) + String(" CPM");
    u8g2.drawStr(1, headerMiddleY, cpm.c_str());

    auto dose = String(stats->dose, 2) + String(" µSv/h");
    auto doseTextWidth = u8g2.getUTF8Width(dose.c_str());
    u8g2.drawUTF8(this->displayWidth - doseTextWidth - 1, headerMiddleY, dose.c_str());
}

void Display::renderGraph(float min, float max, Stats* stats)
{
    u8g2.setDrawColor(1);
    u8g2.setFontMode(1);

    int chartX = 0;
    int chartY = this->headerHeight;
    int chartHeight = this->displayHeight - this->headerHeight;
    int chartWidth = this->displayWidth;

    float yMin = min / 2.0;
    float yRange = max - yMin;
    if (yRange == 0) yRange = 1;

    int xStart = this->displayWidth - stats->buffer.size();
    if (xStart < 0) xStart = 0;

    for (int i = 0; i < stats->buffer.size(); i++) {
        float scaledValue = (stats->buffer[i] - yMin) / yRange;
        if (scaledValue < 0) scaledValue = 0;
        if (scaledValue > 1) scaledValue = 1;

        int pixelHeight = floor(scaledValue * chartHeight);

        int xPos = xStart + i;
        if (xPos >= this->displayWidth) break;

        int yPos = chartY + chartHeight - pixelHeight;

        u8g2.drawVLine(xPos, yPos, pixelHeight);
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

    char maxText[10];
    dtostrf(max, 4, 2, maxText);
    int maxTextWidth = u8g2.getStrWidth(maxText);

    char minText[10];
    dtostrf(min, 4, 2, minText);
    int minTextWidth = u8g2.getStrWidth(minText);

    const char* maxGlyph = "▲";
    const char* minGlyph = "▼";

    u8g2.setFont(u8g2_font_unifont_t_symbols);
    int glyphWidth = u8g2.getUTF8Width(maxGlyph);

    u8g2.setFont(u8g2_font_5x7_tr);
    int textX = 0;
    int textY = this->headerHeight + u8g2.getAscent() + 3;
    int glyphX = textX + maxTextWidth + 2;
    int glyphY = textY + 3;

    int boxWidth = maxTextWidth + glyphWidth + 1;
    int boxHeight = u8g2.getAscent() + 2;

    u8g2.drawBox(textX, this->headerHeight + 1, boxWidth + 2, boxHeight * 2 + 4);

    u8g2.setDrawColor(1);
    u8g2.drawStr(textX, textY, maxText);
    u8g2.setFont(u8g2_font_unifont_t_symbols);
    u8g2.drawUTF8(glyphX, glyphY, maxGlyph);

    u8g2.setFont(u8g2_font_5x7_tr);

    int minTextY = textY + boxHeight + 2;
    glyphY = minTextY + 3;

    boxWidth = minTextWidth + glyphWidth + 3;

    u8g2.setDrawColor(1);
    u8g2.drawStr(textX, minTextY, minText);
    u8g2.setFont(u8g2_font_unifont_t_symbols);
    u8g2.drawUTF8(glyphX, glyphY, minGlyph);

    // X axis

    u8g2.setFont(u8g2_font_5x7_tr);
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

int Display::renderNetwork(Stats* stats, int startY)
{
    int signalStrength = atoi(stats->wifiSignal);

    const unsigned int NO_SIGNAL_GLYPH = 57887;
    const unsigned int POOR_SIGNAL_GLYPH = 57888;
    const unsigned int GOOD_SIGNAL_GLYPH = 57889;
    const unsigned int VERY_GOOD_SIGNAL_GLYPH = 57890;

    unsigned int signalGlyph = NO_SIGNAL_GLYPH;
    if (signalStrength >= -95) {
        signalGlyph = POOR_SIGNAL_GLYPH;
    }
    if (signalStrength >= -85) {
        signalGlyph = GOOD_SIGNAL_GLYPH;
    }
    if (signalStrength >= -75) {
        signalGlyph = VERY_GOOD_SIGNAL_GLYPH;
    }
    if (signalStrength == 0) {
        signalGlyph = NO_SIGNAL_GLYPH;
    }

    u8g2.setFont(u8g2_font_siji_t_6x10);
    int glyphHeight = u8g2.getAscent() - u8g2.getDescent();

    int yPosition = startY;

    u8g2.drawGlyph(
        this->displayWidth - 10,
        yPosition + glyphHeight,
        signalGlyph
    );

    char ipLine[32];
    snprintf(ipLine, sizeof(ipLine), "IP: %s", stats->ipAddress);

    char ssidLine[32];
    snprintf(ssidLine, sizeof(ssidLine), "SSID: %s", stats->network);

    int lineHeight = u8g2.getAscent() - u8g2.getDescent() + 2;

    yPosition += lineHeight;
    u8g2.setFont(u8g2_font_6x12_mf);
    u8g2.drawStr(0, yPosition, ipLine);

    yPosition += lineHeight;
    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.drawStr(0, yPosition, ssidLine);

    yPosition += 2;
    u8g2.drawHLine(0, yPosition, this->displayWidth);

    return yPosition;
}

int Display::renderUptime(Stats* stats, int startY)
{
    u8g2.setFont(u8g2_font_5x7_tr);
    int lineHeight = u8g2.getAscent() - u8g2.getDescent() + 2;

    int yPosition = startY + lineHeight;
    u8g2.drawStr(0, yPosition, "Uptime:");

    u8g2.drawStr(49, yPosition, stats->uptime);

    return yPosition;
}

int Display::renderBoolStatus(const char* name, bool status, int startY)
{
    u8g2.setFont(u8g2_font_5x7_tr);
    int lineHeight = u8g2.getAscent() - u8g2.getDescent() + 2;

    char label[20];
    snprintf(label, sizeof(label), "%s:", name);

    int yPosition = startY + lineHeight;

    u8g2.drawStr(0, yPosition, label);

    const char* statusGlyph = status ? "[+]" : "[ ]";
    int glyphWidth = u8g2.getStrWidth(statusGlyph);
    u8g2.drawStr(
        (this->displayWidth / 2) - glyphWidth,
        yPosition,
        statusGlyph
    );

    return yPosition;
}