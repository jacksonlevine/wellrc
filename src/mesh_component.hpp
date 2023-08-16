#pragma once
#include "GL/glew.h"

struct MeshComponent {
public:
	GLuint vbov;
	GLuint vboc;
	GLuint vbouv;
	int length;
	MeshComponent();
};

#ifdef COMPONENT_IMPLEMENTATIONS

MeshComponent::MeshComponent() : length(0) {
	glGenBuffers(1, &this->vbov);
	glGenBuffers(1, &this->vboc);
	glGenBuffers(1, &this->vbouv);
}

#endif