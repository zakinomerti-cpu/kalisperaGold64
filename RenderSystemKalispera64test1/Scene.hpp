#include <vector>
#include <functional>
#include <string>

class Entity;

class Scene {
private:
    std::hash<std::string> strHacher;
    std::vector<std::vector<Entity*>*> data;
public:
    Scene(size_t outerSize)

    ~Scene() {
        for (auto innerPtr : data) {
            if (innerPtr)
                delete innerPtr;
        }
    }

    std::vector<Entity*>* getInnerArray(size_t index) {
        if (!data[index])
            data[index] = new std::vector<Entity*>();
        return data[index];
    }

    void addEntity(Entity* e) {
        size_t index = strHacher(e->name) % data.size();
        getInnerArray(index)->push_back(e);
    }

    Entity* getEntity(const char* name) {
        size_t outerIndex = strHacher(name) % data.size();
        if (!data[outerIndex]) return nullptr;

        for (auto e : *data[outerIndex]) {
            if (e->name == name) return e;
        }
        return nullptr;
    }

    size_t outerSize() const { return data.size(); }
};