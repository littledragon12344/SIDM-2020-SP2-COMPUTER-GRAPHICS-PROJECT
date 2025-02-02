#include "SceneInterior.h"
#include "GL\glew.h"
#include "Application.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "CCar.h"

#define ROT_LIMIT 45.f;
#define SCALE_LIMIT 5.f;
#define LSPEED 10.f
#define TEXT_SIZE 3

SceneInterior::SceneInterior()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

SceneInterior::~SceneInterior()
{
}

void SceneInterior::Init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	SwitchCamera = 1;
	rotate = 0;
	Yaw = 0;

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	fpsCamera.Init(Vector3(-10, 5, 0), Vector3(10, 0, 0), Vector3(0, 1, 0));
	topCamera.Init(Vector3(-10, 300, 0), Vector3(10, 0, 0), Vector3(0, 0, -1));

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);

	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	//m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Texture.fragmentshader"); 

	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	//Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	//Get a handle for our ""TextColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	glUseProgram(m_programID);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].position.Set(0, 60, 0);
	light[0].color.Set(1.f, 1.f, 1.f);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	light[1].type = Light::LIGHT_POINT;
	light[1].position.Set(fpsCamera.position.x, fpsCamera.position.y, fpsCamera.position.z);
	light[1].color.Set(0.f, 1.f, 1.f);
	light[1].power = 0.5;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.f;
	light[1].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &light[0].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);
	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &light[1].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);
	glUniform1i(m_parameters[U_NUMLIGHTS], 2);

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//sky.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//sky.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//skytop.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//sky.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//sky.tga");

	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("Floor", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_FLOOR]->textureID = LoadTGA("Image//showroom_floor.tga");

	meshList[GEO_LIGHTSPHERE] = MeshBuilder::GenerateSphere("lightBall", Color(1.f, 1.f, 1.f), 9, 36, 1.f);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
}

void SceneInterior::Update(double dt)
{
	if (Application::IsKeyPressed(0x31) && !Application::IsKeyPressed(VK_SHIFT))
	{
		glDisable(GL_CULL_FACE);
	}
	else if (Application::IsKeyPressed(0x32) && !Application::IsKeyPressed(VK_SHIFT))
	{
		glEnable(GL_CULL_FACE);
	}
	else if (Application::IsKeyPressed(0x33) && !Application::IsKeyPressed(VK_SHIFT))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (Application::IsKeyPressed(0x34) && !Application::IsKeyPressed(VK_SHIFT))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (Application::IsKeyPressed(VK_SHIFT) && Application::IsKeyPressed(0x31)) //0x31 is 1 key/;
	{
		dist = 0;
		CCar::CarSwitch = 0;
	}
	if (Application::IsKeyPressed(VK_SHIFT) && Application::IsKeyPressed(0x32))
	{
		dist = 0;
		CCar::CarSwitch = 1;
	}
	if (Application::IsKeyPressed(VK_SHIFT) && Application::IsKeyPressed(0x33))
	{
		dist = 0;
		CCar::CarSwitch = 2;
	}
	if (Application::IsKeyPressed(VK_SHIFT) && Application::IsKeyPressed(0x34))
	{
		dist = 0;
		CCar::CarSwitch = 3;
	}


	if (Application::IsKeyPressed('W') && SwitchCamera == 1)
	{
		moving = true;
		dist += (float)(dt * CCar::AllCar[CCar::CarSwitch]->GetCurrentSpeed());
		CCar::AllCar[CCar::CarSwitch]->SetDist(dist);
	}

	CCar::AllCar[CCar::CarSwitch]->CalculateSpeed(CCar::AllCar[CCar::CarSwitch]->GetAcceleration(), CCar::AllCar[CCar::CarSwitch]->GetCurrentSpeed(), dt);

	CCar::AllCar[CCar::CarSwitch]->SetCurrentSpeed(CCar::AllCar[CCar::CarSwitch]->GetCurrentSpeed());

	if (CCar::AllCar[CCar::CarSwitch]->GetCurrentSpeed() < 0 || moving == false)
	{
		CCar::AllCar[CCar::CarSwitch]->SetCurrentSpeed(0);
	}
	if (CCar::AllCar[CCar::CarSwitch]->GetCurrentSpeed() >= CCar::AllCar[CCar::CarSwitch]->GetMaxSpeed())
	{
		CCar::AllCar[CCar::CarSwitch]->SetCurrentSpeed(CCar::AllCar[CCar::CarSwitch]->GetMaxSpeed());
	}
	fpsCamera.SetCameraSpeed(CCar::AllCar[CCar::CarSwitch]->GetCurrentSpeed());

	float Cameraspeed = 50.f;
	if (Application::IsKeyPressed('A') && SwitchCamera == 1)
	{
		Yaw += Cameraspeed * dt * 2;
	}
	if (Application::IsKeyPressed('D') && SwitchCamera == 1)
	{
		Yaw -= Cameraspeed * dt * 2;
	}
	if (Application::IsKeyPressed('R') && SwitchCamera == 1)
	{
		Yaw = 0;
		dist = 0;
		CCar::AllCar[CCar::CarSwitch]->SetDist(dist);
	}

	moving = false;
    if (Application::IsKeyPressed(0x37) && SwitchCamera != 1)
    {
        SwitchCamera = 1;
    }
	if (Application::IsKeyPressed(0x38))
    {
        freeCamera.position = fpsCamera.position;
		freeCamera.TargetFromPos = fpsCamera.TargetFromPos;
		freeCamera.up = fpsCamera.up;
		freeCamera.target = fpsCamera.target;
		freeCamera.xzTarget = fpsCamera.xzTarget;
        SwitchCamera = 2;
    }
    if (Application::IsKeyPressed(0x39))
    {
        SwitchCamera = 3;
    }

	if (SwitchCamera == 2)
		freeCamera.Update(dt);
    else if (SwitchCamera == 3)
    {
        topCamera.Update(dt);
    }
    else
    {
        fpsCamera.Update(dt);
    }

	if (Application::IsKeyPressed('L'))
	{
		SceneManager::getInstance()->SetNextScene(SceneManager::SCENE_CAR_SELECTION);
	}
}

void SceneInterior::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (SwitchCamera == 1)
	{
		viewStack.LoadIdentity();
		viewStack.LookAt(fpsCamera.position.x, fpsCamera.position.y, fpsCamera.position.z, fpsCamera.target.x, fpsCamera.target.y, fpsCamera.target.z, fpsCamera.up.x, fpsCamera.up.y, fpsCamera.up.z);
		modelStack.LoadIdentity();
	}
	else if (SwitchCamera == 2)
	{
		viewStack.LoadIdentity();
		viewStack.LookAt(freeCamera.position.x, freeCamera.position.y, freeCamera.position.z, freeCamera.target.x, freeCamera.target.y, freeCamera.target.z, freeCamera.up.x, freeCamera.up.y, freeCamera.up.z);
		modelStack.LoadIdentity();
	}
	else if (SwitchCamera == 3)
	{
		viewStack.LoadIdentity();
		viewStack.LookAt(topCamera.position.x, topCamera.position.y, topCamera.position.z, topCamera.target.x, topCamera.target.y, topCamera.target.z, topCamera.up.x, topCamera.up.y, topCamera.up.z);
		modelStack.LoadIdentity();
	}

	// passing the light direction if it is a direction light	
	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	// if it is spot light, pass in position and direction 
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		// default is point light (only position since point light is 360 degrees)
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}
	if (light[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		// default is point light (only position since point light is 360 degrees)
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}

	RenderSkybox();

	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, -1.f, 0.f);
	modelStack.Scale(200.f, 200.f, 200.f);
	modelStack.Rotate(-90.f, 1.f, 0.f, 0.f);
		modelStack.PushMatrix();
		modelStack.Rotate(90.f, 0.f, 0.f, 1.f);
		RenderMesh(meshList[GEO_FLOOR], false);
		modelStack.PopMatrix();
	modelStack.PopMatrix();

	RenderRoom();

	if (CCar::CarSwitch == 0)
	{
		modelStack.PushMatrix();
		modelStack.Translate(fpsCamera.position.x, fpsCamera.position.y, fpsCamera.position.z);
		modelStack.PushMatrix();
		modelStack.Rotate(Yaw, 0, 1, 0);
			modelStack.PushMatrix();
				modelStack.Scale(0.05, 0.05, 0.05);
					modelStack.PushMatrix();
					modelStack.Translate(-0.3, -3.3, -0.8);
					RenderMesh(CCar::AllCar[CCar::CarSwitch]->GetInteriorMesh(), false);
					modelStack.PopMatrix();
				modelStack.PopMatrix();
			modelStack.PopMatrix();
		modelStack.PopMatrix();
	}
	else if (CCar::CarSwitch == 1)
	{
		modelStack.PushMatrix();
		modelStack.Translate(fpsCamera.position.x, fpsCamera.position.y, fpsCamera.position.z);
			modelStack.PushMatrix();
			modelStack.Rotate(Yaw, 0, 1, 0);
				modelStack.PushMatrix();
				modelStack.Translate(-4, -13, -0.2);
				RenderMesh(CCar::AllCar[CCar::CarSwitch]->GetInteriorMesh(), false);
				modelStack.PopMatrix();
			modelStack.PopMatrix();
		modelStack.PopMatrix();
	}
	else if (CCar::CarSwitch == 2)
	{
		modelStack.PushMatrix();
		modelStack.Translate(fpsCamera.position.x, fpsCamera.position.y, fpsCamera.position.z);
		modelStack.PushMatrix();
			modelStack.Rotate(Yaw, 0, 1, 0);
				modelStack.PushMatrix();
					modelStack.Translate(0, -2.7, 1.2);
					RenderMesh(CCar::AllCar[CCar::CarSwitch]->GetInteriorMesh(), false);	
				modelStack.PopMatrix();
			modelStack.PopMatrix();
		modelStack.PopMatrix();
	}
	else if (CCar::CarSwitch == 3)
	{
		modelStack.PushMatrix();
		modelStack.Translate(fpsCamera.position.x, fpsCamera.position.y, fpsCamera.position.z);
		modelStack.PushMatrix();
			modelStack.Rotate(Yaw, 0, 1, 0);
				modelStack.PushMatrix();
				modelStack.Rotate(180, 0, 1, 0);
					modelStack.PushMatrix();
					modelStack.Translate(0, -1.8, 0);
					RenderMesh(CCar::AllCar[CCar::CarSwitch]->GetInteriorMesh(), false);
					modelStack.PopMatrix();
				modelStack.PopMatrix();
			modelStack.PopMatrix();
		modelStack.PopMatrix();
	}

	std::string text = "Speed: " + std::to_string(CCar::AllCar[CCar::CarSwitch]->GetCurrentSpeed());
	RenderTextOnScreen(meshList[GEO_TEXT], text, Color(0, 1, 0), TEXT_SIZE, 0, 0);

	text = "Accerleration: " + std::to_string(CCar::AllCar[CCar::CarSwitch]->GetAcceleration());
	RenderTextOnScreen(meshList[GEO_TEXT], text, Color(0, 1, 0), TEXT_SIZE, 0, 1);

	text = "Distance: " + std::to_string(CCar::AllCar[CCar::CarSwitch]->GetDist());
	RenderTextOnScreen(meshList[GEO_TEXT], text, Color(0, 1, 0), TEXT_SIZE, 0, 2);

	text = "CarModel#: " + std::to_string(CCar::CarSwitch + 1);
	RenderTextOnScreen(meshList[GEO_TEXT], text, Color(0, 1, 0), TEXT_SIZE, 0, 19);

	RenderTextOnScreen(meshList[GEO_TEXT], "Camera: ", Color(0, 1, 0), TEXT_SIZE, 0, 18);
	switch (SwitchCamera)
	{
	case 1:
		RenderTextOnScreen(meshList[GEO_TEXT], "First Person", Color(1, 0, 1), TEXT_SIZE, 8, 18);
		break;
	case 2:
		RenderTextOnScreen(meshList[GEO_TEXT], "Free Camera", Color(0, 1, 1), TEXT_SIZE, 8, 18);
		break;
	case 3:
		RenderTextOnScreen(meshList[GEO_TEXT], "Top Down Camera", Color(1, 1, 0), TEXT_SIZE, 8, 18);
		break;
	default:
		break;
	}
}

void SceneInterior::RenderRoom()
{
	for (int wallIndex = 0; wallIndex < Wall::getNumOfWall(); wallIndex++)
	{
		Wall* wall = Wall::getWall(wallIndex);
		modelStack.PushMatrix();
		modelStack.Translate(wall->getPosition().x, wall->getPosition().y, wall->getPosition().z);
		modelStack.Rotate(90 - wall->getWallNormalRotation(), 0, 1, 0);
		modelStack.Scale(wall->getLength(), wall->getHeight(), wall->getDepth());
		RenderMesh(meshList[GEO_WALL], true);
		modelStack.PopMatrix();
	}
}

void SceneInterior::Exit()
{
	// Cleanup here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i] != NULL)
			delete meshList[i];
	}
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);

}

void SceneInterior::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);


	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0) {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once in the whole function

	if (mesh->textureID > 0) glBindTexture(GL_TEXTURE_2D, 0);
}

void SceneInterior::RenderSkybox()
{
	modelStack.PushMatrix();
	///scale, translate, rotate
	modelStack.Translate(-100.f, 40.f, 0.f);
	modelStack.Scale(200.f, 200.f, 200.f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(100.f, 40.f, 0.f);
	modelStack.Scale(200.f, 200.f, 200.f);
	modelStack.Rotate(-90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, 140.f, 0.f);
	modelStack.Scale(200.f, 200.f, 200.f);
	modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
	modelStack.PushMatrix();
	modelStack.Rotate(90.f, 0.f, 0.f, 1.f);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, -2.f, 0.f);
	modelStack.Scale(200.f, 200.f, 200.f);
	modelStack.Rotate(-90.f, 1.f, 0.f, 0.f);
	modelStack.PushMatrix();
	modelStack.Rotate(90.f, 0.f, 0.f, 1.f);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, 40.f, -100.f);
	modelStack.Scale(200.f, 200.f, 200.f);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, 40.f, 100.f);
	modelStack.Scale(200.f, 200.f, 200.f);
	modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();
}

void SceneInterior::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0)
		return;
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); i++)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneInterior::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0)
		return;
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); i++)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}
