#include "pedaco.h"
/*
Cada pedaco e um nó da lista encadeada que representa a cobra inteira 
Pedaco tem uma linha e uma coluna que serve para desenha-lo na tela 
*/

Pedaco::Pedaco(){} //construtor default

Pedaco::Pedaco(int i1, int j1){ //construtor
    setLineCol(i1,j1);  
    next = NULL;                   
}

void Pedaco::setLineCol(int i1, int j1){  //configura linha e coluna do pedaco
    i = i1;
    j = j1;
    next = NULL;   //configura ponteiro para o proximo pedaco como null                    
}

//metodos get da classe pedaco
int Pedaco::getCol(){  
    return j;         
}

int Pedaco::getLine(){
    return i;        
}

Pedaco* Pedaco::getNext(){
    return next;    
}

void Pedaco::setNext(Pedaco* p){  //configura ponteiro para o proximo pedaco 
    next = p; 
}
