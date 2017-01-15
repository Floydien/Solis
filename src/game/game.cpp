#include "game.h"
#include "node.h"
#include "vertex.h"
#include <ctime>
#include <unistd.h>
#include "debug/logger.h"
#include "player.h"

Game::Game() {

}

Game::~Game() {
	//delete device;
}

void Game::init() {
	//In Game::init()
	//World world;
	//world.generate();
	//world.build(BuildingType::eSawmill, position);
	//auto building = world.at(position);
	//Do stuff

	//In World
	//World() { init stuff }
	//world.build(BuildingType type, position) {
	//	//see if enough resources
	//	//see if all the required blocks are empty
	//	//check if underneath is ground
	//	//set the blocks to the building
	//}

	device = new SolisDevice(VideoDriverType::eOpenGL, 1280, 720);
	driver = device->getVideoDriver();
	scene = device->getScene();

	field = new Field();
	

	scene->addToScene((new Node(Transform(glm::vec3(-0.5,0,-0.5)), "Field"))
		->addComponent(field));

	/* add a player to the scene */
	scene->addToScene((new Node(Transform(glm::vec3(0,0,0), glm::quat(), glm::vec3(0.5,0.5,0.5)), "Player"))
		->addComponent(new Player(field)));

	driver->addShaderFromFile("gui");
	driver->addShaderFromFile("texture");

	scene->addCamera();
	scene->getActiveCamera()->setRotationSpeed(8);
	scene->init();

	field->setBlock(BlockType::eTree, 3, 3);
	field->setBlock(BlockType::eTree, 3, 5);
}

#include <chrono>
#include <thread>
#include "gui/guibutton.h"

void Game::run() {

	float fps = 60;
	float time_per_frame = 1.0f/fps;
	float time_counter = 0;
	int frame_counter = 0;

	auto start = std::chrono::steady_clock::now();
	auto end = start;
	std::chrono::duration<float> diff;
	

	GUIButton button("random image", Rectangle({300, 250}, {500, 400}));
	button.init(scene);
	button.setCallback([this](){ field->build(BuildingType::eSawmill,5,5); });

	while(device->run()) {
		start = std::chrono::steady_clock::now();
		diff = std::chrono::duration_cast<std::chrono::duration<float>>(start - end);

		frame_counter++;
		if((time_counter += diff.count()) >= 1.0) {
			printf("Frames per second: %d\n", frame_counter);
			time_counter -= 1.0;
			frame_counter = 0;
		}

		driver->clearScreenBuffer();
		scene->input(diff.count() * 10);
		scene->updateScene(diff.count() * 10);

		driver->bindShader("texture");
		scene->renderScene();
		
		driver->bindShader("gui");
		button.update(device);
		button.draw(driver);



		if(diff.count() < time_per_frame) {
			std::this_thread::sleep_for(std::chrono::microseconds((int)((time_per_frame - diff.count()) * 1000000)));
		}
		end = start;
	}
}