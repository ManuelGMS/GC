//#pragma once
#ifndef _H_Camera_H_
#define _H_Camera_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Viewport.h"

//-------------------------------------------------------------------------

class Camera {
public:
	explicit Camera(Viewport* vp);
	~Camera() {};

	// viewPort
	Viewport const& viewPort() const { return *mViewPort; };

	// view matrix 
	glm::dmat4 const& viewMat() const { return mViewMat; };
	
	// Vistas 2D y 3D.
	void set2D();
	void set3D();
	
	// **** PRACTICA CAMARA **************************************************************************************

	// Vista Cenital.
	void setCenital();

	// M�todos para desplazar la c�mara en cada uno de sus ejes sin cambiar la direcci�n de vista.
	void moveLR(GLdouble cs); // Left / Right
	void moveFB(GLdouble cs); // Forward / Backward
	void moveUD(GLdouble cs); // Up / Down

	/*
	void pitch(GLdouble a); // rotates a degrees on the X axis
	void yaw(GLdouble a);   // rotates a degrees on the Y axis
	void roll(GLdouble a);  // rotates a degrees on the Z axis
	*/

	// Para realizar con la c�mara una trayectoria circular alrededor de look.
	void orbit(GLdouble incAng, GLdouble incY);

	// Para habilitar/deshabilitar la proyecci�n ortogonal/perspectiva.
	void changePrj();
	
	// **** PRACTICA CAMARA **************************************************************************************

	// projection matrix
	glm::dmat4 const& projMat() const { return mProjMat; };
	
	// sets scene visible area size 
	void setSize(GLdouble xw, GLdouble yh);
	// updates the scale factor 
	void setScale(GLdouble s);

	// transfers its viewport, the view matrix and projection matrix to the GPU
	void upload() const { mViewPort->upload();  uploadVM(); uploadPM(); }; 

protected:
	
	// **** PRACTICA CAMARA *************************************************************************************************

	// Atributos para gestionar el radio y el �ngulo de la circunferencia (Orbit)
	GLdouble mAng; // �ngulo (distancia angular) de la c�mara (desde el eje X positivo 0�, sentido antihorario).
	GLdouble mRadio; // Radio de una esfera imaginaria sobre el cual se mueve la c�mara.
	
	/*
	Los ejes(U, V, N) definen el sistema de coordenadas de la c�mara, se obtienen de las filas (row: 0, 1, 2) de mViewMat.
	Ejes / vectores son mRight(eje X), mUpward(ejeY) y mFront(este �ltimo es hacia donde mira la c�mara(-Z "o" -N)).
	*/
	glm::dvec3 mRight, mUpward, mFront;
	void setVM();
	void setAxes();
	
	// Para habilitar/deshabilitar la proyecci�n ortogonal/perspectiva.
	bool bOrto = true;

	// **** PRACTICA CAMARA *************************************************************************************************

	glm::dvec3 mEye = { 0.0, 0.0, 500.0 };  // camera's position
	glm::dvec3 mLook = { 0.0, 0.0, 0.0 };   // target's position
	glm::dvec3 mUp = { 0.0, 1.0, 0.0 };     // the up vector 

	glm::dmat4 mViewMat;    // view matrix = inverse of modeling matrix 
	void uploadVM() const;  // transfers viewMat to the GPU

	glm::dmat4 mProjMat;     // projection matrix
	void uploadPM() const;   // transfers projMat to the GPU

	GLdouble xRight, xLeft, yTop, yBot;      // size of scene visible area
	GLdouble mNearVal = 1, mFarVal = 10000;  // view volume
	GLdouble mScaleFact = 1;   // scale factor

	Viewport* mViewPort;   // the viewport

	void setPM();
};
//-------------------------------------------------------------------------

#endif //_H_Camera_H_