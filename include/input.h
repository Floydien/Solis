#pragma once
#include <array>

enum class KeyCode {
	eKey0 = 48,
	eKey1,
	eKey2,

	eSize
};

class SolisDevice;

class Input {
public:
	Input(SolisDevice *device) : device(device) {};

	/* Returns true, if the key is pressed */
	bool isKeyPressed(KeyCode);
	/* Returns true, if the key is pressed that frame */
	bool isKeyDown(KeyCode);
	/* Returns true, if the key is released that frame */
	bool isKeyUp(KeyCode);
private:
	SolisDevice *device;

	std::array<bool, static_cast<int>(KeyCode::eSize)> keyDown;
};