#pragma once
#include "SceneSkybox.h"
#include "SceneCarSelection.h"
#include "Minigame.h"
#include "SceneInterior.h"

class SceneManager
{
public:
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
public:
	static SceneManager* getInstance();
	void SetNextScene(SCENES_TYPES sceneID);
	void Update(double dt);
	void DeleteAllScenes();
};
