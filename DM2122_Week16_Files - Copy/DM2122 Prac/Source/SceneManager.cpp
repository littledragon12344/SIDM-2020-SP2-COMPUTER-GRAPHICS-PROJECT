#include "SceneManager.h"

SceneManager* SceneManager::instance = 0;

SceneManager::SceneManager() : currentSceneID(SCENE_SKYBOX)
{
	
}

SceneManager* SceneManager::getInstance()
{
	if (!instance)
	{
		instance = new SceneManager();
		instance->scene_list[SCENE_SKYBOX] = new SceneSkybox();
		instance->scene_list[SCENE_CAR_SELECTION] = new SceneCarSelection();
		instance->scene_list[SCENE_MINIGAME] = new Minigame();
		instance->scene_list[SCENE_INTERIOR] = new SceneInterior();

		instance->scene_list[SCENE_SKYBOX]->Init();

		return instance;
	}
	else
	{
		return instance;
	}
}

void SceneManager::SetNextScene(SCENES_TYPES sceneID)
{
	if (sceneID != currentSceneID)
	{
		scene_list[currentSceneID]->Exit();
		currentSceneID = sceneID;
		scene_list[currentSceneID]->Init();
	}
}

void SceneManager::Update(double dt)
{
	scene_list[currentSceneID]->Update(dt);
	scene_list[currentSceneID]->Render();
}

void SceneManager::DeleteAllScenes()
{
	scene_list[currentSceneID]->Exit();
	for (int i = 0; i < NUM_SCENES; ++i)
		delete scene_list[i];
}
