#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <GL/glut.h>
#include "jogo.h"
          //definicao de cores
#define RED 1.0,0.0,0.0
#define GREEN 0.0,1.0,0.0
#define BLACK 0.0,0.0,0.0
#define WHITE 1.0,1.0,1.0
#define GRAY 0.8,0.8,0.8
#define BLUE 0.0,0.0,1.0
#define GREENBLACK 0.0,0.4,0.0

void desenhaApresentacao();

Jogo* jogo;

bool jogoComecou;
int largura;            //largura da tela
int altura;             //altura da tela
bool podeApagar;        //variavel que indica se a "comida" pode ser redesenhada
bool jogoParado;        //variavel que indica o estado do jogo(parado ou nao)
bool cobraColidiu;      //variavel que indica se a cobra colidiu com ela ou com a parede
int tamQuadrado;        //tamanho dos "quadrados" que formam a parede, a cobra,...

/*
funcao que mapeia cada linha e coluna dos pedacos(quadrado)
para coordenadas na tela, na verdade sao coordenadas do
canto superior direito dos quadrados
*/
int funcao(int i, int j, int k){
     if(k==0)
         return tamQuadrado*(j+2);
     else
         return tamQuadrado*(i+2);
}

void delay(int n){             //funcao atraso
    int _delay = n*30;
    #ifdef _WIN32
    Sleep(_delay);
    #else
    usleep(_delay*1000);
    #endif
}

void inicializaJogo(bool f){
    jogo = new Jogo();        //instancia variavel jogo
    podeApagar = false;
    jogoParado = f;           //inicia jogo parado ou nao
    cobraColidiu = false;
}

/*
funcao que avalia se a cobra comeu alguma comida
ou colidiu com a parede ou com ela mesma
*/
void avaliaJogo(){
     bool b = jogo->comeu();    //se a cobra comeu a comida
     if(b==true)
        podeApagar=true;        //apaga comida e depois redesenha
     if(jogo->colidiuParede()==true || jogo->colidiuCobra()==true){
        cobraColidiu = true;      //variavel indicara que a cobra colidiu
        jogoParado = true;        //jogo sera parado
     }
}


/*
funcao que inicializa a glut e outros parametros
*/
void init(int argc, char **argv){
    tamQuadrado = 10;
    largura = 420;
    altura = 350;
    jogoComecou = false;
    inicializaJogo(true);        //incializa jogo parado
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(largura,altura);
	glutInitWindowPosition(50,50);
	glutCreateWindow("SERPENTE MALDITA - Versao 0.8");
    glClearColor(WHITE,0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,largura,0,altura,-1,1);
}

/*
funcao que desenha a pontuacao
*/
void desenhaPontuacao(){
     int p = jogo->getPontuacao();
     int d1 = p/1000;
     int d2 = (p%1000)/100;
     int d3 = (p%100)/10;
     int d4 = p%10;
     int shift = largura - 5*tamQuadrado, s = 2*tamQuadrado;
     glColor3f(BLACK);
     glRasterPos2i(shift, altura - s);
     glutBitmapCharacter(GLUT_BITMAP_9_BY_15, d1+'0');
     shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, d1+'0');
     glRasterPos2i(shift, altura-s);
     glutBitmapCharacter(GLUT_BITMAP_9_BY_15, d2+'0');
     shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, d2+'0');
     glRasterPos2i(shift, altura-s);
     glutBitmapCharacter(GLUT_BITMAP_9_BY_15, d3+'0');
     shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, d3+'0');
     glRasterPos2i(shift, altura-s);
     glutBitmapCharacter(GLUT_BITMAP_9_BY_15, d4+'0');
     shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, d4+'0');
}

/*
funcao que desenha "game over" no centro da tela
*/
void desenhaGameOver(){
     int shift = 160;
     int s = 180;
     glColor3f(BLUE);
     glRasterPos2i(shift, altura - s);
     glutBitmapCharacter( GLUT_BITMAP_9_BY_15,'G');
     shift += glutBitmapWidth( GLUT_BITMAP_9_BY_15,'G');
     glRasterPos2i(shift, altura-s);
     glutBitmapCharacter(GLUT_BITMAP_9_BY_15,'A');
     shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15,'A');
     glRasterPos2i(shift, altura-s);
     glutBitmapCharacter(GLUT_BITMAP_9_BY_15,'M');
     shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15,'M');
     glRasterPos2i(shift, altura-s);
     glutBitmapCharacter(GLUT_BITMAP_9_BY_15,'E');
     shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15,'E');
     shift +=5;
     glRasterPos2i(shift, altura-s);
     glutBitmapCharacter(GLUT_BITMAP_9_BY_15,'O');
     shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15,'O');
     glRasterPos2i(shift, altura-s);
     glutBitmapCharacter(GLUT_BITMAP_9_BY_15,'V');
     shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15,'V');
     glRasterPos2i(shift, altura-s);
     glutBitmapCharacter(GLUT_BITMAP_9_BY_15,'E');
     shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15,'E');
     glRasterPos2i(shift, altura-s);
     glutBitmapCharacter(GLUT_BITMAP_9_BY_15,'R');
     shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15,'R');
}

/*
funcao que desenha as paredes
*/
void desenhaCenario(){
    int tq = tamQuadrado;
    glBegin(GL_QUADS);
        glColor3f(GRAY);
        glVertex2i(0,altura);
        glVertex2i(largura,altura);
        glVertex2i(largura,altura - 2*tq);
        glVertex2i(0,altura - 2*tq);
        glColor3f(BLUE);
        glVertex2i(0,altura-2*tq);
        glVertex2i(largura,altura-2*tq);
        glVertex2i(largura,altura - 3*tq);
        glVertex2i(0,altura - 3*tq);
        glVertex2i(0,0);
        glVertex2i(tq,0);
        glVertex2i(tq,altura - 3*tq);
        glVertex2i(0,altura - 3*tq);
        glVertex2i(largura-tq,0);
        glVertex2i(largura,0);
        glVertex2i(largura,altura - 3*tq);
        glVertex2i(largura-tq,altura - 3*tq);
        glVertex2i(tq,0);
        glVertex2i(largura-tq,0);
        glVertex2i(largura-tq,tq);
        glVertex2i(tq,tq);
    glEnd();
}

/*
funcao que desenha a cobra
*/
void desenhaCobra(){
     int i,x,y;     //variaveis auxiliares
     int tq = tamQuadrado;
     Pedaco *aux = jogo->getCobra()->getTail();     //ponteiro para a "cauda" da cobra
     for(i = 0; i<jogo->getCobra()->getTam(); i++){
          x = funcao(aux->getLine(),aux->getCol(),0); //coordenada x
          y = funcao(aux->getLine(),aux->getCol(),1); //coordenada y
          if(cobraColidiu==true)                     //se colidiu, a cor da cobra sera vermelha
              glColor3f(RED);
          else                                       //senao a cor sera preta
              glColor3f(GREENBLACK);
          glBegin(GL_QUADS);                        //desenha cada quadrado(pedaco) da cobra
             glVertex2i(x, y);         //a partir das coordenadas do canto superior direito
             glVertex2i(x, y-tq);      //descobrimos as coordenadas dos outros vertices(cantos)
             glVertex2i(x-tq, y-tq);   //utilizando tambem o tamanho dos quadrados
             glVertex2i(x-tq, y);
          glEnd();
          aux = aux->getNext();     //faz ponteiro apontar para o proximo pedaco
     }
}

void desenhaComida(){
    int tq = tamQuadrado;
    int i = jogo->getComida()->getLine(); //linha da "comida"
    int j = jogo->getComida()->getCol();  //coluna da "comida"
    int x = funcao(i, j, 0);       //coordenada x do canto superior direito da "comida"
    int y = funcao(i, j, 1);       //coordenada y do canto superior direito da "comida"
    if(podeApagar==false){        // se nao pode apagar a comida
      glBegin(GL_QUADS);          //desenha a comida com a cor preta
        glColor3f(GREENBLACK);
        glVertex2i(x, y);
        glVertex2i(x, y-tq);
        glVertex2i(x-tq, y-tq);
        glVertex2i(x-tq, y);
      glEnd();
    }else{                         //senao apaga comida
      jogo->criaComida();          //cria nova comida
      podeApagar = false;          //nao pode apagar comida
    }
    if(cobraColidiu==true){        // se cobra colidiu
      glBegin(GL_QUADS);           //desenha a comida com a cor vermelha
        glColor3f(RED);
        glVertex2i(x, y);
        glVertex2i(x, y-tq);
        glVertex2i(x-tq, y-tq);
        glVertex2i(x-tq, y);
      glEnd();
    }
}
/*
void reshape(int w, int h){
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0 ,(GLfloat)w  ,0.0 ,(GLfloat)h);
    largura = w;
    altura = h;
    if(altura<largura){
        tamQuadrado = (int)altura/35;
        if(tamQuadrado<10) tamQuadrado = 10;
    }else{
        tamQuadrado = (int)largura/35;
	    if(tamQuadrado<10) tamQuadrado = 10;
    }
    glMatrixMode(GL_MODELVIEW);
}*/

/*
funcao para eventos no teclado,
serve para controlar a direcao da cobra,
dependendo da direcao atual certas teclas nao
poderao ou nao precisarao ser usadas,
exemplo: se a direcao e norte, nao podemos
teclar a seta p/ baixo.
A cobra se movimenta adicionando pedacos na frente(cabeca)
e retirando pedacos de tras(cauda)
*/
void tecladoDirecao(int tecla, int x, int y){
     if(jogoParado==false){  //se jogo nao estiver parado
       switch(tecla){
          case GLUT_KEY_UP:
               if(jogo->getDirecao()==LESTE || jogo->getDirecao()==OESTE){
                    jogo->getCobra()->removeTail();
                    glutPostRedisplay();    //redesenha
                    jogo->setDirecao(NORTE);
                    jogo->getCobra()->addHead(
                      jogo->getCobra()->getHead()->getLine() + 1,
                      jogo->getCobra()->getHead()->getCol());
               }
               glutPostRedisplay();  //redesenha
               break;
          case GLUT_KEY_DOWN:
               if(jogo->getDirecao()==LESTE || jogo->getDirecao()==OESTE){
                    jogo->getCobra()->removeTail();
                    glutPostRedisplay();        //redesenha
                    jogo->setDirecao(SUL);
                    jogo->getCobra()->addHead(
                      jogo->getCobra()->getHead()->getLine() - 1,
                      jogo->getCobra()->getHead()->getCol());
               }
               glutPostRedisplay();   //redesenha
               break;
          case GLUT_KEY_LEFT:
               if(jogo->getDirecao()==NORTE || jogo->getDirecao()==SUL){
                    jogo->getCobra()->removeTail();
                    glutPostRedisplay();  //redesenha
                    jogo->setDirecao(OESTE);
                    jogo->getCobra()->addHead(
                      jogo->getCobra()->getHead()->getLine(),
                      jogo->getCobra()->getHead()->getCol() - 1);
               }
               glutPostRedisplay(); //redesenha
               break;
          case GLUT_KEY_RIGHT:
               if(jogo->getDirecao()==NORTE || jogo->getDirecao()==SUL){
                    jogo->getCobra()->removeTail();
                    glutPostRedisplay();  //redesenha
                    jogo->setDirecao(LESTE);
                    jogo->getCobra()->addHead(
                      jogo->getCobra()->getHead()->getLine(),
                      jogo->getCobra()->getHead()->getCol() + 1);
               }
               glutPostRedisplay(); //redesenha
               break;
       }
     }
}

/*
desenha na tela
*/
void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    if(jogoComecou==false)
        desenhaApresentacao();
    else{
         desenhaCenario();
         desenhaCobra();
         desenhaComida();
         desenhaPontuacao();
         if(cobraColidiu==true)
             desenhaGameOver();
    }
    glutSwapBuffers();
}

/*
funcao para eventos do teclado
*/
void keyboard(unsigned char tecla, int x, int y){
     switch(tecla){
         case 'q':
         case 'Q':
              exit(0);
              break;
         case 'n':
         case 'N':
              jogoComecou = true;
              inicializaJogo(false);
              break;
         case 'p':
         case 'P':
              if(cobraColidiu==false)
                  jogoParado = !jogoParado;
              break;
     }
}

/*
funcao quando nenhum evento esta ocorrendo
*/
void idle(){
     if(jogoParado==false){ //se a cobra estiver se movendo
        jogo->jogoIdle();   //faz a cobra se movimentar na mesma direcao
        avaliaJogo();       //chama funcao para avaliar jogo
        delay(jogo->getVelocidade());  //atraso para poder redesenhar cobra
        glutPostRedisplay();  //redesenhar
     }
}

/*
funcao que controla menu principal
*/
void opcoesMenu(int id){
     if(id==1) {
          jogoComecou = true;;
          inicializaJogo(false);
     }
     else if(id==2 && jogoParado==true && cobraColidiu==false)
              jogoParado = false;
     else if(id==3) exit(0);
}

/*
funcao que controla submenu "nivel"
*/
void nivelMenu(int id){
     if(jogo->getCobra()->getTam() < 5){
        if(id==1) jogo->setVelocidade(VELOC_BAIXA);
        else if(id==2) jogo->setVelocidade(VELOC_NORMAL);
        else if(id==3) jogo->setVelocidade(VELOC_ALTA);
     }
}

int main(int argc, char** argv){
     init(argc, argv);
     int nmenu;
     nmenu = glutCreateMenu(nivelMenu);
     glutAddMenuEntry("Facil",1);
     glutAddMenuEntry("Medio",2);
     glutAddMenuEntry("Dificil",3);
     glutCreateMenu(opcoesMenu);
     glutAddMenuEntry("Novo jogo",1);
     glutAddMenuEntry("Continuar jogo",2);
     glutAddSubMenu("Nivel",nmenu);
     glutAddMenuEntry("Sair",3);
     glutAttachMenu(GLUT_LEFT_BUTTON);
     glutDisplayFunc(display);
     glutKeyboardFunc(keyboard);
     glutSpecialFunc(tecladoDirecao);
     glutIdleFunc(idle);
   //  glutReshapeFunc(reshape);
     glutMainLoop();
     return 0;
}

void desenhaApresentacao(){
    glClear(GL_COLOR_BUFFER_BIT);
    //Desenhar plano de fundo

    glBegin(GL_POLYGON);
         glColor3f(1.0f, 0.0f, 0.0f);
         glVertex2i(0,0);
         glColor3f(1.0f, 1.0f, 0.0f);
         glVertex2i(largura,0);
         glColor3f(1.0f, 0.5f, 0.0f);
         glVertex2i(largura,altura);
         glColor3f(0.0f, 0.0f, 0.0f);
         glVertex2i(0,altura);
    glEnd();


    //Desenhar corpo da serpente
    glBegin(GL_TRIANGLE_STRIP);
         glColor3f(0.0f, 0.275f, 0.0f);
    	    glVertex2i(0,166);
    	    glVertex2i(0,125);
    	    glVertex2i(16,166);
    	    glVertex2i(17,123);
    	    glVertex2i(34,164);
    	    glColor3f(0.0f, 0.3f, 0.0f);
 	    glVertex2i(31,121);
    	    glVertex2i(51,161);
    	    glVertex2i(47,118);
    	    glVertex2i(68,156);
    	    glVertex2i(54,116);
    	    glColor3f(0.0f, 0.275f, 0.0f);
         glVertex2i(83,149);
    	    glVertex2i(63,113);
    	    glVertex2i(97,140);
    	    glVertex2i(72,106);
    	    glVertex2i(107,132);
    	    glColor3f(0.0f, 0.25f, 0.0f);
    	    glVertex2i(80,100);
    	    glVertex2i(117,120);
    	    glVertex2i(89,88);
    	    glVertex2i(123,115);
    	    glVertex2i(100,80);
         glColor3f(0.0f, 0.275f, 0.0f);
    	    glVertex2i(129,113);
    	    glVertex2i(114,72);
    	    glVertex2i(138,114);
    	    glVertex2i(137,71);
    	    glVertex2i(139,114);
    	    glColor3f(0.0f, 0.3f, 0.0f);
    	    glVertex2i(158,78);
    	    glVertex2i(143,120);
    	    glVertex2i(178,96);
    	    glVertex2i(146,128);
    	    glVertex2i(184,111);
    	    glColor3f(0.0f, 0.275f, 0.0f);
    	    glVertex2i(150,139);
    	    glVertex2i(187,123);
    	    glVertex2i(154,151);
    	    glVertex2i(192,137);
    	    glVertex2i(158,163);
    	    glColor3f(0.0f, 0.25f, 0.0f);
    	    glVertex2i(197,153);
    	    glVertex2i(163,180);
    	    glVertex2i(202,169);
    	    glVertex2i(167,193);
    	    glVertex2i(206,180);
    	    glColor3f(0.0f, 0.275f, 0.0f);
    	    glVertex2i(173,205);
    	    glVertex2i(209,186);
    	    glVertex2i(179,215);
    	    glVertex2i(213,194);
    	    glVertex2i(185,224);
    	    glColor3f(0.0f, 0.3f, 0.0f);
    	    glVertex2i(218,202);
    	    glVertex2i(191,233);
    	    glVertex2i(223,207);
    	    glVertex2i(198,240);
    	    glVertex2i(228,213);
    	    glColor3f(0.0f, 0.275f, 0.0f);
    	    glVertex2i(206,248);
    	    glVertex2i(231,216);
    	    glVertex2i(215,255);
    	    glVertex2i(235,220);
    	    glVertex2i(225,261);
    	    glColor3f(0.0f, 0.25f, 0.0f);
    	    glVertex2i(240,223);
    	    glVertex2i(235,264);
    	    glVertex2i(246,225);
    	    glVertex2i(246,266);
    	    glVertex2i(251,226);
    	    glColor3f(0.0f, 0.275f, 0.0f);
    	    glVertex2i(257,268);
    	    glVertex2i(258,227);
    	    glVertex2i(268,268);
    	    glVertex2i(265,227);
    	    glVertex2i(280,266);
    	    glColor3f(0.0f, 0.3f, 0.0f);
    	    glVertex2i(270,226);
    	    glVertex2i(291,263);
    	    glVertex2i(275,225);
    	    glVertex2i(300,259);
    	    glVertex2i(281,223);
    	    glColor3f(0.0f, 0.275f, 0.0f);
    	    glVertex2i(309,254);
    	    glVertex2i(286,220);
    	    glVertex2i(320,243);
    	    glVertex2i(293,214);
    	    glVertex2i(329,234);
    	    glColor3f(0.0f, 0.25f, 0.0f);
    	    glVertex2i(297,210);
    glEnd();

    //Desenhar cabeca da serpente
    glBegin(GL_POLYGON);
    		glVertex2i(329,234);
    		glVertex2i(342,215);
    		glVertex2i(345,196);
    		glVertex2i(341,191);
    		glVertex2i(324,212);
    glEnd();
    glBegin(GL_POLYGON);
    		glVertex2i(329,234);
    		glVertex2i(324,212);
    		glVertex2i(322,212);
    glEnd();
    glBegin(GL_POLYGON);
    		glVertex2i(329,234);
    		glVertex2i(322,212);
    		glVertex2i(297,210);
    glEnd();
    glBegin(GL_POLYGON);
    		glVertex2i(322,212);
    		glVertex2i(297,210);
    		glVertex2i(315,202);
    glEnd();
    glBegin(GL_POLYGON)	;
    		glVertex2i(297,210);
    		glVertex2i(315,202);
    		glVertex2i(326,187);
    		glVertex2i(323,186);
    		glVertex2i(317,189);
    glEnd();



    //Desenha olho da serpente
    glBegin(GL_POLYGON);
          glColor3f(1.0f, 0.0f, 0.0f);
          glVertex2i(333,222);
          glVertex2i(340,207);
          glVertex2i(330,215);
    glEnd();



    //Desenhar Dentes
    glColor3f(0.950f, 0.950f, 0.950f);
    glBegin(GL_POLYGON);
          glVertex2i(340,193);
          glVertex2i(330,190);
          glVertex2i(336,198);
    glEnd();
    glBegin(GL_POLYGON);
          glVertex2i(336,198);
          glVertex2i(329,198);
          glVertex2i(331,204);
    glEnd();
    glBegin(GL_POLYGON);
          glVertex2i(331,204);
          glVertex2i(325,204);
          glVertex2i(326,210);
    glEnd();

    glBegin(GL_POLYGON);
          glVertex2i(324,190);
          glVertex2i(329,192);
          glVertex2i(321,194);
    glEnd();
    glBegin(GL_POLYGON);
          glVertex2i(321,194);
          glVertex2i(324,198);
          glVertex2i(318,199);
    glEnd();


    //SANGUE!!!
    glColor3f(0.950f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
          glVertex2i(333,altura-160);
          glVertex2i(329,altura-160);
          glVertex2i(332,altura-157);
    glEnd();

    glBegin(GL_POLYGON);
          glVertex2i(332,altura-169);
          glVertex2i(330,altura-175);
          glVertex2i(332,altura-177);
          glVertex2i(330,altura-175);
    glEnd();
    glBegin(GL_POLYGON);
          glVertex2i(327,altura-185);
          glVertex2i(329,altura-193);
          glVertex2i(328,altura-195);
          glVertex2i(326,altura-193);
    glEnd();
    glBegin(GL_POLYGON);
          glVertex2i(333,altura-205);
          glVertex2i(334,altura-218);
          glVertex2i(333,altura-220);
          glVertex2i(331,altura-218);
    glEnd();

    //EVIL SNAKE

    //E
    glColor3f(0.80f, 0.80f, 0.80f);
    glBegin(GL_POLYGON);
          glVertex2i(10,314);
          glVertex2i(40,322);
    glColor3f(0.20f, 0.20f, 0.20f);
          glVertex2i(18,286);
    glEnd();
    glBegin(GL_POLYGON);
          glVertex2i(14,300);
    glColor3f(0.80f, 0.80f, 0.80f);
          glVertex2i(46,295);
          glVertex2i(22,271);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.20f, 0.20f, 0.20f);
          glVertex2i(18,286);
          glVertex2i(57,265);
    glColor3f(0.80f, 0.80f, 0.80f);
          glVertex2i(26,256);
    glEnd();

    //V
    glBegin(GL_POLYGON);
          glVertex2i(41,altura-26);
    glColor3f(0.20f, 0.20f, 0.20f);
          glVertex2i(77,altura-55);
          glVertex2i(75,altura-80);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.80f, 0.80f, 0.80f);
          glVertex2i(61,altura-59);
          glVertex2i(81,altura-17);
    glColor3f(0.20f, 0.20f, 0.20f);
          glVertex2i(75,altura-80);
    glEnd();

    //I
    glBegin(GL_POLYGON);
          glVertex2i(88,altura-15);
    glColor3f(0.80f, 0.80f, 0.80f);
          glVertex2i(106,altura-42);
          glVertex2i(104,altura-73);
    glColor3f(0.20f, 0.20f, 0.20f);
          glVertex2i(85,altura-50);
    glEnd();

    //L
    glBegin(GL_POLYGON);
          glVertex2i(101,altura-12);
    glColor3f(0.80f, 0.80f, 0.80f);
          glVertex2i(124,altura-51);
          glVertex2i(116,altura-70);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.20f, 0.20f, 0.20f);
          glVertex2i(124,altura-51);
          glVertex2i(151,altura-60);
    glColor3f(0.80f, 0.80f, 0.80f);
          glVertex2i(116,altura-70);
    glEnd();

    //S
    glBegin(GL_POLYGON);
          glVertex2i(96,altura-75);
    glColor3f(0.250f, 0.250f, 0.250f);
          glVertex2i(84,altura-122);
          glVertex2i(63,altura-114);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.950f, 0.950f, 0.950f);
          glVertex2i(77,altura-164);
          glVertex2i(87,altura-106);
    glColor3f(0.50f, 0.50f, 0.50f);
          glVertex2i(105,altura-115);
    glEnd();

    //N
    glBegin(GL_POLYGON);
          glVertex2i(100,altura-83);
          glColor3f(0.950f, 0.950f, 0.950f);
          glVertex2i(119,altura-99);
          glVertex2i(115,altura-141);
    glEnd();
    glBegin(GL_POLYGON);
         glColor3f(0.250f, 0.250f, 0.250f);
          glVertex2i(100,altura-83);
          glVertex2i(148,altura-107);
          glColor3f(0.50f, 0.50f, 0.50f);
          glVertex2i(154,altura-127);
          glVertex2i(105,altura-103);
    glEnd();
    glBegin(GL_POLYGON);
          glColor3f(0.950f, 0.950f, 0.950f);
          glVertex2i(140,altura-73);
          glVertex2i(154,altura-127);
          glColor3f(0.250f, 0.250f, 0.250f);
          glVertex2i(138,altura-114);
    glEnd();

    //A
    glBegin(GL_POLYGON);
          glVertex2i(158,altura-129);
          glColor3f(0.50f, 0.50f, 0.50f);
          glVertex2i(162,altura-66);
          glVertex2i(172,altura-104);
    glEnd();
    glBegin(GL_POLYGON);
          glColor3f(0.250f, 0.250f, 0.250f);
          glVertex2i(172,altura-104);
          glVertex2i(162,altura-66);
          glColor3f(0.950f, 0.950f, 0.950f);
          glVertex2i(198,altura-117);

    glEnd();

    //K
    glBegin(GL_POLYGON);
          glVertex2i(186,altura-59);
          glColor3f(0.50f, 0.50f, 0.50f);
          glVertex2i(234,altura-103);
          glVertex2i(195,altura-95);
    glEnd();
    glBegin(GL_POLYGON);
          glColor3f(0.250f, 0.250f, 0.250f);
          glVertex2i(195,altura-95);
          glVertex2i(221,altura-53);
          glColor3f(0.950f, 0.950f, 0.950f);
          glVertex2i(202,altura-118);
    glEnd();

    //E
    glBegin(GL_POLYGON);
          glVertex2i(223,300);
          glColor3f(0.50f, 0.50f, 0.50f);
          glVertex2i(255,308);
          glVertex2i(231,272);
    glEnd();
    glBegin(GL_POLYGON);
          glColor3f(0.250f, 0.250f, 0.250f);
          glVertex2i(228,286);
          glVertex2i(261,281);
          glColor3f(0.950f, 0.950f, 0.950f);
          glVertex2i(236,257);
    glEnd();
    glBegin(GL_POLYGON);
          glVertex2i(231,272);
          glColor3f(0.250f, 0.250f, 0.250f);
          glVertex2i(272,251);
          glVertex2i(241,242);
    glEnd();
}
/*
bool colidiu(){
     GLfloat pixels[3];
     int x, y;
     int i = jogo->getCobra()->getHead()->getLine();
     int j = jogo->getCobra()->getHead()->getCol();
     x = funcao(i,j,0)-tamQuadrado/2;
     y = funcao(i,j,1)-tamQuadrado/2;
     glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixels);
     if((pixels[0]==1.0 && pixels[1]==1.0 && pixels[2]==1.0)||
        (pixels[0]==1.0 && pixels[1]==0.0 && pixels[2]==0.0))
        return false;
     else return true;
}
*/
