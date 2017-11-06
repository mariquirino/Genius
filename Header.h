//
// Created by mariana on 05/11/17.
//

#include <stdio.h>
#include <stdlib.h>
#include "allegro5/allegro.h"
#include "Types.h"

int inicializa();
void fila();//Aqui inicializa a fila de eventos
void tela_Inicial();//Tela Inicial
void genius(int nivel);//Tela do jogo
int verifica(int jogada, int pos, ALLEGRO_BITMAP *cor, int *i, int posX, int posY);//Verficando se a pessoa selecionou a cor certa
void gerando_nivel(int level, int vetor[], int joga, ALLEGRO_BITMAP *imagem, ALLEGRO_BITMAP *verde, ALLEGRO_BITMAP *amarelo, ALLEGRO_BITMAP *azul, ALLEGRO_BITMAP *vermelho);//Gera o nivel 5 e 10
int area_verde(int x, int y);//Aqui o mapeamento da imagem da area verde
int area_amarela(int x, int y);//Aqui o mapeamento da imagem da area amarela
int area_azul(int x, int y);//Aqui o mapeamento da imagem da area azul
int area_vermelha(int x, int y);//Aqui o mapeamento da imagem da area vermelha
void piscaSequencia(int vetor[], ALLEGRO_BITMAP *verde, ALLEGRO_BITMAP *amarelo, ALLEGRO_BITMAP *azul, ALLEGRO_BITMAP *vermelho, int flag);//Pisa a sequencia da jogada
void sequencia(int vetor[]);//Aqui gera a jogada
void errou(int jogador);//Tela pra mostrar q a pessoa errou
void exibir_record();//Tela que exibe o recorde
void modoJogo();//Tela do modo de jogo
void nivelJogo();//Tela que exibe os niveis disponiveis
void vencedor(int jogador);//Tela que exibe quem venceu
void novoRecord();//Salva o recorde
void salva_recorde();
void printaBG(Tback *back);
void atualizarBG(Tback *back);
void InitBG(Tback *back, float x, float y, float velX, float velY, int largura, int altura, int dirX, int dirY, ALLEGRO_BITMAP *imagem);
