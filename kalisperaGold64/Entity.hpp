#ifndef ENTITYCLASS_H
#define ENTITYCLASS_H

#include <string>
#include <vector>

struct shader_float{
	float var;
	const char* name;
};

class Texture;
class Shader;
class Mesh;

class Entity {
public:
	Entity(std::string, bool);
	Entity(std::string);

	void draw();
	void setScale(float x, float y, float z);
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setVertexShader(const char* shader);
	void setFragmentShader(const char* shader);

	void addValueToShaderSendQueue(const char* value_name, float value);
	void updateShaderFloatValue(const char* value_name, float value);
	void sendShaderFloats();
	void shaderInit();
	void setTexture(Texture*);
	void setMesh(Mesh*);

	void setUnvisible();
	void setVisible();

	void setShader(Shader*);
	std::string getName();
	unsigned int getShaderProgram();
private:
	void init();
	bool isVisible;
	bool useCustomShader;
	bool isReady;

	std::string entName;
	std::vector<shader_float> sendvars;
	Mesh* mesh;
	Shader* shd;
	Texture* tx;

	const char* vertexsh;
	const char* fragmentsh;
};

#endif