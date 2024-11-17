#ifndef SCORE_H
#define SCORE_H

#define MAX_SCORES 5

void loadScores(int scores[MAX_SCORES]);
void saveScore(int scores[MAX_SCORES], int score);
void displayScores(const int scores[MAX_SCORES]);

#endif // SCORE_H
