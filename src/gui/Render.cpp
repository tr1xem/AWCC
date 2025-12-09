#include "EffectController.h"
#include "Gui.h"
#include "Renderui.h"
#include "helper.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "resource.h"
#include <format>
#include <loguru.hpp>
#include <string>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

static void glfw_error_callback(int error, const char *description) {
    LOG_S(ERROR) << "GLFW Error " << error << ": " << description;
}

static void keepWindowSizeFixed(GLFWwindow *window, int bufferWidth,
                                int bufferHeight) {
    int w = 600, h = 900;
    glfwGetWindowSize(window, &w, &h);
    if (w != bufferWidth) {
        w = float(w);
        h = float(h);
        glfwSetWindowSize(window, w * w / bufferWidth, h * h / bufferHeight);
    }
}
bool RenderUi::Init(Thermals &thermals, AcpiUtils &acpiUtils,
                    EffectController &effects) {
    glfwSetErrorCallback(glfw_error_callback);
    glfwInitHint(GLFW_WAYLAND_LIBDECOR, GLFW_WAYLAND_DISABLE_LIBDECOR);
    if (glfwInit() == 0)
        return false;

    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    std::string title =
        std::format("Alienware Command Centre - {}", Helper::getDeviceName());
    GLFWwindow *window =
        glfwCreateWindow(600, 900, title.c_str(), nullptr, nullptr);
    if (window == nullptr)
        return true;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImFontConfig cfg;
    cfg.FontDataOwnedByAtlas = false;
    ImFont *font = io.Fonts->AddFontFromMemoryTTF(
        roboto_regular, static_cast<int>(roboto_regular_len), 17.0F, &cfg);
    ImFont *fontbold = io.Fonts->AddFontFromMemoryTTF(
        roboto_bold, static_cast<int>(roboto_bold_len), 18.0F, &cfg);
    ImFont *smallFont = io.Fonts->AddFontFromMemoryTTF(
        roboto_light, static_cast<int>(roboto_light_len), 17.0F, &cfg);
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

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

    static bool turbo{acpiUtils.getTurboBoost()};

    static int h, w;
    // glfwGetFramebufferSize(window, &w, &h);
    // keepWindowSizeFixed(window, w, h);
    // Main loop
    while (glfwWindowShouldClose(window) == 0) {
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        static int h, w;
        glfwGetWindowSize(window, &w, &h);
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImVec4 clear_color = ImVec4(0.00F, 0.00F, 0.00F, 0.00F);

        // LOG_S(INFO) << "Rendering" << h << w;
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
