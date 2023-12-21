// Copyright (c) 2022-2023 tsl0922. All rights reserved.
// SPDX-License-Identifier: GPL-2.0-only

#pragma once
#include <vector>
#include <map>
#include <string>
#include <mpv/client.h>
#include <imgui.h>

// imgui extensions
namespace ImGui {
inline ImVec2 EmVec2(float x, float y) { return ImVec2(GetFontSize() * x, GetFontSize() * y); }
inline float EmSize(float n) { return GetFontSize() * n; }
}  // namespace ImGui

class Debug {
   public:
    explicit Debug(mpv_handle *mpv);
    ~Debug();

    void draw();
    void show();
    void AddLog(const char *prefix, const char *level, const char *text);

   private:
    struct Console {
        explicit Console(mpv_handle *mpv);
        ~Console();

        void init(const char *level, int limit);
        void draw();

        void ClearLog();
        void AddLog(const char *level, const char *fmt, ...);
        void ExecCommand(const char *command_line);
        int TextEditCallback(ImGuiInputTextCallbackData *data);
        void initCommands(std::vector<std::pair<std::string, std::string>> &commands);

        ImVec4 LogColor(const char *level);

        const std::vector<std::string> builtinCommands = {"HELP", "CLEAR", "HISTORY"};

        struct LogItem {
            char *Str;
            const char *Lev;
        };

        mpv_handle *mpv;
        char InputBuf[256];
        ImVector<LogItem> Items;
        ImVector<char *> Commands;
        ImVector<char *> History;
        int HistoryPos = -1;  // -1: new line, 0..History.Size-1 browsing history.
        ImGuiTextFilter Filter;
        bool AutoScroll = true;
        bool ScrollToBottom = false;
        bool CommandInited = false;
        std::string LogLevel = "v";
        int LogLimit = 500;
    };

    struct Binding {
        std::string key;
        std::string cmd;
        std::string comment;
    };

    void drawHeader();
    void drawConsole();
    void drawBindings();
    void drawCommands();
    void drawProperties(const char *title, std::vector<std::string> &props);
    void drawPropNode(const char *name, mpv_node &node, int depth = 0);

    void initData();

    mpv_handle *mpv;
    bool m_open = true;
    Console *console = nullptr;
    std::string version;
    std::string m_node = "Console";
    bool m_demo = false;

    std::vector<std::string> options;
    std::vector<std::string> properties;
    std::vector<std::pair<std::string, std::string>> commands;
    std::vector<Binding> bindings;
};