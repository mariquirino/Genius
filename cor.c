#include <stdio.h>
#include <stdlib.h>
#include "allegro5\allegro.h"
#include "allegro5\allegro_image.h"




int main(){
	ALLEGRO_DISPLAY *janela = NULL;

	// Atributos da tela
	int LARGURA_TELA = 640;
	int ALTURA_TELA = 480;

	if (!al_init()){
		printf("Falha ao inicializar a Allegro.\n");
		return -1;
	}

	janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
	if (!janela){
		printf("Falha ao criar janela.\n");
		return -1;
	}

	// Configura o título da janela
	al_set_window_title(janela, "Genius");

	// Torna apto o uso de mouse na aplicação
	if (!al_install_mouse()){
		printf("Falha ao inicializar o mouse.\n");
		al_destroy_display(janela);
		return -1;
	}

	// Atribui o cursor padrão do sistema para ser usado
	if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)){
		printf("Falha ao atribuir ponteiro do mouse.\n");
		al_destroy_display(janela);
		return -1;
	}

	ALLEGRO_BITMAP *verde = 0, *amarelo = 0, *azul = 0, *vermelho = 0;

	// Alocamos o retângulo central da tela
	verde = al_create_bitmap(LARGURA_TELA / 4, ALTURA_TELA / 4);
	if (!verde){
		printf("Falha ao criar bitmap.\n");
		al_destroy_display(janela);
		return -1;
	}

	

	//al_clear_to_color(al_map_rgb(0, 0, 0));

	//Colorindo o botao verde
	al_set_target_bitmap(verde);
	al_clear_to_color(al_map_rgb(0, 255, 0));

	al_set_target_bitmap(al_get_backbuffer(janela));
	al_draw_bitmap(verde, LARGURA_TELA / 4 - al_get_bitmap_width(verde) / 2,
		ALTURA_TELA / 2 - al_get_bitmap_height(verde) / 2, 0);

	// Atualiza a tela
	al_flip_display();

	al_rest(1.0);

	al_set_target_bitmap(verde);
	al_clear_to_color(al_map_rgb(255, 255, 255));

	al_set_target_bitmap(al_get_backbuffer(janela));
	al_draw_bitmap(verde, LARGURA_TELA / 4 - al_get_bitmap_width(verde) / 2,
		ALTURA_TELA / 2 - al_get_bitmap_height(verde) / 2, 0);

	// Atualiza a tela
	al_flip_display();

	al_rest(1.0);

	//Colorindo o botao verde
	al_set_target_bitmap(verde);
	al_clear_to_color(al_map_rgb(0, 255, 0));

	al_set_target_bitmap(al_get_backbuffer(janela));
	al_draw_bitmap(verde, LARGURA_TELA / 4 - al_get_bitmap_width(verde) / 2,
		ALTURA_TELA / 2 - al_get_bitmap_height(verde) / 2, 0);

	// Atualiza a tela
	al_flip_display();

	al_rest(1.0);

	al_destroy_display(janela);

	return 0;
}