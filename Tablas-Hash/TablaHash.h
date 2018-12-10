#ifndef TABLAHASH_H
#define TABLAHASH_H

#include "NodoHash.h"


template<class CLAVE, class VALOR>
class TablaHash{
  ///Tabla.
  NodoHash<CLAVE,VALOR> **tabla;
  ///Capacidad.
  int cap;
  ///Dimensi&oacuten.
  int dim;
  ///Se utiliza *borrado para evitar errores cuando se busca un valor y ya a sido borrado uno.
  NodoHash<CLAVE,VALOR> *borrado ;
  const static float densidad = 0.7;

  int claveHash(CLAVE, int);
  void insertar(CLAVE, VALOR, NodoHash<CLAVE,VALOR>**, int);
  /**
  \brief M&eacute;etodo para redimensionar la tabla si la densidad es mayor.
  \return true si se redimensiona
  \return false si no es necesario redimensionar.
  */
  bool redimensionMayor();
  /**
  \brief M&eacute;todo para redimensionar la tabla si la densidad es menor.
  \return true si se redimensiona.
  \return false si no es necesario redimensionar.
  */
  bool redimensionMenor();

  public:
  /**
  \brief Constructor inicializa capacidad en 16.
  */
  TablaHash(int _cap = 16);
  /**
  \brief Constructor de copias.
  \param capacidad
  \param TablaHash<CLAVE,VALOR>&
  */
  TablaHash(int, const TablaHash<CLAVE, VALOR>&);
  /**
  \brief Destructor
  \brief Se utiliza el m&eacute;todo vaciar.
  */
  ~TablaHash();
  /**
  \brief M&eacute;todo para vaciar la tabla.
  */
  void Vaciar();
  /**
  \brief M&eacute;todo para borrar un elemento de la tabla.
  \param CLAVE del elemento que se va a borrar.
  \return VALOR del elemento borrado.
  */
  VALOR borrar(CLAVE);
  /**
  \brief M&eacute;todo para obtener valor.
  \param CLAVE necesaria para obtener el valor.
  \return VALOR obtenido de la clave.
  */
  VALOR obtenVal(CLAVE);
  /**
  \brief M&eacute;todo para insertar un elemento a la tabla.
  \param Clave
  \param VALOR
  */
  void insertar(CLAVE, VALOR);
  /**
  \brief M&eacute;todo para obtener la dimensi&oacute;n.
  \return dim.
  */
  int obtenDim() {return dim; }
  /**
  \brief M&eacute;todo para obtener la capacidad.
  \return cap.
  */
  int obtenCap() {return cap; }
  /**
  \brief M&eacute;todo para checar si la tabla est&aacute; vac&iacute;a.
  \return bool true o false.
  */
  bool esVacio() {return(!dim? true: false); }
  /**
  \brief M&eacute;todo para imprimir la tabla.
  */
  void imprimir();
  /**
  \brief Sobrecarga del operador =.
  \param const TablaHash<CLAVE,VALOR> &copia.
  \return tabla.
  */
  TablaHash<CLAVE, VALOR>& operator=(const TablaHash<CLAVE, VALOR> &copia);

};
/***************************************************************************************************************************************************/
template<class CLAVE, class VALOR>
TablaHash<CLAVE,VALOR>::TablaHash(int _cap): cap(_cap), dim(0)
{
  tabla = new NodoHash<CLAVE,VALOR> *[cap];

  for(int i=0 ; i < cap ; i++) {
    tabla[i] = NULL;
  }

  borrado = new NodoHash<CLAVE,VALOR>;
}
/***************************************************************************************************************************************************/
template<class CLAVE, class VALOR>
TablaHash<CLAVE,VALOR>::TablaHash(int _cap, const TablaHash& hash): cap(_cap), dim(0)  {
  NodoHash<CLAVE, VALOR> *nodo = new NodoHash<CLAVE,VALOR>(0, 0);

  tabla = new NodoHash<CLAVE,VALOR> *[cap];
  for(int i=0 ; i < cap ; i++) {
    tabla[i] = NULL;
  }
  borrado = new NodoHash<CLAVE,VALOR>(-1, -1);

  for(int i = 0; i < cap; i++) {
    nodo = hash.tabla[i];
    if(tabla[i]) {
      insertar(nodo -> clv, nodo -> val);
    }
  }
}
/***************************************************************************************************************************************************/
template<class CLAVE, class VALOR>
TablaHash<CLAVE,VALOR>::~TablaHash(){
    Vaciar();

}
/***************************************************************************************************************************************************/
template<class CLAVE, class VALOR>
void TablaHash<CLAVE,VALOR>::Vaciar(){
    NodoHash<CLAVE,VALOR> *temp;
    for (int i = 0; i < cap; i++) {
        temp = tabla[i];
        if(temp) {
            delete(temp);
        }
    }
    delete(tabla);
    cap = 16;
    dim = 0;
}
/***************************************************************************************************************************************************/
template<class CLAVE, class VALOR>
TablaHash<CLAVE, VALOR>& TablaHash<CLAVE, VALOR>::operator=(const TablaHash<CLAVE, VALOR> &copia) {
  cap = copia.cap;
  dim = copia.dim;
  NodoHash<CLAVE,VALOR> *nodo = new NodoHash<CLAVE,VALOR>(0,0);


  for(int i=0 ; i < cap * 2; i++) {
    this->tabla[i] = NULL;
  }

  int indiceHash;
  for(int i = 0; i < cap; i++) {
    nodo = copia[i];
    if(nodo && nodo -> val != -1)
        this->tabla[i] = nodo;

  }

    return this;
}
/***************************************************************************************************************************************************/
template<class CLAVE, class VALOR>
int TablaHash<CLAVE,VALOR>::claveHash(CLAVE _clv, int _cap) {
  return (((int)_clv) % _cap);
}
/***************************************************************************************************************************************************/
template<class CLAVE, class VALOR>
void TablaHash<CLAVE,VALOR>::insertar(CLAVE _clv, VALOR _val) {
  NodoHash<CLAVE,VALOR> *temp = new NodoHash<CLAVE,VALOR>(_clv, _val);

  int indiceHash = claveHash(_clv, cap);

  cout << "Agregando a: " << _clv << " " << "valor: " << _val << " en " << indiceHash << endl;

  while(tabla[indiceHash] != NULL && tabla[indiceHash] -> clv != _clv && tabla[indiceHash] != borrado) {
    indiceHash++;
    indiceHash %= cap;
    cout << "indice " << indiceHash <<  endl;
  }

  if(!tabla[indiceHash] || tabla[indiceHash] == borrado) {
    dim++;
  }

  tabla[indiceHash] = temp;
  redimensionMayor();

}
/***************************************************************************************************************************************************/
template<class CLAVE, class VALOR>
bool TablaHash<CLAVE,VALOR>::redimensionMayor() {
  NodoHash<CLAVE, VALOR> *nodo ;
  int indiceHash;

  if((float)dim/(float)cap < densidad) {
    return false;
  }

  cout << "\nRedimensi\242n necesaria.\n" ;

  NodoHash<CLAVE,VALOR> **reemplazo = new NodoHash<CLAVE,VALOR> *[cap * 2];
  for(int i=0 ; i < cap * 2; i++) {
    reemplazo[i] = NULL;
  }


  for(int i = 0; i < cap; i++) {
    nodo = tabla[i];
    if(nodo && nodo != borrado) {
      indiceHash = claveHash(nodo -> clv, cap * 2);
      while(reemplazo[indiceHash] != NULL && reemplazo[indiceHash]!= borrado) {
        indiceHash++;
        indiceHash %= (cap * 2);
      }
      reemplazo[indiceHash] = nodo;
    }
  }

  cap *= 2;
  delete(this -> tabla);
  this -> tabla = reemplazo;

  return true;
}
/***************************************************************************************************************************************************/
template<class CLAVE, class VALOR>
bool TablaHash<CLAVE,VALOR>::redimensionMenor() {
  NodoHash<CLAVE, VALOR> *nodo ;
  int indiceHash;

  if((float)dim/(float)cap > 1.0 - densidad) {
    return false;
  }

  cout << "\nRedimensi\242n necesaria.\n" ;

  NodoHash<CLAVE,VALOR> **reemplazo = new NodoHash<CLAVE,VALOR> *[cap / 2];
  for(int i = 0 ; i < cap / 2; i++) {
    reemplazo[i] = NULL;
  }


  for(int i = 0; i < cap; i++) {
    nodo = tabla[i];
    if(nodo && nodo != borrado) {
      indiceHash = claveHash(nodo -> clv, cap / 2);
      while(reemplazo[indiceHash] != NULL && reemplazo[indiceHash] != borrado) {
        indiceHash++;
        indiceHash %= (cap / 2);
      }
      reemplazo[indiceHash] = nodo;
    }
  }

  cap /= 2;
  delete(this -> tabla);
  this -> tabla = reemplazo;


  return true;
}

/***************************************************************************************************************************************************/
template<class CLAVE, class VALOR>
VALOR TablaHash<CLAVE,VALOR>::borrar(CLAVE _clv) {
  int indiceHash = claveHash(_clv, cap);

  while(tabla[indiceHash] != NULL) {
    if(tabla[indiceHash] -> clv == _clv) {
      NodoHash<CLAVE,VALOR> *temp = tabla[indiceHash];
      tabla[indiceHash] = borrado;
      dim--;
      return temp -> val;
    }
    indiceHash *= (cap - 1);
    indiceHash %= cap;
    if(cap >= 16) redimensionMenor();
  }

  return((VALOR)0);
}
/***************************************************************************************************************************************************/
template<class CLAVE, class VALOR>
VALOR TablaHash<CLAVE,VALOR>::obtenVal(CLAVE _clv) {
  int indiceHash = claveHash(_clv,cap);

  while(tabla[indiceHash] != NULL) {
    if(tabla[indiceHash] -> clv == _clv) {
      return tabla[indiceHash] -> val;
    }
    indiceHash++;
    indiceHash %= cap;
  }

  return((VALOR)0);
}
/***************************************************************************************************************************************************/
template<class CLAVE, class VALOR>
void TablaHash<CLAVE,VALOR>::imprimir() {
  for(int i = 0; i < cap; i++) {
    if(tabla[i] != NULL && tabla[i] -> clv != -1)
    cout << "\n\326ndice: " << i << " clave = " << tabla[i] -> clv
    <<" valor = "<< tabla[i] -> val ;
  }
  cout << endl;
}

#endif
