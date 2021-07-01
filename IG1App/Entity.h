//#pragma once
#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Mesh.h"
#include "Light.h"
#include "Texture.h"

//-------------------------------------------------------------------------

class Abs_Entity  // abstract class
{
public:

	Abs_Entity(): mModelMat(1.0), mColor(1) {};  // 4x4 identity matrix; Añadido mColor
	virtual ~Abs_Entity() {};

	Abs_Entity(const Abs_Entity& e) = delete;  // no copy constructor
	Abs_Entity& operator=(const Abs_Entity& e) = delete;  // no copy assignment

	virtual void render(glm::dmat4 const& modelViewMat) const = 0;  // abstract method
	virtual void update() {}; // Método update() con implementación vacía para que las subclases puedan redefinirlo.

	glm::dmat4 const& modelMat() const { return mModelMat; }; // modeling matrix
	void setModelMat(glm::dmat4 const& aMat) { mModelMat = aMat; };
	void setColor(glm::dvec4 color) { mColor = color; }; // Método para especificar el color.
	void setTexture(Texture* tex) { mTexture = tex; }; // Método para establecer la textura del objeto.
	
protected:

	Texture* mTexture = nullptr; // Textura del objeto.
	Mesh* mMesh = nullptr; // Malla del objeto.
	glm::dmat4 mModelMat; // modeling matrix
	glm::dvec4 mColor; // Atributo mColor para el color, inícialo a blanco en la constructora con mColor(1). 

	// transfers modelViewMat to the GPU
	virtual void upload(glm::dmat4 const& mModelViewMat) const;

};
//-------------------------------------------------------------------------

class EntityWithMaterial : public Abs_Entity {
public:
	EntityWithMaterial() { };
	virtual ~EntityWithMaterial() { delete material; };
	void setMaterial(Material* matl) { material = matl; };
protected:
	Material* material = nullptr;
};
//-------------------------------------------------------------------------

class EjesRGB : public Abs_Entity 
{
public:
	explicit EjesRGB(GLdouble l);
	~EjesRGB();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

//-------------------------------------------------------------------------

class Poligono : public Abs_Entity
{
public:
	explicit Poligono(GLint numL, GLdouble rd);
	~Poligono();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

//-------------------------------------------------------------------------

class Sierpinski : public Abs_Entity
{
public:
	explicit Sierpinski(GLuint numP, GLdouble rd);
	~Sierpinski();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

//-------------------------------------------------------------------------

class TrianguloRGB : public Abs_Entity
{
public:
	explicit TrianguloRGB(GLdouble rd);
	~TrianguloRGB();
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update(); // Redefinimos el método update.
protected:
	/*
	Añade a la clase TrianguloRGB dos atributos para los ángulos de giro, que se usarán para
	generar un desplazamiento del objeto describiendo una circunferencia, a la vez que gira
	sobre su centro.
	*/
	GLdouble anguloSobreSi;
	GLdouble anguloEnCirculo;
};

//-------------------------------------------------------------------------

class RectanguloRGB : public Abs_Entity
{
public:
	explicit RectanguloRGB(GLdouble w, GLdouble h);
	~RectanguloRGB();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

//-------------------------------------------------------------------------

class Estrella3D : public Abs_Entity
{
public:
	explicit Estrella3D(GLdouble re, GLuint np, GLdouble h);
	~Estrella3D();
	virtual void render(glm::dmat4 const& modelViewMat) const;
	// Redefine el método update para actualizar los ángulos y la matriz mModelMat.
	virtual void update();
	// Setter y Getter para controlar las coordenadas del objeto en la escena.
	void setCoordinates(glm::dvec3 coords);
	glm::dvec3 getCoordinates();
private:
	// Añade a la clase Estrella3D atributos (los ángulos de giro) para que gire sobre su eje Z y sobre su eje Y.
	GLdouble anguloEnY;
	GLdouble anguloEnZ;
	// Coordenadas del objeto en la escena.
	glm::dvec3 coordinates;
};

//-------------------------------------------------------------------------

class ContCubo : public Abs_Entity
{
public:
	explicit ContCubo(GLdouble ld);
	~ContCubo();
	virtual void render(glm::dmat4 const& modelViewMat) const;
	void setTextureAux(Texture* tex) { mTextureAux = tex; }; // Método para establecer la textura secunaria del objeto.

protected:
	Texture* mTextureAux = nullptr; // Textura secundaria del objeto.
};

//-------------------------------------------------------------------------

class CajaConFondo : public Abs_Entity
{
public:
	explicit CajaConFondo(GLdouble ld);
	~CajaConFondo();
	virtual void update(); // Redefinimos el método update.
	virtual void render(glm::dmat4 const& modelViewMat) const;
	void setTextureAux(Texture* tex) { mTextureAux = tex; }; // Método para establecer la textura secunaria del objeto.
	void setModelMat(glm::dmat4 const& aMat) { mModelMatAux = mModelMat = aMat; }; // Hacemos override (para del translate del conjunto).
protected:
	GLdouble ld; // Lado de la caja y el rectangulo de fondo.
	Mesh* mMeshAux; // Para la malla del rectángulo.
	glm::dmat4 mModelMatAux; // Matriz para colocar el rectángulo en el fondo.
	Texture* mTextureAux = nullptr; // Textura secundaria del objeto.
};

//-------------------------------------------------------------------------

class Suelo : public Abs_Entity
{
public:
	explicit Suelo(GLdouble w, GLdouble h, GLuint rw, GLuint rh);
	~Suelo();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

//-------------------------------------------------------------------------

class Foto : public Abs_Entity
{
public:
	explicit Foto(GLdouble w, GLdouble h);
	~Foto();
	virtual void update(); // Redefinimos el método update.
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

//-------------------------------------------------------------------------

class Planta : public Abs_Entity
{
public:
	explicit Planta(GLdouble w, GLdouble h);
	~Planta();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

//-------------------------------------------------------------------------

class Hexagono : public Abs_Entity
{
public:
	explicit Hexagono(GLdouble rd);
	~Hexagono();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

//-------------------------------------------------------------------------

class QuadricEntity : public Abs_Entity {
public:
	QuadricEntity();
	~QuadricEntity() {
		// Cuando el destructor de esta clase es invocado, indicamos a OpenGL que se destruya el objeto cuádrico.
		gluDeleteQuadric(q); 
	};
protected:
	// Declaramos una variable 'q' (heredable) para poder crear objetos cuádricos con OpenGL.
	GLUquadricObj* q;
};

//-------------------------------------------------------------------------

class Sphere : public QuadricEntity {
public:
	Sphere(GLdouble r); // r es el radio de la esfera
	void render(glm::dmat4 const& modelViewMat) const;
protected:
	GLdouble r;
};

class SphereWithFocus : public Sphere {
public:
	SphereWithFocus(GLdouble r); // r es el radio de la esfera
	void render(glm::dmat4 const& modelViewMat) const;
	void enableFocus();
	void disableFocus();
	virtual void update();
protected:
	GLdouble angle = 5.0;
	SpotLight* spotLight;
};

class Cylinder : public QuadricEntity {
public:
	Cylinder(GLdouble baseRadius, GLdouble topRadius, GLdouble height, GLint pp, GLint p);
	void render(glm::dmat4 const& modelViewMat) const;
protected:
	GLint slices, stacks;
	GLdouble  baseRadius, topRadius, height;
};

class Disk : public QuadricEntity {
public:
	Disk(GLdouble innerRadius, GLdouble outerRadius, GLuint slices, GLuint loops);
	void render(glm::dmat4 const& modelViewMat) const;
protected:
	GLuint slices, loops;
	GLdouble innerRadius, outerRadius;
};

class PartialDisk : public QuadricEntity {
public:
	PartialDisk(GLdouble innerRadius, GLdouble outerRadius, GLdouble startAngle, GLdouble diskAngle);
	void render(glm::dmat4 const& modelViewMat) const;
protected:
	GLdouble innerRadius, outerRadius, startAngle, diskAngle;
};

//-------------------------------------------------------------------------

class AnilloCuadrado : public Abs_Entity {
public:
	explicit AnilloCuadrado();
	~AnilloCuadrado();
	void render(glm::dmat4 const& modelViewMat) const;
protected:
};

//-------------------------------------------------------------------------

class Cubo : public Abs_Entity {
public:
	explicit Cubo(GLdouble l);
	~Cubo();
	void render(glm::dmat4 const& modelViewMat) const;
protected:
};

//-------------------------------------------------------------------------

class CompoundEntity : public Abs_Entity {
public:
	explicit CompoundEntity() {};
	~CompoundEntity();
	virtual void update();
	virtual void render(glm::dmat4 const& modelViewMat) const;
	void addEntity(Abs_Entity* entity);
protected:
	// Vector que contiene los elementos de la escena.
	std::vector<Abs_Entity*> gObjects;
	// Vector que contiene las texturas de los objetos.
	std::vector<Texture*> gTextures;
};

//-------------------------------------------------------------------------

class TieFighter : public CompoundEntity {
public:
	explicit TieFighter();
	~TieFighter();
	void setLightOn();
	void setLightOff();
	void render(glm::dmat4 const& modelViewMat) const;
protected:
	SpotLight* spotLight;
};

//-------------------------------------------------------------------------

class TieFighterSquadron : public CompoundEntity {
public:
	explicit TieFighterSquadron(GLdouble radio);
	~TieFighterSquadron() {};
	virtual void update();
	void render(glm::dmat4 const& modelViewMat) const;
	void setLightsOn();
	void setLightsOff();
	void setOrbitOrRotate(bool orbit);
protected:
	bool orbitOrRotate;
};

//-------------------------------------------------------------------------

class Rotor : public CompoundEntity {
public:
	explicit Rotor(GLdouble r, GLdouble h);
	~Rotor() {};
	void render(glm::dmat4 const& modelViewMat) const;
};

//-------------------------------------------------------------------------

class CapAndHat : public CompoundEntity {
public:
	explicit CapAndHat();
	~CapAndHat() {};
	void render(glm::dmat4 const& modelViewMat) const;
};

//-------------------------------------------------------------------------

class Cone : public Abs_Entity {
public:
	explicit Cone(GLdouble h, GLdouble r, GLuint n);
	~Cone();
	void render(glm::dmat4 const& modelViewMat) const;
};

//-------------------------------------------------------------------------

class Grid : public Abs_Entity {
public:
	explicit Grid(GLdouble lado, GLuint nDiv);
	~Grid();
	void render(glm::dmat4 const& modelViewMat) const;
};

//-------------------------------------------------------------------------

class GridCube : public CompoundEntity {
public:
	explicit GridCube(GLdouble lado, GLuint nDiv);
	~GridCube() {};
	void render(glm::dmat4 const& modelViewMat) const;
};

//-------------------------------------------------------------------------

class Esfera : public EntityWithMaterial {
public:
	explicit Esfera(GLdouble r, GLint p, GLuint m);
	~Esfera();
	void render(glm::dmat4 const& modelViewMat) const;
};

//-------------------------------------------------------------------------

class Toroide : public EntityWithMaterial {
public:
	explicit Toroide(GLdouble r, GLint p, GLuint m);
	~Toroide();
	void render(glm::dmat4 const& modelViewMat) const;
};

//-------------------------------------------------------------------------

class Tornillo : public CompoundEntity {
public:
	explicit Tornillo();
	~Tornillo() {};
	void render(glm::dmat4 const& modelViewMat) const;
};

//-------------------------------------------------------------------------

class TornilloConLuz : public Tornillo {
public:
	explicit TornilloConLuz();
	~TornilloConLuz() {};
	void enableSpot();
	void disableSpot();
	virtual void update();
	void render(glm::dmat4 const& modelViewMat) const;
protected:
	SpotLight* spotLight;
};

//-------------------------------------------------------------------------

class Faro : public CompoundEntity {
public:
	explicit Faro(GLdouble radio);
	~Faro() {};
	void setLightOn();
	void setLightOff();
	virtual void update();
	void render(glm::dmat4 const& modelViewMat) const;
protected:
	GLdouble angulo, radio;
};

//-------------------------------------------------------------------------

class Piramide : public Abs_Entity {
public:
	explicit Piramide(GLint caras, GLdouble radio);
	~Piramide() {};
	void render(glm::dmat4 const& modelViewMat) const;
};

//-------------------------------------------------------------------------

#endif //_H_Entities_H_