#ifndef GLUTRENDERER_H
#define GLUTRENDERER_H

#include "IRenderer.hpp"

class GLUTRenderer : public Renderer {
public:
	GLUTRenderer();
	void createWindow(const char* title, int width, int height) override;
	void setKeyboardFunc(void (*)(uchar, int, int)) override;
	void setUpdateFunc(void (*)()) override;
	void setStartFunc(void (*)()) override;
	void* getWindow() override;
	bool shouldClose() override;

	void render() override;
	void init() override;

	void setScene(Scene* scn) override;
private:
	static void updateTrampoline();
};

#endif