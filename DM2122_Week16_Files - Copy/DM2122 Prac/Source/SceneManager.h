#pragma once
#include "SceneSkybox.h"
#include "SceneCarSelection.h"
#include "Minigame.h"
#include "SceneInterior.h"

class SceneManager
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_CAR1=0,
		GEO_CAR1WHEELS,
		Geo_CAR1INT,
		GEO_CAR2,
		GEO_CAR2WHEELS,
		Geo_CAR2INT,
		GEO_CAR3,
		GEO_CAR3WHEELS,
		Geo_CAR3INT,
		GEO_CAR4,
		GEO_CAR4WHEELS,
		Geo_CAR4INT,
		NUM_GEOMETRY,
	};
	enum SCENES_TYPES
	{
		SCENE_SKYBOX = 0,
		SCENE_CAR_SELECTION,
		SCENE_MINIGAME,
		SCENE_INTERIOR,
		NUM_SCENES,
	};
private:
	SCENES_TYPES currentSceneID;
	Scene* scene_list[NUM_SCENES];
	static SceneManager* instance;
	SceneManager();
	SceneManager(SCENES_TYPES type);
public:
	static SceneManager* getInstance();
	Mesh* meshList[NUM_GEOMETRY];
	void SetNextScene(SCENES_TYPES currentScene);
	void Update(double dt);
	void DeleteAllScenes();
	void InitCar();
};
