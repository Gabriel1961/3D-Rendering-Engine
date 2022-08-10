#include "BasicShapesScene.h"
BasicShapesScene basicShapeScene;
static int icoLod = 0;
static Shader* icoShader = 0;
static LitShader* lshader = 0;
static LitShader* lshaderFlat = 0;
static Texture* earthTex = 0;
static Sphere* sph = 0;
static Model* house = 0;
void BasicShapesScene::Start(GLFWwindow* win)
{
	mainCamera = new Camera(glm::perspective(pi/2,AspectR,0.01f,100.f),{0,0,0},1,win);
	icoShader = new Shader(SHADER_PATH "Viewport.shader");
	earthTex = new Texture(ASSETS_PATH "earth.jpg");
	ico = new IcoSphereModel(3,icoShader);
	ico->textures.push_back(*earthTex);
	
	cube = new CubeModel();
	cube->modelMat = scale(translate(mat4(1),{ 0, -2, 0 }), { 10,.1,10 });
	cube->mat->diffuse = Color::FromRgb(52, 140, 49).rgba;
	cube->mat->ambient = .1f*Color::FromRgb(52, 140, 49).rgba;
	ico->color = { 1,1,1,1 };

	lshader = new LitShader();
	lshaderFlat = new LitShader("DefaultLitFlat.shader");
	auto l2 = make_shared<PointLight>( vec3{ 5,1,0 });
	auto l1 = make_shared<PointLight>( vec3{ -2,2.5,0 });


	// Add lights
	lshaderFlat->lights.push_back(l1);
	lshaderFlat->lights.push_back(l2);
	lshader->lights.push_back(l1);
	lshader->lights.push_back(l2);


	ico->sh = lshader;
	cube->sh = lshaderFlat;

	sph = new Sphere({ 100,100 });
	sph->textures.push_back(*earthTex);
	sph->sh = lshader;

	//loadinng a model
	house = new Model(ASSETS_PATH "House/house.obj", lshader);
	house->meshes[0].mat->texAngle = pi / 2;
	house->meshes[0].textures.clear();
	house->meshes[0].textures.push_back(Texture(ASSETS_PATH "House/containerDiffuse.png"));
	house->meshes[0].textures.push_back(Texture(ASSETS_PATH "House/lighting_maps_specular_color.png"));
	house->meshes[0].mat->diffuseTex = &house->meshes[0].textures[0];
	house->meshes[0].mat->specularTex= &house->meshes[0].textures[1];
}

void BasicShapesScene::BasicRender()
{
	lshader->ApplyLights(*mainCamera);
	lshaderFlat->ApplyLights(*mainCamera);
	//ico->Render(*mainCamera);
	cube->Render(*mainCamera);
	//sph->Render(*mainCamera);
	house->Draw(*mainCamera);
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
	ImGui::SliderFloat3("Pos", (float*) & lshader->lights[1]->pos, -3.f, 3.f);

	ImGui::Text("Tex Params");
	ImGui::SliderFloat("Angle",&house->meshes[0].mat->texAngle,0,2*pi);

	ImGui::SliderFloat2("Scale", (float*) & house->meshes[0].mat->texScale, 0.0f, 10.0f);
	ImGui::SliderFloat2("Offset", (float*) & house->meshes[0].mat->texOffset, 0, 10);
	ImGui::SliderFloat2("Shininess", (float*) & house->meshes[0].mat->shininess, 0, 256);
	mainCamera->UpdateInput(); 
}

void BasicShapesScene::Terminate()
{
	delete ico;
	delete cube;
}
