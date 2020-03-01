#ifndef SCENESKYBOXNPC_H
#define SCENESKYBOXNPC_H

#include "Scene.h"
#include "Wall.h"
#include <MatrixStack.h>
#include "Camera2.h"
#include "Camera3.h"
#include "Mesh.h"
#include "Light.h"
#include "NPC.h"
#include <iostream>
#include <string>

using namespace std;

class SceneSkyboxNPC : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_LEFT, 
		GEO_RIGHT, 
		GEO_TOP, 
		GEO_BOTTOM, 
		GEO_FRONT, 
		GEO_BACK,
		GEO_CHAR,
		GEO_FLOOR,
		GEO_PLAYER,
		GEO_CAR1,
		GEO_CAR1WHEELS,
		GEO_CAR2,
		GEO_CAR2WHEELS,
		GEO_CAR3,
		GEO_CAR3WHEELS,
		GEO_CAR4,
		GEO_CAR4WHEELS,
		GEO_WALL,
		GEO_ROOF,
		GEO_STAND,
		GEO_LIGHTSPHERE1,
		GEO_LIGHTSPHERE2,
		GEO_TEXT,
		GEO_NPCB,
		GEO_NPCW,
		GEO_TEXTBOX,
		NUM_GEOMETRY,
	};

	enum Scene5_UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		//add these enum in UNIFORM_TYPE before U_TOTAL
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		//add these enum in UNIFORM_TYPE before U_TOTAL
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_NUMLIGHTS,
		// add these enum for texture
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

private:
	unsigned m_vertexArrayID;
	unsigned m_programID;
	/*unsigned m_indexBuffer;*/
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_parameters[U_TOTAL];

	int SwitchCamera;
	float Yaw;
	float rotate;
	float translateY1;
	float translateY2;
	float translateY3;
	float translateY4;
	float movetocar1;
	float movetocar2x;
	float movetocar2z;
	float movetocar3x;
	float movetocar3z;
	float movetocar4x;
	float movetocar4z;
	float Cameraspeed;

	MS modelStack, viewStack, projectionStack;
	Light light[2];

	Camera2 FPScamera;
	Camera Frecamera;

	float lightcolor;
	bool switchcolor;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderSkybox();
	void RenderRoom();
	void RenderPlayer();
	void RenderEnviroment();
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderTextBox();
	void RenderNPCText();
	

	NPC NPC[1];
	string NPCtext;
	string NPCDialogue();

	void InteractionUpdate(int i);
	

	bool NearNPC();
	bool DuringConvo();

	bool SpeakIntro;
	bool Speak1;
	bool Speak2;
	bool Speak3;
	bool Speak4;
	bool nearPlatform;
	bool duringconvo;
	bool move1;
	bool move2;
	bool move3;
	bool move4;
	bool interactable;

	double bounceTime;
	float timer;

public:
	SceneSkyboxNPC();
	~SceneSkyboxNPC();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif