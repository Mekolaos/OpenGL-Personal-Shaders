#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>


const char *vertexShaderSource = "#version 330 core\n"
								 "layout (location = 0) in vec3 aPos;\n"
								 
								 "void main()\n"
								 "{\n"
    							 	"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
								 "}\n";

const char *fragmentShaderxSource = "#version 330 core\n"
								   "out vec4 FragColor;\n"
								   
								   "void main()\n"
								   "{\n"
								       "FragColor = vec4(0.0f, 0.6f, 0.8f, 1.0f);\n"
								   "}\n";
const char *fragmentShaderySource = "#version 330 core\n"
								   "out vec4 FragColor;\n"
								   
								   "void main()\n"
								   "{\n"
								       "FragColor = vec4(1.0f, 0.1f, 0.3f, 1.0f);\n"
								   "}\n";

void framebuffer_size_callback(GLFWwindow* window, int width, int height); 

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
} 

void process_input(GLFWwindow* window){
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Open fuck you", NULL, NULL);
	if (window == NULL){
		std::cout << "Couldn't initialize window" << std::endl;
		glfwTerminate();

		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShaderx;
	fragmentShaderx = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderx, 1, &fragmentShaderxSource, NULL);
	glCompileShader(fragmentShaderx);

    unsigned int fragmentShadery;
    fragmentShadery = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShadery, 1, &fragmentShaderySource, NULL);
    glCompileShader(fragmentShadery);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glGetShaderiv(fragmentShaderx, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(fragmentShaderx, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShaderx);
	glLinkProgram(shaderProgram);
	glDeleteShader(fragmentShaderx);

    unsigned int shaderProgramy = glCreateProgram();
    glAttachShader(shaderProgramy, vertexShader);
    glAttachShader(shaderProgramy, fragmentShadery);
    glLinkProgram(shaderProgramy);
    glDeleteShader(fragmentShadery);
	glDeleteShader(vertexShader);

	float vertices[] = {
		//  0.5f,  0.5f, 0.0f,
    	//  0.5f, -0.5f, 0.0f,
    	// -0.5f, -0.5f, 0.0f,
    	// -0.5f,  0.5f, 0.0f
         0.25f,  0.25f,  0.0f,
         0.25f, -0.25f,  0.0f,
         0.00f, -0.25f,  0.0f, //Fin du premier triangle
        -0.50f,  0.25f,  0.0f,
        -0.50f, -0.25f,  0.0f,
        -0.25f, -0.25f,  0.0f

	};

    float first_triangle[] = {
        0.25f,  0.25f,  0.0f,
        0.25f, -0.25f,  0.0f,
        0.00f, -0.25f,  0.0f
    };
    float second_triangle[] = {
        -0.50f,  0.25f,  0.0f,
        -0.50f, -0.25f,  0.0f,
        -0.25f, -0.25f,  0.0f
    };
	// unsigned int indices[] = {
	// 	0, 1, 3,
	// 	1, 2, 3
	// };

	unsigned int VBOs[2], VAOs[2]/*, EBO*/;
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
    
	// glGenBuffers(1, &EBO);
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(first_triangle), first_triangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(second_triangle), second_triangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	

	while (!glfwWindowShouldClose(window)){
		process_input(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgramy);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUseProgram(shaderProgram);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderProgramy);

	glfwTerminate();
	
	return 0;
}
