
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "Mesh.h"
#include "CCar.h"

class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	void ChangeScene(Mesh* Playercar,CCar* Carstats,int scenenum);
	static bool IsKeyPressed(unsigned short key);

private:
	Mesh* Player;
	CCar* Carstats;
	//Declare a window object
	StopWatch m_timer;
};

#endif