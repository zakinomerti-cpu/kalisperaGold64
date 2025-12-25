#include "Plane.hpp"
#include "GL/glew.h"

void Plane::render() {
    // Сохраняем ВСЁ состояние OpenGL
    glPushAttrib(GL_TRANSFORM_BIT | GL_ENABLE_BIT);
    glPushMatrix();

    // Получаем текущую матрицу вида (камеры) ДО наших изменений
    float originalModelview[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, originalModelview);

    // Сохраняем оригинальную проекционную матрицу
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    // Возвращаемся к видовой матрице
    glMatrixMode(GL_MODELVIEW);

    // Загружаем оригинальную матрицу вида
    glLoadMatrixf(originalModelview);

    // Применяем ТОЛЬКО перемещение нашего объекта
    glTranslatef(x, y, z);

    // Теперь получаем текущую матрицу вида (с учётом нашего перемещения)
    float modelview[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

    // "Обнуляем" повороты в матрице вида
    // Это делает объект всегда ориентированным к камере
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == j) {
                modelview[i * 4 + j] = 1.0f;
            }
            else {
                modelview[i * 4 + j] = 0.0f;
            }
        }
    }

    // Загружаем модифицированную матрицу
    glLoadMatrixf(modelview);

    // НЕ вызываем glTranslatef(x, y, z) снова - мы уже переместились!
    // Поворачиваем и масштабируем локально (относительно объекта)
    glRotatef(rx, 1, 0, 0);
    glRotatef(ry, 0, 1, 0);
    glRotatef(rz, 0, 0, 1);
    glScalef(sx, sy, sz);

    // Рисуем объект
    draw();

    // Восстанавливаем матрицу проекции
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    // Возвращаемся к видовой матрице
    glMatrixMode(GL_MODELVIEW);

    glPopMatrix();
    glPopAttrib();  // Восстанавливаем все состояния OpenGL
}

void Plane::draw() {
    glBegin(GL_TRIANGLES);
    // Нижняя грань
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);

    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glEnd();
}
