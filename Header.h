#include <stdio.h>
#include <stdlib.h>
#include "allegro5\allegro.h"
#include "Types.h"

int inicializa();
void fila();
void tela_Inicial();
void genius(int nivel);
void gerando_nivel(int level, int vetor[], int joga, ALLEGRO_BITMAP *imagem, ALLEGRO_BITMAP *verde, ALLEGRO_BITMAP *amarelo, ALLEGRO_BITMAP *azul, ALLEGRO_BITMAP *vermelho);
int area_verde(int x, int y);
int area_amarela(int x, int y);
int area_azul(int x, int y);
int area_vermelha(int x, int y);
void piscaSequencia(int vetor[], ALLEGRO_BITMAP *verde, ALLEGRO_BITMAP *amarelo, ALLEGRO_BITMAP *azul, ALLEGRO_BITMAP *vermelho, int flag);
void sequencia(int vetor[]);
void errou(int jogador);
void exibir_record();
void modoJogo();
void nivelJogo();
void vencedor(int jogador);
void novoRecord();
void printaBG(Tback *back);
void atualizarBG(Tback *back);
void InitBG(Tback *back, float x, float y, float velX, float velY, int largura, int altura, int dirX, int dirY, ALLEGRO_BITMAP *imagem);