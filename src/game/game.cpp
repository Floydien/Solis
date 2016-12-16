#include "game.h"
#include "node.h"
#include "rendercomponent.h"
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
			position = {-1.0*i, 0, k};

			uvCoord = {(float) (i % size) , (float) (k % size)};
			normal = {0, 1, 0};
			vertices.push_back(Vertex(position, uvCoord, normal));
		}
	}

	for(size_t i = 0; i < size * size; i++) {
		indices.push_back((i + i / size) + (size + 1));
		indices.push_back((i + i / size) + 1);
		indices.push_back((i + i / size));

		indices.push_back((i + i / size) + (size + 1));
		indices.push_back((i + i / size) + (size + 2));
		indices.push_back((i + i / size) + 1);
	}

	return new VertexBuffer(vertices, indices);
}

Game::Game() {

}

Game::~Game() {
	if(field) {
		delete field;
	}


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

	field = new Field();
	field->blocks.at(1).at(1) = Block(std::make_shared<BlockData>(Tree()));
	field->blocks.at(5).at(5) = field->blocks.at(1).at(1);


	device = new SolisDevice(VideoDriverType::eOpenGL, 1280, 720);
	driver = device->getVideoDriver();
	scene = device->getScene();

	for(int32_t y = 0; y < field->blocks.size(); y++) {
		for(int32_t x = 0; x < field->blocks.at(0).size(); x++) {
			if(field->blocks.at(y).at(x).getType() == BlockType::eTree) {
				scene->addToScene((new Node(Transform(glm::vec3(x,0,y))))
					->addComponent(new RenderComponent(
						scene->getMesh("tree.obj"),
						new Material(driver->getTexture("solis.png")))));
			}
		}	
	}

	scene->addToScene((new Node(Transform(glm::vec3(-0.5,0,-0.5))))
		->addComponent(new RenderComponent(
			scene->getMesh(generateField(32)),
			new Material(driver->getTexture("solis.png")))));

	Player *player = new Player(field);
	scene->addToScene((new Node(Transform(glm::vec3(0,0,0), glm::quat(0,0,0,1), glm::vec3(0.5,0.5,0.5))))
		->addComponent(player)
		->addComponent(new RenderComponent(
			scene->getMesh("Person.obj"),
			new Material(driver->getTexture("solis.png")))));
	player->init();

	driver->addShaderFromFile("texture");

	// scene->addToScene((new Node(Transform(glm::vec3(2,0,0))))
	// 	->addComponent(new RenderComponent(
	// 		scene->getMesh(createBuilding({1.0, 0.5, 1.0})),
	// 		new Material(driver->getTexture("solis.png")))));

	scene->addCamera();
	scene->getActiveCamera()->setRotationSpeed(8);
}

#include <chrono>
#include <thread>

void Game::run() {
	printf("hi\n");
	// Logger::start();
	// Logger::write(LogType::eWarning, "hi", true);
	// Logger::write(LogType::eWarning, "hi", false);
	// Logger::write(LogType::eWarning, "hi", true);
	// Logger::write(LogType::eWarning, "hi", true);
	// Logger::end();

	float fps = 60;
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