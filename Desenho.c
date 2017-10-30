#include <stdio.h>
#include <stdlib.h>
#include "allegro5\allegro.h"
#include "allegro5\allegro_image.h"
#include "allegro5\allegro_font.h"
#include "allegro5\allegro_ttf.h"

// Atributos da tela
int LARGURA_TELA = 640;
int ALTURA_TELA = 480;

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_FONT *font = NULL;

ALLEGRO_BITMAP *imagem = NULL;

void tela_Inicial();

int main(){
	if (!al_init()){
		printf("Falha ao inicializar a Allegro.\n");
		return -1;
	}
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();

	//Inicilizando a janela
	janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
	if (!janela){
		printf("Falha ao criar janela.\n");
		return -1;
	}

	//Inicializando o texto
	font = al_load_font("VCR_OSD_MONO_1.001.ttf", 30, 0);

	imagem = al_load_bitmap("Genius.png");

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

	fila_eventos = al_create_event_queue();

	// Dizemos que vamos tratar os eventos vindos do mouse
	al_register_event_source(fila_eventos, al_get_mouse_event_source());
	//Tratando eventos do display
	al_register_event_source(fila_eventos, al_get_display_event_source(janela));

	tela_Inicial();

	al_destroy_display(janela);
	al_destroy_font(font);
	al_destroy_event_queue(fila_eventos);

	return 0;
}

void tela_Inicial(){
	int sair = 0;
	al_register_event_source(fila_eventos, al_get_display_event_source(janela));
	// Dizemos que vamos tratar os eventos vindos do mouse
	al_register_event_source(fila_eventos, al_get_mouse_event_source());
	al_clear_to_color(al_map_rgb(255, 255, 255));
	// Desenha a imagem na tela
	al_draw_bitmap(imagem, 0, 0, 0);

	while (!sair){
		al_draw_text(font, al_map_rgb(255, 255, 255), 325, 280, ALLEGRO_ALIGN_CENTER, "Nivel:1");
		// Atualiza a tela
		al_flip_display();
		ALLEGRO_EVENT evento;
		al_wait_for_event(fila_eventos, &evento);
		if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
			printf("X = %d, Y = %d \n", evento.mouse.x, evento.mouse.y);
			if ((evento.mouse.x >= 127 && evento.mouse.x <= 311 && evento.mouse.y >= 78 && evento.mouse.y <= 263)){
				//sair = 1;
				printf("Foi \n");
			}
			else
			{
				printf("3 \n");
			}
		}
	}
	

}