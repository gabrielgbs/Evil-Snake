#ifndef COBRA_H
#define COBRA_H
#include "pedaco.h"

class Cobra{
      private:
       int tam;
       Pedaco* head;
       Pedaco* tail;  
      public:
       Cobra();
       Cobra(int,int,int,int);
       Cobra(int,int);
       Pedaco* getHead();          
       Pedaco* getTail();
       void setHead(Pedaco*);  
       void setTail(Pedaco*);
       int getTam();
       void setTam(int);
       void addHead(int,int);
       void addHead(Pedaco*);
       Pedaco* removeTail();
};

#endif 
