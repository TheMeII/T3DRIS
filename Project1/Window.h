#pragma once

#include <string>

#include <GL\freeglut.h>

void makeWindow(int argc, char** argv, std::string windowname);
static void initGlutCallbacks();
static void RenderSceneCB();
void resizeWindow(int width, int height);