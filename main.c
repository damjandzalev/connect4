#include <stdio.h>
#include <stdlib.h>

int tryAt(int matrix[8][8], int pos, int player, int moves, int *win, int *lose, int *count);
int hasWon(int matrix[8][8], int *x, int *y);
void check (int matrix[8][8], int player, int *count, int x, int y, int mX, int mY);
int insertInMatrix(int matrix[8][8], int pos, int player, int *x, int *y);
void removeFromMatrix(int matrix[8][8], int pos);

int tryAt(int matrix[8][8], int pos, int player, int moves, int *win, int *lose, int *count){
    /*player tries a move at pos
    pos - where to try a move
    player - which player tries a move
    moves - how many moves are left to try in the search
    *win - number of wins at top of the tree
    *lose - number of losses at top of the tree
    *count - number of games simulated
    */
    if(moves == 0){
        *count = *count+1;
        return 0;
    }
    int x, y;
    if(insertInMatrix(matrix, pos, player, &x, &y)){
        if(hasWon(matrix, &x, &y) == 1){
            *count = *count+1;
            removeFromMatrix(matrix, pos);
            if(player == 1){
                *lose = *lose+1;
                return -1;
            }
            else{
                *win = *win+1;
                return 1;
            }
        }
        else{
            int player2 = player == 2 ? 1 : 2, b, i;
            if(player == 1){
                b = -2;
                for(i = 0; i < 8; ++i){
                    int q = tryAt(matrix, i, player2, moves-1, win, lose, count);
                    if(q > b){
                        b = q;
                    }
                }
            }
            if(player == 2){
                b = 2;
                for(i = 0; i < 8; ++i){
                    int q = tryAt(matrix, i, player2, moves-1, win, lose, count);
                    if(q < b){
                        b = q;
                    }
                }
            }
            removeFromMatrix(matrix, pos);
            return b;
        }
    }
    else
        return 0;
}

int hasWon(int matrix[8][8], int *x, int *y){
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
    check(matrix, player, &k7, *x-1,*y,   -1,0);
    int k8 = 0;//left-up
    check(matrix, player, &k8, *x-1,*y+1, -1,1);
    if(k1+k5 >= 3 ||
       k2+k6 >= 3 ||
       k3+k7 >= 3 ||
       k4+k8 >= 3)
        return 1;
    return 0;
}

void check (int matrix[8][8], int player, int *count, int x, int y, int mX, int mY){
    if(x>=0 && x<=7 && y>=0 && y<=7){
        if(matrix[x][y] == player){
            *count = *count + 1;
            check(matrix, player, count, x+mX, y+mY, mX, mY);
        }
    }
}

int insertInMatrix(int matrix[8][8], int pos, int player, int *x, int *y){
    /*inserts a circle at the pos-column as player
    returns 1 if the move is possible, 0 if not
    */
    int p = 7, i;
    for(i = 1; i < 8; ++i){
        if(matrix[i][pos] != 0){
            p = i - 1;
            break;
        }
    }
    if(p == 0)
        return 0;
    else{
        matrix[p][pos] = player;
        *x = p;
        *y = pos;
        return 1;
    }
}

void removeFromMatrix(int matrix[8][8], int pos){
    /*removes a circle from the matrix at pos column
    */
    int p = 7, i;
    for(i = 1; i < 8; ++i){
        if(matrix[i][pos] != 0){
            p = i;
            break;
        }
    }
    matrix[p][pos] = 0;
}

void left(int *pos, int matrix[8][8]){
    matrix[0][*pos] = 0;
    *pos = *pos - 1;
    if(*pos == -1)
        *pos = 7;
    matrix[0][*pos] = 8;
}

void right(int *pos, int matrix[8][8]){
    matrix[0][*pos] = 0;
    *pos = *pos + 1;
    if(*pos == 8)
        *pos = 0;
    matrix[0][*pos] = 8;
}

int main()
{
    int matrix[8][8], i, j;
    for(i = 0; i < 8; ++i){
        for(j = 0; j < 8; ++j){
            matrix[i][j] = 0;
        }
    }
    int x=0, y=0;
    while(1){
        int pos = 0;
        matrix[0][0] = 8;
        char nextMove = '-';
        while(1){
            printf("insert a - left, s - drop, d - right\n");
            for(i = 0; i < 8; ++i){
                for(j = 0; j < 8; ++j){
                    printf("%d ", matrix[i][j]);
                }
                printf("\n");
            }
            printf("\n");
            while(nextMove!='d'&&nextMove!='a'&&nextMove!='s'){
                scanf("%c",&nextMove);
            }
            if(nextMove == 'a')
                left(&pos, matrix);
            if(nextMove == 'd')
                right(&pos, matrix);
            if(nextMove == 's'){
                if(insertInMatrix(matrix,pos,1,&x,&y)==1){
                    removeFromMatrix(matrix,pos);
                    matrix[0][pos] = 0;
                    break;
                }
            }
            nextMove = '-';
        }
        insertInMatrix(matrix,pos,1,&x,&y);
        for(i = 0; i < 8; ++i){
            for(j = 0; j < 8; ++j){
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        if(hasWon(matrix,&x, &y)){
            printf("\nplayer wins\n");
            break;
        }
        int res[8], win[8], lose[8], games[8];
        for(i = 0; i < 8; ++i){
            win[i] = 0;
            lose[i] = 0;
            games[i] = 0;
            if(matrix[1][i]==0)
                res[i] = tryAt(matrix, i, 2, 7, &win[i], &lose[i], &games[i]);
            else
                res[i] = -1;
        }
        double maks = -1;
        int r1 = -1, t = 0;
        for(i = 0; i < 8; ++i){
            if(res[i] == r1){
                if((double)win[i]/games[i] - (double)lose[i]/games[i] > maks){
                    t = i;
                    r1 = res[i];
                    maks = (double)win[i]/games[i] - (double)lose[i]/games[i];
                }
            }
            if(res[i] > r1){
                t = i;
                r1 = res[i];
                maks = (double)win[i]/games[i] - (double)lose[i]/games[i];
            }
        }
        insertInMatrix(matrix,t,2,&x,&y);
        printf("\nmaks:%f, pos:%d \n", maks, t);
        for(i = 0; i < 8; ++i){
            for(j = 0; j < 8; ++j){
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        if(hasWon(matrix,&x, &y)){
            printf("\nai wins\n");
            break;
        }
    }
}
