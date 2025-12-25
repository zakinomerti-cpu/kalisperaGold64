#ifndef CUBEHEADER_H
#define CUBEHEADER_H

#include "Mesh.hpp"

class Cube : public Mesh {
public:
	void render() override;
private:
	void draw() override;
};

#endif