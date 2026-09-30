#include "KeyboardMap.h"

#include "imgui.h"

#include <algorithm>
#include <cmath>

struct KeyCell {
    const char *label;
    uint8_t id;
    float units;
};

static constexpr KeyCell kGap{"", 0, 0.55F};

static constexpr KeyCell kRow0[] = {
    {"Esc", 0x01, 1.3F}, {"F1", 0x02, 1},  {"F2", 0x03, 1}, {"F3", 0x04, 1},
    {"F4", 0x05, 1},     kGap,             {"F5", 0x06, 1}, {"F6", 0x07, 1},
    {"F7", 0x08, 1},     {"F8", 0x09, 1},  kGap,            {"F9", 0x0A, 1},
    {"F10", 0x0B, 1},    {"F11", 0x0C, 1}, {"F12", 0x0D, 1}, {"Del", 0x10, 1},
};

static constexpr KeyCell kRow1[] = {
    {"`", 0x15, 1},  {"1", 0x16, 1}, {"2", 0x17, 1}, {"3", 0x18, 1},
    {"4", 0x19, 1},  {"5", 0x1A, 1}, {"6", 0x1B, 1}, {"7", 0x1C, 1},
    {"8", 0x1D, 1},  {"9", 0x1E, 1}, {"0", 0x1F, 1}, {"-", 0x20, 1},
    {"=", 0x21, 1},  {"Bksp", 0x24, 2.0F},
};

static constexpr KeyCell kRow2[] = {
    {"Tab", 0x29, 1.5F}, {"Q", 0x2B, 1}, {"W", 0x2C, 1}, {"E", 0x2D, 1},
    {"R", 0x2E, 1},      {"T", 0x2F, 1}, {"Y", 0x30, 1}, {"U", 0x31, 1},
    {"I", 0x32, 1},      {"O", 0x33, 1}, {"P", 0x34, 1}, {"[", 0x35, 1},
    {"]", 0x36, 1},      {"\\", 0x38, 1.5F},
};

static constexpr KeyCell kRow3[] = {
    {"Caps", 0x3E, 1.8F}, {"A", 0x3F, 1}, {"S", 0x40, 1}, {"D", 0x41, 1},
    {"F", 0x42, 1},       {"G", 0x43, 1}, {"H", 0x44, 1}, {"J", 0x45, 1},
    {"K", 0x46, 1},       {"L", 0x47, 1}, {";", 0x48, 1}, {"'", 0x49, 1},
    {"Enter", 0x4B, 2.2F},
};

static constexpr KeyCell kRow4[] = {
    {"Shift", 0x52, 2.3F}, {"Z", 0x54, 1}, {"X", 0x55, 1}, {"C", 0x56, 1},
    {"V", 0x57, 1},        {"B", 0x58, 1}, {"N", 0x59, 1}, {"M", 0x5A, 1},
    {",", 0x5B, 1},        {".", 0x5C, 1}, {"/", 0x5D, 1}, {"Shift", 0x5F, 2.3F},
};

static constexpr KeyCell kRow5[] = {
    {"Ctrl", 0x65, 1.4F}, {"Fn", 0x66, 1},    {"Win", 0x68, 1.2F},
    {"Alt", 0x69, 1.2F},  {"Space", 0x6A, 5.2F}, {"Alt", 0x70, 1.2F},
    {"Win", 0x6E, 1.2F},  {"Ctrl", 0x71, 1.4F},
};

// Same Darfon ids as the letter keys (m16 map + 1). Mute 0x11, volume down
// 0x12, volume up 0x13, mic mute 0x14.
static constexpr KeyCell kMic{"Mic", 0x14, 1.4F};
static constexpr KeyCell kMute{"Mute", 0x11, 1.4F};
static constexpr KeyCell kVolUp{"Vol+", 0x13, 1.4F};
static constexpr KeyCell kVolDn{"Vol-", 0x12, 1.4F};
static constexpr KeyCell kUp{"Up", 0x73, 1};
static constexpr KeyCell kLeft{"Left", 0x86, 1};
static constexpr KeyCell kDown{"Down", 0x87, 1};
static constexpr KeyCell kRight{"Right", 0x88, 1};
static constexpr KeyCell kPower{"Pwr", kKeyboardPowerId, 1.4F};

struct Preview {
    int effect;
    uint8_t er, eg, eb;
    bool powerOwn;
    uint8_t pr, pg, pb;
    const uint8_t (*color)[3];
    float originX;
    float span;
    float time;
};

static float RowUnits(const KeyCell *row, int count) {
    float units = 0.0F;
    for (int i = 0; i < count; ++i)
        units += row[i].units;
    return units;
}

static void SelectRange(bool selected[256], const KeyCell *row, int count,
                        int from, int to) {
    if (from > to)
        std::swap(from, to);
    for (int i = 0; i < 256; ++i)
        selected[i] = false;
    for (int i = from; i <= to; ++i) {
        if (row[i].label[0] != '\0')
            selected[row[i].id] = true;
    }
}

static void PaletteAt(float t, uint8_t out[3]) {
    static const uint8_t pal[][3] = {
        {255, 0, 0},   {255, 165, 0}, {255, 255, 0}, {0, 128, 0},
        {0, 191, 255}, {0, 0, 255},   {128, 0, 128},
    };
    constexpr int n = 7;
    t = t - std::floor(t);
    const float x = t * static_cast<float>(n);
    const int i = static_cast<int>(x) % n;
    const int j = (i + 1) % n;
    const float f = x - std::floor(x);
    out[0] = static_cast<uint8_t>(pal[i][0] + (pal[j][0] - pal[i][0]) * f);
    out[1] = static_cast<uint8_t>(pal[i][1] + (pal[j][1] - pal[i][1]) * f);
    out[2] = static_cast<uint8_t>(pal[i][2] + (pal[j][2] - pal[i][2]) * f);
}

static void SolidLevel(uint8_t r, uint8_t g, uint8_t b, float level,
                       uint8_t out[3]) {
    level = std::clamp(level, 0.0F, 1.0F);
    out[0] = static_cast<uint8_t>(static_cast<float>(r) * level);
    out[1] = static_cast<uint8_t>(static_cast<float>(g) * level);
    out[2] = static_cast<uint8_t>(static_cast<float>(b) * level);
}

static void ColorBand(float along, float pos, uint8_t r, uint8_t g, uint8_t b,
                      uint8_t out[3]) {
    const float d = std::fabs(along - pos);
    float level = 1.0F - std::min(d / 0.18F, 1.0F);
    level = level * level;
    SolidLevel(r, g, b, 0.10F + 0.90F * level, out);
}

void SampleEffectColor(int effect, uint8_t er, uint8_t eg, uint8_t eb,
                       float along, float timeSec, uint8_t out[3]) {
    along = std::clamp(along, 0.0F, 1.0F);
    switch (effect) {
    case 1: {
        const float s = 0.5F + 0.5F * std::sin(timeSec * 2.1F);
        SolidLevel(er, eg, eb, 0.08F + 0.92F * s, out);
        return;
    }
    case 2:
        PaletteAt(timeSec * 0.14F, out);
        return;
    case 3:
        // Hardware sweep travels from the right side toward the left.
        ColorBand(along, 1.0F - std::fmod(timeSec * 0.32F, 1.0F), er, eg, eb,
                  out);
        return;
    case 4:
        PaletteAt(along + timeSec * 0.14F, out);
        return;
    case 5: {
        const float cycle = std::fmod(timeSec * 0.28F, 2.0F);
        const float pos = cycle <= 1.0F ? 1.0F - cycle : cycle - 1.0F;
        ColorBand(along, pos, er, eg, eb, out);
        return;
    }
    case 6:
        out[0] = 0;
        out[1] = 255;
        out[2] = 255;
        return;
    case 7: {
        const float s = std::sin(timeSec * 3.4F);
        const float p = s > 0.0F ? std::pow(s, 8.0F) : 0.0F;
        SolidLevel(er, eg, eb, 0.06F + 0.94F * p, out);
        return;
    }
    default:
        out[0] = er;
        out[1] = eg;
        out[2] = eb;
        return;
    }
}

static void SampleColor(const Preview &ctx, uint8_t id, float centerX,
                        uint8_t out[3]) {
    if (id == kKeyboardPowerId && ctx.powerOwn) {
        out[0] = ctx.pr;
        out[1] = ctx.pg;
        out[2] = ctx.pb;
        return;
    }
    if (ctx.effect == 0) {
        out[0] = ctx.color[id][0];
        out[1] = ctx.color[id][1];
        out[2] = ctx.color[id][2];
        return;
    }
    float along = 0.5F;
    if (ctx.span > 1.0F)
        along = std::clamp((centerX - ctx.originX) / ctx.span, 0.0F, 1.0F);
    SampleEffectColor(ctx.effect, ctx.er, ctx.eg, ctx.eb, along, ctx.time, out);
}

static float SrgbToLin(uint8_t channel) {
    const float s = static_cast<float>(channel) / 255.0F;
    return s <= 0.04045F ? s / 12.92F
                         : std::pow((s + 0.055F) / 1.055F, 2.4F);
}

static float RelativeLuma(uint8_t r, uint8_t g, uint8_t b) {
    return 0.2126F * SrgbToLin(r) + 0.7152F * SrgbToLin(g) +
           0.0722F * SrgbToLin(b);
}

// Black or white, whichever contrasts more with the color actually drawn
// behind the legend (key color blended over the dark deck).
static ImU32 LegendInk(uint8_t r, uint8_t g, uint8_t b, int fillAlpha) {
    const int deckR = 16;
    const int deckG = 18;
    const int deckB = 24;
    const uint8_t br = static_cast<uint8_t>(
        deckR + (static_cast<int>(r) - deckR) * fillAlpha / 255);
    const uint8_t bg = static_cast<uint8_t>(
        deckG + (static_cast<int>(g) - deckG) * fillAlpha / 255);
    const uint8_t bb = static_cast<uint8_t>(
        deckB + (static_cast<int>(b) - deckB) * fillAlpha / 255);
    const float y = RelativeLuma(br, bg, bb);
    const float contrastWhite = 1.05F / (y + 0.05F);
    const float contrastBlack = (y + 0.05F) / 0.05F;
    return contrastBlack >= contrastWhite ? IM_COL32(0, 0, 0, 255)
                                           : IM_COL32(255, 255, 255, 255);
}

static void DrawLegend(ImDrawList *draw, ImVec2 min, ImVec2 max, ImU32 ink,
                       const char *label) {
    ImFont *font = ImGui::GetFont();
    float size = ImGui::GetFontSize();
    const float pad = 3.0F;
    const float innerW = std::max(1.0F, (max.x - min.x) - pad * 2.0F);
    ImVec2 textSize = font->CalcTextSizeA(size, FLT_MAX, 0.0F, label);
    if (textSize.x > innerW && textSize.x > 0.0F)
        size = std::max(8.0F, size * innerW / textSize.x);
    textSize = font->CalcTextSizeA(size, FLT_MAX, 0.0F, label);
    const ImVec2 textPos(min.x + ((max.x - min.x) - textSize.x) * 0.5F,
                         min.y + ((max.y - min.y) - textSize.y) * 0.5F);
    draw->PushClipRect(ImVec2(min.x + 1.0F, min.y + 1.0F),
                       ImVec2(max.x - 1.0F, max.y - 1.0F), true);
    draw->AddText(font, size, textPos, ink, label);
    draw->PopClipRect();
}

static void DrawKeycap(ImDrawList *draw, ImVec2 min, ImVec2 max, uint8_t r,
                       uint8_t g, uint8_t b, bool selected, bool hovered,
                       const char *label) {
    const float radius = 7.0F;
    const int fillA = hovered ? 210 : 188;
    draw->AddRectFilled(min, max, IM_COL32(r, g, b, fillA), radius);
    if (selected) {
        draw->AddRect(ImVec2(min.x - 2.0F, min.y - 2.0F),
                      ImVec2(max.x + 2.0F, max.y + 2.0F),
                      IM_COL32(255, 255, 255, 230), radius, 0, 1.4F);
    }
    draw->AddRect(min, max, IM_COL32(r, g, b, 255), radius, 0,
                  selected ? 2.0F : 1.5F);
    DrawLegend(draw, min, max, LegendInk(r, g, b, fillA), label);
}

static float DrawRow(const KeyCell *row, int count, int rowIndex, float originX,
                     float y, float unitPx, float gap, float height,
                     bool selected[256], const Preview &preview, int &dragRow,
                     int &dragIndex, bool &dragging) {
    const ImGuiIO &io = ImGui::GetIO();
    ImDrawList *draw = ImGui::GetWindowDrawList();
    float x = originX;

    for (int i = 0; i < count; ++i) {
        const KeyCell &key = row[i];
        const float width = unitPx * key.units;
        if (key.label[0] == '\0') {
            x += width + gap;
            continue;
        }

        ImGui::SetCursorScreenPos(ImVec2(x, y));
        ImGui::PushID(key.id + rowIndex * 300);
        ImGui::InvisibleButton("##key", ImVec2(width, height));
        const bool hovered = ImGui::IsItemHovered(
            ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
        const ImVec2 min = ImGui::GetItemRectMin();
        const ImVec2 max = ImGui::GetItemRectMax();
        uint8_t rgb[3];
        SampleColor(preview, key.id, (min.x + max.x) * 0.5F, rgb);
        DrawKeycap(draw, min, max, rgb[0], rgb[1], rgb[2], selected[key.id],
                   hovered, key.label);
        ImGui::PopID();

        if (hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            if (io.KeyCtrl) {
                selected[key.id] = !selected[key.id];
                dragging = false;
            } else {
                SelectRange(selected, row, count, i, i);
                dragRow = rowIndex;
                dragIndex = i;
                dragging = true;
            }
        } else if (dragging && hovered &&
                   ImGui::IsMouseDown(ImGuiMouseButton_Left) &&
                   dragRow == rowIndex) {
            SelectRange(selected, row, count, dragIndex, i);
        }
        x += width + gap;
    }
    return x - gap;
}

void DrawArea51Keyboard(bool selected[256], const uint8_t color[256][3],
                        int effect, uint8_t effectR, uint8_t effectG,
                        uint8_t effectB, bool powerOwnColor, uint8_t powerR,
                        uint8_t powerG, uint8_t powerB, bool trackOwnColor,
                        int trackEffect, uint8_t trackR, uint8_t trackG,
                        uint8_t trackB) {
    static int dragRow = -1;
    static int dragIndex = -1;
    static bool dragging = false;
    if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
        dragging = false;

    const float gap = 5.0F;
    const float height = 38.0F;
    const float rowGap = 7.0F;
    const float deckPad = 14.0F;
    const float functionGap = 12.0F;
    const float mediaUnits = 1.4F;
    const float extraUnits = 3.0F + mediaUnits;
    const float extraGaps = gap * 4.0F;
    const KeyCell *rows[] = {kRow0, kRow1, kRow2, kRow3, kRow4, kRow5};
    const int counts[] = {
        (int)(sizeof(kRow0) / sizeof(kRow0[0])),
        (int)(sizeof(kRow1) / sizeof(kRow1[0])),
        (int)(sizeof(kRow2) / sizeof(kRow2[0])),
        (int)(sizeof(kRow3) / sizeof(kRow3[0])),
        (int)(sizeof(kRow4) / sizeof(kRow4[0])),
        (int)(sizeof(kRow5) / sizeof(kRow5[0])),
    };

    const float avail = ImGui::GetContentRegionAvail().x;
    float unitPx = 1000.0F;
    float mainUnits = 0.0F;
    for (int i = 0; i < 6; ++i) {
        const float units = RowUnits(rows[i], counts[i]);
        if (units > mainUnits)
            mainUnits = units;
        const float gaps = gap * static_cast<float>(counts[i] - 1);
        const float fit = (avail - deckPad * 2.0F - extraGaps - gaps) /
                          (units + extraUnits);
        if (fit < unitPx)
            unitPx = fit;
    }

    const float assembly =
        (mainUnits + extraUnits) * unitPx +
        gap * static_cast<float>(counts[0] - 1) + extraGaps;
    const float trackH = 54.0F;
    const float trackGap = 20.0F;
    const float deckH = deckPad * 2.0F + height * 6.0F + rowGap * 5.0F +
                        functionGap + trackGap + trackH;
    const ImVec2 deckMin = ImGui::GetCursorScreenPos();
    const ImVec2 deckMax(deckMin.x + avail, deckMin.y + deckH);
    ImDrawList *draw = ImGui::GetWindowDrawList();
    draw->AddRectFilled(deckMin, deckMax, IM_COL32(16, 18, 24, 255), 14.0F);
    draw->AddRect(deckMin, deckMax, IM_COL32(255, 255, 255, 18), 14.0F, 0,
                  1.0F);

    const float originX = deckMin.x + std::max(deckPad, (avail - assembly) * 0.5F);
    auto rowEdge = [&](const KeyCell *row, int count) {
        return originX + RowUnits(row, count) * unitPx +
               gap * static_cast<float>(count - 1);
    };
    const float row4Right = rowEdge(kRow4, counts[4]);
    const float row5Right = rowEdge(kRow5, counts[5]);
    const float downX =
        std::max(row5Right + gap + unitPx + gap, row4Right + gap);
    const float mediaX = downX + (unitPx + gap) * 2.0F;
    const float span = (mediaX + mediaUnits * unitPx) - originX;
    const Preview preview{effect,     effectR, effectG, effectB, powerOwnColor,
                          powerR,     powerG,  powerB,  color,   originX,
                          span,       static_cast<float>(ImGui::GetTime())};

    float y = deckMin.y + deckPad;
    float rowY[6];
    for (int row = 0; row < 6; ++row) {
        rowY[row] = y;
        DrawRow(rows[row], counts[row], row, originX, y, unitPx, gap, height,
                selected, preview, dragRow, dragIndex, dragging);
        y += height + rowGap;
        if (row == 0)
            y += functionGap;
    }

    auto drawOne = [&](const KeyCell &key, int rowIndex, float x, float keyY) {
        KeyCell alone = key;
        DrawRow(&alone, 1, rowIndex, x, keyY, unitPx, gap, height, selected,
                preview, dragRow, dragIndex, dragging);
    };

    drawOne(kLeft, 6, downX - unitPx - gap, rowY[5]);
    drawOne(kDown, 7, downX, rowY[5]);
    drawOne(kRight, 8, downX + unitPx + gap, rowY[5]);
    drawOne(kUp, 9, downX, rowY[4]);
    drawOne(kPower, 10, mediaX, rowY[0]);
    drawOne(kMic, 11, mediaX, rowY[1]);
    drawOne(kMute, 12, mediaX, rowY[2]);
    drawOne(kVolUp, 13, mediaX, rowY[3]);
    drawOne(kVolDn, 14, mediaX, rowY[4]);

    float blockRight = originX;
    for (int row = 0; row < 6; ++row)
        blockRight = std::max(blockRight, rowEdge(rows[row], counts[row]));
    const float padW = std::min((blockRight - originX) * 0.46F, unitPx * 6.4F);
    const float padX = originX + ((blockRight - originX) - padW) * 0.5F;
    const float padY = rowY[5] + height + trackGap;
    const ImVec2 padMin(padX, padY);
    const ImVec2 padMax(padX + padW, padY + trackH);
    const int padEffect = trackOwnColor ? trackEffect : effect;
    const uint8_t padR = trackOwnColor ? trackR : effectR;
    const uint8_t padG = trackOwnColor ? trackG : effectG;
    const uint8_t padB = trackOwnColor ? trackB : effectB;
    uint8_t midC[3];
    SampleEffectColor(padEffect, padR, padG, padB, 0.5F, preview.time, midC);
    const int padFill = 188;
    draw->AddRectFilled(padMin, padMax,
                        IM_COL32(midC[0], midC[1], midC[2], padFill), 12.0F);
    draw->AddRect(padMin, padMax, IM_COL32(midC[0], midC[1], midC[2], 255),
                  12.0F, 0, 1.6F);
    DrawLegend(draw, padMin, padMax, LegendInk(midC[0], midC[1], midC[2], padFill),
               "Trackpad");

    ImGui::SetCursorScreenPos(ImVec2(deckMin.x, deckMax.y + 4.0F));
    ImGui::Dummy(ImVec2(avail, 1.0F));
}
