#include <cstdlib>
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

#include "TablaHash.h"
/**
    \brief Tabla Hash
    \date 05/30/2018
    \author Mario Castro,Jos&eacute; Gonzales,Ariana S&aacute;nchez
    \version 1.0
*/
int main()
{
    TablaHash<int, string> hash = TablaHash<int, string>();

    cout << "----Programa que implementa una tabla hash con direccionamiento abierto----" << endl;

    string cadenas[] = {"hola", "adios", "que tranza", "con la panza", "que short"};

    int tam = sizeof cadenas/sizeof (string);

    for(int i = 0; i < 15; ++i) {
    hash.insertar((i + 1) * (rand() % 100 + 1), cadenas[rand() % tam]);

    }

    hash.insertar(5, "lo que sea");


    cout << "Tabla hash: " << endl;
    hash.imprimir();

    int clave;
    for(int i = 0; i < 15; ++i) {
           clave = (i + 1) * (rand() % 100 + 1);
           cout << "Borrando a: " << clave << " resultado: "<< hash.borrar(clave) << endl;
    }

    //cout << "Tabla hash: " << endl;
    //hash.imprimir();

    //hash.Vaciar();

    cout << "\n----Versi\242n 1.0----" << endl;

    return 0;
}
