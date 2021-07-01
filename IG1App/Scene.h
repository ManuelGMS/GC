//#pragma once
#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Camera.h"
#include "Entity.h"
#include "Texture.h"

#include <vector>

//-------------------------------------------------------------------------

class Scene	
{ 
public:
	
	Scene() {
		
		// Escena a mostrar por defecto.
		this->mId = 1;

		// Parte de la asignatura por defecto.
		this->part = 2;

		/*
		
		ILUMINACION:
		
		* La iluminaci�n dicta c�mo se refleja y dispersa la luz cuando incide sobre una superficie.

		* Lo �nico que cambia con la iluminaci�n son los v�rtices. Son los �nicos que se ven afectados 
		por la luz, el resto de puntos y c�mo se ven afectados se saca por interpolaci�n. Por lo tanto,
		la iluminaci�n de una figura es m�s realista cuantos m�s v�rtices la compongan, porque si da en
		una zona de una figura en la que no hay ning�n v�rtice, entonces no se va a iluminar nada.

		* En OpenGL, la luz no se detiene aunque haya algo delante, o dicho de otra forma, en OpenGL los
		objetos no absorven la luz.
		
		* Los vectores normales se usan para la dispersi�n y la reflexi�n de la luz.
			+ Dispersi�n DIFUSA: es la luz que se refleja (reirradia) en todas las direcciones.
				++ Si un objeto reiradia todo el verde y absorve el azul y el rojo, lo que ver�amos es ese verde.
				++ La cantidad de luz que reiradia depende de lo que est� hecho el objeto (color).
			+ Reflexi�n ESPECULAR: luz que releja la superficie (en una direcci�n).

		* Luz que le llega a un v�rtice: Difusa (diff) + Especular (spec) + Ambiente (amb)
			+ Luz ESPECULAR: tiene que ver con el material (superficie) de los objetos.
			+ Luz DIFUSA: color de los rayos que emite la fuente de luz.
			+ Luz AMBIENTE: define el color de los rayos que no llegan directamente al objeto, llega a todos los objetos por igual.
			+ La componente ambiente y difusa de una luz no deber�an ser iguales.
			+ Valores por defecto:
				++ ambiente (0,0,0,1) --> Donde no ilumina directamente la fuente de luz, se ilumina en negro.
				++ difusa --> (1,1,1,1) --> Los rayos de luz emitidos por la fuente de luz son blancos.
				++ especular --> (0.5,0.5,0.5,1) --> Para que la luz no refleje mucho.

		* Tipos de fuentes de luz:
			+ Direccionales (o remotas) --> DirLight:
				++ Est�n infinitamente lejos y, por tanto, sus rayos de luz llegan paralelos a la escena.
				++ Por su lejan�a infinita se supone que no se aten�an. 
				++ Se especifican mediante el vector que va del origen a la fuente de luz.
			+ Posicionales (o locales) --> PosLight y SpotLight:
				++ Est�n localizadas en un punto y, por tanto, los objetos de la escena son m�s o menos iluminados seg�n su exposici�n.
				++ Se especifican mediante el punto en el que se encuentra la fuente de luz.
				++ Estas fuentes admiten atenuaci�n.
				++ Tipos:
					+++ Focos:
						++++ Tienen un vector que indica la direcci�n de la emisi�n.
						++++ Un �ngulo que nos dice la amplitud de la emisi�n.
						++++ Un �ngulo que sirve para definir la concentraci�n de la luz del foco (0 -> misma luz en todos los puntos).
					+++ Omnidireccionales: 
						++++ Emiten luz por igual en todas direcciones.
			+ NOTAS:
				++ OpenGL distingue si una fuente de luz es remota (0) o local (!= 0) por la cuarta componente su posici�n.
				++ Por defecto todas las fuentes de luz tienen posici�n (0, 0, 1, 0), es decir, son direccionales y miran a +Z.
				++ Las coordenadas de una fuente de luz posicional son mundiales.

		* Si hacemos alg�n cambio de alguna caracter�stica de la luz, hay que hacer upload para que se refleje el cambio.

		* Material: OpenGL simula materiales que reaccionan ante la luz (R,G,B) de cada componente (diff, spec, amb) de la fuente de luz.
			+ Ejem: Una superficie con componentes difusa y ambiente verde (0, 1, 0) bajo una luz con componentes difusa y ambiente 
			roja (1, 0, 0) resulta ser (0*1, 1*0, 0*0) = (0, 0, 0).
			+ Los materiales se especifican con las mismas componentes (diff, spec, amb) que las fuentes de luz.
			+ Para los materiales, las anteriores componentes se llaman coeficientes de reflexi�n o reflectancias del material.
			+ Cada coeficiente del material indica c�mo reacciona la superficie ante la correspondiente componente de cada fuente de luz. 
				++ Ejem: Una superficie con reflexi�n especular alto parecer� brillante bajo una fuente de luz con componente especular alta.
			+ Los coeficientes de reflexi�n ambiente y difuso son los responsables del color, en los materiales estos se suelen tomar iguales.
			+ El coeficiente especular es responsable de los reflejos que tiene la superficie, en los materiales se suele tomar gris de esta
			forma el color de la superficie no se ve alterado por el color de la luz incidente.

		* Recuerda no crear comandos de luz, si no objetos relacionados con la luz.

		* En OpenGL podemos hacer uso de un m�ximo de 8 luces.

		*/
		
		// Luz direccional (LIGHT0).
		this->dirLight = new DirLight();
		this->dirLight->setPosDir({ 1, 1, 1 });
		this->dirLight->setAmb({ 0, 0, 0, 1 });
		this->dirLight->setDiff({ 1, 1, 1, 1 });
		this->dirLight->setSpec({ 0.5, 0.5, 0.5, 1 });

		// Luz posicional (LIGHT1).
		this->posLight = new PosLight();
		this->posLight->setPosDir({ 300, 300, 0 });
		this->posLight->setAmb({ 0, 0, 0, 1 });
		this->posLight->setDiff({ 0, 1, 0, 1 });
		this->posLight->setSpec({ 0.5, 0.5, 0.5, 1 });

		// Foco de luz (LIGHT2).
		this->spotLight = new SpotLight();
		this->spotLight->setAmb({ 0, 0, 0, 1 });
		this->spotLight->setDiff({ 1, 1, 1, 1 });
		this->spotLight->setSpec({ 0.5, 0.5, 0.5, 1 });

		/*
		1�) Vector que indica la direcci�n en la que apunta el foco.
		2�) Cutoff (�ngulo de corte): Especifica el �ngulo del cono de luz (/|\), todo lo que queda fuera de el no es iluminado.
		3�) Exponent (Distribuci�n de la luz): 0 Es un valor especial para indicar que la intensidad de la luz es igual para toda la base.
		*/
		this->spotLight->setSpot({ 0, 0, -1 }, 45, 0);
		
	};
	
	~Scene() { 
		
		// Libera la memoria de todos los recursos de la escena.
		free(); 

		// Deshabilita todas las configuraciones establecidas.
		resetGL(); 

		// Liberamos la memoria de las luces de la escena.
		if (dirLight != nullptr) {
			delete this->dirLight;
			this->dirLight = nullptr;
		}
		if (posLight != nullptr) {
			delete this->posLight;
			this->posLight = nullptr;
		}
		if (spotLight != nullptr) {
			delete this->spotLight;
			this->spotLight = nullptr;
		}

	};

	Scene(const Scene& s) = delete;  // no copy constructor
	Scene& operator=(const Scene& s) = delete;  // no copy assignment
		
	void init();

    void render(Camera const& cam) const;
	
	// A�ade a la clase Scene un m�todo que indique a las entidades que se actualicen.
	void update();

	// Crea un m�todo para cambiar el atributo mId, este elimina la escena actual y genera la nueva.
	// La escena 2D es la escena 0, y con la tecla 1 cambiamos a la escena 1 (la primera escena 3D) que de
	// momento contiene los ejes RGB.
	void changeScene(int id);

	// M�todo para guardar la foto en el disco.
	void saveTheFoto();

	// M�todo para iluminar la escena 3 (TIE FIghter)
	void sceneDirLight(Camera const& cam) const;

	// M�todo para encender o apagar una luz.
	void turnLightState(int light, bool enable);

	// M�todo para oscurecer completamente la escena.
	void disableDefaultAmbientLight(bool disable);

	// Orbita el escuadr�n de cazas TIE.
	void orbita();

	// Rota el escuadr�n de cazas TIE.
	void rotate();

	// Enciende las luces de los cazas TIE.
	void TurnLightsOn();
	
	// Apaga las luces de los cazas TIE.
	void TurnLightsOff();

	// Cambia de la P2 a la P1 de GC.
	void changePart();

	// Para establecer la parte de GC en la que nos movemos.
	void setPart(int part) { this->part = part; }

	// Para capturar la parte de GC en la que nos movemos.
    int getPart() { return this->part; }

private:

	void free();
	void setGL();
	void resetGL();

	// M�todo donde se cargan las entidades. 
	void loadEntities();

	// M�todo donde se cargan las texturas.
	void loadTextures();

	// M�todo para ajustar las luces.
	void configureLights();

	// Identificador de la escena.
	int mId;

	// Para cambiar de la P2 a la P1 de GC (o a otras partes).
	int part;

	// Luces de la escena.
	DirLight* dirLight = nullptr;
	PosLight* posLight = nullptr;
	SpotLight* spotLight = nullptr;

	// Vector que contiene los elementos de la escena.
	std::vector<Abs_Entity*> gObjects;

	// Vector que contiene las texturas de los objetos.
	std::vector<Texture*> gTextures;

};
//-------------------------------------------------------------------------

#endif //_H_Scene_H_

