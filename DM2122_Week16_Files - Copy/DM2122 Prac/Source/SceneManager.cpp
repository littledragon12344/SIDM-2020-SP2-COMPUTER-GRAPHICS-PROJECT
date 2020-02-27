#include "SceneManager.h"
#include "Application.h"
SceneManager* SceneManager::instance = 0;

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

Scene* SceneManager::getCurrentScene()
{
    return currentScene;
}

void SceneManager::SceneInitialise()
{
    scene_list[SCENE_SKYBOX] = new SceneSkybox();
    scene_list[SCENE_CAR_SELECTION] = new SceneCarSelection();
    scene_list[SCENE_MINIGAME] = new Minigame();
    scene_list[SCENE_INTERIOR] = new SceneInterior();
}

void SceneManager::SceneUpdate(double dt)
{
    currentScene->Update(dt);
    currentScene->Render();

}

void SceneManager::DeleteAllScenes()
{

}