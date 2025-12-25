#include "Shader.hpp"
#include "GL/glew.h"
#include <iostream>

Shader::Shader() {

}

void Shader::createVertexShader(const char* src) {
	vsi = true;
	vshader = createShader(GL_VERTEX_SHADER, src);
}

void Shader::createFragmentShader(const char* src) {
	fsi = true;
	fshader = createShader(GL_FRAGMENT_SHADER, src);
}

bool Shader::isReady() {
	return isReadyVar;
}

void Shader::init() {
	if (fsi && vsi) {
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vshader);
		glAttachShader(shaderProgram, fshader);
		glLinkProgram(shaderProgram);

		glDeleteShader(vshader);
		glDeleteShader(fshader);
	}

	isReadyVar = true;
}

void Shader::use() {
	if((shaderProgram != 0) && (isReadyVar == true))
		glUseProgram(shaderProgram);
}

uint Shader::getShaderProgram() {
	return shaderProgram;
}

uint Shader::createShader(GLenum type, const char* src) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, 0);
	glCompileShader(shader);

	int success = 0;
	char log[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, log);
		std::cerr << log << std::endl;
		exit(0);
	}

	return shader;
}
