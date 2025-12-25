#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include "Scene.hpp"

typedef struct Entity {
    int x = 0;
    int y = 0;
    char* name;
} Entity;

int main() {
    Scene* s = new Scene(1024);
    return 0;

}