/***********************************************************************
 * Module:  Validador.cpp
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 9:47:10
 * Purpose: Implementation of the class Validador
 ***********************************************************************/

// Archivo: Validador.cpp
// Contiene la implementación de la clase Validador.
// Cada expresión regular se declara static const dentro de su método para que
// se construya una sola vez y no en cada llamada.

#include "Validador.h"
#include <regex>

////////////////////////////////////////////////////////////////////////
// Name:       Validador::validarCedula(std::string cedula)
// Purpose:    Implementation of Validador::validarCedula()
// Parameters:
// - cedula
// Return:     bool
////////////////////////////////////////////////////////////////////////

// Valida una cédula ecuatoriana con el algoritmo oficial de módulo 10.
bool Validador::validarCedula(std::string cedula)
{
   // Paso 1 - Formato: exactamente 10 dígitos.
   // Acepta "1710034065". Rechaza vacío, 9 u 11 dígitos, letras, espacios o guiones.
   static const std::regex formato(R"(^\d{10}$)");
   if (!std::regex_match(cedula, formato))
   {
      return false;
   }

   // Paso 2 - Provincia: los dos primeros dígitos entre 01 y 24, o 30 (registrados en el exterior).
   // 0[1-9] cubre 01-09, 1\d cubre 10-19, 2[0-4] cubre 20-24.
   // Rechaza 00, 25-29 y 31-99.
   static const std::regex provincia(R"(^(0[1-9]|1\d|2[0-4]|30)\d{8}$)");
   if (!std::regex_match(cedula, provincia))
   {
      return false;
   }

   // Paso 3 - Tercer dígito menor que 6 (0 a 5 corresponde a persona natural).
   // Rechaza 6, 7, 8 y 9 en la tercera posición.
   static const std::regex tercerDigito(R"(^\d{2}[0-5]\d{7}$)");
   if (!std::regex_match(cedula, tercerDigito))
   {
      return false;
   }

   // Paso 4 - Dígito verificador. Una regex no puede hacer cuentas, por eso este paso es aritmético.
   // A estas alturas la regex ya garantizó que los 10 caracteres son dígitos.
   int suma = 0;
   for (int i = 0; i < 9; i++)   // se recorren solo los primeros 9 dígitos
   {
      // Restar '0' convierte el carácter del dígito en su valor numérico ('7' - '0' = 7)
      int digito = cedula[i] - '0';

      // Las posiciones impares (1.ª, 3.ª, 5.ª, 7.ª, 9.ª) son los índices pares 0, 2, 4, 6, 8:
      // esas se multiplican por 2; las posiciones pares se multiplican por 1 (quedan igual).
      if (i % 2 == 0)
      {
         digito = digito * 2;

         // Si el producto pasa de 9 se le resta 9 (equivale a sumar sus dos cifras: 14 -> 1+4 = 5)
         if (digito > 9)
         {
            digito = digito - 9;
         }
      }

      // Se acumulan todos los resultados
      suma = suma + digito;
   }

   // El verificador es lo que le falta a la suma para llegar a la siguiente decena.
   // El % 10 final hace que, si la suma ya es múltiplo de 10, el verificador sea 0 y no 10.
   int verificador = (10 - (suma % 10)) % 10;

   // La cédula es real solo si el verificador calculado coincide con el décimo dígito
   int decimoDigito = cedula[9] - '0';
   return verificador == decimoDigito;
}

////////////////////////////////////////////////////////////////////////
// Name:       Validador::validarNombre(std::string nombre)
// Purpose:    Implementation of Validador::validarNombre()
// Parameters:
// - nombre
// Return:     bool
////////////////////////////////////////////////////////////////////////

// Valida un nombre o apellido.
bool Validador::validarNombre(std::string nombre)
{
   // Una "letra" es una de A-Z, a-z o una vocal con tilde, ñ o ü.
   // Las letras con tilde ocupan 2 bytes en UTF-8, por eso van como alternativas (|)
   // y no dentro de [ ], donde la regex las partiría en bytes sueltos.
   // Estructura: una palabra de letras, seguida de cero o más grupos "un espacio + palabra".
   // Acepta "Juan", "María José", "Ñañez".
   // Rechaza vacío, solo espacios, espacios al inicio o al final, dos espacios seguidos,
   // dígitos ("Juan3") y símbolos.
   static const std::regex patron(
      R"(^(?:[A-Za-z]|á|é|í|ó|ú|Á|É|Í|Ó|Ú|ñ|Ñ|ü|Ü)+)"
      R"((?: (?:[A-Za-z]|á|é|í|ó|ú|Á|É|Í|Ó|Ú|ñ|Ñ|ü|Ü)+)*$)");
   return std::regex_match(nombre, patron);
}

////////////////////////////////////////////////////////////////////////
// Name:       Validador::validarHora(std::string horaIngresada)
// Purpose:    Implementation of Validador::validarHora()
// Parameters:
// - horaIngresada
// Return:     bool
////////////////////////////////////////////////////////////////////////

// Valida una hora en formato HH:MM de 24 horas.
bool Validador::validarHora(std::string horaIngresada)
{
   // Horas: [01]\d cubre 00-19 y 2[0-3] cubre 20-23. Minutos: [0-5]\d cubre 00-59.
   // Acepta "07:10" y "23:59".
   // Rechaza "24:00", "07:60", "7:10" (falta el cero a la izquierda) y "ab:cd".
   static const std::regex patron(R"(^([01]\d|2[0-3]):[0-5]\d$)");
   return std::regex_match(horaIngresada, patron);
}

////////////////////////////////////////////////////////////////////////
// Name:       Validador::validarFecha(std::string fecha)
// Purpose:    Implementation of Validador::validarFecha()
// Parameters:
// - fecha
// Return:     bool
////////////////////////////////////////////////////////////////////////

// Valida una fecha en formato AAAA-MM-DD, incluidos los días de cada mes y el año bisiesto.
bool Validador::validarFecha(std::string fecha)
{
   // Paso 1 - Formato: 4 dígitos, guion, 2 dígitos, guion, 2 dígitos.
   // Acepta "2026-11-03". Rechaza "03-11-2026" (orden invertido), "2026/11/03",
   // "2026-1-3" (sin ceros a la izquierda) y el texto vacío.
   static const std::regex formato(R"(^\d{4}-\d{2}-\d{2}$)");
   if (!std::regex_match(fecha, formato))
   {
      return false;
   }

   // Paso 2 - Mes entre 01 y 12: 0[1-9] cubre 01-09 y 1[0-2] cubre 10-12.
   // Rechaza "00" y "13" en adelante.
   static const std::regex mes(R"(^\d{4}-(0[1-9]|1[0-2])-\d{2}$)");
   if (!std::regex_match(fecha, mes))
   {
      return false;
   }

   // Paso 3 - Día entre 01 y el máximo del mes. Tres alternativas según el mes:
   //  - meses de 31 días (01, 03, 05, 07, 08, 10, 12): días 01-31
   //  - meses de 30 días (04, 06, 09, 11): días 01-30
   //  - febrero (02): días 01-29 (si el 29 es válido se decide en el paso 4)
   // Rechaza el día 00, "2026-04-31", "2026-02-30" y "2026-01-32".
   static const std::regex diaDelMes(
      R"(^\d{4}-(?:)"
      R"((?:0[13578]|1[02])-(?:0[1-9]|[12]\d|3[01]))"
      R"(|(?:0[469]|11)-(?:0[1-9]|[12]\d|30))"
      R"(|02-(?:0[1-9]|1\d|2\d))"
      R"()$)");
   if (!std::regex_match(fecha, diaDelMes))
   {
      return false;
   }

   // Paso 4 - Año bisiesto. Solo importa si la fecha es un 29 de febrero:
   // cualquier otra fecha que llegó hasta aquí ya es válida.
   static const std::regex veintinueveFebrero(R"(^\d{4}-02-29$)");
   if (!std::regex_match(fecha, veintinueveFebrero))
   {
      return true;
   }

   // Una regex no puede calcular divisibilidad, por eso este paso es aritmético.
   // El paso 1 garantizó que los 4 primeros caracteres son dígitos, así que stoi no falla.
   int anio = std::stoi(fecha.substr(0, 4));

   // Un año es bisiesto si es múltiplo de 4, salvo los múltiplos de 100 que no lo sean de 400
   // (2024 sí, 2026 no, 2100 no, 2000 sí)
   bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);

   // El 29 de febrero solo existe en los años bisiestos
   return bisiesto;
}

////////////////////////////////////////////////////////////////////////
// Name:       Validador::validarAula(std::string aula)
// Purpose:    Implementation of Validador::validarAula()
// Parameters:
// - aula
// Return:     bool
////////////////////////////////////////////////////////////////////////

// Valida el nombre de un aula.
bool Validador::validarAula(std::string aula)
{
   // Uno o más bloques de letras o dígitos, unidos por un solo guion.
   // Acepta "A-101", "Lab3", "B2-LAB-1".
   // Rechaza vacío, espacios ("A 101"), símbolos ("!"), guion al inicio o al final y "--".
   static const std::regex patron(R"(^[A-Za-z0-9]+(?:-[A-Za-z0-9]+)*$)");
   return std::regex_match(aula, patron);
}

////////////////////////////////////////////////////////////////////////
// Name:       Validador::validarOpcionMenu(std::string opcion)
// Purpose:    Implementation of Validador::validarOpcionMenu()
// Parameters:
// - opcion
// Return:     bool
////////////////////////////////////////////////////////////////////////

// Valida que la opción escrita en un menú sea un número.
bool Validador::validarOpcionMenu(std::string opcion)
{
   // Dos alternativas, sin signo ni espacios:
   //  - "0" exactamente (la opción de volver o salir)
   //  - [1-9][0-9]?: un número de 1 o 2 dígitos cuyo primer dígito no es cero
   // Así cada opción tiene una sola forma de escribirse: "1" sí, pero "01", "001" y "00" no
   // (antes "01" se aceptaba como "1" y "001" no, lo que era incoherente).
   // El límite de dos dígitos evita textos enormes que desbordarían al convertirlos a int.
   // Acepta "0", "1", "7", "10", "99".
   // Rechaza "01", "00", "001" (ceros a la izquierda), "100" (3 dígitos), "a", "", "-1", " 1" y "1 ".
   static const std::regex patron(R"(^(0|[1-9][0-9]?)$)");
   return std::regex_match(opcion, patron);
}

////////////////////////////////////////////////////////////////////////
// Name:       Validador::esFinDeSemana(std::string fecha)
// Purpose:    Implementation of Validador::esFinDeSemana()
// Parameters:
// - fecha
// Return:     bool
////////////////////////////////////////////////////////////////////////

// Indica si una fecha válida cae en sábado o domingo.
// El día de la semana se calcula con aritmética (algoritmo de Sakamoto) porque una regex
// no puede hacerlo, y no se usa <ctime> porque mktime falla en Windows con fechas anteriores a 1970.
bool Validador::esFinDeSemana(std::string fecha)
{
   // Paso 1 - La fecha debe existir en el calendario; una fecha inválida no es fin de semana
   if (!validarFecha(fecha))
   {
      return false;
   }

   // Paso 2 - Separar año, mes y día. validarFecha garantizó el formato AAAA-MM-DD,
   // así que las posiciones son fijas y stoi no falla
   int anio = std::stoi(fecha.substr(0, 4));   // caracteres 0-3: año
   int mes = std::stoi(fecha.substr(5, 2));    // caracteres 5-6: mes (1 a 12)
   int dia = std::stoi(fecha.substr(8, 2));    // caracteres 8-9: día del mes

   // Paso 3 - Tabla de desplazamiento de cada mes (arreglo simple de C++, no std::array).
   // Cada valor corrige cuántos días de la semana se "corre" el primer día de ese mes
   int tabla[12] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

   // Paso 4 - Enero y febrero se cuentan como parte del año anterior,
   // así el día bisiesto (29 de febrero) queda al final del año y no altera los meses siguientes
   if (mes < 3)
   {
      anio = anio - 1;
   }

   // Paso 5 - Fórmula de Sakamoto:
   //  - anio: cada año común corre el día de la semana en 1
   //  - anio / 4 - anio / 100 + anio / 400: un día extra por cada año bisiesto transcurrido
   //  - tabla[mes - 1]: desplazamiento del mes (el arreglo empieza en 0)
   //  - dia: el día dentro del mes
   // El resto de dividir entre 7 da el día de la semana: 0 = domingo, 1 = lunes, ... , 6 = sábado
   int diaSemana = (anio + anio / 4 - anio / 100 + anio / 400 + tabla[mes - 1] + dia) % 7;

   // Paso 6 - Fin de semana: domingo (0) o sábado (6)
   return diaSemana == 0 || diaSemana == 6;
}
