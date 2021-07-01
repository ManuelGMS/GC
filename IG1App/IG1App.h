//#pragma once
#ifndef _H_IG1App_H_
#define _H_IG1App_H_

//#include <Windows.h>
//#include <gl/GL.h>    // OpenGL
//#include <gl/GLU.h>   // OpenGL Utility Library
//#include <GL/glut.h>  // OpenGL Utility Toolkit

#include <GL/freeglut.h>  // OpenGL Utility Toolkit
#include <glm.hpp>        // OpenGL Mathematics

#include "Viewport.h"
#include "Camera.h"
#include "Scene.h"

//-------------------------------------------------------------------------

class IG1App
{
public:  
	// static single instance (singleton pattern)
	static IG1App s_ig1app;

	IG1App(IG1App const & J) = delete; // no copy constructor
	void operator =(IG1App const & J) = delete; // no copy assignment
	
	// Viewport position and size
	Viewport const& viewPort() { return *mViewPort; };
	// Camera position, view volume and projection
	Camera const& camera() { return *mCamera; };
	// Graphics objects of the scene
	Scene const& scene() { return *mScene; };

	void run();    // the main event processing loop
	void close();  // the application

	int winWidth() { return s_ig1app.mWinW; };
	int winHeight() { return s_ig1app.mWinH; };
	
protected:

	IG1App() { 
		this->mBot = 0;
		this->m2Vistas = false;
		this->m2Escenas = false;
		this->mLastUpdateTime = 0; 
		this->mCoord = glm::dvec2(0, 0);
		this->activarDesactivarAnimacion = false;
	};
	
	~IG1App() { close(); };

	void init();
	void iniWinOpenGL();
	void free();   

	// ********************************************************************************************

	// Métodos para los eventos del ratón.
	void motion(int x, int y);
	void mouse(int b, int s, int x, int y);
	void mouseWheel(int n, int d, int x, int y);

	// ********************************************************************************************

	void display() const;   // the scene
	void resize(int newWidth, int newHeight);   // the viewport (without changing the scale) 
	void key(unsigned char key, int x, int y);  // keypress event
	void specialKey(int key, int x, int y);     // keypress event for special characters

	// ********************************************************************************************

	// Para la visualización de 2 vistas en la ventana de la aplicación.
	bool m2Vistas, m2Escenas;
	void display2Vistas() const;
	void display2Escenas() const;

	// ********************************************************************************************

	/* 
	Añade, a la clase IG1App, el método update() (sin argumentos)
	y la función estática s_update() (que invoca al método update()) para el callback de glutIdleFunc.
	*/
	GLuint mLastUpdateTime;
	GLboolean activarDesactivarAnimacion;
	void update();  
	static void s_update() { s_ig1app.update(); };

	// ********************************************************************************************
 
	int mBot; // Guarda el botón que se ha pulsado.
	glm::dvec2 mCoord; // Guarda las coordenadas del ratón.

	// Callbacks para los eventos del ratón.
	static void s_motion(int x, int y) { s_ig1app.motion(x, y); }
	static void s_mouse(int b, int s, int x, int y) { s_ig1app.mouse(b, s, x, y); }
	static void s_mouseWheel(int n, int d, int x, int y) { s_ig1app.mouseWheel(n, d, x, y); }

	// ********************************************************************************************

	// static callbacks 
	static void s_display() { s_ig1app.display(); };
	static void s_resize(int newWidth, int newHeight) { s_ig1app.resize(newWidth, newHeight); };
	static void s_key(unsigned char key, int x, int y) { s_ig1app.key(key, x, y); };
	static void s_specialKey(int key, int x, int y) { s_ig1app.specialKey(key, x, y); };

	
	Viewport *mViewPort = nullptr; // Viewport position and size
	Camera *mCamera = nullptr; // Camera position, view volume and projection
	Scene *mScene = nullptr; // Graphics objects of the scene
	
	Viewport* mViewPortAux = nullptr; // Viewport position and size
	Camera* mCameraAux = nullptr; // Camera position, view volume and projection
	Scene* mSceneAux = nullptr; // Graphics objects of the scene

	bool mStop = false; // main event processing loop
	int mWinId = 0;	    // window's identifier
	int mWinW = 800;    // window's width 
	int mWinH = 600;    // window's height
};
//-------------------------------------------------------------------------

#endif //_H_IG1App_H_
