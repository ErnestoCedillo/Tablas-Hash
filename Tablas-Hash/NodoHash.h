#ifndef NODOHASH_H
#define NODOHASH_H

template<typename CLAVE, typename VALOR>
class NodoHash {
public:
  VALOR val;
  CLAVE clv;
  NodoHash(){}

  NodoHash(CLAVE _clv, VALOR _val): val(_val), clv(_clv) { }
};

#endif
