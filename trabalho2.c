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
    printf("Artista: %s, Título: %s\n", atual->artista, atual->titulo); // Imprime o artista e título da música atual
    atual = atual->prox; // Avança para a próxima música
  } while (atual != cabecaPlaylist); // Continua até voltar à cabeça da lista (indicando uma lista circular)
}

// Função para remover uma música pelo título
void removerMusica(const char* titulo) {
  if (cabecaPlaylist == NULL) {
    return;
  }

  Musica* atual = cabecaPlaylist;
  do {
    if (strcmp(atual->titulo, titulo) == 0) { // Encontra a música com o título correspondente
      // Remove a música da playlist
      if (atual->prox == atual) {  // Única música na playlist
        cabecaPlaylist = NULL;
      } else {
        atual->ant->prox = atual->prox; // Atualiza o ponteiro "prox" da música anterior
        atual->prox->ant = atual->ant; // Atualiza o ponteiro "ant" da próxima música
        if (atual == cabecaPlaylist) {
          cabecaPlaylist = atual->prox; // Atualiza a cabeça da lista se a música removida for a primeira
        }
      }
      free(atual); // Libera a memória da música removida
      return;  // Saia após remover a primeira música correspondente
    }
    atual = atual->prox; // Avança para a próxima música
  } while (atual != cabecaPlaylist);

  printf("Música '%s' não encontrada na playlist.\n", titulo); // Informa se a música não foi encontrada
}

// Função para encontrar uma música pelo título (retorna o ponteiro da música ou NULL)
Musica* encontrarMusica(const char* titulo) {
  if (cabecaPlaylist == NULL) {
    return NULL;
  }

  Musica* atual = cabecaPlaylist;
  do {
    if (strcmp(atual->titulo, titulo) == 0) { // Encontra a música com o título correspondente
      return atual; // Retorna o ponteiro para a música encontrada
    }
    atual = atual->prox; // Avança para a próxima música
  } while (atual != cabecaPlaylist);

  return NULL; // Retorna NULL se a música não for encontrada
}

// Função para carregar músicas de um arquivo (substitua "musicas.txt" pelo nome do seu arquivo)
void carregarMusicasDoArquivo(const char* nomeArquivo) {
  FILE* arquivo = fopen(nomeArquivo, "r"); // Abre o arquivo para leitura
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }

  char artista[char1], titulo[char1];
  while (fscanf(arquivo, "%[^;];%[^\n]\n", artista, titulo) == 2) { // Lê artista e título do arquivo
    adicionarMusica(artista, titulo); // Adiciona cada música lida à playlist
  }

  fclose(arquivo); // Fecha o arquivo
}

// Função para exibir o menu e realizar operações na playlist
void menu() {
  int escolha;
  char artista[char1], titulo[char1];
  Musica* atual = cabecaPlaylist;

  do {
    // Exibe as opções do menu
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
    getchar(); // Limpar o caractere de nova linha

    switch (escolha) {
      case 1:
        imprimirPlaylist(); // Chama a função para imprimir a playlist
        break;
      case 2:
        printf("Digite o nome do artista: ");
        fgets(artista, char1, stdin); // Lê o nome do artista do usuário
        strtok(artista, "\n"); // Remove o caractere de nova linha
        printf("Digite o título da música: ");
        fgets(titulo, char1, stdin); // Lê o título da música do usuário
        strtok(titulo, "\n"); // Remove o caractere de nova linha
        adicionarMusica(artista, titulo); // Adiciona a nova música à playlist
        break;
      case 3:
        printf("Digite o título da música a ser removida: ");
        fgets(titulo, char1, stdin); // Lê o título da música a ser removida
        strtok(titulo, "\n"); // Remove o caractere de nova linha
        removerMusica(titulo); // Remove a música da playlist
        break;
      case 4:
        printf("Digite o título da música a ser buscada: ");
        fgets(titulo, char1, stdin); // Lê o título da música a ser buscada
        strtok(titulo, "\n"); // Remove o caractere de nova linha
        Musica* encontrada = encontrarMusica(titulo); // Busca a música na playlist
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
