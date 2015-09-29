#include "comida.h"

Comida::Comida(){}  //construtor default

Comida::Comida(int i1, int j1){ //construtor
    setLineCol(i1,j1);
}

void Comida::setLineCol(int i1, int j1){  //configura linha e coluna
    i = i1;
    j = j1;
}

int Comida::getCol(){    //retorna linha da comida   
    return j;        
}

int Comida::getLine(){   //retorna coluna da comida
    return i;        
}
