// Copyright (c) 2023 tsl0922. All rights reserved.
// SPDX-License-Identifier: GPL-2.0-only

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <thread>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>
#include <mpv/client.h>
#include "debug.h"

static mpv_handle* mpv = NULL;
static Debug* debug = NULL;

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

static int gui_thread(void* data) {
    mpv_handle* mpv = (mpv_handle*)data;
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return 1;

#if defined(IMGUI_IMPL_OPENGL_ES2)
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(400, 600, "Debug", nullptr, nullptr);
    if (window == nullptr) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGuiStyle style;
    ImGui::StyleColorsDark(&style);
    float xscale, yscale;
    glfwGetWindowContentScale(window, &xscale, &yscale);
    float scale = std::max(xscale, yscale);
    style.ScaleAllSizes(scale);
    ImGui::GetStyle() = style;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImFontConfig font_cfg;
    font_cfg.SizePixels = 13 * scale;
    io.Fonts->AddFontDefault(&font_cfg);

    debug = new Debug(mpv);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!glfwWindowShouldClose(window)) {
        glfwWaitEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiViewport* vp = ImGui::GetMainViewport();
        vp->Flags &= ~ImGuiViewportFlags_CanHostOtherWindows;

        debug->draw();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
                     clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    delete debug;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

static void show_debug() {
    if (!debug) {
        std::thread(gui_thread, mpv).detach();
    } else {
        debug->show();
        glfwPostEmptyEvent();
    }
}

static void handle_client_message(mpv_event* event) {
    mpv_event_client_message* msg = (mpv_event_client_message*)event->data;
    if (msg->num_args < 1) return;

    const char* cmd = msg->args[0];
    if (strcmp(cmd, "show") == 0) show_debug();
}

static void handle_log_message(mpv_event* event) {
    mpv_event_log_message* msg = (mpv_event_log_message*)event->data;
    if (debug) {
        debug->AddLog(msg->prefix, msg->level, msg->text);
        glfwPostEmptyEvent();
    }
}

extern "C" MPV_EXPORT int mpv_open_cplugin(mpv_handle* handle) {
    mpv = handle;

    while (mpv) {
        mpv_event* event = mpv_wait_event(mpv, -1);
        if (event->event_id == MPV_EVENT_SHUTDOWN) break;

        switch (event->event_id) {
            case MPV_EVENT_CLIENT_MESSAGE:
                handle_client_message(event);
                break;
            case MPV_EVENT_LOG_MESSAGE:
                handle_log_message(event);
                break;
            default:
                break;
        }
    }

    return 0;
}