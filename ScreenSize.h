#pragma once
extern int Window_Width;
extern int Window_Height;
extern float AspectR;
struct GLFWwindow;
extern bool isAppClosed;
void UpdateScreenResize(GLFWwindow* win);