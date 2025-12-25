#include "GlutRenderer.hpp"
#include "Entity.hpp"
#include "Cube.hpp"
#include "Plane2D.hpp"
#include "Plane.hpp"
#include "GL/glew.h"
#include "Texture.hpp"
#include "Scene.hpp"
#include "windows.h"
#include <random>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl2.h"

float spx = 1.0f;
float spy = 1.0f;
float spz = 1.0f;

float spsize = 1.0f;

Scene* scn;

float b1 = 0.0f;
float b2 = 0.0f;

float b1_ispressed = false;
float b2_ispressed = b1_ispressed;
bool cust = false;

const char* my_vs =
"#version 120\n"
"varying vec2 texCoord;\n"
"\n"
"void main() {\n"
"    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"    gl_FrontColor = gl_Color;\n"
"    // Автоматически берет текстурные координаты из FFP\n"
"    texCoord = gl_MultiTexCoord0.st;\n"
"}";

const char* my_fs =
"#version 120\n"
"varying vec2 texCoord;\n"
"uniform sampler2D tex;\n"
"\n"
"void main() {\n"
"    vec4 texColor = texture2D(tex, texCoord);\n"
"    \n"
"    // Если цвет белый (или близкий к белому), отбрасываем пиксель\n"
"    if (texColor.r > 0.95 && texColor.g > 0.95 && texColor.b > 0.95) {\n"
"        discard;  // Отбрасываем пиксель\n"
"    }\n"
"    \n"
"    // Комбинируем цвет текстуры с цветом вершины\n"
"    gl_FragColor = texColor * gl_Color;\n"
"}";

const char* my_fs2 =
"#version 120\n"
"varying vec2 texCoord;\n"
"uniform sampler2D tex;\n"
"\n"
"void main() {\n"
"    vec4 texColor = texture2D(tex, texCoord);\n"
"    \n"
"    // Если цвет белый (или близкий к белому), отбрасываем пиксель\n"
"    if (texColor.g > 0.99) {\n"
"        discard;  // Отбрасываем пиксель\n"
"    }\n"
"    \n"
"    // Комбинируем цвет текстуры с цветом вершины\n"
"    gl_FragColor = texColor * gl_Color;\n"
"}";

const char* my_fs3 =
"#version 120\n"
"varying vec2 texCoord;\n"
"uniform sampler2D tex;\n"
//"uniform vec2 rs;\n"
"uniform float rx;\n"
"uniform float spsize;\n"
"uniform float ry;\n"
"uniform float time;\n"
"uniform float b1;\n"
"uniform float b2;\n"
"\n"
"float random(in vec2 st) {\n"
"    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);\n"
"}\n"
"\n"
"float noise(in vec2 st) {\n"
"    vec2 i = floor(st);\n"
"    vec2 f = fract(st);\n"
"\n"
"    float a = random(i);\n"
"    float b = random(i + vec2(1.0, 0.0));\n"
"    float c = random(i + vec2(0.0, 1.0));\n"
"    float d = random(i + vec2(1.0, 1.0));\n"
"\n"
"    vec2 u = f * f * (3.0 - 2.0 * f);\n"
"\n"
"    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;\n"
"}\n"
"\n"
"#define OCTAVES 6\n"
"float fbm(in vec2 st) {\n"
"    float value = 0.0;\n"
"    float amplitude = 0.5;\n"
"    for (int i = 0; i < OCTAVES; i++) {\n"
"        value += amplitude * noise(st);\n"
"        st *= 2.0;\n"
"        amplitude *= 0.5;\n"
"    }\n"
"    return value;\n"
"}\n"
"\n"
"void main() {\n"
"    vec2 rs = vec2(rx, ry);\n"
"    vec2 fc = gl_FragCoord.xy;\n"
"    vec2 uv = vec2(fc.x / rs.x * (rs.x / rs.y) - 0.5 * (rs.x / rs.y), fc.y / rs.y - 0.5);\n"
"\n"
"    uv/=(spsize*0.5);\n"
"    float c = length(uv);\n"
"    c = step(c, 0.4);\n"
"    c *= fbm(vec2(uv.x, uv.y) * 3.0);\n"
"    \n"
"    vec3 color = vec3(0.2, 0.4, 0.1);\n"
"    if (c > 0.2 * sin(b2))\n"
"        color = vec3(0.5, 0.5, 0.1);\n"
"    if (c > 0.3 * sin(b1))\n"
"        color = vec3(0.1, 0.1, 0.4);\n"
"    if (c == 0.0)\n"
"        discard;\n"
"    \n"
"    color.rgb *= c * 4.0;\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}";

struct My_Camera {
	float x = 0; float y = 0; float z = 0;
	float xRot = 0; float yRot = 0; float zRot = 0;
};

My_Camera camera;
float timer = 0;

void update() {
	scn->getEntityByIndex(1019)->updateShaderFloatValue("spsize", spsize);
	scn->getEntityByIndex(1019)->updateShaderFloatValue("b1", b1);
	scn->getEntityByIndex(1019)->updateShaderFloatValue("b2", b2);
	scn->getEntityByIndex(1019)->updateShaderFloatValue("rx", 1600.0);
	scn->getEntityByIndex(1019)->updateShaderFloatValue("ry", 900.0);
	scn->getEntityByIndex(1019)->updateShaderFloatValue("time", timer);
	scn->renderScene();
	Sleep(16);
	timer += 0.01;
	// ===== Вычисляем направление камеры =====
	float yawRad = camera.yRot * 3.1415926f / 180.0f;
	float pitchRad = camera.xRot * 3.1415926f / 180.0f;

	// Вектор направления вперед
	float forwardX = cos(pitchRad) * sin(yawRad);
	float forwardY = sin(pitchRad);
	float forwardZ = cos(pitchRad) * cos(yawRad);

	// Нормализовать не обязательно, но можно
	float len = sqrt(forwardX * forwardX + forwardY * forwardY + forwardZ * forwardZ);
	forwardX /= len;
	forwardY /= len;
	forwardZ /= len;

	// Вектор вправо (кросс-продукт forward × up)
	float rightX = sin(yawRad - 3.1415926f / 2.0f);
	float rightZ = cos(yawRad - 3.1415926f / 2.0f);

	float speed = 0.1f;

	// ===== Движение вперёд/назад =====
	if (GetAsyncKeyState('W') & 0x8000) {
		camera.x += forwardX * speed;
		camera.y += forwardY * speed;
		camera.z += forwardZ * speed;
		scn->setCameraPos(camera.x, camera.y, camera.z);
	}

	if (GetAsyncKeyState('S') & 0x8000) {
		camera.x -= forwardX * speed;
		camera.y -= forwardY * speed;
		camera.z -= forwardZ * speed;
		scn->setCameraPos(camera.x, camera.y, camera.z);
	}

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
		scn->setCameraPos(camera.x, camera.y-=0.1, camera.z);
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		scn->setCameraPos(camera.x, camera.y+=0.1, camera.z);
	}

	// ===== Страфы влево/вправо =====
	if (GetAsyncKeyState('D') & 0x8000) {
		camera.x += rightX * speed;
		camera.z += rightZ * speed;
		scn->setCameraPos(camera.x, camera.y, camera.z);
	}

	if (GetAsyncKeyState('A') & 0x8000) {
		camera.x -= rightX * speed;
		camera.z -= rightZ * speed;
		scn->setCameraPos(camera.x, camera.y, camera.z);
	}

	// ===== Повороты камеры (как было) =====
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		scn->setCameraRot(camera.xRot, camera.yRot += 1.0f, camera.zRot);

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		scn->setCameraRot(camera.xRot, camera.yRot -= 1.0f, camera.zRot);

	if (GetAsyncKeyState(VK_UP) & 0x8000)
		scn->setCameraRot(camera.xRot += 1.0f, camera.yRot, camera.zRot);

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		scn->setCameraRot(camera.xRot -= 1.0f, camera.yRot, camera.zRot);


	glUseProgram(0);
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Test Window");

	if (ImGui::Button("spawn new vrag")) {
		Entity* vrag = new Entity("vrag", 1);
		vrag->setVertexShader(my_vs);
		vrag->setFragmentShader(my_fs2);
		vrag->shaderInit();
		vrag->setMesh(new Plane());
		vrag->setTexture(new Texture("C://Users//Xinitrix//Pictures//gtextures//vrag.bmp", vrag->getShaderProgram()));
		vrag->setPosition(-2.5f * (rand()%1000)*sin(timer)/1000.0f, -0.5f, -2.5f * (rand() % 1000) * cos(timer) / 1000.0f);
		vrag->setRotation(-90.0f, -90.0, 0.0f);
		scn->addEntity(vrag);
	}

	if (ImGui::Button("spawn custball 1")) {
		Entity* vrag = new Entity("vrag", 1);
		vrag->setVertexShader(my_vs);
		vrag->setFragmentShader(my_fs2);
		vrag->shaderInit();
		vrag->setMesh(new Plane());
		vrag->setTexture(new Texture("C://Users//Xinitrix//Pictures//gtextures//custbol1.bmp", vrag->getShaderProgram()));
		vrag->setPosition(camera.x, camera.y, camera.z);
		vrag->setScale(0.25, 0.25, 0.25);
		vrag->setRotation(-90.0f, -90.0, 0.0f);
		scn->addEntity(vrag);
	}

	if (ImGui::Button("spawn custball 2")) {
		Entity* vrag = new Entity("vrag", 1);
		vrag->setVertexShader(my_vs);
		vrag->setFragmentShader(my_fs2);
		vrag->shaderInit();
		vrag->setMesh(new Plane());
		vrag->setTexture(new Texture("C://Users//Xinitrix//Pictures//gtextures//custbol2.bmp", vrag->getShaderProgram()));
		vrag->setPosition(camera.x, camera.y, camera.z);
		vrag->setScale(0.25, 0.25, 0.25);
		vrag->setRotation(-90.0f, -90.0, 0.0f);
		scn->addEntity(vrag);
	}

	if (ImGui::Button("show menu")) {
		spx = 1.0f;
		spy = 1.0f;
		spz = 1.0f;
		spsize = 1.0f;

		b1 = 0.0f;
		b2 = 0.0f;

		b1_ispressed = false;
		b2_ispressed = b1_ispressed;
		cust = false;

		scn->getEntityByIndex(1019)->setScale(1.0f, 1.0f, 1.0f);
	}

	if (ImGui::Button("add ryne number one")) {
		b1_ispressed = true;
	}

	if (ImGui::Button("add ryne number two")) {
		b2_ispressed = true;
	}
	if (b1_ispressed) b1 += (b1 >= 1.0f) ? 0.0f : 0.01f;
	if (b2_ispressed) b2 += (b2 >= 1.0f) ? 0.0f : 0.01f;

	if (ImGui::Button("cust")) {
		cust = true;
	}

	if (cust) {
		scn->getEntityByIndex(1019)->setScale(spx -= (spx <= 0.0f) ? 0.0f : 0.1f, spy -= (spy <= 0.0f) ? 0.0f : 0.1f, spz);
		scn->getEntityByIndex(1019)->updateShaderFloatValue("spsize", spsize-= (spsize == 0.0f) ? 0.0f : 0.1f);
	}



	scn->getEntity("vrag")->setPosition(sin(timer) - 2.5f, -0.5f, cos(timer) - 2.5f);

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

}

void start() {
	srand(time(NULL));
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	scn = new Scene(1024);
	for (float i = -5.0f; i < 1; i+=1.0f) {
		for (float j = -5.0f; j < 1; j+=1.0f) {
			float tile_size = 1.0f;
			std::string name = "plane" + std::to_string((int)abs(j*i));
			Entity* ent = new Entity(name);
			ent->setMesh(new Cube());
			ent->setScale(1* tile_size, 0.001, 1* tile_size);
			ent->setPosition(1.0f*i* tile_size, -1, 1*j* tile_size);
			ent->setTexture(new Texture("C://Users//Xinitrix//Pictures//gtextures//grass.bmp", ent->getShaderProgram()));
			scn->addEntity(ent);
		}
	}

	Entity* ent2 = new Entity("brick");
	ent2->setMesh(new Cube());
	ent2->setPosition(-3, -1, -3);
	ent2->setScale(1, 0.002, 1);
	ent2->setTexture(new Texture("C://Users//Xinitrix//Pictures//gtextures//grass_lighted.bmp", ent2->getShaderProgram()));
	scn->addEntity(ent2);


	Entity* entInterface0 = new Entity("InterFace", 1);
	entInterface0->setVertexShader(my_vs);
	entInterface0->setFragmentShader(my_fs);
	entInterface0->shaderInit();
	entInterface0->setMesh(new Plane2D());
	entInterface0->setTexture(new Texture("C://Users//Xinitrix//Pictures//gtextures//rukaR.bmp", entInterface0->getShaderProgram()));
	entInterface0->setPosition(-0.5f, -.25f, 0.0f);
	scn->addEntityToIndex(entInterface0, 1020);

	Entity* entInterface = new Entity("InterFace", 1);
	entInterface->setVertexShader(my_vs);
	entInterface->setFragmentShader(my_fs);
	entInterface->shaderInit();
	entInterface->setMesh(new Plane2D());
	entInterface->setTexture(new Texture("C://Users//Xinitrix//Pictures//gtextures//rukaL.bmp", entInterface->getShaderProgram()));
	entInterface->setPosition(0.5f, -.25f, 0.0f);
	scn->addEntityToIndex(entInterface, 1021);

	Entity* entInterface2 = new Entity("InterFace2", 1);
	entInterface2->setVertexShader(my_vs);
	entInterface2->setFragmentShader(my_fs);
	entInterface2->shaderInit();
	entInterface2->setMesh(new Plane2D());
	entInterface2->setTexture(new Texture("C://Users//Xinitrix//Pictures//gtextures//interface_left.bmp", entInterface2->getShaderProgram()));
	entInterface2->setPosition(-0.5f, -.5f, 0.0f);
	scn->addEntityToIndex(entInterface2, 1022);

	Entity* entInterface3 = new Entity("InterFace2", 1);
	entInterface3->setVertexShader(my_vs);
	entInterface3->setFragmentShader(my_fs);
	entInterface3->shaderInit();
	entInterface3->setMesh(new Plane2D());
	entInterface3->setTexture(new Texture("C://Users//Xinitrix//Pictures//gtextures//interface_right.bmp", entInterface3->getShaderProgram()));
	entInterface3->setPosition(0.5f, -.5f, 0.0f);
	scn->addEntityToIndex(entInterface3, 1023);

	Entity* vrag = new Entity("vrag", 1);
	vrag->setVertexShader(my_vs);
	vrag->setFragmentShader(my_fs2);
	vrag->shaderInit();
	vrag->setMesh(new Plane());
	vrag->setTexture(new Texture("C://Users//Xinitrix//Pictures//gtextures//vrag.bmp", vrag->getShaderProgram()));
	vrag->setPosition(-2.5f, -0.5f, -2.5f);
	vrag->setRotation(-90.0f, -90.0, 0.0f);
	scn->addEntity(vrag);

	Entity* entInterfaceShpere = new Entity("entInterfaceShpere", 1);
	entInterfaceShpere->setVertexShader(my_vs);
	entInterfaceShpere->setFragmentShader(my_fs3);
	entInterfaceShpere->shaderInit();
	entInterfaceShpere->setMesh(new Plane2D());
	entInterfaceShpere->setTexture(new Texture("C://Users//Xinitrix//Pictures//gtextures//vrag.bmp", entInterfaceShpere->getShaderProgram()));
	scn->addEntityToIndex(entInterfaceShpere, 1019);

}



int main() {
	
	Renderer* renderer = new GLUTRenderer();
	renderer->createWindow("window", 1600, 900);
	renderer->setUpdateFunc(update);
	renderer->setStartFunc(start);
	renderer->init();

	while (!renderer->shouldClose()) {
		renderer->render();
	}
	return 0;
}