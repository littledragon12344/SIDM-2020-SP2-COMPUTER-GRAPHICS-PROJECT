#pragma once
#include <vector>
#include "SceneSkybox.h"
#include "SceneCarSelection.h"
#include "Minigame.h"
#include "SceneInterior.h"

class SceneManager
{
private:
    enum SCENES_TYPES
    {
        SCENE_SKYBOX,
        SCENE_CAR_SELECTION,
        SCENE_MINIGAME,
        SCENE_INTERIOR,
        NUM_SCENES,
    };
    static SceneManager* instance;
    static Scene* scene_list[NUM_SCENES];
    static Scene* currentScene;
    SceneManager();
public:
    static SceneManager* getInstance();
    static Scene* getCurrentScene();
    static void SceneInitialise();
    static void SceneUpdate(double dt);
    static void DeleteAllScenes();
};