#include "Assignment.h"
Assignment instance;
static Camera* cam;
static shared_ptr<CubeModel> cube;
static shared_ptr<CubeModel> cube1;

static shared_ptr<SphereModel> sph;
static shared_ptr<Model> gun;
void Assignment::Start(GLFWwindow* win)
{
	//camera
	float far = 100;
	float near = 0.1;
	float fov = pi / 4;
	cam = new Camera(fov, ((float)Window_Width / Window_Height), near, far, glm::vec3(0, 0, 10),1, win);
	
	auto lsh = make_shared<LitShader>();
	
	//creating cube models
	cube = make_shared<CubeModel>(make_shared<Material>(),lsh);
	cube1 = make_shared<CubeModel>(make_shared<Material>(),lsh);
	cube1->modelMat = glm::translate(mat4(1), { 3,0,0 });

	// loading textures
	auto tex = make_shared<Texture>("Scenes/BasicShapesScene/Assets/earth.jpg");
	auto tex1 = make_shared<Texture>("Scenes/BasicShapesScene/Assets/3242sd.png");
	
	//displaying a sphere
	sph = make_shared<SphereModel>(ivec2{ 10,10 }, make_shared<Material>(), lsh);
	sph->modelMat = glm::translate(mat4(1), { 0,2,0 });
	sph->m->textures.push_back(tex);
	sph->m->sh = lsh;


	//loadinng a model
	auto modelSh = make_shared<Shader>(SHADER_PATH "DefaultMesh.shader");
	gun = make_shared<Model>(ASSETS "gun/Handgun_obj.obj",modelSh);
}

void Assignment::Render()
{
	cam->UpdateInput();
	//tex->Bind(0);
	//cube->Render(*cam);
	//tex1->Bind(0);
	//cube1->Render(*cam);
	//sph->Render(*cam);
	gun->Render(*cam);
}

void Assignment::UiRender()
{
}

void Assignment::Update()
{
}

void Assignment::Terminate()
{
}

void Assignment::GuiRender()
{
}
