#pragma once
#include "RenderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SOIL/SOIL.h>
#include <vector>

class Demo :
	public RenderEngine
{
public:
	Demo();
	~Demo();
private:
	float angle = 0;
	GLuint shaderProgram, VBO, VAO, EBO, windows, 
		VBO2, VAO2, EBO2, grass, 
		VBO3, VAO3, EBO3, wood,
		VBO4, VAO4, EBO4, doubleDoor,
		VBO5, VAO5, EBO5, redWall,
		VBO6, VAO6, EBO6, brickWall,
		VBO7, VAO7, EBO7, sakura,
		VBO8, VAO8, EBO8, stair,
		VBO9, VAO9, EBO9, leaves,
		VBO10, VAO10, EBO10, spinner
		;
	float viewCamX, viewCamY, viewCamZ, upCamX, upCamY, upCamZ, posCamX, posCamY, posCamZ, CAMERA_SPEED, fovy;
	virtual void Init();
	virtual void DeInit();
	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void ProcessInput(GLFWwindow* window);
	void BuildColoredCube();
	void BuildColoredPlane();
	void DrawColoredCube();
	void DrawColoredPlane();
	void MoveCamera(float speed);
	void StrafeCamera(float speed);
	void RotateCamera(float speed);
	void InitCamera();

	void usualGLstuffs();

	void BuildRedWall();
	void DrawRedWall();
	void BuildBrickWall();
	void DrawBrickWall();

	void BuildLeaves();
	void DrawLeaves();

	void BuildDoubleDoor();
	void DrawDoubleDoor();

	void BuildWindows();
	void DrawWindows();

	void BuildWood();
	void DrawWood();

	void BuildSakura();
	void DrawSakura();

	void BuildStair();
	void DrawStair();

	void BuildSpinner();
	void DrawSpinner();

	void glTexInit();

};

