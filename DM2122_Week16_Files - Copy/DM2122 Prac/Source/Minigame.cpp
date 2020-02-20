#include "Minigame.h"
#include "GL\glew.h"
#include "Application.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
//#include "PathBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"

#define ROT_LIMIT 45.f;
#define SCALE_LIMIT 5.f;
#define LSPEED 10.f

Minigame::Minigame()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

Minigame::~Minigame()
{
}

void Minigame::Init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	FPScamera.Init(Vector3(-10, 2, 0), Vector3(10, 0, 0), Vector3(0, 1, 0));
	Frecamera.Init(Vector3(-10, 2, 0), Vector3(10, 0, 0), Vector3(0, 1, 0));
	TPSCamera.Init(Vector3(-30, 2, 0), FPScamera.position, FPScamera.up);
	TopCamera.Init(Vector3(-10, 300, 0), Vector3(-10, 0, 0), Vector3(1, 0, 0));

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
	light[1].position.Set(FPScamera.position.x, FPScamera.position.y, FPScamera.position.z);
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

	meshList[GEO_CHAR] = MeshBuilder::GenerateQuad("char", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_CHAR]->textureID = LoadTGA("Image//char.tga");

	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("Floor", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_FLOOR]->textureID = LoadTGA("Image//Grass.tga");

	meshList[GEO_LIGHTSPHERE] = MeshBuilder::GenerateSphere("lightBall", Color(1.f, 1.f, 1.f), 9, 36, 0.5f);

	meshList[GEO_CARTARGET] = MeshBuilder::GenerateSphere("lightBall", Color(0.f, 1.f, 1.f), 9, 36, 0.5f);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_PLAYER] = MeshBuilder::GenerateOBJ("Car", "OBJ//car_frame.obj");
	meshList[GEO_PLAYER]->textureID = LoadTGA("Image//CarBody_texture.tga");
	Car1.GeneratePath("OBJ//Path.obj",1.5,Vector3(0,0,10));//PathObj , scale, Offset
	Car1.init(Vector3(0, 0, 0), Vector3(0, 0, 100), Vector3(0, 1, 0));
	SwitchCamera = 1;
	switchcolor = false;
	lightcolor = 0.f;
}

void Minigame::Update(double dt)
{
	if (lightcolor >= 0 && !switchcolor)//Switching color
	{
		lightcolor += 0.5 * dt;
	}
	if (lightcolor >= 1.f)
	{
		lightcolor = 1.f;
		switchcolor = true;
	}
	if (lightcolor <= 1.f && switchcolor)
	{
		lightcolor -= 0.5 * dt;
	}
	if (lightcolor <= 0.f)
	{
		lightcolor = 0.f;
		switchcolor = false;
	}
	if (Application::IsKeyPressed('J'))//Button to switch
	{
		light[1].color.Set(0 + lightcolor, 1 - lightcolor, lightcolor / 2);
		glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	}
	if (Application::IsKeyPressed(0x31))
	{
		glDisable(GL_CULL_FACE);
	}
	else if (Application::IsKeyPressed(0x32))
	{
		glEnable(GL_CULL_FACE);
	}
	else if (Application::IsKeyPressed(0x33))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (Application::IsKeyPressed(0x34))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	//light[1].position.Set(camera.position.x, camera.position.y, camera.position.z);
	if (Application::IsKeyPressed('I'))
		light[0].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K'))
		light[0].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		light[0].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		light[0].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('O'))
		light[0].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('P'))
		light[0].position.y += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('Z') && SwitchCamera != 2)
	{
		Frecamera.position = FPScamera.position;
		Frecamera.TargetFromPos = FPScamera.TargetFromPos;
		Frecamera.up = FPScamera.up;
		Frecamera.target = FPScamera.target;
		Frecamera.xzTarget = FPScamera.xzTarget;
		SwitchCamera = 2;
	}
	if (Application::IsKeyPressed('X'))
	{
		SwitchCamera = 1;
	}
	if (Application::IsKeyPressed('C'))
	{
		SwitchCamera = 3;
	}
	if (Application::IsKeyPressed('V'))
	{
		SwitchCamera = 4;
	}
	if (Application::IsKeyPressed('5'))
	{
		//to do: switch light type to POINT and pass the information to
		light[0].type = Light::LIGHT_POINT;
	}
	else if (Application::IsKeyPressed('6'))
	{
		//to do: switch light type to DIRECTIONAL and pass the
		light[0].type = Light::LIGHT_DIRECTIONAL;
	}
	else if (Application::IsKeyPressed('7'))
	{
		//to do: switch light type to SPOT and pass the information to
		light[0].type = Light::LIGHT_SPOT;
	}
	float Cameraspeed = 50.f;
	if (Application::IsKeyPressed(VK_LEFT) && SwitchCamera != 2)
	{
		Yaw += Cameraspeed * dt * 2;
	}
	if (Application::IsKeyPressed(VK_RIGHT) && SwitchCamera != 2)
	{
		Yaw -= Cameraspeed * dt * 2;
	}
	if (Application::IsKeyPressed('R'))
	{
		Yaw = 0;
	}
	if (SwitchCamera == 2)
		Frecamera.Update(dt);
	else
	{
		FPScamera.Update(dt);
		TPSCamera.Update(dt);
		TopCamera.Update(dt);
	}
	light[1].position.Set(FPScamera.position.x, FPScamera.position.y, FPScamera.position.z);
	Car1.Updates(dt);
}

void Minigame::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	if (SwitchCamera == 2)
		viewStack.LookAt(Frecamera.position.x, Frecamera.position.y, Frecamera.position.z, Frecamera.target.x, Frecamera.target.y, Frecamera.target.z, Frecamera.up.x, Frecamera.up.y, Frecamera.up.z);
	else if (SwitchCamera == 1)
		viewStack.LookAt(FPScamera.position.x, FPScamera.position.y, FPScamera.position.z, FPScamera.target.x, FPScamera.target.y, FPScamera.target.z, FPScamera.up.x, FPScamera.up.y, FPScamera.up.z);
	else if(SwitchCamera==3)
		viewStack.LookAt(TPSCamera.position.x, TPSCamera.position.y, TPSCamera.position.z, TPSCamera.target.x, TPSCamera.target.y, TPSCamera.target.z, TPSCamera.up.x, TPSCamera.up.y, TPSCamera.up.z);
	else
		viewStack.LookAt(TopCamera.position.x, TopCamera.position.y, TopCamera.position.z, TopCamera.target.x, TopCamera.target.y, TopCamera.target.z, TopCamera.up.x, TopCamera.up.y, TopCamera.up.z);
	modelStack.LoadIdentity();

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
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, -1.f, 0.f);
	modelStack.Scale(200.f, 200.f, 200.f);
	modelStack.Rotate(-90.f, 1.f, 0.f, 0.f);
	modelStack.Rotate(90.f, 0.f, 0.f, 1.f);
	RenderMesh(meshList[GEO_FLOOR], false);
	modelStack.PopMatrix();
	RenderSkybox();
	RenderEnviroment();
	RenderPlayer();
	modelStack.PushMatrix();
	modelStack.Translate(Car1.position.x, Car1.position.y, Car1.position.z);
	modelStack.Rotate(Car1.rotationy, 0, 1, 0);
	RenderMesh(meshList[GEO_PLAYER], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTSPHERE], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(light[1].position.x, light[1].position.y, light[1].position.z);
	RenderMesh(meshList[GEO_LIGHTSPHERE], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	RenderText(meshList[GEO_TEXT], "HELLO WORLD", Color(0, 1, 0));
	modelStack.PopMatrix();
	RenderTextOnScreen(meshList[GEO_TEXT], "Hello World", Color(0, 1, 0), 4, 0, 0,true);
	RenderTextOnScreen(meshList[GEO_TEXT], "+", Color(0, 1, 0), 4, 9.85, 7.1,false);
	RenderMeshOnScreen(meshList[GEO_TEXT], 4, 4, 10, 7., 1);
}

void Minigame::Exit()
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

void Minigame::RenderMesh(Mesh* mesh, bool enableLight)
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

void Minigame::RenderSkybox()
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

void Minigame::RenderPlayer()
{
	//modelStack.PushMatrix();
	//modelStack.Translate(FPScamera.position.x+10, FPScamera.position.y, FPScamera.position.z);
	//RenderMesh(meshList[GEO_PLAYER], false);
	//modelStack.PopMatrix();
}


void Minigame::RenderEnviroment()//Put Enviromentobject here ETC Cars tand,station car,booth ,plants, well anything static
{
	for (int i = 0; i < Car1.Point.size(); i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(Car1.Point[i].x, Car1.Point[i].y, Car1.Point[i].z);
		RenderMesh(meshList[GEO_LIGHTSPHERE], false);
		modelStack.PopMatrix();
	}
	modelStack.PushMatrix();
	modelStack.Translate(Car1.target.x, Car1.target.y, Car1.target.z);
	RenderMesh(meshList[GEO_CARTARGET], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(Car1.TargetFromPos.x, Car1.TargetFromPos.y, Car1.TargetFromPos.z);
	RenderMesh(meshList[GEO_CARTARGET], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(Car1.defaultPosition.x, Car1.defaultPosition.y, Car1.defaultPosition.z);
	RenderMesh(meshList[GEO_CARTARGET], false);
	modelStack.PopMatrix();
	/*modelStack.PushMatrix();
	modelStack.Translate(Car1.Point[0].x, Car1.Point[0].y, Car1.Point[0].z);
	RenderMesh(meshList[GEO_PLAYER], false);
	modelStack.PopMatrix();*/
}

void Minigame::RenderText(Mesh* mesh, std::string text, Color color)
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

void Minigame::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y,bool scaleFirst)
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
void Minigame::RenderMeshOnScreen(Mesh* mesh, float sizex,float sizey, float x, float y,int animFrames)
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
	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizex, sizey,1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	mesh->Render((unsigned)animFrames * 6, 6);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}