#include "Texture.h"

//-------------------------------------------------------------------------

Texture::~Texture() 
{
	if(mId != 0) // Si la textura posee un identificador propio (está inicializada) ...
		glDeleteTextures(1, &mId); // Se elimina una única textura con un identificador concreto (la textura actual).
}
//-------------------------------------------------------------------------

void Texture::init()
{
	// Genera un identificador para la textura actual.
	glGenTextures(1, &mId);
	
	// Activa la textura actual, ahora podemos trabajar con ella.
	glBindTexture(GL_TEXTURE_2D, mId);
 
	/*
	Filtering (Filtrado)
	
	¿Que hacer con la textura cuando no coinciden texeles y píxeles?

	GL_TEXTURE_MAG_FILTER (Ampliación): Cuando un texel ocupa menos que un pixel, hay que agrandar el texel.

	GL_TEXTURE_MIN_FILTER (Minimización): Cuando un texel ocupa más que un pixel, hay que reducir el texel.
	
	Opciones ante la ampliación o la minimicación:

	GL_NEAREST: 

	GL_LINEAR:   	
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST

	/*
	Wrapping (Envoltura)
	
	¿Quese hace cuando las coordenadas de textura salen del rango [0 - 1]?
	
	GL_TEXTURE_WRAP_S (Horizontal): Para indicar que ocurrirá cuando las coordenadas se salen de rango en la horizontal.

	GL_TEXTURE_WRAP_T (Vertical): Para indicar que ocurrirá cuando las coordenadas se salen de rango en la vertical.

	Opciones ante la salida del rango:

	GL_REPEAT (Tiling): Cada unidad que excede [0 - 1] es una repetición de la textura.

	GL_CLAMP (Clamping): Lleva los valores mayores que 1 a 1 y los menores que 0 a 0.

	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // GL_CLAMP
}
//-------------------------------------------------------------------------

void Texture::loadColorBuffer(GLuint width, GLuint height, GLuint buffer) {
	
	// Si la textura actual no tiene un identificador (no se ha inicializado), entonces la inicializamos dándole uno.
	if (mId == 0) init();

	// Establecemos el ancho y el alto de la textura actual.
	mWidth = width;
	mHeight = height;

	// Activa la textura actual, ahora podemos trabajar con ella.
	glBindTexture(GL_TEXTURE_2D, mId);

	// Los datos se obtendrán del buffer de color activo (por defecto BACK).
	glReadBuffer(buffer); // Obtenemos el buffer por parámetro (GL_FRONT o GL_BACK).

	// Copia en la textura activa (mId) una imagen/frame almacenado en el buffer de color indicado anteriormente.
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, width, height, 0);

	// Los datos se obtienen del buffer de color activo (por defecto BACK).
	glReadBuffer(GL_BACK); // Volvemos a dejar la configuración por defecto.

	// Desactiva la textura actual.
	glBindTexture(GL_TEXTURE_2D, 0);

}

//-------------------------------------------------------------------------

void Texture::save(const std::string& BMP_Name) {

	// Clase de los profesores para trabajar con imagenes "BitMap".
	PixMap32RGBA pixMap;

	// Reservamos memoria para la imagen.
	pixMap.reserve(mWidth, mHeight);

	// Activa la textura actual, ahora podemos trabajar con ella.
	glBindTexture(GL_TEXTURE_2D, mId);

	// Obtener (de GPU a CPU) la imagen de la textura que esté activa.
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixMap.data());
	
	// Desactiva la textura actual.
	glBindTexture(GL_TEXTURE_2D, 0);

	// Guarda la imagen en el disco.
	pixMap.save_bmp24BGR(BMP_Name);

}

//-------------------------------------------------------------------------

void Texture::load(const std::string& BMP_Name, glm::u8vec3 color, GLubyte alpha) {

	// 0 --> La textura actual no tiene un identificador (no se ha inicializado).
	if (mId == 0) init();

	// Clase de los profesores para trabajar con imagenes "BitMap".
	PixMap32RGBA pixMap;

	// Carga una imagen en el disco.
	pixMap.load_bmp24BGR(BMP_Name);

	// Establece el canal alpha (intensidad) de los píxeles con un color específico.
	if (alpha != 255)
		pixMap.set_colorkey_alpha(color, alpha);

	// Establece el ancho y el alto de la textura actual.
	mWidth = pixMap.width();
	mHeight = pixMap.height();

	// Activa la textura actual, ahora podemos trabajar con ella.
	glBindTexture(GL_TEXTURE_2D, mId);

	// Transfiere la imagen activa de CPU a GPU.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixMap.data());
	
	// Desactiva la textura actual.
	glBindTexture(GL_TEXTURE_2D, 0);

}

//-------------------------------------------------------------------------

void Texture::bind(GLuint mixMode)
{
	// Activa la textura actual, ahora podemos trabajar con ella.
	glBindTexture(GL_TEXTURE_2D, mId);

	/*
	Establece el modo de mezcla de colores.
	
	Color de la figura (entidad) C; Coordenadas de textura (s, t); T(s,t) = Color (texel) de las coordenadas (s,t)
	GL_REPLACE: Utilizar exclusivamente el color de la textura --> Cr = T(s,t)
	GL_MODULATE: Modular ambos colores --> Cr = C * T(s,t)
	GL_ADD: Sumar ambos colores --> Cr = C + T(s,t)
	*/
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mixMode);  
}
//-------------------------------------------------------------------------

void Texture::load(const std::string & BMP_Name, GLubyte alpha)
{
	if (mId == 0) init();
	
	PixMap32RGBA pixMap;
  
    pixMap.load_bmp24BGR(BMP_Name);

    if (alpha != 255)
       pixMap.set_alpha(alpha);

	mWidth = pixMap.width();
	mHeight = pixMap.height();

    GLint level = 0;   //Base image level
	GLint border = 0;  //No border
	
	glBindTexture(GL_TEXTURE_2D, mId);
    glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, mWidth, mHeight, border, GL_RGBA, GL_UNSIGNED_BYTE, pixMap.data());
    glBindTexture(GL_TEXTURE_2D, 0); 
}
//-------------------------------------------------------------------------

void Texture::setWrap(GLuint wp) // GL_REPEAT, GL_CLAMP
{
	glBindTexture(GL_TEXTURE_2D, mId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wp);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wp);  
	glBindTexture(GL_TEXTURE_2D, 0); 
}

//-------------------------------------------------------------------------