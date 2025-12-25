#ifndef SCENECLASS_H
#define SCENECLASS_H

#include <vector>
#include <string>
#include <functional>
class Entity;

struct Camera {
	float x = 0; float y = 0; float z = 0;
	float xRot = 0; float yRot = 0; float zRot = 0;
};

class Scene {
public:
	Scene(size_t outerSize);
	~Scene();

    void addEntity(Entity* e);
	void addEntityToIndex(Entity* e, size_t index);
    Entity* getEntity(std::string iname);
	Entity* getEntityByIndex(size_t index);
    std::vector<Entity*>* getInnerArray(size_t index);

	void renderScene();
	void setCameraPos(float x, float y, float z);
	void setCameraRot(float x, float y, float z);
	
private:
    
    std::hash<std::string> strHacher;
    std::vector<std::vector<Entity*>*> data;
	struct Camera camera;
};

#endif