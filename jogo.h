#ifndef JOGO_H
#define JOGO_H

#include "cobra.h"
#include "comida.h"

#define NORTE 1
#define LESTE 2
#define SUL 3
#define OESTE 4

#define VELOC_BAIXA 7
#define VELOC_NORMAL 4
#define VELOC_ALTA 1

#define NUM_COL 40
#define NUM_LINE 30

class Jogo{
      private:
       int velocidade;
       int pontuacao;
       int direcao;
       int numQuadradoHorizontal;
       int numQuadradoVertical;
       Cobra* cobra;
       Comida* comida;       
      public:
       Jogo();
       void setNumQuadrado(int ,int);
       int getNumQuadradoVertical();
       int getNumQuadradoHorizontal();
       void setPontuacao(int);
       int getPontuacao();
       void setDirecao(int);
       int getDirecao();            
       void setVelocidade(int);
       int getVelocidade();
       Cobra* getCobra();
       void setCobra(Cobra*);
       void setCobra(int,int,int,int);
       Comida* getComida();
       void setComida(Comida*);
       void setComida(int,int);
       void jogoIdle();
       void criaComida(); 
       bool comeu();
       bool colidiuParede();
       bool colidiuCobra();     
};

#endif
