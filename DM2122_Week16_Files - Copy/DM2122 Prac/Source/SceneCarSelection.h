#ifndef SCENECARSELECTION_H
#define SCENECARSELECTION_H

#include "Scene.h"
#include <MatrixStack.h>
#include "Mesh.h"
#include "Camera.h"
#include "SceneManager.h"
#include "Light.h"
#include "CCar.h"

class SceneCarSelection : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_TEXT= 0,
		GEO_BACKGROUND,
		// User Interface
		GEO_UI_BASE,
		GEO_UI_GRAY_BAR,
		GEO_UI_BLUE_BAR,
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
		//add these enum in UNIFORM_TYPE before U_TOTAL
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHTENABLED,
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
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_parameters[U_TOTAL];

	MS modelStack, viewStack, projectionStack;
	Light light[1];

	Camera camera;

	float carAngle; // Car rotation angle
	float animAngle; // Animation angle when switching car
	int backgroundNo;
	bool bPressesd;

	// use reference so no need copy the data over
	std::vector<CCar*>& allCars = CCar::AllCar;
	int currentCarIndex;

	float currentCarMaxSpd, currentCarAccel;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey);

	void RenderCar(int carNo);

	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void CalculateFrameRate();

public:
	SceneCarSelection();
	~SceneCarSelection();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif
