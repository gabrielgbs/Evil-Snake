#include "jogo.h"
#include <time.h>

Jogo::Jogo(){    //construtor
     setDirecao(LESTE);      //direcao inicial
     setVelocidade(VELOC_NORMAL);  //velocidade inicial
     setPontuacao(0);              //pontuacao inicial
     setCobra(new Cobra(1,1));     
     setNumQuadrado(30, 40);       //numero de quadrados da tela
     criaComida();                //cria comida 
}

//configura o numero de "quadrados" na horizontal e na vertical
void Jogo::setNumQuadrado(int h,int v){
    if(h>5 && v>5){           
        numQuadradoHorizontal = h;   
        numQuadradoVertical = v;
    } else {
      numQuadradoVertical = 5;    
      numQuadradoHorizontal = 5;
    }  
}

//retorna o numero de qudrados na vertical
int Jogo::getNumQuadradoVertical(){
    return numQuadradoVertical;        
}

//retorna o numero de qudrados na horizontal
int Jogo::getNumQuadradoHorizontal(){
    return numQuadradoHorizontal;        
}

//configura a pontuacao
void Jogo::setPontuacao(int p){
    if(p>0){
        pontuacao = p;
    } else pontuacao = 0;    
}

//retorna a pontuacao
int Jogo::getPontuacao(){
    return pontuacao;        
}

//configura a direcao
void Jogo::setDirecao(int d){
    if(d>0 && d<5){
        direcao = d;        
    } else direcao = LESTE;   
}

//retorna a direcao atual
int Jogo::getDirecao(){
    return direcao;        
}

//configura a velocidade
void Jogo::setVelocidade(int v){
    if(v>0 && v<10){
        velocidade = v;        
    } else velocidade = VELOC_NORMAL;    
}

//retorna a velocidade
int Jogo::getVelocidade(){
    return velocidade;        
}

//retorna um ponteiro para um objeto "comida"
Comida* Jogo::getComida(){
     return comida;   
}

//configura a "comida"
void Jogo::setComida(Comida *c){
     comida = c;
}

void Jogo::setComida(int i, int j){
     Comida *c = new Comida(i, j);
     comida = c;
}

//retorna um ponteiro para um objeto "cobra"
Cobra* Jogo::getCobra(){
     return cobra;  
}

//configura a "cobra" do jogo atual
void Jogo::setCobra(Cobra* c){
     cobra = c;
}

void Jogo::setCobra(int i1,int j1,int i2,int j2){
     Cobra *c = new Cobra(i1, j1, i2, j2);
     cobra = c;
}

/*
Esta funcao faz a cobra se movimentar numa mesma
direcao, adicionando um "pedaco" na frente e
removendo um "pedaco" de tras de acordo com a direcao atual
*/
void Jogo::jogoIdle(){
     switch(getDirecao()){                
         case NORTE: 
              getCobra()->addHead(getCobra()->getHead()->getLine() + 1,
                             getCobra()->getHead()->getCol());                                    
              break;               
         case LESTE:
              getCobra()->addHead(getCobra()->getHead()->getLine(),
                             getCobra()->getHead()->getCol() + 1);                                    
              break;
         case SUL:
              getCobra()->addHead(getCobra()->getHead()->getLine() - 1,
                             getCobra()->getHead()->getCol());                                    
              break;
         case OESTE: 
              getCobra()->addHead(getCobra()->getHead()->getLine(),
                             getCobra()->getHead()->getCol() - 1);                                    
              break;
     }
     getCobra()->removeTail(); 
}

/*
Esta funcao cria uma comida de coordenadas aleatorias
se as coordenadas sao as mesmas de algum pedaco da cobra
escolhe-se outras coordenadas para a "comida" aleatoriamente
*/
void Jogo::criaComida(){
     srand(time(NULL));
     bool b = false;
     int x, y, i;
     Pedaco* aux = this->getCobra()->getTail();
     while(b!=true){
       x = rand()%getNumQuadradoHorizontal();
       y = rand()%getNumQuadradoVertical();
       for(i = 0; i<getCobra()->getTam(); i++){
          if((x==aux->getLine() || y==aux->getCol()) && aux->getNext()!=NULL){
              aux = aux->getNext();                 
          }
          else b = true; 
       } 
     }
     setComida(new Comida(x, y));  
}

/*
Esta funcao verifica se a cobra comeu alguma "comida"
retornando true ou false, se for true aumentamos
a pontuacao do jogo utilizando a funcao para 
configurar a pontuacao e tambem adicionamos um "pedaco" na cobra
*/
bool Jogo::comeu(){
     int i1 = getCobra()->getHead()->getLine();
     int j1 = getCobra()->getHead()->getCol();
     int i2 = getCobra()->getTail()->getLine();
     int j2 = getCobra()->getTail()->getCol();
     if((i1==getComida()->getLine() && j1==getComida()->getCol()) || 
        (i2==getComida()->getLine() && j2==getComida()->getCol()))
     {
          getCobra()->addHead(i1,j1); 
          setPontuacao(getPontuacao()+(8-getVelocidade()));
          return true;
     }else 
          return false;                       
}

/*
Esta funcao verifica se a cobra colidiu com a parede
de acordo com as coordenadas da "cabeca" da cobra
*/
bool Jogo::colidiuParede(){
     int i = getCobra()->getHead()->getLine();
     int j = getCobra()->getHead()->getCol();
     if(i<0 || i>30 || j<0 || j>=40)
         return true;
     else return false;    
}

/*
Esta funcao verifica se a cobra colidiu com ela 
mesma, testando as coordenadas da cabeca com todos
os pedacos da cobra(exceto os pedacos proximos
da cabeca) 
*/
bool Jogo::colidiuCobra(){
     int k, t = getCobra()->getTam() - 2;                          
     int i = getCobra()->getHead()->getLine();
     int j = getCobra()->getHead()->getCol();
     Pedaco* aux = this->getCobra()->getTail();
     for(k = 0; k<t; k++){
          if(i==aux->getLine() && j==aux->getCol())
              return true;
          else aux = aux->getNext();                      
     }
     return false;    
}
