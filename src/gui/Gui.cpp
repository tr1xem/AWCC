#include "Gui.h"
#include "AcpiUtils.h"
#include "EffectController.h"
#include "Resources.hpp"
#include "database.h"
#include "imgui.h"
#define STB_IMAGE_IMPLEMENTATION
#include <GL/gl.h>
#include <stb_image.h>
#include <string>

void Gui::SetupImGuiStyle() {

    ImGuiStyle &style = ImGui::GetStyle();
    style.Alpha = 1.0F;
    style.DisabledAlpha = 0.5F;
    style.WindowPadding = ImVec2(12.0F, 12.0F);
    style.WindowRounding = 0.0F;
    style.WindowBorderSize = 0.0F;
    style.WindowMinSize = ImVec2(20.0F, 20.0F);
    style.WindowTitleAlign = ImVec2(0.5F, 0.5F);
    style.WindowMenuButtonPosition = ImGuiDir_Right;
    style.ChildRounding = 0.0F;
    style.ChildBorderSize = 1.0F;
    style.PopupRounding = 0.0F;
    style.PopupBorderSize = 1.0F;
    style.FramePadding = ImVec2(20.0F, 3.400000095367432F);
    style.FrameRounding = 11.89999961853027F;
    style.FrameBorderSize = 0.0F;
    style.ItemSpacing = ImVec2(4.300000190734863F, 5.5F);
    style.ItemInnerSpacing = ImVec2(7.099999904632568F, 1.799999952316284F);
    style.CellPadding = ImVec2(12.10000038146973F, 9.199999809265137F);
    style.IndentSpacing = 0.0F;
    style.ColumnsMinSpacing = 4.900000095367432F;
    style.ScrollbarSize = 11.60000038146973F;
    style.ScrollbarRounding = 15.89999961853027F;
    style.GrabMinSize = 3.700000047683716F;
    style.GrabRounding = 20.0F;
    style.TabRounding = 0.0F;
    style.TabBorderSize = 0.0F;
    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5F, 0.5F);
    style.SelectableTextAlign = ImVec2(0.0F, 0.0F);

    style.Colors[ImGuiCol_Text] = ImVec4(1.0F, 1.0F, 1.0F, 1.0F);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(
        0.2745098173618317F, 0.3176470696926117F, 0.4509803950786591F, 1.0F);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(
        0.0784313753247261F, 0.08627451211214066F, 0.1019607856869698F, 1.0F);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(
        0.09411764889955521F, 0.1019607856869698F, 0.1176470592617989F, 1.0F);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(
        0.0784313753247261F, 0.08627451211214066F, 0.1019607856869698F, 1.0F);
    style.Colors[ImGuiCol_Border] = ImVec4(
        0.1568627506494522F, 0.168627455830574F, 0.1921568661928177F, 1.0F);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(
        0.0784313753247261F, 0.08627451211214066F, 0.1019607856869698F, 1.0F);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(
        0.1137254908680916F, 0.125490203499794F, 0.1529411822557449F, 1.0F);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(
        0.1568627506494522F, 0.168627455830574F, 0.1921568661928177F, 1.0F);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(
        0.1568627506494522F, 0.168627455830574F, 0.1921568661928177F, 1.0F);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(
        0.0470588244497776F, 0.05490196123719215F, 0.07058823853731155F, 1.0F);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(
        0.0470588244497776F, 0.05490196123719215F, 0.07058823853731155F, 1.0F);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(
        0.0784313753247261F, 0.08627451211214066F, 0.1019607856869698F, 1.0F);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(
        0.09803921729326248F, 0.105882354080677F, 0.1215686276555061F, 1.0F);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(
        0.0470588244497776F, 0.05490196123719215F, 0.07058823853731155F, 1.0F);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(
        0.1176470592617989F, 0.1333333402872086F, 0.1490196138620377F, 1.0F);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(
        0.1568627506494522F, 0.168627455830574F, 0.1921568661928177F, 1.0F);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(
        0.1176470592617989F, 0.1333333402872086F, 0.1490196138620377F, 1.0F);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0F, 1.0F, 1.0F, 1.0F);

    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.0F, 1.0F, 1.0F, 1.0F);
    style.Colors[ImGuiCol_SliderGrabActive] =
        ImVec4(1.0F, 0.7960784435272217F, 0.4980392158031464F, 1.0F);
    style.Colors[ImGuiCol_Button] = ImVec4(
        0.1176470592617989F, 0.1333333402872086F, 0.1490196138620377F, 1.0F);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(
        0.1803921610116959F, 0.1882352977991104F, 0.196078434586525F, 1.0F);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(
        0.1529411822557449F, 0.1529411822557449F, 0.1529411822557449F, 1.0F);
    style.Colors[ImGuiCol_Header] = ImVec4(
        0.1411764770746231F, 0.1647058874368668F, 0.2078431397676468F, 1.0F);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(
        0.105882354080677F, 0.105882354080677F, 0.105882354080677F, 1.0F);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(
        0.0784313753247261F, 0.08627451211214066F, 0.1019607856869698F, 1.0F);
    style.Colors[ImGuiCol_Separator] = ImVec4(
        0.1294117718935013F, 0.1490196138620377F, 0.1921568661928177F, 1.0F);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(
        0.1568627506494522F, 0.1843137294054031F, 0.250980406999588F, 1.0F);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(
        0.1568627506494522F, 0.1843137294054031F, 0.250980406999588F, 1.0F);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(
        0.1450980454683304F, 0.1450980454683304F, 0.1450980454683304F, 1.0F);
    style.Colors[ImGuiCol_ResizeGripHovered] =
        ImVec4(0.9725490212440491F, 1.0F, 0.4980392158031464F, 1.0F);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.0F, 1.0F, 1.0F, 1.0F);
    style.Colors[ImGuiCol_Tab] = ImVec4(
        0.0784313753247261F, 0.08627451211214066F, 0.1019607856869698F, 1.0F);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(
        0.1176470592617989F, 0.1333333402872086F, 0.1490196138620377F, 1.0F);
    style.Colors[ImGuiCol_TabActive] = ImVec4(
        0.1176470592617989F, 0.1333333402872086F, 0.1490196138620377F, 1.0F);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(
        0.0784313753247261F, 0.08627451211214066F, 0.1019607856869698F, 1.0F);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(
        0.125490203499794F, 0.2745098173618317F, 0.572549045085907F, 1.0F);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(
        0.5215686559677124F, 0.6000000238418579F, 0.7019608020782471F, 1.0F);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(
        0.03921568766236305F, 0.9803921580314636F, 0.9803921580314636F, 1.0F);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(
        0.8823529481887817F, 0.7960784435272217F, 0.5607843399047852F, 1.0F);
    style.Colors[ImGuiCol_PlotHistogramHovered] =
        ImVec4(0.95686274766922F, 0.95686274766922F, 0.95686274766922F, 1.0F);
    style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(
        0.0470588244497776F, 0.05490196123719215F, 0.07058823853731155F, 1.0F);
    style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(
        0.0470588244497776F, 0.05490196123719215F, 0.07058823853731155F, 1.0F);
    style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0F, 0.0F, 0.0F, 1.0F);
    style.Colors[ImGuiCol_TableRowBg] = ImVec4(
        0.1176470592617989F, 0.1333333402872086F, 0.1490196138620377F, 1.0F);
    style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(
        0.09803921729326248F, 0.105882354080677F, 0.1215686276555061F, 1.0F);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(
        0.9372549057006836F, 0.9372549057006836F, 0.9372549057006836F, 1.0F);
    style.Colors[ImGuiCol_DragDropTarget] =
        ImVec4(0.4980392158031464F, 0.5137255191802979F, 1.0F, 1.0F);
    style.Colors[ImGuiCol_NavHighlight] =
        ImVec4(0.2666666805744171F, 0.2901960909366608F, 1.0F, 1.0F);
    style.Colors[ImGuiCol_NavWindowingHighlight] =
        ImVec4(0.4980392158031464F, 0.5137255191802979F, 1.0F, 1.0F);
    style.Colors[ImGuiCol_NavWindowingDimBg] =
        ImVec4(0.196078434586525F, 0.1764705926179886F, 0.5450980663299561F,
               0.501960813999176F);
    style.Colors[ImGuiCol_ModalWindowDimBg] =
        ImVec4(0.196078434586525F, 0.1764705926179886F, 0.5450980663299561F,
               0.501960813999176F);
}
static inline bool SelectableImageButton(
    ImTextureID tex_id, ImVec2 img_size, const char *label,
    bool selected = false, ImVec2 button_size = ImVec2(96, 80),
    std::optional<ImU32> image_col = std::nullopt // NEW: optional color
) {
    bool pressed = false;

    ImGui::BeginGroup();
    ImGui::PushID(label);

    if (ImGui::InvisibleButton("##btn", button_size))
        pressed = true;

    ImVec2 btn_min = ImGui::GetItemRectMin();
    ImVec2 btn_max = ImGui::GetItemRectMax();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    ImU32 bg_col = ImGui::GetColorU32(ImGuiCol_Button);
    ImU32 border_col =
        selected ? IM_COL32(255, 60, 60, 255) : IM_COL32(60, 60, 60, 255);
    float rounding = 8.0F;
    draw_list->AddRectFilled(btn_min, btn_max, bg_col, rounding);
    draw_list->AddRect(btn_min, btn_max, border_col, rounding, 0,
                       selected ? 2.5F : 1.2F);

    // Determine tint color
    ImU32 tint = image_col.value_or(IM_COL32_WHITE);

    float icon_x = btn_min.x + ((button_size.x - img_size.x) * 0.5F);
    float icon_y = btn_min.y + 12.0F;
    draw_list->AddImage(tex_id, ImVec2(icon_x, icon_y),
                        ImVec2(icon_x + img_size.x, icon_y + img_size.y),
                        ImVec2(0, 0), ImVec2(1, 1),
                        tint // <-- Use tint here
    );

    ImVec2 text_size = ImGui::CalcTextSize(label);
    float text_x = btn_min.x + ((button_size.x - text_size.x) * 0.5F);
    float text_y = icon_y + img_size.y + 8.0F;
    draw_list->AddText(ImVec2(text_x, text_y),
                       ImGui::GetColorU32(ImGuiCol_Text), label);

    ImGui::PopID();
    ImGui::EndGroup();

    return pressed;
}
static GLuint LoadTextureFromMemory(const unsigned char *data, int len,
                                    int *outWidth = nullptr,
                                    int *outHeight = nullptr) {
    int width, height, channels;
    unsigned char *image_data = stbi_load_from_memory(
        data, len, &width, &height, &channels, 4); // force RGBA

    if (image_data == nullptr)
        return 0;

    GLuint tex_id = 0;
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    if (outWidth != nullptr)
        *outWidth = width;
    if (outHeight != nullptr)
        *outHeight = height;

    return tex_id;
}

static void ModeButtonList(const char *labels[], Thermals &thermals,
                           AcpiUtils &acpiUtils, int &selectedMode) {
    static GLuint quiteModeimg =
        LoadTextureFromMemory(quiteMode, quiteMode_len);
    static GLuint balancedModeimg =
        LoadTextureFromMemory(balancedMode, balancedMode_len);

    static GLuint batteryModeimg =
        LoadTextureFromMemory(batteryMode, batteryMode_len);
    static GLuint performanceModeimg =
        LoadTextureFromMemory(performanceMode, performanceMode_len);
    static GLuint gmodeimg = LoadTextureFromMemory(gMode, gMode_len);
    constexpr int button_count = 5;
    float spacing = 10.0F;
    float total_spacing = spacing * (button_count - 1);
    float region_width = ImGui::GetContentRegionAvail().x;
    float button_width = (region_width - total_spacing) / button_count;
    float button_height = 80.0F;
    ImTextureID icons[] = {batteryModeimg, quiteModeimg, balancedModeimg,
                           performanceModeimg, gmodeimg};

    for (int i = 0; i < button_count; ++i) {
        if (SelectableImageButton(
                icons[i], ImVec2(32, 32), labels[i], selectedMode == i,
                ImVec2(button_width, button_height), IM_COL32_WHITE)) {
            switch (i) {
            case 0:
                if (acpiUtils.hasThermalMode(ThermalModeSet::BatterySaver)) {
                    thermals.setThermalMode(ThermalModes::BatterySaver);
                } else {
                    thermals.setThermalMode(ThermalModes::Cool);
                }
                selectedMode = 0;
                break;

            case 1:
                thermals.setThermalMode(ThermalModes::Quiet);
                selectedMode = 1;
                break;
            case 2:
                thermals.setThermalMode(ThermalModes::Balanced);
                selectedMode = 2;
                break;
            case 3:
                thermals.setThermalMode(ThermalModes::Performance);
                selectedMode = 3;
                break;
            case 4:
                thermals.setThermalMode(ThermalModes::Gmode);
                selectedMode = 4;
                break;
            }
        }
        if (i < button_count - 1)
            ImGui::SameLine(0.0F, spacing);
    }
}

static inline uint32_t ToRGB(const ImVec4 &color) {
    return ((uint32_t)(color.x * 255.0F) << 16) |
           ((uint32_t)(color.y * 255.0F) << 8) | ((uint32_t)(color.z * 255.0F));
}
static inline void GuiKeyboardLightingBar(int &selectedMode, ImVec4 &color,
                                          bool &colorEnabled,
                                          EffectController &effects) {
    static const std::vector<const char *> modes = {
        "Static",  "Breathe",        "Spectrum",    "Wave",
        "Rainbow", "Back and Forth", "Default blue"};

    float comboWidth = 180.0F;
    float buttonWidth = 180.0F;
    float colorLabelWidth = ImGui::CalcTextSize("Color").x;
    float colorButtonWidth = ImGui::GetFrameHeight();
    float colorGap = 8.0F;
    float colorGroupWidth = colorLabelWidth + colorGap + colorButtonWidth;

    float y = ImGui::GetCursorPosY();

    // --- Combo: left ---
    ImGui::PushItemWidth(comboWidth);
    ImGui::Combo("##mode", &selectedMode, modes.data(), modes.size());
    ImGui::PopItemWidth();

    // --- Center color group ---
    // Calculate available width for center
    float fullWidth = ImGui::GetContentRegionAvail().x + ImGui::GetCursorPosX();
    float centerStart = (fullWidth - colorGroupWidth) / 2.0F;

    ImGui::SameLine();
    ImGui::SetCursorPosY(y);
    ImGui::SetCursorPosX(centerStart);
    bool canPickColor = (selectedMode == 0 || selectedMode == 1 ||
                         selectedMode == 3 || selectedMode == 5);

    ImGui::BeginDisabled(!canPickColor);
    ImGui::BeginGroup();
    ImGui::TextUnformatted("Color");
    ImGui::SameLine(0, colorGap);
    ImGui::ColorEdit3("##color", (float *)&color, ImGuiColorEditFlags_NoInputs);
    ImGui::EndDisabled();
    ImGui::EndGroup();

    // --- Right: Apply button ---
    ImGui::SameLine();
    ImGui::SetCursorPosY(y);
    ImGui::SetCursorPosX(fullWidth - buttonWidth);
    if (ImGui::Button("Apply", ImVec2(buttonWidth, 0))) {
        uint32_t color32 = ToRGB(color);
        constexpr uint16_t defaultDuration = 1000;
        switch (selectedMode) {
        case 0:
            effects.StaticColor(color32);
            break;
        case 1:
            effects.Breathe(color32);
            break;
        case 2:
            effects.Spectrum(1000);
            break;
        case 3:
            effects.Wave(color32);
            break;
        case 4:
            effects.Rainbow(500);
            break;
        case 5:
            effects.BackAndForth(color32);
            break;
        case 6:
            effects.DefaultBlue();
            break;
        }
    }
}
void Gui::App(int h, int w, Thermals &thermals, AcpiUtils &acpiUtils,
              int &selectedMode, int &gpuBoost, int &cpuBoost,
              ImFont &smallFont, ImFont &fontbold, int &brightness,
              EffectController &effects, bool &turbo) {

    static int kblastval = 50;
    static float kbtimer = 0.0F;
    static int cpulastVal = 50;
    static float cputimer = 0.0F;
    static int gpulastVal = 50;
    static float gputimer = 0.0F;
    constexpr float debounceSeconds = 0.5F; // 300 ms

    static ImGuiWindowFlags main_flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    {
        static float f = 0.0F;
        static int counter = 0;

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2((float)w, (float)h), ImGuiCond_Always);
        ImGui::Begin("Main Window", nullptr, main_flags);

        SetupImGuiStyle();
        // NOTE:: Title

        ImGui::PushFont(&fontbold);
        ImGui::SetCursorPos(ImVec2(10, 10));
        ImGui::Text("Alienware Command Centre - %s",
                    AcpiUtils::getDeviceName());
        // ImGui::SameLine();
        // ImGui::SetCursorPosX(ImGui::GetWindowWidth() -
        //                      ImGui::CalcTextSize("x").x -
        //                      ImGui::GetStyle().ItemSpacing.x - 5.0F);
        // ImGui::Text("x");
        ImGui::Separator();

        // NOTE:: POWER MODE
        ImGui::Text("Power Modes");
        ImGui::Dummy(ImVec2(5, 5));
        ImGui::PopFont();
        const char *firstMode{};
        if (acpiUtils.hasThermalMode(ThermalModeSet::BatterySaver)) {
            firstMode = "Battery";
        } else {

            firstMode = "Cool";
        }
        const char *labels[] = {firstMode, "Quiet", "Balanced", "Performance",
                                "G-mode"};

        ImGui::PushFont(&smallFont);
        ModeButtonList(labels, thermals, acpiUtils, selectedMode);
        ImGui::PopFont();

        // NOTE:: CPU FAN BOOST
        ImGui::Dummy(ImVec2(0, 10));
        ImGui::PushFont(&fontbold);
        ImGui::Text("CPU Fan Boost");
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() -
                             ImGui::CalcTextSize("00%").x -
                             ImGui::GetStyle().ItemSpacing.x - 5.0F);
        ImGui::Text("%d%%", cpuBoost);
        ImGui::PopFont();
        ImGui::Spacing();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::SliderInt("##cpuBoost", &cpuBoost, 0, 100)) {
            cputimer = 0.0F;
        } else {
            cputimer +=
                ImGui::GetIO().DeltaTime; // Accumulate time when not changing
        }

        ImGui::PopItemWidth();
        if (cpuBoost != cpulastVal && cputimer > debounceSeconds) {
            cpulastVal = cpuBoost;
            thermals.setCpuBoost(cpuBoost);
        }

        // NOTE:: GPU FAN BOOST
        ImGui::Dummy(ImVec2(0, 10));
        ImGui::PushFont(&fontbold);
        ImGui::Text("GPU Fan Boost");
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() -
                             ImGui::CalcTextSize("00%").x -
                             ImGui::GetStyle().ItemSpacing.x - 5.0F);
        ImGui::Text("%d%%", gpuBoost);
        ImGui::PopFont();
        ImGui::Spacing();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::SliderInt("##gpuBoost", &gpuBoost, 0, 100)) {
            gputimer = 0.0F;
        } else {
            gputimer +=
                ImGui::GetIO().DeltaTime; // Accumulate time when not changing
        }

        ImGui::PopItemWidth();
        if (gpuBoost != gpulastVal && gputimer > debounceSeconds) {
            gpulastVal = gpuBoost;
            thermals.setGpuBoost(gpuBoost);
        }

        // NOTE : KB LIGHTING
        ImGui::Dummy(ImVec2(0, 10));
        ImGui::PushFont(&fontbold);
        ImGui::Text("Keyboard Lighting Effects");
        ImGui::PopFont();
        ImGui::Dummy(ImVec2(0, 5));
        static int kb_selectedMode = 0;
        static ImVec4 kb_color = ImVec4(1.F, 0.F, 0.F, 1.F);
        static bool kb_colorEnabled = true;

        GuiKeyboardLightingBar(kb_selectedMode, kb_color, kb_colorEnabled,
                               effects);

        // NOTE:: Brightness
        ImGui::Dummy(ImVec2(0, 10));
        ImGui::PushFont(&fontbold);
        ImGui::Text("Keyboard Brightness");
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() -
                             ImGui::CalcTextSize("00%").x -
                             ImGui::GetStyle().ItemSpacing.x - 5.0F);
        ImGui::Text("%d%%", brightness);
        ImGui::PopFont();
        ImGui::Dummy(ImVec2(0, 5));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::SliderInt("##kbbrightness", &brightness, 0, 100)) {
            kbtimer = 0.0F;
        } else {
            kbtimer +=
                ImGui::GetIO().DeltaTime; // Accumulate time when not changing
        }
        ImGui::PopItemWidth();
        if (brightness != kblastval && kbtimer > debounceSeconds) {
            kblastval = brightness;
            effects.Brightness(brightness);
        }

        // NOTE :: TURBO BOOST
        ImGui::Dummy(ImVec2(0, 10));
        ImGui::PushFont(&fontbold);
        ImGui::Text("Extra Settings");
        ImGui::PopFont();
        ImGui::Dummy(ImVec2(0, 5));
        if (ImGui::Checkbox("Turbo Boost", &turbo)) {
            acpiUtils.setTurboBoost(turbo);
        }
        ImGui::SetItemTooltip("Set Cpu's Max Frequency to a higher State");

        // NOTE: FO0TER

        static ImGuiIO &io = ImGui::GetIO();
        static ImFont *fontfooter = ImGui::GetIO().Fonts->AddFontFromFileTTF(
            "/usr/share/fonts/TTF/Roboto-Medium.ttf", 15.0F);
        if (fontfooter == nullptr) {
            // Fallback 2: use default font
            fontfooter = io.Fonts->AddFontDefault();
        }
        ImGui::PushFont(fontfooter);
#ifndef NDEBUG
        const std::string VerText =
            std::string("Version ") + VERSION + "-Debug";
#else
        const std::string VerText =
            std::string("Version ") + VERSION + "-Stable";
#endif
        ImGui::SetCursorPos(
            ImVec2(10, ImGui::GetWindowHeight() -
                           ImGui::CalcTextSize(VerText.c_str()).y - 15.0F));
        ImGui::Text("%s", VerText.c_str());

        // ImGui::SameLine();
        // ImGui::SetCursorPosX(
        //     ImGui::GetWindowWidth() -
        //     ImGui::CalcTextSize("AVG 0000f ms/frame (000 FPS)").x - 28.0F);
        // ImGui::Text("AVG %.3f ms/frame (%.1f FPS)", 1000.0F / io.Framerate,
        //             io.Framerate);
        ImGui::SetCursorPosY(ImGui::GetWindowHeight() -
                             ImGui::CalcTextSize(VerText.c_str()).y - 15.0F);
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 255, 255));
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() -
                             ImGui::CalcTextSize("Support").x - 10.0F);
        if (ImGui::Selectable("Support", false,
                              ImGuiSelectableFlags_DontClosePopups)) {
            system("xdg-open "
                   "https://github.com/tr1xem/"
                   "AWCC?tab=readme-ov-file#support-and-feedback &");
        }

        ImGui::PopStyleColor();
        ImGui::PopFont();
        ImGui::End();
    }
}
