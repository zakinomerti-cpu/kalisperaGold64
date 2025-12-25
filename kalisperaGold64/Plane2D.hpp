#ifndef PLANE2DHEADER_H
#define PLANE2DHEADER_H

#include "Mesh.hpp"

class Plane2D : public Mesh {
public:
	void render() override;
private:
	void draw() override;
};

#endif