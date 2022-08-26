#include "ScreenSize.h"

#include "Common.h"
int Window_Width = 800;
int Window_Height = 600;
float AspectR = (float)Window_Width / Window_Height;
bool isAppClosed = false;
void UpdateScreenResize(GLFWwindow* win)
{
	int w, h;
	glfwGetWindowSize(win, &w, &h);
	Window_Width = w;
	Window_Height = h;
	if(h)
	AspectR = (float)w / h;
	glViewport(0, 0, w, h);
}
