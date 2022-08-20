#pragma once

// Wrapper
#include "../OpenGLWrapper/VertexArray.h"
#include "../OpenGLWrapper/VertexBuffer.h"
#include "../OpenGLWrapper/VertexBufferLayout.h"
#include "../OpenGLWrapper/Shader.h"
#include "../OpenGLWrapper/Renderer.h"
#include "../OpenGLWrapper/Transform.h"
#include "../OpenGLWrapper/Common.h"
#include "../OpenGLWrapper/Quad.h"

#include "../ScreenSize.h"
#include "../../OpenGLWrapper/vendor/imgui/ImguiLib.h"
#include "../Scenes/Scene.h"
// Input
#include <Input/Input.h>

// Engine modules
#include "Modules/Camera.h"
#include "Models/PreBuilt/CubeModel.h"
#include "Models/PreBuilt/IcoSphereModel.h"
#include "Models/PreBuilt/Sphere.h"
#include "Modules/Mesh.h"
#include "Modules/CubeMap/CubeMap.h"
#include "Modules/Gizmos/Gizmo2D.h"
#include "Modules/Gizmos/Gizmo3D.h"
#include "Modules/LitShader.h"
#include "Modules/Material/Material.h"
#include "Modules/Physics/BoundingBox.h"