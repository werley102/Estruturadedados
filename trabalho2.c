#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define char1 100

typedef struct Musica {
  char artista[char1];
  char titulo[char1];
  struct Musica* prox;  
  struct Musica* ant;    
} Musica;


Musica* cabecaPlaylist = NULL;


Musica* criarMusica(const char* artista, const char* titulo) {
  Musica* novaMusica = (Musica*)malloc(sizeof(Musica)); 
  strncpy(novaMusica->artista, artista, char1);    
  strncpy(novaMusica->titulo, titulo, char1);      
  novaMusica->prox = novaMusica->ant = NULL;  
  return novaMusica; 
}

 
void adicionarMusica(const char* artista, const char* titulo) {
  Musica* novaMusica = criarMusica(artista, titulo); 

  if (cabecaPlaylist == NULL) {
    cabecaPlaylist = novaMusica;
    cabecaPlaylist->prox = cabecaPlaylist->ant = cabecaPlaylist; 
    return;
  }

  
  Musica* atual = cabecaPlaylist;
  while (atual->prox != cabecaPlaylist) {
    atual = atual->prox;
  }

  
  atual->prox = novaMusica;
  novaMusica->ant = atual;
  novaMusica->prox = cabecaPlaylist;
  cabecaPlaylist->ant = novaMusica;
}


void imprimirPlaylist() {
  if (cabecaPlaylist == NULL) {
    printf("Playlist está vazia.\n");
    return;
  }

  Musica* atual = cabecaPlaylist;
  do {
    printf("Artista: %s, Título: %s\n", atual->artista, atual->titulo); 
    atual = atual->prox; 
  } while (atual != cabecaPlaylist); 
}


void removerMusica(const char* titulo) {
  if (cabecaPlaylist == NULL) {
    return;
  }

  Musica* atual = cabecaPlaylist;
  do {
    if (strcmp(atual->titulo, titulo) == 0) { 
      
      if (atual->prox == atual) {  
        cabecaPlaylist = NULL;
      } else {
        atual->ant->prox = atual->prox; 
        atual->prox->ant = atual->ant; 
        if (atual == cabecaPlaylist) {
          cabecaPlaylist = atual->prox; 
        }
      }
      free(atual); 
      return;  
    }
    atual = atual->prox; 
  } while (atual != cabecaPlaylist);

  printf("Música '%s' não encontrada na playlist.\n", titulo); 
}


Musica* encontrarMusica(const char* titulo) {
  if (cabecaPlaylist == NULL) {
    return NULL;
  }

  Musica* atual = cabecaPlaylist;
  do {
    if (strcmp(atual->titulo, titulo) == 0) {  
      return atual;  
    }
    atual = atual->prox; 
  } while (atual != cabecaPlaylist);

  return NULL;
}

 
void carregarMusicasDoArquivo(const char* nomeArquivo) {
  FILE* arquivo = fopen(nomeArquivo, "r");  
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }

  char artista[char1], titulo[char1];
  while (fscanf(arquivo, "%[^;];%[^\n]\n", artista, titulo) == 2) {  
    adicionarMusica(artista, titulo);  
  }

  fclose(arquivo);  
}

 
void menu() {
  int escolha;
  char artista[char1], titulo[char1];
  Musica* atual = cabecaPlaylist;

  do {
     
    printf("\nMenu:\n");
    printf("1. Exibir playlist na ordem de cadastro\n");
    printf("2. Inserir nova música\n");
    printf("3. Remover uma música\n");
    printf("4. Buscar uma música\n");
    printf("5. Avançar para a próxima música\n");
    printf("6. Voltar para a música anterior\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &escolha);
    getchar(); 

    switch (escolha) {
      case 1:
        imprimirPlaylist(); 
        break;
      case 2:
        printf("Digite o nome do artista: ");
        fgets(artista, char1, stdin); 
        strtok(artista, "\n"); 
        printf("Digite o título da música: ");
        fgets(titulo, char1, stdin); 
        strtok(titulo, "\n"); 
        adicionarMusica(artista, titulo);  
        break;
      case 3:
        printf("Digite o título da música a ser removida: ");
        fgets(titulo, char1, stdin);  
        strtok(titulo, "\n"); 
        removerMusica(titulo);  
        break;
      case 4:
        printf("Digite o título da música a ser buscada: ");
        fgets(titulo, char1, stdin); 
        strtok(titulo, "\n");  
        Musica* encontrada = encontrarMusica(titulo);  
        if (encontrada != NULL) {
          printf("Música encontrada: Artista: %s, Título: %s\n", encontrada->artista, encontrada->titulo); // Exibe a música encontrada
        } else {
          printf("Música não encontrada.\n"); // Informa se a música não foi encontrada
        }
        break;
      case 5:
        if (atual != NULL) {
          atual = atual->prox; // Avança para a próxima música
          printf("Música atual: Artista: %s, Título: %s\n", atual->artista, atual->titulo); // Exibe a música atual
        } else {
          printf("Playlist está vazia.\n");
        }
        break;
      case 6:
        if (atual != NULL) {
          atual = atual->ant; // Volta para a música anterior
          printf("Música atual: Artista: %s, Título: %s\n", atual->artista, atual->titulo); // Exibe a música atual
        } else {
          printf("Playlist está vazia.\n");
        }
        break;
      case 0:
        printf("Saindo...\n"); // Sai do programa
        break;
      default:
        printf("Opção inválida.\n"); // Informa se a opção é inválida
    }
  } while (escolha != 0); // Continua até que o usuário escolha sair
}

int main() {
  carregarMusicasDoArquivo("musicas.txt"); // Carrega as músicas do arquivo
  menu(); // Chama a função para exibir o menu
  return 0;
}
