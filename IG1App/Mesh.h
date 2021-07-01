//#pragma once
#ifndef _H_Mesh_H_
#define _H_Mesh_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include <vector>

//-------------------------------------------------------------------------

class Mesh 
{
public:

	Mesh() {};
	virtual ~Mesh() {};

	Mesh(const Mesh & m) = delete;  // no copy constructor
	Mesh & operator=(const Mesh & m) = delete;  // no copy assignment
			
	virtual void render() const;
	
	GLuint size() const { return mNumVertices; };   // number of elements
	std::vector<glm::dvec3> const& vertices() const { return vVertices; };
	std::vector<glm::dvec4> const& colors() const { return vColors; };

	static Mesh* createRGBAxes(GLdouble l); // creates a new 3D-RGB axes mesh.
	static Mesh* generaPoligono(GLuint numL, GLdouble rd); // Para polígono.
	static Mesh* generaSierpinski(GLdouble rd, GLuint numP); // Para Sierpinski.
	static Mesh* generaTrianguloRGB(GLdouble rd); // Para triangulo RGB.
	static Mesh* generaRectangulo(GLdouble w, GLdouble h); // Para generar un rectángulo.
	static Mesh* generaRectanguloRGB(GLdouble w, GLdouble h); // Para rectángulo RGB.
	static Mesh* generaEstrella3D(GLdouble re, GLuint np, GLdouble h); // Para estrella 3D.
	static Mesh* generaContCubo(GLdouble ld); // Para cubo.

	// Cubrir el rectángulo con una imagen que se repite rw veces a lo ancho y rh veces a lo alto.
	static Mesh* generaRectanguloTexCor(GLdouble w, GLdouble h, GLuint rw, GLuint rh); 

	// Para estrella con textura.
	static Mesh* generaEstrellaTexCor(GLdouble re, GLuint np, GLdouble h);

	// Para cubo con textura.
	static Mesh* generaContCuboTexCor(GLdouble nl);

	// Para hexagono con textura.
	static Mesh* generaPoligonoTexCor(GLuint numL, GLdouble rd);

	// ****************************************************************************************

protected:

	GLuint mPrimitive = GL_TRIANGLES;   // graphic primitive: GL_POINTS, GL_LINES, GL_TRIANGLES, ...
	GLuint mNumVertices = 0;  // number of elements ( = vVertices.size())
	
	std::vector<glm::dvec3> vVertices;  // vertex array
	std::vector<glm::dvec4> vColors;    // color array
	std::vector<glm::dvec2> vTexCoords; // texture array
	std::vector<glm::dvec3> vNormals; // normals array
	
	virtual void draw() const;

};

//-------------------------------------------------------------------------

class IndexMesh : public Mesh
{
protected:

	GLuint* vIndices = nullptr; // tabla de índices
	GLuint nNumIndices = 0;

public:

	IndexMesh() { mPrimitive = GL_TRIANGLES; }
	~IndexMesh() { delete[] vIndices; }
	
	virtual void render() const;
	virtual void draw() const;

	void buildNormalVectors();

	static IndexMesh* generaAnilloCuadradoIndexado();
	static IndexMesh* generaCuboConTapasIndexado(GLdouble l);
	static IndexMesh* generateGrid(GLdouble lado, GLuint nDiv);
	static IndexMesh* generateGridTex(GLdouble lado, GLuint nDiv);

	// Para piramide indexada.
	static IndexMesh* generaPiramide(GLint caras, GLdouble radio);

};

//-------------------------------------------------------------------------

class MbR : public IndexMesh
{
protected:

	int m; // Número de puntos del perfil.
	int n; // Número de rotaciones (muestras que se toman).
	glm::dvec3* perfil; // Perfil definido en el plano XY (se dan desde ABAJO hacia ARRIBA).
	
public:

	MbR(int m, int n, glm::dvec3* perfil) { 
		this->m = m;
		this->n = n;
		this->perfil = perfil;
	}

	~MbR() { delete perfil; }

	static MbR* generaMallaIndexadaPorRevolucion(int mm, int nn, glm::dvec3* perfil);

};

#endif //_H_Scene_H_