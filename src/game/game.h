#pragma once
#include "solisdevice.h"
#include "videodriver.h"
#include "rendercomponent.h"
#include "field.h"

class Game {
public:
	Game();
	~Game();

	void run();
	void init();

private:
	Field *field;

	std::shared_ptr<SolisDevice> device;
	// SolisDevice *device;
	VideoDriver *driver;
	Scene *scene;
};