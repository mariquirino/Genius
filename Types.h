#include "allegro5\allegro.h"
#ifndef ESTRUTURAS
#define ESTRUTURAS
typedef struct Tback {
	float x, y;
	float velocidade_x, velocidade_y;
	int dirX, dirY;
	int largura, altura;
	ALLEGRO_BITMAP *backGround;
}Tback;

typedef struct usuario{
	char nome[6];
	int recorde;
}TUsuario;
#endif