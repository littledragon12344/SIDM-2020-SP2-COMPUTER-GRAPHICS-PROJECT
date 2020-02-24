#include "SceneCarSelection.h"
#include "GL\glew.h"
#include "Application.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"

#define ROT_LIMIT 45.f;
#define SCALE_LIMIT 5.f;
#define LSPEED 10.f

SceneCarSelection::SceneCarSelection()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

SceneCarSelection::~SceneCarSelection()
{
}

void SceneCarSelection::Init()
{
	glClearColor(0.f, 0.f, 0.f, 0.0f);
	
	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera.Init(Vector3(0, 17, 0), Vector3(10, 17, 0), Vector3(0, 1, 0));

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
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	//Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");


	glUseProgram(m_programID);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
/*
	light[0].type = Light::LIGHT_SPOT;
	light[0].position.Set(75, 90, 0);
	light[0].color.Set(0.5f, 0.5f, 0.5f);
	light[0].power = 15;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);*/


	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].position.Set(-5, 9, 0);
	light[0].color.Set(0.5f, 0.5f, 0.5f);
	light[0].power = 3;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

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
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibriBoldItalic.tga");

	// Generating all cars

	// Car 1
	meshList[GEO_CAR_FRAME_1] = MeshBuilder::GenerateOBJ("car", "OBJ//carframe3.obj");
	meshList[GEO_CAR_FRAME_1]->textureID = LoadTGA("Image//spcarframe2.tga");
	
	meshList[GEO_CAR_WHEEL_1] = MeshBuilder::GenerateOBJ("car", "OBJ//car1_wheels.obj");
	meshList[GEO_CAR_WHEEL_1]->textureID = LoadTGA("Image//spcar2_wheel.tga");

	// Car 2
	meshList[GEO_CAR_FRAME_2] = MeshBuilder::GenerateOBJ("car", "OBJ//carframe3.obj");
	meshList[GEO_CAR_FRAME_2]->textureID = LoadTGA("Image//spcarframe3.tga");

	meshList[GEO_CAR_WHEEL_2] = MeshBuilder::GenerateOBJ("car", "OBJ//car1_wheels.obj");
	meshList[GEO_CAR_WHEEL_2]->textureID = LoadTGA("Image//spcar2_wheel.tga");

	// Car 3
	meshList[GEO_CAR_FRAME_3] = MeshBuilder::GenerateOBJ("car", "OBJ//carframe3.obj");
	meshList[GEO_CAR_FRAME_3]->textureID = LoadTGA("Image//spcarframe4.tga");

	meshList[GEO_CAR_WHEEL_3] = MeshBuilder::GenerateOBJ("car", "OBJ//car1_wheels.obj");
	meshList[GEO_CAR_WHEEL_3]->textureID = LoadTGA("Image//spcar2_wheel.tga");

	// Car 4
	meshList[GEO_CAR_FRAME_4] = MeshBuilder::GenerateOBJ("car", "OBJ//carframe3.obj");
	meshList[GEO_CAR_FRAME_4]->textureID = LoadTGA("Image//spcarFrame1.tga");

	meshList[GEO_CAR_WHEEL_4] = MeshBuilder::GenerateOBJ("car", "OBJ//car1_wheels.obj");
	meshList[GEO_CAR_WHEEL_4]->textureID = LoadTGA("Image//spcar2_wheel.tga");

	// User Interface / UI
	meshList[GEO_UI_BASE] = MeshBuilder::GenerateQuad("uiBase", Color(0.55f, 0.55f, 0.55f), 27.5f, 12.8f);
	meshList[GEO_UI_GRAY_BAR] = MeshBuilder::GenerateQuad("uiGrayBlue", Color(0.4f, 0.4f, 0.4f), 1.f, 1.f);
	meshList[GEO_UI_BLUE_BAR] = MeshBuilder::GenerateQuad("uiGrayBlue", Color(0.2f, 0.6f, 1.f), 1.f, 1.f);

	meshList[GEO_BACKGROUND] = MeshBuilder::GenerateQuad("background", Color(0, 0, 0), 200, 150);
	meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image//background1.tga");

	// Initialising other variables
	carAngle = 155.f; // Starting angle

	accel = 20.f;
	topSpd = 30.f;

	numOfCars = 4;
	currentCarNo = 1;

	animAngle = 0.f;

	bPressesd = false;
	backgroundNo = 1;
}

void SceneCarSelection::Update(double dt)
{
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

	// Switch to the next car
	if (Application::IsKeyPressed('C') && animAngle == 0.f)
	{
		currentCarNo = (currentCarNo) % numOfCars + 1;
		animAngle += 120.f * dt;
	}

	// Switch to the previous car
	if (Application::IsKeyPressed('V') && animAngle == 0.f)
	{
		currentCarNo = (currentCarNo + 2) % numOfCars + 1;
		animAngle -= 120.f * dt;
	}

	// Resetting animAngle
	if (abs(animAngle) >= 89.f)
		animAngle = 0.f;
	// Adding to animAngle if its > 0
	else if (animAngle > 0.f)
		animAngle += 120.f * dt;
	// Adding to animAngle if its < 0
	else if (animAngle < 0.f) 
		animAngle -= 120.f * dt;

	// Rotate the car clockwise from top view
	if (Application::IsKeyPressed('Z') && animAngle == 0.f)
		carAngle -= 3.f;	
	// Rotate the car anit-clockwise from top view
	if (Application::IsKeyPressed('X') && animAngle == 0.f)
		carAngle += 3.f;

	// Only allows player to press B if the player has already released B.
	// If never do this, the code if B is pressed will run every frame which would mean that the background changes every frame.
	if (Application::IsKeyPressed('B') && !bPressesd)
	{
		bPressesd = true;
		backgroundNo = (backgroundNo + 2) % 2 + 1;
		if (backgroundNo == 1)
			meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image//background1.tga");
		else if (backgroundNo == 2)
			meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image//background2.tga");
	}
	// Runs when player let go of bPressed
	if (!Application::IsKeyPressed('B') && bPressesd)
	{
		bPressesd = false;
	}
}

void SceneCarSelection::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();

	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);

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
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	// Rendering the car depending if the car is still switching
	// If its still switching, render 2 cars, the current car and the previous car shown
	if (animAngle > 0)
	{
		modelStack.PushMatrix();
			modelStack.Rotate(animAngle, 0, 1, 0);
			modelStack.Translate(75.f, 0.f, 0.f);
			modelStack.Rotate(carAngle, 0, 1, 0);
			modelStack.Scale(10.f, 10.f, 10.f);
			RenderCar((currentCarNo + 2) % numOfCars + 1);
		modelStack.PopMatrix();
		
		modelStack.PushMatrix();
			modelStack.Rotate(animAngle - 90.f, 0, 1, 0);
			modelStack.Translate(75.f, 0.f, 0.f);
			modelStack.Rotate(carAngle, 0, 1, 0);
			modelStack.Scale(10.f, 10.f, 10.f);
			RenderCar(currentCarNo);
		modelStack.PopMatrix();
	}
	else if (animAngle < 0)
	{
		modelStack.PushMatrix();
			modelStack.Rotate(animAngle, 0, 1, 0);
			modelStack.Translate(75.f, 0.f, 0.f);
			modelStack.Rotate(carAngle, 0, 1, 0);
			modelStack.Scale(10.f, 10.f, 10.f);
			RenderCar((currentCarNo) % numOfCars + 1);
		modelStack.PopMatrix();
		
		modelStack.PushMatrix();
			modelStack.Rotate(animAngle + 90.f, 0, 1, 0);
			modelStack.Translate(75.f, 0.f, 0.f);
			modelStack.Rotate(carAngle, 0, 1, 0);
			modelStack.Scale(10.f, 10.f, 10.f);
			RenderCar(currentCarNo);
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
			modelStack.Translate(75.f, 0.f, 0.f);
			modelStack.Rotate(carAngle, 0, 1, 0);
			modelStack.Scale(10.f, 10.f, 10.f);
			RenderCar(currentCarNo);
		modelStack.PopMatrix();
	}

	// Background
	modelStack.PushMatrix();
		modelStack.Translate(180.f, 17.f, 0);
		modelStack.Rotate(-90.f, 0.f, 1.f, 0.f);
		RenderMesh(meshList[GEO_BACKGROUND], false);
	modelStack.PopMatrix();

	// User Interface (UI)
	RenderMeshOnScreen(meshList[GEO_UI_BASE], 17.f, 50.f, 1.f, 1.f);
	RenderTextOnScreen(meshList[GEO_TEXT], "CARNAME", Color(1, 1, 1), 2.1f, 5.f, 53.5f);

	// Top speed
	RenderTextOnScreen(meshList[GEO_TEXT], "TOP SPEED    " + std::to_string((int)accel) + "KM/H", Color(1, 1, 1), 2.1f, 5.f, 51.f);
	RenderMeshOnScreen(meshList[GEO_UI_GRAY_BAR], 17.f, 50.f, 24.f, 1.f);
	RenderMeshOnScreen(meshList[GEO_UI_BLUE_BAR], 5.f + 24.f * (topSpd / 100.f), 50.f, 24.f * (topSpd / 50.f), 1.5f);

	// Acceleration
	RenderTextOnScreen(meshList[GEO_TEXT], "ACCELERATION " + std::to_string((int)accel) + "KM/H2", Color(1, 1, 1), 2.1f, 5.f, 47.f);
	RenderMeshOnScreen(meshList[GEO_UI_GRAY_BAR], 17.f, 46.f, 24.f, 1.f);
	RenderMeshOnScreen(meshList[GEO_UI_BLUE_BAR], 5.f + 24.f * (accel / 100.f), 46.f, 24.f * (accel / 50.f), 1.5f);
}

void SceneCarSelection::Exit()
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

void SceneCarSelection::RenderMesh(Mesh* mesh, bool enableLight)
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

void SceneCarSelection::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);

}

void SceneCarSelection::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.6f, 0, 0); //1.0f is the spacing of each character, you may change this value
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

void SceneCarSelection::CalculateFrameRate()
{
	static float framesPerSecond = 0.0f;
	static int fps;
	static float lastTime = 0.0f;
	float currentTime = GetTickCount() * 0.001f;
	++framesPerSecond;
	printf("Current Frames Per Second: %d\n\n", fps);
	if (currentTime - lastTime > 1.0f)
	{
		lastTime = currentTime;
		fps = (int)framesPerSecond;
		framesPerSecond = 0;
	}
}

void SceneCarSelection::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); // size of screen
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); // No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	//to do: scale and translate accordingly
	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizex, sizey, 1);
	RenderMesh(mesh, false); // UI should not have light
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneCarSelection::RenderCar(int carNo)
{
	switch (carNo)
	{
	case 1:
		// Car 2
		modelStack.PushMatrix();
			RenderMesh(meshList[GEO_CAR_FRAME_1], true);
			// Front left wheel
			modelStack.PushMatrix();
				modelStack.Translate(1.67f, 0.48f, -1.2f);
				RenderMesh(meshList[GEO_CAR_WHEEL_1], true);
			modelStack.PopMatrix();
			// Front right wheel
			modelStack.PushMatrix();
				modelStack.Translate(1.67f, 0.48f, 1.2f);
				RenderMesh(meshList[GEO_CAR_WHEEL_1], false);
			modelStack.PopMatrix();
			// Back left wheel
			modelStack.PushMatrix();
				modelStack.Translate(-1.32f, 0.48f, -1.2f);
				RenderMesh(meshList[GEO_CAR_WHEEL_1], false);
			modelStack.PopMatrix();
			// Back left wheel
			modelStack.PushMatrix();
				modelStack.Translate(-1.32f, 0.48f, 1.2f);
				RenderMesh(meshList[GEO_CAR_WHEEL_1], false);
			modelStack.PopMatrix();
		modelStack.PopMatrix();
	case 2:
		// Car 3
		modelStack.PushMatrix();
			RenderMesh(meshList[GEO_CAR_FRAME_2], true);
			// Front left wheel
			modelStack.PushMatrix();
				modelStack.Translate(1.67f, 0.48f, -1.2f);
				RenderMesh(meshList[GEO_CAR_WHEEL_2], true);
			modelStack.PopMatrix();
			// Front right wheel
			modelStack.PushMatrix();
				modelStack.Translate(1.67f, 0.48f, 1.2f);
				RenderMesh(meshList[GEO_CAR_WHEEL_2], false);
			modelStack.PopMatrix();
			// Back left wheel
			modelStack.PushMatrix();
				modelStack.Translate(-1.32f, 0.48f, -1.2f);
				RenderMesh(meshList[GEO_CAR_WHEEL_2], false);
			modelStack.PopMatrix();
			// Back left wheel
			modelStack.PushMatrix();
				modelStack.Translate(-1.32f, 0.48f, 1.2f);
				RenderMesh(meshList[GEO_CAR_WHEEL_2], false);
			modelStack.PopMatrix();
		modelStack.PopMatrix();
	case 3:
		// Car 4
		modelStack.PushMatrix();
			RenderMesh(meshList[GEO_CAR_FRAME_3], true);
			// Front left wheel
			modelStack.PushMatrix();
				modelStack.Translate(1.67f, 0.48f, -1.2f);
				RenderMesh(meshList[GEO_CAR_WHEEL_3], true);
			modelStack.PopMatrix();
			// Front right wheel
			modelStack.PushMatrix();
				modelStack.Translate(1.67f, 0.48f, 1.2f);
				RenderMesh(meshList[GEO_CAR_WHEEL_3], false);
			modelStack.PopMatrix();
			// Back left wheel
			modelStack.PushMatrix();
				modelStack.Translate(-1.32f, 0.48f, -1.2f);
				RenderMesh(meshList[GEO_CAR_WHEEL_3], false);
			modelStack.PopMatrix();
			// Back left wheel
			modelStack.PushMatrix();
				modelStack.Translate(-1.32f, 0.48f, 1.2f);
				RenderMesh(meshList[GEO_CAR_WHEEL_3], false);
			modelStack.PopMatrix();
		modelStack.PopMatrix();
	case 4:
		// Car 1
		modelStack.PushMatrix();
			RenderMesh(meshList[GEO_CAR_FRAME_4], true);
			// Front left wheel
			modelStack.PushMatrix();
				modelStack.Translate(1.67f, 0.48f, -1.2f);
				RenderMesh(meshList[GEO_CAR_WHEEL_4], true);
			modelStack.PopMatrix();
			// Front right wheel
			modelStack.PushMatrix();
				modelStack.Translate(1.67f, 0.48f, 1.2f);
				RenderMesh(meshList[GEO_CAR_WHEEL_4], false);
			modelStack.PopMatrix();
			// Back left wheel
			modelStack.PushMatrix();
				modelStack.Translate(-1.32f, 0.48f, -1.2f);
				RenderMesh(meshList[GEO_CAR_WHEEL_4], false);
			modelStack.PopMatrix();
			// Back left wheel
			modelStack.PushMatrix();
				modelStack.Translate(-1.32f, 0.48f, 1.2f);
				RenderMesh(meshList[GEO_CAR_WHEEL_4], false);
			modelStack.PopMatrix();
		modelStack.PopMatrix();
	}
}