/***********************************************************************
 * Module:  Profesor.cpp
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:35:39
 * Purpose: Implementation of the class Profesor
 ***********************************************************************/

// Archivo: Profesor.cpp
// Contiene la implementación de la clase Profesor.

#include "FranjaHoraria.h"
#include "Profesor.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////
// Name:       Profesor::Profesor()
// Purpose:    Implementation of Profesor::Profesor()
// Return:
////////////////////////////////////////////////////////////////////////

// Profesor sin datos: Persona() deja los textos vacíos y se crea la lista de franjas vacía.
Profesor::Profesor() : Persona(), franjas(new ListaDoble<FranjaHoraria>())
{
}

////////////////////////////////////////////////////////////////////////
// Name:       Profesor::Profesor(std::string cedula, std::string nombre, std::string apellido)
// Purpose:    Implementation of Profesor::Profesor()
// Parameters:
// - cedula
// - nombre
// - apellido
// Return:
////////////////////////////////////////////////////////////////////////

// Los datos personales los guarda Persona; aquí solo se crea la lista propia de franjas.
Profesor::Profesor(std::string cedula, std::string nombre, std::string apellido)
   : Persona(cedula, nombre, apellido), franjas(new ListaDoble<FranjaHoraria>())
{
}

////////////////////////////////////////////////////////////////////////
// Name:       Profesor::~Profesor()
// Purpose:    Implementation of Profesor::~Profesor()
// Return:
////////////////////////////////////////////////////////////////////////

// Al borrar la lista, su destructor libera cada FranjaHoraria, y cada franja libera sus horas.
Profesor::~Profesor()
{
   delete franjas;
   franjas = nullptr;
}

////////////////////////////////////////////////////////////////////////
// Name:       Profesor::getFranjas()
// Purpose:    Implementation of Profesor::getFranjas()
// Return:     ListaDoble<FranjaHoraria>*
////////////////////////////////////////////////////////////////////////

// Devuelve la lista sin transferir su propiedad.
ListaDoble<FranjaHoraria>* Profesor::getFranjas(void) const
{
   return franjas;
}

////////////////////////////////////////////////////////////////////////
// Name:       Profesor::agregarFranja(FranjaHoraria* franja)
// Purpose:    Implementation of Profesor::agregarFranja()
// Parameters:
// - franja
// Return:     bool
////////////////////////////////////////////////////////////////////////

// Aplica las validaciones en orden y, si todas pasan, guarda la franja.
// Si devuelve false NO se libera la franja: quien llamó sigue siendo su dueño (decisión 4).
bool Profesor::agregarFranja(FranjaHoraria* franja)
{
   // Validación 1 - Datos completos: sin franja o sin horas no hay nada que comprobar
   if (franja == nullptr || franja->getHoraInicio() == nullptr || franja->getHoraFinal() == nullptr)
   {
      return false;
   }

   // Validación 2 - La franja debe pertenecer a este profesor. Si se corrigiera en silencio
   // se escondería un error de quien llamó, y la clave de la franja no coincidiría con su dueño
   if (franja->getCedulaProfesor() != getCedula())
   {
      return false;
   }

   // Validación 3 - Debe ser una de las 7 franjas predefinidas (sección 1):
   Hora* inicio = franja->getHoraInicio();
   Hora* fin = franja->getHoraFinal();

   //  a) Los minutos de ambas horas deben ser 00 (rechaza 07:30-08:15)
   if (inicio->getMinutos() != 0 || fin->getMinutos() != 0)
   {
      return false;
   }

   //  b) La hora de inicio debe ser 07, 09, 11, 13, 15, 17 o 19: un número impar entre 7 y 19.
   //     Rechaza 08:00 (no empieza en un bloque), 05:00 (antes de las 07:00) y 21:00 (pasa de las 21:00)
   int horaInicio = inicio->getHoras();
   if (horaInicio < 7 || horaInicio > 19 || horaInicio % 2 == 0)
   {
      return false;
   }

   //  c) El fin debe ser exactamente 2 horas (120 minutos) después del inicio.
   //     Rechaza 07:00-08:00 (dura 1 hora) y 09:00-07:00 (el fin es anterior al inicio)
   if (fin->getTotalMinutos() != inicio->getTotalMinutos() + 120)
   {
      return false;
   }

   // Validación 4 - Duplicada: la clave (cédula + hora de inicio) ya existe en la lista.
   // También rechaza la misma franja con otra aula, porque el aula no forma parte de la clave
   if (franjas->buscar(franja->getClave()) != nullptr)
   {
      return false;
   }

   // Validación 5 - Cruce de horarios con otra franja del profesor.
   // IMPORTANTE: hoy esta validación no se activa nunca. Como las franjas son fijas y están
   // alineadas cada 2 horas, dos franjas predefinidas distintas no pueden solaparse, y la única
   // forma de "cruce" (la misma franja) ya la rechaza la validación 4. Se deja por si algún día
   // cambian los bloques horarios; por eso no tiene una prueba de unidad propia.
   Nodo<FranjaHoraria>* actual = franjas->getCabeza();
   while (actual != nullptr)
   {
      FranjaHoraria* existente = actual->getDato();
      // Dos intervalos se cruzan si cada uno empieza antes de que el otro termine.
      // Se usa < estricto para que franjas consecutivas (7-9 y 9-11) no cuenten como cruce
      bool seCruzan = inicio->getTotalMinutos() < existente->getHoraFinal()->getTotalMinutos()
                   && existente->getHoraInicio()->getTotalMinutos() < fin->getTotalMinutos();
      if (seCruzan)
      {
         return false;
      }
      actual = actual->getSiguiente();
   }

   // Todas las validaciones pasaron: desde aquí la lista del profesor es dueña de la franja
   franjas->insertarFinal(franja);
   return true;
}

////////////////////////////////////////////////////////////////////////
// Name:       Profesor::buscarFranjaParaEntrada(Hora* hora)
// Purpose:    Implementation of Profesor::buscarFranjaParaEntrada()
// Parameters:
// - hora
// Return:     FranjaHoraria*
////////////////////////////////////////////////////////////////////////

// Recorre las franjas y devuelve la primera cuya ventana de entrada contiene la hora.
FranjaHoraria* Profesor::buscarFranjaParaEntrada(Hora* hora) const
{
   // Sin hora no hay ventana que comparar
   if (hora == nullptr)
   {
      return nullptr;
   }

   Nodo<FranjaHoraria>* actual = franjas->getCabeza();
   while (actual != nullptr)
   {
      // La franja decide si la hora está en [inicio, inicio + 10]
      if (actual->getDato()->puedeRegistrarEntrada(hora))
      {
         return actual->getDato();   // sigue siendo del profesor; quien la recibe no la libera
      }
      actual = actual->getSiguiente();
   }
   // Ninguna franja acepta la entrada a esa hora (o el profesor no tiene franjas)
   return nullptr;
}

////////////////////////////////////////////////////////////////////////
// Name:       Profesor::buscarFranjaParaSalida(Hora* hora)
// Purpose:    Implementation of Profesor::buscarFranjaParaSalida()
// Parameters:
// - hora
// Return:     FranjaHoraria*
////////////////////////////////////////////////////////////////////////

// Recorre las franjas y devuelve la primera cuya ventana de salida contiene la hora.
// Con franjas consecutivas (7-9 y 9-11), a las 09:05 la salida es de 7-9, porque
// la ventana de salida se mide desde la hora final de cada franja.
FranjaHoraria* Profesor::buscarFranjaParaSalida(Hora* hora) const
{
   // Sin hora no hay ventana que comparar
   if (hora == nullptr)
   {
      return nullptr;
   }

   Nodo<FranjaHoraria>* actual = franjas->getCabeza();
   while (actual != nullptr)
   {
      // La franja decide si la hora está en [final, final + 10]
      if (actual->getDato()->puedeRegistrarSalida(hora))
      {
         return actual->getDato();   // sigue siendo del profesor; quien la recibe no la libera
      }
      actual = actual->getSiguiente();
   }
   // Ninguna franja acepta la salida a esa hora (o el profesor no tiene franjas)
   return nullptr;
}

////////////////////////////////////////////////////////////////////////
// Name:       Profesor::mostrar()
// Purpose:    Implementation of Profesor::mostrar()
// Return:     void
////////////////////////////////////////////////////////////////////////

// Reutiliza Persona::mostrar() para los datos personales y luego lista las franjas.
void Profesor::mostrar(void) const
{
   Persona::mostrar();   // primera línea: cédula y nombre

   if (franjas->estaVacia())
   {
      std::cout << "  Sin franjas asignadas" << std::endl;
      return;
   }

   // No se usa mostrarTodos() porque cada franja debe ir con sangría
   Nodo<FranjaHoraria>* actual = franjas->getCabeza();
   while (actual != nullptr)
   {
      std::cout << "  ";
      actual->getDato()->mostrar();
      actual = actual->getSiguiente();
   }
}
