#include "SceneManager.h"

SceneManager* SceneManager::instance = 0;

SceneManager::SceneManager() : currSceneID(0)
{
	
}

SceneManager* SceneManager::getInstance()
{
	if (!instance)
	{
		instance = new SceneManager();
		return instance;
	}
	else
	{
		return instance;
	}
}

void SceneManager::AddScene(Scene* scene)
{
	scene->Init();
	scenes.push_back(scene);
}
void SceneManager::SetNextScene(int sceneID)
{
	if (sceneID != currSceneID)
	{
		scenes[currSceneID]->Exit();
		currSceneID = sceneID;
		scenes[currSceneID]->Init();
	}
}

void SceneManager::Update(double dt)
{
	scenes[currSceneID]->Update(dt);
	scenes[currSceneID]->Render();
}

void SceneManager::DeleteAllScenes()
{
	scenes[currSceneID]->Exit();
	for (int i = 0; i < scenes.size(); ++i)
		delete scenes[i];
	
	scenes.clear();
}
