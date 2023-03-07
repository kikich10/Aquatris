/* TETRIS pour Mattel AQUARIUS
zcc +aquarius -clib=ansi -lm -create-app -subtype=rom -Cz--audio  -Cz--fast -DSOUND -o aquatris src\aquatris.c
block / carartere / encre / fond
L	  / 196       / 1     / 13
""__  / 135       / 1     / 4	
zone de jeu 10 x 18



*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include <sound.h>

char Table[20][15] = {0};
unsigned char temp1,temp2;

char texte[30];
int tetrominoactuel[4][4];
const int tetromino_I[4][4] = {
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

const int tetromino_O[4][4] = {
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0}
};

const int tetromino_T[4][4] = {
    {0, 0, 0, 0},
    {0, 1, 1, 1},
    {0, 0, 1, 0},
    {0, 0, 0, 0}
};

const int tetromino_S[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 1, 1},
    {0, 1, 1, 0},
    {0, 0, 0, 0}
};

const int tetromino_Z[4][4] = {
    {0, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0}
};

const int tetromino_L[4][4] = {
    {0, 0, 0, 0},
    {0, 1, 1, 1},
    {0, 1, 0, 0},
    {0, 0, 0, 0}
};

const int tetromino_J[4][4] = {
    {0, 0, 0, 0},
    {0, 1, 1, 1},
    {0, 0, 0, 1},
    {0, 0, 0, 0}
};

// ---------------------------- FONCTIONS-----------------------------------------------------------------------------------------
// fonction poke
void poke( uint16_t addr, uint8_t value )
        {
                *((uint8_t*)addr) = value ;
        }
void cls (unsigned char caractere,unsigned char encre,unsigned char fond)
{   
    temp1=caractere;
    temp2=encre*16 + fond;
	#asm
    ld hl, _temp1
    ld b, (hl)
    ld hl, $3000
    call $1e59
    ld hl, _temp2
    ld b, (hl)
    ld hl, $3400
    call $1e59
    #endasm
}
// fonction pour afficher un caractere ASCII a une position et avec les couleurs.
void affchr(unsigned char x,unsigned char y,unsigned char caractere,unsigned char encre,unsigned char fond)
        {
                poke(12328+(y*40)+x,caractere);// affiche caractere 
                poke(12328  + 1024 + (y*40)+x ,  encre*16 + fond); // definit les couleurs

        }
// fonction pour afficher une ligne du caractere ASCII a une position et avec les couleurs.
void ligne(unsigned char xa,char ya,unsigned char xb, char yb,unsigned char caractere,unsigned char encre,unsigned char fond)
        {
            signed char x,y;
			x=xa;y=ya;
			while (1)
			{
				affchr(x,y,caractere,encre,fond);
				if (x==xb && y==yb){break;}
				if (xb-xa<0) {x--;}
				if (xb-xa>0) {x++;} 
				if (yb-ya<0) {y--;}
				if (yb-ya>0) {y++;}
			}
			
        }
// fonction pour afficheret repeter un caractere ASCII a une position et avec les couleurs.
void print(unsigned char x,unsigned char y,char texte[],unsigned char encre,unsigned char fond)
        {
		unsigned char i=0;
		do
			{
				affchr(x+i,y,texte[i],encre,fond);
				i++;
			} while (texte[i]!=0);
		}
void rotate_tetromino(int tetromino[4][4])
{
    int temp[4][4];

    // On effectue une rotation de 90 degrés en échangeant les valeurs dans le tableau temporaire
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[j][3 - i] = tetromino[i][j];
        }
    }

    // On copie les valeurs du tableau temporaire dans le tableau original
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            tetromino[i][j] = temp[i][j];
        }
    }
}
void changetetro(char tetronimos)// Copie du tetromino source dans le tetronimo actuel
{
    switch (tetronimos)
    {
    case 0:
        memcpy(tetrominoactuel, tetromino_I, sizeof(tetromino_I));
        break;
    
    case 1:
        memcpy(tetrominoactuel, tetromino_O, sizeof(tetromino_O));
        break;
    case 2:
        memcpy(tetrominoactuel, tetromino_T, sizeof(tetromino_T));
        break;
    case 3:
        memcpy(tetrominoactuel, tetromino_S, sizeof(tetromino_S));
        break;
    case 4:
        memcpy(tetrominoactuel, tetromino_Z, sizeof(tetromino_Z));
        break;
    case 5:
        memcpy(tetrominoactuel, tetromino_L, sizeof(tetromino_L));
        break;
    case 6:
        memcpy(tetrominoactuel, tetromino_J, sizeof(tetromino_J));
        break;
    }
    
}

void main(){
char x, y,key,i;
cls(32,1,0);

while (1)
{
	/* code */
	key = getk();
	if (key=='r') {rotate_tetromino(tetrominoactuel);msleep(100);}
    if (key=='t') {i++;if (i==7){i=0;}changetetro(i);msleep(100);}
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
		if (tetrominoactuel[x][y]==1) {affchr(x,y,135,1,4);} else {affchr(x,y,32,1,0);}
		}
		
	}
}

	
}