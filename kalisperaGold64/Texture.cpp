#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"
#include "GL/glew.h"
#include "iostream"

Texture::Texture(const char* path, GLuint pr) {
	sp = pr;
	texID = 0;
	uniformLoc = -1;
	ready = false;

	int w, h, n;
	unsigned char* data = stbi_load(path, &w, &h, &n, 0);
	if (!data) {
		std::cerr << "Texture " << path << " not found\n";
		exit(-1);
	}

	GLenum format = (n == 4) ? GL_RGBA : GL_RGB;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Получаем location uniform переменной
    glUseProgram(sp);
    uniformLoc = glGetUniformLocation(sp, "tex");
    if (uniformLoc != -1) {
        glUniform1i(uniformLoc, 0); // sampler2D -> GL_TEXTURE0
    }
    glUseProgram(0);

    ready = true;
}

Texture::~Texture() {
    if (texID != 0) {
        glDeleteTextures(1, &texID);
    }
}

void Texture::bind() {
    if (!ready || uniformLoc == -1) return;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    // uniform уже установлен в конструкторе
}