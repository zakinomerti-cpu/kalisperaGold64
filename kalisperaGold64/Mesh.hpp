#ifndef MESHCLASS_H
#define MESHCLASS_H

class Mesh {
public:
	Mesh() {
		x = 0; y = 0; z = 0;
		rx = 0; ry = 0; rz = 0;
		sx = 1; sy = 1; sz = 1;
	}
	virtual void render() = 0;
	virtual void rotate(float ix, float iy, float iz);
	virtual void setScale(float ix, float iy, float iz);
	virtual void setPosition(float ix, float iy, float iz);

protected:
	virtual void draw() = 0;
	float x, y, z;
	float sx, sy, sz;
	float rx, ry, rz;
};

#endif