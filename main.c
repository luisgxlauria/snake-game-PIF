#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "keyboard.h"
#include "screen.h"
#include "timer.h"
#include "score.h"

#define COLUNAS 40
#define LINHAS 19
#define MAX_SCORES 5
#define INCREMENTO_VELOCIDADE 5
#define NUM_OBSTACULOS 5

typedef struct {
    int x, y;
} Coordenada;

typedef struct {
    Coordenada *posicao;
    int tamanho;
    int velocidade;
} Cobra;

typedef struct {
    Coordenada posicao;
} Food;

Cobra cobra;
Food food;
Coordenada obstaculos[NUM_OBSTACULOS];
int sair = 0, fimDoJogo = 0, pausado = 0;
int direcaox = 1, direcaoy = 0;
int scores[MAX_SCORES];
int tipoComida;

void initializeGame();
void drawBorders();
void drawGame();
void updateGame();
void generateFood();
void generateObstacles();
void endGame();
void showScoreAndHints();
void drawFood();

void generateFood() {
    int valid;
    do {
        valid = 1;
        food.posicao.x = rand() % COLUNAS;
        food.posicao.y = rand() % LINHAS;
        tipoComida = rand() % 3;  // Diferentes tipos de comida

        for (int i = 0; i < cobra.tamanho; i++) {
            if (cobra.posicao[i].x == food.posicao.x && cobra.posicao[i].y == food.posicao.y) {
                valid = 0;
                break;
            }
        }
    } while (valid == 0);
}

void initializeGame() {
    screenHideCursor();
    keyboardInit();
    timerInit(55);
    loadScores(scores);  // Carrega as pontuações existentes

    cobra.tamanho = 1;
    cobra.velocidade = 55;
    cobra.posicao = (Coordenada *)malloc(cobra.tamanho * sizeof(Coordenada));
    if (cobra.posicao == NULL) {
        printf("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    cobra.posicao[0].x = COLUNAS / 2;
    cobra.posicao[0].y = LINHAS / 2;

    generateObstacles();
    drawBorders();  // Desenha as bordas apenas uma vez
    generateFood(); // Gera a comida inicial
}

void generateObstacles() {
    for (int i = 0; i < NUM_OBSTACULOS; i++) {
        obstaculos[i].x = rand() % COLUNAS;
        obstaculos[i].y = rand() % LINHAS;
    }
}

void drawBorders() {
    screenClear();  // Limpa a tela apenas uma vez no início
    printf("┌");
    for (int i = 0; i < COLUNAS; i++)
        printf("─");
    printf("┐\n");
    for (int j = 0; j < LINHAS; j++) {
        printf("│");
        for (int i = 0; i < COLUNAS; i++)
            printf(" ");
        printf("│\n");
    }
    printf("└");
    for (int i = 0; i < COLUNAS; i++)
        printf("─");
    printf("┘\n");

    showScoreAndHints();
}

void showScoreAndHints() {
    screenGotoxy(0, LINHAS + 1);
    printf("Pontuação: %d | Use 'WASD' para mover, 'Q' para sair, 'P' para pausar", cobra.tamanho - 1);
}

void drawFood() {
    screenGotoxy(food.posicao.x, food.posicao.y);
    switch (tipoComida) {
        case 0:
            screenSetColor(RED, BLACK);
            printf("♥"); // Comida tipo 1
            break;
        case 1:
            screenSetColor(YELLOW, BLACK);
            printf("♦"); // Comida tipo 2
            break;
        case 2:
            screenSetColor(MAGENTA, BLACK);
            printf("♣"); // Comida tipo 3
            break;
    }
}

void drawGame() {
    // Desenha a comida na posição atual
    drawFood();

    // Apaga a última posição da cauda da cobra
    screenGotoxy(cobra.posicao[cobra.tamanho - 1].x, cobra.posicao[cobra.tamanho - 1].y);
    printf(" ");

    // Desenhar a cabeça da cobra
    screenSetColor(LIGHTGREEN, BLACK);
    screenGotoxy(cobra.posicao[0].x, cobra.posicao[0].y);
    printf("■");

    // Desenhar o corpo da cobra
    screenSetColor(GREEN, BLACK);
    for (int i = 1; i < cobra.tamanho; i++) {
        screenGotoxy(cobra.posicao[i].x, cobra.posicao[i].y);
        printf("▒");
    }

    screenSetNormal();
    screenUpdate();
}

void updateGame() {
    if (fimDoJogo || pausado) return;

    // Calcule a nova posição da cabeça da cobra
    int newX = cobra.posicao[0].x + direcaox;
    int newY = cobra.posicao[0].y + direcaoy;

    // Verificar colisão com as bordas do campo de jogo
    if (newX < 0 || newX >= COLUNAS || newY < 0 || newY >= LINHAS) {
        fimDoJogo = 1;
        endGame();
        return;
    }

    // Verificar colisão com os obstáculos
    for (int i = 0; i < NUM_OBSTACULOS; i++) {
        if (newX == obstaculos[i].x && newY == obstaculos[i].y) {
            fimDoJogo = 1;
            endGame();
            return;
        }
    }

    // Verificar colisão com o próprio corpo
    for (int i = 1; i < cobra.tamanho; i++) {
        if (cobra.posicao[i].x == newX && cobra.posicao[i].y == newY) {
            fimDoJogo = 1;
            endGame();
            return;
        }
    }

    // Verificar se a cobra comeu a comida
    if (newX == food.posicao.x && newY == food.posicao.y) {
        // Aumenta o tamanho da cobra e gera nova comida
        cobra.tamanho++;
        cobra.posicao = (Coordenada *)realloc(cobra.posicao, cobra.tamanho * sizeof(Coordenada));
        if (cobra.posicao == NULL) {
            printf("Erro ao realocar memória");
            exit(EXIT_FAILURE);
        }
        generateFood();

        // Aumenta a velocidade da cobra a cada incremento
        if (cobra.tamanho % INCREMENTO_VELOCIDADE == 0) {
            cobra.velocidade -= 5;
            timerInit(cobra.velocidade);
        }
    } else {
        // Apaga a última posição da cauda quando a cobra se move
        screenGotoxy(cobra.posicao[cobra.tamanho - 1].x, cobra.posicao[cobra.tamanho - 1].y);
        printf(" ");
    }

    // Mover o corpo da cobra para a nova posição
    for (int i = cobra.tamanho - 1; i > 0; i--) {
        cobra.posicao[i] = cobra.posicao[i - 1];
    }

    // Atualizar a cabeça da cobra para a nova posição
    cobra.posicao[0].x = newX;
    cobra.posicao[0].y = newY;
}

void endGame() {
    screenGotoxy(COLUNAS / 2 - 5, LINHAS / 2);
    printf("Fim do Jogo! Pontuação: %d ponto(s)\n", cobra.tamanho - 1);
    screenUpdate();
    saveScore(scores, cobra.tamanho - 1);
    displayScores(scores);
}

int main() {
    initializeGame();
    drawGame();

    while (!sair) {
        if (timerTimeOver()) {
            if (keyhit()) {
                int tecla = readch();
                if (tecla == 27 || tecla == 'q') {
                    sair = 1;
                } else if (tecla == 'p') {
                    pausado = !pausado;
                } else if (tecla == 'w' && direcaoy != 1) {
                    direcaox = 0;
                    direcaoy = -1;
                } else if (tecla == 'a' && direcaox != 1) {
                    direcaox = -1;
                    direcaoy = 0;
                } else if (tecla == 's' && direcaoy != -1) {
                    direcaox = 0;
                    direcaoy = 1;
                } else if (tecla == 'd' && direcaox != -1) {
                    direcaox = 1;
                    direcaoy = 0;
                }
            }
            updateGame();
            drawGame();
        }
    }

    free(cobra.posicao);
    screenShowCursor();
    keyboardDestroy();
    timerDestroy();
    return 0;
}
