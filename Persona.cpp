/***********************************************************************
 * Module:  Persona.cpp
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 9:04:15
 * Purpose: Implementation of the class Persona
 ***********************************************************************/

// Archivo: Persona.cpp
// Contiene la implementación de la clase Persona.

#include "Persona.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////
// Name:       Persona::Persona()
// Purpose:    Implementation of Persona::Persona()
// Return:
////////////////////////////////////////////////////////////////////////

// Deja los tres textos vacíos (std::string ya se inicializa vacío; se escribe explícito por claridad).
Persona::Persona() : cedula(""), nombre(""), apellido("")
{
}

////////////////////////////////////////////////////////////////////////
// Name:       Persona::Persona(std::string cedula, std::string nombre, std::string apellido)
// Purpose:    Implementation of Persona::Persona()
// Parameters:
// - cedula
// - nombre
// - apellido
// Return:
////////////////////////////////////////////////////////////////////////

// Guarda los datos recibidos. En la lista de inicialización, el nombre de afuera es el
// atributo y el de adentro del paréntesis es el parámetro, aunque se llamen igual.
Persona::Persona(std::string cedula, std::string nombre, std::string apellido)
   : cedula(cedula), nombre(nombre), apellido(apellido)
{
}

////////////////////////////////////////////////////////////////////////
// Name:       Persona::getCedula()
// Purpose:    Implementation of Persona::getCedula()
// Return:     std::string
////////////////////////////////////////////////////////////////////////

// Devuelve la cédula guardada.
std::string Persona::getCedula(void) const
{
   return cedula;
}

////////////////////////////////////////////////////////////////////////
// Name:       Persona::setCedula(std::string newCedula)
// Purpose:    Implementation of Persona::setCedula()
// Parameters:
// - newCedula
// Return:     void
////////////////////////////////////////////////////////////////////////

// Reemplaza la cédula.
void Persona::setCedula(std::string newCedula)
{
   cedula = newCedula;
}

////////////////////////////////////////////////////////////////////////
// Name:       Persona::getNombre()
// Purpose:    Implementation of Persona::getNombre()
// Return:     std::string
////////////////////////////////////////////////////////////////////////

// Devuelve el nombre guardado.
std::string Persona::getNombre(void) const
{
   return nombre;
}

////////////////////////////////////////////////////////////////////////
// Name:       Persona::setNombre(std::string newNombre)
// Purpose:    Implementation of Persona::setNombre()
// Parameters:
// - newNombre
// Return:     void
////////////////////////////////////////////////////////////////////////

// Reemplaza el nombre.
void Persona::setNombre(std::string newNombre)
{
   nombre = newNombre;
}

////////////////////////////////////////////////////////////////////////
// Name:       Persona::getApellido()
// Purpose:    Implementation of Persona::getApellido()
// Return:     std::string
////////////////////////////////////////////////////////////////////////

// Devuelve el apellido guardado.
std::string Persona::getApellido(void) const
{
   return apellido;
}

////////////////////////////////////////////////////////////////////////
// Name:       Persona::setApellido(std::string newApellido)
// Purpose:    Implementation of Persona::setApellido()
// Parameters:
// - newApellido
// Return:     void
////////////////////////////////////////////////////////////////////////

// Reemplaza el apellido.
void Persona::setApellido(std::string newApellido)
{
   apellido = newApellido;
}

////////////////////////////////////////////////////////////////////////
// Name:       Persona::getClave()
// Purpose:    Implementation of Persona::getClave()
// Return:     std::string
////////////////////////////////////////////////////////////////////////

// La cédula identifica de forma única a la persona, por eso es su clave
// en la ListaDoble (sección 4, tabla de claves).
std::string Persona::getClave(void) const
{
   return cedula;
}

////////////////////////////////////////////////////////////////////////
// Name:       Persona::mostrar()
// Purpose:    Implementation of Persona::mostrar()
// Return:     void
////////////////////////////////////////////////////////////////////////

// Imprime los datos de la persona en una línea.
void Persona::mostrar(void) const
{
   std::cout << "Cédula: " << cedula << " | Nombre: " << nombre << " " << apellido << std::endl;
}
