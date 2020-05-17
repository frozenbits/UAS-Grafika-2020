#include "Demo.h"
#include <iostream>
#include <vector>

using namespace std;


Demo::Demo() {

}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);
	BuildColoredPlane();

	BuildRedWall();
	BuildBrickWall();
	BuildLeaves();
	BuildDoubleDoor();
	BuildWood();
	BuildSakura();
	BuildStair();
	BuildWindows();
	BuildSpinner();

	InitCamera();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
	glDeleteVertexArrays(1, &VAO3);
	glDeleteBuffers(1, &VBO3);
	glDeleteBuffers(1, &EBO3);
	glDeleteVertexArrays(1, &VAO4);
	glDeleteBuffers(1, &VBO4);
	glDeleteBuffers(1, &EBO4);
	glDeleteVertexArrays(1, &VAO5);
	glDeleteBuffers(1, &VBO5);
	glDeleteBuffers(1, &EBO5);
	glDeleteVertexArrays(1, &VAO6);
	glDeleteBuffers(1, &VBO6);
	glDeleteBuffers(1, &EBO6);
	glDeleteVertexArrays(1, &VAO7);
	glDeleteBuffers(1, &VBO7);
	glDeleteBuffers(1, &EBO7);
	glDeleteVertexArrays(1, &VAO8);
	glDeleteBuffers(1, &VBO8);
	glDeleteBuffers(1, &EBO8);
	glDeleteVertexArrays(1, &VAO9);
	glDeleteBuffers(1, &VBO9);
	glDeleteBuffers(1, &EBO9); 
	glDeleteVertexArrays(1, &VAO10);
	glDeleteBuffers(1, &VBO10);
	glDeleteBuffers(1, &EBO10);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// zoom camera
	// -----------
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (fovy < 90) {
			fovy += 0.0001f;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (fovy > 0) {
			fovy -= 0.0001f;
		}
	}

	// update camera movement 
	// -------------
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}

	// update camera rotation
	// ----------------------
	double mouseX, mouseY;
	double midX = screenWidth / 2;
	double midY = screenHeight / 2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// Get mouse position
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// Set mouse position
	glfwSetCursorPos(window, midX, midY);

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angleY = (float)((midX - mouseX)) / 1000;
	angleZ = (float)((midY - mouseY)) / 1000;

	// The higher the value is the faster the camera looks around.
	viewCamY += angleZ * 2;

	// limit the rotation around the x-axis
	if ((viewCamY - posCamY) > 8) {
		viewCamY = posCamY + 8;
	}
	if ((viewCamY - posCamY) < -8) {
		viewCamY = posCamY - 8;
	}
	RotateCamera(-angleY);




}

void Demo::Update(double deltaTime) {
	angle += (float)((deltaTime * 0.5f) / 1000);
}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.788f, 0.850f, 0.929f, 1.0f);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(fovy, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(posCamX, posCamY, posCamZ), glm::vec3(viewCamX, viewCamY, viewCamZ), glm::vec3(upCamX, upCamY, upCamZ));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	DrawColoredPlane();

	DrawRedWall();
	DrawBrickWall();
	DrawLeaves();
	DrawDoubleDoor();
	DrawWood();
	DrawSakura();
	DrawStair();
	DrawWindows();
	DrawSpinner();

	glDisable(GL_DEPTH_TEST);
}

void Demo::glTexInit() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Demo::usualGLstuffs() {
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Demo::BuildColoredPlane()
{
	// Load and create a texture 
	glGenTextures(1, &grass);
	glBindTexture(GL_TEXTURE_2D, grass);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("grass.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-25.0, -0.5, -25.0,  0,  0,
		 25.0, -0.5, -25.0, 50,  0,
		 25.0, -0.5,  25.0, 50, 50,
		-25.0, -0.5,  25.0,  0, 50,
	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawColoredPlane()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, grass);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildRedWall() {
	// Load and create a texture 
	glGenTextures(1, &redWall);
	glBindTexture(GL_TEXTURE_2D, redWall);

	glTexInit();

	int width, height;
	unsigned char* image = SOIL_load_image("redbrick.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float xPos[] = {
		-3.5, 4.5, //belakang
		3.5, 4.5, //kanan
		-3.5, -2.5, //kiri
		-1.5, 2.5, //depan-atas
		-2.5, -1.5, //depan-kiri-bawah
		-1.5, -0.5, //depan-kiri-tegak
		2.5, 3.5, //depan-kanan-bawah
		1.5, 2.5, //depan-kanan-tegak
	};

	float yPos[] = {
		0.5, 3.5, //general
		2.5, 3.5, //depan-atas
		0.5, 1.5, //depan-bawah
		0.5, 2.5, //depan-tegak

	};

	float zPos[] = {
		0.5, -0.5, //depan
		-5.5, -6.5, //belakang
		0.5, -5.5 //kiri/kanan
	};

	// Build geometry
	float vertices[] = {
		// format position, tex coords

		//belakang===========================
		// front
		xPos[0], yPos[0], zPos[2], 0, 0,  // 0
		xPos[1], yPos[0], zPos[2], 8, 0,   // 1
		xPos[1], yPos[1], zPos[2], 8, 3,   // 2
		xPos[0], yPos[1], zPos[2], 0, 3,  // 3

		// right
		xPos[1], yPos[0], zPos[2], 0, 0,  // 4
		xPos[1], yPos[0], zPos[3], 1, 0,  // 5
		xPos[1], yPos[1], zPos[3], 1, 3,  // 6
		xPos[1], yPos[1], zPos[2], 0, 3,  // 7

		// back
		xPos[0], yPos[0], zPos[3], 0, 0, // 8 
		xPos[1], yPos[0], zPos[3], 8, 0, // 9
		xPos[1], yPos[1], zPos[3], 8, 3, // 10
		xPos[0], yPos[1], zPos[3], 0, 3, // 11

		// left
		xPos[0], yPos[0], zPos[3], 0, 0, // 12
		xPos[0], yPos[0], zPos[2], 1, 0, // 13
		xPos[0], yPos[1], zPos[2], 1, 3, // 14
		xPos[0], yPos[1], zPos[3], 0, 3, // 15

		//// upper
		xPos[1], yPos[1], zPos[2], 0, 0,  // 16
		xPos[0], yPos[1], zPos[2], 8, 0,   // 17
		xPos[0], yPos[1], zPos[3], 8, 1,   // 18
		xPos[1], yPos[1], zPos[3], 0, 1,  // 19

		//// bottom
		xPos[0], yPos[0], zPos[3], 0, 0,  // 20
		xPos[1], yPos[0], zPos[3], 8, 0,   // 21
		xPos[1], yPos[0], zPos[2], 8, 1,   // 22
		xPos[0], yPos[0], zPos[2], 0, 1,  // 23
		
		//kanan===========================
		// front
		xPos[2], yPos[0], zPos[4], 0, 0,  // 0
		xPos[3], yPos[0], zPos[4], 1, 0,   // 1
		xPos[3], yPos[1], zPos[4], 1, 3,   // 2
		xPos[2], yPos[1], zPos[4], 0, 3,  // 3

		// right
		xPos[3], yPos[0], zPos[4], 0, 0,  // 4
		xPos[3], yPos[0], zPos[5], 7, 0,  // 5
		xPos[3], yPos[1], zPos[5], 7, 3,  // 6
		xPos[3], yPos[1], zPos[4], 0, 3,  // 7

		// back
		xPos[2], yPos[0], zPos[5], 0, 0, // 8 
		xPos[3], yPos[0], zPos[5], 1, 0, // 9
		xPos[3], yPos[1], zPos[5], 1, 3, // 10
		xPos[2], yPos[1], zPos[5], 0, 3, // 11

		// left
		xPos[2], yPos[0], zPos[5], 0, 0, // 12
		xPos[2], yPos[0], zPos[4], 7, 0, // 13
		xPos[2], yPos[1], zPos[4], 7, 3, // 14
		xPos[2], yPos[1], zPos[5], 0, 3, // 15

		//// upper
		xPos[3], yPos[1], zPos[4], 0, 0,  // 16
		xPos[2], yPos[1], zPos[4], 7, 0,   // 17
		xPos[2], yPos[1], zPos[5], 7, 1,   // 18
		xPos[3], yPos[1], zPos[5], 0, 1,  // 19

		//// bottom
		xPos[2], yPos[0], zPos[5], 0, 0,  // 20
		xPos[3], yPos[0], zPos[5], 7, 0,   // 21
		xPos[3], yPos[0], zPos[4], 7, 1,   // 22
		xPos[2], yPos[0], zPos[4], 0, 1,  // 23

		//kiri===========================
		// front
		xPos[4], yPos[0], zPos[4], 0, 0,  // 0
		xPos[5], yPos[0], zPos[4], 1, 0,   // 1
		xPos[5], yPos[1], zPos[4], 1, 3,   // 2
		xPos[4], yPos[1], zPos[4], 0, 3,  // 3

		// right
		xPos[5], yPos[0], zPos[4], 0, 0,  // 4
		xPos[5], yPos[0], zPos[5], 7, 0,  // 5
		xPos[5], yPos[1], zPos[5], 7, 3,  // 6
		xPos[5], yPos[1], zPos[4], 0, 3,  // 7

		// back
		xPos[4], yPos[0], zPos[5], 0, 0, // 8 
		xPos[5], yPos[0], zPos[5], 1, 0, // 9
		xPos[5], yPos[1], zPos[5], 1, 3, // 10
		xPos[4], yPos[1], zPos[5], 0, 3, // 11

		// left
		xPos[4], yPos[0], zPos[5], 0, 0, // 12
		xPos[4], yPos[0], zPos[4], 7, 0, // 13
		xPos[4], yPos[1], zPos[4], 7, 3, // 14
		xPos[4], yPos[1], zPos[5], 0, 3, // 15

		//// upper
		xPos[5], yPos[1], zPos[4], 0, 0,  // 16
		xPos[4], yPos[1], zPos[4], 7, 0,   // 17
		xPos[4], yPos[1], zPos[5], 7, 1,   // 18
		xPos[5], yPos[1], zPos[5], 0, 1,  // 19

		//// bottom
		xPos[4], yPos[0], zPos[5], 0, 0,  // 20
		xPos[5], yPos[0], zPos[5], 7, 0,   // 21
		xPos[5], yPos[0], zPos[4], 7, 1,   // 22
		xPos[4], yPos[0], zPos[4], 0, 1,  // 23

		//depan-atas===========================
		// front
		xPos[6], yPos[2], zPos[0], 0, 0,  // 0
		xPos[7], yPos[2], zPos[0], 4, 0,   // 1
		xPos[7], yPos[3], zPos[0], 4, 1,   // 2
		xPos[6], yPos[3], zPos[0], 0, 1,  // 3

		// right
		xPos[7], yPos[2], zPos[0], 0, 0,  // 4
		xPos[7], yPos[2], zPos[1], 1, 0,  // 5
		xPos[7], yPos[3], zPos[1], 1, 1,  // 6
		xPos[7], yPos[3], zPos[0], 0, 1,  // 7

		// back
		xPos[6], yPos[2], zPos[1], 0, 0, // 8 
		xPos[7], yPos[2], zPos[1], 4, 0, // 9
		xPos[7], yPos[3], zPos[1], 4, 1, // 10
		xPos[6], yPos[3], zPos[1], 0, 1, // 11

		// left
		xPos[6], yPos[2], zPos[1], 0, 0, // 12
		xPos[6], yPos[2], zPos[0], 1, 0, // 13
		xPos[6], yPos[3], zPos[0], 1, 1, // 14
		xPos[6], yPos[3], zPos[1], 0, 1, // 15

		//// upper
		xPos[7], yPos[3], zPos[0], 0, 0,  // 16
		xPos[6], yPos[3], zPos[0], 4, 0,   // 17
		xPos[6], yPos[3], zPos[1], 4, 1,   // 18
		xPos[7], yPos[3], zPos[1], 0, 1,  // 19

		//// bottom
		xPos[6], yPos[2], zPos[1], 0, 0,  // 20
		xPos[7], yPos[2], zPos[1], 4, 0,   // 21
		xPos[7], yPos[2], zPos[0], 4, 1,   // 22
		xPos[6], yPos[2], zPos[0], 0, 1,  // 23

		//depan-kiri-bawah===========================
		// front
		xPos[8], yPos[4], zPos[0], 0, 0,  // 0
		xPos[9], yPos[4], zPos[0], 1, 0,   // 1
		xPos[9], yPos[5], zPos[0], 1, 1,   // 2
		xPos[8], yPos[5], zPos[0], 0, 1,  // 3

		// right
		xPos[9], yPos[4], zPos[0], 0, 0,  // 4
		xPos[9], yPos[4], zPos[1], 1, 0,  // 5
		xPos[9], yPos[5], zPos[1], 1, 1,  // 6
		xPos[9], yPos[5], zPos[0], 0, 1,  // 7

		// back
		xPos[8], yPos[4], zPos[1], 0, 0, // 8 
		xPos[9], yPos[4], zPos[1], 1, 0, // 9
		xPos[9], yPos[5], zPos[1], 1, 1, // 10
		xPos[8], yPos[5], zPos[1], 0, 1, // 11

		// left
		xPos[8], yPos[4], zPos[1], 0, 0, // 12
		xPos[8], yPos[4], zPos[0], 1, 0, // 13
		xPos[8], yPos[5], zPos[0], 1, 1, // 14
		xPos[8], yPos[5], zPos[1], 0, 1, // 15

		//// upper
		xPos[9], yPos[5], zPos[0], 0, 0,  // 16
		xPos[8], yPos[5], zPos[0], 1, 0,   // 17
		xPos[8], yPos[5], zPos[1], 1, 1,   // 18
		xPos[9], yPos[5], zPos[1], 0, 1,  // 19

		//// bottom
		xPos[8], yPos[4], zPos[1], 0, 0,  // 20
		xPos[9], yPos[4], zPos[1], 4, 0,   // 21
		xPos[9], yPos[4], zPos[0], 4, 1,   // 22
		xPos[8], yPos[4], zPos[0], 0, 1,  // 23

		//depan-kiri-tegak===========================
		// front
		xPos[10], yPos[6], zPos[0], 0, 0,  // 0
		xPos[11], yPos[6], zPos[0], 1, 0,   // 1
		xPos[11], yPos[7], zPos[0], 1, 2,   // 2
		xPos[10], yPos[7], zPos[0], 0, 2,  // 3

		// right
		xPos[11], yPos[6], zPos[0], 0, 0,  // 4
		xPos[11], yPos[6], zPos[1], 1, 0,  // 5
		xPos[11], yPos[7], zPos[1], 1, 2,  // 6
		xPos[11], yPos[7], zPos[0], 0, 2,  // 7

		// back
		xPos[10], yPos[6], zPos[1], 0, 0, // 8 
		xPos[11], yPos[6], zPos[1], 1, 0, // 9
		xPos[11], yPos[7], zPos[1], 1, 2, // 10
		xPos[10], yPos[7], zPos[1], 0, 2, // 11

		// left
		xPos[10], yPos[6], zPos[1], 0, 0, // 12
		xPos[10], yPos[6], zPos[0], 1, 0, // 13
		xPos[10], yPos[7], zPos[0], 1, 2, // 14
		xPos[10], yPos[7], zPos[1], 0, 2, // 15

		//// upper
		xPos[11], yPos[7], zPos[0], 0, 0,  // 16
		xPos[10], yPos[7], zPos[0], 1, 0,   // 17
		xPos[10], yPos[7], zPos[1], 1, 1,   // 18
		xPos[11], yPos[7], zPos[1], 0, 1,  // 19

		//// bottom
		xPos[10], yPos[6], zPos[1], 0, 0,  // 20
		xPos[11], yPos[6], zPos[1], 4, 0,   // 21
		xPos[11], yPos[6], zPos[0], 4, 1,   // 22
		xPos[10], yPos[6], zPos[0], 0, 1,  // 23

		//depan-kanan-bawah===========================
		// front
		xPos[12], yPos[4], zPos[0], 0, 0,  // 0
		xPos[13], yPos[4], zPos[0], 1, 0,   // 1
		xPos[13], yPos[5], zPos[0], 1, 1,   // 2
		xPos[12], yPos[5], zPos[0], 0, 1,  // 3

		// right
		xPos[13], yPos[4], zPos[0], 0, 0,  // 4
		xPos[13], yPos[4], zPos[1], 1, 0,  // 5
		xPos[13], yPos[5], zPos[1], 1, 1,  // 6
		xPos[13], yPos[5], zPos[0], 0, 1,  // 7

		// back
		xPos[12], yPos[4], zPos[1], 0, 0, // 8 
		xPos[13], yPos[4], zPos[1], 1, 0, // 9
		xPos[13], yPos[5], zPos[1], 1, 1, // 10
		xPos[12], yPos[5], zPos[1], 0, 1, // 11

		// left
		xPos[12], yPos[4], zPos[1], 0, 0, // 12
		xPos[12], yPos[4], zPos[0], 1, 0, // 13
		xPos[12], yPos[5], zPos[0], 1, 1, // 14
		xPos[12], yPos[5], zPos[1], 0, 1, // 15

		//// upper
		xPos[13], yPos[5], zPos[0], 0, 0,  // 16
		xPos[12], yPos[5], zPos[0], 1, 0,   // 17
		xPos[12], yPos[5], zPos[1], 1, 1,   // 18
		xPos[13], yPos[5], zPos[1], 0, 1,  // 19

		//// bottom
		xPos[12], yPos[4], zPos[1], 0, 0,  // 20
		xPos[13], yPos[4], zPos[1], 4, 0,   // 21
		xPos[13], yPos[4], zPos[0], 4, 1,   // 22
		xPos[12], yPos[4], zPos[0], 0, 1,  // 23

		//depan-kanan-tegak===========================
		// front
		xPos[14], yPos[6], zPos[0], 0, 0,  // 0
		xPos[15], yPos[6], zPos[0], 1, 0,   // 1
		xPos[15], yPos[7], zPos[0], 1, 2,   // 2
		xPos[14], yPos[7], zPos[0], 0, 2,  // 3

		// right
		xPos[15], yPos[6], zPos[0], 0, 0,  // 4
		xPos[15], yPos[6], zPos[1], 1, 0,  // 5
		xPos[15], yPos[7], zPos[1], 1, 2,  // 6
		xPos[15], yPos[7], zPos[0], 0, 2,  // 7

		// back
		xPos[14], yPos[6], zPos[1], 0, 0, // 8 
		xPos[15], yPos[6], zPos[1], 1, 0, // 9
		xPos[15], yPos[7], zPos[1], 1, 2, // 10
		xPos[14], yPos[7], zPos[1], 0, 2, // 11

		// left
		xPos[14], yPos[6], zPos[1], 0, 0, // 12
		xPos[14], yPos[6], zPos[0], 1, 0, // 13
		xPos[14], yPos[7], zPos[0], 1, 2, // 14
		xPos[14], yPos[7], zPos[1], 0, 2, // 15

		//// upper
		xPos[15], yPos[7], zPos[0], 0, 0,  // 16
		xPos[14], yPos[7], zPos[0], 1, 0,   // 17
		xPos[14], yPos[7], zPos[1], 1, 1,   // 18
		xPos[15], yPos[7], zPos[1], 0, 1,  // 19

		//// bottom
		xPos[14], yPos[6], zPos[1], 0, 0,  // 20
		xPos[15], yPos[6], zPos[1], 4, 0,   // 21
		xPos[15], yPos[6], zPos[0], 4, 1,   // 22
		xPos[14], yPos[6], zPos[0], 0, 1,  // 23
	};

	unsigned int indices[] = {
		//belakang
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
		//kanan
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
		//kiri
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
		//depan-atas
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
		//depan-kiri-bawah
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
		//depan-kiri-tegak
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
		//depan-kanan-bawah
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
		//depan-kanan-tegak
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
	};

	int shapeCounter = 8;

	int indices_size = (36 * shapeCounter);

	for (int i = 1; i < shapeCounter; i++)
	{
		for (int j = (36 * i); j < indices_size; j++) {
			indices[j] += 24;
		};
	}

	glGenVertexArrays(1, &VAO5);
	glGenBuffers(1, &VBO5);
	glGenBuffers(1, &EBO5);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO5);

	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO5);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	usualGLstuffs();
}

void Demo::DrawRedWall() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, redWall);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO5); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, (36*8), GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildBrickWall() {
	// Load and create a texture 
	glGenTextures(1, &brickWall);
	glBindTexture(GL_TEXTURE_2D, brickWall);

	glTexInit();

	int width, height;
	unsigned char* image = SOIL_load_image("brick.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float xPos[] = {
		-3.5, 4.5
	};

	float yPos[] = {
		-0.5, 0.5,
		3.5, 4.5 
	};

	float zPos[] = {
		0.5, -6.5
	};

	// Build geometry
	float vertices[] = {
		// format position, tex coords
		//bawah===========================

		// front
		xPos[0], yPos[0], zPos[0], 0, 0,  // 0
		xPos[1], yPos[0], zPos[0], 8, 0,   // 1
		xPos[1], yPos[1], zPos[0], 8, 1,   // 2
		xPos[0], yPos[1], zPos[0], 0, 1,  // 3

		// right
		xPos[1], yPos[0], zPos[0], 0, 0,  // 4
		xPos[1], yPos[0], zPos[1], 8, 0,  // 5
		xPos[1], yPos[1], zPos[1], 8, 1,  // 6
		xPos[1], yPos[1], zPos[0], 0, 1,  // 7

		// back
		xPos[0], yPos[0], zPos[1], 0, 0, // 8 
		xPos[1], yPos[0], zPos[1], 8, 0, // 9
		xPos[1], yPos[1], zPos[1], 8, 1, // 10
		xPos[0], yPos[1], zPos[1], 0, 1, // 11

		// left
		xPos[0], yPos[0], zPos[1], 0, 0, // 12
		xPos[0], yPos[0], zPos[0], 8, 0, // 13
		xPos[0], yPos[1], zPos[0], 8, 1, // 14
		xPos[0], yPos[1], zPos[1], 0, 1, // 15

		//// upper
		xPos[1], yPos[1], zPos[0], 0, 0,  // 16
		xPos[0], yPos[1], zPos[0], 8, 0,   // 17
		xPos[0], yPos[1], zPos[1], 8, 7,   // 18
		xPos[1], yPos[1], zPos[1], 0, 7,  // 19

		//// bottom
		xPos[0], yPos[0], zPos[1], 0, 0,  // 20
		xPos[1], yPos[0], zPos[1], 8, 0,   // 21
		xPos[1], yPos[0], zPos[0], 8, 7,   // 22
		xPos[0], yPos[0], zPos[0], 0, 7,  // 23

		//atas=========================================

		// front
		xPos[0], yPos[2], zPos[0], 0, 0,  // 0
		xPos[1], yPos[2], zPos[0], 8, 0,   // 1
		xPos[1], yPos[3], zPos[0], 8, 1,   // 2
		xPos[0], yPos[3], zPos[0], 0, 1,  // 3

		// right
		xPos[1], yPos[2], zPos[0], 0, 0,  // 4
		xPos[1], yPos[2], zPos[1], 8, 0,  // 5
		xPos[1], yPos[3], zPos[1], 8, 1,  // 6
		xPos[1], yPos[3], zPos[0], 0, 1,  // 7

		// back
		xPos[0], yPos[2], zPos[1], 0, 0, // 8 
		xPos[1], yPos[2], zPos[1], 8, 0, // 9
		xPos[1], yPos[3], zPos[1], 8, 1, // 10
		xPos[0], yPos[3], zPos[1], 0, 1, // 11

		// left
		xPos[0], yPos[2], zPos[1], 0, 0, // 12
		xPos[0], yPos[2], zPos[0], 8, 0, // 13
		xPos[0], yPos[3], zPos[0], 8, 1, // 14
		xPos[0], yPos[3], zPos[1], 0, 1, // 15

		//// upper
		xPos[1], yPos[3], zPos[0], 0, 0,  // 16
		xPos[0], yPos[3], zPos[0], 8, 0,   // 17
		xPos[0], yPos[3], zPos[1], 8, 7,   // 18
		xPos[1], yPos[3], zPos[1], 0, 7,  // 19

		//// bottom
		xPos[0], yPos[2], zPos[1], 0, 0,  // 20
		xPos[1], yPos[2], zPos[1], 8, 0,   // 21
		xPos[1], yPos[2], zPos[0], 8, 7,   // 22
		xPos[0], yPos[2], zPos[0], 0, 7,  // 23
	};

	unsigned int indices[] = {
	//belakang, bawah
	0,  1,  2,  0,  2,  3,   // front
	4,  5,  6,  4,  6,  7,   // right
	8,  9,  10, 8,  10, 11,  // back
	12, 14, 13, 12, 15, 14,  // left
	16, 18, 17, 16, 19, 18,  // upper
	20, 22, 21, 20, 23, 22, // bottom
	//belakang, atas
	0,  1,  2,  0,  2,  3,   // front
	4,  5,  6,  4,  6,  7,   // right
	8,  9,  10, 8,  10, 11,  // back
	12, 14, 13, 12, 15, 14,  // left
	16, 18, 17, 16, 19, 18,  // upper
	20, 22, 21, 20, 23, 22 // bottom
	};

	int shapeCounter = 2;
	int indices_size = (36 * shapeCounter);

	for (int i = (36 * 1); i < indices_size; i++) {
		indices[i] += 24;
	};
	//for (int i = (36 * 2); i < indices_size; i++) {
	//	indices[i] += 24;
	//};

	glGenVertexArrays(1, &VAO6);
	glGenBuffers(1, &VBO6);
	glGenBuffers(1, &EBO6);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO6);

	glBindBuffer(GL_ARRAY_BUFFER, VBO6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO6);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	usualGLstuffs();
}

void Demo::DrawBrickWall() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brickWall);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO6); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, (36*2), GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildWindows() {
	// Load and create a texture 
	glGenTextures(1, &windows);
	glBindTexture(GL_TEXTURE_2D, windows);

	glTexInit();

	int width, height;
	unsigned char* image = SOIL_load_image("window.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float xPos[] = {
		-2.5, -1.5,
		2.5, 3.5
	};

	float yPos[] = {
		1.5, 3.5
	};

	float zPos[] = {
		0.5, -0.5
	};

	// Build geometry
	float vertices[] = {
		//kiri======================
		// front
		xPos[0], yPos[0], zPos[0], 0, 0,  // 0
		xPos[1], yPos[0], zPos[0], 1, 0,   // 1
		xPos[1], yPos[1], zPos[0], 1, 1,   // 2
		xPos[0], yPos[1], zPos[0], 0, 1,  // 3

		// right
		xPos[1], yPos[0], zPos[0], 0, 0,  // 4
		xPos[1], yPos[0], zPos[1], 1, 0,  // 5
		xPos[1], yPos[1], zPos[1], 1, 1,  // 6
		xPos[1], yPos[1], zPos[0], 0, 1,  // 7

		// back
		xPos[0], yPos[0], zPos[1], 0, 0, // 8 
		xPos[1], yPos[0], zPos[1], 1, 0, // 9
		xPos[1], yPos[1], zPos[1], 1, 1, // 10
		xPos[0], yPos[1], zPos[1], 0, 1, // 11

		// left
		xPos[0], yPos[0], zPos[1], 0, 0, // 12
		xPos[0], yPos[0], zPos[0], 1, 0, // 13
		xPos[0], yPos[1], zPos[0], 1, 1, // 14
		xPos[0], yPos[1], zPos[1], 0, 1, // 15

		//// upper
		xPos[1], yPos[1], zPos[0], 0, 0,  // 16
		xPos[0], yPos[1], zPos[0], 1, 0,   // 17
		xPos[0], yPos[1], zPos[1], 1, 1,   // 18
		xPos[1], yPos[1], zPos[1], 0, 1,  // 19

		//// bottom
		xPos[0], yPos[0], zPos[1], 0, 0,  // 20
		xPos[1], yPos[0], zPos[1], 1, 0,   // 21
		xPos[1], yPos[0], zPos[0], 1, 1,   // 22
		xPos[0], yPos[0], zPos[0], 0, 1,  // 23

		//kanan======================
		// front
		xPos[2], yPos[0], zPos[0], 0, 0,  // 0
		xPos[3], yPos[0], zPos[0], 1, 0,   // 1
		xPos[3], yPos[1], zPos[0], 1, 1,   // 2
		xPos[2], yPos[1], zPos[0], 0, 1,  // 3

		// right
		xPos[3], yPos[0], zPos[0], 0, 0,  // 4
		xPos[3], yPos[0], zPos[1], 1, 0,  // 5
		xPos[3], yPos[1], zPos[1], 1, 1,  // 6
		xPos[3], yPos[1], zPos[0], 0, 1,  // 7

		// back
		xPos[2], yPos[0], zPos[1], 0, 0, // 8 
		xPos[3], yPos[0], zPos[1], 1, 0, // 9
		xPos[3], yPos[1], zPos[1], 1, 1, // 10
		xPos[2], yPos[1], zPos[1], 0, 1, // 11

		// left
		xPos[2], yPos[0], zPos[1], 0, 0, // 12
		xPos[2], yPos[0], zPos[0], 1, 0, // 13
		xPos[2], yPos[1], zPos[0], 1, 1, // 14
		xPos[2], yPos[1], zPos[1], 0, 1, // 15

		//// upper
		xPos[3], yPos[1], zPos[0], 0, 0,  // 16
		xPos[2], yPos[1], zPos[0], 1, 0,   // 17
		xPos[2], yPos[1], zPos[1], 1, 1,   // 18
		xPos[3], yPos[1], zPos[1], 0, 1,  // 19

		//// bottom
		xPos[2], yPos[0], zPos[1], 0, 0,  // 20
		xPos[3], yPos[0], zPos[1], 1, 0,   // 21
		xPos[3], yPos[0], zPos[0], 1, 1,   // 22
		xPos[2], yPos[0], zPos[0], 0, 1,  // 23
	};

	unsigned int indices[] = {
		//kiri
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
		//kanan
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
	};

	int shapeCounter = 2;
	int indices_size = (36 * shapeCounter);

	for (int i = 1; i < shapeCounter; i++)
	{
		for (int j = (36 * i); j < indices_size; j++) {
			indices[j] += 24;
		};
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	usualGLstuffs();
}

void Demo::DrawWindows() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, windows);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, (36 * 2), GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildDoubleDoor() {
	// Load and create a texture 
	glGenTextures(1, &doubleDoor);
	glBindTexture(GL_TEXTURE_2D, doubleDoor);

	glTexInit();

	int width, height;
	unsigned char* image = SOIL_load_image("doubledoor.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float xPos[] = {
		-0.5, 1.5
	};

	float yPos[] = {
		0.5, 2.5
	};

	float zPos[] = {
		0.5, 0.25
	};

	// Build geometry
	float vertices[] = {
		//===========================
		// front
		xPos[0], yPos[0], zPos[0], 0, 0,  // 0
		xPos[1], yPos[0], zPos[0], 1, 0,   // 1
		xPos[1], yPos[1], zPos[0], 1, 1,   // 2
		xPos[0], yPos[1], zPos[0], 0, 1,  // 3

		// right
		xPos[1], yPos[0], zPos[0], 0, 0,  // 4
		xPos[1], yPos[0], zPos[1], 1, 0,  // 5
		xPos[1], yPos[1], zPos[1], 1, 1,  // 6
		xPos[1], yPos[1], zPos[0], 0, 1,  // 7

		// back
		xPos[0], yPos[0], zPos[1], 0, 0, // 8 
		xPos[1], yPos[0], zPos[1], 1, 0, // 9
		xPos[1], yPos[1], zPos[1], 1, 1, // 10
		xPos[0], yPos[1], zPos[1], 0, 1, // 11

		// left
		xPos[0], yPos[0], zPos[1], 0, 0, // 12
		xPos[0], yPos[0], zPos[0], 1, 0, // 13
		xPos[0], yPos[1], zPos[0], 1, 1, // 14
		xPos[0], yPos[1], zPos[1], 0, 1, // 15

		//// upper
		xPos[1], yPos[1], zPos[0], 0, 0,  // 16
		xPos[0], yPos[1], zPos[0], 1, 0,   // 17
		xPos[0], yPos[1], zPos[1], 1, 1,   // 18
		xPos[1], yPos[1], zPos[1], 0, 1,  // 19

		//// bottom
		xPos[0], yPos[0], zPos[1], 0, 0,  // 20
		xPos[1], yPos[0], zPos[1], 1, 0,   // 21
		xPos[1], yPos[0], zPos[0], 1, 1,   // 22
		xPos[0], yPos[0], zPos[0], 0, 1,  // 23
	};

	unsigned int indices[] = {
		//belakang
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
	};

	int shapeCounter = 1;
	int indices_size = (36 * shapeCounter);

	for (int i = 1; i < shapeCounter; i++)
	{
		for (int j = (36 * i); j < indices_size; j++) {
			indices[j] += 24;
		};
	}

	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);
	glGenBuffers(1, &EBO4);
	glBindVertexArray(VAO4);
	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	usualGLstuffs();
}

void Demo::DrawDoubleDoor() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, doubleDoor);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO4);

	glDrawElements(GL_TRIANGLES, (36 * 1), GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildWood() {
	// Load and create a texture 
	glGenTextures(1, &wood);
	glBindTexture(GL_TEXTURE_2D, wood);

	glTexInit();

	int width, height;
	unsigned char* image = SOIL_load_image("wood.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float xPos[] = {
		5.5, 6.5
	};

	float yPos[] = {
		-0.5, 4.5
	};

	float zPos[] = {
		4.5, 3.5
	};

	float xPos2[] = {
		5.5, 6.5
	};

	float yPos2[] = {
		-0.5, 4.5
	};

	float zPos2[] = {
		4.5, 3.5
	};

	for (int i = 0; i < 2; i++)
	{
		xPos2[i] = xPos[i] - 12;
		zPos2[i] = zPos[i] - 6;
	}

	// Build geometry
	float vertices[] = {
		//================================
		// front
		xPos[0], yPos[0], zPos[0], 0, 0,  // 0
		xPos[1], yPos[0], zPos[0], 1, 0,   // 1
		xPos[1], yPos[1], zPos[0], 1, 5,   // 2
		xPos[0], yPos[1], zPos[0], 0, 5,  // 3

		// right
		xPos[1], yPos[0], zPos[0], 0, 0,  // 4
		xPos[1], yPos[0], zPos[1], 1, 0,  // 5
		xPos[1], yPos[1], zPos[1], 1, 5,  // 6
		xPos[1], yPos[1], zPos[0], 0, 5,  // 7

		// back
		xPos[0], yPos[0], zPos[1], 0, 0, // 8 
		xPos[1], yPos[0], zPos[1], 1, 0, // 9
		xPos[1], yPos[1], zPos[1], 1, 5, // 10
		xPos[0], yPos[1], zPos[1], 0, 5, // 11

		// left
		xPos[0], yPos[0], zPos[1], 0, 0, // 12
		xPos[0], yPos[0], zPos[0], 1, 0, // 13
		xPos[0], yPos[1], zPos[0], 1, 5, // 14
		xPos[0], yPos[1], zPos[1], 0, 5, // 15

		//// upper
		xPos[1], yPos[1], zPos[0], 0, 0,  // 16
		xPos[0], yPos[1], zPos[0], 1, 0,   // 17
		xPos[0], yPos[1], zPos[1], 1, 1,   // 18
		xPos[1], yPos[1], zPos[1], 0, 1,  // 19

		//// bottom
		xPos[0], yPos[0], zPos[1], 0, 0,  // 20
		xPos[1], yPos[0], zPos[1], 1, 0,   // 21
		xPos[1], yPos[0], zPos[0], 1, 1,   // 22
		xPos[0], yPos[0], zPos[0], 0, 1,  // 23
		
		//secondtree====================								  
		// front
		xPos2[0], yPos2[0], zPos2[0], 0, 0,  // 0
		xPos2[1], yPos2[0], zPos2[0], 1, 0,   // 1
		xPos2[1], yPos2[1], zPos2[0], 1, 5,   // 2
		xPos2[0], yPos2[1], zPos2[0], 0, 5,  // 3

		// right
		xPos2[1], yPos2[0], zPos2[0], 0, 0,  // 4
		xPos2[1], yPos2[0], zPos2[1], 1, 0,  // 5
		xPos2[1], yPos2[1], zPos2[1], 1, 5,  // 6
		xPos2[1], yPos2[1], zPos2[0], 0, 5,  // 7

		// back
		xPos2[0], yPos2[0], zPos2[1], 0, 0, // 8 
		xPos2[1], yPos2[0], zPos2[1], 1, 0, // 9
		xPos2[1], yPos2[1], zPos2[1], 1, 5, // 10
		xPos2[0], yPos2[1], zPos2[1], 0, 5, // 11

		// left
		xPos2[0], yPos2[0], zPos2[1], 0, 0, // 12
		xPos2[0], yPos2[0], zPos2[0], 1, 0, // 13
		xPos2[0], yPos2[1], zPos2[0], 1, 5, // 14
		xPos2[0], yPos2[1], zPos2[1], 0, 5, // 15

		//// upper
		xPos2[1], yPos2[1], zPos2[0], 0, 0,  // 16
		xPos2[0], yPos2[1], zPos2[0], 1, 0,   // 17
		xPos2[0], yPos2[1], zPos2[1], 1, 1,   // 18
		xPos2[1], yPos2[1], zPos2[1], 0, 1,  // 19

		//// bottom
		xPos2[0], yPos2[0], zPos2[1], 0, 0,  // 20
		xPos2[1], yPos2[0], zPos2[1], 1, 0,   // 21
		xPos2[1], yPos2[0], zPos2[0], 1, 1,   // 22
		xPos2[0], yPos2[0], zPos2[0], 0, 1,  // 23
	};

	unsigned int indices[] = {
		//satu
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
		//dua
		0, 1, 2, 0, 2, 3,   // front
		4, 5, 6, 4, 6, 7,   // right
		8, 9, 10, 8, 10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
	};

	int shapeCounter = 2;
	int indices_size = (36 * shapeCounter);

	for (int i = 1; i < shapeCounter; i++)
	{
		for (int j = (36 * i); j < indices_size; j++) {
			indices[j] += 24;
		};
	}

	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO3);
	glBindVertexArray(VAO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	usualGLstuffs();
}

void Demo::DrawWood() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wood);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO3); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, (36 * 2), GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildSakura() {
	// Load and create a texture 
	glGenTextures(1, &sakura);
	glBindTexture(GL_TEXTURE_2D, sakura);

	glTexInit();

	int width, height;
	unsigned char* image = SOIL_load_image("sakura.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float xPos[] = {
		3.5, 8.5,
		4.5, 7.5
	};

	float yPos[] = {
		3.5, 5.5,
		5.5, 6.5
	};

	float zPos[] = {
		6.5, 1.5,
		5.5, 2.5
	};

	float xPos2[] = {
		3.5, 8.5,
		4.5, 7.5
	};

	float yPos2[] = {
		3.5, 5.5,
		5.5, 6.5
	};

	float zPos2[] = {
		6.5, 1.5,
		5.5, 2.5
	};

	for (int i = 0; i < 4; i++)
	{
		xPos2[i] = xPos[i] - 12;
		zPos2[i] = zPos[i] - 6;
	}

	// Build geometry
	float vertices[] = {
		//bawah ================================
		// front
		xPos[0], yPos[0], zPos[0], 0, 0,  // 0
		xPos[1], yPos[0], zPos[0], 5, 0,   // 1
		xPos[1], yPos[1], zPos[0], 5, 3,   // 2
		xPos[0], yPos[1], zPos[0], 0, 3,  // 3

		// right
		xPos[1], yPos[0], zPos[0], 0, 0,  // 4
		xPos[1], yPos[0], zPos[1], 5, 0,  // 5
		xPos[1], yPos[1], zPos[1], 5, 3,  // 6
		xPos[1], yPos[1], zPos[0], 0, 3,  // 7

		// back
		xPos[0], yPos[0], zPos[1], 0, 0, // 8 
		xPos[1], yPos[0], zPos[1], 5, 0, // 9
		xPos[1], yPos[1], zPos[1], 5, 3, // 10
		xPos[0], yPos[1], zPos[1], 0, 3, // 11

		// left
		xPos[0], yPos[0], zPos[1], 0, 0, // 12
		xPos[0], yPos[0], zPos[0], 5, 0, // 13
		xPos[0], yPos[1], zPos[0], 5, 3, // 14
		xPos[0], yPos[1], zPos[1], 0, 3, // 15

		//// upper
		xPos[1], yPos[1], zPos[0], 0, 0,  // 16
		xPos[0], yPos[1], zPos[0], 5, 0,   // 17
		xPos[0], yPos[1], zPos[1], 5, 5,   // 18
		xPos[1], yPos[1], zPos[1], 0, 5,  // 19

		//// bottom
		xPos[0], yPos[0], zPos[1], 0, 0,  // 20
		xPos[1], yPos[0], zPos[1], 5, 0,   // 21
		xPos[1], yPos[0], zPos[0], 5, 5,   // 22
		xPos[0], yPos[0], zPos[0], 0, 5,  // 23
		//atas================================
		// front
		xPos[2], yPos[2], zPos[2], 0, 0,  // 0
		xPos[3], yPos[2], zPos[2], 5, 0,   // 1
		xPos[3], yPos[3], zPos[2], 5, 3,   // 2
		xPos[2], yPos[3], zPos[2], 0, 3,  // 3

		// right
		xPos[3], yPos[2], zPos[2], 0, 0,  // 4
		xPos[3], yPos[2], zPos[3], 5, 0,  // 5
		xPos[3], yPos[3], zPos[3], 5, 3,  // 6
		xPos[3], yPos[3], zPos[2], 0, 3,  // 7

		// back
		xPos[2], yPos[2], zPos[3], 0, 0, // 8 
		xPos[3], yPos[2], zPos[3], 5, 0, // 9
		xPos[3], yPos[3], zPos[3], 5, 3, // 10
		xPos[2], yPos[3], zPos[3], 0, 3, // 11

		// left
		xPos[2], yPos[2], zPos[3], 0, 0, // 12
		xPos[2], yPos[2], zPos[2], 5, 0, // 13
		xPos[2], yPos[3], zPos[2], 5, 3, // 14
		xPos[2], yPos[3], zPos[3], 0, 3, // 15

		//// upper
		xPos[3], yPos[3], zPos[2], 0, 0,  // 16
		xPos[2], yPos[3], zPos[2], 5, 0,   // 17
		xPos[2], yPos[3], zPos[3], 5, 5,   // 18
		xPos[3], yPos[3], zPos[3], 0, 5,  // 19

		//// bottom
		xPos[2], yPos[2], zPos[3], 0, 0,  // 20
		xPos[3], yPos[2], zPos[3], 5, 0,   // 21
		xPos[3], yPos[2], zPos[2], 5, 5,   // 22
		xPos[2], yPos[2], zPos[2], 0, 5,  // 23
		//secondtree								  
		//bawah ================================
		// front
		xPos2[0], yPos2[0], zPos2[0], 0, 0,  // 0
		xPos2[1], yPos2[0], zPos2[0], 5, 0,   // 1
		xPos2[1], yPos2[1], zPos2[0], 5, 3,   // 2
		xPos2[0], yPos2[1], zPos2[0], 0, 3,  // 3

		// right
		xPos2[1], yPos2[0], zPos2[0], 0, 0,  // 4
		xPos2[1], yPos2[0], zPos2[1], 5, 0,  // 5
		xPos2[1], yPos2[1], zPos2[1], 5, 3,  // 6
		xPos2[1], yPos2[1], zPos2[0], 0, 3,  // 7

		// back
		xPos2[0], yPos2[0], zPos2[1], 0, 0, // 8 
		xPos2[1], yPos2[0], zPos2[1], 5, 0, // 9
		xPos2[1], yPos2[1], zPos2[1], 5, 3, // 10
		xPos2[0], yPos2[1], zPos2[1], 0, 3, // 11

		// left
		xPos2[0], yPos2[0], zPos2[1], 0, 0, // 12
		xPos2[0], yPos2[0], zPos2[0], 5, 0, // 13
		xPos2[0], yPos2[1], zPos2[0], 5, 3, // 14
		xPos2[0], yPos2[1], zPos2[1], 0, 3, // 15

		//// upper
		xPos2[1], yPos2[1], zPos2[0], 0, 0,  // 16
		xPos2[0], yPos2[1], zPos2[0], 5, 0,   // 17
		xPos2[0], yPos2[1], zPos2[1], 5, 5,   // 18
		xPos2[1], yPos2[1], zPos2[1], 0, 5,  // 19

		//// bottom
		xPos2[0], yPos2[0], zPos2[1], 0, 0,  // 20
		xPos2[1], yPos2[0], zPos2[1], 5, 0,   // 21
		xPos2[1], yPos2[0], zPos2[0], 5, 5,   // 22
		xPos2[0], yPos2[0], zPos2[0], 0, 5,  // 23
		//atas================================
		// front
		xPos2[2], yPos2[2], zPos2[2], 0, 0,  // 0
		xPos2[3], yPos2[2], zPos2[2], 5, 0,   // 1
		xPos2[3], yPos2[3], zPos2[2], 5, 3,   // 2
		xPos2[2], yPos2[3], zPos2[2], 0, 3,  // 3

		// right
		xPos2[3], yPos2[2], zPos2[2], 0, 0,  // 4
		xPos2[3], yPos2[2], zPos2[3], 5, 0,  // 5
		xPos2[3], yPos2[3], zPos2[3], 5, 3,  // 6
		xPos2[3], yPos2[3], zPos2[2], 0, 3,  // 7

		// back
		xPos2[2], yPos2[2], zPos2[3], 0, 0, // 8 
		xPos2[3], yPos2[2], zPos2[3], 5, 0, // 9
		xPos2[3], yPos2[3], zPos2[3], 5, 3, // 10
		xPos2[2], yPos2[3], zPos2[3], 0, 3, // 11

		// left
		xPos2[2], yPos2[2], zPos2[3], 0, 0, // 12
		xPos2[2], yPos2[2], zPos2[2], 5, 0, // 13
		xPos2[2], yPos2[3], zPos2[2], 5, 3, // 14
		xPos2[2], yPos2[3], zPos2[3], 0, 3, // 15

		//// upper
		xPos2[3], yPos2[3], zPos2[2], 0, 0,  // 16
		xPos2[2], yPos2[3], zPos2[2], 5, 0,   // 17
		xPos2[2], yPos2[3], zPos2[3], 5, 5,   // 18
		xPos2[3], yPos2[3], zPos2[3], 0, 5,  // 19

		//// bottom
		xPos[2], yPos[2], zPos[3], 0, 0,  // 20
		xPos[3], yPos[2], zPos[3], 5, 0,   // 21
		xPos[3], yPos[2], zPos[2], 5, 5,   // 22
		xPos[2], yPos[2], zPos[2], 0, 5,  // 23
	};

	unsigned int indices[] = {
		//bawah
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
		//atas
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
		//bawah
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
		//atas
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
	};

	int shapeCounter = 4;
	int indices_size = (36 * shapeCounter);

	for (int i = 1; i < shapeCounter; i++)
	{
		for (int j = (36 * i); j < indices_size; j++) {
			indices[j] += 24;
		};
	}

	glGenVertexArrays(1, &VAO7);
	glGenBuffers(1, &VBO7);
	glGenBuffers(1, &EBO7);
	glBindVertexArray(VAO7);
	glBindBuffer(GL_ARRAY_BUFFER, VBO7);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO7);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	usualGLstuffs();
}

void Demo::DrawSakura() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sakura);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO7); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, (36 * 4), GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildSpinner() {
	// Load and create a texture 
	glGenTextures(1, &spinner);
	glBindTexture(GL_TEXTURE_2D, spinner);

	glTexInit();

	int width, height;
	unsigned char* image = SOIL_load_image("cloud.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float xPos[] = {
		-10.5, -6.5
	};

	float yPos[] = {
		1.5, 3.5
	};

	float zPos[] = {
		-1.5, -3.5
	}; 
	
	float xPos2[] = {
		-11.5, -5.5
	};

	float yPos2[] = {
		1.5, 2.5
	};

	float zPos2[] = {
		-0.5, -4.5
	};
	float xPos3[] = {
		-10.5, -6.5
	};

	float yPos3[] = {
		0.5, 1.5
	};

	float zPos3[] = {
		-1.5, -3.5
	};

	// Build geometry
	float vertices[] = {
		//dalam ================================
		// front
		xPos[0], yPos[0], zPos[0], 0, 0,  // 0
		xPos[1], yPos[0], zPos[0], 4, 0,   // 1
		xPos[1], yPos[1], zPos[0], 4, 2,   // 2
		xPos[0], yPos[1], zPos[0], 0, 2,  // 3

		// right
		xPos[1], yPos[0], zPos[0], 0, 0,  // 4
		xPos[1], yPos[0], zPos[1], 4, 0,  // 5
		xPos[1], yPos[1], zPos[1], 4, 2,  // 6
		xPos[1], yPos[1], zPos[0], 0, 2,  // 7

		// back
		xPos[0], yPos[0], zPos[1], 0, 0, // 8 
		xPos[1], yPos[0], zPos[1], 4, 0, // 9
		xPos[1], yPos[1], zPos[1], 4, 2, // 10
		xPos[0], yPos[1], zPos[1], 0, 2, // 11

		// left
		xPos[0], yPos[0], zPos[1], 0, 0, // 12
		xPos[0], yPos[0], zPos[0], 4, 0, // 13
		xPos[0], yPos[1], zPos[0], 4, 2, // 14
		xPos[0], yPos[1], zPos[1], 0, 2, // 15

		//// upper
		xPos[1], yPos[1], zPos[0], 0, 0,  // 16
		xPos[0], yPos[1], zPos[0], 4, 0,   // 17
		xPos[0], yPos[1], zPos[1], 4, 2,   // 18
		xPos[1], yPos[1], zPos[1], 0, 2,  // 19

		//// bottom
		xPos[0], yPos[0], zPos[1], 0, 0,  // 20
		xPos[1], yPos[0], zPos[1], 4, 0,   // 21
		xPos[1], yPos[0], zPos[0], 4, 2,   // 22
		xPos[0], yPos[0], zPos[0], 0, 2,  // 23
		//luar ================================
		// front
		xPos2[0], yPos2[0], zPos2[0], 0, 0,  // 0
		xPos2[1], yPos2[0], zPos2[0], 4, 0,   // 1
		xPos2[1], yPos2[1], zPos2[0], 4, 2,   // 2
		xPos2[0], yPos2[1], zPos2[0], 0, 2,  // 3

		// right
		xPos2[1], yPos2[0], zPos2[0], 0, 0,  // 4
		xPos2[1], yPos2[0], zPos2[1], 4, 0,  // 5
		xPos2[1], yPos2[1], zPos2[1], 4, 2,  // 6
		xPos2[1], yPos2[1], zPos2[0], 0, 2,  // 7

		// back
		xPos2[0], yPos2[0], zPos2[1], 0, 0, // 8 
		xPos2[1], yPos2[0], zPos2[1], 4, 0, // 9
		xPos2[1], yPos2[1], zPos2[1], 4, 2, // 10
		xPos2[0], yPos2[1], zPos2[1], 0, 2, // 11

		// left
		xPos2[0], yPos2[0], zPos2[1], 0, 0, // 12
		xPos2[0], yPos2[0], zPos2[0], 4, 0, // 13
		xPos2[0], yPos2[1], zPos2[0], 4, 2, // 14
		xPos2[0], yPos2[1], zPos2[1], 0, 2, // 15

		//// upper
		xPos2[1], yPos2[1], zPos2[0], 0, 0,  // 16
		xPos2[0], yPos2[1], zPos2[0], 4, 0,   // 17
		xPos2[0], yPos2[1], zPos2[1], 4, 2,   // 18
		xPos2[1], yPos2[1], zPos2[1], 0, 2,  // 19

		//// bottom
		xPos2[0], yPos2[0], zPos2[1], 0, 0,  // 20
		xPos2[1], yPos2[0], zPos2[1], 4, 0,   // 21
		xPos2[1], yPos2[0], zPos2[0], 4, 2,   // 22
		xPos2[0], yPos2[0], zPos2[0], 0, 2,  // 23
		//bawah ================================
		// front
		xPos3[0], yPos3[0], zPos3[0], 0, 0,  // 0
		xPos3[1], yPos3[0], zPos3[0], 4, 0,   // 1
		xPos3[1], yPos3[1], zPos3[0], 4, 2,   // 2
		xPos3[0], yPos3[1], zPos3[0], 0, 2,  // 3

		// right
		xPos3[1], yPos3[0], zPos3[0], 0, 0,  // 4
		xPos3[1], yPos3[0], zPos3[1], 4, 0,  // 5
		xPos3[1], yPos3[1], zPos3[1], 4, 2,  // 6
		xPos3[1], yPos3[1], zPos3[0], 0, 2,  // 7

		// back
		xPos3[0], yPos3[0], zPos3[1], 0, 0, // 8 
		xPos3[1], yPos3[0], zPos3[1], 4, 0, // 9
		xPos3[1], yPos3[1], zPos3[1], 4, 2, // 10
		xPos3[0], yPos3[1], zPos3[1], 0, 2, // 11

		// left
		xPos3[0], yPos3[0], zPos3[1], 0, 0, // 12
		xPos3[0], yPos3[0], zPos3[0], 4, 0, // 13
		xPos3[0], yPos3[1], zPos3[0], 4, 2, // 14
		xPos3[0], yPos3[1], zPos3[1], 0, 2, // 15

		//// upper
		xPos3[1], yPos3[1], zPos3[0], 0, 0,  // 16
		xPos3[0], yPos3[1], zPos3[0], 4, 0,   // 17
		xPos3[0], yPos3[1], zPos3[1], 4, 2,   // 18
		xPos3[1], yPos3[1], zPos3[1], 0, 2,  // 19

		//// bottom
		xPos3[0], yPos3[0], zPos3[1], 0, 0,  // 20
		xPos3[1], yPos3[0], zPos3[1], 4, 0,   // 21
		xPos3[1], yPos3[0], zPos3[0], 4, 2,   // 22
		xPos3[0], yPos3[0], zPos3[0], 0, 2,  // 23
	};

	unsigned int indices[] = {
		//bawah
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
		//bawah
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
		//bawah
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
	};

	int shapeCounter = 3;
	int indices_size = (36 * shapeCounter);

	for (int i = 1; i < shapeCounter; i++)
	{
		for (int j = (36 * i); j < indices_size; j++) {
			indices[j] += 24;
		};
	}

	glGenVertexArrays(1, &VAO10);
	glGenBuffers(1, &VBO10);
	glGenBuffers(1, &EBO10);
	glBindVertexArray(VAO10);
	glBindBuffer(GL_ARRAY_BUFFER, VBO10);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO10);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	usualGLstuffs();
}

void Demo::DrawSpinner() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, spinner);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO10); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 5, 0));
	model = glm::rotate(model, angle, glm::vec3(0, 1, 0));


	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, (36 * 3), GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildStair() {
	// Load and create a texture 
	glGenTextures(1, &stair);
	glBindTexture(GL_TEXTURE_2D, stair);

	glTexInit();

	int width, height;
	unsigned char* image = SOIL_load_image("brick.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float xPos[] = {
		-0.5, 1.5
	};

	float yPos[] = {
		-0.5, 0, //pendek depan
		-0.5, 0.5 //tinggi belakang
	};

	float zPos[] = {
		1.5, 1, //pendek depan
		1, 0.5 //tinggi belakang
	};

	// Build geometry
	float vertices[] = {
		//pendek depan================================
		// front
		xPos[0], yPos[0], zPos[0], 0, 0,  // 0
		xPos[1], yPos[0], zPos[0], 2, 0,   // 1
		xPos[1], yPos[1], zPos[0], 2, 0.5,   // 2
		xPos[0], yPos[1], zPos[0], 0, 0.5,  // 3

		// right
		xPos[1], yPos[0], zPos[0], 0.5, 0,  // 4
		xPos[1], yPos[0], zPos[1], 0, 0,  // 5
		xPos[1], yPos[1], zPos[1], 0, 0.5,  // 6
		xPos[1], yPos[1], zPos[0], 0.5, 0.5,  // 7

		// back
		xPos[0], yPos[0], zPos[1], 0, 0, // 8 
		xPos[1], yPos[0], zPos[1], 2, 0, // 9
		xPos[1], yPos[1], zPos[1], 2, 0.5, // 10
		xPos[0], yPos[1], zPos[1], 0, 0.5, // 11

		// left
		xPos[0], yPos[0], zPos[1], 0.5, 0, // 12
		xPos[0], yPos[0], zPos[0], 0, 0, // 13
		xPos[0], yPos[1], zPos[0], 0, 0.5, // 14
		xPos[0], yPos[1], zPos[1], 0.5, 0.5, // 15

		//// upper
		xPos[1], yPos[1], zPos[0], 0, 0,  // 16
		xPos[0], yPos[1], zPos[0], 2, 0,   // 17
		xPos[0], yPos[1], zPos[1], 2, 0.5,   // 18
		xPos[1], yPos[1], zPos[1], 0, 0.5,  // 19

		//// bottom
		xPos[0], yPos[0], zPos[1], 0, 0,  // 20
		xPos[1], yPos[0], zPos[1], 2, 0,   // 21
		xPos[1], yPos[0], zPos[0], 2, 0.5,   // 22
		xPos[0], yPos[0], zPos[0], 0, 0.5,  // 23
		//belakang tinggi================================
		// front
		xPos[0], yPos[2], zPos[2], 0, 0,  // 0
		xPos[1], yPos[2], zPos[2], 2, 0,   // 1
		xPos[1], yPos[3], zPos[2], 2, 1,   // 2
		xPos[0], yPos[3], zPos[2], 0, 1,  // 3

		// right
		xPos[1], yPos[2], zPos[2], 0, 0,  // 4
		xPos[1], yPos[2], zPos[3], 0.5, 0,  // 5
		xPos[1], yPos[3], zPos[3], 0.5, 1,  // 6
		xPos[1], yPos[3], zPos[2], 0, 1,  // 7

		// back
		xPos[0], yPos[2], zPos[3], 0, 0, // 8 
		xPos[1], yPos[2], zPos[3], 2, 0, // 9
		xPos[1], yPos[3], zPos[3], 2, 1, // 10
		xPos[0], yPos[3], zPos[3], 0, 1, // 11

		// left
		xPos[0], yPos[2], zPos[3], 0, 0, // 12
		xPos[0], yPos[2], zPos[2], 0.5, 0, // 13
		xPos[0], yPos[3], zPos[2], 0.5, 1, // 14
		xPos[0], yPos[3], zPos[3], 0, 1, // 15

		//// upper
		xPos[1], yPos[3], zPos[2], 0, 0,  // 16
		xPos[0], yPos[3], zPos[2], 2, 0,   // 17
		xPos[0], yPos[3], zPos[3], 2, 0.5,   // 18
		xPos[1], yPos[3], zPos[3], 0, 0.5,  // 19

		//// bottom
		xPos[0], yPos[2], zPos[3], 0, 0,  // 20
		xPos[1], yPos[2], zPos[3], 2, 0,   // 21
		xPos[1], yPos[2], zPos[2], 2, 0.5,   // 22
		xPos[0], yPos[2], zPos[2], 0, 0.5,  // 23
	};

	unsigned int indices[] = {
		//bawah
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
		//atas
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
	};

	int shapeCounter = 2;
	int indices_size = (36 * shapeCounter);

	for (int i = 1; i < shapeCounter; i++)
	{
		for (int j = (36 * i); j < indices_size; j++) {
			indices[j] += 24;
		};
	}

	glGenVertexArrays(1, &VAO8);
	glGenBuffers(1, &VBO8);
	glGenBuffers(1, &EBO8);
	glBindVertexArray(VAO8);
	glBindBuffer(GL_ARRAY_BUFFER, VBO8);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO8);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	usualGLstuffs();
}

void Demo::DrawStair() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, stair);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO8); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, (36 * 2), GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildLeaves() {
	// Load and create a texture 
	glGenTextures(1, &leaves);
	glBindTexture(GL_TEXTURE_2D, leaves);

	glTexInit();

	int width, height;
	unsigned char* image = SOIL_load_image("leaves.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float xPos[] = {
		-25.0, 25.0
	};

	float yPos[] = {
		-0.5, 3.5
	};

	float zPos[] = {
		-8.5, -9.5
	};

	// Build geometry
	float vertices[] = {
		//bawah ================================
		// front
		xPos[0], yPos[0], zPos[0], 0, 0,  // 0
		xPos[1], yPos[0], zPos[0], 50, 0,   // 1
		xPos[1], yPos[1], zPos[0], 50, 3,   // 2
		xPos[0], yPos[1], zPos[0], 0, 3,  // 3

		// right
		xPos[1], yPos[0], zPos[0], 0, 0,  // 4
		xPos[1], yPos[0], zPos[1], 1, 0,  // 5
		xPos[1], yPos[1], zPos[1], 1, 4,  // 6
		xPos[1], yPos[1], zPos[0], 0, 4,  // 7

		// back
		xPos[0], yPos[0], zPos[1], 0, 0, // 8 
		xPos[1], yPos[0], zPos[1], 50, 0, // 9
		xPos[1], yPos[1], zPos[1], 50, 3, // 10
		xPos[0], yPos[1], zPos[1], 0, 3, // 11

		// left
		xPos[0], yPos[0], zPos[1], 0, 0, // 12
		xPos[0], yPos[0], zPos[0], 1, 0, // 13
		xPos[0], yPos[1], zPos[0], 1, 4, // 14
		xPos[0], yPos[1], zPos[1], 0, 4, // 15

		//// upper
		xPos[1], yPos[1], zPos[0], 0, 0,  // 16
		xPos[0], yPos[1], zPos[0], 50, 0,   // 17
		xPos[0], yPos[1], zPos[1], 50, 1,   // 18
		xPos[1], yPos[1], zPos[1], 0, 1,  // 19

		//// bottom
		xPos[0], yPos[0], zPos[1], 0, 0,  // 20
		xPos[1], yPos[0], zPos[1], 50, 0,   // 21
		xPos[1], yPos[0], zPos[0], 50, 1,   // 22
		xPos[0], yPos[0], zPos[0], 0, 1,  // 23
	};

	unsigned int indices[] = {
		//belakang
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22, // bottom
	};

	int shapeCounter = 1;
	int indices_size = (36 * shapeCounter);

	for (int i = 1; i < shapeCounter; i++)
	{
		for (int j = (36 * i); j < indices_size; j++) {
			indices[j] += 24;
		};
	}

	glGenVertexArrays(1, &VAO9);
	glGenBuffers(1, &VBO9);
	glGenBuffers(1, &EBO9);
	glBindVertexArray(VAO9);
	glBindBuffer(GL_ARRAY_BUFFER, VBO9);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO9);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	usualGLstuffs();
}

void Demo::DrawLeaves() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, leaves);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO9); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, (36), GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::InitCamera()
{
	posCamX = 0.0f;
	posCamY = 1.0f;
	posCamZ = 8.0f;
	viewCamX = 0.0f;
	viewCamY = 1.0f;
	viewCamZ = 0.0f;
	upCamX = 0.0f;
	upCamY = 1.0f;
	upCamZ = 0.0f;
	CAMERA_SPEED = 0.001f;
	fovy = 45.0f;
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


void Demo::MoveCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	// forward positive cameraspeed and backward negative -cameraspeed.
	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;
}

void Demo::StrafeCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	float orthoX = -z;
	float orthoZ = x;

	// left positive cameraspeed and right negative -cameraspeed.
	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}

void Demo::RotateCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}


int main(int argc, char** argv) {
	RenderEngine& app = Demo();
	app.Start("Kelompok 5 - Project Akhir Grafika Komputer dan Visualisasi", 800, 600, false, false);
}