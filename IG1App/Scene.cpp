#include "Scene.h"
#include "CheckML.h"
#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>

using namespace glm;

//-------------------------------------------------------------------------
// Método para cambiar la escena.
void Scene::changeScene(int id) {
	
	// Puede cargarse cualquier escena de la P2 o alguna de las dos de la P1.
	if ((part == 1 && id <= 2) || part == 2) {
		this->free(); // Libera los objetos y las texturas.
		this->mId = id; // Indica la escena activa.
		this->resetGL(); // Reinicia la configuración de OpenGL.
		this->init(); // Cargamos los elementos de la escena.
	}

}
//-------------------------------------------------------------------------
// Método que se llama para que las entidades se actualicen.
void Scene::update() {
	for (Abs_Entity* obj : gObjects)
		obj->update();
}
//-------------------------------------------------------------------------
void Scene::loadTextures() {

	if (this->part == 2) { // P2

		/*
		switch (this->mId)
		{
			default:
			break;
		}
		*/

	} else if(this->part == 1) { // P1

		switch (this->mId)
		{

			case 2: // Texturas de la escena de la cristalera.

				// Textura del suelo.
				Texture* t0 = new Texture();
				t0->load("..\\Bmps\\baldosaC.bmp");
				gTextures.push_back(t0);

				// Textura de la estrella.
				Texture* t1 = new Texture();
				t1->load("..\\Bmps\\baldosaP.bmp");
				gTextures.push_back(t1);

				// Textura externa e interna del contenedor/cubo.
				Texture* t2 = new Texture();
				t2->load("..\\Bmps\\container.bmp");
				gTextures.push_back(t2);
				Texture* t3 = new Texture();
				t3->load("..\\Bmps\\papelE.bmp");
				gTextures.push_back(t3);

				// Textura de la planta.
				Texture* t4 = new Texture();
				t4->load("..\\Bmps\\grass.bmp", glm::u8vec3(0, 0, 0), 0);
				gTextures.push_back(t4);

				// Textura de la cristalera translúcida.
				Texture* t5 = new Texture();
				t5->load("..\\Bmps\\windowV.bmp", 122);
				gTextures.push_back(t5);

			break;

		}

	}

}
// ------------------------------------------------------------------------
void Scene::configureLights()
{

	if(part == 2)

		switch (this->mId)
		{
			case 5: // Cubo
				this->spotLight->setPosDir({ 0, 0, 190 }); 
			break;
			default:
				// Planeta, TIEs y otros.
				this->spotLight->setPosDir({ 0, 0, 300 });
		}

}
// ------------------------------------------------------------------------
void Scene::loadEntities() {

	// Ejes X, Y, Z.
	gObjects.push_back(new EjesRGB(400.0));

	if (this->part == 2) { // P2

		switch (this->mId)
		{			
			case 1:
		
				// TIE Fighter compuesto.
				gObjects.push_back(new TieFighter());

			break;
			case 2:
			
				// Anillo cuadrado.
				gObjects.push_back(new AnilloCuadrado());

			break;
			case 3:
			
				// Cubo verde.
				gObjects.push_back(new Cubo(200));
				gObjects.back()->setColor({ 0.0, 1.0, 0.0, 0.0 });

			break;
			case 4:
			
				// Esfera (sólido por revolución).
				gObjects.push_back(new Esfera(100, 50, 50));
				gObjects.back()->setColor(dvec4(1.0, 0.0, 0.0, 1.0));
				gObjects.back()->setModelMat(translate(dmat4(1), dvec3(250.0, 0.0, 0.0)));

				// Esfera (entidad cuádrica).
				gObjects.push_back(new Sphere(100.0));
				gObjects.back()->setModelMat(translate(dmat4(1), dvec3(0.0, 0.0, 250.0)));

				// Cono.
				gObjects.push_back(new Cone(200, 100, 60));

			break;
			case 5:
			{

				// Cubo con texturas.
				gObjects.push_back(new GridCube(200, 100));

				// gObjects.push_back(new Piramide(100, 120));

			} 
			break;
			case 6:

				// Esfera (sólido por revolución).
				// Material de la esfera.
				Material* material = new Material();
				material->setBrass();
				// Radio de la esfera.
				GLdouble radio = 250;
				// Declaración de la esfera.
				Esfera* esfera = new Esfera(radio, 300, 300);
				// Color o material de la esfera.
				esfera->setMaterial(material);
				esfera->setColor(dvec4(0.0, 1.0, 1.0, 1.0));
				// Añadimos la esfera a los objetos de la escena.
				gObjects.push_back(esfera);

				// Escuadrón de cazas TIE.
				gObjects.push_back(new TieFighterSquadron(radio + 50));

			break;
			//default:

				/*
				// Cono sobre esfera

				// Esfera de radio 100.
				Sphere* esfera = new Sphere(100.0);
				gObjects.push_back(esfera);

				// Cono (radio base, radio tope, altura)
				Cylinder* cono = new Cylinder(50.0, 0, 100.0);
				glm::dmat4 mAux = cono->modelMat();
				mAux = translate(mAux, dvec3(0, 85, 0));
				mAux = rotate(mAux, radians(-90.0), dvec3(1.0, 0, 0));
				cono->setModelMat(mAux);
				gObjects.push_back(cono);
				*/

				/*
				// Disco y DiscoParcial.

				Disk* disco = new Disk(100.0, 200);
				gObjects.push_back(disco);

				PartialDisk* partialDisk = new PartialDisk(100.0, 200, 0.0, 360.0);
				gObjects.push_back(partialDisk);
				*/

				// Sombrero con gorrito.
				// gObjects.push_back(new CapAndHat());
				
				// Rotor.
				// gObjects.push_back(new Rotor(100, 50));

				// Toroide.
				// gObjects.push_back(new Toroide(50, 50, 50));
				
				// Tornillo.
				// gObjects.push_back(new Tornillo(50));

				/*
				// Material de la esfera.
				Material* material = new Material();
				material->setBrass();

				// Radio de la esfera.
				GLdouble radio = 250;
				// Declaración de la esfera.
				Esfera* planeta = new Esfera(radio, 300, 300);
				// Color o material de la esfera.
				planeta->setMaterial(material);
				planeta->setColor(dvec4(0.0, 1.0, 1.0, 1.0));
				// Añadimos la esfera a los objetos de la escena.
				gObjects.push_back(planeta);

				// Faro.
				gObjects.push_back(new Faro(radio));
				*/

				/*
				// Material de la esfera.
				Material* material = new Material();
				material->setBrass();

				// Radio de la esfera.
				GLdouble radio = 250;
				// Declaración de la esfera.
				Esfera* planeta = new Esfera(radio, 300, 300);
				// Color o material de la esfera.
				planeta->setMaterial(material);
				planeta->setColor(dvec4(0.0, 1.0, 1.0, 1.0));
				// Añadimos la esfera a los objetos de la escena.
				gObjects.push_back(planeta);

				// Faro.
				gObjects.push_back(new Faro(radio));
				*/

				/*
				// Suelo
				gObjects.push_back(new Grid(500, 500));

				// Tornillo.
				gObjects.push_back(new TornilloConLuz());
				*/

			//break;

		}

	} else if(part == 1) { // P1

		switch (this->mId)
		{
			case 1:
			{
				
				// Sierpinsky

				Poligono* circulo = new Poligono(3300, 300);
				circulo->setColor(dvec4(1.0, 0.0, 1.0, 1));
				gObjects.push_back(circulo);

				Sierpinski* spk = new Sierpinski(3000, 300);
				spk->setColor(dvec4(1.0, 1.0, 0.0, 1));
				gObjects.push_back(spk);

				RectanguloRGB* r = new RectanguloRGB(800.0, 600.0);
				r->setModelMat(translate(dmat4(1), dvec3(0.0, 0.0, -100.0)));
				gObjects.push_back(r);

				TrianguloRGB* g = new TrianguloRGB(40);
				g->setColor(glm::dvec4(0.5, 0.5, 0.0, 1.0));
				g->setModelMat(glm::translate(dmat4(1), dvec3(300.0, 0.0, 0.0))); // 2º
				g->setModelMat(glm::rotate(g->modelMat(), radians(-25.0), dvec3(0, 0, 1))); // 1º
				gObjects.push_back(g);

			}
			break;
			case 2:
			{
				
				// Cristalera

				// Suelo con textura.
				GLuint rw = 800 / gTextures[0]->width();
				GLuint rh = 600 / gTextures[0]->height();
				gObjects.push_back(new Suelo(600, 600, rw, rh));
				gObjects.back()->setTexture(gTextures[0]);

				// Estrella 3D con textura.
				Estrella3D* s3d = new Estrella3D(100, 8, 40);
				s3d->setTexture(gTextures[1]);
				s3d->setCoordinates(dvec3(-200.0, 250.0, -200.0));
				s3d->setModelMat(translate(dmat4(1), s3d->getCoordinates()));
				gObjects.push_back(s3d);

				// Caja con fondo y texturas.
				CajaConFondo* ccf = new CajaConFondo(100);
				ccf->setTexture(gTextures[2]);
				ccf->setTextureAux(gTextures[3]);
				ccf->setModelMat(translate(dmat4(1), dvec3(-200.0, 50.1, -200.0)));
				gObjects.push_back(ccf);

				// Fotografia de la escena.
				Foto* f = new Foto(300, 200);
				f->setModelMat(glm::translate(dmat4(1), dvec3(0.0, 0.1, 0.0))); // 2º
				f->setModelMat(glm::rotate(f->modelMat(), radians(90.0), dvec3(1, 0, 0))); // 1º
				gObjects.push_back(f);

				// Planta (hierbas).
				Planta* p = new Planta(100, 180);
				p->setTexture(gTextures[4]);
				p->setModelMat(translate(dmat4(1), dvec3(200.0, 90.0, -200.0)));
				gObjects.push_back(p);

				// Cristalera translúcida.
				ContCubo* ct = new ContCubo(600);
				ct->setTexture(gTextures[5]);
				ct->setTextureAux(gTextures[5]);
				ct->setModelMat(translate(dmat4(1), dvec3(0.0, 300.0, 0.0)));
				gObjects.push_back(ct);

			}
			
		}

	}

}
//-------------------------------------------------------------------------
void Scene::init()
{ 

	// Settings.
	setGL();

	// Textures.
	loadTextures();

	// Ajusta las luces.
	configureLights();

	// Entidades.
	loadEntities();

}
//-------------------------------------------------------------------------
void Scene::free() 
{ 
	// Vaciamos el array de objetos a mostrar en la escena (liberamos memoria).   
	for (Abs_Entity* el : gObjects)
	{
		delete el;
		el = nullptr;
	}
	gObjects.clear(); 

	// Vaciamos el array de texturas (liberamos memoria).
	for (Texture* tx : gTextures)
	{
		delete tx;
		tx = nullptr;
	}
	gTextures.clear();
}
//-------------------------------------------------------------------------
void Scene::setGL() 
{

	// Habilita el test de profundidad (este test evita que se dibuje aquello que no se ve).
	glEnable(GL_DEPTH_TEST);

	// Habilita el modo de trabajo con texturas 2D.
	glEnable(GL_TEXTURE_2D);

	if (part == 2) { // P2

		// Activa la iluminación.
		glEnable(GL_LIGHTING);

		/*
		Activa la normalización de los vectores normales, nosotros ya lo hacemos, pero
		este comando norma liza además otros vectores que tienen que ser normales y con
		los que no tratamos directamente.
		*/
		glEnable(GL_NORMALIZE);

		switch (this->mId)
		{
			case 1:
				// TIE Fighter (fondo negro).
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			break;
			case 2:
				// AnilloCuadrado (fondo SkyBlue).
				glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
			break;
			case 3:
				// Cubo con tapas (fondo SkyBlue).
				glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
			break;
			case 4:
				// Cubo con tapas (fondo SkyBlue).
				glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
			break;
			case 5:
				// Cubo con tapas (fondo SkyBlue).
				glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
			break;
			case 6:
				// Escuadron de TIE's sobre la esfera (fondo negro).
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			break;
		}

	} else if (part == 1) { // P1

		switch (this->mId)
		{
			case 1:
				// Sierpinsky (fondo negro).
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			break;
			case 2:
				// Cristalera (fondo blanco).
				glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		}

	}

}
//-------------------------------------------------------------------------
void Scene::resetGL() 
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);
}
//-------------------------------------------------------------------------
void Scene::render(Camera const& cam) const 
{
	// Luz direccional definida manualmente.
	// P2: Fighter, AnilloCuadrado, Cubo y Esferas. 
	// this->sceneDirLight(cam);

	// Luz direccional de la escena.
	if(this->dirLight != nullptr)
		this->dirLight->upload(cam.viewMat());

	// Luz posicional de la escena.
	if (this->posLight != nullptr)
		this->posLight->upload(cam.viewMat());

	// Foco de luz.
	if (this->spotLight != nullptr)
		this->spotLight->upload(cam.viewMat());

	// Transfiere el viewport, la view matrix y la projection matrix a la GPU.
	cam.upload();

	// Renderiza todos los objetos de la escena.
	for (Abs_Entity* el : gObjects)
		el->render(cam.viewMat());
}
//-------------------------------------------------------------------------
void Scene::saveTheFoto()
{
	// Obtiene la textura de la foto y la guarda en el disco.
	if(this->part == 1 && this->mId == 2)
		gTextures[4]->save("../Bmps/Foto.bmp");
}
//-------------------------------------------------------------------------
void Scene::sceneDirLight(Camera const& cam) const {
	
	// Esta función define manualmente una luz direccional.

	// setGL();
	glEnable(GL_LIGHTING);
	
	// DirLight();
	glEnable(GL_LIGHT0);
	
	// dirLight->upload(cam.viewMat());
	glm::fvec4 posDir = { 1, 1, 1, 0 };
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(cam.viewMat()));
	glLightfv(GL_LIGHT0, GL_POSITION, value_ptr(posDir));
	
	// dirLight->upload(cam.viewMat());
	// upLoadL();
	glm::fvec4 ambient = { 0, 0, 0, 1 };
	glm::fvec4 diffuse = { 1, 1, 1, 1 };
	glm::fvec4 specular = { 0.5, 0.5, 0.5, 1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, value_ptr(ambient));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, value_ptr(diffuse));
	glLightfv(GL_LIGHT0, GL_SPECULAR, value_ptr(specular));

}
//-------------------------------------------------------------------------
void Scene::turnLightState(int light, bool enable)
{

	switch (light)
	{
		case 0:

			if (dirLight != nullptr)
				if(enable)
					dirLight->enable();
				else
					dirLight->disable();
		
		break;
		case 1:
		
			if (posLight != nullptr) 
				if (enable)
					posLight->enable();
				else
					posLight->disable();
			
		break;
		case 2:

			if (spotLight != nullptr) 
				if (enable)
					spotLight->enable();
				else
					spotLight->disable();

		break;
	}

}
//-------------------------------------------------------------------------
void Scene::disableDefaultAmbientLight(bool disable)
{
	/*
	Color de la luz ambiente global: por defecto es (0.2, 0.2, 0.2, 1.0),
	por lo que la escena está pues siempre algo iluminada, a menos que no
	haya fuentes de luz y esta componente ambiente global se haya definido
	de color negro, en este caso, todo se verá negro salvo el fondo.
	*/
	float value = (disable) ? 0.0f : 0.2f;
	glm::fvec4 amb = { value, value, value, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, value_ptr(amb));
}
//-------------------------------------------------------------------------
void Scene::orbita() {

	// Escena planeta y TIEs.
	if (this->mId == 6) {
		TieFighterSquadron* tfs = (TieFighterSquadron*)gObjects.back();
		tfs->setOrbitOrRotate(true);
		this->update();
	}

}
//-------------------------------------------------------------------------
void Scene::rotate() {
	
	// Escena planeta y TIEs.
	if (this->mId == 6) {
		TieFighterSquadron* tfs = (TieFighterSquadron*)gObjects.back();
		tfs->setOrbitOrRotate(false);
		this->update();
	}

}
//-------------------------------------------------------------------------
void Scene::TurnLightsOn()
{

	// Escena planeta y TIEs.
	if (this->mId == 6) {
		TieFighterSquadron* tfs = (TieFighterSquadron*) gObjects.back();
		tfs->setLightsOn();
	}

	// Escena planeta y faro.
	if (this->mId == 5) {
		// Faro* f = (Faro*)gObjects.back();
		// f->setLightOn();
		// TornilloConLuz* t = (TornilloConLuz*)gObjects.back();
		// t->enableSpot();
	}

}
//-------------------------------------------------------------------------
void Scene::TurnLightsOff()
{
	
	// Escena planeta y TIEs.
	if (this->mId == 6) {
		TieFighterSquadron* tfs = (TieFighterSquadron*)gObjects.back();
		tfs->setLightsOff();
	}

	// Escena planeta y faro.
	if (this->mId == 5) {
		// Faro* f = (Faro*)gObjects.back();
		// f->setLightOff();
		// TornilloConLuz* t = (TornilloConLuz*)gObjects.back();
		// t->disableSpot();
	}

}
//-------------------------------------------------------------------------
void Scene::changePart()
{
	// Para parte nueva cambiar el "2".
	this->part = (part < 2) ? (part + 1) : 1;
	this->changeScene(1);
}
//-------------------------------------------------------------------------