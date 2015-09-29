#ifndef PEDACO_H
#define PEDACO_H
#include <stdlib.h>

class Pedaco{
      private:
       int i;
       int j;
       Pedaco* next; 
      public:
       Pedaco();
       Pedaco(int,int);
       void setLineCol(int,int);
       int getLine();
       int getCol();
       Pedaco* getNext();
       void setNext(Pedaco*);            
};

#endif 
