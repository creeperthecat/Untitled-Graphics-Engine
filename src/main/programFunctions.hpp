#pragma once
#define GLFW_INCLUDE_NONE

#include "GLFW/glfw3.h"

void setKeybinds();

// Functions for keybinds

void onEscape();
void onW();
void onA();
void onS();
void onD();

// Mouse callback functions to pass to GLFW

void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

// Runs render loop
void runProgram();
