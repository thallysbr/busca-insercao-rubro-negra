#include <stdio.h>
#include <stdlib.h>

// define as cores para os nós (int vermelho = 1, preto = 0)
#define RED 1
#define BLACK 0

// estrutura do nó da arvore rubro negra
typedef struct No {
  int valor;
  int cor; // cor do nó (RED ou BLACK)
  struct No *esquerda, *direita, *pai;
} No;

// cria o no com valor, cor, filhos e pai
No *criarNo(int valor, int cor, No *pai) {
  No *novo = (No *)malloc(sizeof(No));
  novo->valor = valor;
  novo->cor = cor;
  novo->esquerda = novo->direita = NULL;
  novo->pai = pai;
  return novo;
}

// fazer busca na arvore
No *buscar(No *arvore, int valor) {
  // enquanto raiz != NULL e valor != valor da raiz da arvore
  while (arvore != NULL && valor != arvore->valor) {
    // se valor menor que valor da raiz da arvore
    if (valor < arvore->valor) {
      arvore = arvore->esquerda;
    } else {
      arvore = arvore->direita;
    }
  }
  return arvore;
}

// função rotação esquerda (faz a rotação a esquerda para o nó x)
void rotacaoEsquerda(No **raiz, No *pivo) {

  // define noDireita como filho direito de pivo
  No *noDireita = pivo->direita;

  // define o filho direito de pivo como filho esquerdo de noDireita
  pivo->direita = noDireita->esquerda;

  // se filho esquerdo de noDireita != NULL define o pai como pivo
  if (noDireita->esquerda != NULL) {
    noDireita->esquerda->pai = pivo;
  }

  // define o pai de noDireita como pai de pivo
  noDireita->pai = pivo->pai;

  // se pai de pivo == NULL, define a raiz como noDireita
  if (pivo->pai == NULL) {
    *raiz = noDireita;
  }

  // se não, se pivo for filho esquerdo do seu pai, define como noDireita
  else if (pivo == pivo->pai->esquerda) {
    pivo->pai->esquerda = noDireita;
  }

  // se pivo for filho direito do seu pai, define como noDireita
  else {
    pivo->pai->direita = noDireita;
  }

  // define o filho esquerdo de noDireita como pivo
  noDireita->esquerda = pivo;

  // define o pai de pivo como noDireita
  pivo->pai = noDireita;
}

// função rotação direita
void rotacaoDireita(No **raiz, No *noAlterar) {
  // define noEsquerda como filho esquerdo de noAlterar
  No *noEsquerda = noAlterar->esquerda;

  // define filho esquerdo de noAlterar como filho direito de noEsquerda
  noAlterar->esquerda = noEsquerda->direita;

  // se filho direito de noEsquerda != NULL, define o pai do filho direito de
  // noEsquerda como noAlterar
  if (noEsquerda->direita != NULL) {
    noEsquerda->direita->pai = noAlterar;
  }

  // define pai de noEsquerda como pai de noAlterar
  noEsquerda->pai = noAlterar->pai;

  // se pai de noAlterar == NULL, define a raiz como noEsquerda
  if (noAlterar->pai == NULL) {
    *raiz = noEsquerda;
  }

  // se noAlterar é filho esquerdo do pai dele, define como noEsquerda
  else if (noAlterar == noAlterar->pai->esquerda) {
    noAlterar->pai->esquerda = noEsquerda;
  }
  // se noAlterar é filho direito do pai dele, define como noEsquerda
  else {
    noAlterar->pai->direita = noEsquerda;
  }

  // define filho direito de noEsquerda como noAlterar
  noEsquerda->direita = noAlterar;

  // define pai de noAlterar como noEsquerda
  noAlterar->pai = noEsquerda;
}

void correcao(No **raiz, No *novoNo) {
  // se o pai do no A é vermelho viola a regra
  while (novoNo != *raiz && novoNo->pai->cor == RED) {

    // se o pai do novoNo for igual ao filho esquerdo do vo do novo nó
    if (novoNo->pai == novoNo->pai->pai->esquerda) {

      // tio é definido como o tio do novo nó (filho à direita do vo do novo
      // nó)
      No *tio = novoNo->pai->pai->direita;

      // se tio é vermelho
      if (tio != NULL && tio->cor == RED) {
        // corrige
        novoNo->pai->cor = BLACK;
        tio->cor = BLACK;
        novoNo->pai->pai->cor = RED;
        // define novoNo no topo da arvore
        novoNo = novoNo->pai->pai;
      } else {
        // se novo no é filho direito
        if (novoNo == novoNo->pai->direita) {
          novoNo = novoNo->pai;
          rotacaoEsquerda(raiz, novoNo);
        }
        // se novo no é filho esquerdo
        novoNo->pai->cor = BLACK;
        novoNo->pai->pai->cor = RED;
        rotacaoDireita(raiz, novoNo->pai->pai);
      }
    } else {
      // define tio como filho esquerdo do vo de novo no
      No *tio = novoNo->pai->pai->esquerda;

      // se o tio é vermelho
      if (tio != NULL && tio->cor == RED) {
        novoNo->pai->cor = BLACK;
        tio->cor = BLACK;
        novoNo->pai->pai->cor = RED;
        novoNo = novoNo->pai->pai;
      } else {
        // se novo no é filho esquerdo
        if (novoNo == novoNo->pai->esquerda) {
          novoNo = novoNo->pai;
          rotacaoDireita(raiz, novoNo);
        }
        novoNo->pai->cor = BLACK;
        novoNo->pai->pai->cor = RED;
        // faz rotação a esquerda no vo do novo no
        rotacaoEsquerda(raiz, novoNo->pai->pai);
      }
    }
  }
  // raiz da arvore é preta
  (*raiz)->cor = BLACK;
}

// Insere o novo nó na arvore
void inserir(No **raiz, int valor) {
  // define um novo nó com o valor informado, cor padrão vermelha e sem pai
  No *novoNo = criarNo(valor, 1, NULL);

  // se a arvore for vazia o novo nó e a raiz
  if (*raiz == NULL) {
    *raiz = novoNo;
  } else {
    // define o nó pai como nulo
    No *pai = NULL;

    // define atual como raiz da arvore
    No *atual = *raiz;

    // encontra a posição do novo nó com um while na arvore
    while (atual != NULL) {
      // atualiza o nó pai para o atual
      pai = atual;
      // vai para esquerda ou direita de acordo com valor
      if (novoNo->valor < atual->valor) {
        atual = atual->esquerda;
      } else {
        atual = atual->direita;
      }
    }

    /// define o pai do novo nó = pai
    novoNo->pai = pai;

    // define se o nó é fihlo esquerdo ou direito do pai dele
    if (novoNo->valor < pai->valor) {
      pai->esquerda = novoNo;
    } else {
      pai->direita = novoNo;
    }
  }

  // corrige violações
  correcao(raiz, novoNo);
}

int main() {
  No *arvore = NULL;

  // passa o endereço da arvore e o valor para adicionar na arvore
  inserir(&arvore, 60);
  inserir(&arvore, 36);
  inserir(&arvore, 49);
  inserir(&arvore, 32);
  inserir(&arvore, 76);

  // seta variavel para o resultado da busca e faz a busca
  No *resultado = buscar(arvore, 49);

  if (resultado) {
    printf("BUSCA NA ARVORE\n");
    printf("Valor encontrado: %d\n", resultado->valor);
    printf("Cor: %s (%d)", resultado->cor == 1 ? "RED" : "BLACK",
           resultado->cor);
  } else {
    printf("Valor não encontrado na arvore");
  }

  return 0;
}