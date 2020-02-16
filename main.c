#include <stdio.h>
#include <stdlib.h>

int tryAt(int matrix[7][8], int pos, int player, int moves);
int hasWon(int matrix[7][8], int *x, int *y);
void check (int matrix[7][8], int player, int *count, int x, int y, int mX, int mY);
int insertInMatrix(int matrix[7][8], int pos, int player, int *x, int *y);
void removeFromMatrix(int matrix[7][8], int pos, int player);

int tryAt(int matrix[7][8], int pos, int player, int moves){
    /*player tries a move at pos*/
    if(moves == 0){
        return 0;
    }
    int x, y;
    if(insertInMatrix(matrix, pos, player, &x, &y)){
        if(hasWon(matrix, &x, &y) == 1){
            removeFromMatrix(matrix, pos, player);
            if(player == 1)
                return -1;
            else
                return 1;
        }
        else{
            int player2 = player == 2 ? 1 : 2, b, i;
            if(player == 1){
                b = -2;
                for(i = 0; i < 8; ++i){
                    int q = tryAt(matrix, i, player2, moves-1);
                    if(q > b){
                        b = q;
                    }
                }
            }
            if(player == 2){
                b = 2;
                for(i = 0; i < 8; ++i){
                    int q = tryAt(matrix, i, player2, moves-1);
                    if(q < b){
                        b = q;
                    }
                }
            }
            removeFromMatrix(matrix, pos, player);
            return b;
        }
    }
    else
        return 0;
}

int hasWon(int matrix[7][8], int *x, int *y){
    /* checks whether the new input at x, y has won the game for the player that made the move*/
    int player = matrix[*x][*y];
    int k1 = 0;//up
    check(matrix, player, &k1, *x,*y+1,   0,1);
    int k2 = 0;//up-right
    check(matrix, player, &k2, *x+1,*y+1, 1,1);
    int k3 = 0;//right
    check(matrix, player, &k3, *x+1,*y,   1,0);
    int k4 = 0;//down-right
    check(matrix, player, &k4, *x+1,*y-1, 1,-1);
    int k5 = 0;//down
    check(matrix, player, &k5, *x,*y-1,   0,-1);
    int k6 = 0;//down-left
    check(matrix, player, &k6, *x-1,*y-1, -1,-1);
    int k7 = 0;//left
    check(matrix, player, &k2, *x-1,*y,   -1,0);
    int k8 = 0;//left-up
    check(matrix, player, &k2, *x-1,*y+1, -1,1);
    if(k1+k5 >= 3 ||
       k2+k6 >= 3 ||
       k3+k7 >= 3 ||
       k4+k8 >= 3)
        return 1;
    return 0;
}

void check (int matrix[7][8], int player, int *count, int x, int y, int mX, int mY){
    if(x>=0 && x<=7 && y>=0 && y<=6){
        if(matrix[x][y] == player){
            *count = *count + 1;
            check(matrix, player, count, x+mX, y+mY, mX, mY);
        }
    }
}

int insertInMatrix(int matrix[7][8], int pos, int player, int *x, int *y){
    /*inserts a circle at the pos-column as player
    returns true if the move is possible
    */
    int p = 6, i;
    for(i = 0; i < 7; ++i){
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

void removeFromMatrix(int matrix[7][8], int pos, int player){
    /*removes a circle from the matrix from the player
    */
    int p = 6, i;
    for(i = 0; i < 7; ++i){
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
    int matrix[n][m], i, j;
    for(i = 0; i < n; ++i){
        for(j = 0; j < m; ++j){
            matrix[i][j] = 0;
        }
    }
    int x=0, y=0;
    while(1){
        int pos = 0;
        printf("insert 0-7\n");
        scanf("%d",&pos);
        insertInMatrix(matrix,pos,1,&x,&y);
        for(i = 0; i < 7; ++i){
            for(j = 0; j < 8; ++j){
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        int r = -1, t = 0;
        for(i = 0; i < m; ++i){
            int r1 = tryAt(matrix, i, 2, 8);
            printf("%d\n", r1);
            if(r1 > r){
                r = r1;
                t = i;
            }
        }
        insertInMatrix(matrix,t,2,&x,&y);
        for(i = 0; i < 7; ++i){
            for(j = 0; j < 8; ++j){
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
    return 0;
}
