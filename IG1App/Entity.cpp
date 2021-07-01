#include "Entity.h"
#include "IG1App.h"

#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>

using namespace glm;

//-------------------------------------------------------------------------

void Abs_Entity::upload(dmat4 const& modelViewMat) const 
{ 
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(modelViewMat));  // transfers modelView matrix to the GPU
}

//-------------------------------------------------------------------------

EjesRGB::EjesRGB(GLdouble l): Abs_Entity()
{
	mMesh = Mesh::createRGBAxes(l);
}

EjesRGB::~EjesRGB() 
{ 
	delete mMesh; 
	mMesh = nullptr; 
};

void EjesRGB::render(dmat4 const& modelViewMat) const 
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glLineWidth(2);
		glEnable(GL_COLOR_MATERIAL);
		mMesh->render();
		glDisable(GL_COLOR_MATERIAL);
		glLineWidth(1);
	}
}

//-------------------------------------------------------------------------

Poligono::Poligono(GLint numL, GLdouble rd) : Abs_Entity() {
	mMesh = Mesh::generaPoligono(numL, rd);
}

Poligono::~Poligono()
{
	delete mMesh; 
	mMesh = nullptr;
};

void Poligono::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		// Configurar las propiedades de las primitivas.
		glColor3d(mColor.r, mColor.g, mColor.b);
		glLineWidth(2);

		mMesh->render(); // Se ordena el dibujado en la escena.

		// Hay que reestabecer la configuración por defecto (Color blanco y ancho de línea 1).
		glColor3d(1, 1, 1);
		glLineWidth(1);
	}
}

//-------------------------------------------------------------------------

Sierpinski::Sierpinski(GLuint numP, GLdouble rd) : Abs_Entity() {
	mMesh = Mesh::generaSierpinski(rd, numP);
}

Sierpinski::~Sierpinski()
{
	delete mMesh;
	mMesh = nullptr;
};

void Sierpinski::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		// Configurar las propiedades de las primitivas.
		glPointSize(2);
		glColor4dv(value_ptr(mColor));

		mMesh->render(); // Se ordena el dibujado en la escena.

		// Hay que reestabecer la configuración por defecto (Color blanco y ancho del punto 1).
		glPointSize(1);
		glColor4d(1, 1, 1, 1); // glColor4dv(value_ptr(glm::dvec4(1,1,1,1)));
	}
}

//-------------------------------------------------------------------------

TrianguloRGB::TrianguloRGB(GLdouble rd) : Abs_Entity() {
	this->anguloSobreSi = 0;
	this->anguloEnCirculo = 0;
	mMesh = Mesh::generaTrianguloRGB(rd);
}

TrianguloRGB::~TrianguloRGB()
{
	delete mMesh;
	mMesh = nullptr;
};

void TrianguloRGB::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		upload(aMat);

		// Configurar las propiedades de las primitivas.
		// Establece el modo de rasterización (Generación de gragmentos internos. Por defecto GL_FRONT_AND_BACK con GL_FILL).
		glPolygonMode(GL_BACK, GL_LINE);
		// glPolygonMode(GL_BACK, GL_POINT);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		mMesh->render(); // Se ordena el dibujado en la escena.

		// Hay que reestabecer la configuración por defecto.
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void TrianguloRGB::update()
{
	// Angulos de giro
	this->anguloSobreSi -= 1;
	this->anguloEnCirculo += 2;
	// Transformaciones
	GLdouble x = 0 /*centro*/ + 300 /*radio*/ * cos(radians(anguloEnCirculo)); // X Formula de la circunferencia.
	GLdouble y = 0 /*centro*/ + 300 /*radio*/ * sin(radians(anguloEnCirculo)); // Y Formula de la circunferencia.
	this->mModelMat = translate(dmat4(1), dvec3(x, y, 0.0)); // 2º) Mover en (X, Y, Z)
	this->mModelMat = rotate(this->modelMat(), radians(anguloSobreSi), dvec3(0.0, 0.0, 1.0)); // 1º) Rotar en (X, Y, Z)
}

//-------------------------------------------------------------------------

RectanguloRGB::RectanguloRGB(GLdouble w, GLdouble h) : Abs_Entity() {
	mMesh = Mesh::generaRectanguloRGB(w, h);
}

RectanguloRGB::~RectanguloRGB()
{
	delete mMesh;
	mMesh = nullptr;
};

void RectanguloRGB::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		// Configurar las propiedades de las primitivas.
		// Establece el modo de rasterización (Generación de gragmentos internos. Por defecto GL_FRONT_AND_BACK con GL_FILL).
		// glPolygonMode(GL_BACK, GL_LINE); // Muestra las aristas.

		mMesh->render(); // Se ordena el dibujado en la escena.

		// Hay que reestabecer la configuración por defecto.
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

//-------------------------------------------------------------------------

Estrella3D::Estrella3D(GLdouble re, GLuint np, GLdouble h) : Abs_Entity() {
	this->anguloEnY = 0; // Para decidir el sentido del giro entorno al eje Y.
	this->anguloEnZ = 0; // Para decidir el sentido del giro entorno al eje Z.
	this->coordinates = glm::dvec3(0, 0, 0); // Coordenadas en las que la estrella se posiciona por defecto.
	// mMesh = Mesh::generaEstrella3D(re, np, h);
	mMesh = Mesh::generaEstrellaTexCor(re, np, h);
}

Estrella3D::~Estrella3D()
{
	delete mMesh;
	mMesh = nullptr;
};

void Estrella3D::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
	
		dmat4 aMat;

		//******************************************************************************************

		aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		// Configurar las propiedades de las primitivas.
		glColor4d(0.5, 0.5, 0.5, 1);
		
		/*
		Activa la textura actual (ahora podemos trabajar con ella) y establece el modo de mezcla de colores.

		Color de la figura (entidad) C; Coordenadas de textura (s, t); T(s,t) = Color (texel) de las coordenadas (s,t)
		GL_REPLACE: Utilizar exclusivamente el color de la textura --> Cr = T(s,t)
		GL_MODULATE: Modular ambos colores --> Cr = C * T(s,t)
		GL_ADD: Sumar ambos colores --> Cr = C + T(s,t)
		*/
		mTexture->bind(GL_REPLACE);

		mMesh->render(); // Se ordena el dibujado en la escena.

		//******************************************************************************************

		aMat = modelViewMat * rotate(this->modelMat(), radians(180.0), dvec3(1.0, 0.0, 0.0));
		upload(aMat);

		mMesh->render(); // Se ordena el dibujado en la escena.

		// Desactiva la textura activa.
		mTexture->unbind();

		// Hay que reestabecer la configuración por defecto.
		glColor4d(1, 1, 1, 1);

	}
}

void Estrella3D::update()
{
	// Para girar entorno a Y y Z.
	this->anguloEnY += 1;
	this->anguloEnZ += 1; 
	// Transformaciones (Se escriben al revés de como se hacen --> LIFO).
	this->mModelMat = translate(dmat4(1), this->getCoordinates()); // 3º) Posicionar la figura.
	this->mModelMat = rotate(this->mModelMat, radians(this->anguloEnY), dvec3(0.0, 1.0, 0.0)); // 2º) Rotar en Y.
	this->mModelMat = rotate(this->mModelMat, radians(this->anguloEnZ), dvec3(0.0, 0.0, 1.0)); // 1º) Rotar en Z.
}

void Estrella3D::setCoordinates(glm::dvec3 coords)
{
	this->coordinates = coords;
}

glm::dvec3 Estrella3D::getCoordinates()
{
	return this->coordinates;
}

//-------------------------------------------------------------------------

ContCubo::ContCubo(GLdouble ld) : Abs_Entity() {
	// mMesh = Mesh::generaContCubo(ld);
	mMesh = Mesh::generaContCuboTexCor(ld);
}

ContCubo::~ContCubo()
{
	delete mMesh;
	mMesh = nullptr;
};

void ContCubo::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {

		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		glColor4d(0.5, 0.5, 0.5, 1); // Color de la figura.
		glEnable(GL_CULL_FACE); // Habilitamos la "selección" de "caras" (FRONT o BACK).
		glDepthMask(GL_FALSE); // Desabilita la escritura en el buffer de profundidad (solo se escribe en el color buffer).
		glCullFace(GL_BACK); // Indicamos a OpenGL que no queremos que pinte la cara BACK.
		glEnable(GL_BLEND); // Activamos el blending para poder dar transparencia a los objetos.

		/*
		BLENDING (mezcla) es el nombre de la técnica para dar transparencia a los objetos.

		Esta técnica consiste en que los objetos no tienen un color sólido, sino que tienen
		una combinación de colores del propio objeto y de cualquier otro objeto que esté detrás.

		¿Una ventana con un cristal de color es un objeto transparente? El cristal tiene un color
		propio, pero el color resultante contiene también los colores de todos los objetos que
		hay detrás del cristal. De ahí viene el nombre de blending, pues mezclamos varios colores
		de píxeles (de diferentes objetos) en un solo color.

		La mezcla de los dos colores se obtiene dependiendo de los factores de blending que estén establecidos.
		Ecuación del BLENDING --> dstColor = srcBFactor * srcColor + dstBFactor * dstColor
		srcColor = (srcR, srcG, srcB, srcA) --> Color RGBA del fragmento en proceso.
		dstColor = (dstR, dstG, dstB, dstA) --> Es el vector de color que se almacena actualmente en el buffer de color.
		srcBFactor y dstBFactor --> Factores de blending.

		Los factores de la ecuación se establecen con: glBlendFunc(srcBFactor, dstBFactor) ambos factores € [0, 1].

		Tipos de blending:
			* Blending --> Suma ponderada de los colores correspondientes a distintos objetos.
				** Ecuación del BLENDING --> dstColor = srcBFactor * srcColor + dstBFactor * dstColor
			* Alpha blending --> Los factores de ponderación se determinan en función de la componente alpha.
				** Ecuación del BLENDING --> dstColor = srcAlpha * srcColor + (1 - srcAlpha) * dstColor
				** Alpha € [0 - 1]
					*** Si Alpha = 1 entonces la opacidad es total y el color del fragmento actual reemplaza al color del buffer.
					*** Si Alpha = 0 entonces la transparencia es total y el color en el buffer de color no se ve alterado.
		*/
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Para poder trabajar con la ecuación del alpha blending.

		mTexture->bind(GL_REPLACE); // Activa la textura actual(ahora podemos trabajar con ella) y establece el modo de mezcla de colores.
		mMesh->render(); // Se ordena el dibujado en la escena.
		mTexture->unbind(); // Desactiva la textura.

		glCullFace(GL_FRONT); // Indicamos a OpenGL que no queremos que pinte la cara FRONT.
		
		mTextureAux->bind(GL_REPLACE); // Activa la textura actual (ahora podemos trabajar con ella) y establece el modo de mezcla de colores.
		mMesh->render(); // Se ordena el dibujado en la escena.
		mTextureAux->unbind(); // Desactiva la textura.
		
		// Hay que reestabecer la configuración por defecto.
		glDisable(GL_BLEND);
		glColor4d(1, 1, 1, 1);
		glDepthMask(GL_TRUE);
		glDisable(GL_CULL_FACE);

	}
}

//-------------------------------------------------------------------------

CajaConFondo::CajaConFondo(GLdouble ld) : Abs_Entity() {
	this->ld = ld;
	this->mModelMatAux = dmat4(1.0);
	// mMesh = Mesh::generaContCubo(ld);
	// mMeshAux = Mesh::generaRectangulo(ld, ld);
	mMesh = Mesh::generaContCuboTexCor(ld);
	mMeshAux = Mesh::generaRectanguloTexCor(ld, ld, 1, 1);
}

CajaConFondo::~CajaConFondo()
{
	// Hay que liberar la malla de la caja y la del fondo.
	delete mMesh; mMesh = nullptr;
	delete mMeshAux; mMeshAux = nullptr;
};

void CajaConFondo::render(dmat4 const& modelViewMat) const
{

	if (mMesh != nullptr) {
		
		dmat4 aMat;

		// ************************************************************************************

		aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		glEnable(GL_CULL_FACE); // Habilitamos la "selección" de "caras" (FRONT o BACK).

		glCullFace(GL_BACK); // Indicamos a OpenGL que no queremos que pinte la "cara" BACK.

		mTexture->bind(GL_REPLACE); // Activa la textura actual (ahora podemos trabajar con ella) y establece el modo de mezcla de colores.
		mMesh->render(); // Se ordena el dibujado en la escena.
		mTexture->unbind(); // Desactiva la textura.

		glCullFace(GL_FRONT); // Indicamos a OpenGL que no queremos que pinte la "cara" FRONT.

		mTextureAux->bind(GL_REPLACE); // Activa la textura actual (ahora podemos trabajar con ella) y establece el modo de mezcla de colores.
		mMesh->render(); // Se ordena el dibujado en la escena.
		mTextureAux->unbind(); // Desactiva la textura.
		
		// ************************************************************************************

		if (mMeshAux != nullptr) {

			// El orden que aplica en OpenGL a las transformaciones es inverso al intuitivo.
			aMat = modelViewMat * translate(mModelMatAux, dvec3(0.0, -ld * 0.5, 0.0)); // 2º) Transladamos
			aMat = rotate(aMat, radians(-90.0), dvec3(1.0, 0.0, 0.0)); // 1º) Rotamos
			upload(aMat);

			glPolygonMode(GL_BACK, GL_FILL); // Rellena.

			glCullFace(GL_FRONT); // Indicamos a OpenGL que no queremos que pinte la cara FRONT.
			
			mTexture->bind(GL_REPLACE); // Activa la textura actual (ahora podemos trabajar con ella) y establece el modo de mezcla de colores.
			mMeshAux->render(); // Se ordena el dibujado del objeto (Rectangulo) en la escena.
			mTexture->unbind(); // Desactiva la textura.

			glCullFace(GL_BACK); // Indicamos a OpenGL que no queremos que pinte la cara BACK.

			mTextureAux->bind(GL_REPLACE); // Activa la textura actual (ahora podemos trabajar con ella) y establece el modo de mezcla de colores.
			mMeshAux->render(); // Se ordena el dibujado del objeto (Rectangulo) en la escena.
			mTextureAux->unbind(); // Desactiva la textura.

		}

		// ************************************************************************************

		// Hay que reestabecer la configuración por defecto.
		glDisable(GL_CULL_FACE);

	}
}

void CajaConFondo::update()
{
	this->mModelMat = rotate(this->mModelMat, radians(1.0), dvec3(0.0, 0.0, 1.0));
	this->mModelMatAux = rotate(this->mModelMatAux, radians(1.0), dvec3(0.0, 0.0, 1.0));
}

//-------------------------------------------------------------------------

Suelo::Suelo(GLdouble w, GLdouble h, GLuint rw, GLuint rh) : Abs_Entity() {
	mMesh = Mesh::generaRectanguloTexCor(w, h, rw, rh);
}

Suelo::~Suelo()
{
	delete mMesh;
	mMesh = nullptr;
};

void Suelo::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {

		dmat4 aMat = modelViewMat * rotate(mModelMat, radians(90.0), dvec3(1.0, 0.0, 0.0));  // glm matrix multiplication
		upload(aMat);

		// Color que modularemos con la textura.
		glColor4d(0.9, 1, 1, 1);
		
		mTexture->bind(GL_MODULATE); // Activa la textura actual(ahora podemos trabajar con ella) y establece el modo de mezcla de colores.
		mMesh->render(); // Se ordena el dibujado en la escena.
		mTexture->unbind(); // Desactiva la textura.

		// Hay que reestabecer la configuración por defecto.
		glColor4d(1, 1, 1, 1);
		
	}
}

//-------------------------------------------------------------------------

Foto::Foto(GLdouble w, GLdouble h) : Abs_Entity()
{
	mTexture = new Texture();
	mMesh = Mesh::generaRectanguloTexCor(w, h, 1, 1);
	mTexture->loadColorBuffer(IG1App::s_ig1app.winWidth(), IG1App::s_ig1app.winHeight(), GL_FRONT);
}

Foto::~Foto()
{
	delete mMesh; mMesh = nullptr;
	delete mTexture; mTexture = nullptr;
};

void Foto::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		
		mTexture->bind(GL_REPLACE); // Activa la textura actual(ahora podemos trabajar con ella) y establece el modo de mezcla de colores.
		mMesh->render(); // Se ordena el dibujado en la escena.
		mTexture->unbind(); // Desactiva la textura.

	}
}

void Foto::update()
{
	mTexture->loadColorBuffer(IG1App::s_ig1app.winWidth(), IG1App::s_ig1app.winHeight(), GL_FRONT);
}

//-------------------------------------------------------------------------

Planta::Planta(GLdouble w, GLdouble h)
{
	mMesh = Mesh::generaRectanguloTexCor(w, h, 1, 1);
}

Planta::~Planta()
{
	delete mMesh;
	mMesh = nullptr;
}

void Planta::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat;
	
	// glDepthMask(GL_FALSE); // Desabilita la escritura en el buffer de profundidad (solo se escribe en el color buffer).
	glEnable(GL_ALPHA_TEST); // Habilita el test Alpha (Este test descarta fragmentos según la comparación especificada).
	glAlphaFunc(GL_GREATER, 0.0); // Solo pasan el test los fragmentos no trasparentes (Alpha > 0).
	
	mTexture->bind(GL_REPLACE); // Activa la textura actual(ahora podemos trabajar con ella) y establece el modo de mezcla de colores.

	for (double angle = 0.0; angle < 180; angle += 45.0) {
		aMat = modelViewMat * rotate(mModelMat, radians(angle), dvec3(0, 1, 0));  // glm matrix multiplication
		upload(aMat);
		mMesh->render(); // Se ordena el dibujado en la escena.
	}
	
	mTexture->unbind(); // Desactiva la textura.
	
	// Hay que reestabecer la configuración por defecto.
	glDepthMask(GL_TRUE);
	glDisable(GL_ALPHA_TEST);
}

//-------------------------------------------------------------------------

Hexagono::Hexagono(GLdouble rd)
{
	mMesh = Mesh::generaPoligonoTexCor(6, rd);
}

Hexagono::~Hexagono()
{
	delete mMesh;
	mMesh = nullptr;
}

void Hexagono::render(glm::dmat4 const& modelViewMat) const
{
	// Envía las coordenadas de cámara.
	upload(modelViewMat * mModelMat);

	glColor4f(0.0f, 0.25f, 0.42f, 0.75f); // Color de la figura.
	glEnable(GL_COLOR_MATERIAL); // Indica que el material de la figura viene definido por su color.
	glDepthMask(GL_FALSE); // Deshabilita la escritura en el buffer de profundidad (solo se escribe en el color buffer).
	glEnable(GL_BLEND); // Activamos el blending para poder dar transparencia a los objetos.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Para hacer alpha blending (transparentar).

	mTexture->bind(GL_MODULATE); // Activa la textura actual(ahora podemos trabajar con ella) y establece el modo de mezcla de colores.
	mMesh->render(); // Se ordena el dibujado en la escena.
	mTexture->unbind(); // Desactiva la textura.

	// Hay que reestabecer la configuración por defecto.
	glDisable(GL_COLOR_MATERIAL);
	glColor4f(1.0f, 1.0f, 1.1f, 1.0f);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	
}

//-------------------------------------------------------------------------

QuadricEntity::QuadricEntity() {
	// Indica a OpenGL que debe de contruir la entidad cuádrica.
	q = gluNewQuadric();
}

//-------------------------------------------------------------------------

// Se requiere el radio de la esfera.
Sphere::Sphere(GLdouble r) { this->r = r; } 

void Sphere::render(glm::dmat4 const& modelViewMat) const {
	
	// Envía las coordenadas de cámara.
	upload(modelViewMat * mModelMat);
	
	// Establecemos el color de la esfera.
	glEnable(GL_COLOR_MATERIAL); // Indica que el material de la figura viene definido por su color.
	glColor4f(0.0f, 0.25f, 0.42f, 1.0f); // Color de la figura.
	
	/*
	Especificamos como se dibuja la entidad cuádrica con: gluQuadricDrawStyle(QuadricEntity, tipoDeRecubrimiento);
	GLU_POINT: Muestra los puntos que definen el objeto cuádrico.
	GLU_LINE: Muestra las líneas que definen el objeto cuádrico.
	GLU_FILL: (Por defecto) Rellena el armazón del objeto cuádrico teniendo en cuenta la iluminación.
	gluQuadricDrawStyle(q, GLU_FILL);
	*/
	// gluSphere(q, radius, meridians, parallels);
	gluSphere(q, r, 50, 50);

	// Hemos de reestablecer la configuración por defecto.
	glDisable(GL_COLOR_MATERIAL);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//-------------------------------------------------------------------------

SphereWithFocus::SphereWithFocus(GLdouble r) : Sphere(r)
{
	this->spotLight = new SpotLight();
	this->spotLight->setAmb({ 0, 0, 0, 1 });
	this->spotLight->setDiff({ 1, 1, 1, 1 });
	this->spotLight->setSpec({ 0.5, 0.5, 0.5, 1 });
	this->spotLight->setPosDir({ 16 , 250 + 15, 0 });
	this->spotLight->setSpot({ 0.3, -1, 0 }, 10, 0);
}

void SphereWithFocus::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	this->spotLight->upload(aMat);
	Sphere::render(modelViewMat);
}

void SphereWithFocus::enableFocus()
{
	this->spotLight->enable();
}

void SphereWithFocus::disableFocus()
{
	this->spotLight->disable();
}

void SphereWithFocus::update()
{
	this->setModelMat(rotate(this->modelMat(), radians(this->angle), dvec3(0.0, 1.0, 0.0)));
}

//-------------------------------------------------------------------------

Cylinder::Cylinder(GLdouble baseRadius, GLdouble topRadius, GLdouble height, GLint slices = 50, GLint stacks = 50)
{ 
	this->stacks = stacks;
	this->slices = slices;
	this->height = height;
	this->topRadius = topRadius;
	this->baseRadius = baseRadius;
}

void Cylinder::render(glm::dmat4 const& modelViewMat) const
{
	// Envía las coordenadas de cámara.
	upload(modelViewMat * mModelMat);

	// Establecemos el color de la esfera.
	glEnable(GL_COLOR_MATERIAL); // Indica que el material de la figura viene definido por su color.
	glColor4f(0.0f, 0.25f, 0.42f, 1.0f); // Color de la figura.

	/*
	Especificamos como se dibuja la entidad cuádrica con: gluQuadricDrawStyle(QuadricEntity, tipoDeRecubrimiento);
	GLU_POINT: Muestra los puntos que definen el objeto cuádrico.
	GLU_LINE: Muestra las líneas que definen el objeto cuádrico.
	GLU_FILL: (Por defecto) Rellena el armazón del objeto cuádrico teniendo en cuenta la iluminación.
	gluQuadricDrawStyle(q, GLU_FILL);
	*/
	// gluCylinder(q, baseRadius, topRadius, height, parallelPoints, parallels);
	gluCylinder(q, this->baseRadius, this->topRadius, this->height, this->slices, this->stacks);

	// Hemos de reestablecer la configuración por defecto.
	glDisable(GL_COLOR_MATERIAL);
	glColor4f(1.0, 1.0, 1.0, 1.0);
}

//-------------------------------------------------------------------------

Disk::Disk(GLdouble innerRadius, GLdouble outerRadius, GLuint slices = 50, GLuint loops = 50)
{
	this->loops = loops;
	this->slices = slices;
	this->innerRadius = innerRadius;
	this->outerRadius = outerRadius;
}

void Disk::render(glm::dmat4 const& modelViewMat) const
{
	// Envía las coordenadas de cámara.
	upload(modelViewMat * mModelMat);

	// Establecemos el color de la esfera.
	glEnable(GL_COLOR_MATERIAL); // Indica que el material de la figura viene definido por su color.
	glColor4f(0.0f, 0.25f, 0.42f, 1.0f); // Color de la figura.

	/*
	Especificamos como se dibuja la entidad cuádrica con: gluQuadricDrawStyle(QuadricEntity, tipoDeRecubrimiento);
	GLU_POINT: Muestra los puntos que definen el objeto cuádrico.
	GLU_LINE: Muestra las líneas que definen el objeto cuádrico.
	GLU_FILL: (Por defecto) Rellena el armazón del objeto cuádrico teniendo en cuenta la iluminación.
	gluQuadricDrawStyle(q, GLU_FILL);
	*/
	// gluDisk(q, innerRadius, outerRadius, circlesPoints, rings);
	gluDisk(q, this->innerRadius, this->outerRadius, this->slices, this->loops);
	
	// Hemos de reestablecer la configuración por defecto.
	glDisable(GL_COLOR_MATERIAL);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//-------------------------------------------------------------------------

PartialDisk::PartialDisk(GLdouble innerRadius, GLdouble outerRadius, GLdouble startAngle, GLdouble diskAngle)
{
	this->diskAngle = diskAngle;
	this->startAngle = startAngle;
	this->innerRadius = innerRadius;
	this->outerRadius = outerRadius;
}

void PartialDisk::render(glm::dmat4 const& modelViewMat) const {
	
	// Envía las coordenadas de cámara.
	upload(modelViewMat * mModelMat);

	// Establecemos el color de la esfera.
	glEnable(GL_COLOR_MATERIAL); // Indica que el material de la figura viene definido por su color.
	glColor4f(0.0f, 0.25f, 0.42f, 1.0f); // Color de la figura.

	/*
	Especificamos como se dibuja la entidad cuádrica con: gluQuadricDrawStyle(QuadricEntity, tipoDeRecubrimiento);
	GLU_POINT: Muestra los puntos que definen el objeto cuádrico.
	GLU_LINE: Muestra las líneas que definen el objeto cuádrico.
	GLU_FILL: (Por defecto) Rellena el armazón del objeto cuádrico teniendo en cuenta la iluminación.
	gluQuadricDrawStyle(q, GLU_FILL);
	*/
	// gluPartialDisk(q, innerRadius, outerRadius, circlesPoints, rings, startAngle(90 + Parameter), diskAngle);
	gluPartialDisk(q, innerRadius, outerRadius, 50, 50, 0.0, 360.0);
	
	// Hemos de reestablecer la configuración por defecto.
	glDisable(GL_COLOR_MATERIAL);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0);
}

//-------------------------------------------------------------------------

AnilloCuadrado::AnilloCuadrado()
{
	mMesh = IndexMesh::generaAnilloCuadradoIndexado();
}

AnilloCuadrado::~AnilloCuadrado()
{
	delete mMesh;
	mMesh = nullptr;
}

void AnilloCuadrado::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		upload(modelViewMat * mModelMat); // Envía las coordenadas de cámara.
		glEnable(GL_COLOR_MATERIAL); // Indica que el material de la figura viene definido por su color.
		mMesh->render(); // Envía todos los arrays para el dibujado del objeto en la escena.
		glDisable(GL_COLOR_MATERIAL);
	}
}

//-------------------------------------------------------------------------

Cubo::Cubo(GLdouble l)
{
	mMesh = IndexMesh::generaCuboConTapasIndexado(l);
}

Cubo::~Cubo()
{
	delete mMesh;
	mMesh = nullptr;
}

void Cubo::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		upload(modelViewMat * mModelMat); // Envía las coordenadas de cámara.
		glEnable(GL_COLOR_MATERIAL); // Indica que el material de la figura viene definido por su color.
		glColor4dv(value_ptr(mColor));
		mMesh->render(); // Envía todos los arrays para el dibujado del objeto en la escena.
		glColor4f(1.0f, 1.0f, 1.0f, 1.0);
		glDisable(GL_COLOR_MATERIAL);
	}
}

//-------------------------------------------------------------------------

CompoundEntity::~CompoundEntity()
{
	
	// Vaciamos el array de objetos de esta entidad compuesta (liberamos memoria).   
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

void CompoundEntity::update()
{
	for (auto& g : gObjects)
		g->update();
}

void CompoundEntity::render(glm::dmat4 const& modelViewMat) const
{

	// Proceso para renderizar una entidad compuesta:
		
	// 1º) Multiplicar la matriz matriz de vista (modelViewMat) por la matriz de modelado de la CompoundEntity (mModelMat).
	dmat4 aMat = modelViewMat * this->mModelMat;
	
	// 2º) Cargar la matriz resultante del producto anterior con upload(aMat).
	upload(aMat);

	// 3º) Renderizar las sub - entidades en gObjects con respecto a aMat.
	for (Abs_Entity* el : gObjects)
		el->render(aMat);

}

void CompoundEntity::addEntity(Abs_Entity* entity)
{
	// Añade una entidad a la entidad compuesta.
	gObjects.push_back(entity);
}

//-------------------------------------------------------------------------

Rotor::Rotor(GLdouble r, GLdouble h)
{
	Cylinder* tub = new Cylinder(r, r, h);
	RectanguloRGB* pal = new RectanguloRGB(2 * r, h);
	pal->setModelMat(translate(dmat4(1), dvec3(0.0, 0.0, h / 2)));
	pal->setModelMat(rotate(pal->modelMat(), radians(90.0), dvec3(1, 0, 0)));
	this->addEntity(tub);
	this->addEntity(pal);
}

void Rotor::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	CompoundEntity::render(modelViewMat);
}

//-------------------------------------------------------------------------

CapAndHat::CapAndHat()
{

	Sphere* cap = new Sphere(100.0);
	this->addEntity(cap);

	CompoundEntity* hat = new CompoundEntity();
	hat->setModelMat(translate(hat->modelMat(), dvec3(0.0, 100.0, 0.0)));
	this->addEntity(hat);

	Cubo* base = new Cubo(100);
	base->setModelMat(scale(base->modelMat(), dvec3(1.0, 0.05, 1.0)));
	hat->addEntity(base);

	Cylinder* capirote = new Cylinder(50.0, 0.0, 100);
	capirote->setModelMat(rotate(capirote->modelMat(), radians(-90.0), dvec3(1, 0, 0)));
	hat->addEntity(capirote);

}

void CapAndHat::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	CompoundEntity::render(modelViewMat);
}

//-------------------------------------------------------------------------

Cone::Cone(GLdouble h, GLdouble r, GLuint n) {
	// h=altura del cono
	// r=radio de la base
	// n=número de muestras
	// m=número de puntos del perfil
	int m = 3;
	dvec3* perfil = new dvec3[m];
	perfil[0] = dvec3(0.5, 0.0, 0.0);
	perfil[1] = dvec3(r, 0.0, 0.0);
	perfil[2] = dvec3(0.5, h, 0.0);
	this->mMesh = MbR::generaMallaIndexadaPorRevolucion(m, n, perfil);
}

Cone::~Cone()
{
	delete this->mMesh;
	this->mMesh = nullptr;
}

void Cone::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		glEnable(GL_COLOR_MATERIAL);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor4d(0.0, 1.0, 0.0, 0.0);
		upload(modelViewMat * mModelMat); // Envía las coordenadas de cámara.
		mMesh->render(); // Envía todos los arrays para el dibujado del objeto en la escena.
		glDisable(GL_COLOR_MATERIAL);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

//-------------------------------------------------------------------------

Grid::Grid(GLdouble lado, GLuint nDiv)
{
	// this->mMesh = IndexMesh::generateGrid(lado, nDiv);
	this->mMesh = IndexMesh::generateGridTex(lado, nDiv);
}

Grid::~Grid()
{
	delete this->mMesh;
	this->mMesh = nullptr;
}

void Grid::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		
		upload(modelViewMat * mModelMat);
		// glPolygonMode(GL_BACK, GL_LINE);
		glColor4dv(value_ptr(mColor));
		glEnable(GL_COLOR_MATERIAL);
		
		/*
		bind: Activa la textura actual (ahora podemos trabajar con ella) y establece el modo de mezcla de colores.
		Color de la figura (entidad) C; Coordenadas de textura (s, t); T(s,t) = Color (texel) de las coordenadas (s,t)
		GL_REPLACE: Utilizar exclusivamente el color de la textura --> Cr = T(s,t)
		GL_MODULATE: Modular ambos colores --> Cr = C * T(s,t)
		GL_ADD: Sumar ambos colores --> Cr = C + T(s,t)
		*/
		if (mTexture != nullptr)
			mTexture->bind(GL_MODULATE);

		mMesh->render();
		
		if (mTexture != nullptr)
			mTexture->unbind();

		glDisable(GL_COLOR_MATERIAL);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	}
}

//-------------------------------------------------------------------------

// Creamos el GridCube como una entidad compuesta.
GridCube::GridCube(GLdouble lado, GLuint nDiv)
{

	// Color de las caras (por defecto es blanco si no se indica nada).
	// mColor = { 1, 1, 0, 1 };

	// Textura superior e inferior.
	Texture* t0 = new Texture();
	t0->load("..\\Bmps\\checker.bmp");
	gTextures.push_back(t0);

	// Texturas laterales.
	Texture* t1 = new Texture();
	t1->load("..\\Bmps\\stones.bmp");
	gTextures.push_back(t1);

	// Cara superior.
	Grid* superior = new Grid(lado, nDiv);
	superior->setModelMat(translate(dmat4(1), dvec3(0.0, lado * 0.5, 0.0)));
	superior->setModelMat(rotate(superior->modelMat(), radians(90.0), dvec3(0.0, 1.0, 0.0)));
	superior->setColor(mColor);
	superior->setTexture(gTextures[0]);
	this->addEntity(superior);

	// Cara inferior.
	Grid* inferior = new Grid(lado, nDiv);
	inferior->setModelMat(translate(dmat4(1), dvec3(0.0, -lado * 0.5, 0.0)));
	inferior->setModelMat(rotate(inferior->modelMat(), radians(180.0), dvec3(1.0, 0.0, 0.0)));
	inferior->setColor(mColor);
	inferior->setTexture(gTextures[0]);
	this->addEntity(inferior);
	
	// Cara izquierda.
	Grid* izquierda = new Grid(lado, nDiv);
	izquierda->setModelMat(translate(izquierda->modelMat(), dvec3(-lado * 0.5, 0.0, 0.0)));
	izquierda->setModelMat(rotate(izquierda->modelMat(), radians(90.0), dvec3(0.0, 0.0, 1.0)));
	izquierda->setColor(mColor);
	izquierda->setTexture(gTextures[1]);
	this->addEntity(izquierda);

	// Cara derecha.
	Grid* derecha = new Grid(lado, nDiv);
	derecha->setModelMat(translate(dmat4(1), dvec3(lado * 0.5, 0.0, 0.0)));
	derecha->setModelMat(rotate(derecha->modelMat(), radians(-90.0), dvec3(0.0, 0.0, 1.0)));
	derecha->setColor(mColor);
	derecha->setTexture(gTextures[1]);
	this->addEntity(derecha);

	// Cara frontal.
	Grid* frontal = new Grid(lado, nDiv);
	frontal->setModelMat(translate(dmat4(1), dvec3(0.0, 0.0, lado * 0.5)));
	frontal->setModelMat(rotate(frontal->modelMat(), radians(90.0), dvec3(0.0, 0.0, 1.0)));
	frontal->setModelMat(rotate(frontal->modelMat(), radians(90.0), dvec3(1.0, 0.0, 0.0)));
	frontal->setColor(mColor);
	frontal->setTexture(gTextures[1]);
	this->addEntity(frontal);

	// Cara trasera.
	Grid* trasera = new Grid(lado, nDiv);
	trasera->setModelMat(translate(dmat4(1), dvec3(0.0, 0.0, -lado * 0.5)));
	trasera->setModelMat(rotate(trasera->modelMat(), radians(90.0), dvec3(0.0, 0.0, 1.0)));
	trasera->setModelMat(rotate(trasera->modelMat(), radians(-90.0), dvec3(1.0, 0.0, 0.0)));
	trasera->setColor(mColor);
	trasera->setTexture(gTextures[1]);
	this->addEntity(trasera);

}

// Renderizamos todas las paredes del GridCube.
void GridCube::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	CompoundEntity::render(modelViewMat);
}

//-------------------------------------------------------------------------

// Creamos el TIE como una entidad compuesta de otras sub-entidades.
TieFighter::TieFighter()
{

	// Textura de las alas.
	Texture* t0 = new Texture();
	t0->load("..\\Bmps\\noche.bmp", 122);
	gTextures.push_back(t0);

	// A continuación las partes del TIE Fighter.
	Sphere* core = new Sphere(100.0);
	this->addEntity(core);

	CompoundEntity* front = new CompoundEntity();
	this->addEntity(front);

		Cylinder* cylinder = new Cylinder(70.0, 70.0, 40.0);
		cylinder->setModelMat(translate(cylinder->modelMat(), dvec3(100, 0, 0)));
		cylinder->setModelMat(rotate(cylinder->modelMat(), radians(-90.0), dvec3(0, 1, 0)));
		front->addEntity(cylinder);

		Disk* disk = new Disk(0.0, 70.0);
		disk->setModelMat(translate(disk->modelMat(), dvec3(100, 0, 0)));
		disk->setModelMat(rotate(disk->modelMat(), radians(90.0), dvec3(0, 1, 0)));
		front->addEntity(disk);

	Cylinder* shaft = new Cylinder(25.0, 25.0, 400.0);
	shaft->setModelMat(translate(shaft->modelMat(), dvec3(0, 0, 200)));
	shaft->setModelMat(rotate(shaft->modelMat(), radians(180.0), dvec3(0, 1, 0)));
	this->addEntity(shaft);

	Hexagono* wingL = new Hexagono(300);
	wingL->setTexture(gTextures[0]);
	wingL->setModelMat(translate(wingL->modelMat(), dvec3(0, 0, 200)));
	wingL->setModelMat(rotate(wingL->modelMat(), radians(30.0), dvec3(0, 0, 1)));
	this->addEntity(wingL);

	Hexagono* wingR = new Hexagono(300);
	wingR->setTexture(gTextures[0]);
	wingR->setModelMat(translate(wingR->modelMat(), dvec3(0, 0, -200)));
	wingR->setModelMat(rotate(wingR->modelMat(), radians(30.0), dvec3(0, 0, 1)));
	this->addEntity(wingR);

	// Luz (foco) del TIE.
	this->spotLight = new SpotLight();
	this->spotLight->setAmb({ 0, 0, 0, 1 });
	this->spotLight->setDiff({ 1, 1, 1, 1 });
	this->spotLight->setSpec({ 0.5, 0.5, 0.5, 1 });
	this->spotLight->setSpot({ 0, -1, 0 }, 35, 0);
	
}

// Destruye la luz propia del TIE.
TieFighter::~TieFighter()
{
	delete this->spotLight;
	this->spotLight = nullptr;
}

// Enciende la luz del TIE.
void TieFighter::setLightOn()
{
	this->spotLight->enable();
}

// Apaga la luz del TIE.
void TieFighter::setLightOff()
{
	this->spotLight->disable();
}

/*
Actualizamos el foco para lograr el seguimiento y luego 
renderizamos todas las entidades que componen el TIE.
*/
void TieFighter::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);	
	this->spotLight->upload(aMat);
	CompoundEntity::render(modelViewMat);
}

//-------------------------------------------------------------------------

// Creamos el escuadrón haciendo que los TIE formen un arco.
TieFighterSquadron::TieFighterSquadron(GLdouble radio)
{

	// Por defecto el escuadrón orbita al actualizarse la escena.
	this->orbitOrRotate = true;

	// TIE Fighter (Central)
	TieFighter* central = new TieFighter();
	central->setModelMat(translate(dmat4(1), dvec3(0.0, 0 + radio * sin(radians(90.0)), 0 - radio * cos(radians(90.0)))));
	central->setModelMat(scale(central->modelMat(), dvec3(0.15, 0.15, 0.15)));
	this->addEntity(central);

	// TIE Fighter (Derecho)
	TieFighter* derecho = new TieFighter();
	derecho->setModelMat(translate(dmat4(1), dvec3(0.0, 0 + radio * sin(radians(70.0)), 0 - radio * cos(radians(70.0)))));
	derecho->setModelMat(rotate(derecho->modelMat(), radians(-15.0), dvec3(1.0, 0.0, 0.0)));
	derecho->setModelMat(scale(derecho->modelMat(), dvec3(0.15, 0.15, 0.15)));
	this->addEntity(derecho);

	// TIE Fighter (Izquierdo)
	TieFighter* izquierdo = new TieFighter();
	izquierdo->setModelMat(translate(dmat4(1), dvec3(0.0, 0 + radio * sin(radians(110.0)), 0 - radio * cos(radians(110.0)))));
	izquierdo->setModelMat(rotate(izquierdo->modelMat(), radians(15.0), dvec3(1.0, 0.0, 0.0)));
	izquierdo->setModelMat(scale(izquierdo->modelMat(), dvec3(0.15, 0.15, 0.15)));
	this->addEntity(izquierdo);

}

// Renderiza todas las naves del escuadrón.
void TieFighterSquadron::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	CompoundEntity::render(modelViewMat);
}

// Enciende las luces de todas las naves del escuadrón.
void TieFighterSquadron::setLightsOn()
{
	for (auto entity : gObjects) {
		TieFighter* tie = (TieFighter*) entity;
		tie->setLightOn();
	}
}

// Apaga las luces de todas las naves del escuadrón.
void TieFighterSquadron::setLightsOff()
{
	for (auto entity : gObjects) {
		TieFighter* tie = (TieFighter*) entity;
		tie->setLightOff();
	}
}

// Actualizamos la posición del escuadrón en función del booleano.
void TieFighterSquadron::update()
{
	if (orbitOrRotate) {
		// Hacemos orbitar el escuadrón de cazas.
		this->setModelMat(glm::rotate(this->modelMat(), radians(4.0), dvec3(0.0, 0.0, -1.0)));
	} else {
		// Hacemos rotar el escuadrón de cazas.
		this->setModelMat(glm::rotate(this->modelMat(), radians(4.0), dvec3(0.0, 1.0, 0.0)));
	}
}

// true -> orbit ; false -> rotate
void TieFighterSquadron::setOrbitOrRotate(bool orbit) {
	this->orbitOrRotate = orbit;
}

//-------------------------------------------------------------------------

Esfera::Esfera(GLdouble r, GLint p, GLuint m)
{

	/*
	r (radio): radio de la esfera.
	m (meridian): número de muestras que se toman.
	p (parallel): puntos de perfil.
	*/

	// El total de puntos del perfil es p (paralelos).
	dvec3* perfil = new dvec3[p];

	// Definimos el arco de 180º y lo dividimos en trozos iguales.
	GLdouble ang = 270; // Ángulo inicial.
	GLdouble increment = 180.0 / (p - 1.0); // Dividimos el arco en sub-arcos equitativos.

	// Recorremos del segundo vértice al penúltimo, ambos incluidos.
	for (GLint i = 0; i < p; i++, ang += increment)

		// Incorpora el punto del perfil.
		perfil[i] = dvec3(0.05 + r * cos(radians(ang)), 0.0 + r * sin(radians(ang)), 0.0);

	// Obtenemos la malla.
	this->mMesh = MbR::generaMallaIndexadaPorRevolucion(m, p, perfil);

}

Esfera::~Esfera()
{
	delete this->mMesh;
	this->mMesh = nullptr;
}

void Esfera::render(glm::dmat4 const& modelViewMat) const
{

	if (mMesh != nullptr) {
		
		// Envía las coordenadas de cámara.
		upload(modelViewMat * mModelMat); 

		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		if (material != nullptr) {
			material->upload();
		} else {
			glEnable(GL_COLOR_MATERIAL);
			glColor4dv(value_ptr(mColor));
		}

		// Envía todos los arrays para el dibujado del objeto en la escena.
		mMesh->render(); 

		glDisable(GL_COLOR_MATERIAL);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}

}

//-------------------------------------------------------------------------

Toroide::Toroide(GLdouble r, GLint p, GLuint m)
{

	/*
	r (radio): radio de la esfera.
	m (meridian): número de muestras que se toman.
	p (parallel): puntos de perfil.
	*/

	// Total de puntos del perfil.
	dvec3* perfil = new dvec3[p];

	// Definimos el arco de 380º y lo dividimos en trozos iguales.
	GLdouble ang = 90; // Ángulo inicial.
	GLdouble increment = 360.0 / (p - 1.0); // Dividimos el circulo en sub-arcos equitativos.

	// Recorremos del segundo vértice al penúltimo, ambos incluidos.
	for (GLint i = 0; i < p; i++, ang += increment)

		// Incorpora el punto del perfil.
		perfil[i] = dvec3((r + (r / 2)) + r * cos(radians(ang)), 0.0 + r * sin(radians(ang)), 0.0);

	// Obtenemos la malla.
	this->mMesh = MbR::generaMallaIndexadaPorRevolucion(m, p, perfil);

}

Toroide::~Toroide()
{
	delete this->mMesh;
	this->mMesh = nullptr;
}

void Toroide::render(glm::dmat4 const& modelViewMat) const
{

	if (mMesh != nullptr) {

		// Envía las coordenadas de cámara.
		upload(modelViewMat * mModelMat);

		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		if (material != nullptr) {
			material->upload();
		}
		else {
			glEnable(GL_COLOR_MATERIAL);
			glColor4dv(value_ptr(mColor));
		}

		// Envía todos los arrays para el dibujado del objeto en la escena.
		mMesh->render();

		glDisable(GL_COLOR_MATERIAL);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}

}

//-------------------------------------------------------------------------

Tornillo::Tornillo()
{
	
	/*
	Recordar:
		* Escalar, rotar, trasladar.
		* En las rotaciones, (90) es CW y (-90) es CWW.
		* Las transformaciones se ejecutan en orden inverso, es decir, la ultima en escribirse se ejecuta primero.
	*/

	Cylinder* c1 = new Cylinder(50, 50, 40, 6, 50);
	c1->setModelMat(rotate(c1->modelMat(), radians(-90.0), dvec3(1.0, 0.0, 0.0)));
	this->addEntity(c1);

	Disk* d1 = new Disk(0, 50, 6, 1);
	d1->setModelMat(rotate(d1->modelMat(), radians(-90.0), dvec3(1.0, 0.0, 0.0)));
	this->addEntity(d1);

	Disk* d2 = new Disk(0, 50, 6, 1);
	d2->setModelMat(translate(dmat4(1), dvec3(0.0, 40.0, 0.0)));
	d2->setModelMat(rotate(d2->modelMat(), radians(-90.0), dvec3(1.0, 0.0, 0.0)));
	this->addEntity(d2);

	Cylinder* c2 = new Cylinder(25, 25, 100, 6, 50);
	c2->setModelMat(translate(dmat4(1), dvec3(0.0, 40.0, 0.0)));
	c2->setModelMat(rotate(c2->modelMat(), radians(-90.0), dvec3(1.0, 0.0, 0.0)));
	this->addEntity(c2);

}

void Tornillo::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	CompoundEntity::render(modelViewMat);
}

//-------------------------------------------------------------------------

TornilloConLuz::TornilloConLuz()
{

	this->setModelMat(translate(dmat4(1), dvec3(0.0, 340.0, 0.0)));
	this->setModelMat(rotate(this->modelMat(), radians(180.0), dvec3(1.0, 0.0, 0.0)));
	
	this->spotLight = new SpotLight();
	this->spotLight->setAmb({ 0, 0, 0, 1 });
	this->spotLight->setDiff({ 1, 1, 1, 1 });
	this->spotLight->setSpec({ 0.5, 0.5, 0.5, 1 });
	this->spotLight->setPosDir({ 0 , 120, 0 });
	this->spotLight->setSpot({ 0, 1, 0 }, 45, 0);

}

void TornilloConLuz::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	this->spotLight->upload(aMat);
	Tornillo::render(modelViewMat);
}

void TornilloConLuz::update()
{
	this->setModelMat(translate(this->modelMat(), dvec3(0.0, 1.0, 0.0)));
}

void TornilloConLuz::enableSpot()
{
	this->spotLight->enable();
}

void TornilloConLuz::disableSpot()
{
	this->spotLight->disable();
}

//-------------------------------------------------------------------------

Faro::Faro(GLdouble radio)
{

	// Atributos de clase
	this->radio = radio;
	this->angulo = 90.0;

	// Cubo
	Cubo* central = new Cubo(50);
	central->setColor({ 0.0f, 0.25f, 0.42f, 1.0f });
	central->setModelMat(translate(dmat4(1), dvec3(0.0, 25, 0.0)));
	this->addEntity(central);

	// Esfera
	SphereWithFocus* swf = new SphereWithFocus(15);
	swf->setModelMat(translate(dmat4(1), dvec3(0.0, 50 + 15, 0.0)));
	this->addEntity(swf);

	// Piramide
	Cylinder* piramide = new Cylinder(25.0, 0.0, 20.0, 4, 50);
	piramide->setModelMat(translate(dmat4(1), dvec3(0.0, 80, 0.0)));
	piramide->setModelMat(rotate(piramide->modelMat(), radians(-45.0), dvec3(0.0, 1.0, 0.0)));
	piramide->setModelMat(rotate(piramide->modelMat(), radians(-90.0), dvec3(1.0, 0.0, 0.0)));
	this->addEntity(piramide);
	
	// Desplazamos toda la entidad.
	this->setModelMat(translate(dmat4(1), dvec3(0.0, radio, 0.0)));

}

void Faro::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	CompoundEntity::render(modelViewMat);
}

void Faro::update()
{

	// Angulo de giro
	this->angulo += 5;

	// Posicion
	GLdouble x = 0.0 + radio * cos(radians(angulo));
	GLdouble y = 0.0 + radio * sin(radians(angulo));
	
	// Transformaciones de la esfera con como
	CompoundEntity::update();

	// Transformaciones del Faro
	this->setModelMat(translate(dmat4(1), dvec3(x, y, 0)));
	this->setModelMat(rotate(this->modelMat(), radians(angulo - 90.0), dvec3(0.0, 0.0, 1.0)));

}

// Enciende la luz del faro.
void Faro::setLightOn()
{
	SphereWithFocus*  swf = (SphereWithFocus*) gObjects[1];
	swf->enableFocus();
}

// Apaga la luz del faro.
void Faro::setLightOff()
{
	SphereWithFocus* swf = (SphereWithFocus*)gObjects[1];
	swf->enableFocus();
}

//-------------------------------------------------------------------------

Piramide::Piramide(GLint caras, GLdouble radio)
{

	// Como minimo, dibujamos un triangulo.
	if (caras < 2)
		caras = 2;

	// Obtenemos la malla.
	this->mMesh = IndexMesh::generaPiramide(caras, radio);

}

void Piramide::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	glEnable(GL_COLOR_MATERIAL);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	mMesh->render();
	glDisable(GL_COLOR_MATERIAL);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
