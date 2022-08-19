#include "Assignment.h"
Assignment instance;
static Camera* cam;
static CubeModel* cube;
static CubeModel* cube1;

static Sphere* sph;
static Model* gun;
void Assignment::Start(GLFWwindow* win)
{
	//camera
	float far = 100;
	float near = 0.1;
	float fov = pi / 4;
	cam = new Camera(glm::perspective(fov, ((float)Window_Width / Window_Height), near, far), glm::vec3(0, 0, 10),1, win);
	
	//creating cube models
	cube = new CubeModel();
	cube1 = new CubeModel();
	cube1->modelMat = glm::translate(mat4(1), { 3,0,0 });

	// loading textures
	auto tex = make_shared<Texture>("Scenes/BasicShapesScene/Assets/earth.jpg");
	auto tex1 = make_shared<Texture>("Scenes/BasicShapesScene/Assets/3242sd.png");
	
	//displaying a sphere
	auto lsh = make_shared<LitShader>();
	sph = new Sphere({ 10,10 });
	sph->modelMat = glm::translate(mat4(1), { 0,2,0 });
	sph->textures.push_back(tex);
	sph->sh = lsh;


	//loadinng a model
	auto modelSh = make_shared<Shader>(SHADER_PATH "DefaultMesh.shader");
	gun = new Model(ASSETS "gun/Handgun_obj.obj",modelSh);
}

void Assignment::Render()
{
	cam->UpdateInput();
	//tex->Bind(0);
	//cube->Render(*cam);
	//tex1->Bind(0);
	//cube1->Render(*cam);
	//sph->Render(*cam);
	gun->Draw(*cam);
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
