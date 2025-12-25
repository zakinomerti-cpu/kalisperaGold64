#ifndef TEXTURECLASS_H
#define TEXTURECLASS_H

typedef unsigned int uint;

class Texture {
public:
	Texture(const char* path, uint pr);
	~Texture();
	void bind();
	void unbind() {}
	bool isReady() const { return ready;  }
private:
	uint sp;
	uint texID;
	uint uniformLoc;
	bool ready = false;
};

#endif