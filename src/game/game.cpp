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
	delete device;
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
#include <iostream>
#include "gui/guibutton.h"
#include "physics/raycast.h"
#include "physics/aabb.h"
#include <glm/gtx/rotate_vector.hpp>



void Game::run() {
	float fps = 120;
	float time_per_frame = 1.0f/fps;
	float time_counter = 0;
	int frame_counter = 0;

	auto start = std::chrono::steady_clock::now();
	auto end = start;
	std::chrono::duration<float> diff;
	
	AABB millBox(glm::vec3(0,0,2), glm::vec3(1, 0.5f, 0.5f));

	RayCast test1(glm::vec3(0.06f, 0, 0.1f), glm::normalize(glm::vec3(0.5f, 0, 0.865f)));
	RayCast test2(glm::vec3(-0.06f, 0, 0.1f), glm::normalize(glm::vec3(-0.5f, 0, 0.865f)));

	if(test1.intersects(millBox)) {
		std::cout << "hey1" << std::endl;
	}
	if(test2.intersects(millBox)) {
		std::cout << "hey2" << std::endl;
	}

	std::vector<Vertex> vertices = {
		Vertex(millBox.position + glm::vec3( millBox.halfExtent.x,  millBox.halfExtent.y,  millBox.halfExtent.z)),
		Vertex(millBox.position + glm::vec3( millBox.halfExtent.x,  millBox.halfExtent.y, -millBox.halfExtent.z)),
		Vertex(millBox.position + glm::vec3( millBox.halfExtent.x, -millBox.halfExtent.y,  millBox.halfExtent.z)),
		Vertex(millBox.position + glm::vec3( millBox.halfExtent.x, -millBox.halfExtent.y, -millBox.halfExtent.z)),
		Vertex(millBox.position + glm::vec3(-millBox.halfExtent.x,  millBox.halfExtent.y,  millBox.halfExtent.z)),
		Vertex(millBox.position + glm::vec3(-millBox.halfExtent.x,  millBox.halfExtent.y, -millBox.halfExtent.z)),
		Vertex(millBox.position + glm::vec3(-millBox.halfExtent.x, -millBox.halfExtent.y,  millBox.halfExtent.z)),
		Vertex(millBox.position + glm::vec3(-millBox.halfExtent.x, -millBox.halfExtent.y, -millBox.halfExtent.z)),
	};
	std::vector<uint32_t> indices = {
 		1, 3, 0,
 		7, 5, 4,
 		4, 1, 0,
 		5, 2, 1,
 		6, 3, 2,
 		0, 7, 4,
 		1, 2, 3,
 		7, 6, 5,
 		4, 5, 1,
 		5, 6, 2,
 		6, 7, 3,
 		0, 3, 7,
	};
	scene->addToScene((new Node())->addComponent(new RenderComponent(scene->getMesh("physicsBox", new VertexBuffer(vertices, indices)), new Material(new Texture("solis.png")))));


	GUIButton button("random image", Rectangle({300, 250}, {500, 400}));
	GUIButton center("random image2", Rectangle({1280/2 - 1, 720/2 - 1}, {2, 2}));
	center.init(scene);
	button.init(scene);
	button.setCallback([this, &millBox](){ 
		field->build(BuildingType::eSawmill,5,1	); 
		millBox.position = glm::vec3(5.5,0.5,0.5);
		millBox.halfExtent = glm::vec3(1, 0.5f, 0.5f);
	});


	while(device->run()) {
		start = std::chrono::steady_clock::now();
		diff = std::chrono::duration_cast<std::chrono::duration<float>>(start - end);

		frame_counter++;
		if((time_counter += diff.count()) >= 1.0) {
			std::cout << "Frames per second: " << frame_counter << std::endl;
			// printf("Frames per second: %d\n", frame_counter);
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
		center.draw(driver);

		if(glfwGetMouseButton(device->getWindow(), GLFW_MOUSE_BUTTON_1)) {
			double posX, posY;
			glfwGetCursorPos(device->getWindow(), &posX, &posY);
			posX = ((posX / device->getWidth()) - 0.5) * 2.0;
			posY = ((posY / device->getHeight()) - 0.5) * -2.0;

			auto proj = scene->getActiveCamera()->getProjection();
			glm::mat4 inverseProjection = glm::inverse(proj);			
			auto view = scene->getActiveCamera()->getView();
			glm::mat4 inverseView = glm::inverse(view);
			auto m = glm::inverse(proj * view);

			glm::vec4 rayStart(posX, posY, -1.0, 1.0);
			glm::vec4 rayEnd(posX, posY, 0.0, 1.0);


			rayStart = inverseProjection * rayStart;
			glm::vec4 rayPos = (rayStart / rayStart.w);
			rayPos = inverseView * rayPos;
			rayStart.z = -1.0;
			rayStart.w = 0.0;
			glm::vec3 raydir = glm::normalize(glm::vec3(inverseView * rayStart));

			RayCast r(glm::vec3(rayPos), raydir);
			if(r.intersects(millBox)) {
				std::cout << "hit" << std::endl;
			}

		}



		if(diff.count() < time_per_frame) {
			std::this_thread::sleep_for(std::chrono::microseconds((int)((time_per_frame - diff.count()) * 1000000)));
		}
		end = start;
	}
	
}