#include "BasicShapesScene.h"
BasicShapesScene basicShapeScene;
static int icoLod = 0;
static Shader* icoShader = 0;
static LitShader* lshader = 0;
static LitShader* lshaderFlat = 0;
static Texture* earthTex = 0;
static Sphere* sph = 0;

void BasicShapesScene::Start(GLFWwindow* win)
{
	mainCamera = new Camera(glm::perspective(pi/2,AspectR,0.01f,100.f),{0,0,0},win);
	icoShader = new Shader(SHADER_PATH "Viewport.shader");
	earthTex = new Texture(ASSETS_PATH "earth.jpg");
	ico = new IcoSphereModel(3,icoShader);
	ico->textures.push_back(*earthTex);
	
	cube = new CubeModel();
	cube->viewMat = scale(translate(mat4(1),{ 0, -2, 0 }), { 10,.1,10 });
	cube->color = Color::FromRgb(52, 140, 49).rgba;
	ico->color = { 1,1,1,1 };

	lshader = new LitShader();
	lshaderFlat = new LitShader("DefaultLitFlat.shader");
	auto l2 = make_shared<Light>(LightType::PointLight, vec3{ 5,1,0 });
	auto l1 = make_shared<Light>(LightType::PointLight, vec3{ 0,2.5,0 });

	l1->color = Color::White().rgb;
	l2->color = Color::White().rgb;
	// Add lights
	lshaderFlat->lights.push_back(l1);
	lshaderFlat->lights.push_back(l2);
	lshader->lights.push_back(l1);
	lshader->lights.push_back(l2);

	lshaderFlat->ApplyLights();
	lshader->ApplyLights();
	ico->sh = lshader;
	cube->sh = lshaderFlat;

	sph = new Sphere({ 100,100 });
	sph->textures.push_back(*earthTex);
	sph->sh = lshader;
}

void BasicShapesScene::BasicRender()
{
	lshader->ApplyLights();
	//ico->Render(*mainCamera);
	cube->Render(*mainCamera);
	sph->Render(*mainCamera);
}

void BasicShapesScene::Render()
{
	BasicRender();
}

void BasicShapesScene::UiRender()
{

	lshader->RenderLightGizmos(*mainCamera);
}

void BasicShapesScene::Update()
{
	ImGui::SliderFloat3("Pos", (float*) & lshader->lights[0]->pos, -3.f, 3.f);
	mainCamera->UpdateInput(); 
}

void BasicShapesScene::Terminate()
{
	delete ico;
	delete cube;
}
