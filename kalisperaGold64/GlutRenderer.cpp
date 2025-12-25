#include "GlutRenderer.hpp"
#include "GL/glew.h"
#include "gl/freeglut.h"
#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl2.h"

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

// потому что статик
void (*Renderer::update_)(void) = nullptr;
void (*Renderer::keyboard_)(uchar, int, int) = nullptr;
static bool shouldCloseTrampoline = false;

GLUTRenderer::GLUTRenderer() {
	scn_ = nullptr;
	ready_ = false;

	start_ = nullptr;
	window_ = nullptr;

	int a = 1;
	glutInit(&a, nullptr);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitContextVersion(2, 1);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
}

void GLUTRenderer::createWindow(const char* title, int width, int height) {
	glutInitWindowSize(width, height);
	glutCreateWindow(title);
}

void GLUTRenderer::setKeyboardFunc(void(*func)(uchar, int, int)) {
	keyboard_ = func;
}

void GLUTRenderer::setUpdateFunc(void(*func)()) {
	update_ = func;
}

void GLUTRenderer::setStartFunc(void(*func)())  {
	start_ = func;
}

void* GLUTRenderer::getWindow() {
	return window_;
}

bool GLUTRenderer::shouldClose() {
	return shouldCloseTrampoline;
}

void GLUTRenderer::render() {
	if (shouldCloseTrampoline != true) {
		glutMainLoopEvent();
		glutPostRedisplay();
	}
}

void GLUTRenderer::init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();

	// Установим DisplaySize вручную
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	io.DisplaySize = ImVec2((float)width, (float)height);

	ImGui_ImplGLUT_Init();      // GLUT backend
	ImGui_ImplOpenGL2_Init();   // OpenGL2 backend
	ImGui_ImplGLUT_InstallFuncs();

	glutDisplayFunc(updateTrampoline);
	if (glewInit() != GLEW_OK) {
		std::cerr << "glewInit()" << std::endl;
		exit(-1);
	}
	if (start_ != nullptr)
		start_();
	glutKeyboardFunc(keyboard_);
	glutCloseFunc([]() {
		shouldCloseTrampoline = true;
	});

	glutReshapeFunc(reshape);

	glEnable(GL_DEPTH_TEST);
}

void GLUTRenderer::setScene(Scene* scn) {
}

void GLUTRenderer::updateTrampoline() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 1.0f, 0.1f, 100.f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (update_ != nullptr) {
		update_();
	}

	glutSwapBuffers();
}
