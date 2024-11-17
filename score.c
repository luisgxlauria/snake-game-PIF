#include <stdio.h>
#include "score.h"

void loadScores(int scores[MAX_SCORES]) {
    FILE* file = fopen("scores.txt", "r");
    if (!file) {
        for (int i = 0; i < MAX_SCORES; i++) {
            scores[i] = 0;
        }
    } else {
        for (int i = 0; i < MAX_SCORES; i++) {
            fscanf(file, "%d", &scores[i]);
        }
        fclose(file);
    }
}

void saveScore(int scores[MAX_SCORES], int score) {
    int i;
    for (i = 0; i < MAX_SCORES; i++) {
        if (score > scores[i]) {
            break;
        }
    }

    if (i < MAX_SCORES) {
        for (int j = MAX_SCORES - 1; j > i; j--) {
            scores[j] = scores[j - 1];
        }
        scores[i] = score;

        FILE* file = fopen("scores.txt", "w");
        if (file != NULL) {
            for (int k = 0; k < MAX_SCORES; k++) {
                fprintf(file, "%d\n", scores[k]);
            }
            fclose(file);
        }
    }
}

void displayScores(const int scores[MAX_SCORES]) {
    printf("\nTop Scores:\n");
    for (int i = 0; i < MAX_SCORES; i++) {
        printf("%d: %d\n", i + 1, scores[i]);
    }
}
