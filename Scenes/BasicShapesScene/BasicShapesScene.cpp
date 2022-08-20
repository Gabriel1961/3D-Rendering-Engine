#include "BasicShapesScene.h"
#include <DebugLine.h>
BasicShapesScene basicShapeScene;
static int icoLod = 0;
static shared_ptr<Shader> icoShader = 0;

static shared_ptr<SphereModel> sph;
static shared_ptr<Model> house;
static shared_ptr<CubeModel> cube;
static shared_ptr<CubeModel> cube2;
static shared_ptr<LitShader> lshader;
static shared_ptr<LitShader> lshaderFlat;
static TransformGizmo3D* giz;
static DebugLine* dbl;
void BasicShapesScene::Start(GLFWwindow* win)
{

	mainCamera = new Camera(pi/2,AspectR,0.01f,100.f,{0,0,10},1,win);
	icoShader = make_shared<Shader>(SHADER_PATH "Viewport.shader");
	auto earthTex = make_shared<Texture>(ASSETS_PATH "earth.jpg");
	
	cube = make_shared<CubeModel>();
	cube->modelMat = scale(translate(mat4(1),{ 0, -2, 0 }), { 10,.1,10 });
	cube->m->mat->diffuse = Color::FromRgb(52, 140, 49).rgba;
	cube->m->mat->ambient = .1f*Color::FromRgb(52, 140, 49).rgba;


	lshader = make_shared<LitShader>();
	lshaderFlat = make_shared<LitShader>("DefaultLitFlat.shader");
	auto l1 = make_shared<PointLight>( vec3{ -2,2.5,0 });
	auto l2 = make_shared<PointLight>( vec3{ 5,1,0});

	cube2 = make_shared<CubeModel>(make_shared<Material>(),lshaderFlat);

	// Add lights
	lshaderFlat->lights.push_back(l1);
	lshaderFlat->lights.push_back(l2);
	lshader->lights.push_back(l1);
	lshader->lights.push_back(l2);

	
	cube->m->sh = lshaderFlat;
	cube->m->mat = make_shared<Material>(vec4{ 1,.6,0,1 });
	
	sph = make_shared<SphereModel>(vec2{ 100,100 },make_shared<Material>(), lshader);
	sph->m->textures.push_back(earthTex);
	sph->m->sh = lshader;

	//loadinng a model
	house = make_shared<Model>(ASSETS_PATH "House/house.obj", lshader);
	house->meshes[0].mat->texAngle = pi / 2;
	house->meshes[0].textures.clear();
	house->meshes[0].textures.push_back(make_shared<Texture>(ASSETS_PATH "House/containerDiffuse.png"));
	house->meshes[0].textures.push_back(make_shared<Texture>(ASSETS_PATH "House/containerSpecular.png"));
	house->meshes[0].mat->diffuseTex = house->meshes[0].textures[0];
	house->meshes[0].mat->specularTex = house->meshes[0].textures[1];
	house->modelMat *= scale(mat4(1), { .1,.1,.1 });
	giz = new TransformGizmo3D();

	// add models to vector
	models.push_back(house);
	models.push_back(sph);
	models.push_back(cube);
	models.push_back(cube2);

	using namespace Physics;
	dbl = new DebugLine({ 0,0,0 }, { 1,1,1 },{1,0,1,1});
}

void BasicShapesScene::BasicRender()
{
	mat4 sc = scale(mat4(1), { 1,1,1 });
	lshader->ApplyLights(*mainCamera);
	lshaderFlat->ApplyLights(*mainCamera);
	DrawModels();
	dbl->RenderLines(mainCamera->GetViewMat(),mainCamera->projMat);
}

void BasicShapesScene::Render()
{
	BasicRender();
	mainCamera->DrawSelectedOutline();
}

void BasicShapesScene::UiRender()
{

	
	//Gizmo::DrawAllGizmos(*mainCamera);
	
	using namespace Physics;
	if (glfwGetMouseButton(window, 0) == GLFW_PRESS)
	{
		Ray r = mainCamera->GetMouseRay();
		dbl->start = r.pos;
		dbl->end = r.GetPosOnRay(10);
		BoundingBox bb({ -1,-1,-1 }, { 1,1,1 });
		Rayhit rh;
		if (bb.Intersect(r, rh))
			cout << rh.pos.x << " " << rh.pos.y << " " << rh.pos.z << "\n";
		else cout << "No\n";
	}
	
	ImGui::SliderFloat3("Pos", (float*)&lshader->lights[0]->pos, -3.f, 3.f);
	ImGui::SliderFloat3("Pos2", (float*)&lshader->lights[1]->pos, -3.f, 3.f);

	ImGui::Text("Tex Params");
	ImGui::SliderFloat("Angle", &house->meshes[0].mat->texAngle, 0, 2 * pi);

	ImGui::SliderFloat2("Scale", (float*)&house->meshes[0].mat->texScale, 0.0f, 10.0f);
	ImGui::SliderFloat2("Offset", (float*)&house->meshes[0].mat->texOffset, 0, 10);
	ImGui::SliderFloat2("Shininess", (float*)&house->meshes[0].mat->shininess, 0, 256);
	cube->m->mat->GetImgui("cube");
}

void BasicShapesScene::Update()
{
	
	mainCamera->UpdateInput(); 
}

void BasicShapesScene::Terminate()
{
}
