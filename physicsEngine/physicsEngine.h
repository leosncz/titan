#pragma once
#include "qu3e/q3.h"
#include <iostream>
using namespace std;
class physicsEngine
{
public:
	physicsEngine()
	{
		m_id = rand();
		cout << "--> Physics engine ID=" << m_id << " created !" << endl;
		m_scene = new q3Scene(r32(1.0f / 60.0f));
	}
	void simulate()
	{
		m_scene->Step();
	}
	~physicsEngine()
	{
		cout << "--> Deleting physics engine ID=" << m_id << endl;
		delete m_scene;
	}
private:
	int m_id;
	q3Scene *m_scene;
};