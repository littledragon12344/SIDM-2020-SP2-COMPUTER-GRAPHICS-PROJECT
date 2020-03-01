#include "SceneManager.h"

SceneManager* SceneManager::instance = 0;

SceneManager::SceneManager(SCENES_TYPES currentScene) : currentSceneID(currentScene)
{
	
}

SceneManager* SceneManager::getInstance()
{
	if (!instance)
	{
		instance = new SceneManager(SCENE_SKYBOX);
		instance->scene_list[SCENE_SKYBOX] = new SceneSkybox();
		instance->scene_list[SCENE_CAR_SELECTION] = new SceneCarSelection();
		instance->scene_list[SCENE_MINIGAME] = new Minigame();
		instance->scene_list[SCENE_INTERIOR] = new SceneInterior();

		instance->InitCar();

		instance->scene_list[instance->currentSceneID]->Init();

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
	delete instance;
	CCar::DeleteAllCar();
}
#include "GL\glew.h"
#include "LoadTGA.h"
#include "MeshBuilder.h"
void SceneManager::InitCar()
{
	meshList[GEO_CAR1] = MeshBuilder::GenerateOBJ("Car1", "OBJ//car_frame.obj");
	meshList[GEO_CAR1]->textureID = LoadTGA("Image//CarBody_texture.tga");
	meshList[GEO_CAR1WHEELS] = MeshBuilder::GenerateOBJ("Car1Wheels", "OBJ//car1_wheels.obj");
	meshList[GEO_CAR1WHEELS]->textureID = LoadTGA("Image//carwheels_1.tga");
	meshList[Geo_CAR1INT] = MeshBuilder::GenerateOBJ("Car2Interior", "OBJ//car2Interior.obj");
	meshList[Geo_CAR1INT]->textureID = LoadTGA("Image//car2Interior.tga");
	
	meshList[GEO_CAR2] = MeshBuilder::GenerateOBJ("Car2", "OBJ//carframe2.obj");
	meshList[GEO_CAR2]->textureID = LoadTGA("Image//spcarframe2.tga");
	meshList[GEO_CAR2WHEELS] = MeshBuilder::GenerateOBJ("Car2Wheels", "OBJ//spcar2_wheels.obj");
	meshList[GEO_CAR2WHEELS]->textureID = LoadTGA("Image//spcar2_wheel.tga");
	meshList[Geo_CAR2INT] = MeshBuilder::GenerateOBJ("Car1Interior", "OBJ//car1Interior.obj");
	meshList[Geo_CAR2INT]->textureID = LoadTGA("Image//car1Interior.tga");

	meshList[GEO_CAR3] = MeshBuilder::GenerateOBJ("car3", "Obj//gray.obj");
	meshList[GEO_CAR3]->textureID = LoadTGA("Image//car_cyan.tga");\
	meshList[GEO_CAR3WHEELS] = MeshBuilder::GenerateOBJ("car3wheels", "Obj//car3_wheel.obj");
	meshList[GEO_CAR3WHEELS]->textureID = LoadTGA("Image//car_cyan.tga");
	meshList[Geo_CAR3INT] = MeshBuilder::GenerateOBJ("Car4Interior", "OBJ//gray.obj");
	meshList[Geo_CAR3INT]->textureID = LoadTGA("Image//car_cyan.tga");

	meshList[GEO_CAR4] = MeshBuilder::GenerateOBJ("car4", "Obj//Carzx.obj");
	meshList[GEO_CAR4]->textureID = LoadTGA("Image//Carzx.tga");
	meshList[GEO_CAR4WHEELS] = MeshBuilder::GenerateOBJ("car4wheels", "Obj//Car4_Wheels.obj");
	meshList[GEO_CAR4WHEELS]->textureID = LoadTGA("Image//car_wheels4.tga");
	meshList[Geo_CAR4INT] = MeshBuilder::GenerateOBJ("Car3Interior", "OBJ//carZX_Interior.obj");
	meshList[Geo_CAR4INT]->textureID = LoadTGA("Image//carZX_Interior.tga");

	CCar::CreateCar(meshList[GEO_CAR1], meshList[GEO_CAR1WHEELS], meshList[Geo_CAR1INT], 60.f, 6.f);
	CCar::CreateCar(meshList[GEO_CAR2], meshList[GEO_CAR2WHEELS], meshList[Geo_CAR2INT], 50.f, 8.f);
	CCar::CreateCar(meshList[GEO_CAR3], meshList[GEO_CAR3WHEELS], meshList[Geo_CAR3INT], 80.f, 5.f);
	CCar::CreateCar(meshList[GEO_CAR4], meshList[GEO_CAR4WHEELS], meshList[Geo_CAR4INT], 100.f, 4.f);
}
