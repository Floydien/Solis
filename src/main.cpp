#include <vector>
#include "game/game.h"


int main() {
	#ifndef USE_VULKAN
	{ 
		Game g;
		g.init();
		g.run();
	}
	#else
	SolisDevice device(VideoDriverType::eVulkan, 1280, 720);
	#endif
}
