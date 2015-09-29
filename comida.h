#ifndef COMIDA_H
#define COMIDA_H

class Comida{
      private:
       int i;
       int j;        
      public:
       Comida();
       Comida(int,int);
       void setLineCol(int,int);
       int getLine();
       int getCol();       
};

#endif
