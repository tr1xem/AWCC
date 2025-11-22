#include "EffectController.h"
#include "Gui.h"
#include "Renderui.h"
#include "helper.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <format>
#include <loguru.hpp>
#include <string>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

static void glfw_error_callback(int error, const char *description) {
    LOG_S(ERROR) << "GLFW Error " << error << ": " << description;
}

bool RenderUi::Init(Thermals &thermals, AcpiUtils &acpiUtils,
                    EffectController &effects) {
    glfwSetErrorCallback(glfw_error_callback);
    if (glfwInit() == 0)
        return false;

    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    //
    std::string title =
        std::format("Alienware Command Centre - {}", Helper::getDeviceName());
    float main_scale =
        ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
    GLFWwindow *window =
        glfwCreateWindow((int)(600 * main_scale), (int)(900 * main_scale),
                         title.c_str(), nullptr, nullptr);
    if (window == nullptr)
        return true;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    // ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);

    style.FontScaleDpi = main_scale;

    // if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
    //     style.WindowRounding = 0.0F;
    //     style.Colors[ImGuiCol_WindowBg].w = 1.0F;
    // }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImFont *font =
        io.Fonts->AddFontFromFileTTF("/usr/share/fonts/TTF/Roboto-Regular.ttf");
    if (font == nullptr) {
        // Fallback 2: use default font
        font = io.Fonts->AddFontDefault();
    }

    IM_ASSERT(font != nullptr);

    // Our state

    ThermalModes buttonModes[] = {
        acpiUtils.hasThermalMode(ThermalModeSet::BatterySaver)
            ? ThermalModes::BatterySaver
            : ThermalModes::Cool,
        ThermalModes::Quiet,
        ThermalModes::Balanced,
        ThermalModes::Performance,
        ThermalModes::Gmode,
    };

    static int selected{0};
    {
        ThermalModes current = thermals.getCurrentMode();
        for (int i = 0; i < 5; ++i) {
            if (buttonModes[i] == current) {
                selected = i;
                break;
            }
        }
    }

    static int gpuBoost{thermals.getGpuBoost()};
    static int cpuBoost{thermals.getCpuBoost()};
    static int brightness{effects.getBrightness()};
    ImFont *fontbold =
        io.Fonts->AddFontFromFileTTF("/usr/share/fonts/TTF/Roboto-Bold.ttf");
    if (fontbold == nullptr) {
        // Fallback 2: use default font
        fontbold = io.Fonts->AddFontDefault();
    }
    ImFont *smallFont = io.Fonts->AddFontFromFileTTF(
        "/usr/share/fonts/TTF/Roboto-Light.ttf", 17.0F);
    if (smallFont == nullptr) {
        // Fallback 2: use default font
        smallFont = io.Fonts->AddFontDefault();
    }
    static bool turbo{acpiUtils.getTurboBoost()};

    // Main loop
    while (glfwWindowShouldClose(window) == 0) {
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        static int h, w;
        glfwGetFramebufferSize(window, &w, &h);
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImVec4 clear_color = ImVec4(0.45F, 0.55F, 0.60F, 1.00F);

        Gui::App(h, w, thermals, acpiUtils, selected, gpuBoost, cpuBoost,
                 *smallFont, *fontbold, brightness, effects, turbo);

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w,
                     clear_color.y * clear_color.w,
                     clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we
        // save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call
        //  glfwMakeContextCurrent(window) directly)
        // if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
        //     GLFWwindow *backup_current_context = glfwGetCurrentContext();
        //     ImGui::UpdatePlatformWindows();
        //     ImGui::RenderPlatformWindowsDefault();
        //     glfwMakeContextCurrent(backup_current_context);
        // }

        glfwSwapBuffers(window);
    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return false;
}
