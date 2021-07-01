#include "Camera.h"

#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_access.hpp>

using namespace glm;

//-------------------------------------------------------------------------

Camera::Camera(Viewport* vp): mViewPort(vp), mViewMat(1.0), mProjMat(1.0),  
							  xRight(vp->width() / 2.0), xLeft(-xRight),
							  yTop(vp->height() / 2.0), yBot(-yTop)
{
    this->setPM();
}
//-------------------------------------------------------------------------

void Camera::uploadVM() const 
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(mViewMat)); // transfers view matrix to the GPU 
}
//-------------------------------------------------------------------------

void Camera::set2D() 
{
	this->mAng = 270; // �ngulo (m�todo Orbit)
	this->mRadio = 500; // Radio (m�todo orbit)
	this->mEye = dvec3(0, 0, 500); // Posicionamos la c�mara en las coordenadas iniciales de la circunferencia.
	this->mLook = dvec3(0, 0, 0); // Punto al que mira la c�mara.
	this->mUp = dvec3(0, 1, 0); // Vector que indica la inclinaci�n de la c�mara.
	this->setVM(); // Para mantener actualizada la matriz de vista y el sistema de coordenadas de la c�mara.
}
//-------------------------------------------------------------------------

void Camera::set3D() 
{
	this->mAng = 315; // �ngulo (m�todo Orbit)
	this->mRadio = 500; // Radio (m�todo orbit)
	GLdouble xOrbit = mLook.x + cos(radians(mAng)) * mRadio; // Posici�n "x" (m�todo Orbit).
	GLdouble zOrbit = mLook.z - sin(radians(mAng)) * mRadio; // Posici�n "z" (m�todo Orbit).
	this->mEye = dvec3(xOrbit, 500, zOrbit); // Posicionamos la c�mara en las coordenadas iniciales de la circunferencia.
	this->mLook = dvec3(0, 10, 0); // Punto al que mira la c�mara.
	this->mUp = dvec3(0, 1, 0); // Vector que indica la inclinaci�n de la c�mara.
	this->setVM(); // Para mantener actualizada la matriz de vista y el sistema de coordenadas de la c�mara.
}
//-------------------------------------------------------------------------

void Camera::setCenital() {
	this->mAng = 270; // �ngulo (m�todo Orbit)
	this->mRadio = 500; // Radio (m�todo orbit)
	this->mEye = dvec3(0, 500, 0); // Posicionamos la c�mara en las coordenadas iniciales de la circunferencia.
	this->mLook = dvec3(0, 0, 0); // Punto al que mira la c�mara.
	this->mUp = dvec3(1, 0, 0); // Vector que indica la inclinaci�n de la c�mara.
	this->setVM(); // Para mantener actualizada la matriz de vista y el sistema de coordenadas de la c�mara.
}
//-------------------------------------------------------------------------

/*
Obtiene sistema de coordenadas de la c�mara (dado por los ejes mRight, mUpward y mFront)
a partir de la matriz de vista (mViewMat). Donde mRight (eje X), mUpward (ejeY) y mFront 
(es hacia donde mira la c�mara (-Z "o" -N)).
*/
void Camera::setAxes() {
	mRight = row(mViewMat, 0);  // U / X
	mUpward = row(mViewMat, 1); // V / Y
	mFront = -row(mViewMat, 2); // -N / -Z 
}

// Obtiene la matriz de vista (mViewMat), luego llama a setAxes() para actualizar el sistema de coordenadas de la c�mara.
void Camera::setVM() {
	// La matriz de vista (mViewMat) transforma coordenadas globales a coordenadas de c�mara.
	mViewMat = lookAt(mEye, mLook, mUp);
	// Actualiza el sistema de coordenadas de la c�mara.
	this->setAxes();
}

//-------------------------------------------------------------------------

// M�todos para desplazar la c�mara en cada uno de sus ejes sin cambiar la direcci�n de vista.

void Camera::moveLR(GLdouble cs) { // Left / Right
	mEye += mRight * cs; // Para mover la c�mara en su eje (U / X).
	mLook += mRight * cs; // Para que la c�mara cambie el punto al que mira (look / target).
	this->setVM(); // Para mantener actualizada la matriz de vista y el sistema de coordenadas de la c�mara.
}

void Camera::moveUD(GLdouble cs) { // Up / Down
	mEye += mUpward * cs; // Para mover la c�mara en su eje (V / Y).
	mLook += mUpward * cs; // Para que la c�mara cambie el punto al que mira (look / target).
	this->setVM(); // Para mantener actualizada la matriz de vista y el sistema de coordenadas de la c�mara.
}

void Camera::moveFB(GLdouble cs) { // Forward / Backward
	mEye += mFront * cs; // Para mover la c�mara en su eje (N / Z).
	mLook += mFront * cs; // Para que la c�mara cambie el punto al que mira (look / target).
	this->setVM(); // Para mantener actualizada la matriz de vista y el sistema de coordenadas de la c�mara.
}

//-------------------------------------------------------------------------

// M�todo para desplazar eye por la circunferencia, al mismo tiempo, se permite subir y bajar la c�mara.
void Camera::orbit(GLdouble incAng, GLdouble incY) {
	mAng += incAng; // Incrementa-Decrementa el �ngulo .
	mEye.x = mLook.x + cos(radians(mAng)) * mRadio; // Posici�n de la c�mara en el eje X.
	mEye.z = mLook.z - sin(radians(mAng)) * mRadio; // Posici�n de la c�mara en el eje Z.
	mEye.y += incY; // Mueve la c�mara en su eje vertical.
	this->setVM(); // Para mantener actualizada la matriz de vista y el sistema de coordenadas de la c�mara.
}

//-------------------------------------------------------------------------

// Para habilitar/deshabilitar la proyecci�n ortogonal/perspectiva.
void Camera::changePrj() {
	this->bOrto = !this->bOrto;
	this->setPM();
}

//-------------------------------------------------------------------------

/*
void Camera::pitch(GLdouble a) 
{  
	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(1.0, 0, 0));
	// glm::rotate returns mViewMat * rotationMatrix
}
*/
//-------------------------------------------------------------------------
/*
void Camera::yaw(GLdouble a) 
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(0, 1.0, 0));
	// glm::rotate returns mViewMat * rotationMatrix
}
*/
//-------------------------------------------------------------------------
/*
void Camera::roll(GLdouble a) 
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(0, 0, 1.0));
	// glm::rotate returns mViewMat * rotationMatrix
}
*/
//-------------------------------------------------------------------------

void Camera::setSize(GLdouble xw, GLdouble yh) 
{
	xRight = xw / 2.0;
	xLeft = -xRight;
	yTop = yh / 2.0;
	yBot = -yTop;
	setPM();
}
//-------------------------------------------------------------------------

void Camera::setScale(GLdouble s) 
{
	mScaleFact -= s;
	if (mScaleFact < 0) mScaleFact = 0.01;
	setPM();
}
//-------------------------------------------------------------------------

void Camera::setPM() 
{
	// Cambia el tipo de proyecci�n.
	if (bOrto) {
		// Genera una matriz de proyecci�n ortogonal.
		mProjMat = glm::ortho(xLeft*mScaleFact, xRight*mScaleFact, yBot*mScaleFact, yTop*mScaleFact, mNearVal, mFarVal);
	} else {
		/*
		Genera una matriz de proyecci�n con perspectiva. 
		
		Eye, Near y Top establecen el �ngulo del campo de visi�n en el eje Y (fovy: Feel Of View Y).
		fovy --> Debe pertenecer al intervalo [60� - 90�]
		AspectRatio = Ancho / Alto

		Relaci�n Top-Near: tan(fovy/2) = Top / Near
		Si (fovy = 60�) --> tan(30) = 0,5773 -> Near = 2 * Top
		Si (fovy = 90�) --> tan(45) = 1 -> Near = Top
		*/ 
		GLdouble AspectRatio = (-xLeft * mScaleFact + xRight * mScaleFact) / (-yBot * mScaleFact + yTop * mScaleFact);
		mProjMat = glm::perspective(90.0, AspectRatio, mNearVal, mFarVal);
	}
}
//-------------------------------------------------------------------------

void Camera::uploadPM() const 
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(value_ptr(mProjMat)); // transfers projection matrix to the GPU
	glMatrixMode(GL_MODELVIEW);
}
//-------------------------------------------------------------------------


