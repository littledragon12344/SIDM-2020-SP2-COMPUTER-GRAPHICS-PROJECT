#include "SceneSkybox.h"
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

SceneSkybox::SceneSkybox()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

SceneSkybox::~SceneSkybox()
{
}

void SceneSkybox::Init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	rotate = 0;
	translateY1 = 0.7f;
	translateY2 = 0.5f;
	translateY3 = 1.2f;
	translateY4 = 1.5f;
	Cameraspeed = 25.f;
	movesUp = false;

	//for NPC
	bounceTime = 0;
	timer = 0;

	//NPC BOOLS//
	SpeakIntro = false;
	Speak1 = false;
	Speak2 = false;
	Speak3 = false;
	Speak4 = false;

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	FPScamera.Init(Vector3(-10, 2, 0), Vector3(10, 0, 0), Vector3(0, 1, 0));
	Frecamera.Init(Vector3(-10, 2, 0), Vector3(10, 0, 0), Vector3(0, 1, 0));

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
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");

	m_parameters[U_LIGHT3_POSITION] = glGetUniformLocation(m_programID, "lights[3].position_cameraspace");
	m_parameters[U_LIGHT3_COLOR] = glGetUniformLocation(m_programID, "lights[3].color");
	m_parameters[U_LIGHT3_POWER] = glGetUniformLocation(m_programID, "lights[3].power");
	m_parameters[U_LIGHT3_KC] = glGetUniformLocation(m_programID, "lights[3].kC");
	m_parameters[U_LIGHT3_KL] = glGetUniformLocation(m_programID, "lights[3].kL");
	m_parameters[U_LIGHT3_KQ] = glGetUniformLocation(m_programID, "lights[3].kQ");
	m_parameters[U_LIGHT3_TYPE] = glGetUniformLocation(m_programID, "lights[3].type");
	m_parameters[U_LIGHT3_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[3].spotDirection");
	m_parameters[U_LIGHT3_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[3].cosCutoff");
	m_parameters[U_LIGHT3_COSINNER] = glGetUniformLocation(m_programID, "lights[3].cosInner");
	m_parameters[U_LIGHT3_EXPONENT] = glGetUniformLocation(m_programID, "lights[3].exponent");

	m_parameters[U_LIGHT4_POSITION] = glGetUniformLocation(m_programID, "lights[4].position_cameraspace");
	m_parameters[U_LIGHT4_COLOR] = glGetUniformLocation(m_programID, "lights[4].color");
	m_parameters[U_LIGHT4_POWER] = glGetUniformLocation(m_programID, "lights[4].power");
	m_parameters[U_LIGHT4_KC] = glGetUniformLocation(m_programID, "lights[4].kC");
	m_parameters[U_LIGHT4_KL] = glGetUniformLocation(m_programID, "lights[4].kL");
	m_parameters[U_LIGHT4_KQ] = glGetUniformLocation(m_programID, "lights[4].kQ");
	m_parameters[U_LIGHT4_TYPE] = glGetUniformLocation(m_programID, "lights[4].type");
	m_parameters[U_LIGHT4_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[4].spotDirection");
	m_parameters[U_LIGHT4_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[4].cosCutoff");
	m_parameters[U_LIGHT4_COSINNER] = glGetUniformLocation(m_programID, "lights[4].cosInner");
	m_parameters[U_LIGHT4_EXPONENT] = glGetUniformLocation(m_programID, "lights[4].exponent");

	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	//Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	//Get a handle for our ""TextColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	glUseProgram(m_programID);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	light[0].type = Light::LIGHT_SPOT;
	light[0].position.Set(10.f, 20.f, 20.f);
	light[0].color.Set(0.5f, 0.5f, 0.5f);
	light[0].power = 3;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	light[1].type = Light::LIGHT_SPOT;
	light[1].position.Set(10.f, 20.f, -20.f);
	light[1].color.Set(0.5f, 0.5f, 0.5f);
	light[1].power = 3;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.f;
	light[1].spotDirection.Set(0.f, 1.f, 0.f);

	light[2].type = Light::LIGHT_SPOT;
	light[2].position.Set(-20.f, 20.f, 20.f);
	light[2].color.Set(0.5f, 0.5f, 0.5f);
	light[2].power = 3;
	light[2].kC = 1.f;
	light[2].kL = 0.01f;
	light[2].kQ = 0.001f;
	light[2].cosCutoff = cos(Math::DegreeToRadian(45));
	light[2].cosInner = cos(Math::DegreeToRadian(30));
	light[2].exponent = 3.f;
	light[2].spotDirection.Set(0.f, 1.f, 0.f);

	light[3].type = Light::LIGHT_SPOT;
	light[3].position.Set(-20.f, 20.f, -20.f);
	light[3].color.Set(0.5f, 0.5f, 0.5f);
	light[3].power = 3;
	light[3].kC = 1.f;
	light[3].kL = 0.01f;
	light[3].kQ = 0.001f;
	light[3].cosCutoff = cos(Math::DegreeToRadian(45));
	light[3].cosInner = cos(Math::DegreeToRadian(30));
	light[3].exponent = 3.f;
	light[3].spotDirection.Set(0.f, 1.f, 0.f);

	light[4].type = Light::LIGHT_DIRECTIONAL;
	light[4].position.Set(0.f, 10.f, 10.f);
	light[4].color.Set(0.5f, 0.5f, 0.5f);
	light[4].power = 2;
	light[4].kC = 1.f;
	light[4].kL = 0.01f;
	light[4].kQ = 0.001f;
	light[4].cosCutoff = cos(Math::DegreeToRadian(45));
	light[4].cosInner = cos(Math::DegreeToRadian(30));
	light[4].exponent = 3.f;
	light[4].spotDirection.Set(0.f, 1.f, 0.f);

	// Passing light values to shader
	for (int i = 0; i < 5; ++i)
	{
		int tmp = i * 11 + U_LIGHT0_POSITION + 1;
		glUniform3fv(m_parameters[tmp], 1, &light[i].color.r);
		glUniform1f(m_parameters[tmp + 1], light[i].power);
		glUniform1f(m_parameters[tmp + 2], light[i].kC);
		glUniform1f(m_parameters[tmp + 3], light[i].kL);
		glUniform1f(m_parameters[tmp + 4], light[i].kQ);
		glUniform1i(m_parameters[tmp + 5], light[i].type);
		glUniform3fv(m_parameters[tmp + 6], 1, &light[i].spotDirection.x);
		glUniform1f(m_parameters[tmp + 7], light[i].cosCutoff);
		glUniform1f(m_parameters[tmp + 8], light[i].cosInner);
		glUniform1f(m_parameters[tmp + 9], light[i].exponent);
	}

	glUniform1i(m_parameters[U_NUMLIGHTS], 5); 

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
	meshList[GEO_FLOOR]->textureID = LoadTGA("Image//showroom_floor.tga");

	meshList[GEO_STAND] = MeshBuilder::GenerateOBJ("stand", "Obj//car_stand.obj");
	meshList[GEO_STAND]->textureID = LoadTGA("Image//stand.tga");

	meshList[GEO_WALL] = MeshBuilder::GenerateCuboid("wall", Color(1, 1, 1), 1, 1, 1);
	Wall::generateWalls("Obj//wall_room.obj");

	meshList[GEO_ROOF] = MeshBuilder::GenerateCuboid("roof", Color(0, 0, 0.5), 130.f, 1.f, 130.f);

	meshList[GEO_LIGHTSPHERE1] = MeshBuilder::GenerateSphere("lightBall", Color(1.f, 1.f, 1.f), 9, 36, 1.f);

	meshList[GEO_LIGHTSPHERE2] = MeshBuilder::GenerateSphere("lightBall2", Color(1.f, 1.f, 1.f), 9, 36, 1.f);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	SwitchCamera = 1;

	meshList[GEO_NPCB] = MeshBuilder::GenerateOBJ("NPC", "OBJ//NPC_Body.obj");
	NPC[0].setCoords(0, 0);
	meshList[GEO_TEXTBOX] = MeshBuilder::GenerateQuad("TextBox", Color(0, 0, 0), 3, 1);
}

void SceneSkybox::Update(double dt)
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
	
	if (Application::IsKeyPressed('Z')&&SwitchCamera!=2)
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
	
	if (Application::IsKeyPressed('L'))
	{
		SceneManager::getInstance()->SetNextScene(SceneManager::SCENE_CAR_SELECTION);
	}

	if (Application::IsKeyPressed(VK_LEFT) && SwitchCamera!=2)
	{
		Yaw += Cameraspeed * dt * 2;
	}
	if (Application::IsKeyPressed(VK_RIGHT)&&SwitchCamera!=2)
	{
		Yaw -= Cameraspeed * dt * 2;
	}
	if (Application::IsKeyPressed('R'))
	{
		Yaw =0;
	}
	if (SwitchCamera == 2)
	{
		ShowCursor(true);
		Frecamera.Update(dt);
	}
	else
	{
		ShowCursor(false);
		FPScamera.Update(dt);
	}
	//light[1].position.Set(FPScamera.position.x, FPScamera.position.y, FPScamera.position.z);

	if (rotate < 360)
	{
		rotate += (float)(dt * LSPEED);
	}

	else if (rotate >= 360)
	{
		rotate = 0;
	}

	if (translateY1 > 0.4f && !movesUp)
	{
		translateY1 -= (float)(dt * 0.1);

	}

	if (translateY1 <= 0.4f)
	{
		translateY1 = 0.4;
		movesUp = true;
	}

	if (translateY1 < 0.8f && movesUp)
	{
		translateY1 += (float)(dt * 0.1);
	}

	if (translateY1 >= 0.8f)
	{
		translateY1 = 0.8f;
		movesUp = false;
	}

	if (translateY2 > -0.2f && !movesUp)
	{
		translateY2 -= (float)(dt * 0.1);

	}

	if (translateY2 <= -0.2f)
	{
		translateY2 = -0.2;
		movesUp = true;
	}

	if (translateY2 < 0.3f && movesUp)
	{
		translateY2 += (float)(dt * 0.1);
	}

	if (translateY2 >= 0.3f)
	{
		translateY2 = 0.3f;
		movesUp = false;
	}


	if (translateY3 > 0.4f && !movesUp)
	{
		translateY3 -= (float)(dt * 0.1);

	}

	if (translateY3 <= 0.4f)
	{
		translateY3 = 0.4f;
		movesUp = true;
	}

	if (translateY3 < 0.8f && movesUp)
	{
		translateY3 += (float)(dt * 0.1);
	}

	if (translateY3 >= 0.8f)
	{
		translateY3 = 0.8f;
		movesUp = false;
	}

	if (translateY4 > 0.5f && !movesUp)
	{
		translateY4 -= (float)(dt * 0.1);

	}

	if (translateY4 <= 0.5f)
	{
		translateY4 = 0.5f;
		movesUp = true;
	}

	if (translateY4 < 1.5f && movesUp)
	{
		translateY4 += (float)(dt * 0.1);
	}

	if (translateY4 >= 1.5f)
	{
		translateY4 = 1.5f;
		movesUp = false;
	}

	//for NPC
	if (bounceTime > 0)
	{
		bounceTime -= (float)(1 * dt);
	}

	if (timer > 0)
	{
		timer = timer - (float)(1 * dt);
	}

	//Car 1: (-15, -15)
	if (SpeakIntro == true && Speak1 == false && movetocar1 != -15.0f && movetocar1 > -15.0f && move1 == true)
	{
		movetocar1 -= (float)(dt * 10.f);
		NPC->setCoords(movetocar1, movetocar1);
		timer = 1;


	}
	//Car 2: (-15, 15)
	if (Speak1 == true && Speak2 == false && movetocar2x != -15.0f && movetocar2z < 15.0f && move2 == true)
	{

		movetocar2z -= (float)(dt * -10.f);
		NPC->setCoords(movetocar2x, movetocar2z);
		timer = 1;

	}
	//Car 3: (15, 15)
	if (Speak2 == true && Speak3 == false && movetocar3x != 15.0f && movetocar3x < 15.0f && move3 == true)
	{

		movetocar3x -= (float)(dt * -10.f);
		NPC->setCoords(movetocar3x, movetocar3z);
		timer = 1;

	}
	//Car 4: (15, -15)
	if (Speak3 == true && Speak4 == false && movetocar4z != 15.0f && movetocar4z > -15.0f && move4 == true)
	{
		movetocar4z -= (float)(dt * 10.f);
		NPC->setCoords(movetocar4x, movetocar4z);
		timer = 1;
	}

	if (timer < 0 && SpeakIntro == true)
	{
		move1 = true;
		NPC[0].Interact(0);
	}

	if (timer < 0 && Speak1 == true)
	{
		move2 = true;
		NPC[0].Interact(0);
	}

	if (timer < 0 && Speak2 == true)
	{
		move3 = true;
		NPC[0].Interact(0);
	}

	if (timer < 0 && Speak3 == true)
	{
		move4 = true;
		NPC[0].Interact(0);
	}

	if (SpeakIntro == false || timer <= 0)
	{
		interactable = true;
	}
	else
	{
		interactable = false;
	}
	if (Application::IsKeyPressed('E') && NearNPC() && bounceTime <= 0 && SpeakIntro == false && interactable == true)
	{
		RenderTextBox();
		timer = 3;
		bounceTime = 0.5f;
		NPCtext = NPCDialogue();
		NPC[0].Interact(1);
		SpeakIntro = true;
	}

	if (Application::IsKeyPressed('E') && NearNPC() && bounceTime <= 0 && SpeakIntro == true && Speak1 == false && interactable == true)
	{
		movetocar2z = movetocar1;
		movetocar2x = movetocar1;
		timer = 3;
		bounceTime = 0.5f;
		NPCtext = NPCDialogue();
		NPC[0].Interact(1);
		Speak1 = true;
	}

	if (Application::IsKeyPressed('E') && NearNPC() && bounceTime <= 0 && Speak1 == true && Speak2 == false && interactable == true)
	{
		movetocar3z = movetocar2z;
		movetocar3x = movetocar2x;
		timer = 3;
		bounceTime = 0.5f;
		NPCtext = NPCDialogue();
		NPC[0].Interact(1);
		Speak2 = true;
	}

	if (Application::IsKeyPressed('E') && NearNPC() && bounceTime <= 0 && Speak2 == true && Speak3 == false && interactable == true)
	{
		movetocar4z = movetocar3z;
		movetocar4x = movetocar3x;
		timer = 3;
		bounceTime = 0.5f;
		NPCtext = NPCDialogue();
		NPC[0].Interact(1);
		Speak3 = true;
	}

	if (Application::IsKeyPressed('E') && NearNPC() && bounceTime <= 0 && Speak3 == true && Speak4 == false && interactable == true)
	{
		timer = 3;
		bounceTime = 0.5f;
		NPCtext = NPCDialogue();
		NPC[0].Interact(1);
		Speak4 = true;
	}

}

void SceneSkybox::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	if (SwitchCamera==2)
		viewStack.LookAt(Frecamera.position.x, Frecamera.position.y, Frecamera.position.z, Frecamera.target.x, Frecamera.target.y, Frecamera.target.z, Frecamera.up.x, Frecamera.up.y, Frecamera.up.z);
	else if(SwitchCamera==1)
		viewStack.LookAt(FPScamera.position.x, FPScamera.position.y, FPScamera.position.z, FPScamera.target.x, FPScamera.target.y, FPScamera.target.z, FPScamera.up.x, FPScamera.up.y, FPScamera.up.z);
	modelStack.LoadIdentity();

	// Since all lights types are spot light and it won't change, use the code for spot light for all lights
	// lights 0-3 are spot lights
	// light 4 is directional light

	// Spot lights
	Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
	glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	
	lightPosition_cameraspace = viewStack.Top() * light[1].position;
	glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	spotDirection_cameraspace = viewStack.Top() * light[1].spotDirection;
	glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);

	lightPosition_cameraspace = viewStack.Top() * light[2].position;
	glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
	spotDirection_cameraspace = viewStack.Top() * light[2].spotDirection;
	glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);

	lightPosition_cameraspace = viewStack.Top() * light[3].position;
	glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightPosition_cameraspace.x);
	spotDirection_cameraspace = viewStack.Top() * light[3].spotDirection;
	glUniform3fv(m_parameters[U_LIGHT3_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);

	// Directional light
	Vector3 lightDir(light[4].position.x, light[4].position.y, light[4].position.z);
	Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
	glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, &lightDirection_cameraspace.x);

	static float framesPerSecond = -0.0f;
	static int fps;
	static float lastTime = 0.0f;
	float currentTime = GetTickCount() * 0.001f;

	int x_pos = FPScamera.position.x;
	int z_pos = FPScamera.position.z;

	++framesPerSecond;
	if (currentTime - lastTime > 1.0f)
	{
		lastTime = currentTime;
		fps = (int)framesPerSecond;
		framesPerSecond = 0;
	}
	std::string num = std::to_string(fps);

	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, -1.f, 0.f);
	modelStack.Scale(200.f, 200.f, 200.f);
	modelStack.Rotate(-90.f, 1.f, 0.f, 0.f);
	modelStack.PushMatrix();
	modelStack.Rotate(90.f, 0.f, 0.f, 1.f);
	RenderMesh(meshList[GEO_FLOOR], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	RenderSkybox();
	RenderRoom();
	RenderEnviroment();

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTSPHERE1], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(light[1].position.x, light[1].position.y, light[1].position.z);
	RenderMesh(meshList[GEO_LIGHTSPHERE1], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(light[2].position.x, light[2].position.y, light[2].position.z);
	RenderMesh(meshList[GEO_LIGHTSPHERE1], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(light[3].position.x, light[3].position.y, light[3].position.z);
	RenderMesh(meshList[GEO_LIGHTSPHERE1], false);
	modelStack.PopMatrix();
	RenderTextOnScreen(meshList[GEO_TEXT], "Current Frames Per Second: \n\n", Color(0, 0, 0), 2, 0, 1);
	RenderTextOnScreen(meshList[GEO_TEXT], num, Color(0, 0, 0), 2, 0, 0);
	RenderTextOnScreen(meshList[GEO_TEXT], "Press L to go to selection menu.", Color(0, 0, 0), 2, 0, 2);
	RenderTextOnScreen(meshList[GEO_TEXT], "+", Color(0, 1, 0), 4, 10, 7.5);

	//for NPC

	if ((SpeakIntro == false))
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 0);
		RenderMesh(meshList[GEO_NPCB], true);
		modelStack.PopMatrix();
	}
	else if ((SpeakIntro == true) && (Speak1 == false))
	{
		modelStack.PushMatrix();
		modelStack.Translate(movetocar1, 0, movetocar1);
		RenderMesh(meshList[GEO_NPCB], true);
		modelStack.PopMatrix();
	}
	else if ((Speak1 == true) && (Speak2 == false))
	{

		modelStack.PushMatrix();
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(movetocar2x, 0, movetocar2z);
		RenderMesh(meshList[GEO_NPCB], true);
		modelStack.PopMatrix();
	}
	else if ((Speak2 == true) && (Speak3 == false))
	{
		modelStack.PushMatrix();
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(movetocar3x, 0, movetocar3z);
		RenderMesh(meshList[GEO_NPCB], true);
		modelStack.PopMatrix();
	}
	else if ((Speak3 == true) && (Speak4 == false))
	{
		modelStack.PushMatrix();
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(movetocar4x, 0, movetocar4z);
		RenderMesh(meshList[GEO_NPCB], true);
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		modelStack.Translate(movetocar4x, 0, movetocar4z);
		RenderMesh(meshList[GEO_NPCB], true);
		modelStack.PopMatrix();
	}

	if (NearNPC() && interactable == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Press 'E' to Interact", Color(0, 0, 0), 3.5, 1.3, 1.5);
	}

	if (NPC[0].IsInteracting())
	{
		RenderTextBox();
		//RenderNPCText();
	}
}

void SceneSkybox::Exit()
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
	Wall::deleteWalls();
}

void SceneSkybox::RenderMesh(Mesh* mesh, bool enableLight)
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

	if(mesh->textureID > 0){ 
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);} 
	else { 
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	} 
	mesh->Render(); //this line should only be called once in the whole function

	if(mesh->textureID > 0) glBindTexture(GL_TEXTURE_2D, 0);
}

void SceneSkybox::RenderSkybox()
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

void SceneSkybox::RenderPlayer()
{
	modelStack.PushMatrix();
	modelStack.Translate(FPScamera.position.x, FPScamera.position.y, FPScamera.position.z);
	RenderMesh(meshList[GEO_PLAYER], false);
	modelStack.PopMatrix();
}

void SceneSkybox::RenderRoom()
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

	modelStack.PushMatrix();
	modelStack.Translate(0.f, 30.f, -5.f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_ROOF], true);
	modelStack.PopMatrix();
}

void SceneSkybox::RenderEnviroment()//Put Enviromentobject here ETC Cars tand,station car,booth ,plants, well anything static
{
	modelStack.PushMatrix();
	modelStack.Translate(10.f, -0.5f, 20.f);
	modelStack.Rotate(rotate, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_STAND], true);
	modelStack.PushMatrix();
	modelStack.Translate(0.f, translateY1, 0.f);
	modelStack.Scale(0.5f, 0.5f, 0.5f);
	modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
	RenderMesh(CCar::AllCar[0]->GetCarMesh(), true);
	modelStack.PushMatrix();
	modelStack.Translate(0.5f, -0.3f, -0.1f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	RenderMesh(CCar::AllCar[0]->GetWheelMesh(), true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(6.5f, -0.3f, -0.1f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	RenderMesh(CCar::AllCar[0]->GetWheelMesh(), true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.5f, -0.3f, -6.1f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	RenderMesh(CCar::AllCar[0]->GetWheelMesh(), true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(6.5f, -0.3f, -6.1f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	RenderMesh(CCar::AllCar[0]->GetWheelMesh(), true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(10.f, -0.5f, -20.f);
	modelStack.Rotate(-rotate, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_STAND], true);
	modelStack.PushMatrix();
	modelStack.Translate(0.f, translateY2, 0.f);
	modelStack.Scale(0.5f, 0.5f, 0.5f);
	modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
	RenderMesh(CCar::AllCar[1]->GetCarMesh(), true);
	modelStack.PushMatrix();
	modelStack.Translate(0.1f, 0.6f, 0.f);
	modelStack.Scale(2.f, 2.f, 2.f);
	RenderMesh(CCar::AllCar[1]->GetWheelMesh(), true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.1f, 0.6f, -5.f);
	modelStack.Scale(2.f, 2.f, 2.f);
	RenderMesh(CCar::AllCar[1]->GetWheelMesh(), true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-5.95f, 0.6f, 0.f);
	modelStack.Scale(2.f, 2.f, 2.f);
	RenderMesh(CCar::AllCar[1]->GetWheelMesh(), true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-5.95f, 0.6f, -5.f);
	modelStack.Scale(2.f, 2.f, 2.f);
	RenderMesh(CCar::AllCar[1]->GetWheelMesh(), true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-20.f, -0.5f, 20.f);
	modelStack.Rotate(rotate, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_STAND], true);
	modelStack.PushMatrix();
	modelStack.Translate(0.f, translateY3, 0.f);
	modelStack.Scale(0.5f, 0.5f, 0.5f);
	RenderMesh(CCar::AllCar[2]->GetCarMesh(), true);
	modelStack.PushMatrix();
	modelStack.Scale(1.1f, 1.1f, 1.1f);
	modelStack.Translate(-4.25f, 0.6f, 2.35f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	RenderMesh(CCar::AllCar[2]->GetWheelMesh(), true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Scale(1.1f, 1.1f, 1.1f);
	modelStack.Translate(-4.25f, 0.6f, -2.35f);
	modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
	RenderMesh(CCar::AllCar[2]->GetWheelMesh(), true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Scale(1.1f, 1.1f, 1.1f);
	modelStack.Translate(3.7f, 0.6f, 2.35f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	RenderMesh(CCar::AllCar[2]->GetWheelMesh(), true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Scale(1.1f, 1.1f, 1.1f);
	modelStack.Translate(3.7f, 0.6f, -2.35f);
	modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
	RenderMesh(CCar::AllCar[2]->GetWheelMesh(), true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-20.f, -0.3f, -20.f);
	modelStack.Rotate(-rotate, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_STAND], true);
	modelStack.PushMatrix();
	modelStack.Scale(0.45f, 0.45f, 0.45f);
	modelStack.Translate(0.f, translateY4, 0.f);
	RenderMesh(CCar::AllCar[3]->GetCarMesh(), true);
	modelStack.PushMatrix();
	modelStack.Scale(2.9f, 2.9f, 2.9f);
	modelStack.Translate(0.05f, -0.3f, 0.1f);
	RenderMesh(CCar::AllCar[3]->GetWheelMesh(), true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Scale(2.9f, 2.9f, 2.9f);
	modelStack.Translate(0.05f, -0.3f, -1.6f);
	RenderMesh(CCar::AllCar[3]->GetWheelMesh(), true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Scale(2.9f, 2.9f, 2.9f);
	modelStack.Translate(1.95f, -0.3f, 0.1f);
	RenderMesh(CCar::AllCar[3]->GetWheelMesh(), true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Scale(2.9f, 2.9f, 2.9f);
	modelStack.Translate(1.95f, -0.3f, -1.6f);
	RenderMesh(CCar::AllCar[3]->GetWheelMesh(), true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void SceneSkybox::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneSkybox::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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


std::string SceneSkybox::NPCDialogue() // to edit
{
	bounceTime = 1.0f;

	if (SpeakIntro == false)
	{
		return "Welcome!";
	}

	if ((SpeakIntro == true) && (Speak1 == false))
	{
		return "This is Car #1";
	}

	if ((SpeakIntro == true) && (Speak1 == true) && (Speak2 == false))
	{
		return "This is Car #2";
	}

	if ((SpeakIntro == true) && (Speak1 == true) && (Speak2 == true) && (Speak3 == false))
	{
		return "This is Car #3";
	}

	if ((SpeakIntro == true) && (Speak1 == true) && (Speak2 == true) && (Speak3 == true) && (Speak4 == false))
	{
		return "This is Car #4";
	}

}

void SceneSkybox::InteractionUpdate(int i)
{
	if (Application::IsKeyPressed('E') && bounceTime <= 0)
	{
		NPC[0].Interact(0);
		bounceTime = 0.2f;
	}
}

bool SceneSkybox::NearNPC() //To check if player is close to NPC
{
	if (FPScamera.position.x >= NPC[0].getX() - 8.f && FPScamera.position.x <= NPC[0].getX() + 8.f)
	{
		if (FPScamera.position.z >= NPC[0].getZ() - 8.f && FPScamera.position.z <= NPC[0].getZ() + 8.f)
		{
			return true;

		}
	}
	return false;
}

void SceneSkybox::RenderTextBox()
{
	if (SpeakIntro == true)
	{
		modelStack.PushMatrix();
		modelStack.Translate(NPC[0].getX() - 1, 1, NPC[0].getZ());
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(1, 0.5, 1.1);
		RenderMesh(meshList[GEO_TEXTBOX], true);
		modelStack.Translate(-1.2, 0, 0);
		modelStack.Scale(0.2, 0.4, 0.2);
		RenderText(meshList[GEO_TEXT], NPCtext, Color(1, 1, 1));
		modelStack.PopMatrix();
	}

	if (Speak1 == true)
	{
		modelStack.PushMatrix();
		modelStack.Translate(NPC[0].getX() - 1, 1, NPC[0].getZ());
		modelStack.Rotate(120, 0, 1, 0);			modelStack.Scale(1, 0.5, 1.1);
		RenderMesh(meshList[GEO_TEXTBOX], true);
		modelStack.Translate(-1.2, 0, 0);
		modelStack.Scale(0.2, 0.3, 0.2);
		RenderText(meshList[GEO_TEXT], NPCtext, Color(1, 1, 1));
		modelStack.PopMatrix();
	}
}

void SceneSkybox::RenderNPCText()
{
	modelStack.PushMatrix();
	modelStack.Translate(NPC[0].getX() - 1, 1, NPC[0].getZ() - 1.3);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(0.2, 0.2, 0.2);
	RenderText(meshList[GEO_TEXT], NPCtext, Color(1, 1, 1));
	modelStack.PopMatrix();
}

bool SceneSkybox::DuringConvo()
{
	if (timer > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
