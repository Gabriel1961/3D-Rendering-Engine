#pragma once
// Wrapper
#include "../OpenGLWrapper/VertexArray.h"
#include "../OpenGLWrapper/VertexBuffer.h"
#include "../OpenGLWrapper/VertexBufferLayout.h"
#include "../OpenGLWrapper/Shader.h"
#include "../OpenGLWrapper/Renderer.h"
#include "../OpenGLWrapper/Transform.h"
#include "../OpenGLWrapper/Common.h"

#include "../ScreenSize.h"
#include "../../OpenGLWrapper/vendor/imgui/ImguiLib.h"
#include <memory>
// Input
#include <Input/Input.h>
// standard libs
#include <chrono>
#include <future>
#include <ctime>

/// Architecture ///
/// This header will contain the shared definitions between the core modules
// The header 3DRenderingEngineModules.h will include all the core modules of the engine and other libraries related to the project
// It will be included as needed in scenes
/// Scenes
// Scenes are found in the Scenes folder 
// To add a scene create a new folder after the name of the scene and follow the prebuild scenes as a template
/// Scenes will be automatically loaded if you create global instance of the object 
// to select a scene you will need to select it from the Scene Selection Menu
#define BASE_PATH "RenderingEngine/"
#define MODULE_PATH BASE_PATH "Modules/"
#define SHADER_PATH BASE_PATH "Shaders/"
#define MODEL_PATH BASE_PATH "Models/"
class RenderingEngine
{
public:
	static void Start(GLFWwindow* window);
	static void Update(); // gets called before the render
	static void Render();
	static void UiRender(); // gets called after the render
	static void Terminate();
	static void CheckRecompileAllShaders();

	static float TimeNow() {
		using std::chrono::duration_cast;
		using std::chrono::milliseconds;
		using std::chrono::seconds;
		using std::chrono::system_clock;
		long long d =  duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		d = d % (long long)(1e6);
		return d/1000.0f;
	}
};
