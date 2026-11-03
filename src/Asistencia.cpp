/***********************************************************************
 * Module:  Asistencia.cpp
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:35:39
 * Purpose: Implementation of the class Asistencia
 ***********************************************************************/

// Archivo: Asistencia.cpp
// Contiene la implementación de la clase Asistencia.

#include "Hora.h"
#include "Asistencia.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////
// Name:       Asistencia::Asistencia()
// Purpose:    Implementation of Asistencia::Asistencia()
// Return:     
////////////////////////////////////////////////////////////////////////

// Asistencia vacía: punteros en nullptr para que el destructor pueda hacer delete sin riesgo.
Asistencia::Asistencia()
   : cedulaProfesor(""), fecha(""), horaInicioFranja(nullptr),
     horaEntrada(nullptr), horaSalida(nullptr), estado(SIN_REGISTRO)
{
}

////////////////////////////////////////////////////////////////////////
// Name:       Asistencia::Asistencia(std::string cedulaProfesor, std::string fecha, Hora* horaInicioFranja)
// Purpose:    Implementation of Asistencia::Asistencia()
// Parameters:
// - cedulaProfesor
// - fecha
// - horaInicioFranja
// Return:     
////////////////////////////////////////////////////////////////////////

// Guarda los datos y una copia de la hora de la franja. Todavía no hay entrada ni salida.
Asistencia::Asistencia(std::string cedulaProfesor, std::string fecha, Hora* horaInicioFranja)
   : cedulaProfesor(cedulaProfesor), fecha(fecha), horaInicioFranja(nullptr),
     horaEntrada(nullptr), horaSalida(nullptr), estado(SIN_REGISTRO)
{
   // La hora recibida es de la franja: se crea un objeto nuevo con el mismo valor para
   // no compartir el puntero (si no, la franja y la asistencia harían delete del mismo objeto).
   // this->horaInicioFranja es el atributo; horaInicioFranja (sin this) es el parámetro.
   if (horaInicioFranja != nullptr)
   {
      this->horaInicioFranja = new Hora(horaInicioFranja->getHoras(), horaInicioFranja->getMinutos());
   }
}

////////////////////////////////////////////////////////////////////////
// Name:       Asistencia::~Asistencia()
// Purpose:    Implementation of Asistencia::~Asistencia()
// Return:     
////////////////////////////////////////////////////////////////////////

// Libera las tres horas, de las que la asistencia es dueña (delete de nullptr no hace nada).
Asistencia::~Asistencia()
{
   delete horaInicioFranja;
   delete horaEntrada;
   delete horaSalida;
   horaInicioFranja = nullptr;
   horaEntrada = nullptr;
   horaSalida = nullptr;
}

////////////////////////////////////////////////////////////////////////
// Name:       Asistencia::getCedulaProfesor()
// Purpose:    Implementation of Asistencia::getCedulaProfesor()
// Return:     std::string
////////////////////////////////////////////////////////////////////////

// Devuelve la cédula del profesor.
std::string Asistencia::getCedulaProfesor(void) const
{
   return cedulaProfesor;
}

////////////////////////////////////////////////////////////////////////
// Name:       Asistencia::getFecha()
// Purpose:    Implementation of Asistencia::getFecha()
// Return:     std::string
////////////////////////////////////////////////////////////////////////

// Devuelve la fecha de la asistencia.
std::string Asistencia::getFecha(void) const
{
   return fecha;
}

////////////////////////////////////////////////////////////////////////
// Name:       Asistencia::getHoraEntrada()
// Purpose:    Implementation of Asistencia::getHoraEntrada()
// Return:     Hora*
////////////////////////////////////////////////////////////////////////

// Devuelve la hora de entrada sin transferir su propiedad.
Hora* Asistencia::getHoraEntrada(void) const
{
   return horaEntrada;
}

////////////////////////////////////////////////////////////////////////
// Name:       Asistencia::setHoraEntrada(Hora* newHoraEntrada)
// Purpose:    Implementation of Asistencia::setHoraEntrada()
// Parameters:
// - newHoraEntrada
// Return:     void
////////////////////////////////////////////////////////////////////////

// Reemplaza la hora de entrada liberando la anterior. No toca el estado:
// solo registrarEntrada y registrarSalida pueden cambiarlo.
void Asistencia::setHoraEntrada(Hora* newHoraEntrada)
{
   // Si es el mismo puntero, borrarlo dejaría a la asistencia apuntando a memoria liberada
   if (newHoraEntrada == horaEntrada)
   {
      return;
   }
   delete horaEntrada;             // se libera la hora anterior
   horaEntrada = newHoraEntrada;   // y se toma posesión de la nueva
}

////////////////////////////////////////////////////////////////////////
// Name:       Asistencia::getHoraSalida()
// Purpose:    Implementation of Asistencia::getHoraSalida()
// Return:     Hora*
////////////////////////////////////////////////////////////////////////

// Devuelve la hora de salida sin transferir su propiedad.
Hora* Asistencia::getHoraSalida(void) const
{
   return horaSalida;
}

////////////////////////////////////////////////////////////////////////
// Name:       Asistencia::setHoraSalida(Hora* newHoraSalida)
// Purpose:    Implementation of Asistencia::setHoraSalida()
// Parameters:
// - newHoraSalida
// Return:     void
////////////////////////////////////////////////////////////////////////

// Reemplaza la hora de salida liberando la anterior. No toca el estado.
void Asistencia::setHoraSalida(Hora* newHoraSalida)
{
   // Si es el mismo puntero, borrarlo dejaría a la asistencia apuntando a memoria liberada
   if (newHoraSalida == horaSalida)
   {
      return;
   }
   delete horaSalida;            // se libera la hora anterior
   horaSalida = newHoraSalida;   // y se toma posesión de la nueva
}

////////////////////////////////////////////////////////////////////////
// Name:       Asistencia::getEstado()
// Purpose:    Implementation of Asistencia::getEstado()
// Return:     int
////////////////////////////////////////////////////////////////////////

// Devuelve el estado numérico (0, 1 o 2).
int Asistencia::getEstado(void) const
{
   return estado;
}

////////////////////////////////////////////////////////////////////////
// Name:       Asistencia::estaSinRegistro()
// Purpose:    Implementation of Asistencia::estaSinRegistro()
// Return:     bool
////////////////////////////////////////////////////////////////////////

// Estado 0: no se ha registrado nada todavía.
bool Asistencia::estaSinRegistro(void) const
{
   return estado == SIN_REGISTRO;
}

////////////////////////////////////////////////////////////////////////
// Name:       Asistencia::tieneEntrada()
// Purpose:    Implementation of Asistencia::tieneEntrada()
// Return:     bool
////////////////////////////////////////////////////////////////////////

// Estado 1: hay entrada y falta la salida.
bool Asistencia::tieneEntrada(void) const
{
   return estado == ENTRADA_REGISTRADA;
}

////////////////////////////////////////////////////////////////////////
// Name:       Asistencia::getHoraInicioFranja()
// Purpose:    Implementation of Asistencia::getHoraInicioFranja()
// Return:     Hora*
////////////////////////////////////////////////////////////////////////

// Devuelve la copia de la hora de inicio de la franja sin transferir su propiedad.
Hora* Asistencia::getHoraInicioFranja(void) const
{
   return horaInicioFranja;
}

////////////////////////////////////////////////////////////////////////
// Name:       Asistencia::mostrar()
// Purpose:    Implementation of Asistencia::mostrar()
// Return:     void
////////////////////////////////////////////////////////////////////////

// Imprime la asistencia en una línea con el formato acordado.
void Asistencia::mostrar(void) const
{
   // Las horas que todavía no existen se muestran como "--:--" en lugar de acceder a nullptr
   std::string franja = (horaInicioFranja != nullptr) ? horaInicioFranja->toString() : "--:--";
   std::string entrada = (horaEntrada != nullptr) ? horaEntrada->toString() : "--:--";
   std::string salida = (horaSalida != nullptr) ? horaSalida->toString() : "--:--";

   // Texto del estado para que el usuario no tenga que interpretar 0, 1 o 2
   std::string textoEstado = "Sin registro";
   if (estado == ENTRADA_REGISTRADA)
   {
      textoEstado = "Entrada registrada";
   }
   else if (estado == COMPLETA)
   {
      textoEstado = "Completa";
   }

   std::cout << fecha << " | Cédula: " << cedulaProfesor
             << " | Franja: " << franja
             << " | Entrada: " << entrada
             << " | Salida: " << salida
             << " | Estado: " << textoEstado << std::endl;
}

////////////////////////////////////////////////////////////////////////
// Name:       Asistencia::estaCompleta()
// Purpose:    Implementation of Asistencia::estaCompleta()
// Return:     bool
////////////////////////////////////////////////////////////////////////

// Estado 2: entrada y salida registradas.
bool Asistencia::estaCompleta(void) const
{
   return estado == COMPLETA;
}

////////////////////////////////////////////////////////////////////////
// Name:       Asistencia::registrarEntrada(Hora* hora)
// Purpose:    Implementation of Asistencia::registrarEntrada()
// Parameters:
// - hora
// Return:     void
////////////////////////////////////////////////////////////////////////

// Transición 0 -> 1. Solo se permite desde "sin registro": una segunda entrada no cambia nada.
void Asistencia::registrarEntrada(Hora* hora)
{
   if (hora == nullptr || estado != SIN_REGISTRO)
   {
      return;
   }
   // Se guarda una copia: la hora recibida sigue siendo de quien llamó.
   // Se libera la hora anterior por si se había asignado una con setHoraEntrada
   delete horaEntrada;
   horaEntrada = new Hora(hora->getHoras(), hora->getMinutos());
   estado = ENTRADA_REGISTRADA;
}

////////////////////////////////////////////////////////////////////////
// Name:       Asistencia::registrarSalida(Hora* hora)
// Purpose:    Implementation of Asistencia::registrarSalida()
// Parameters:
// - hora
// Return:     void
////////////////////////////////////////////////////////////////////////

// Transición 1 -> 2. Sin entrada previa (estado 0) o con la asistencia ya completa (estado 2)
// no se registra nada: no puede haber salida sin entrada ni dos salidas.
void Asistencia::registrarSalida(Hora* hora)
{
   if (hora == nullptr || estado != ENTRADA_REGISTRADA)
   {
      return;
   }
   // Se guarda una copia: la hora recibida sigue siendo de quien llamó.
   // Se libera la hora anterior por si se había asignado una con setHoraSalida
   delete horaSalida;
   horaSalida = new Hora(hora->getHoras(), hora->getMinutos());
   estado = COMPLETA;
}

////////////////////////////////////////////////////////////////////////
// Name:       Asistencia::getClave()
// Purpose:    Implementation of Asistencia::getClave()
// Return:     std::string
////////////////////////////////////////////////////////////////////////

// Cédula + fecha + hora de inicio de la franja identifican la asistencia: así no puede
// haber dos asistencias para el mismo profesor, la misma fecha y la misma franja.
std::string Asistencia::getClave(void) const
{
   // Sin hora de franja solo quedan la cédula y la fecha
   if (horaInicioFranja == nullptr)
   {
      return cedulaProfesor + "-" + fecha + "-";
   }
   return cedulaProfesor + "-" + fecha + "-" + horaInicioFranja->toString();
}
