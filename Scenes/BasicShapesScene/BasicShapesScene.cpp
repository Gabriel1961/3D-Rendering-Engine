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
static shared_ptr<Shader> shadowSh;
static shared_ptr<DirectionalLight> ldir;
static shared_ptr<FrameBuffer> fb;
static Quad* q;

void BasicShapesScene::Start(GLFWwindow* win)
{
	mainCamera = make_shared<Camera>(true,win,this, vec3{ 0,0,10 });
	
	lshader = make_shared<LitShader>();
	lshaderFlat = make_shared<LitShader>("DefaultLitFlat.shader");
	
	icoShader = make_shared<Shader>(SHADER_PATH "Viewport.shader");
	auto earthTex = make_shared<Texture>(ASSETS_PATH "earth.jpg");
	
	cube = make_shared<CubeModel>();
	cube->modelMat = scale(translate(mat4(1),{ 0, -2, 0 }), { 10,.1,10 });
	cube->m->mat = make_shared<Material>(vec4{ 1,.6,0,1 });
	cube->m->sh = lshaderFlat;

	cube2 = make_shared<CubeModel>(make_shared<Material>(),lshaderFlat);
	
	// Add lights
	auto l1 = make_shared<PointLight>( vec3{ -2,2.5,0 });
	auto l2 = make_shared<PointLight>( vec3{ 5,1,0});


	lshaderFlat->lights.push_back(l1);
	lshaderFlat->lights.push_back(l2);
	lshader->lights.push_back(l1);
	lshader->lights.push_back(l2);

	// Add gizmos
	gizmos.push_back(l1->gizmo);
	gizmos.push_back(l2->gizmo);

	// sphere
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

	// add models to vector
	//models.push_back(house);
	//models.push_back(sph);
	models.push_back(cube);
	models.push_back(cube2);

	//shadow
	shadowSh = make_shared<Shader>(SHADER_PATH "Shadow.shader");
	ldir = make_shared<DirectionalLight>(vec3{ 10,20,10 }, vec3{ -2.0f, 4.0f, -1.0f });
	lshaderFlat->preRender = [](Shader* sh) {
		sh->SetUniform1i("shadowMap", 5);
		sh->SetUniformMat4f("lightMat", ldir->GetWP());
	};
	shadowSh->preRender = [](Shader* sh) {
		sh->SetUniform3f("lightDir", ldir->dir);
	};
	fb = make_shared<FrameBuffer>();
	fb->AttachDepthTexture(make_shared<DepthTexture>(ivec2{ 2048,2048 }));
	q = new Quad();
}

void BasicShapesScene::BasicRender(const Camera& cam,shared_ptr<Shader> shOverr)
{
	lshader->ApplyLights(cam);
	lshaderFlat->ApplyLights(cam);
	DrawModels(cam,shOverr);
}
void BasicShapesScene::Render()
{
	
	fb->Bind();
	glClear(GL_DEPTH_BUFFER_BIT);
	fb->depthTexture->Bind(5);
	glCullFace(GL_FRONT);
	BasicRender(Camera(ldir->GetWP()), shadowSh);


	glCullFace(GL_BACK);
	fb->Unbind();
	BasicRender(*mainCamera);

}

void BasicShapesScene::UiRender()
{

	
	Gizmo::DrawAllGizmos(*mainCamera,this);
	mainCamera->UpdateClickSelectInput(models);
	using namespace Physics;

	ImGui::SliderFloat3("Pos", (float*)&lshader->lights[0]->pos, -3.f, 3.f);
	ImGui::SliderFloat3("Pos2", (float*)&lshader->lights[1]->pos, -3.f, 3.f);
	ImGui::Text("Tex Params");
	ImGui::SliderFloat("Angle", &house->meshes[0].mat->texAngle, 0, 2 * pi);

	glCullFace(GL_BACK);
	ImGui::SliderFloat2("Scale", (float*)&house->meshes[0].mat->texScale, 0.0f, 10.0f);
	ImGui::SliderFloat2("Offset", (float*)&house->meshes[0].mat->texOffset, 0, 10);
	ImGui::SliderFloat2("Shininess", (float*)&house->meshes[0].mat->shininess, 0, 256);
	cube->m->mat->GetImgui("cube");
}

void BasicShapesScene::Update()
{
	mainCamera->UpdateInput(); 
	mainCamera->projMat = perspective(pi / 2.f, AspectR, .1f, 1000.f);
	
}

void BasicShapesScene::Terminate()
{
}
