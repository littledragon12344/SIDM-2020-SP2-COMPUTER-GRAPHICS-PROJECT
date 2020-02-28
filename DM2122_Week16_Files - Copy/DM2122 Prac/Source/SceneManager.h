
#pragma once
#include "SceneSkybox.h"
#include "SceneCarSelection.h"
#include "Minigame.h"
#include "SceneInterior.h"

class SceneManager
{
private:
	int currSceneID;
	std::vector<Scene*> scenes;
	static SceneManager* instance;	
	SceneManager();
public:
	static SceneManager* getInstance();
	void AddScene(Scene* scene);
	void SetNextScene(int sceneID);
	void Update(double dt);
	void DeleteAllScenes();
};
