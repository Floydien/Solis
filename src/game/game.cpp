#include "game.h"
#include "node.h"
#include "vertex.h"
#include <ctime>
#include <unistd.h>
#include "debug/logger.h"
#include "player.h"

VertexBuffer *generateField(uint32_t size) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	glm::vec3 position;
	glm::vec2 uvCoord;
	glm::vec3 normal;

	for(size_t i = 0; i < size + 1; i++) {
		for(size_t k = 0; k < size + 1; k++) {
			position = {i, 0, k};

			uvCoord = {(float) (i % size) , (float) (k % size)};
			normal = {0, 1, 0};
			vertices.push_back(Vertex(position, uvCoord, normal));
		}
	}

	for(size_t i = 0; i < size * size; i++) {
		indices.push_back((i + i / size));
		indices.push_back((i + i / size) + 1);
		indices.push_back((i + i / size) + (size + 1));

		indices.push_back((i + i / size) + 1);
		indices.push_back((i + i / size) + (size + 2));
		indices.push_back((i + i / size) + (size + 1));
	}

	return new VertexBuffer(vertices, indices);
}

Game::Game() {

}

Game::~Game() {
	if(device) {
		delete device;
	}
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
	scene->addToScene((new Node(Transform(), "Field"))
		->addComponent(field));

	field->setBlock(BlockType::eTree, 10, 10);

	scene->addToScene((new Node(Transform(glm::vec3(-0.5,0,-0.5)), "Generated Field"))
		->addComponent(new RenderComponent(
			scene->getMesh(generateField(32)),
			new Material(driver->getTexture("solis.png")))));

	Player *player = new Player(field);
	scene->addToScene((new Node(Transform(glm::vec3(0,0,0), glm::quat(), glm::vec3(0.5,0.5,0.5)), "Player"))
		->addComponent(player)
		->addComponent(new RenderComponent(
			scene->getMesh("Person.obj"),
			new Material(driver->getTexture("solis.png")))));
	player->init();

	driver->addShaderFromFile("texture");

	scene->addCamera();
	scene->getActiveCamera()->setRotationSpeed(8);
}

#include <chrono>
#include <thread>

void Game::run() {
	// Logger::start();
	// Logger::write(LogType::eWarning, "hi", true);
	// Logger::write(LogType::eWarning, "hi", false);
	// Logger::write(LogType::eWarning, "hi", true);
	// Logger::write(LogType::eWarning, "hi", true);
	// Logger::end();

	float fps = 120;
	float time_per_frame = 1.0f/fps;
	float counter = 0;
	int counter2 = 0;

	auto start = std::chrono::steady_clock::now();
	auto end = start;
	std::chrono::duration<float> diff;


	while(device->run()) {
		device->getInput()->isKeyDown(KeyCode::eKey0);
		start = std::chrono::steady_clock::now();
		diff = std::chrono::duration_cast<std::chrono::duration<float>>(start - end);
		counter2++;
		if((counter += diff.count()) >= 1.0) {
			printf("Frames per second: %d\n", counter2);
			counter -= 1.0;
			counter2 = 0;
		}

		driver->clearScreenBuffer();
		scene->input(diff.count() * 10);
		scene->updateScene(diff.count() * 10);

		scene->renderScene();


		if(diff.count() < time_per_frame) {
			std::this_thread::sleep_for(std::chrono::microseconds((int)((time_per_frame - diff.count()) * 1000000)));
		}
		end = start;
	}

}