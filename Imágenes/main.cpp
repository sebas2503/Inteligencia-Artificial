#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <random>

#include "AlgoritmoGenetico.h"


#define ciudades 10
int generaciones = 1000;

unsigned int VAO, VBO;
float vertex[ciudades * 3];
int individuos = 20;

TSP tsp(ciudades, individuos);

using namespace std;


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glad_glViewport(0, 0, width, height);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


int main() {




	//Initialice and Setup
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Creating a window
	GLFWwindow* window = glfwCreateWindow(980, 980, "Problema del Agente Viajero", NULL, NULL);
	//Parameters: width, height, title, monitor for full screen, share resources with window
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();//Destruye ventanas y limpia recursos
		return -1;
	}
	glfwMakeContextCurrent(window);//Hace actual el contexto de la ventana

	//Load glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	glViewport(40, 40, 900, 900);
	//location of lower left corner, width and height

	//Identifica cuando la ventana se cambia de tamanio
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetKeyCallback(window, key_callback);

	// Shader Vertex
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"out vec4 vertexColor; // specify a color output to the fragment shader\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos, 1.0);\n"
		"   vertexColor  = vec4(0.5, 0.0, 0.0, 1.0);\n"
		"}\0";
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Fragment Shader
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 ourColor; // we set this variable in the OpenGL code.\n"
		"void main()\n"
		"{\n"
		"   FragColor = ourColor;\n"
		"}\0";
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	//SHADER PROGRAM
	//Program Object
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	//Attach
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//Verify
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}


	//glUseProgram(shaderProgram);//Activar el programa creado
	//Unlink shader objects when they're already linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	
	
	

	tsp.CrearPoblacion();
	//tsp.printGrafo();
	tsp.LinkCiudades();
	//tsp.printdistancias();
	tsp.generarPoblacion();
	//tsp.printPoblacion();
	tsp.mejor_camino();

	tsp.seleccion();
	tsp.vertices(vertex);
	//tsp.mejor_camino();

	
	//BUFFER
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//Tipo de buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);//Copia la data al buffer

	//****
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//*****

	//float timeValue = glfwGetTime();
	//float value = (sin(timeValue) / 2.0f) + 0.5f;

	//PARAMS
	glPointSize(20);
	glLineWidth(5);

	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
	float colorCity[] = { 0.0f, 1.0f, 0.0f };
	float colorLine[] = { 0.5f, 0.0f, 0.5f };

	//Render Loop
	while (!glfwWindowShouldClose(window)) {//Check if GLFW has been instructed to close


		//Clear window
		glClearColor(0.87f, 0.87f, 0.87f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//RENDERING COMANDS HERE
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		glUseProgram(shaderProgram);
		glUniform4f(vertexColorLocation, colorLine[0], colorLine[1], colorLine[2], 1.0f);
		glDrawArrays(GL_LINE_LOOP, 0, ciudades);

		glUseProgram(shaderProgram);
		glUniform4f(vertexColorLocation, colorCity[0], colorCity[1], colorCity[2], 1.0f);
		glDrawArrays(GL_POINTS, 0, ciudades);


		glfwSwapBuffers(window);//swap the color buffer
		glfwPollEvents();//Check events Triggered (keyboard o mouse)
		// calls the corresponding functions registered by callback methods
	}

	glfwTerminate();
	return 0;
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		if(generaciones) {
			
			tsp.seleccion();
			tsp.mejor_camino();
			tsp.mejor(vertex);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_DYNAMIC_DRAW);//Copia la data al buffer
			generaciones--;
			if (!generaciones) cout << "Se ejecutaron todas las generaciones" << endl;
		}
	}
}