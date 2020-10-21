#!/bin/zsh
g++ -lglfw -lglad main.cpp include/imgui/imgui.cpp include/imgui/imgui_impl_glfw.cpp include/imgui/imgui_draw.cpp include/imgui/imgui_widgets.cpp include/imgui/imgui_impl_opengl3.cpp s.cpp -o display.exe
./display.exe
