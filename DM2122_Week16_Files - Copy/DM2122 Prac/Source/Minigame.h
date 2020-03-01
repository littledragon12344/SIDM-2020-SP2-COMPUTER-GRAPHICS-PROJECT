#ifndef MINIGAME_H
#define MINIGAME_H

#include "Path.h"
#include "Scene.h"
#include <MatrixStack.h>
#include "Camera2.h"
//#include "Camera3.h"
#include "TopDownCamera.h"
#include "Mesh.h"
#include "Light.h"
#include "CarAI.h"
#include "SceneManager.h"
#include "CarMinigame.h"
#include "FirstPersonCamera.h"
#include "Wall.h"


class Minigame : public Scene
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
		GEO_CARAI,
		GEO_LIGHTSPHERE,
		GEO_CARTARGET,//testing angles
		GEO_ROAD,
		GEO_SPEEDMETER,
		GEO_PLAYER,
		GEO_WALL,
		GEO_TEXT,
		GEO_POINTER,
		GEO_STARTLIGHT,
		GEO_PAUSE,
		GEO_BLACK,
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
	float LegAngle;
	float Yaw;
	bool Pause;
	float selection;

	MS modelStack, viewStack, projectionStack;
	Light light[2];

	Camera2 FPScamera;//Testing cameras
	Camera Frecamera;
	TopDownCamera TopCamera;//Main camera

	Path path1;
	CarAi Car1;
	CarMinigame Player;

	float lightcolor;
	bool switchcolor;
	int AmounttoPOP;
	float StartTime;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderSkybox();
	void RenderPlayer();
	void RenderEnviroment();
	void RenderUI();
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, Color color, float sizex, float sizey, float x, float y,int animFrames, float Rotation);

public:
	Minigame();
	~Minigame();

	virtual void Init();
	virtual void InitPlayerCar();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif