#include "Entity.hpp"
#include "GL/glew.h"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"

const char* standart_vs =
"#version 120\n"
"varying vec2 texCoord;\n"
"\n"
"void main() {\n"
"    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"    gl_FrontColor = gl_Color;\n"
"    // Автоматически берет текстурные координаты из FFP\n"
"    texCoord = gl_MultiTexCoord0.st;\n"
"}";

const char* standart_fs =
"#version 120\n"
"varying vec2 texCoord;\n"
"uniform sampler2D tex;\n"
"\n"
"void main() {\n"
"    vec4 texColor = texture2D(tex, texCoord);\n"
"    // Комбинируем цвет текстуры с цветом вершины\n"
"    gl_FragColor = texColor * gl_Color;\n"
"}";

Entity::Entity(std::string name, bool iuseCustomShader) {
	useCustomShader = iuseCustomShader;
	mesh = nullptr;
	shd = nullptr;
	tx = nullptr;

	sendvars.resize(0);

	vertexsh = nullptr;
	fragmentsh = nullptr;
	isReady = false;
	isVisible = true;

	entName = name;
	init();
}

Entity::Entity(std::string name) : useCustomShader(false) {
	mesh = nullptr;
	shd = nullptr;
	tx = nullptr;
	isVisible = true;
	
	entName = name;
	init();
}

void Entity::draw() {
	if (isVisible == true) {
		shd->use();
		sendShaderFloats();
		if (tx != nullptr)
			tx->bind();
		mesh->render();
	}
}

void Entity::setScale(float x, float y, float z) {
	mesh->setScale(x, y, z);
}

void Entity::setPosition(float x, float y, float z) {
	mesh->setPosition(x, y, z);
}

void Entity::setRotation(float x, float y, float z) {
	mesh->rotate(x, y, z);
}

void Entity::setVertexShader(const char* shader) {
	vertexsh = shader;
}

void Entity::setFragmentShader(const char* shader) {
	fragmentsh = shader;
}

void Entity::addValueToShaderSendQueue(const char* value_name, float value) {
	shader_float sf;
	sf.name = value_name;
	sf.var = value;
	sendvars.push_back(sf);
}

void Entity::updateShaderFloatValue(const char* value_name, float new_value)
{
	for (auto& sf : sendvars)
	{
		if (strcmp(sf.name, value_name) == 0)
		{
			sf.var = new_value;
			return;
		}
	}

	// Если значения нет — можно автоматически добавить
	shader_float sf;
	sf.name = value_name;
	sf.var = new_value;
	sendvars.push_back(sf);
}


void Entity::sendShaderFloats()
{
	if (!shd) return;

	GLuint program = shd->getShaderProgram(); // или твой метод getShaderProgram()

	for (auto& sf : sendvars)
	{
		GLint loc = glGetUniformLocation(program, sf.name);
		if (loc == -1) {
			// переменная не найдена в шейдере — возможно, оптимизировалась
			continue;
		}
		glUniform1f(loc, sf.var);
	}

	// можно очистить очередь (по желанию)
	// sendvars.clear();
}



void Entity::shaderInit() {
	shd = new Shader();
	if (shd != nullptr) {
		shd->createVertexShader(vertexsh);
		shd->createFragmentShader(fragmentsh);
		shd->init();
	}
}

void Entity::setTexture(Texture* t) {
	tx = t;
}

void Entity::setMesh(Mesh* m) {
	mesh = m;
}

void Entity::setUnvisible() {
	isVisible = false;
}

void Entity::setVisible() {
	isVisible = true;
}

void Entity::setShader(Shader* s) {
	shd = s;
}

std::string Entity::getName() {
	return entName;
}

unsigned int Entity::getShaderProgram() {
	return shd->getShaderProgram();
}

void Entity::init() {
	if (useCustomShader == true) {}
	else {
		shd = new Shader();
		if (shd != nullptr) {
			shd->createVertexShader(standart_vs);
			shd->createFragmentShader(standart_fs);
			shd->init();
		}
	}
}
