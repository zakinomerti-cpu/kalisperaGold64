#ifndef SHADERCLASS_H
#define SHADERCLASS_H

typedef unsigned int uint;

class Shader {
public:
	Shader();
	
	void createVertexShader(const char* src);
	void createFragmentShader(const char* src);
	bool isReady();
	void init();
	void use();

	uint getShaderProgram();
private:
	bool isReadyVar = false;
	bool vsi = false;
	bool fsi = false;

	uint shaderProgram = 0;
	uint vshader = 0;
	uint fshader = 0;
	uint createShader(uint type, const char* src);

};

#endif