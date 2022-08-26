#include <Common.h>
#include "ScreenSize.h"
#include <Renderer.h>
#include <fstream>
#include <string>
#include <sstream>
#include <Math.h>
#include <conio.h>
#include <iomanip>
#define _USE_MATH_DEFINES

#include "3DRenderingEngine.h"
#include "WindowsSpecific.h"

int main(void)
{
	GLFWwindow* window;
	/* Initialize the library */
	if (!glfwInit())
		return -1;
	/* Create a windowed mode window and its OpenGL context */
	glfwWindowHint(GLFW_SAMPLES, 4);
	std::cout << glfwGetVersionString() << endl;
	
	window = glfwCreateWindow(Window_Width, Window_Height, "OpenGLPractice", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	// Enable Vsync
	glfwSwapInterval(0);
	EnableVSync();

	/* Make the window's context current */

	
	glfwMakeContextCurrent(window);
	{
		if (glewInit() != GLEW_OK) {
			std::cout << "Failed to initilize glew\n" << endl;
			ASSERT(false);
		}
		Imgui_Start(window);
		/// ///////////////////////////////////////////
		double lastTime = glfwGetTime();
		int nbFrames = 0;
		/// ///////////////////////////////////////////
#define PROJECT RenderingEngine
		PROJECT::Start(window);
		while (!glfwWindowShouldClose(window))
		{
			/* Poll for and process events */
			glfwPollEvents();

#pragma region FrameCounter
			double currentTime = glfwGetTime();
			nbFrames++;
			if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
				// printf and reset timer
				glfwSetWindowTitle(window, std::to_string(nbFrames).c_str());
				nbFrames = 0;
				lastTime += 1.0;
			}
#pragma endregion
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
			glCullFace(GL_BACK);
			glEnable(GL_DEPTH_TEST);

			UpdateScreenResize(window);

			Renderer::Clear();

			Imgui_NewFrame();
			
			PROJECT::Update();

			/* Render here */
			PROJECT::Render();

			PROJECT::UiRender();

			Imgui_Render();
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

		}
		isAppClosed = true;
		PROJECT::Terminate();
	}
	Imgui_Close();

	glfwTerminate();
	return 0;
}
