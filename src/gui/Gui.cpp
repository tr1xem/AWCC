#include "Gui.h"
#include "AcpiUtils.h"
#include "EffectController.h"
#include "FanCurve.h"
#include "database.h"
#include "imgui.h"
#include "resource.h"
#define STB_IMAGE_IMPLEMENTATION
#include "KeyboardMap.h"
#include "helper.h"
#include <GL/gl.h>
#include <stb_image.h>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include <fcntl.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

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
    style.ChildRounding = 10.0F;
    style.ChildBorderSize = 1.0F;
    style.PopupRounding = 0.0F;
    style.PopupBorderSize = 1.0F;
    style.FramePadding = ImVec2(12.0F, 7.0F);
    style.FrameRounding = 6.0F;
    style.FrameBorderSize = 0.0F;
    style.ItemSpacing = ImVec2(10.0F, 10.0F);
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
    float rounding = 8.0F;
    draw_list->AddRectFilled(btn_min, btn_max, bg_col, rounding);
    draw_list->AddRect(btn_min, btn_max, IM_COL32(55, 62, 72, 255), rounding, 0,
                       1.2F);
    (void)selected;

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

static void HoverTip(const char *text) {
    if (!ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
        return;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12.0F, 10.0F));
    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(360.0F);
    ImGui::TextUnformatted(text);
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
    ImGui::PopStyleVar();
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
    ImTextureID icons[] = {(ImTextureID)(intptr_t)batteryModeimg,
                           (ImTextureID)(intptr_t)quiteModeimg,
                           (ImTextureID)(intptr_t)balancedModeimg,
                           (ImTextureID)(intptr_t)performanceModeimg,
                           (ImTextureID)(intptr_t)gmodeimg};
    ImVec2 modeMin[button_count];
    ImVec2 modeMax[button_count];

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
        const bool battery =
            acpiUtils.hasThermalMode(ThermalModeSet::BatterySaver);
        const char *tips[] = {
            battery ? "The unplugged profile. Processor and graphics power "
                      "limits come down to draw less power, and the fans stay "
                      "on a quiet curve. Runtime goes up and sustained speed "
                      "comes down."
                    : "The cool profile. Fans speed up earlier to hold a lower "
                      "temperature. Power limits stay modest so the chassis "
                      "and the components run cooler.",
            "The low-noise profile. Fan speeds stay down, and processor and "
            "graphics power limits drop so the machine can hold temperature "
            "without spinning the fans up.",
            "The everyday profile. Processor and graphics use their standard "
            "power limits, and the fans follow a middle curve as the heat "
            "rises.",
            "Raises processor and graphics power limits above Balanced, so "
            "clocks hold higher under a heavy load. The fan curve steepens to "
            "carry the extra heat.",
            "The highest profile. It turns on the firmware G-mode path, raises "
            "processor and graphics power limits above Performance, and runs "
            "both fans on the steepest curve.",
        };
        HoverTip(tips[i]);
        modeMin[i] = ImGui::GetItemRectMin();
        modeMax[i] = ImGui::GetItemRectMax();
        if (i < button_count - 1)
            ImGui::SameLine(0.0F, spacing);
    }

    if (selectedMode >= 0 && selectedMode < button_count) {
        const ImVec2 targetMin = modeMin[selectedMode];
        const ImVec2 targetMax = modeMax[selectedMode];
        static ImVec2 slideMin;
        static ImVec2 slideMax;
        static bool slideReady = false;
        if (!slideReady) {
            slideMin = targetMin;
            slideMax = targetMax;
            slideReady = true;
        } else {
            const float t = 1.0F - std::exp(-ImGui::GetIO().DeltaTime * 14.0F);
            slideMin.x += (targetMin.x - slideMin.x) * t;
            slideMin.y += (targetMin.y - slideMin.y) * t;
            slideMax.x += (targetMax.x - slideMax.x) * t;
            slideMax.y += (targetMax.y - slideMax.y) * t;
        }
        ImGui::GetWindowDrawList()->AddRect(slideMin, slideMax,
                                            IM_COL32(0, 214, 230, 255), 8.0F, 0,
                                            2.5F);
    }
}

static std::string UserAlienFxCliPath() {
    const char *home = std::getenv("HOME");
    if (home == nullptr || home[0] == '\0')
        return {};
    return std::string(home) + "/.local/bin/alienfx_cli";
}

static std::string AlienFxCliPath() {
    if (const char *env = std::getenv("AWCC_ALIENFX_CLI")) {
        if (env[0] != '\0' && access(env, X_OK) == 0)
            return env;
        return {};
    }
    const char *pathEnv = std::getenv("PATH");
    if (pathEnv != nullptr) {
        std::string paths(pathEnv);
        std::size_t start = 0;
        while (start <= paths.size()) {
            std::size_t colon = paths.find(':', start);
            if (colon == std::string::npos)
                colon = paths.size();
            std::string candidate =
                paths.substr(start, colon - start) + "/alienfx_cli";
            if (access(candidate.c_str(), X_OK) == 0)
                return candidate;
            if (colon == paths.size())
                break;
            start = colon + 1;
        }
    }
    const std::string userBin = UserAlienFxCliPath();
    if (!userBin.empty() && access(userBin.c_str(), X_OK) == 0)
        return userBin;
    return {};
}

static std::mutex gCliMu;
static bool gCliBusy = false;
static std::string gCliError;

static int SpawnWait(const std::vector<std::string> &args,
                     const char *stdoutPath) {
    std::vector<char *> argv;
    argv.reserve(args.size() + 1);
    for (const std::string &arg : args)
        argv.push_back(const_cast<char *>(arg.c_str()));
    argv.push_back(nullptr);

    posix_spawn_file_actions_t actions;
    posix_spawn_file_actions_init(&actions);
    if (stdoutPath != nullptr) {
        posix_spawn_file_actions_addopen(&actions, STDOUT_FILENO, stdoutPath,
                                         O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }
    pid_t pid = 0;
    const int rc = posix_spawnp(&pid, args[0].c_str(), &actions, nullptr,
                                argv.data(), environ);
    posix_spawn_file_actions_destroy(&actions);
    if (rc != 0)
        return -1;
    int status = 0;
    if (waitpid(pid, &status, 0) < 0)
        return -1;
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return -1;
}

static std::string InstallAlienFxRelease() {
    const std::string dest = UserAlienFxCliPath();
    if (dest.empty())
        return "Could not find the home directory.";

    std::string tmp = "/tmp/awcc-alienfx-XXXXXX";
    if (mkdtemp(tmp.data()) == nullptr)
        return "Could not create a temporary directory.";
    const std::filesystem::path root(tmp);
    const auto cleanup = [&]() {
        std::error_code ec;
        std::filesystem::remove_all(root, ec);
    };

    const std::filesystem::path meta = root / "release.json";
    if (SpawnWait({"curl", "-fsSL", "-A", "awcc", "-o", meta.string(),
                   "https://api.github.com/repos/tr1xem/alienfx-linux/"
                   "releases/latest"},
                  nullptr) != 0) {
        cleanup();
        return "Could not reach the alienfx-linux release.";
    }

    std::string url;
    std::string digest;
    try {
        std::ifstream in(meta);
        const nlohmann::json release = nlohmann::json::parse(in);
        for (const auto &asset : release.value("assets", nlohmann::json::array())) {
            if (asset.value("name", "") != "alienfx_cli.7z")
                continue;
            url = asset.value("browser_download_url", "");
            digest = asset.value("digest", "");
            break;
        }
    } catch (...) {
        cleanup();
        return "Could not read the release information.";
    }

    const std::string prefix =
        "https://github.com/tr1xem/alienfx-linux/releases/download/";
    if (!url.starts_with(prefix) || !url.ends_with("/alienfx_cli.7z")) {
        cleanup();
        return "The latest release has no alienfx_cli download.";
    }
    if (!digest.starts_with("sha256:") || digest.size() != 7 + 64) {
        cleanup();
        return "The release download has no checksum.";
    }

    const std::filesystem::path archive = root / "alienfx_cli.7z";
    if (SpawnWait({"curl", "-fsSL", "-A", "awcc", "-o", archive.string(), url},
                  nullptr) != 0) {
        cleanup();
        return "Could not download alienfx_cli.";
    }

    const std::filesystem::path sumFile = root / "sha256.txt";
    if (SpawnWait({"sha256sum", archive.string()}, sumFile.c_str()) != 0) {
        cleanup();
        return "Could not check the download.";
    }
    std::string sum;
    {
        std::ifstream in(sumFile);
        in >> sum;
    }
    if (sum.size() != 64 ||
        !std::equal(sum.begin(), sum.end(), digest.begin() + 7,
                    [](char a, char b) {
                        return std::tolower(static_cast<unsigned char>(a)) ==
                               std::tolower(static_cast<unsigned char>(b));
                    })) {
        cleanup();
        return "The download did not match the published checksum.";
    }

    const int extracted =
        SpawnWait({"7z", "e", "-y", "-o" + root.string(), archive.string(),
                   "alienfx_cli"},
                  nullptr);
    const std::filesystem::path binary = root / "alienfx_cli";
    if (extracted != 0 || !std::filesystem::is_regular_file(binary)) {
        cleanup();
        return "Could not extract alienfx_cli. 7z is required for that.";
    }
    {
        std::ifstream in(binary, std::ios::binary);
        char magic[4] = {};
        in.read(magic, 4);
        if (!in || magic[0] != 0x7F || magic[1] != 'E' || magic[2] != 'L' ||
            magic[3] != 'F') {
            cleanup();
            return "The release file is not a program.";
        }
    }

    std::error_code ec;
    std::filesystem::create_directories(std::filesystem::path(dest).parent_path(),
                                        ec);
    if (ec) {
        cleanup();
        return "Could not create ~/.local/bin.";
    }
    const std::filesystem::path staged = std::filesystem::path(dest + ".new");
    std::filesystem::copy_file(binary, staged,
                               std::filesystem::copy_options::overwrite_existing,
                               ec);
    if (ec) {
        cleanup();
        return "Could not write ~/.local/bin/alienfx_cli.";
    }
    std::filesystem::permissions(staged, std::filesystem::perms::owner_all |
                                             std::filesystem::perms::group_read |
                                             std::filesystem::perms::group_exec |
                                             std::filesystem::perms::others_read |
                                             std::filesystem::perms::others_exec,
                                 ec);
    std::filesystem::rename(staged, dest, ec);
    cleanup();
    if (ec)
        return "Could not install ~/.local/bin/alienfx_cli.";
    return {};
}

static void StartAlienFxInstall() {
    {
        std::lock_guard<std::mutex> lock(gCliMu);
        if (gCliBusy)
            return;
        gCliBusy = true;
        gCliError.clear();
    }
    std::thread([] {
        const std::string error = InstallAlienFxRelease();
        std::lock_guard<std::mutex> lock(gCliMu);
        gCliBusy = false;
        gCliError = error;
    }).detach();
}

static int RunAlienFx(const std::string &bin,
                      const std::vector<std::string> &args) {
    std::vector<char *> argv;
    argv.reserve(args.size() + 2);
    argv.push_back(const_cast<char *>(bin.c_str()));
    for (const std::string &arg : args)
        argv.push_back(const_cast<char *>(arg.c_str()));
    argv.push_back(nullptr);

    const pid_t pid = fork();
    if (pid < 0)
        return -1;
    if (pid == 0) {
        execv(bin.c_str(), argv.data());
        _exit(127);
    }
    int status = 0;
    if (waitpid(pid, &status, 0) < 0)
        return -1;
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return -1;
}

static inline uint32_t ToRGB(const ImVec4 &color) {
    return ((uint32_t)(color.x * 255.0F) << 16) |
           ((uint32_t)(color.y * 255.0F) << 8) | ((uint32_t)(color.z * 255.0F));
}
constexpr float kDebounce = 0.5F;

static const char *kChassisModes[] = {"Static", "Breathe",       "Spectrum",
                                      "Wave",   "Rainbow",       "Back and Forth",
                                      "Default blue"};

static bool ChassisColorEnabled(int mode) {
    return mode == 0 || mode == 1 || mode == 3 || mode == 5;
}

struct ColorPickerState {
    ImVec4 backup{1.0F, 1.0F, 1.0F, 1.0F};
    bool open = false;
    bool accept = false;
};

static void ColorEditCommit(const char *id, ImVec4 &color) {
    ImGui::PushID(id);
    static std::unordered_map<ImGuiID, ColorPickerState> pickers;
    const ImGuiID popupId = ImGui::GetID("picker");
    ColorPickerState &state = pickers[popupId];
    if (ImGui::ColorButton("##swatch", color,
                           ImGuiColorEditFlags_NoAlpha |
                               ImGuiColorEditFlags_NoTooltip,
                           ImVec2(34.0F, 28.0F))) {
        state.backup = color;
        state.accept = false;
        state.open = true;
        ImGui::OpenPopup("picker");
    }
    if (ImGui::BeginPopup("picker")) {
        state.open = true;
        ImGui::ColorPicker3("##wheel", reinterpret_cast<float *>(&color),
                            ImGuiColorEditFlags_PickerHueBar |
                                ImGuiColorEditFlags_DisplayRGB);
        ImGui::Spacing();
        ImGui::PushStyleColor(ImGuiCol_Button,
                              ImVec4(0.00F, 0.72F, 0.78F, 1.0F));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImVec4(0.28F, 0.86F, 0.91F, 1.0F));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                              ImVec4(0.00F, 0.58F, 0.64F, 1.0F));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.04F, 0.07F, 0.09F, 1.0F));
        const bool ok = ImGui::Button("OK", ImVec2(88.0F, 30.0F));
        ImGui::PopStyleColor(4);
        ImGui::SameLine();
        const bool cancel = ImGui::Button("Cancel", ImVec2(88.0F, 30.0F));
        if (ok) {
            state.accept = true;
            ImGui::CloseCurrentPopup();
        } else if (cancel) {
            color = state.backup;
            state.accept = true;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    } else if (state.open) {
        if (!state.accept)
            color = state.backup;
        state.open = false;
        state.accept = false;
    }
    ImGui::PopID();
}

static bool AccentButton(const char *label, bool enabled = true) {
    ImGui::BeginDisabled(!enabled);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.00F, 0.72F, 0.78F, 1.0F));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          ImVec4(0.28F, 0.86F, 0.91F, 1.0F));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                          ImVec4(0.00F, 0.58F, 0.64F, 1.0F));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.04F, 0.07F, 0.09F, 1.0F));
    const bool pressed = ImGui::Button(label, ImVec2(112.0F, 34.0F));
    ImGui::PopStyleColor(4);
    ImGui::EndDisabled();
    return pressed && enabled;
}

struct NavSlot {
    ImVec2 min;
    ImVec2 max;
    bool selected;
};

static NavSlot gNavSlots[16];
static int gNavCount = 0;

static bool NavItem(const char *label, bool selected, bool sub) {
    ImGui::SetCursorPosX(sub ? 26.0F : 12.0F);
    const float width = ImGui::GetContentRegionAvail().x - 12.0F;
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          ImVec4(0.12F, 0.15F, 0.18F, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                          ImVec4(0.09F, 0.15F, 0.17F, 1));
    if (selected)
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.62F, 0.95F, 1.0F, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0F);
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0F, 0.5F));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12.0F, 6.0F));
    const bool pressed = ImGui::Button(label, ImVec2(width, 34.0F));
    if (gNavCount < 16) {
        gNavSlots[gNavCount].min = ImGui::GetItemRectMin();
        gNavSlots[gNavCount].max = ImGui::GetItemRectMax();
        gNavSlots[gNavCount].selected = selected;
        ++gNavCount;
    }
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(selected ? 4 : 3);
    return pressed;
}

static void DrawSlidingNavSelection() {
    int selected = -1;
    for (int i = 0; i < gNavCount; ++i) {
        if (gNavSlots[i].selected)
            selected = i;
    }
    if (selected < 0)
        return;

    const ImVec2 targetMin = gNavSlots[selected].min;
    const ImVec2 targetMax = gNavSlots[selected].max;
    static ImVec2 posMin;
    static ImVec2 posMax;
    static bool ready = false;
    if (!ready) {
        posMin = targetMin;
        posMax = targetMax;
        ready = true;
    } else {
        const float t =
            1.0F - std::exp(-ImGui::GetIO().DeltaTime * 14.0F);
        posMin.x += (targetMin.x - posMin.x) * t;
        posMin.y += (targetMin.y - posMin.y) * t;
        posMax.x += (targetMax.x - posMax.x) * t;
        posMax.y += (targetMax.y - posMax.y) * t;
    }

    ImDrawList *draw = ImGui::GetWindowDrawList();
    draw->AddRectFilled(posMin, posMax, IM_COL32(23, 38, 43, 255), 8.0F);
    draw->AddRectFilled(ImVec2(posMin.x, posMin.y + 8.0F),
                        ImVec2(posMin.x + 3.0F, posMax.y - 8.0F),
                        IM_COL32(0, 214, 230, 255), 2.0F);
}

static void NavHeading(const char *text) {
    ImGui::SetCursorPosX(18.0F);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.45F, 0.50F, 0.56F, 1));
    ImGui::TextUnformatted(text);
    ImGui::PopStyleColor();
    ImGui::Dummy(ImVec2(0, 2));
}

static void PageHeader(ImFont &bold, const char *title, const char *detail) {
    ImGui::PushFont(&bold);
    ImGui::TextUnformatted(title);
    ImGui::PopFont();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.62F, 0.67F, 0.73F, 1));
    ImGui::TextWrapped("%s", detail);
    ImGui::PopStyleColor();
    ImGui::Dummy(ImVec2(0, 6));
}

static void BeginCard(const char *id) {
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0F);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(18.0F, 16.0F));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.16F, 0.18F, 0.22F, 1));
    ImGui::BeginChild(id, ImVec2(0, 0),
                      ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Borders |
                          ImGuiChildFlags_AlwaysUseWindowPadding);
}

static void EndCard() {
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
    ImGui::Dummy(ImVec2(0, 10));
}

struct ChassisPane {
    const char *name;
    const char *detail;
    std::vector<uint8_t> zones;
    int mode = 0;
    ImVec4 color = ImVec4(0.62F, 0.66F, 0.70F, 1.0F);
    bool configured = false;
    bool known = false;
    int bright = 100;
    int brightLast = 100;
    float brightTimer = 0.0F;
    std::string status;
};

static std::vector<uint8_t> ZonesInRange(const std::vector<uint8_t> &all,
                                         uint8_t lo, uint8_t hi) {
    std::vector<uint8_t> out;
    for (uint8_t zone : all) {
        if (zone >= lo && zone <= hi)
            out.push_back(zone);
    }
    return out;
}

static std::vector<uint8_t> ZonesForEffect(const std::vector<uint8_t> &zones,
                                          int mode) {
    if (mode != 4)
        return zones;
    // Zone 0x1b is the power button. A chassis rainbow on that one LED runs
    // on its own clock and strobes. Rainbow for it is powerrainbow instead.
    std::vector<uint8_t> out;
    out.reserve(zones.size());
    for (uint8_t zone : zones) {
        if (zone != 0x1b)
            out.push_back(zone);
    }
    return out;
}

static void ApplyConfigured(EffectController &effects,
                            const std::vector<ChassisPane> &panes) {
    std::vector<ChassisProgram> programs;
    for (const ChassisPane &pane : panes) {
        if (!pane.configured || pane.zones.empty())
            continue;
        const std::vector<uint8_t> zones = ZonesForEffect(pane.zones, pane.mode);
        if (zones.empty())
            continue;
        programs.push_back(ChassisProgram{zones, pane.mode, ToRGB(pane.color)});
    }
    effects.ApplyPrograms(programs);
}

static bool DrawBrightnessRow(const char *id, int &value, int &last,
                              float &timer, const std::vector<uint8_t> &zones,
                              EffectController &effects, bool persist,
                              std::string &status) {
    ImGui::PushID(id);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Brightness");
    ImGui::SameLine();
    ImGui::TextDisabled("%d%%", value);
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    if (ImGui::SliderInt("##bright", &value, 0, 100, "%d%%"))
        timer = 0.0F;
    else
        timer += ImGui::GetIO().DeltaTime;
    ImGui::PopItemWidth();
    ImGui::PopID();
    if (zones.empty() || value == last || timer <= kDebounce)
        return false;
    last = value;
    effects.BrightnessZones(static_cast<uint8_t>(value), zones, persist);
    status = "Brightness applied";
    return true;
}

static void DrawEffectPreview(int mode, const ImVec4 &color, int cells) {
    if (cells < 1)
        cells = 1;
    if (cells > 14)
        cells = 14;
    const uint32_t rgb = ToRGB(color);
    const uint8_t er = static_cast<uint8_t>((rgb >> 16) & 0xFF);
    const uint8_t eg = static_cast<uint8_t>((rgb >> 8) & 0xFF);
    const uint8_t eb = static_cast<uint8_t>(rgb & 0xFF);
    const float time = static_cast<float>(ImGui::GetTime());
    const float width = ImGui::GetContentRegionAvail().x;
    const float height = 22.0F;
    const float gap = cells > 1 ? 4.0F : 0.0F;
    const float cellW =
        (width - gap * static_cast<float>(cells - 1)) / static_cast<float>(cells);
    const ImVec2 origin = ImGui::GetCursorScreenPos();
    ImGui::Dummy(ImVec2(width, height));
    ImDrawList *draw = ImGui::GetWindowDrawList();
    for (int i = 0; i < cells; ++i) {
        const float along =
            cells == 1 ? 0.5F
                       : static_cast<float>(i) / static_cast<float>(cells - 1);
        uint8_t sample[3];
        SampleEffectColor(mode, er, eg, eb, along, time, sample);
        const ImVec2 min(origin.x + static_cast<float>(i) * (cellW + gap),
                         origin.y);
        const ImVec2 max(min.x + cellW, min.y + height);
        draw->AddRectFilled(min, max,
                            IM_COL32(sample[0], sample[1], sample[2], 230),
                            5.0F);
    }
}

static bool DrawEffectControls(const char *id, int &mode, ImVec4 &color,
                               int cells) {
    ImGui::PushID(id);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Effect");
    ImGui::SameLine(0, 12.0F);
    ImGui::SetNextItemWidth(220.0F);
    ImGui::Combo("##effect", &mode, kChassisModes, IM_ARRAYSIZE(kChassisModes));
    ImGui::SameLine(0, 18.0F);
    ImGui::BeginDisabled(!ChassisColorEnabled(mode));
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Color");
    ImGui::SameLine(0, 8.0F);
    ColorEditCommit("##color", color);
    ImGui::EndDisabled();
    ImGui::SameLine(0, 18.0F);
    const bool apply = AccentButton("Apply");
    ImGui::Dummy(ImVec2(0, 2.0F));
    DrawEffectPreview(mode, color, cells);
    ImGui::PopID();
    return apply;
}


static bool BoostSlider(const char *id, int &value) {
    ImGui::PushID(id);
    const float width = ImGui::GetContentRegionAvail().x;
    const float hitHeight = 28.0F;
    const float knobR = 8.0F;
    const ImVec2 origin = ImGui::GetCursorScreenPos();
    ImGui::InvisibleButton("##track", ImVec2(width, hitHeight));
    const float trackLeft = origin.x + knobR;
    const float trackWidth = std::max(1.0F, width - knobR * 2.0F);
    bool changed = false;
    if (ImGui::IsItemActive()) {
        float t = (ImGui::GetIO().MousePos.x - trackLeft) / trackWidth;
        if (t < 0.0F)
            t = 0.0F;
        if (t > 1.0F)
            t = 1.0F;
        const int next = static_cast<int>(t * 100.0F + 0.5F);
        if (next != value) {
            value = next;
            changed = true;
        }
    }
    const bool hovered = ImGui::IsItemHovered();
    const float trackH = 6.0F;
    const float trackY = origin.y + (hitHeight - trackH) * 0.5F;
    const float knobX = trackLeft + trackWidth * (static_cast<float>(value) / 100.0F);
    ImDrawList *draw = ImGui::GetWindowDrawList();
    draw->AddRectFilled(ImVec2(trackLeft, trackY),
                        ImVec2(trackLeft + trackWidth, trackY + trackH),
                        IM_COL32(28, 32, 40, 255), 3.0F);
    if (knobX > trackLeft + 1.0F) {
        draw->AddRectFilled(ImVec2(trackLeft, trackY),
                            ImVec2(knobX, trackY + trackH),
                            IM_COL32(0, 186, 200, 255), 3.0F);
    }
    const float knob = hovered || ImGui::IsItemActive() ? 9.0F : 8.0F;
    draw->AddCircleFilled(ImVec2(knobX, trackY + trackH * 0.5F), knob,
                          IM_COL32(236, 248, 250, 255));
    ImGui::PopID();
    return changed;
}

static void DrawFanTile(ImFont &bold, const char *title, int degrees, int rpm,
                        int &boost, int &last, float &timer, Thermals &thermals,
                        bool cpu, bool manual) {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.055F, 0.062F, 0.078F, 1));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.14F, 0.16F, 0.20F, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0F);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16.0F, 14.0F));
    ImGui::BeginChild(title, ImVec2(0, 0),
                      ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Borders |
                          ImGuiChildFlags_AlwaysUseWindowPadding);

    ImGui::AlignTextToFramePadding();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.94F, 0.95F, 0.97F, 1));
    ImGui::TextUnformatted(title);
    ImGui::PopStyleColor();
    HoverTip(cpu ? "The processor fan." : "The graphics fan.");
    ImGui::SameLine();
    const char *tempText = degrees >= 0 ? nullptr : "—";
    char tempBuf[16];
    if (degrees >= 0)
        std::snprintf(tempBuf, sizeof(tempBuf), "%d°C", degrees);
    const char *shown = degrees >= 0 ? tempBuf : tempText;
    ImGui::PushFont(&bold);
    const float tempWidth = ImGui::CalcTextSize(shown).x;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                         ImGui::GetContentRegionAvail().x - tempWidth);
    ImGui::TextUnformatted(shown);
    ImGui::PopFont();
    HoverTip(cpu ? "Processor temperature, read from the CPU sensor."
                 : "Graphics temperature, read from the video sensor.");

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.82F, 0.87F, 0.92F, 1));
    if (rpm >= 0)
        ImGui::Text("%d rpm", rpm);
    else
        ImGui::TextUnformatted("Speed unavailable");
    ImGui::PopStyleColor();
    HoverTip(cpu ? "Current speed of the processor fan."
                 : "Current speed of the graphics fan.");

    const char *boostTip =
        cpu ? "Extra speed for the processor fan, added on top of the thermal "
             "mode. 0% leaves that curve alone, so the fan still spins. 1% is "
             "a small raise above it, not 1% of maximum speed."
            : "Extra speed for the graphics fan, added on top of the thermal "
              "mode. 0% leaves that curve alone, so the fan still spins. 1% "
              "is a small raise above it, not 1% of maximum speed.";
    ImGui::Dummy(ImVec2(0, 8));
    ImGui::TextUnformatted("Extra");
    HoverTip(boostTip);
    ImGui::SameLine();
    char percent[8];
    std::snprintf(percent, sizeof(percent), "%d%%", boost);
    const float percentWidth = ImGui::CalcTextSize(percent).x;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                         ImGui::GetContentRegionAvail().x - percentWidth);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.55F, 0.90F, 0.95F, 1));
    ImGui::TextUnformatted(percent);
    ImGui::PopStyleColor();
    HoverTip(manual ? boostTip
                    : "The custom curve is setting this. Turn the curve off "
                      "to move it yourself.");
    if (!manual)
        ImGui::BeginDisabled();
    if (BoostSlider(cpu ? "cpu" : "gpu", boost))
        timer = 0.0F;
    else
        timer += ImGui::GetIO().DeltaTime;
    if (!manual)
        ImGui::EndDisabled();
    if (manual && boost != last && timer > kDebounce) {
        last = boost;
        if (cpu)
            thermals.setCpuBoost(boost);
        else
            thermals.setGpuBoost(boost);
    }
    HoverTip(manual ? boostTip
                    : "The custom curve is setting this. Turn the curve off "
                      "to move it yourself.");

    ImGui::EndChild();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(2);
}

static void DrawCurveGraph(const FanCurvePoint *points) {
    const float width = ImGui::GetContentRegionAvail().x;
    const float height = 108.0F;
    const ImVec2 origin = ImGui::GetCursorScreenPos();
    ImGui::Dummy(ImVec2(width, height));
    ImDrawList *draw = ImGui::GetWindowDrawList();
    draw->AddRectFilled(origin, ImVec2(origin.x + width, origin.y + height),
                        IM_COL32(16, 18, 24, 255), 8.0F);
    FanCurvePoint sorted[4];
    std::copy(points, points + 4, sorted);
    std::sort(sorted, sorted + 4, [](const FanCurvePoint &a, const FanCurvePoint &b) {
        return a.tempC < b.tempC;
    });
    auto pointAt = [&](int tempC, int extra) {
        const float x =
            origin.x + 10.0F +
            (width - 20.0F) * (static_cast<float>(std::clamp(tempC, 30, 100) - 30) / 70.0F);
        const float y =
            origin.y + height - 10.0F -
            (height - 20.0F) * (static_cast<float>(std::clamp(extra, 0, 100)) / 100.0F);
        return ImVec2(x, y);
    };
    for (int i = 0; i < 3; ++i) {
        draw->AddLine(pointAt(sorted[i].tempC, sorted[i].extra),
                      pointAt(sorted[i + 1].tempC, sorted[i + 1].extra),
                      IM_COL32(0, 186, 200, 255), 2.0F);
    }
    for (int i = 0; i < 4; ++i)
        draw->AddCircleFilled(pointAt(sorted[i].tempC, sorted[i].extra), 4.0F,
                              IM_COL32(236, 248, 250, 255));
}

static bool DrawCurvePoints(const char *id, FanCurvePoint *points) {
    bool changed = false;
    ImGui::PushID(id);
    DrawCurveGraph(points);
    ImGui::Dummy(ImVec2(0, 6));
    for (int i = 0; i < 4; ++i) {
        ImGui::PushID(i);
        ImGui::SetNextItemWidth(72.0F);
        if (ImGui::InputInt("°C", &points[i].tempC, 0, 0))
            changed = true;
        points[i].tempC = std::clamp(points[i].tempC, 30, 105);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::SliderInt("##extra", &points[i].extra, 0, 100, "Extra %d%%"))
            changed = true;
        points[i].extra = std::clamp(points[i].extra, 0, 100);
        ImGui::PopID();
    }
    ImGui::PopID();
    return changed;
}

static void DrawSessionBar(ImFont &fontbold, bool &requestHide,
                           bool &requestQuit) {
    ImGui::Dummy(ImVec2(0, 16));
    const float inset = 16.0F;
    ImGui::SetCursorPosX(inset);
    const float barWidth = ImGui::GetContentRegionAvail().x - inset;
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.06F, 0.10F, 0.12F, 1));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00F, 0.55F, 0.60F, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(18.0F, 14.0F));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0F);
    ImGui::BeginChild("session-bar", ImVec2(barWidth, 0),
                      ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Borders |
                          ImGuiChildFlags_AlwaysUseWindowPadding);
    ImGui::PushFont(&fontbold);
    ImGui::AlignTextToFramePadding();
    bool startup = AutostartEnabled();
    if (ImGui::Checkbox("Launch on startup", &startup))
        SetAutostartEnabled(startup);
    ImGui::PopFont();
    HoverTip("Starts AWCC when you sign in, so a fan curve is already "
             "running. Quitting still stops it until the next sign-in.");
    ImGui::SameLine(0, 28.0F);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.00F, 0.72F, 0.78F, 1.0F));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          ImVec4(0.28F, 0.86F, 0.91F, 1.0F));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                          ImVec4(0.00F, 0.58F, 0.64F, 1.0F));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.04F, 0.07F, 0.09F, 1.0F));
    if (ImGui::Button("Hide to background", ImVec2(190.0F, 34.0F)))
        requestHide = true;
    ImGui::PopStyleColor(4);
    HoverTip("Hides this window. AWCC keeps running. Open AWCC again to show "
             "it. The X in the title bar does the same thing.");
    ImGui::SameLine();
    const float quitWidth = 148.0F;
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - quitWidth -
                         ImGui::GetStyle().WindowPadding.x);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.45F, 0.16F, 0.18F, 1.0F));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          ImVec4(0.62F, 0.22F, 0.24F, 1.0F));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                          ImVec4(0.36F, 0.12F, 0.14F, 1.0F));
    if (ImGui::Button("Quit AWCC", ImVec2(quitWidth, 34.0F)))
        requestQuit = true;
    ImGui::PopStyleColor(3);
    HoverTip("Stops AWCC completely. The fan curve stops with it. The X only "
             "hides the window.");
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.82F, 0.90F, 0.92F, 1));
    ImGui::TextWrapped(
        "The X and Hide to background only hide this window. Quit AWCC stops "
        "the app and the fan curve.");
    ImGui::PopStyleColor();
    ImGui::EndChild();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(2);
}

static void DrawPerformance(Thermals &thermals, AcpiUtils &acpiUtils,
                            int &selectedMode, int &gpuBoost, int &cpuBoost,
                            ImFont &fontbold, bool &turbo, FanCurve &curve) {
    static int cpuLast = -1;
    static float cpuTimer = 0.0F;
    static int gpuLast = -1;
    static float gpuTimer = 0.0F;
    if (cpuLast < 0)
        cpuLast = cpuBoost;
    if (gpuLast < 0)
        gpuLast = gpuBoost;

    static FanSensors fans;
    static float fanPoll = 1.0F;
    fanPoll += ImGui::GetIO().DeltaTime;
    if (fanPoll >= 1.0F) {
        fans = ReadFanSensors();
        fanPoll = 0.0F;
    }
    if (curve.enabled) {
        cpuLast = cpuBoost;
        gpuLast = gpuBoost;
    }

    PageHeader(fontbold, "Performance",
               "Thermal mode, fan boost, and processor turbo.");

    BeginCard("perf-modes");
    ImGui::TextUnformatted("Thermal mode");
    HoverTip("Picks the firmware profile for power limits and fan behavior. "
             "One profile covers the processor, the graphics chip, and both "
             "fans.");
    ImGui::Dummy(ImVec2(0, 6));
    const char *firstMode =
        acpiUtils.hasThermalMode(ThermalModeSet::BatterySaver) ? "Battery"
                                                               : "Cool";
    const char *labels[] = {firstMode, "Quiet", "Balanced", "Performance",
                            "G-mode"};
    ModeButtonList(labels, thermals, acpiUtils, selectedMode);
    EndCard();

    BeginCard("perf-fans");
    ImGui::TextUnformatted("Fans");
    HoverTip("Live temperature and fan speed. The sliders add fan speed on "
             "top of the thermal profile.");
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.78F, 0.83F, 0.88F, 1));
    ImGui::TextWrapped(
        "%s",
        "RPM is how fast the fan is spinning right now. Extra adds speed on "
        "top of the thermal mode. At 0, only the thermal mode is in control, "
        "and the fan still spins.");
    ImGui::PopStyleColor();
    ImGui::Dummy(ImVec2(0, 8));
    const float columnWidth =
        (ImGui::GetContentRegionAvail().x - 12.0F) * 0.5F;
    ImGui::BeginChild("cpu-slot", ImVec2(columnWidth, 0),
                      ImGuiChildFlags_AutoResizeY);
    DrawFanTile(fontbold, "CPU", fans.cpuC, fans.cpuRpm, cpuBoost, cpuLast,
                cpuTimer, thermals, true, !curve.enabled);
    ImGui::EndChild();
    ImGui::SameLine(0, 12.0F);
    ImGui::BeginChild("gpu-slot", ImVec2(columnWidth, 0),
                      ImGuiChildFlags_AutoResizeY);
    DrawFanTile(fontbold, "GPU", fans.gpuC, fans.gpuRpm, gpuBoost, gpuLast,
                gpuTimer, thermals, false, !curve.enabled);
    ImGui::EndChild();
    EndCard();

    BeginCard("perf-curve");
    ImGui::PushFont(&fontbold);
    ImGui::TextUnformatted("Custom curve");
    ImGui::PopFont();
    ImGui::SameLine();
    bool enabled = curve.enabled;
    const float checkWidth = ImGui::GetFrameHeight();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - checkWidth -
                         ImGui::GetStyle().WindowPadding.x);
    if (ImGui::Checkbox("##curve", &enabled)) {
        curve.enabled = enabled;
        curve.save();
    }
    HoverTip("While this is on, temperature sets Extra for each fan. It keeps "
             "running after the window hides, and stops when you quit.");
    if (curve.enabled && (curve.seenCpuC >= 0 || curve.seenGpuC >= 0)) {
        ImGui::Dummy(ImVec2(0, 4));
        if (curve.seenCpuC >= 0)
            ImGui::Text("CPU %d°C is setting Extra to %d%%.", curve.seenCpuC,
                        cpuBoost);
        if (curve.seenGpuC >= 0)
            ImGui::Text("GPU %d°C is setting Extra to %d%%.", curve.seenGpuC,
                        gpuBoost);
    }
    ImGui::Dummy(ImVec2(0, 8));
    const float curveWidth = (ImGui::GetContentRegionAvail().x - 16.0F) * 0.5F;
    bool curveChanged = false;
    ImGui::BeginChild("cpu-curve", ImVec2(curveWidth, 0),
                      ImGuiChildFlags_AutoResizeY);
    ImGui::TextUnformatted("CPU");
    curveChanged = DrawCurvePoints("cpu", curve.cpu) || curveChanged;
    ImGui::EndChild();
    ImGui::SameLine(0, 16.0F);
    ImGui::BeginChild("gpu-curve", ImVec2(curveWidth, 0),
                      ImGuiChildFlags_AutoResizeY);
    ImGui::TextUnformatted("GPU");
    curveChanged = DrawCurvePoints("gpu", curve.gpu) || curveChanged;
    ImGui::EndChild();
    if (curveChanged)
        curve.save();
    EndCard();

    BeginCard("perf-turbo");
    ImGui::BeginGroup();
    ImGui::PushFont(&fontbold);
    ImGui::TextUnformatted("Turbo boost");
    ImGui::PopFont();
    HoverTip("Allows the processor to clock above its base frequency while "
             "power and temperature allow it. Off, the processor stays at its "
             "base clock. This is separate from the thermal profile and from "
             "the fans.");
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.78F, 0.83F, 0.88F, 1));
    ImGui::TextUnformatted("Raise the processor's maximum frequency.");
    ImGui::PopStyleColor();
    HoverTip("Allows the processor to clock above its base frequency while "
             "power and temperature allow it. Off, the processor stays at its "
             "base clock. This is separate from the thermal profile and from "
             "the fans.");
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - checkWidth -
                         ImGui::GetStyle().WindowPadding.x);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.0F);
    if (ImGui::Checkbox("##turbo", &turbo))
        acpiUtils.setTurboBoost(turbo);
    HoverTip("Allows the processor to clock above its base frequency while "
             "power and temperature allow it. Off, the processor stays at its "
             "base clock. This is separate from the thermal profile and from "
             "the fans.");
    EndCard();
}

static const char *kKeyboardEffects[] = {
    "Static",        "Breathe", "Spectrum",    "Wave",
    "Rainbow",       "Back and Forth", "Default blue", "Pulse"};

struct KeyboardShow {
    int effect = 0;
    uint8_t r = 255;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t color[256][3]{};
    bool ready = false;
};

static KeyboardShow gKeys;
static bool gKeyboardKnown = false;
static bool gPowerOwn = false;
static bool gPowerKnown = false;
static ImVec4 gPowerAc = ImVec4(0.62F, 0.66F, 0.70F, 1.F);
static ImVec4 gPowerBatt = ImVec4(0.62F, 0.66F, 0.70F, 1.F);
static bool gTrackOwn = false;
static int gTrackMode = 0;
static ImVec4 gTrackColor = ImVec4(0.62F, 0.66F, 0.70F, 1.F);
static ImVec4 gKeyColor = ImVec4(0.62F, 0.66F, 0.70F, 1.F);
static bool gAllKnown = false;
static int gAllMode = 0;
static ImVec4 gAllColor = ImVec4(0.62F, 0.66F, 0.70F, 1.F);

static std::filesystem::path LightingFile() {
    if (const char *xdg = std::getenv("XDG_CONFIG_HOME");
        xdg != nullptr && xdg[0] != '\0')
        return std::filesystem::path(xdg) / "awcc" / "lighting.json";
    const char *home = std::getenv("HOME");
    return std::filesystem::path(home != nullptr ? home : ".") / ".config" /
           "awcc" / "lighting.json";
}

static nlohmann::json ColorJson(const ImVec4 &color) {
    const uint32_t rgb = ToRGB(color);
    return nlohmann::json::array({(rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF});
}

static ImVec4 ColorFromJson(const nlohmann::json &value, const ImVec4 &fallback) {
    if (!value.is_array() || value.size() < 3)
        return fallback;
    return ImVec4(std::clamp(value[0].get<int>(), 0, 255) / 255.0F,
                  std::clamp(value[1].get<int>(), 0, 255) / 255.0F,
                  std::clamp(value[2].get<int>(), 0, 255) / 255.0F, 1.0F);
}

static void SaveLighting(const std::vector<ChassisPane> &panes) {
    nlohmann::json json;
    json["keyboard"] = {
        {"known", gKeyboardKnown},
        {"effect", gKeys.effect},
        {"color", ColorJson(gKeyColor)},
    };
    if (gKeyboardKnown) {
        nlohmann::json keys = nlohmann::json::array();
        for (int i = 0; i < 0xB4; ++i) {
            keys.push_back(nlohmann::json::array(
                {gKeys.color[i][0], gKeys.color[i][1], gKeys.color[i][2]}));
        }
        json["keyboard"]["keys"] = std::move(keys);
        json["keyboard"]["powerKey"] = nlohmann::json::array(
            {gKeys.color[kKeyboardPowerId][0], gKeys.color[kKeyboardPowerId][1],
             gKeys.color[kKeyboardPowerId][2]});
    }
    json["power"] = {{"known", gPowerKnown},
                     {"own", gPowerOwn},
                     {"ac", ColorJson(gPowerAc)},
                     {"battery", ColorJson(gPowerBatt)}};
    json["trackpad"] = {{"own", gTrackOwn},
                        {"mode", gTrackMode},
                        {"color", ColorJson(gTrackColor)}};
    json["all"] = {{"known", gAllKnown},
                   {"mode", gAllMode},
                   {"color", ColorJson(gAllColor)}};
    json["panes"] = nlohmann::json::object();
    for (const ChassisPane &pane : panes) {
        json["panes"][pane.name] = {{"known", pane.known},
                                    {"configured", pane.configured},
                                    {"mode", pane.mode},
                                    {"color", ColorJson(pane.color)}};
    }
    const std::filesystem::path file = LightingFile();
    std::error_code error;
    std::filesystem::create_directories(file.parent_path(), error);
    std::ofstream out(file, std::ios::trunc);
    if (out)
        out << json.dump(2) << '\n';
}

static void EnsureKeyboardShow();

static void LoadLighting(std::vector<ChassisPane> &panes) {
    static bool loaded = false;
    if (loaded)
        return;
    loaded = true;
    EnsureKeyboardShow();
    std::ifstream in(LightingFile());
    if (!in)
        return;
    nlohmann::json json;
    try {
        in >> json;
    } catch (const nlohmann::json::exception &) {
        return;
    }
    if (json.contains("keyboard") && json["keyboard"].is_object()) {
        const nlohmann::json &keyboard = json["keyboard"];
        gKeyboardKnown = keyboard.value("known", false);
        gKeys.effect = std::clamp(keyboard.value("effect", 0), 0, 7);
        gKeyColor = ColorFromJson(keyboard.value("color", nlohmann::json()), gKeyColor);
        const uint32_t rgb = ToRGB(gKeyColor);
        gKeys.r = static_cast<uint8_t>((rgb >> 16) & 0xFF);
        gKeys.g = static_cast<uint8_t>((rgb >> 8) & 0xFF);
        gKeys.b = static_cast<uint8_t>(rgb & 0xFF);
        if (gKeyboardKnown && keyboard.contains("keys") &&
            keyboard["keys"].is_array()) {
            const int count = std::min(0xB4, static_cast<int>(keyboard["keys"].size()));
            for (int i = 0; i < count; ++i) {
                const nlohmann::json &entry = keyboard["keys"][i];
                if (!entry.is_array() || entry.size() < 3)
                    continue;
                gKeys.color[i][0] = static_cast<uint8_t>(std::clamp(entry[0].get<int>(), 0, 255));
                gKeys.color[i][1] = static_cast<uint8_t>(std::clamp(entry[1].get<int>(), 0, 255));
                gKeys.color[i][2] = static_cast<uint8_t>(std::clamp(entry[2].get<int>(), 0, 255));
            }
        }
        if (keyboard.contains("powerKey")) {
            const ImVec4 power = ColorFromJson(keyboard["powerKey"], gKeyColor);
            const uint32_t powerRgb = ToRGB(power);
            gKeys.color[kKeyboardPowerId][0] = static_cast<uint8_t>((powerRgb >> 16) & 0xFF);
            gKeys.color[kKeyboardPowerId][1] = static_cast<uint8_t>((powerRgb >> 8) & 0xFF);
            gKeys.color[kKeyboardPowerId][2] = static_cast<uint8_t>(powerRgb & 0xFF);
        }
    }
    if (json.contains("power") && json["power"].is_object()) {
        const nlohmann::json &power = json["power"];
        gPowerKnown = power.value("known", false);
        gPowerOwn = power.value("own", false);
        gPowerAc = ColorFromJson(power.value("ac", nlohmann::json()), gPowerAc);
        gPowerBatt = ColorFromJson(power.value("battery", nlohmann::json()), gPowerBatt);
    }
    if (json.contains("trackpad") && json["trackpad"].is_object()) {
        const nlohmann::json &track = json["trackpad"];
        gTrackOwn = track.value("own", false);
        gTrackMode = std::clamp(track.value("mode", 0), 0, 6);
        gTrackColor = ColorFromJson(track.value("color", nlohmann::json()), gTrackColor);
    }
    if (json.contains("all") && json["all"].is_object()) {
        const nlohmann::json &all = json["all"];
        gAllKnown = all.value("known", false);
        gAllMode = std::clamp(all.value("mode", 0), 0, 6);
        gAllColor = ColorFromJson(all.value("color", nlohmann::json()), gAllColor);
    }
    if (json.contains("panes") && json["panes"].is_object()) {
        for (ChassisPane &pane : panes) {
            if (!json["panes"].contains(pane.name))
                continue;
            const nlohmann::json &saved = json["panes"][pane.name];
            pane.known = saved.value("known", false);
            pane.configured = saved.value("configured", false);
            pane.mode = std::clamp(saved.value("mode", 0), 0, 6);
            pane.color = ColorFromJson(saved.value("color", nlohmann::json()), pane.color);
        }
    }
}

static void LightingRecallLine(bool known) {
    ImGui::TextDisabled("%s", known ? "Last set in AWCC. A terminal command "
                                      "is not shown until you Apply here."
                                    : "Not set in AWCC yet.");
}

static void EnsureKeyboardShow() {
    if (gKeys.ready)
        return;
    for (int i = 0; i < 256; ++i) {
        gKeys.color[i][0] = 158;
        gKeys.color[i][1] = 164;
        gKeys.color[i][2] = 172;
    }
    gKeys.color[kKeyboardPowerId][0] = 158;
    gKeys.color[kKeyboardPowerId][1] = 164;
    gKeys.color[kKeyboardPowerId][2] = 172;
    gKeys.ready = true;
}

static void NoteKeyboardEffect(int effect, int r, int g, int b) {
    EnsureKeyboardShow();
    if (effect == 6) {
        r = 0;
        g = 255;
        b = 255;
    }
    gKeys.effect = effect;
    gKeys.r = static_cast<uint8_t>(r);
    gKeys.g = static_cast<uint8_t>(g);
    gKeys.b = static_cast<uint8_t>(b);
    gKeyboardKnown = true;
    gPowerKnown = true;
    if (effect == 0 || effect == 6) {
        for (int i = 0; i < 0xB4; ++i) {
            gKeys.color[i][0] = gKeys.r;
            gKeys.color[i][1] = gKeys.g;
            gKeys.color[i][2] = gKeys.b;
        }
        gKeys.color[kKeyboardPowerId][0] = gKeys.r;
        gKeys.color[kKeyboardPowerId][1] = gKeys.g;
        gKeys.color[kKeyboardPowerId][2] = gKeys.b;
    }
    gPowerOwn = false;
    gPowerAc = ImVec4(r / 255.0F, g / 255.0F, b / 255.0F, 1.0F);
    gPowerBatt = gPowerAc;
    gTrackOwn = false;
}

static ChassisPane *TrackPane(std::vector<ChassisPane> &panes) {
    for (ChassisPane &pane : panes) {
        if (!pane.zones.empty() && pane.zones.front() == 0x1F)
            return &pane;
    }
    return nullptr;
}

static ChassisPane *PowerPane(std::vector<ChassisPane> &panes) {
    for (ChassisPane &pane : panes) {
        for (uint8_t zone : pane.zones) {
            if (zone == 0x1b)
                return &pane;
        }
    }
    return nullptr;
}

static void ReleasePowerFromAnimation(EffectController &effects,
                                      std::vector<ChassisPane> &panes) {
    if (ChassisPane *power = PowerPane(panes))
        power->configured = false;
    bool any = false;
    for (const ChassisPane &pane : panes) {
        if (pane.configured && !pane.zones.empty())
            any = true;
    }
    if (any)
        ApplyConfigured(effects, panes);
    else
        effects.ClearPrograms();
}

static void KeyboardTakesDeck(EffectController &effects,
                              std::vector<ChassisPane> &panes,
                              const std::string &cli, int effect, int r, int g,
                              int b) {
    gPowerOwn = false;
    gPowerKnown = true;
    gTrackOwn = false;
    gPowerAc = ImVec4(r / 255.0F, g / 255.0F, b / 255.0F, 1.0F);
    gPowerBatt = gPowerAc;
    gTrackColor = gPowerAc;
    gTrackMode = effect == 7 ? 0 : effect;
    const int chassisMode = gTrackMode;
    bool wrote = false;
    if (ChassisPane *power = PowerPane(panes)) {
        power->configured = true;
        power->known = true;
        power->mode = chassisMode;
        power->color = gPowerAc;
        wrote = true;
    }
    if (ChassisPane *track = TrackPane(panes)) {
        track->configured = true;
        track->known = true;
        track->mode = chassisMode;
        track->color = gPowerAc;
        wrote = true;
    }
    if (wrote)
        ApplyConfigured(effects, panes);
    if (chassisMode == 0 || chassisMode == 6) {
        const std::string channel = std::to_string(r);
        const std::string green = std::to_string(g);
        const std::string blue = std::to_string(b);
        RunAlienFx(cli, {"powerbutton", channel, green, blue, channel, green,
                         blue});
    } else if (chassisMode == 4) {
        RunAlienFx(cli, {"powerrainbow"});
    }
}

static const char *KeyboardCliEffect(int effect) {
    switch (effect) {
    case 1:
        return "breathe";
    case 2:
        return "spectrum";
    case 3:
        return "wave";
    case 4:
        return "rainbow";
    case 5:
        return "wave";
    case 7:
        return "pulse";
    default:
        return nullptr;
    }
}

static void DrawKeyboardPage(ImFont &fontbold, ImFont &smallFont,
                             EffectController &effects,
                             std::vector<ChassisPane> &panes) {
    PageHeader(fontbold, "Keyboard",
               "Click a key, Ctrl-click to add keys, or drag across a row. "
               "Apply here includes the power button and the trackpad. "
               "The drawing is a simple preview of the effect selected here.");

    EnsureKeyboardShow();
    static bool keySelected[256]{};
    static int keyBright = 100;
    static int keyBrightLast = 100;
    static float keyBrightTimer = 0.0F;
    static std::string keyStatus;

    const std::string cli = AlienFxCliPath();
    BeginCard("kb-controls");
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Effect");
    ImGui::SameLine(0, 12.0F);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::Combo("##keyeffect", &gKeys.effect, kKeyboardEffects,
                 IM_ARRAYSIZE(kKeyboardEffects), IM_ARRAYSIZE(kKeyboardEffects));
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Color");
    ImGui::SameLine(0, 8.0F);
    ColorEditCommit("##keycolor", gKeyColor);
    ImGui::SameLine(0, 18.0F);
    if (ImGui::Button("Select all", ImVec2(110, 34))) {
        for (int i = 0; i < 256; ++i)
            keySelected[i] = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear", ImVec2(90, 34))) {
        for (int i = 0; i < 256; ++i)
            keySelected[i] = false;
    }
    ImGui::SameLine();
    if (AccentButton("Apply", !cli.empty())) {
        const uint32_t color32 = ToRGB(gKeyColor);
        int r = static_cast<int>((color32 >> 16) & 0xFF);
        int g = static_cast<int>((color32 >> 8) & 0xFF);
        int b = static_cast<int>(color32 & 0xFF);
        int rc = 1;
        const int keyEffect = gKeys.effect;
        if (keyEffect == 6) {
            r = 0;
            g = 255;
            b = 255;
        }
        if (keyEffect == 0 || keyEffect == 6) {
            bool any = false;
            if (keyEffect != 6) {
                for (int i = 0; i < 256; ++i) {
                    if (keySelected[i])
                        any = true;
                }
            }
            const bool paintPower =
                keyEffect == 6 || !any || keySelected[kKeyboardPowerId];
            for (int i = 0; i < 0xB4; ++i) {
                if (any && !keySelected[i])
                    continue;
                gKeys.color[i][0] = static_cast<uint8_t>(r);
                gKeys.color[i][1] = static_cast<uint8_t>(g);
                gKeys.color[i][2] = static_cast<uint8_t>(b);
            }
            if (paintPower) {
                gKeys.color[kKeyboardPowerId][0] = static_cast<uint8_t>(r);
                gKeys.color[kKeyboardPowerId][1] = static_cast<uint8_t>(g);
                gKeys.color[kKeyboardPowerId][2] = static_cast<uint8_t>(b);
            }
            std::vector<std::string> args{"setkeys"};
            for (int i = 0; i < 0xB4; ++i) {
                args.push_back(std::to_string(i));
                args.push_back(std::to_string(gKeys.color[i][0]));
                args.push_back(std::to_string(gKeys.color[i][1]));
                args.push_back(std::to_string(gKeys.color[i][2]));
            }
            rc = RunAlienFx(cli, args);
            if (rc == 0 && paintPower)
                KeyboardTakesDeck(effects, panes, cli, keyEffect, r, g, b);
            if (rc == 0) {
                gKeys.effect = keyEffect;
                gKeys.r = static_cast<uint8_t>(r);
                gKeys.g = static_cast<uint8_t>(g);
                gKeys.b = static_cast<uint8_t>(b);
                gKeyboardKnown = true;
                gKeyColor = ImVec4(r / 255.0F, g / 255.0F, b / 255.0F, 1.0F);
                SaveLighting(panes);
                keyStatus = "Keyboard colors applied";
            } else {
                keyStatus = "Could not set the keys";
            }
        } else {
            const char *name = KeyboardCliEffect(keyEffect);
            rc = RunAlienFx(cli, {"keyboardeffect", name, std::to_string(r),
                                  std::to_string(g), std::to_string(b)});
            if (rc == 0) {
                gKeys.r = static_cast<uint8_t>(r);
                gKeys.g = static_cast<uint8_t>(g);
                gKeys.b = static_cast<uint8_t>(b);
                gKeyColor = ImVec4(r / 255.0F, g / 255.0F, b / 255.0F, 1.0F);
                gKeyboardKnown = true;
                KeyboardTakesDeck(effects, panes, cli, keyEffect, r, g, b);
                SaveLighting(panes);
                keyStatus = "Keyboard effect applied";
            } else {
                keyStatus = "Could not set the effect";
            }
        }
    }
    if (cli.empty())
        ImGui::TextDisabled("alienfx_cli is not installed");
    else if (!keyStatus.empty())
        ImGui::TextDisabled("%s", keyStatus.c_str());
    LightingRecallLine(gKeyboardKnown);
    EndCard();

    BeginCard("kb-map");
    ImGui::PushFont(&smallFont);
    const uint32_t shown = ToRGB(gKeyColor);
    const uint8_t showR = gKeys.effect == 0 ? gKeys.r
                                            : static_cast<uint8_t>((shown >> 16) & 0xFF);
    const uint8_t showG = gKeys.effect == 0 ? gKeys.g
                                            : static_cast<uint8_t>((shown >> 8) & 0xFF);
    const uint8_t showB = gKeys.effect == 0 ? gKeys.b
                                            : static_cast<uint8_t>(shown & 0xFF);
    const uint32_t powerShown = ToRGB(gPowerAc);
    const uint32_t trackShown = ToRGB(gTrackColor);
    DrawArea51Keyboard(
        keySelected, gKeys.color, gKeys.effect, showR, showG, showB, gPowerOwn,
        static_cast<uint8_t>((powerShown >> 16) & 0xFF),
        static_cast<uint8_t>((powerShown >> 8) & 0xFF),
        static_cast<uint8_t>(powerShown & 0xFF), gTrackOwn, gTrackMode,
        static_cast<uint8_t>((trackShown >> 16) & 0xFF),
        static_cast<uint8_t>((trackShown >> 8) & 0xFF),
        static_cast<uint8_t>(trackShown & 0xFF));
    ImGui::PopFont();
    EndCard();

    BeginCard("kb-bright");
    ImGui::TextUnformatted("Keyboard brightness");
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    if (ImGui::SliderInt("##keybright", &keyBright, 0, 100, "%d%%"))
        keyBrightTimer = 0.0F;
    else
        keyBrightTimer += ImGui::GetIO().DeltaTime;
    ImGui::PopItemWidth();
    if (!cli.empty() && keyBright != keyBrightLast &&
        keyBrightTimer > kDebounce) {
        keyBrightLast = keyBright;
        const int level = keyBright * 255 / 100;
        const int rc =
            RunAlienFx(cli, {"keyboarddim", std::to_string(level)});
        keyStatus = (rc == 0) ? "Keyboard brightness applied"
                              : "Could not set keyboard brightness";
    }
    EndCard();
}

static void DrawChassisPane(ChassisPane &pane, EffectController &effects,
                            std::vector<ChassisPane> &panes, ImFont &fontbold) {
    PageHeader(fontbold, pane.name, pane.detail);
    if (pane.zones.empty()) {
        BeginCard("missing-zone");
        ImGui::TextWrapped(
            "This model has no zones mapped for %s.", pane.name);
        EndCard();
        return;
    }

    BeginCard("zone-effect");
    if (DrawEffectControls(pane.name, pane.mode, pane.color,
                           static_cast<int>(pane.zones.size()))) {
        pane.configured = true;
        pane.known = true;
        ApplyConfigured(effects, panes);
        if (!pane.zones.empty() && pane.zones.front() == 0x1F) {
            gTrackOwn = true;
            gTrackMode = pane.mode;
            gTrackColor = pane.color;
        }
        SaveLighting(panes);
        pane.status = "Applied";
    }
    if (!pane.status.empty())
        ImGui::TextDisabled("%s", pane.status.c_str());
    ImGui::TextDisabled(
        "Other chassis lights stay as you last applied them in AWCC.");
    LightingRecallLine(pane.known);
    EndCard();

    BeginCard("zone-bright");
    DrawBrightnessRow(pane.name, pane.bright, pane.brightLast, pane.brightTimer,
                      pane.zones, effects, false, pane.status);
    EndCard();
}

static void DrawPowerPage(ImFont &fontbold, EffectController &effects,
                          std::vector<ChassisPane> &panes) {
    PageHeader(fontbold, "Power",
               "Plugged-in and battery colors for the power button only. "
               "The other keys stay as they are. A later keyboard Apply "
               "replaces these colors.");
    static std::string powerStatus;
    const std::string cli = AlienFxCliPath();

    BeginCard("power-colors");
    ImGui::TextUnformatted("Plugged in");
    ImGui::SameLine(0, 8.0F);
    ColorEditCommit("##acpower", gPowerAc);
    ImGui::SameLine(0, 24.0F);
    ImGui::TextUnformatted("Battery");
    ImGui::SameLine(0, 8.0F);
    ColorEditCommit("##battpower", gPowerBatt);
    ImGui::SameLine(0, 24.0F);
    if (AccentButton("Apply##power", !cli.empty())) {
        const uint32_t ac = ToRGB(gPowerAc);
        const uint32_t batt = ToRGB(gPowerBatt);
        const int rc = RunAlienFx(
            cli, {"powerbutton", std::to_string((ac >> 16) & 0xFF),
                  std::to_string((ac >> 8) & 0xFF), std::to_string(ac & 0xFF),
                  std::to_string((batt >> 16) & 0xFF),
                  std::to_string((batt >> 8) & 0xFF),
                  std::to_string(batt & 0xFF)});
        if (rc == 0) {
            gPowerOwn = true;
            gPowerKnown = true;
            ReleasePowerFromAnimation(effects, panes);
            gKeys.color[kKeyboardPowerId][0] =
                static_cast<uint8_t>((ac >> 16) & 0xFF);
            gKeys.color[kKeyboardPowerId][1] =
                static_cast<uint8_t>((ac >> 8) & 0xFF);
            gKeys.color[kKeyboardPowerId][2] =
                static_cast<uint8_t>(ac & 0xFF);
            SaveLighting(panes);
            powerStatus = "Power button colors applied";
        } else {
            powerStatus = "Could not set the power button";
        }
    }
    if (cli.empty())
        ImGui::TextDisabled("alienfx_cli is not installed");
    else if (!powerStatus.empty())
        ImGui::TextDisabled("%s", powerStatus.c_str());
    LightingRecallLine(gPowerKnown);
    EndCard();
}

static int ApplyKeyboardForMode(const std::string &cli, int mode, int r,
                                int g, int b) {
    if (cli.empty())
        return 1;
    if (mode == 6) {
        r = 0;
        g = 255;
        b = 255;
    }
    if (mode == 0 || mode == 6) {
        std::vector<std::string> args{"setkeys"};
        for (int i = 0; i < 0xB4; ++i) {
            args.push_back(std::to_string(i));
            args.push_back(std::to_string(r));
            args.push_back(std::to_string(g));
            args.push_back(std::to_string(b));
        }
        const int rc = RunAlienFx(cli, args);
        if (rc == 0) {
            NoteKeyboardEffect(mode, r, g, b);
            const std::string rs = std::to_string(r);
            const std::string gs = std::to_string(g);
            const std::string bs = std::to_string(b);
            RunAlienFx(cli, {"powerbutton", rs, gs, bs, rs, gs, bs});
        }
        return rc;
    }
    const char *name = KeyboardCliEffect(mode);
    if (name == nullptr)
        name = "wave";
    const int rc = RunAlienFx(cli, {"keyboardeffect", name, std::to_string(r),
                                    std::to_string(g), std::to_string(b)});
    if (rc == 0) {
        NoteKeyboardEffect(mode, r, g, b);
        if (mode == 4)
            RunAlienFx(cli, {"powerrainbow"});
    }
    return rc;
}

static void DrawAllLighting(EffectController &effects, AcpiUtils &acpiUtils,
                            std::vector<ChassisPane> &panes, int &brightness,
                            ImFont &fontbold) {
    static int brightLast = -1;
    static float brightTimer = 0.0F;
    static std::string status;
    if (brightLast < 0)
        brightLast = brightness;

    PageHeader(fontbold, "All lighting",
               "One effect for the keyboard, lightbar, logo, speakers, "
               "trackpad, and power button.");

    BeginCard("all-effect");
    if (DrawEffectControls("all", gAllMode, gAllColor, 12)) {
        const uint32_t rgb = ToRGB(gAllColor);
        const int r = static_cast<int>((rgb >> 16) & 0xFF);
        const int g = static_cast<int>((rgb >> 8) & 0xFF);
        const int b = static_cast<int>(rgb & 0xFF);
        for (ChassisPane &pane : panes) {
            pane.mode = gAllMode;
            pane.color = gAllColor;
            pane.configured = !pane.zones.empty();
            pane.known = !pane.zones.empty();
        }
        effects.ApplyPrograms(
            {ChassisProgram{ZonesForEffect(acpiUtils.getKeyboardZones(), gAllMode),
                            gAllMode, rgb}});
        const int keyRc = ApplyKeyboardForMode(AlienFxCliPath(), gAllMode, r, g, b);
        if (keyRc == 0)
            gKeyColor = ImVec4(r / 255.0F, g / 255.0F, b / 255.0F, 1.0F);
        gAllKnown = true;
        gPowerKnown = true;
        SaveLighting(panes);
        status = (keyRc == 0) ? "Applied to the chassis and the keyboard"
                              : "Chassis updated. The keyboard did not change";
    }
    if (!status.empty())
        ImGui::TextDisabled("%s", status.c_str());
    LightingRecallLine(gAllKnown);
    EndCard();

    BeginCard("all-bright");
    if (DrawBrightnessRow("all", brightness, brightLast, brightTimer,
                          acpiUtils.getKeyboardZones(), effects, true,
                          status)) {
        for (ChassisPane &pane : panes) {
            pane.bright = brightness;
            pane.brightLast = brightness;
        }
    }
    EndCard();
}

static std::vector<ChassisPane> &ChassisPanes(AcpiUtils &acpiUtils,
                                              int brightness) {
    static std::vector<ChassisPane> panes;
    static bool ready = false;
    if (ready)
        return panes;
    const std::vector<uint8_t> all = acpiUtils.getKeyboardZones();
    auto add = [&](const char *name, const char *detail, uint8_t lo,
                   uint8_t hi) {
        ChassisPane pane;
        pane.name = name;
        pane.detail = detail;
        pane.zones = ZonesInRange(all, lo, hi);
        pane.bright = brightness;
        pane.brightLast = brightness;
        panes.push_back(std::move(pane));
    };
    add("Lightbar", "Rear light bar.", 0x02, 0x1A);
    add("Logo", "Alienware logo on the back of the display.", 0x1C, 0x1C);
    add("Speakers", "Left and right speaker lights.", 0x1D, 0x1E);
    add("Trackpad",
        "Sets only the trackpad. The keys stay as they are. A later keyboard "
        "Apply replaces this.",
        0x1F, 0x28);
    std::vector<uint8_t> rest;
    for (uint8_t zone : all) {
        const bool named = (zone >= 0x02 && zone <= 0x1A) || zone == 0x1C ||
                           (zone >= 0x1D && zone <= 0x1E) ||
                           (zone >= 0x1F && zone <= 0x28);
        if (!named)
            rest.push_back(zone);
    }
    ChassisPane other;
    other.name = "Other";
    other.detail = "";
    other.zones = std::move(rest);
    other.bright = brightness;
    other.brightLast = brightness;
    panes.push_back(std::move(other));
    ready = true;
    return panes;
}

void Gui::App(int h, int w, Thermals &thermals, AcpiUtils &acpiUtils,
              int &selectedMode, int &gpuBoost, int &cpuBoost,
              ImFont &smallFont, ImFont &fontbold, int &brightness,
              EffectController &effects, bool &turbo, FanCurve &curve,
              bool &requestQuit, bool &requestHide) {
    (void)h;
    static int section = 0;
    static int lightPage = 0;
    std::vector<ChassisPane> &panes = ChassisPanes(acpiUtils, brightness);
    LoadLighting(panes);

    const ImGuiWindowFlags mainFlags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)w, (float)h), ImGuiCond_Always);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Main Window", nullptr, mainFlags);
    SetupImGuiStyle();

    const float sideW = 232.0F;
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.045F, 0.05F, 0.065F, 1));
    ImGui::BeginChild("sidebar", ImVec2(sideW, 0), ImGuiChildFlags_None);
    ImGui::Dummy(ImVec2(0, 16));
    ImGui::SetCursorPosX(18.0F);
    ImGui::PushFont(&fontbold);
    ImGui::TextUnformatted("Alienware");
    ImGui::PopFont();
    ImGui::SetCursorPosX(18.0F);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.55F, 0.60F, 0.66F, 1));
    ImGui::TextWrapped("%s", Helper::getDeviceName());
    ImGui::PopStyleColor();
    ImGui::Dummy(ImVec2(0, 16));

    NavHeading("DEVICES");
    ImDrawList *navDraw = ImGui::GetWindowDrawList();
    navDraw->ChannelsSplit(2);
    navDraw->ChannelsSetCurrent(1);
    gNavCount = 0;
    if (NavItem("Performance", section == 0, false))
        section = 0;
    if (NavItem("Lighting", section == 1, false))
        section = 1;
    if (section == 1) {
        ImGui::Dummy(ImVec2(0, 4));
        const char *items[] = {"Keyboard", "Logo",     "Lightbar", "Speakers",
                               "Trackpad", "Power",    "All lighting"};
        for (int i = 0; i < 7; ++i) {
            if (NavItem(items[i], lightPage == i, true))
                lightPage = i;
        }
    }
    navDraw->ChannelsSetCurrent(0);
    DrawSlidingNavSelection();
    navDraw->ChannelsMerge();

#ifndef NDEBUG
    const std::string verText = std::string("Version ") + VERSION + "  Debug";
#else
    const std::string verText = std::string("Version ") + VERSION + "  Stable";
#endif
    ImGui::PushFont(&smallFont);
    bool cliBusy = false;
    std::string cliError;
    {
        std::lock_guard<std::mutex> lock(gCliMu);
        cliBusy = gCliBusy;
        cliError = gCliError;
    }
    const std::string cliPath = AlienFxCliPath();
    const float row = ImGui::GetFrameHeight();
    const float versionH = ImGui::GetTextLineHeight();
    const float statusH = 28.0F;
    const float errorH =
        cliError.empty() ? 0.0F : versionH * 2.0F + 6.0F;
    const float block =
        statusH + errorH + 8.0F + row + row + versionH + 20.0F;
    ImGui::SetCursorPos(ImVec2(18.0F, ImGui::GetWindowHeight() - block - 14.0F));
    ImGui::PushItemWidth(196.0F);
    if (cliBusy) {
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted("Installing alienfx_cli...");
    } else if (!cliPath.empty()) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.55F, 0.82F, 0.72F, 1));
        ImGui::Selectable("alienfx_cli installed", false,
                          ImGuiSelectableFlags_DontClosePopups);
        ImGui::PopStyleColor();
        HoverTip(cliPath.c_str());
    } else {
        if (ImGui::Button("Install alienfx_cli", ImVec2(196.0F, 28.0F)))
            StartAlienFxInstall();
        HoverTip("Downloads the latest alienfx-linux release and installs "
                 "alienfx_cli into ~/.local/bin for this user.");
        if (!cliError.empty()) {
            ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + 196.0F);
            ImGui::TextWrapped("%s", cliError.c_str());
            ImGui::PopTextWrapPos();
        }
    }
    ImGui::PopItemWidth();
    ImGui::SetCursorPosX(18.0F);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.78F, 0.82F, 0.86F, 1));
    if (ImGui::Selectable("Quit", false, ImGuiSelectableFlags_DontClosePopups))
        requestQuit = true;
    ImGui::PopStyleColor();
    HoverTip("Stops AWCC. The fan curve stops with it. Closing the window "
             "only hides AWCC.");
    ImGui::SetCursorPosX(18.0F);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.45F, 0.85F, 0.90F, 1));
    if (ImGui::Selectable("Support", false,
                          ImGuiSelectableFlags_DontClosePopups)) {
        system("xdg-open "
               "https://github.com/tr1xem/"
               "AWCC?tab=readme-ov-file#support-and-feedback &");
    }
    ImGui::PopStyleColor();
    ImGui::SetCursorPosX(18.0F);
    ImGui::TextDisabled("%s", verText.c_str());
    ImGui::PopFont();
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::SameLine(0, 0);
    ImGui::BeginChild("main-column", ImVec2(0, 0), ImGuiChildFlags_None);
    DrawSessionBar(fontbold, requestHide, requestQuit);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(28.0F, 22.0F));
    ImGui::BeginChild("content", ImVec2(0, 0),
                      ImGuiChildFlags_AlwaysUseWindowPadding);

    static int shownSection = 0;
    static int shownLight = 0;
    static float pageAlpha = 1.0F;
    static int pagePhase = 0;
    const int wantLight = section == 1 ? lightPage : 0;
    const bool pageChanged =
        shownSection != section || shownLight != wantLight;
    if (pageChanged && pagePhase != 1)
        pagePhase = 1;
    const float dt = ImGui::GetIO().DeltaTime;
    constexpr float kFade = 0.14F;
    if (pagePhase == 1) {
        pageAlpha -= dt / kFade;
        if (pageAlpha <= 0.0F) {
            pageAlpha = 0.0F;
            shownSection = section;
            shownLight = wantLight;
            pagePhase = 2;
        }
    } else if (pagePhase == 2) {
        pageAlpha += dt / kFade;
        if (pageAlpha >= 1.0F) {
            pageAlpha = 1.0F;
            pagePhase = 0;
        }
    }

    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, pageAlpha);
    if (shownSection == 0) {
        DrawPerformance(thermals, acpiUtils, selectedMode, gpuBoost, cpuBoost,
                        fontbold, turbo, curve);
    } else if (shownLight == 0) {
        DrawKeyboardPage(fontbold, smallFont, effects, panes);
    } else if (shownLight == 1) {
        DrawChassisPane(panes[1], effects, panes, fontbold);
    } else if (shownLight == 2) {
        DrawChassisPane(panes[0], effects, panes, fontbold);
    } else if (shownLight == 3) {
        DrawChassisPane(panes[2], effects, panes, fontbold);
    } else if (shownLight == 4) {
        DrawChassisPane(panes[3], effects, panes, fontbold);
    } else if (shownLight == 5) {
        DrawPowerPage(fontbold, effects, panes);
    } else {
        DrawAllLighting(effects, acpiUtils, panes, brightness, fontbold);
    }
    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::End();
    ImGui::PopStyleVar();
}

