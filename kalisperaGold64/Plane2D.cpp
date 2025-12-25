#include "Plane2D.hpp"
#include "GL/glew.h"

void Plane2D::render() {
    glPushAttrib(GL_ENABLE_BIT | GL_TRANSFORM_BIT); 

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glTranslatef(x, y, z);
    glRotatef(rx, 1, 0, 0);
    glRotatef(ry, 0, 1, 0);
    glRotatef(rz, 0, 0, 1);
    glScalef(sx, sy, sz);

    glDisable(GL_DEPTH_TEST);
    draw();
    glEnable(GL_DEPTH_TEST);

    glPopMatrix(); // projection
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glPopAttrib();
}


void Plane2D::draw() {
    glBegin(GL_TRIANGLES);

    // ѕередн€€ грань
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);

    glEnd();
}
