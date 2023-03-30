// pch.h: este es un archivo de encabezado precompilado.
// Los archivos que se muestran a continuación se compilan solo una vez, lo que mejora el rendimiento de la compilación en futuras compilaciones.
// Esto también afecta al rendimiento de IntelliSense, incluida la integridad del código y muchas funciones de exploración del código.
// Sin embargo, los archivos que se muestran aquí se vuelven TODOS a compilar si alguno de ellos se actualiza entre compilaciones.
// No agregue aquí los archivos que se vayan a actualizar con frecuencia, ya que esto invalida la ventaja de rendimiento.

#pragma once

#ifndef PCH_H
#define PCH_H

// Http client
#include <httplib/httplib.h>

#include <Windows.h>
#include <stdio.h>
#include <iostream>

#include <plusaes/plusaes.hpp>
#include <str_obfuscator/obfuscator.hpp>

using namespace std;

#endif //PCH_H
