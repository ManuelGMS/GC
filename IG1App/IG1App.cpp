#include "IG1App.h"
#include "CheckML.h"
#include <iostream>

#include "Texture.h"

using namespace std;

//-------------------------------------------------------------------------
// static single instance (singleton pattern)
// default constructor (constructor with no parameters)
IG1App IG1App::s_ig1app;  
//-------------------------------------------------------------------------

/* 
Este c�digo se ejecuta cuando se mueve el rat�n y alg�n bot�n est� presionado.
Argumentos: Coordenadas(x, y) de la ventana.
*/
void IG1App::motion(int x, int y) {

	// Adaptamos la Y de coordenadas de la VENTANA a coordenadas del PUERTO DE VISTA.
	y = glutGet(GLUT_WINDOW_HEIGHT) - y;

	// Guardamos en una variable auxiliar (mp) la diferencia entre mCoord y (x, y).
	glm::dvec2 mp = mCoord - glm::dvec2(x, y);

	// Guardamos en mCoord la posici�n (x, y) del rat�n.
	this->mCoord = glm::dvec2(x, y);

	if (!this->m2Escenas) {
		
		// Si mBot es el bot�n izquierdo, entonces orbitar (rotar la c�mara al rededor de la escena).
		if (this->mBot == GLUT_LEFT_BUTTON)
			mCamera->orbit(mp.x * 0.05, mp.y);

		// Si mBot es el bot�n derecho, mover la c�mara en sus ejes X e Y con moveUD() y moveLR() seg�n lo indicado en mp.
		if (this->mBot == GLUT_RIGHT_BUTTON) {
			mCamera->moveLR(mp.x);
			mCamera->moveUD(mp.y);
		}
	
	} else {

			if (this->mBot == GLUT_LEFT_BUTTON)
				(x <= mWinW / 2) ? (*mCamera).orbit(mp.x * 0.05, mp.y) : (*mCameraAux).orbit(mp.x * 0.05, mp.y);

			if (this->mBot == GLUT_RIGHT_BUTTON) {
				(x <= mWinW / 2) ? (*mCamera).moveLR(mp.x) : (*mCameraAux).moveLR(mp.x);
				(x <= mWinW / 2) ? (*mCamera).moveUD(mp.y) : (*mCameraAux).moveUD(mp.y);
			}
	
	}

	// Redibujar la escena.
	glutPostRedisplay();

}

/*
Este c�digo se ejecuta al presionar o soltar un bot�n del rat�n.
Argumentos: Bot�n pulado (b), coordenadas (x, y) de la ventana, estado (state) del bot�n (pulsado o liberado).
*/
void IG1App::mouse(int b, int s, int x, int y) {

	// 0�) Adaptamos la Y de coordenadas de la VENTANA a coordenadas del PUERTO DE VISTA.
	y = glutGet(GLUT_WINDOW_HEIGHT) - y;
	
	// 1�) Guarda el bot�n pulsado.
	this->mBot = b; 

	// 2�) Guarda las coorenadas del rat�n.
	this->mCoord = glm::dvec2(x, y);

}

/*
Este c�digo se ejecuta al mover la rueda del rat�n en la direction +Z / -Z.
Argumentos: Direcci�n en el eje Z (d), coordenadas (x, y) de la ventana.
*/
void IG1App::mouseWheel(int n, int d, int x, int y) {
	
	/*
	1�) Averiguamos si se ha pulado alguna tecla modificadora (CTRL, ALT, SHIFT). 
	2�) Si no hay ninguna tecla modificadora pulsada la c�mara se mueve con moveFB() seg�n el valor de d (+Z/-Z).
	3�) Si est� pulsada la tecla CTRL la c�mara cambia la escala de la escena con setScale() seg�n el valor de d (+1/-1).
	*/
	switch (glutGetModifiers())
	{
		case GLUT_ACTIVE_ALT:
			// La pr�ctica no especifica nada de esta tecla, pero es requisito para no pulsar m�s de un modificador.
		break;
		case GLUT_ACTIVE_CTRL:
			// El zoom en la vista ortogonal se hace aumentando la escala.
			if (!this->m2Escenas) {
				(d > 0) ? mCamera->setScale(0.1) : mCamera->setScale(-0.1);
			} else {
				if(x <= mWinW / 2) (d > 0) ? mCamera->setScale(0.1) : mCamera->setScale(-0.1);
				else (d > 0) ? mCameraAux->setScale(0.1) : mCameraAux->setScale(-0.1);
			}
		break;
		case GLUT_ACTIVE_SHIFT:
			// La pr�ctica no especifica nada de esta tecla, pero es requisito para no pulsar m�s de un modificador.
		break;
		default:
			// El zoom en la vista con perspectiva se hace aumentando moviendo la c�mara.
			if (!this->m2Escenas) {
				mCamera->moveFB(10.0 * d);
			} else {
				(x <= mWinW / 2) ? mCamera->moveFB(10.0 * d) : mCameraAux->moveFB(10.0 * d);
			}
		break; 
	}

	// 4�) Redibujar la escena.
	glutPostRedisplay();

}

//-------------------------------------------------------------------------
// A�ade, a la clase IG1App, el m�todo update() (sin argumentos) 
// El m�todo update() debe indicar a la escena que se actualice cada cierto tiempo(no m�s de 60 veces por segundo).
void IG1App::update()
{
	if(activarDesactivarAnimacion) {
		// Milisegundos transcurridos desde que se inici� la ejecuci�n --> glutGet(GLUT_ELAPSED_TIME); (siempre crece)
		// (1000/60=17) --> Si quiero 60 frames x second (Cada segundo muestro 60 fotogramas; m�s fotogramas por unidad de tiempo)
		// (1000/32=32) --> Si quiero 32 frames x second (Cada segundo muestro 32 fotogramas; menos fotogramas por unidad de tiempo)
		if (glutGet(GLUT_ELAPSED_TIME) - this->mLastUpdateTime >= 32) { // Mas peque�o m�s veces se ejecuta.
			this->mLastUpdateTime = glutGet(GLUT_ELAPSED_TIME); 
			mScene->update(); // Recalcula toda la escena (datos) (el update del triangulo actualiza y lo hace orbitar).
			glutPostRedisplay(); // Llama a display() para volver a mostrar la imagen.
		}
	}
}
//-------------------------------------------------------------------------

void IG1App::close()  
{
	if (!mStop) { // if main loop has not stopped
		cout << "Closing glut...\n";
		glutLeaveMainLoop(); // stops main loop and destroy the OpenGL context
		mStop = true; // main loop stopped  
	}
	free();
}
//-------------------------------------------------------------------------

void IG1App::run() // enters the main event processing loop
{
	if (mWinId == 0) { // if not initialized
		init(); // initialize the application 
		glutMainLoop(); // enters the main event processing loop 
		mStop = true; // main loop has stopped  
	}
}
//-------------------------------------------------------------------------

void IG1App::init()
{
	
	// create an OpenGL Context
	iniWinOpenGL();   

	// *************************************************************************************************************

	// create the scene after creating the context 
	// allocate memory and resources
	mViewPort = new Viewport(mWinW, mWinH); // glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)
	mCamera = new Camera(mViewPort);
	mScene = new Scene();
	// initalization
	mCamera->set2D();
	mScene->init();
	
	// *************************************************************************************************************

	// create the scene after creating the context 
	// allocate memory and resources
	/*
	mViewPortAux = new Viewport(mWinW, mWinH); // glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)
	mCameraAux = new Camera(mViewPortAux);
	mSceneAux = new Scene();
	// initalization
	mCameraAux->set2D();
	mSceneAux->init();
	*/

}
//-------------------------------------------------------------------------

void IG1App::iniWinOpenGL() 
{  // Initialization
	cout << "Starting glut...\n";
	int argc = 0;
	glutInit(&argc, nullptr);
		
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);  // GLUT_CORE_PROFILE
	glutInitContextFlags(GLUT_DEBUG);		// GLUT_FORWARD_COMPATIBLE

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(mWinW, mWinH);   // window size
	//glutInitWindowPosition (140, 140);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH /*| GLUT_STENCIL*/); // RGBA colors, double buffer, depth buffer and stencil buffer   
	
	mWinId = glutCreateWindow("IG1App");  // with its associated OpenGL context, return window's identifier 
	
	// Callback registration
	glutReshapeFunc(s_resize);
	glutKeyboardFunc(s_key);
	glutSpecialFunc(s_specialKey);
	glutDisplayFunc(s_display);
	glutIdleFunc(s_update); // Cuando la aplicaci�n est� IDLE (inactiva).
	glutMouseFunc(s_mouse); //
	glutMotionFunc(s_motion); //
	glutMouseWheelFunc(s_mouseWheel); // 

	cout << glGetString(GL_VERSION) << '\n';
	cout << glGetString(GL_VENDOR) << '\n';
}
//-------------------------------------------------------------------------

void IG1App::free() 
{  
	// release memory and resources
	delete mScene; mScene = nullptr;
	delete mCamera; mCamera = nullptr;
	delete mViewPort; mViewPort = nullptr;
	// release memory and resources
	delete mSceneAux; mSceneAux = nullptr;
	delete mCameraAux; mCameraAux = nullptr;
	delete mViewPortAux; mViewPortAux = nullptr;
}
//-------------------------------------------------------------------------

void IG1App::display() const   
{
	// Borra los buffers de COLOR y PROFUNDIDAD.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	
	if (this->m2Vistas) // Vista normal y vista cenital.
		display2Vistas();
	else if (this->m2Escenas) // Escena 0 y escena 1. 
		display2Escenas();
	else // Modo de visualizaci�n normal.
		mScene->render(*mCamera);

	// Pasa el contenido del buffer BACK (ah� se pinta/renderiza) al buffer FRONT (lo que vemos).
	glutSwapBuffers(); 
}
//-------------------------------------------------------------------------

void IG1App::display2Escenas() const {
	
	// C�mara auxiliar (copia de la original), ViewPort (pointer) compartido por ambas.
	Camera auxCam1 = (*mCamera);
	Camera auxCam2 = (*mCameraAux);

	// Hacemos una copia del viewPort en una variable auxiliar para restaurar el puerto de vista.
	Viewport auxVP1 = (*mViewPort);
	Viewport auxVP2 = (*mViewPortAux);

	// **************************************************************************************************

	// Establece el tama�o del viewPort.
	mViewPort->setSize(mWinW / 2, mWinH);

	// Hacemos que la c�mara tenga el mismo tama�o para mantener la proporci�n 1:1 con el ViewPort.
	auxCam1.setSize(mViewPort->width(), mViewPort->height());

	// Configuramos la posici�n de este puerto de vista en la ventana de la aplicaci�n.
	mViewPort->setPos(0, 0);

	// Renderizamos ahora con la c�mara y el puerto de vista configurados.
	mScene->render(auxCam1);

	// **************************************************************************************************
	
	// Establece el tama�o del viewPort.
	mViewPortAux->setSize(mWinW / 2, mWinH);

	// Hacemos que la c�mara tenga el mismo tama�o para mantener la proporci�n 1:1 con el ViewPort.
	auxCam2.setSize(mViewPortAux->width(), mViewPortAux->height());

	// Configuramos la posici�n de este puerto de vista en la ventana de la aplicaci�n.
	mViewPortAux->setPos(mWinW / 2, 0);

	// Renderizamos ahora con la c�mara y el puerto de vista configurados.
	mSceneAux->render(auxCam2);

	// **************************************************************************************************

	// Restaurar el puerto de vista.
	(*mViewPort) = auxVP1;
	(*mViewPortAux) = auxVP2;
}
//-------------------------------------------------------------------------

void IG1App::display2Vistas() const { // LLamar desde display.

	// C�mara auxiliar (copia de la original -> mCamera), viewPort(pointer) compartido por ambas.
	Camera auxCam = *mCamera;
	
	// Hacemos una copia del viewPort en una variable auxiliar para restaurar el puerto de vista.
	Viewport auxVP = *mViewPort; 
	
	// Establece el tama�o del viewPort (este es compartido por las 2 c�maras).
	mViewPort->setSize(mWinW / 2, mWinH);

	// Hacemos que la c�mara tenga el mismo tama�o para mantener la proporci�n 1:1 con el ViewPort.
	auxCam.setSize(mViewPort->width(), mViewPort->height());

	// Vista Usuario ************************************************************************************
	
	// Configuramos la posici�n de este puerto de vista en la ventana de la aplicaci�n.
	mViewPort->setPos(0, 0);
	
	// Renderizamos ahora con la c�mara y el puerto de vista configurados.
	mScene->render(auxCam);

	// Vista Cenital ************************************************************************************
	
	// Configuramos la posici�n de este puerto de vista en la ventana de la aplicaci�n.
	mViewPort->setPos(mWinW / 2, 0);

	// Cambiamos la posici�n y la orientaci�n de la c�mara.
	auxCam.setCenital();
	
	// Renderizamos ahora con la c�mara y el puerto de vista configurados.
	mScene->render(auxCam);

	// **************************************************************************************************

	// Restaurar el puerto de vista.
	(*mViewPort) = auxVP;

}

//-------------------------------------------------------------------------

void IG1App::resize(int newWidth, int newHeight) 
{
	mWinW = newWidth; mWinH = newHeight;

	// Resize Viewport to the new window size
	mViewPort->setSize(newWidth, newHeight);

	// Resize Scene Visible Area such that the scale is not modified
	mCamera->setSize(mViewPort->width(), mViewPort->height()); 
}
//-------------------------------------------------------------------------

void IG1App::key(unsigned char key, int x, int y) 
{
	bool need_redisplay = true;
	
	switch (key) {
	case 27:  // Escape key 
		glutLeaveMainLoop();  // stops main loop and destroy the OpenGL context
	case '+':
		mCamera->setScale(+0.01);  // zoom in  (increases the scale)
		break;
	case '-':
		mCamera->setScale(-0.01);  // zoom out (decreases the scale)
		break;
	case 'l':
		mCamera->set3D();
		break;
	case 'o':
		mCamera->set2D();
		break;
	case 'u': // Activa la aniaci�n un instante.
		if (!this->m2Escenas) mScene->update();
		else (this->mCoord.x <= mWinW / 2) ? mScene->update() : mSceneAux->update();
		break;
	case 'U': // Para activar o desactivar la animaci�n.
		this->mLastUpdateTime = glutGet(GLUT_ELAPSED_TIME);
		this->activarDesactivarAnimacion = !this->activarDesactivarAnimacion;
		break;
	case 'F': // Para guardar la escena en el disco.
		mScene->saveTheFoto();
		break;
	case 'p': // Alterna entre la proyecci�n ortogonal y con perspectiva. 
		if (!this->m2Escenas) mCamera->changePrj();
		else (this->mCoord.x <= mWinW / 2) ? mCamera->changePrj() : mCameraAux->changePrj();
		break;
	case 'k': // Alterna entre la visualizaci�n normal y la doble visualizaci�n normal/cenital.
		this->m2Escenas = false;
		this->m2Vistas = !this->m2Vistas;
		break;
	case 'n': // Alterna entre la visualizaci�n normal y la doble visualizaci�n escena1/escena0.
		/*
		this->m2Vistas = false;
		this->m2Escenas = !this->m2Escenas;
		mScene->changeScene(2);
		mSceneAux->setPart(mScene->getPart());
		mSceneAux->changeScene(1);
		*/
		break;
	case 'q':
		mScene->turnLightState(0, true);
		break;
	case 'w':
		mScene->turnLightState(0, false);
		break;
	case 'a':
		mScene->turnLightState(1, true);
		break;
	case 's':
		mScene->turnLightState(1, false);
		break;
	case 'z':
		mScene->turnLightState(2, true);
		break;
	case 'x':
		mScene->turnLightState(2, false);
		break;
	case 'e':
		mScene->disableDefaultAmbientLight(true);
		break;
	case 'r':
		mScene->disableDefaultAmbientLight(false);
		break;
	case 'y':
		mScene->orbita();
		break;
	case 'b':
		mScene->rotate();
		break;
	case 't':
		mScene->TurnLightsOn();
		break;
	case 'g':
		mScene->TurnLightsOff();
		break;
	case 'c': // P2 <--> P1
		this->m2Vistas = false;
		this->m2Escenas = false;
		mScene->changePart();
		break;
	case '1': // Escena
		this->m2Vistas = false;
		this->m2Escenas = false;
		mScene->changeScene(1);
		break;
	case '2': // Escena
		this->m2Vistas = false;
		this->m2Escenas = false;
		mScene->changeScene(2);
		break;
	case '3': // Escena
		this->m2Vistas = false;
		this->m2Escenas = false;
		mScene->changeScene(3);
		break;
	case '4': // Escena
		this->m2Vistas = false;
		this->m2Escenas = false;
		mScene->changeScene(4);
		break;
	case '5': // Escena
		this->m2Vistas = false;
		this->m2Escenas = false;
		mScene->changeScene(5);
		break;
	case '6': // Escena
		this->m2Vistas = false;
		this->m2Escenas = false;
		mScene->changeScene(6);
		break;
	default:
		need_redisplay = false;
		break;
	}

	if (need_redisplay)
		glutPostRedisplay(); // marks the window as needing to be redisplayed -> calls to display()
}
//-------------------------------------------------------------------------

void IG1App::specialKey(int key, int x, int y) 
{
	bool need_redisplay = true;
	int mdf = glutGetModifiers(); // returns the modifiers (Shift, Ctrl, Alt)
	
	switch (key) {
	case GLUT_KEY_RIGHT:
		if (mdf == GLUT_ACTIVE_CTRL)
			mCamera->moveLR(10);
			// mCamera->pitch(-1); // rotates -1 on the X axis
		else
			mCamera->moveLR(-10);
			// mCamera->pitch(1); // rotates 1 on the X axis
		break;
	case GLUT_KEY_LEFT:
		if (mdf == GLUT_ACTIVE_CTRL)
		    mCamera->moveUD(10);
			// mCamera->yaw(1);      // rotates 1 on the Y axis 
		else 
			mCamera->moveUD(-10);
			// mCamera->yaw(-1);     // rotate -1 on the Y axis
		break;
	case GLUT_KEY_UP:
		mCamera->moveFB(10);
		// mCamera->roll(1);    // rotates 1 on the Z axis
		break;
	case GLUT_KEY_DOWN:
		mCamera->moveFB(-10);
		// mCamera->roll(-1);   // rotates -1 on the Z axis
		break;
	default:
		need_redisplay = false;
		break;
	}

	if (need_redisplay)
		glutPostRedisplay(); // marks the window as needing to be redisplayed -> calls to display()
}
//-------------------------------------------------------------------------

