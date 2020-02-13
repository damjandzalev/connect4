#include <stdio.h>
#include <stdlib.h>

int tryAt(int matrix[7][8],int n, int m, int pos, int player, int moves, int *k);
int hasWon(int matrix[7][8], int n, int m, int *x, int *y);
int insertInMatrix(int matrix[7][8], int n, int m, int pos, int player, int *x, int *y);
void removeFromMatrix(int matrix[7][8], int n, int m, int pos, int player);

int tryAt(int matrix[7][8],int n, int m, int pos, int player, int moves, int *k){
    /*player tries a move at pos*/
    if(moves == 0){
        return 0;
    }
    int x, y;
    if(insertInMatrix(matrix, n, m, pos, player, &x, &y)){
        *k = *k + 1;
        if(hasWon(matrix, n, m, &x, &y)){
            removeFromMatrix(matrix, n, m, pos, player);
            return 1;
        }
        else{
            int l = 0;
            l = l + tryAt(matrix, n, m, 0,player == 2 ? 1 : 2, moves-1, k);
            l = l + tryAt(matrix, n, m, 1,player == 2 ? 1 : 2, moves-1, k);
            l = l + tryAt(matrix, n, m, 2,player == 2 ? 1 : 2, moves-1, k);
            l = l + tryAt(matrix, n, m, 3,player == 2 ? 1 : 2, moves-1, k);
            l = l + tryAt(matrix, n, m, 4,player == 2 ? 1 : 2, moves-1, k);
            l = l + tryAt(matrix, n, m, 5,player == 2 ? 1 : 2, moves-1, k);
            l = l + tryAt(matrix, n, m, 6,player == 2 ? 1 : 2, moves-1, k);
            l = l + tryAt(matrix, n, m, 7,player == 2 ? 1 : 2, moves-1, k);
            removeFromMatrix(matrix, n, m, pos, player);
            return l;
        }
    }
    return 0;
}

int hasWon(int matrix[7][8], int n, int m, int *x, int *y){
    /* checks whether the new input at x, y has won the game for the player that made the move*/
    return 0;
}

int insertInMatrix(int matrix[7][8], int n, int m, int pos, int player, int *x, int *y){
    /*inserts a circle at the pos-column as player
    returns true if the move is possible
    */
    int p = 6;
    for(int i = 0; i < n; ++i){
        if(matrix[i][pos] != 0){
            p = i - 1;
            break;
        }
    }
    if(p == -1)
        return 0;
    else{
        matrix[p][pos] = player;
        *x = p;
        *y = pos;
        return 1;
    }
}

void removeFromMatrix(int matrix[7][8], int n, int m, int pos, int player){
    /*removes a circle from the matrix from the player
    */
    int p = 6;
    for(int i = 0; i < 7; ++i){
        if(matrix[i][pos] != 0){
            p = i;
            break;
        }
    }
    matrix[p][pos] = 0;
}

int main()
{
    //initialize
    int n=7, m=8;
    int matrix[n][m];
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m; ++j){
            matrix[i][j] = 0;
        }
    }
    int wins[8], games[8];
    double predicts[8];
    for(int i = 0; i < m; ++i){
        wins[i] = 0;
        games[i] = 0;
        predicts[i] = 0;
    }
    for(int i = 0; i < m; ++i){
        int k = 0;
        wins[i] = tryAt(matrix, n, m, i, 2, 9, &k);
        games[i] = k;
        predicts[i] = wins[i]/games[i];
        printf("%d %d\n", wins[i], games[i]);
    }
    for(int i = 0; i < 7; ++i){
        for(int j = 0; j < 8; ++j){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    return 0;
}
