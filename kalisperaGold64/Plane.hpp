#ifndef PLANEHEADER_H
#define PLANEHEADER_H

#include "Mesh.hpp"

class Plane : public Mesh {
public:
	void render() override;
private:
	void draw() override;
};

#endif