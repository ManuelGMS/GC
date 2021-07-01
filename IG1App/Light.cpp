#include "Light.h"
#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>

//-----------------------------------------------------------------------------------------

// Atributo para poder generar un nuevo id cada vez.
GLuint Light::cont = 0;

// Creamos la luz, dándole un id y habilitándola.
Light::Light() {
	if (cont < GL_MAX_LIGHTS) {
		id = GL_LIGHT0 + cont;
		++cont;
		// glEnable(id);
	}
};

// Transfiere las características de la luz a la GPU.
void Light::upLoadL() const {
	glLightfv(id, GL_AMBIENT, value_ptr(ambient));
	glLightfv(id, GL_DIFFUSE, value_ptr(diffuse));
	glLightfv(id, GL_SPECULAR, value_ptr(specular));
}

// Deshabilita una luz concreta.
void Light::disable() { if (id < GL_LIGHT0 + GL_MAX_LIGHTS) glDisable(id); };

// Habilita una luz concreta.
void Light::enable() { if (id < GL_LIGHT0 + GL_MAX_LIGHTS) glEnable(id); };

// Establece la componente de luz ambiente.
void Light::setAmb(glm::fvec4 amb) { ambient = amb; upLoadL(); };

// Establece la componente de luz de difusión.
void Light::setDiff(glm::fvec4 diff) { diffuse = diff; upLoadL(); };

// Establece la componente de luz especular.
void Light::setSpec(glm::fvec4 spec) { specular = spec; upLoadL(); };

//-----------------------------------------------------------------------------------------

void DirLight::upload(glm::dmat4 const& modelViewMat) const {
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(modelViewMat));
	glLightfv(id, GL_POSITION, value_ptr(posDir));
	upLoadL();
}

// Ojo al 0.0 que determina que la luz sea remota.
void DirLight::setPosDir(glm::fvec3 dir) {
	posDir = glm::fvec4(dir, 0.0);
}

//-----------------------------------------------------------------------------------------

void PosLight::upload(glm::dmat4 const& modelViewMat) const {
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(modelViewMat));
	glLightfv(id, GL_POSITION, value_ptr(posDir));
	glLightf(id, GL_CONSTANT_ATTENUATION, kc);
	glLightf(id, GL_LINEAR_ATTENUATION, kl);
	glLightf(id, GL_QUADRATIC_ATTENUATION, kq);
	upLoadL();
}

glm::fvec3 PosLight::getPosDir()
{
	return this->posDir;
}

// Ojo al 1.0: determina que la luz sea local (punto donde se ubica).
void PosLight::setPosDir(glm::fvec3 dir) {
	posDir = glm::fvec4(dir, 1.0);
}

void PosLight::setAtte(GLfloat kc, GLfloat kl, GLfloat kq)
{
	this->kc = kc;
	this->kl = kl;
	this->kq = kq;
}

//-----------------------------------------------------------------------------------------

void SpotLight::upload(glm::dmat4 const& modelViewMat) const {
	PosLight::upload(modelViewMat);
	glLightfv(id, GL_SPOT_DIRECTION, value_ptr(direction));
	glLightf(id, GL_SPOT_CUTOFF, cutoff);
	glLightf(id, GL_SPOT_EXPONENT, exp);
}

// Ojo al 0.0: la dirección de emisión del foco es un vector.
void SpotLight::setSpot(glm::fvec3 dir, GLfloat cf, GLfloat e) {
	direction = glm::fvec4(dir, 0.0);
	cutoff = cf;
	exp = e;
}

//-----------------------------------------------------------------------------------------

void Material::upload() {
	glMaterialfv(face, GL_AMBIENT, value_ptr(ambient));
	glMaterialfv(face, GL_DIFFUSE, value_ptr(diffuse));
	glMaterialfv(face, GL_SPECULAR, value_ptr(specular));
	glMaterialf(face, GL_SHININESS, expF);
	glShadeModel(sh);
	// glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE); // Defecto
}

// Material -> Cobre.
void Material::setCopper() {
	ambient = { 0.19125, 0.0735, 0.0225, 1 };
	diffuse = { 0.7038, 0.27048, 0.0828, 1 };
	specular = { 0.256777, 0.137622, 0.086014, 1 };
	expF = 12.8f;
}

// Material -> Latón.
void Material::setBrass() {
	ambient = { 0.329412, 0.223529, 0.027451, 1 };
	diffuse = { 0.780392, 0.568627, 0.113725, 1 };
	specular = { 0.992157, 0.941176, 0.807843, 1 };
	expF = 27.8974f;
}

//-----------------------------------------------------------------------------------------
