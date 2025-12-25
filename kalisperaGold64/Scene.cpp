#include "Scene.hpp"
#include "Entity.hpp"
#include "GL/glew.h"

Scene::Scene(size_t outerSize) {
	data.resize(outerSize, nullptr);
}

Scene::~Scene() {
	for (auto innerPtr : data) {
		if (innerPtr)
			delete innerPtr;
	}
}

std::vector<Entity*>* Scene::getInnerArray(size_t index) {
	if (!data[index])
		data[index] = new std::vector<Entity*>();
	return data[index];
}

void Scene::addEntity(Entity* e) {
	size_t index = strHacher(e->getName()) % data.size();
	getInnerArray(index)->push_back(e);
}

void Scene::addEntityToIndex(Entity* e, size_t index) {
	if(index < data.size())
		getInnerArray(index)->push_back(e);
}

Entity* Scene::getEntity(std::string iname) {
	size_t outerIndex = strHacher(iname) % data.size();
	if (!data[outerIndex]) return nullptr;
	for (auto e : *data[outerIndex])
		if (e->getName() == iname) return e;
	return nullptr;
}

Entity* Scene::getEntityByIndex(size_t index) {
	return (*data[index])[0];
}

void Scene::renderScene() {
	for (size_t iter = 0; iter < data.size(); iter+=1) {
		if (data[iter] != nullptr) {
			for (auto e : *data[iter]) {
				glPushMatrix();
			

    // -----------------------------
    // 1. Вычисляем направление камеры
    // -----------------------------
    float yaw   = camera.yRot;   // вращение вокруг Y
    float pitch = camera.xRot;   // вращение вокруг X

    // конвертируем градусы → радианы
    float yawRad   = yaw   * 3.1415926f / 180.0f;
    float pitchRad = pitch * 3.1415926f / 180.0f;

    // вектор направления
    float dirX = cos(pitchRad) * sin(yawRad);
    float dirY = sin(pitchRad);
    float dirZ = cos(pitchRad) * cos(yawRad);

    // точка, куда смотрит камера
    float targetX = camera.x + dirX;
    float targetY = camera.y + dirY;
    float targetZ = camera.z + dirZ;

    // -----------------------------
    // 2. Формируем матрицу вида
    // -----------------------------
    gluLookAt(
        camera.x, camera.y, camera.z,      // позиция камеры
        targetX, targetY, targetZ,         // точка куда смотрим
        0.0f, 1.0f, 0.0f                   // вектор "вверх"
    );

				e->draw();
				glPopMatrix();
			}
		}
	}
}

void Scene::setCameraPos(float x, float y, float z) {
	camera.x = x;
	camera.y = y;
	camera.z = z;
}

void Scene::setCameraRot(float x, float y, float z) {
	camera.xRot = x;
	camera.yRot = y;
	camera.zRot = z;
}

