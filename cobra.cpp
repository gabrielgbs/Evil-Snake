#include "cobra.h"
/*
Classe cobra é uma lista encadeada formada por pedacos(nós)
*/
Cobra::Cobra(){}  //construtor default


//cria cobra com 4 pedacos 
Cobra::Cobra(int i1,int j1,int i2, int j2){  //construtor
    tam = 4;     //tamanho inicial da cobra e 4 
    Pedaco* aux1 = new Pedaco(i1,j1+1);    //instancia pedacos
    Pedaco*aux2 = new Pedaco(i2,j2-1);     //instancia pedacos
    setHead(new Pedaco(i2,j2));           //instancia pedacos
    setTail(new Pedaco(i1,j1));           //instancia pedacos
    getTail()->setNext(aux1);       //configura ligacao entre os pedacos      
    aux1->setNext(aux2);            //configura ligacao entre os pedacos
    aux2->setNext(getHead());       //configura ligacao entre os pedacos
}

//cria cobra com 4 pedacos 
Cobra::Cobra(int i1, int j1){   //outro construtor
    tam = 4;        //tamanho inicial da cobra e 4 
    Pedaco* aux1 = new Pedaco(i1,j1+1);        //instancia pedacos
    Pedaco*aux2 = new Pedaco(i1,j1+2);         //instancia pedacos
    setHead(new Pedaco(i1,j1+3));              //instancia pedacos
    setTail(new Pedaco(i1,j1));                //instancia pedacos
    getTail()->setNext(aux1);      //configura ligacao entre os pedacos 
    aux1->setNext(aux2);           //configura ligacao entre os pedacos          
    aux2->setNext(getHead());      //configura ligacao entre os pedacos
} 

Pedaco* Cobra::getHead(){      //retorna o primeiro nó(pedaco) da lista encadeada que representa a "cobra"
    return head;    
}

Pedaco* Cobra::getTail(){      //retorna o ultimo nó(pedaco) da lista encadeada que representa a "cobra"
    return tail;    
}

void Cobra::setHead(Pedaco *h){  //configura o primeiro nó(pedaco) da lista encadeada que representa a "cobra"        
     head = h;
}

void Cobra::setTail(Pedaco *t){  //configura o ultimo nó(pedaco) da lista encadeada que representa a "cobra"
     tail = t;
}

int Cobra::getTam(){       //retorna tamanho da lista encadeada que representa a "cobra"
    return tam;
}

void Cobra::setTam(int n){    //configura tamanho da lista encadeada que representa a "cobra"    
    tam = n; 
}

void Cobra::addHead(Pedaco *p){      //adiciona um pedaco "cabeca" na cobra     
    getHead()->setNext(p);
    setHead(p);
    setTam(getTam()+1);              //incrementa tamanho
}

void Cobra::addHead(int i1, int j1){     //adiciona um pedaco "cabeca" na cobra      
    Pedaco *p = new Pedaco(i1, j1);      //instancia pedaco com linha i1 e coluna j1
    p->setNext(NULL);          //pedaco apontara para null
    getHead()->setNext(p);
    setHead(p);                //configura pedaco como sendo a "cabeca" da cobra
    setTam(getTam()+1);        //incrementa tamanho
}

Pedaco* Cobra::removeTail(){   //remove o ultimo pedaco da cobra
    Pedaco *aux = getTail();
    setTail(aux->getNext());
    aux->setNext(NULL);     
    setTam(getTam()-1);     //decrementa tamanho
    return aux;             //retorna pedaco removido
}
