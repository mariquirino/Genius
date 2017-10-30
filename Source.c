#include <stdio.h>
#include <stdlib.h>
#include "allegro5\allegro.h"
#include "allegro5\allegro_image.h"
#include "allegro5/allegro_audio.h"
#include "allegro5/allegro_acodec.h"
#include "allegro5\allegro_font.h"
#include "allegro5\allegro_ttf.h"
#include "Header.h"

FILE * criarArquivo(char nome[]) {
	FILE * arq;
	arq = fopen(nome, "r+b");
	if (arq == NULL) {
		arq = fopen(nome, "w+b");
	}
	return arq;
}

// Atributos da tela
int LARGURA_TELA = 640;
int ALTURA_TELA = 480;

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_FONT *font = NULL;
ALLEGRO_AUDIO_STREAM *musica = NULL;
ALLEGRO_BITMAP *animacao = NULL;

int nivel = 0, qtdJogadores = 0;
TUsuario usuario;
char str[6];
FILE *arq;

int main(){
	int status, tam;
	arq = criarArquivo("Recorde.txt");
	if (!inicializa()){
		return -1;
	}

	fseek(arq, -sizeof(TUsuario), 2);
	tam = ftell(arq) + sizeof(TUsuario) / sizeof(TUsuario);
	if (tam == 3){
		status = fread(&usuario, sizeof(TUsuario), 1, arq);
		if (status != 1){
			printf("Erro de leitura\n");
		}
	}else{
		usuario.recorde = 0;
		strcpy(usuario.nome, "");
	}
	tela_Inicial();

	al_destroy_display(janela);
	al_destroy_font(font);
	al_destroy_event_queue(fila_eventos);
	al_destroy_audio_stream(musica);
	fclose(arq);
	
	return 0;
}

int inicializa(){
	if (!al_init()){
		printf("Falha ao inicializar a Allegro.\n");
		return 0;
	}
	if (!al_install_audio()){
		printf("Falha ao inicializar �udio.\n");
		return 0;
	}
	if (!al_init_acodec_addon()){
		printf("Falha ao inicializar codecs de �udio.\n");
		return 0;
	}
	al_init_font_addon();
	if (!al_init_ttf_addon()){
		printf("Falha ao inicializar allegro_ttf.\n");
		return 0;
	}
	if (!al_init_image_addon()){
		printf("Falha ao inicializar allegro_image.\n");
		return 0;
	}
	if (!al_reserve_samples(1)){
		printf("Falha ao alocar canais de �udio.\n");
		return 0;
	}
	//Inicilizando a janela
	janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
	if (!janela){
		printf("Falha ao criar a janela.\n");
		return 0;
	}
	//Inicializando o texto
	font = al_load_font("VCR_OSD_MONO_1.001.ttf", 30, 0);
	if (!font){
		printf("Falha ao carregar \"VCR_OSD_MONO_1.001.ttf\".\n");
		return 0;
	}
	//Inicializando a musica de fundo
	musica = al_load_audio_stream("Sua cara.wav", 4, 1024);
	if (!musica){
		printf("Falha ao carregar \"Sua cara.wav\".\n");
		return 0;
	}
	animacao = al_load_bitmap("animacao_genius.png");
	if (!animacao){
		printf("Falha ao carregar imagem \n");
		return 0;
	}
	// Torna apto o uso de mouse na aplica��o
	if (!al_install_mouse()){
		printf("Falha ao inicializar o mouse.\n");
		return 0;
	}
	if (!al_install_keyboard()){
		printf("Falha ao inicializar teclado.\n");
		return 0;
	}
	// Atribui o cursor padr�o do sistema para ser usado
	if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)){
		printf("Falha ao atribuir ponteiro do mouse.\n");
		return 0;
	}
	fila();
	// Configura o t�tulo da janela
	al_set_window_title(janela, "Genius");
	/*al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
	al_set_audio_stream_playmode(musica, ALLEGRO_PLAYMODE_LOOP);*/
	return 1;
}

void fila(){
	fila_eventos = al_create_event_queue();
	if (!fila_eventos){
		printf("Falha ao criar fila de eventos.\n");
		return 0;
	}
	al_register_event_source(fila_eventos, al_get_mouse_event_source());// Dizemos que vamos tratar os eventos vindos do mouse
	al_register_event_source(fila_eventos, al_get_display_event_source(janela));//Tratando eventos do display
	al_register_event_source(fila_eventos, al_get_keyboard_event_source());//Eventos do teclado
}

void desenha(ALLEGRO_BITMAP *verde, ALLEGRO_BITMAP *amarelo, ALLEGRO_BITMAP *azul, ALLEGRO_BITMAP *vermelho){
	// Desenhamos a imagem na tela
	al_draw_bitmap(verde, 85, 40, 0);
	al_draw_bitmap(amarelo, 85, 238, 0);
	al_draw_bitmap(azul, 284, 238, 0);
	al_draw_bitmap(vermelho, 284, 40, 0);
	
	// Atualiza a tela
	al_flip_display();
}

void piscaSequencia(int vetor[], ALLEGRO_BITMAP *verde, ALLEGRO_BITMAP *amarelo, ALLEGRO_BITMAP *azul, ALLEGRO_BITMAP *vermelho, int flag){
	ALLEGRO_BITMAP *fundo;
	fundo = al_load_bitmap("Genius.png");

	//Aqui pisca a sequencia
	for (int j = 0; j < nivel; j++){
		al_draw_bitmap(fundo, 0, 0, 0);
		al_draw_text(font, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, 0, ALLEGRO_ALIGN_CENTER, "Macaxeira eh o poder");
		al_draw_textf(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 280, ALLEGRO_ALIGN_CENTER, "Jogador:%d", flag % qtdJogadores + 1);
		al_draw_textf(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 310, ALLEGRO_ALIGN_CENTER, "Nivel:%d", nivel);
		if (vetor[j] == 0){
			al_draw_bitmap(verde, 85, 40, 0);
		}else if (vetor[j] == 1){
			al_draw_bitmap(amarelo, 85, 238, 0);
		}else if (vetor[j] == 2){
			al_draw_bitmap(azul, 284, 238, 0);
		}else{
			al_draw_bitmap(vermelho, 284, 40, 0);
		}
		al_flip_display();
		al_rest(0.5);

		al_draw_bitmap(fundo, 0, 0, 0);
		al_draw_text(font, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, 0, ALLEGRO_ALIGN_CENTER, "Macaxeira eh o poder");
		al_draw_textf(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 280, ALLEGRO_ALIGN_CENTER, "Jogador:%d", flag % qtdJogadores + 1);
		al_draw_textf(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 310, ALLEGRO_ALIGN_CENTER, "Nivel:%d", nivel);
		al_flip_display();
		al_rest(0.5);
	}
	al_destroy_bitmap(fundo);
}

void sequencia(int vetor[]){
	int aleatorio;
	//Aqui gera a sequencia
	srand(time(NULL));
	aleatorio = rand() % 4;
	vetor[nivel] = aleatorio;
	nivel++;
}

void tela_Inicial(){
	int sair = 0;
	Tback back;
	InitBG(&back, 0, 0, 0.01, 0, LARGURA_TELA, ALTURA_TELA, -1, 1, animacao);
	while (!sair){
		//Atualizando o record
		if (usuario.recorde < nivel){
			novoRecord();
			nivel = 0;
		}
		
		printaBG(&back);
		//Desenhando a tela inicial
		al_draw_text(font, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 3, ALLEGRO_ALIGN_CENTER, "Genius");
		al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 3 + 100, ALLEGRO_ALIGN_CENTER, "Jogar");
		al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 3 + 150, ALLEGRO_ALIGN_CENTER, "Record");
		al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 3 + 200, ALLEGRO_ALIGN_CENTER, "Sair");
		al_flip_display();
		atualizarBG(&back);
		
		// Verificamos se h� eventos na fila
		while (!al_event_queue_is_empty(fila_eventos)){
			ALLEGRO_EVENT evento;
			al_wait_for_event(fila_eventos, &evento);
			// Se o evento foi de click do mouse
			if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
				if (evento.mouse.x >= 276 && evento.mouse.x <= 356 && evento.mouse.y >= 268  && evento.mouse.y <= 284){
					modoJogo();
				}else if (evento.mouse.x >= 269 && evento.mouse.x <= 371 && evento.mouse.y >= 318 && evento.mouse.y <= 333){//Fazer arquivo para guardar os records
					exibir_record();
				}else if (evento.mouse.x >= 286 && evento.mouse.x <= 352 && evento.mouse.y >= 366 && evento.mouse.y <= 385){
					sair = 1;
				}
			}else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){//Quando o usuario clicar no X
				sair = 1;
			}
		}
	}
}

void errou(int jogador){
	al_set_audio_stream_playing(musica, false);
	ALLEGRO_SAMPLE *sample = NULL;
	sample = al_load_sample("faustao.ogg");
	al_play_sample(sample, 2.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	al_clear_to_color(al_map_rgb(82, 255, 238));
	al_draw_text(font, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 3 - 50, ALLEGRO_ALIGN_CENTER, "Sequencia Errada");
	al_draw_textf(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 3, ALLEGRO_ALIGN_CENTER, "Jogador %d perdeu", jogador);
	al_flip_display();
	al_rest(1.5);
	al_destroy_sample(sample);
	al_set_audio_stream_playmode(musica, ALLEGRO_PLAYMODE_LOOP);
}

void exibir_record(){
	int status;
	TUsuario aux;
	int y = ALTURA_TELA / 3 + 50;
	al_clear_to_color(al_map_rgb(82, 255, 238));
	al_draw_text(font, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, 100, ALLEGRO_ALIGN_CENTER, "Recordes");
	fseek(arq, 0, 0);
	while (1){
		status = fread(&aux, sizeof(TUsuario), 1, arq);
		if (status != 1) {
			if (feof(arq)) {
				printf("Fim de arquivo\n");
				break;
			}else{
				printf("Erro na leitura\n");
				return -1;
			}
		}else{
			al_draw_textf(font, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, y, ALLEGRO_ALIGN_CENTER, "Nome:%s ---- Record:%d", aux.nome, aux.recorde);
			al_flip_display();
			y += 50;
		}
	}
	al_rest(1.5);
}

void modoJogo(){
	int sair = 0;
	Tback back;
	InitBG(&back, 0, 0, 0.01, 0, LARGURA_TELA, ALTURA_TELA, -1, 1, animacao);
	while (!sair){
		//Zerando o jogo
		nivel = 0;

		//Desenhando o modo de jogo
		printaBG(&back);
		al_draw_text(font, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 3, ALLEGRO_ALIGN_CENTER, "Genius");
		al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 3 + 100, ALLEGRO_ALIGN_CENTER, "Um jogador");
		al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 3 + 135, ALLEGRO_ALIGN_CENTER, "Dois jogadores");
		al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 3 + 170, ALLEGRO_ALIGN_CENTER, "Tres jogadores");
		al_flip_display();
		atualizarBG(&back);

		// Verificamos se h� eventos na fila
		while (!al_event_queue_is_empty(fila_eventos)){
			ALLEGRO_EVENT evento;
			al_wait_for_event(fila_eventos, &evento);
			// Se o evento foi de click do mouse
			if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
				if (evento.mouse.x >= 234 && evento.mouse.x <= 405 && evento.mouse.y >= 267 && evento.mouse.y <= 284){
					qtdJogadores = 1;
					nivelJogo();
					sair = 1;
				}else if (evento.mouse.x >= 200 && evento.mouse.x <= 440 && evento.mouse.y >= 302 && evento.mouse.y <= 319){//Dois jogadores
					qtdJogadores = 2;
					nivelJogo();
					sair = 1;
				}else if (evento.mouse.x >= 199 && evento.mouse.x <= 440 && evento.mouse.y >= 337 && evento.mouse.y <= 355){//Tres jogadores
					qtdJogadores = 3;
					nivelJogo();
					sair = 1;
				}
			}else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){//Quando o usuario clicar no X
				sair = 1;
			}
		}
	}
}

void nivelJogo(){
	int sair = 0;
	Tback back;
	InitBG(&back, 0, 0, 0.01, 0, LARGURA_TELA, ALTURA_TELA, -1, 1, animacao);
	while (!sair){
		//Zerando o jogo
		nivel = 0;

		printaBG(&back);
		//Desenhando o modo de jogo
		al_draw_text(font, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 3, ALLEGRO_ALIGN_CENTER, "Genius");
		al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 3 + 100, ALLEGRO_ALIGN_CENTER, "Nivel 1");
		al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 3 + 135, ALLEGRO_ALIGN_CENTER, "Nivel 5");
		al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 3 + 170, ALLEGRO_ALIGN_CENTER, "Nivel 10");
		al_flip_display();
		atualizarBG(&back);

		// Verificamos se h� eventos na fila
		while (!al_event_queue_is_empty(fila_eventos)){
			ALLEGRO_EVENT evento;
			al_wait_for_event(fila_eventos, &evento);
			// Se o evento foi de click do mouse
			if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
				if (evento.mouse.x >= 259 && evento.mouse.x <= 374 && evento.mouse.y >= 264 && evento.mouse.y <= 284){
					genius(1);
					sair = 1;
				}else if (evento.mouse.x >= 259 && evento.mouse.x <= 380 && evento.mouse.y >= 299 && evento.mouse.y <= 319){//Nivel 5
					genius(5);
					sair = 1;
				}else if (evento.mouse.x >= 250 && evento.mouse.x <= 388 && evento.mouse.y >= 334 && evento.mouse.y <= 355){//Nivel 10
					genius(10);
					sair = 1;
				}
			}else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){//Quando o usuario clicar no X
				sair = 1;
			}
		}
	}
}

void genius(int level){
	int vetor[1000], vetor2[1000], vetor3[1000];
	ALLEGRO_BITMAP *verde = 0, *amarelo = 0, *azul = 0, *vermelho = 0;
	ALLEGRO_BITMAP *imagem = NULL;
	// Flag que condicionar� nosso looping
	int sair = 0, i = 0, flag = -1, perdeu = 0;

	// Alocamos as cores
	verde = al_load_bitmap("VerdeNeon.png");
	amarelo = al_load_bitmap("AmareloNeon.png");
	azul = al_load_bitmap("AzulNeon.png");
	vermelho = al_load_bitmap("VermelhoNeon.png");
	//Alocamos o fundo
	imagem = al_load_bitmap("Genius.png");

	//Nivel
	if (level != 1){
		gerando_nivel(level, vetor, 1, imagem, verde, amarelo, azul, vermelho);
		if (qtdJogadores >= 2){
			nivel = 0;
			gerando_nivel(level, vetor2, 1, imagem, verde, amarelo, azul, vermelho);
		}
		if (qtdJogadores == 3){
			nivel = 0;
			gerando_nivel(level, vetor3, 1, imagem, verde, amarelo, azul, vermelho);
		}
		//Desenha o fundo
		al_draw_bitmap(imagem, 0, 0, 0);
		//Colocando os textos
		al_draw_text(font, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, 0, ALLEGRO_ALIGN_CENTER, "Macaxeira eh o poder");
		al_draw_textf(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 280, ALLEGRO_ALIGN_CENTER, "Jogador:%d", flag % qtdJogadores + 1);
		al_draw_textf(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 310, ALLEGRO_ALIGN_CENTER, "Nivel:%d", nivel);
		al_flip_display();
		piscaSequencia(vetor, verde, amarelo, azul, vermelho, 0);
		flag++;
	}

	//Quando o sair for 3 � pq os dois erraram
	while (!sair){
		
		//Gerando a jogada
		if (i == nivel){
			al_rest(0.5);
			i = 0;
			flag++;
			if (qtdJogadores == 1){
				flag = 1;
			}
			if (flag % qtdJogadores == 0){
				sequencia(vetor);
				piscaSequencia(vetor, verde, amarelo, azul, vermelho, flag);
			}else if(flag % qtdJogadores == 1){
				nivel--;
				sequencia(vetor2);
				piscaSequencia(vetor2, verde, amarelo, azul, vermelho, flag);
			}else if (flag % qtdJogadores == 2){
				nivel--;
				sequencia(vetor3);
				piscaSequencia(vetor3, verde, amarelo, azul, vermelho, flag);
			}
			fila();
		}

		al_draw_text(font, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, 0, ALLEGRO_ALIGN_CENTER, "Macaxeira eh o poder");
		al_draw_textf(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 280, ALLEGRO_ALIGN_CENTER, "Jogador:%d", flag % qtdJogadores + 1);
		al_draw_textf(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 310, ALLEGRO_ALIGN_CENTER, "Nivel:%d", nivel);
		al_flip_display();

		// Verificamos se h� eventos na fila
		while (!al_is_event_queue_empty(fila_eventos)){
			ALLEGRO_EVENT evento;
			al_wait_for_event(fila_eventos, &evento);

			// Se o evento foi de click do mouse
			if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
				if (area_verde(evento.mouse.x, evento.mouse.y)){// Verificamos se ele est� sobre a regi�o do verde
					if (flag % qtdJogadores == 0){
						if (vetor[i] == 0){
							al_draw_bitmap(verde, 85, 40, 0);
							i++;
						}else{
							perdeu = 1;
						}
					}else if(flag % qtdJogadores == 1){
						if (vetor2[i] == 0){
							al_draw_bitmap(verde, 85, 40, 0);
							i++;
						}else{
							perdeu = 2;
						}
					}else{
						if (vetor3[i] == 0){
							al_draw_bitmap(verde, 85, 40, 0);
							i++;
						}else{
							perdeu = 3;
						}
					}
				}else if (area_amarela(evento.mouse.x, evento.mouse.y)){ // Verificamos se ele est� sobre a regi�o no amarelo
					if (flag % qtdJogadores == 0){
						if (vetor[i] == 1){
							al_draw_bitmap(amarelo, 85, 238, 0);
							i++;
						}else{
							perdeu = 1;
						}
					}else if(flag % qtdJogadores == 1){
						if (vetor2[i] == 1){
							al_draw_bitmap(amarelo, 85, 238, 0);
							i++;
						}else{
							perdeu = 2;
						}
					}else{
						if (vetor3[i] == 1){
							al_draw_bitmap(amarelo, 85, 238, 0);
							i++;
						}else{
							perdeu = 3;
						}
					}
				}else if (area_azul(evento.mouse.x, evento.mouse.y)){// Verificamos se ele est� sobre a regi�o no azul
					if (flag % qtdJogadores == 0){
						if (vetor[i] == 2){
							al_draw_bitmap(azul, 284, 238, 0);
							i++;
						}else{
							perdeu = 1;
						}
					}else if(flag % qtdJogadores == 1){
						if (vetor2[i] == 2){
							al_draw_bitmap(azul, 284, 238, 0);
							i++;
						}else{
							perdeu = 2;
						}
					}else{
						if (vetor3[i] == 2){
							al_draw_bitmap(azul, 284, 238, 0);
							i++;
						}else{
							perdeu = 3;
						}
					}
				}else if (area_vermelha(evento.mouse.x, evento.mouse.y)){// Verificamos se ele est� sobre a regi�o no vermelho
					if (flag % qtdJogadores == 0){
						if (vetor[i] == 3){
							al_draw_bitmap(vermelho, 284, 40, 0);
							i++;
						}else{
							perdeu = 1;
						}
					}else if(flag % qtdJogadores == 1){
						if (vetor2[i] == 3){
							al_draw_bitmap(vermelho, 284, 40, 0);
							i++;
						}else{
							perdeu = 2;
						}
					}else{
						if (vetor3[i] == 3){
							al_draw_bitmap(vermelho, 284, 40, 0);
							i++;
						}else{
							perdeu = 3;
						}
					}
				}
				if (perdeu != 0){
					errou(perdeu);
					if (qtdJogadores > 1){
						if (perdeu == 1){
							vencedor(2);
							vencedor(3);
						}else if(perdeu == 2){
							vencedor(1);
							vencedor(3);
						}else if(perdeu == 3){
							vencedor(1);
							vencedor(2);
						}
					}
					sair = 1;
				}
				if (!sair){
					al_flip_display();
					al_rest(0.2);
				}
			}else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && !sair){
				al_draw_bitmap(imagem, 0, 0, 0);
				al_draw_text(font, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, 0, ALLEGRO_ALIGN_CENTER, "Macaxeira eh o poder");
				al_draw_textf(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 280, ALLEGRO_ALIGN_CENTER, "Jogador:%d", flag % qtdJogadores + 1);
				al_draw_textf(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 310, ALLEGRO_ALIGN_CENTER, "Nivel:%d", nivel);
				al_flip_display();
			}else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){//Quando o usuario clicar no X
				sair = 1;
				nivel = 0;
			}
		}
	}

	// Desaloca os recursos utilizados na aplica��o
	al_destroy_bitmap(verde);
	al_destroy_bitmap(amarelo);
	al_destroy_bitmap(azul);
	al_destroy_bitmap(vermelho);

}

void gerando_nivel(int level, int vetor[],int joga, ALLEGRO_BITMAP *imagem, ALLEGRO_BITMAP *verde, ALLEGRO_BITMAP *amarelo, ALLEGRO_BITMAP *azul, ALLEGRO_BITMAP *vermelho){
	//Gerando a sequencia do level
	for (int i = 0; i < level; i++){
		al_rest(1.0);
		sequencia(vetor);
	}
}

void vencedor(int jogador){
	al_clear_to_color(al_map_rgb(82, 255, 238));
	al_draw_textf(font, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2 - 100, ALLEGRO_ALIGN_CENTER, "Fim de jogo");
	al_draw_textf(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2, ALLEGRO_ALIGN_CENTER, "Jogador %d venceu", jogador);
	al_flip_display();
	al_rest(1.5);
}

void novoRecord(){
	int sair = 0, tam, status;
	TUsuario aux;
	strcpy(str, "");
	al_clear_to_color(al_map_rgb(82, 255, 238));
	al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA - 300, ALLEGRO_ALIGN_CENTER, "Novo recorde batido.");
	al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA - 250, ALLEGRO_ALIGN_CENTER, "Digite seu nome: ");
	al_flip_display();
	while (!sair){
		al_clear_to_color(al_map_rgb(82, 255, 238));
		al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA - 300, ALLEGRO_ALIGN_CENTER, "Novo recorde batido.");
		if (!al_event_queue_is_empty(fila_eventos)){
			ALLEGRO_EVENT evento;
			al_wait_for_event(fila_eventos, &evento);
			if (evento.type == ALLEGRO_EVENT_KEY_CHAR){
				if (strlen(str) <= 4){
					char temp[] = { evento.keyboard.unichar, '\0' };
					if (evento.keyboard.unichar >= '0' && evento.keyboard.unichar <= '9'){
						strcat(str, temp);
					}
					else if (evento.keyboard.unichar >= 'A' && evento.keyboard.unichar <= 'Z'){
						strcat(str, temp);
					}
					else if (evento.keyboard.unichar >= 'a' && evento.keyboard.unichar <= 'z'){
						strcat(str, temp);
					}
				}
				else{
					sair = 1;
				}

				if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(str) != 0){
					str[strlen(str) - 1] = '\0';
				}
			}
			if (evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
				sair = 1;
			}
			al_draw_textf(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA - 250, ALLEGRO_ALIGN_CENTER, "Digite seu nome:%s", str);
			al_flip_display();
		}
	}
	usuario.recorde = nivel;
	strcpy(usuario.nome, str);
	fseek(arq, 0, 2);
	tam = ftell(arq) / sizeof(TUsuario);
	if (tam < 3){
		fseek(arq, tam - 1 * sizeof(TUsuario), 0);
		status = fwrite(&usuario, sizeof(TUsuario), 1, arq);
		if (status != 1){
			printf("Erro de leitura. \n");
		}
		usuario.recorde = 0;
	}else{
		fseek(arq, 0, 0);
		for (int i = 0; i < tam; i++){
			status = fread(&aux, sizeof(TUsuario), 1, arq);
			if (status == 1){
				if (aux.recorde < usuario.recorde){
					if (i == 0){//Primeiro lugar
						printf("Primeiro lugar. \n");
						TUsuario terceiro;
						status = fread(&terceiro, sizeof(TUsuario), 1, arq);
						fseek(arq, 0, 0);
						status = fwrite(&usuario, sizeof(TUsuario), 1, arq);
						status = fwrite(&aux, sizeof(TUsuario), 1, arq);
						status = fwrite(&terceiro, sizeof(TUsuario), 1, arq);
					}else if (i == 1){//Segundo lugar
						printf("Segundo lugar. \n");
						fseek(arq, -sizeof(TUsuario), 1);
						status = fwrite(&usuario, sizeof(TUsuario), 1, arq);
						if (status != 1){
							printf("Erro de Escrita. \n");
						}
						fseek(arq, 0, 2);
						status = fwrite(&aux, sizeof(TUsuario), 1, arq);
						if (status != 1){
							printf("Erro de Escrita. \n");
						}
					}else{//Terceiro lugar
						printf("Terceiro lugar. \n");
						fseek(arq, -sizeof(TUsuario), 1);
						status = fwrite(&usuario, sizeof(TUsuario), 1, arq);
						if (status != 1){
							printf("Erro de Escrita. \n");
						}
					}
				}
			}else{
				printf("Erro de leitura. \n");
			}
		}
	}
}

void InitBG(Tback *back, float x, float y, float velX, float velY, int largura, int altura, int dirX, int dirY, ALLEGRO_BITMAP *imagem) {
	back->x = x;
	back->y = y;
	back->altura = altura;
	back->largura = largura;
	back->velocidade_x = velX;
	back->velocidade_y = velY;
	back->dirX = dirX;
	back->dirY = dirY;
	back->backGround = imagem;
}

void atualizarBG(Tback *back) {
	back->x += back->velocidade_x * back->dirX;
	if (back->x + back->largura <= 0) {
		back->x = 0;
	}
}

void printaBG(Tback *back) {
	al_draw_bitmap(back->backGround, back->x, back->y, 0);
	if (back->x + back->largura < LARGURA_TELA) {
		al_draw_bitmap(back->backGround, back->x + back->largura, back->y, 0);
	}
}

int area_verde(int x, int y){
	double i, j;
	for (i = 311 - 40, j = 79; i > 127 && j < 225 - 99; i = i - 2, j++){
		double a, b;
		for (a = 127, b = 264 - 40 ; a < 311 - 99, b > 140; a = a + 0.5, b--){
			if ((x >= i && x <= i + 40 && y >= j && y <= j + 96) || (x >= a && x <= a + 96 && y >= b && y <= b + 40)){
				return 1;
			}
		}
	}
	return 0;
}

int area_amarela(int x, int y){
	double i, j;
	for (i = 313 - 40, j = 366; i > 127 && j > 320; i = i - 2, j--){
		double a, b;
		for (a = 127, b = 279; a < 311 - 99, b < 360; a = a + 0.5, b++){
			if ((x >= i && x <= i + 40 && y >= j && y <= j + 96) || (x >= a && x <= a + 96 && y >= b && y <= b + 40)){
				return 1;
			}
		}
	}
	return 0;
}

int area_azul(int x, int y){
	double i, j;
	for (i = 327, j = 366; i < 415 && j > 320; i = i + 2, j--){
		double a, b;
		for (a = 417, b = 279; a > 311 - 99, b < 360; a = a - 0.5, b++){
			if ((x >= i && x <= i + 40 && y >= j && y <= j + 96) || (x >= a && x <= a + 96 && y >= b && y <= b + 40)){
				return 1;
			}
		}
	}
	return 0;
}

int area_vermelha(int x, int y){
	double i, j;
	for (i = 327, j = 79; i < 415 && j < 225 - 99; i = i + 2, j++){
		double a, b;
		for (a = 417, b = 264 - 40; a > 311 - 99, b > 140; a = a - 0.5, b--){
			if ((x >= i && x <= i + 40 && y >= j && y <= j + 96) || (x >= a && x <= a + 96 && y >= b && y <= b + 40)){
				return 1;
			}
		}
	}
	return 0;
}