#ifndef IRENDERER_H
#define IRENDERER_H

class Scene;

typedef unsigned char uchar;

class Renderer {
public:
	Renderer() {}
	virtual void createWindow(const char* title, int width, int height) = 0;
	virtual void setKeyboardFunc(void (*)(uchar, int, int)) = 0;
	virtual void setUpdateFunc(void (*)()) = 0;
	virtual void setStartFunc(void (*)()) = 0;
	virtual void* getWindow() = 0;
	virtual bool shouldClose() = 0;
	
	virtual void render() = 0;
	virtual void init() = 0;

	virtual void setScene(Scene*) = 0;

protected:
	Scene* scn_;
	bool ready_;
	void* window_;
	bool shouldClose_;
	static void (*update_)(void);
	static void (*keyboard_)(uchar, int, int);
	void (*start_)(void);
};

#endif