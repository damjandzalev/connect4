
#include "LedControl.h"

#include "stdio.h"
#include "stdlib.h"


int matrix[8][8]={{8,0,0,0,0,0,0,0},{0},{0},{0},{0},{0},{0},{0}};
int i, j, x, y;
const int buttonLeft = 10;
const int buttonDrop = 11;
const int buttonRight = 12;
LedControl lc1=LedControl(2,0,1,1);

int buttonLeftState = 0;
int buttonDropState = 0;
int buttonRightState = 0;

int playerMove = 1;
int pos = 0;

byte lost[]={
  B10000000,
  B10000000,
  B10000000,
  B10000000,
  B10000000,
  B10000000,
  B10000000,
  B11111000,
  };

byte won[]={
  B10000001,
  B10000001,
  B10000001,
  B10000001,
  B10011001,
  B10100101,
  B10100101,
  B11000011,
  };
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
                    if(moves-1 == 0){break;}
                }
            }
            if(player == 2){
                b = 2;
                for(i = 0; i < 8; ++i){
                    int q = tryAt(matrix, i, player2, moves-1, win, lose, count);
                    if(q < b){
                        b = q;
                    }
                    if(moves-1 == 0){break;}
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
    if(player == 0 || player == 8 )
      return 0;
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
    for(i = 1; i < 7; ++i){
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

void reset(byte message[8]){
    
  for(i=0; i<8; i++){
    lc1.setRow(0,i,message[i]);
    }
      delay(3000);
  
  lc1.clearDisplay(0);
  for(int i=0; i<8; i++){
    for(int j=0; j<8; j++){
      if(i==0 and j==0){
        matrix[i][j]=8;
        //lc1.setLed(0,i,j,true);
      }
      else{
        matrix[i][j]=0;
        //lc1.setLed(0,i,j,false);
      }
    }
  }
  playerMove=1;
  pos=0;

  buttonLeftState = 0;
  buttonDropState = 0;
  buttonRightState = 0;
  }
  

void setup() {
    // put your setup code here, to run once:
    lc1.shutdown(0,false);
    pinMode(buttonRight, INPUT_PULLUP);
    pinMode(buttonDrop, INPUT_PULLUP);
    pinMode(buttonLeft, INPUT_PULLUP);
    randomSeed(analogRead(0));
}

void loop() {
    // put your main code here, to run repeatedly:
    if(playerMove == 1){
        if(digitalRead(buttonLeft) == HIGH){
            buttonLeftState = HIGH;
        }
        if(digitalRead(buttonRight) == HIGH){
            buttonRightState = HIGH;
        }
        if(digitalRead(buttonDrop) == HIGH){
            buttonDropState = HIGH;
        }
        if(digitalRead(buttonLeft) == LOW and buttonLeftState == HIGH){
            left(&pos, matrix);
            buttonLeftState = LOW;
        }
        if(digitalRead(buttonRight) == LOW and buttonRightState == HIGH){
            right(&pos, matrix);
            buttonRightState = LOW;
        }
        if(digitalRead(buttonDrop) == LOW and buttonDropState == HIGH){
            if(insertInMatrix(matrix,pos,1,&x,&y)==1){
              buttonDropState = LOW;
              playerMove = 2;
            }
        }
    }
    if(hasWon(matrix,&x, &y)){
        //player won
        reset(won);
    }
    if(playerMove == 2){
        int res[8], win[8], lose[8], games[8];
        lc1.clearDisplay(0);
        for(i = 0; i < 8; ++i){
            win[i] = 0;
            lose[i] = 0;
            games[i] = 0;
            if(matrix[1][i]==0)
                res[i] = tryAt(matrix, i, 2, 5, &win[i], &lose[i], &games[i]);
            else
                res[i] = -1;
            lc1.setLed(0,0,i,true);
        }
        double maks = -1;
        int r1 = -1, t = 0;
        for(i = 0; i < 8; ++i){
            if(res[i] == r1){
                if((double)win[i]/games[i] - (double)lose[i]/games[i] > maks){
                    t = i;
                    maks = (double)win[i]/games[i] - (double)lose[i]/games[i];
                }
                if((double)win[i]/games[i] - (double)lose[i]/games[i] == maks){
                    t = random(100)%2 == 0 ? t : i;
                }
            }
            if(res[i] > r1){
                t = i;
                r1 = res[i];
                maks = (double)win[i]/games[i] - (double)lose[i]/games[i];
            }
        }
        insertInMatrix(matrix,t,2,&x,&y);
        playerMove = 1;
    }
    
    if(hasWon(matrix,&x, &y)){
      //ai won
      reset(lost);      
    }
    lc1.clearDisplay(0);
    lc1.setIntensity(0, 0);
    for(i = 0; i < 8; ++i){
        for(j = 0; j < 8; ++j){
            if(matrix[i][j]==2){
                lc1.setLed(0,i,j,true);
            }
        }
    }
    lc1.clearDisplay(0);
    lc1.setIntensity(0, 15);
    for(i = 0; i < 8; ++i){
      for(j = 0; j < 8; ++j){
          if(matrix[i][j]==1){
            lc1.setLed(0,i,j,true);
          }
          if(matrix[i][j]==8){
            lc1.setLed(0,i,j,true);
          }
      }
    }
}
