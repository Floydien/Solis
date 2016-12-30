#pragma once
#include "solisdevice.h"
#include "videodriver.h"
#include "scene.h"
#include "chunk.h"
#include "field.h"

class Game {
public:
	Game();
	~Game();

	void run();
	void init();

private:
	Field *field;

	SolisDevice *device;
	VideoDriver *driver;
	Scene *scene;
};