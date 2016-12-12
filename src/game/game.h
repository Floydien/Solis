#pragma once
#include "solisdevice.h"
#include "videodriver.h"
#include "scene.h"
#include "chunk.h"

// struct Field {
// 	std::array< std::array<Block,32>, 32> blocks;
// };

class Game {
public:
	Game();
	~Game();

	void run();
	void init();

private:
	SolisDevice *device;
	VideoDriver *driver;
	Scene *scene;
};