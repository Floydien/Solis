#include <vector>
#include "game/game.h"

int main() {
	printf("Hey\n");
	{ 
		Game g;
		g.init();
		g.run();
	}
}
