#include "Mesh.h"
#include "CheckML.h"
#include <fstream>
using namespace std;
using namespace glm;

//-------------------------------------------------------------------------
void Mesh::draw() const 
{
    // Primitiva, stride y n�mero de elementos a renderizar.
    glDrawArrays(mPrimitive, 0, size());
}
//-------------------------------------------------------------------------
void IndexMesh::draw() const {
    // Primitiva, n�mero de �ndices, tipo de dato y vector de elementos.
    glDrawElements(mPrimitive, nNumIndices, GL_UNSIGNED_INT, vIndices);
}
//-------------------------------------------------------------------------
void IndexMesh::buildNormalVectors()
{
    
    // 1�) Construir el vector de normales con el mismo tama�o que el de v�rtices.
    this->vNormals.reserve(this->mNumVertices);

    // 2�) Inicializar las componentes del vector de normales al vector 0 = { (0,0,0), ... , (0,0,0) }
    for (size_t i = 0; i < this->mNumVertices; ++i)
        this->vNormals.push_back({ 0.0, 0.0, 0.0 });

    // 3�) Recorrer los tri�ngulos:
    for (size_t i = 0; i < this->nNumIndices; i += 3)
    {

        // 3.1�) Extraer los �ndices de los tri�ngulos (a, b, c).
        GLuint a = this->vIndices[i];
        GLuint b = this->vIndices[i + 1];
        GLuint c = this->vIndices[i + 2];

        // 3.2�) Calcular el vector "n" normal al tri�ngulo tal como se ha explicado.
        glm::dvec3 v0 = this->vVertices[a];
        glm::dvec3 v1 = this->vVertices[b];
        glm::dvec3 v2 = this->vVertices[c];
        glm::dvec3 n = normalize(cross(v2 - v1, v0 - v1));

        // 3.3�) Sumar n al vector normal de cada v�rtice del tri�ngulo.
        this->vNormals[a] += n;
        this->vNormals[b] += n;
        this->vNormals[c] += n;

    }

    // 4�) Normalizar los vectores normales.
    for (size_t i = 0; i < this->mNumVertices; ++i)
        this->vNormals[i] = normalize(this->vNormals[i]);
    
}

//-------------------------------------------------------------------------
void Mesh::render() const 
{
  
    // Transferir datos.
    if (vVertices.size() > 0) {
    
        // Indica que se va a transferir a la GPU el array de v�rtices.
        glEnableClientState(GL_VERTEX_ARRAY);
        // N�mero de componentes (XYZ=3), tipo de datos de cada componente, desplazamiento y datos.
        glVertexPointer(3, GL_DOUBLE, 0, vVertices.data()); 
    
        // Transferir colores.
        if (vColors.size() > 0) {
          // Indica que se va a transferir a la GPU el array de color.
          glEnableClientState(GL_COLOR_ARRAY);
          // N�mero de componentes (RGBA=4), tipo de datos de cada componente, desplazamiento y datos.
          glColorPointer(4, GL_DOUBLE, 0, vColors.data());    
        }

        // Transferir texturas.
        if (vTexCoords.size() > 0) {
            // Indica que se va a transferir a la GPU el array de texturas.
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            // N�mero de componentes, tipo de datos de cada componente, desplazamiento y datos.
            glTexCoordPointer(2, GL_DOUBLE, 0, vTexCoords.data());
        }

        // Transferir normales.
        if (vNormals.size() > 0) {
            // Indica que se va a transferir a la GPU el array de normales.
            glEnableClientState(GL_NORMAL_ARRAY);
            // Tipo de dato, desplazamiento y datos.
            glNormalPointer(GL_DOUBLE, 0, vNormals.data());    
        }

        // Dibuja la malla con la informaci�n de los 4 arrays.
	    draw();

        // Reestablece la configuraci�n por defecto.
        glDisableClientState(GL_COLOR_ARRAY);
	    glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    }

}
//-------------------------------------------------------------------------
void IndexMesh::render() const {

    // Transferir datos.
    if (vVertices.size() > 0) {

        // Indica que se va a transferir a la GPU el array de v�rtices.
        glEnableClientState(GL_VERTEX_ARRAY);
        // N�mero de componentes (XYZ=3), tipo de datos de cada componente, desplazamiento y datos.
        glVertexPointer(3, GL_DOUBLE, 0, vVertices.data());

        // Transferir colores.
        if (vColors.size() > 0) {
            // Indica que se va a transferir a la GPU el array de color.
            glEnableClientState(GL_COLOR_ARRAY);
            // N�mero de componentes (RGBA=4), tipo de datos de cada componente, desplazamiento y datos.
            glColorPointer(4, GL_DOUBLE, 0, vColors.data());
        }

        // Transferir texturas.
        if (vTexCoords.size() > 0) {
            // Indica que se va a transferir a la GPU el array de texturas.
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            // N�mero de componentes (...), tipo de datos de cada componente, desplazamiento y datos.
            glTexCoordPointer(2, GL_DOUBLE, 0, vTexCoords.data());
        }

        // Transferir normales.
        if (vNormals.size() > 0) {
            // Indica que se va a transferir a la GPU el array de normales.
            glEnableClientState(GL_NORMAL_ARRAY);
            // Tipo de dato, desplazamiento y datos.
            glNormalPointer(GL_DOUBLE, 0, vNormals.data());
        }

        // Transferir �ndices.
        if (vIndices != nullptr) {
            glEnableClientState(GL_INDEX_ARRAY);
            glIndexPointer(GL_UNSIGNED_INT, 0, vIndices);
        }

        // Dibuja la malla con la informaci�n de los 3 arrays.
        draw();

        // Comandos OpenGLpara deshabilitar los datos enviados.
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_INDEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    }

}
//-------------------------------------------------------------------------
Mesh * Mesh::createRGBAxes(GLdouble l)
{
  Mesh* mesh = new Mesh();

  mesh->mPrimitive = GL_LINES;

  mesh->mNumVertices = 6;
  mesh->vVertices.reserve(mesh->mNumVertices);

  // X axis vertices
  mesh->vVertices.push_back({ 0.0, 0.0, 0.0 });
  mesh->vVertices.push_back({ l, 0.0, 0.0 });
  // Y axis vertices
  mesh->vVertices.push_back({ 0, 0.0, 0.0 });
  mesh->vVertices.push_back({ 0.0, l, 0.0 });
  // Z axis vertices
  mesh->vVertices.push_back({ 0.0, 0.0, 0.0 });
  mesh->vVertices.push_back({ 0.0, 0.0, l });

  mesh->vColors.reserve(mesh->mNumVertices);
  // X axis color: red  (Alpha = 1 : fully opaque)
  mesh->vColors.push_back({ 1.0, 0.0, 0.0, 1.0 });
  mesh->vColors.push_back({ 1.0, 0.0, 0.0, 1.0 });
  // Y axis color: green
  mesh->vColors.push_back({ 0.0, 1.0, 0.0, 1.0 });
  mesh->vColors.push_back({ 0.0, 1.0, 0.0, 1.0 });
  // Z axis color: blue
  mesh->vColors.push_back({ 0.0, 0.0, 1.0, 1.0 });
  mesh->vColors.push_back({ 0.0, 0.0, 1.0, 1.0 });

  return mesh;
}
//-------------------------------------------------------------------------
Mesh* Mesh::generaPoligono(GLuint numL, GLdouble rd)
{

    // Genera una malla.
    Mesh* mesh = new Mesh();

    // Utiliza la primitiva GL_LINE_LOOP
    mesh->mPrimitive = GL_LINE_LOOP;

    // Establecemos el n�meros de v�rtices/lados.
    mesh->mNumVertices = numL;
    mesh->vVertices.reserve(mesh->mNumVertices);

    GLdouble x = 0;
    GLdouble y = 0;
    GLdouble ang = 90; // Empezamos en el �ngulo de 90�.
    GLdouble incremento = 360.0 / numL;

    for (size_t i = 0; i < mesh->mNumVertices; i++) {
        
        // Ecuaci�n de la circunferencia con centro C (0,0) y radio R
        x = 0 + rd * cos(radians(ang));
        y = 0 + rd * sin(radians(ang));
        ang += incremento;

        // Vertices que componen la circunferencia.
        mesh->vVertices.push_back({ x, y, 0.0 });

    }

    return mesh;
 
}
//-------------------------------------------------------------------------
Mesh* Mesh::generaSierpinski(GLdouble rd, GLuint numP) {

    // Genera una malla.
    Mesh* mesh = new Mesh();

    // Genera un pol�gono (vertices,radio)
    Mesh* triangulo = generaPoligono(3, rd);

    // Utiliza la primitiva GL_POINTS
    mesh->mPrimitive = GL_POINTS;

    // Establecemos el n�meros de v�rtices.
    mesh->mNumVertices = numP;
    mesh->vVertices.reserve(mesh->mNumVertices);

    // *********************************************************************************************

    // Se parte de un punto P0 elegido al azar de entre los tres v�rtices Ti. 
    mesh->vVertices.push_back(triangulo->vVertices[rand() % 3]);

    for (size_t k = 1; k <= numP; k++)
    {
        
        // Elegimos aleatoriamente uno de los tres v�rtices.
        glm::dvec3 randVertice = triangulo->vVertices[rand() % 3];

        /* 
        En cada iteraci�n (k>0) generar otro punto Pk calculando el punto medio entre el 
        punto anterior (Pk-1)  y uno de los tres v�rtices iniciales Ti elegido aleatoriamente. 
        */
        glm::dvec3 punto = (mesh->vVertices[k - 1] + randVertice) / 2.0;

        // A�adimos el nuevo punto a la malla.
        mesh->vVertices.push_back(punto);

    }

    // *********************************************************************************************

    // Liberamos los recursos asociados al puntero.
    delete triangulo; 
    triangulo = nullptr;

    return mesh;

}
//-------------------------------------------------------------------------
Mesh* Mesh::generaTrianguloRGB(GLdouble rd) {

    // Genera una malla.
    Mesh* mesh = generaPoligono(3, rd);

    // Utiliza la primitiva GL_TRIANGLES
    mesh->mPrimitive = GL_TRIANGLES; // Cada tres v�rtices es un tri�ngulo.

    // Reserva para 3 vertices
    mesh->mNumVertices = 3;

    // Reserva para 3 colores.
    mesh->vColors.reserve(mesh->mNumVertices);
    
    // A�adir los colores RGB en los v�rtices correspondientes.
    mesh->vColors.push_back({ 1.0, 0.0, 0.0, 1.0 }); // Opacidad 1
    mesh->vColors.push_back({ 0.0, 1.0, 0.0, 1.0 }); // Opacidad 1
    mesh->vColors.push_back({ 0.0, 0.0, 1.0, 1.0 }); // Opacidad 1

    return mesh;
}
//-------------------------------------------------------------------------
Mesh* Mesh::generaRectangulo(GLdouble w, GLdouble h) {

    // Genera una malla.
    Mesh* mesh = new Mesh();

    // Utiliza la primitiva GL_TRIANGLE_STRIP (Cada tres v�rtices, un tri�ngulo).
    mesh->mPrimitive = GL_TRIANGLE_STRIP;

    // Establecemos el n�meros de v�rtices/lados.
    mesh->mNumVertices = 4;
    mesh->vVertices.reserve(mesh->mNumVertices);

   /*
   Damos los v�rtices del rect�ngulo
   Coordenadas OpenGL (El (0,0) es cartesiano, no es la esquina superior izquierda)
   Arriba    (+Y)
   Abajo     (-Y)
   Izquierda (-X)
   Derecha   (+X)
   Fondo     (-Z)
   Traer     (+Z)
   */
    mesh->vVertices.push_back({ -w * 0.5, h * 0.5, 0.0 });
    mesh->vVertices.push_back({ -w * 0.5, -h * 0.5, 0.0 });
    mesh->vVertices.push_back({ w * 0.5, h * 0.5, 0.0 });
    mesh->vVertices.push_back({ w * 0.5, -h * 0.5, 0.0 });

    return mesh;
}
//----------------------------------------------------------------------------
Mesh* Mesh::generaRectanguloRGB(GLdouble w, GLdouble h) {

    // Genera una malla.
    Mesh* mesh = generaRectangulo(w, h);

    // Reserva para 4 colores.
    mesh->vColors.reserve(mesh->mNumVertices);

    // A�adir los colores RGB en los v�rtices correspondientes.
    mesh->vColors.push_back({ 1.0, 0.0, 0.0, 1.0 }); // (R) Opacidad 1
    mesh->vColors.push_back({ 0.0, 1.0, 0.0, 1.0 }); // (G) Opacidad 1
    mesh->vColors.push_back({ 0.0, 0.0, 1.0, 1.0 }); // (B) Opacidad 1
    mesh->vColors.push_back({ 0.5, 0.5, 0.0, 1.0 }); // (Y) Opacidad 1

    return mesh;

}
//----------------------------------------------------------------------------
Mesh* Mesh::generaEstrella3D(GLdouble re, GLuint np, GLdouble h) {

    // Genera una malla.
    Mesh* mesh = new Mesh();

    // Utiliza la primitiva GL_TRIANGLE_FAN.
    mesh->mPrimitive = GL_TRIANGLE_FAN;

    // Establecemos el n�meros de v�rtices.
    mesh->mNumVertices = 2 * np + 2;
    mesh->vVertices.reserve(mesh->mNumVertices);

    GLdouble x = 0;
    GLdouble y = 0;
    GLdouble ang = 90; // Empezamos en el �ngulo de 90�.
    GLdouble angInicial = ang; // �ngulo inicial (para el cierre de la estrella).
    GLdouble incremento = 360.0 / (mesh->mNumVertices - 2); // Quitamos el punto (0, 0, 0).

    // Primer v�rtice (0, 0, 0).
    mesh->vVertices.push_back({ x, y, 0.0 });

    for (size_t i = 1; i < mesh->mNumVertices; i++) { // Quitamos el punto (0, 0, 0) y el cierre (angInicial).

        // Ecuaci�n de la circunferencia con centro C (0,0) y radio R.
        x = 0 + ((i % 2 != 0)? (re) : (re/2)) * cos(radians(ang));
        y = 0 + ((i % 2 != 0)? (re) : (re/2)) * sin(radians(ang));
        ang += incremento;

        // La estrella est� centrada en el plano Z = h.
        mesh->vVertices.push_back({ x, y, h });
        
    }

    return mesh;
}
//----------------------------------------------------------------------------
Mesh* Mesh::generaContCubo(GLdouble ld) {

    // Genera una malla.
    Mesh* mesh = new Mesh();

    // Utiliza la primitiva GL_TRIANGLE_STRIP (Cada tres v�rtices, un tri�ngulo).
    mesh->mPrimitive = GL_TRIANGLE_STRIP;

    // Establecemos el n�meros de v�rtices/lados.
    mesh->mNumVertices = 10;
    mesh->vVertices.reserve(mesh->mNumVertices);

    /*
    Damos los v�rtices del rect�ngulo
    Coordenadas OpenGL (El (0,0) es cartesiano, no es la esquina superior izquierda)
    Arriba         (+Y)
    Abajo          (-Y)
    Izquierda      (+X)
    Derecha        (-X)
    Hacia nosotros (+Z)
    Hacia lo lejos (-Z)
    */
    // Cara frontal
    mesh->vVertices.push_back({ -ld * 0.5, ld * 0.5, 0.5 * ld });
    mesh->vVertices.push_back({ -ld * 0.5, -ld * 0.5, 0.5 * ld });
    mesh->vVertices.push_back({ ld * 0.5, ld * 0.5, 0.5 * ld });
    mesh->vVertices.push_back({ ld * 0.5, -ld * 0.5, 0.5 * ld });
    // Cara derecha
    mesh->vVertices.push_back({ ld * 0.5, ld * 0.5, 0.5 * -ld });
    mesh->vVertices.push_back({ ld * 0.5, -ld * 0.5, 0.5 * -ld });
    // Cara trasera
    mesh->vVertices.push_back({ -ld * 0.5, ld * 0.5, 0.5 * -ld });
    mesh->vVertices.push_back({ -ld * 0.5, -ld * 0.5, 0.5 * -ld });
    // Cara izquierda
    mesh->vVertices.push_back({ -ld * 0.5, ld * 0.5, 0.5 * ld });
    mesh->vVertices.push_back({ -ld * 0.5, -ld * 0.5, 0.5 * ld });

    return mesh;

}
//----------------------------------------------------------------------------
Mesh* Mesh::generaRectanguloTexCor(GLdouble w, GLdouble h, GLuint rw, GLuint rh) {

    Mesh* mesh = generaRectangulo(w, h);
    mesh->vTexCoords.reserve(mesh->mNumVertices);
    
    /*
    Damos las coordenasdas de las texturas (U, V).
    Ejem textura cuadrangular:
    
    A        D
    +--------+
    |XXXXXXXX|
    |XXXXXXXX|
    |XXXXXXXX|
    +--------+
    B        C

    A: (0, 1)
    B: (0, 0)
    C: (1, 0)
    D: (1, 1)

    NOTA: Si nos pasamos del sistema [0 - 1] la textura se repite.
    */

    // El orden de los v�rtices es por la primitiva GL_TRIANGLE_STRIP.
    mesh->vTexCoords.push_back({ 0, rh });
    mesh->vTexCoords.push_back({ 0, 0 });
    mesh->vTexCoords.push_back({ rw, rh });
    mesh->vTexCoords.push_back({ rw, 0 });

    return mesh;
}
//----------------------------------------------------------------------------
Mesh* Mesh::generaEstrellaTexCor(GLdouble re, GLuint np, GLdouble h) {

    // Genera una malla.
    Mesh* mesh = generaEstrella3D(re, np, h);

    // Reservamos memoria para las coordenadas de las texturas.
    mesh->vTexCoords.reserve(mesh->mNumVertices);

    GLdouble ang = 90; // Empezamos en el �ngulo de 90�.
    GLdouble incremento = 360.0 / (mesh->mNumVertices - 2); // Quitamos el punto (0, 0, 0) y el punto de cierre.

    // Primera coordenada de textura.
    mesh->vTexCoords.push_back({ 0.5, 0.5 });

    for (size_t i = 1; i < mesh->mNumVertices; i++, ang += incremento)

        mesh->vTexCoords.push_back({ 0.5 + cos(radians(ang)) * 0.5, 0.5 + sin(radians(ang)) * 0.5 });
    
    return mesh;

}
//----------------------------------------------------------------------------
Mesh* Mesh::generaContCuboTexCor(GLdouble ld) {

    // Genera una malla.
    Mesh* mesh = generaContCubo(ld);

    // Establecemos el n�meros de v�rtices/lados.
    mesh->vTexCoords.reserve(mesh->mNumVertices);

    // A�adimos las coordenadas de textura.
    // La primitiva de los v�rtices es GL_TRIANGLE_STRIP.
    // Cara frontal.
    mesh->vTexCoords.push_back({ 0, 1 });
    mesh->vTexCoords.push_back({ 0, 0 });
    mesh->vTexCoords.push_back({ 1, 1 });
    mesh->vTexCoords.push_back({ 1, 0 });
    // Cara derecha.
    mesh->vTexCoords.push_back({ 2, 1 });
    mesh->vTexCoords.push_back({ 2, 0 });
    // Cara trasera.
    mesh->vTexCoords.push_back({ 3, 1 });
    mesh->vTexCoords.push_back({ 3, 0 });
    // Cara izquierda.
    mesh->vTexCoords.push_back({ 4, 1 });
    mesh->vTexCoords.push_back({ 4, 0 });

    return mesh;
}
//----------------------------------------------------------------------------
Mesh* Mesh::generaPoligonoTexCor(GLuint numL, GLdouble rd) {

    // Genera una malla.
    Mesh* mesh = generaPoligono(numL, rd);

    // Para el dibujado de pol�gonos convexos.
    mesh->mPrimitive = GL_POLYGON;

    // Reservamos memoria para las coordenadas de las texturas.
    mesh->vTexCoords.reserve(mesh->mNumVertices);

    // �ngulo inicial e incremento angular.
    GLdouble ang = 90; // Empezamos en el �ngulo de 90�.
    GLdouble incremento = 360.0 / (mesh->mNumVertices - 1); // Quitamos el punto punto central.

    // Primera coordenada de textura (punto central).
    mesh->vTexCoords.push_back({ 0.5, 0.5 });

    // Restro de coordenadas de textura.
    for (size_t i = 1; i < mesh->mNumVertices; i++, ang += incremento)
        mesh->vTexCoords.push_back({ 0.5 + cos(radians(ang)) * 0.5, 0.5 + sin(radians(ang)) * 0.5 });
  
    return mesh;

}
//----------------------------------------------------------------------------
IndexMesh* IndexMesh::generaAnilloCuadradoIndexado()
{
    // Creaci�n de la malla.
    IndexMesh* mesh = new IndexMesh();

    // Primitiva para dibujar la malla.
    mesh->mPrimitive = GL_TRIANGLE_STRIP;

    // Reservas de memoria.
    mesh->mNumVertices = 8;
    mesh->nNumIndices = 10;
    mesh->vColors.reserve(mesh->mNumVertices);
    mesh->vNormals.reserve(mesh->mNumVertices);
    mesh->vVertices.reserve(mesh->mNumVertices);
    mesh->vIndices = new GLuint[mesh->nNumIndices];

    // Array de v�rtices.
    mesh->vVertices.push_back({ 30.0, 30.0, 0.0 });
    mesh->vVertices.push_back({ 10.0, 10.0, 0.0 });
    mesh->vVertices.push_back({ 70.0, 30.0, 0.0 });
    mesh->vVertices.push_back({ 90.0, 10.0, 0.0 });
    mesh->vVertices.push_back({ 70.0, 70.0, 0.0 });
    mesh->vVertices.push_back({ 90.0, 90.0, 0.0 });
    mesh->vVertices.push_back({ 30.0, 70.0, 0.0 });
    mesh->vVertices.push_back({ 10.0, 90.0, 0.0 });

    // Array de colores.
    mesh->vColors.push_back({ 0.0, 0.0, 0.0, 1.0 });
    mesh->vColors.push_back({ 1.0, 0.0, 0.0, 1.0 });
    mesh->vColors.push_back({ 0.0, 1.0, 0.0, 1.0 });
    mesh->vColors.push_back({ 0.0, 0.0, 1.0, 1.0 });
    mesh->vColors.push_back({ 1.0, 1.0, 0.0, 1.0 });
    mesh->vColors.push_back({ 1.0, 0.0, 1.0, 1.0 });
    mesh->vColors.push_back({ 0.0, 1.0, 1.0, 1.0 });
    mesh->vColors.push_back({ 1.0, 0.0, 0.0, 1.0 });

    // Array de normales.
    for (size_t i = 0; i < mesh->mNumVertices; ++i)
        mesh->vNormals.push_back({ 0.0, 0.0, 1.0 });

    // Array de �ndices.
    for (size_t i = 0; i < mesh->nNumIndices; ++i)
        mesh->vIndices[i] = (i % 8);
    
    return mesh;
}
//----------------------------------------------------------------------------
IndexMesh* IndexMesh::generaCuboConTapasIndexado(GLdouble l)
{

    // Creaci�n de la malla.
    IndexMesh* mesh = new IndexMesh();

    // Primitiva para dibujar la malla.
    mesh->mPrimitive = GL_TRIANGLES;

    // Reservas de memoria.
    mesh->mNumVertices = 8;
    mesh->nNumIndices = 36; // 6 V�rtices por cara.
    mesh->vColors.reserve(mesh->mNumVertices);
    mesh->vVertices.reserve(mesh->mNumVertices);
    mesh->vIndices = new GLuint[mesh->nNumIndices];

    // Coordenadas de los v�rtices.
    // V�rtices frontales (CCW)
    mesh->vVertices.push_back({ -l * 0.5, l * 0.5, l * 0.5 }); // V0
    mesh->vVertices.push_back({ -l * 0.5, -l * 0.5, l * 0.5 }); // V1
    mesh->vVertices.push_back({ l * 0.5, l * 0.5, l * 0.5 }); // V2
    mesh->vVertices.push_back({ l * 0.5, -l * 0.5, l * 0.5 }); // V3
    // V�rtices traseros (CW)
    mesh->vVertices.push_back({l * 0.5, l * 0.5, -l * 0.5}); // V4
    mesh->vVertices.push_back({l * 0.5, -l * 0.5, -l * 0.5}); // V5
    mesh->vVertices.push_back({-l * 0.5, -l * 0.5, -l * 0.5}); // V6
    mesh->vVertices.push_back({-l * 0.5, l * 0.5, -l * 0.5}); // V7

    // Colores de los v�rtices.
    // for (size_t i = 0; i < mesh->mNumVertices; ++i)
    // mesh->vColors.push_back({ 0.0, 1.0, 0.0, 1.0 });

    // �ndices de los v�rtices.
    GLuint indexes[] = {
        0, 1, 2, 1, 3, 2, // Cara frontal.
        4, 5, 6, 4, 6, 7, // Cara trasera.
        2, 3, 4, 3, 5, 4, // Cara derecha.
        6, 1, 0, 7, 6, 0, // Cara izquierda.
        0, 2, 4, 7, 0, 4, // Cara superior.
        6, 3, 1, 6, 5, 3 // Cara inferior.
    };

    for (size_t i = 0; i < mesh->nNumIndices; ++i)
        mesh->vIndices[i] = indexes[i];

    // Construcci�n del vector de normales.
    mesh->buildNormalVectors();

    return mesh;
}
//----------------------------------------------------------------------------

MbR* MbR::generaMallaIndexadaPorRevolucion(int mm, int nn, glm::dvec3* perfil)
{
    /*
    Creaci�n de la malla.
    mm: N�mero de puntos del perfil.
    nn: N�mero de rotaciones (muestras que se toman).
    perfil: Perfil definido en el plano XY (se dan desde ABAJO hacia ARRIBA).
    */
    MbR* mesh = new MbR(mm, nn, perfil);
    
    // Primitiva para dibujar la malla.
    mesh->mPrimitive = GL_TRIANGLES;

    // Total de v�rtices (nn * mm):
    mesh->mNumVertices = nn * mm;
    mesh->vVertices.reserve(mesh->mNumVertices);
    // Vector auxiliar de v�rtices.
    dvec3* vertices = new dvec3[mesh->mNumVertices];
  
    /*
    Total de �ndices:
    (m - 1) * n --> N� Cuadrados
    2 Tri�ngulos
    3 �ndices por tri�ngulo    
    */
    mesh->nNumIndices = (((mm-1) * nn) * 2) * 3;
    mesh->vIndices = new GLuint[mesh->nNumIndices];

    // Calcula los (nn * mm) v�rtices (en el espacio).
    for (int i = 0; i < nn; ++i) {
        // Generar la muestra i-�simade v�rtices
        GLdouble theta = i * 360 / nn;
        GLdouble c = cos(radians(theta));
        GLdouble s = sin(radians(theta));
        // R_y(theta): matriz de rotaci�n alrededor del eje Y
        for (int j = 0; j < mm; j++) {
            int indice = i * mm + j;
            GLdouble x = c * perfil[j].x + s * perfil[j].z;
            GLdouble z = -s * perfil[j].x + c * perfil[j].z;
            vertices[indice] = dvec3(x, perfil[j].y, z);
        }
    }

    // Volcar el array auxiliar vertices en el array de v�rtices mesh->vVertices.
    for (size_t i = 0; i < mesh->mNumVertices; i++)
        mesh->vVertices.push_back(vertices[i]);
    
    // Liberar el array auxiliar.
    delete []vertices;
    vertices = nullptr;

    /*
    Determinamos los �ndices de las caras cuadrangulares.
    i: N� de rotaci�n / muestras tomadas al rededor del eje Y.
    j: Para recorrer los v�rtices del perfil (desde ABAJO hacia ARRIBA).
    */
    for (int i = 0, indiceMayor = 0; i < nn; ++i)
        
        for (int j = 0; j < mm - 1; ++j) {
            
            // indice = rotacion(i) * puntosPerfil(mm) + verticeActualDelPerfil(j).
            int indice = i * mm + j; 
    
            /*
            Las caras cuadrangulares resultan al unir la muestra i-�sima con la (i+1) % nn

            Los cuatro �ndices de la cara cuadrangular son: indice, (indice + mm) % (nn * mm), (indice + mm + 1) % (nn * mm), indice + 1

            Hay que recorrer la cara cuadrangular desde la esquina inferior izquierda en el sentido CCW.

            (indice+1)              (indice+mm+1) % (nn*mm)
            +************************+
            *                        *
            *                        * 
            *                        *
            *                        * 
            *                        *
            *                        *
            *                        * 
            *                        * 
            +************************+
            (indice)                (indice+mm) % (nn*mm)

            */

            // Damos los �ndices que conformar�n los tri�ngulos.
            // Tri�ngulo inferior.
            mesh->vIndices[indiceMayor++] = indice;
            mesh->vIndices[indiceMayor++] = (indice + mm) % (nn * mm);
            mesh->vIndices[indiceMayor++] = (indice + mm + 1) % (nn * mm);
            // Tri�ngulo superior.
            mesh->vIndices[indiceMayor++] = indice;
            mesh->vIndices[indiceMayor++] = (indice + mm + 1) % (nn * mm);
            mesh->vIndices[indiceMayor++] = (indice + 1);

        }
    
    // Construcci�n del vector de normales.
    mesh->buildNormalVectors();

    return mesh;
}
//----------------------------------------------------------------------------

IndexMesh* IndexMesh::generateGrid(GLdouble lado, GLuint numDiv)
{

    // Creaci�n de la malla.
    IndexMesh* mesh = new IndexMesh();

    // Primitiva para dibujar la malla.
    mesh->mPrimitive = GL_TRIANGLES;

    // Total de v�rtices.
    GLuint numFC = numDiv + 1;
    mesh->mNumVertices = numFC * numFC;
    mesh->vVertices.reserve(mesh->mNumVertices);

    
    // Total de �ndices.
    mesh->nNumIndices = numDiv * numDiv * 2 * 3;
    mesh->vIndices = new GLuint[mesh->nNumIndices];
    
    // Posicionamos los v�rtices.
    GLdouble incr = lado / numDiv;
    for (size_t row = 0; row < numFC; ++row)
        for (size_t column = 0; column < numFC; ++column)
            mesh->vVertices.push_back({ (-lado * 0.5) + (incr * column), 0.0, (-lado * 0.5) + (incr * row) });
    
    // Damos los �ndices.
    for (GLuint row = 0, index = 0; row < numFC - 1; ++row)
        for (GLuint column = 0; column < numFC - 1; ++column) {

            // �ndice del primer v�rtice del cuadrado.
            GLuint iv = row * numFC + column;

            // Tri�ngulo superior.
            mesh->vIndices[index++] = iv;
            mesh->vIndices[index++] = iv + numFC;
            mesh->vIndices[index++] = iv + 1;

            // Tri�ngulo inferior.
            mesh->vIndices[index++] = iv + 1;
            mesh->vIndices[index++] = iv + numFC;
            mesh->vIndices[index++] = iv + numFC + 1;

        }

    // Construcci�n del vector de normales.
    mesh->buildNormalVectors();

    return mesh;

}

//----------------------------------------------------------------------------

IndexMesh* IndexMesh::generateGridTex(GLdouble lado, GLuint numDiv)
{ 
    IndexMesh* mesh = generateGrid(lado, numDiv);
    mesh->vTexCoords.reserve(mesh->mNumVertices);
    
    GLuint numFC = numDiv + 1;
    GLdouble incr = 1.0 / numDiv;
    
    for (GLuint row = 0; row < numFC; ++row)
        for (GLuint column = 0; column < numFC; ++column)
            mesh->vTexCoords.push_back({ 0.0 + (incr * column), 1.0 - (incr * row) });

    return mesh;
}
//----------------------------------------------------------------------------

IndexMesh* IndexMesh::generaPiramide(GLint caras, GLdouble radio)
{
    
    // Creaci�n de la malla.
    IndexMesh* mesh = new IndexMesh();

    // Primitiva para dibujar la malla.
    mesh->mPrimitive = GL_TRIANGLES;

    // Total de v�rtices.
    mesh->mNumVertices = caras + 1;
    mesh->vVertices.reserve(mesh->mNumVertices);
    
    // Total de �ndices.
    mesh->nNumIndices = 3 * caras;
    mesh->vIndices = new GLuint[mesh->nNumIndices];

    // Variables locales
    GLdouble angle = 0.0;
    GLdouble incr = 360.0 / (mesh->mNumVertices - 1);

    // Vertices de la base
    for (size_t i = 0; i < (mesh->mNumVertices - 1); ++i, angle += incr)
    {
        GLdouble x = 0.0 + radio * cos(radians(angle));
        GLdouble y = 0.0 + radio * sin(radians(angle));
        mesh->vVertices.push_back({ x, 0.0, y });
    }

    mesh->vVertices.push_back({ 0.0, radio, 0.0 });

    // Vertices de colores
    srand(time(NULL));
    mesh->vColors.reserve(mesh->mNumVertices);
    for (size_t i = 0; i < mesh->mNumVertices; i++)
        mesh->vColors.push_back({ rand() % 2, rand() % 2, rand() % 2, 1.0 });

    // Indices
    size_t i = 0;
    size_t index = 0; 

    for (; index < mesh->nNumIndices - 3; index += 3, i++) {
        mesh->vIndices[index] = i;
        mesh->vIndices[index + 1] = i + 1;
        mesh->vIndices[index + 2] = mesh->mNumVertices - 1;
    }

    mesh->vIndices[index] = i;
    mesh->vIndices[index + 1] = 0;
    mesh->vIndices[index + 2] = mesh->mNumVertices - 1;

    // Construcci�n del vector de normales.
    mesh->buildNormalVectors();

    return mesh;

}