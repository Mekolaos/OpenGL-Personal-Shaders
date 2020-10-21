#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <math.h>
#include "Shader.h"
#include "stb_image.h"
#include "helper.h"
#include "include/glm/glm.hpp"
#include "include/glm/gtc/quaternion.hpp"
#include "include/glm/gtx/quaternion.hpp"
#include "include/imgui/imgui.h"
#include "include/imgui/imgui_impl_glfw.h"
#include "include/imgui/imgui_impl_opengl3.h"



int SCREEN_X = 1280;
int SCREEN_Y = 720;

static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);

void framebuffer_size_callback(GLFWwindow* window, int width, int height); 

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	SCREEN_X = width;
	SCREEN_Y = height;
	glViewport(0, 0, width, height);
} 

float mouse[2];
float side_input;
float forward_input;
float vertical_input;
bool showUI = false;

// Camera stuff
glm::vec3 cameraPos   = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 3.0f, 1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
float yaw   = 90.0f;
float pitch =  0.0f;
float lastX =  SCREEN_X / 2.0;
float lastY =  SCREEN_Y / 2.0;
// Cam sensivity
float sensitivity = 0.1f;
float fov   =  45.0f;
// To set values only
bool firstMouse = true;
float speed = .15f;

double mousePanX[2];
double mousePanY[2];
bool autoMode = false;

void process_input(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Closes the program.
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	// Activates the UI/Debug/Pause menu
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) showUI = !showUI;
	// Forward movement on Z axis only.
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){ cameraPos += speed * cameraFront;}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){ cameraPos -= speed * cameraFront;}
	else forward_input = 0;
	// Side movement on X axis only.
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){ cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;}
	else { side_input = 0;}
	// Vertical movement on Y axis only.
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) vertical_input = 1;
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) vertical_input = -1;
	else vertical_input = 0;
	// Trigger automatic forward movement for the camera. Works on z axis only.
	if(key == GLFW_KEY_C && action == GLFW_PRESS) autoMode = !autoMode;
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_X, SCREEN_Y, "Ray marching for fags", NULL, NULL);
	if (window == NULL){
		std::cout << "Couldn't initialize window" << std::endl;
		glfwTerminate();

		return -1;
	}
	glfwSetCursorPosCallback(window, cursorPositionCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 
	glfwSetKeyCallback(window, process_input);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
// imgui init -----------------------------------------
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();
// ----------------------------------------------------

	

	float vertices[] = {
        // positions          // colors           // texture coords
         1.0f,  1.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,    1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),(void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);



	
	glm::vec3 position = glm::vec3(0.0f, 1.0f, 0.0f);
	
	Shader myShader("shader.vert", "fragShader.frag");
	myShader.use();
	myShader.setFloat("time", glfwGetTime());
	myShader.setFloat("x", SCREEN_X);
	myShader.setFloat("y", SCREEN_Y);
	// myShader.setVec3("ro", position);
	position[1] = 2.f;
	float clear_color[3] = {1.0f, 0.0f, 0.0f};
	// std::cout <<  << std::endl;
	bool activator = 0;
	glm::quat rotato;
	// float radius = 10.0f;

	
	// Game loop my man
	while (!glfwWindowShouldClose(window)){
		// process_input(window);
		float timeValue = glfwGetTime();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


	// ImGui shit --------------------------------------------------
		if(showUI)
		{
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            static int counter = 0;
			bool show_demo_window = false;
			bool show_another_window = false;

            ImGui::Begin("Debug Window");                          

            ImGui::Text("REVIENS INESSSSS");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Activate weird lighting", &activator);      // Edit bools storing our window open/close state
            ImGui::SliderFloat("Speed", &speed, 0.0f, .5f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat("Cam Sensitivity", &sensitivity, 0.0f, 0.5f);
			// ImGui::SliderFloat3("tfou", (float*)&cameraFront,-25.0,25.0, "%.3f", 1.0f);
			
			ImGui::Text("x : %.3f | y : %.3f | z : %.3f", cameraFront[0], cameraFront[1], cameraFront[2]);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			if(ImGui::Button("Reset Camera Position")){
				position = {.0f, 1.0f, .0f};
			}
            ImGui::End();
        }
		else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// -------------------------------------------------------------

// weird shit
		
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)(SCREEN_X / SCREEN_Y), 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		view = view * projection;
		
// ?
		myShader.setInt("effectActivator", activator);
		myShader.setFloat("x", SCREEN_X);
		myShader.setFloat("y", SCREEN_Y);
		float movement[3] = {side_input * speed, vertical_input * .35f, forward_input * speed};
		for (int i = 0; i < 3; i++)
		{
			position[i] += movement[i];
		}
		if(autoMode)
		 position[2] += 1.0f * speed;

		glClearColor(0.f, .5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		
		// float mouseX = mouse[0];
		// float mouseY = mouse[1];
		myShader.setVec3("cunt", cameraFront);
		myShader.use();
		myShader.setVec3("ro", cameraPos);
		myShader.setFloat("time", timeValue);
		// myShader.setFloat("mouseX", mouseX);
		// myShader.setFloat("mouseY", mouseY);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
	glfwTerminate();
	
	return 0;
}

static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
	if(!showUI){
		if (firstMouse)
    	{
    	    lastX = xPos;
    	    lastY = yPos;
    	    firstMouse = false;
    	}

    	float xoffset = xPos - lastX;
    	float yoffset = lastY - yPos; // reversed since y-coordinates go from bottom to top
    	lastX = xPos;
    	lastY = yPos;

    	 // change this value to your liking
    	xoffset *= sensitivity;
    	yoffset *= sensitivity;

    	yaw += xoffset;
    	pitch += yoffset;

    	// make sure that when pitch is out of bounds, screen doesn't get flipped
    	if (pitch > 89.0f)
    	    pitch = 89.0f;
    	if (pitch < -45.0f)
    	    pitch = -45.0f;

    	glm::vec3 front;
    	front.x = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    	front.y = sin(glm::radians(pitch))+1.0f;
    	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    	cameraFront = glm::normalize(front);
		std::cout << "khtek : " << cameraFront.z << std::endl;
	}

}
