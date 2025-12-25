#include "Mesh.hpp"
#include "GL/glew.h"

void Mesh::rotate(float ix, float iy, float iz) {
	rx = ix; ry = ix; rz = iz;
}

void Mesh::setScale(float ix, float iy, float iz) {
	sx = ix; sy = iy; sz = iz;
}

void Mesh::setPosition(float ix, float iy, float iz) {
	x = ix; y = iy; z = iz;
}
