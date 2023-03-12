/* TETRIS pour Mattel AQUARIUS
AQUATRIS
zcc +aquarius -clib=ansi -lm -create-app -subtype=rom -Cz--fast -o aquatris src\aquatris.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
char grille[11][21],texte[30];
signed char	joueurx,joueury;
unsigned char temp1,temp2;
int compteur,score,level,nbligne;
// definition tetros
typedef struct {char hauteur;char largeur;char couleur;char forme[4][4];} tetros;
tetros actuel = {0,0,1,{{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}};
tetros temporaire = {0,0,1,{{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}};
tetros prochain = {0,0,1,{{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}};
const tetros I = {1,4,6,{{1,1,1,1}}};
const tetros O = {2,2,3,{{1, 1},{1, 1}}};
const tetros T = {2,3,10,{{ 1, 1, 1},{ 0, 1, 0}}};
const tetros S = {2,3,2,{{ 0, 1, 1},{1, 1, 0}}};
const tetros Z = {2,3,1,{{1, 1, 0},{0, 1, 1}}};
const tetros L = {2,3,8,{{1, 1, 1},{ 1, 0, 0}}};
const tetros J = {2,3,4,{{ 1, 1, 1},{0, 0, 1}}};
// definition score
const int table_score[]={0,40,100,300,1200};
// ---------------------------- FONCTIONS-----------------------------------------------------------------------------------------
void poke( uint16_t addr, uint8_t value )// fonction poke
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
            signed int x,y;
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
// fonction pour afficher une chaine de caractere ASCII a une position et avec les couleurs.
void print(signed char x, signed char y, char texte[], unsigned char encre, unsigned char fond) {
    if (x < 0) {
        x = (40 - strlen(texte)) / 2;
    }
    for (char* c = texte; *c != '\0'; c++) {
        affchr(x, y, *c, encre, fond);
        x++;
    }
}

void colision(signed char xtest,signed char ytest) //test de colision
{
bool colision=false;
for (char y = 0; y < 4; y++) {
		for (char x = 0; x < 4; x++) {
		    if (y+ytest+4>3 && y+ytest+4<24 )
            {
                if (actuel.forme[y][x]==1 && grille[x+xtest][y+ytest]!=0) {colision=true;}
            }
            }
		}
return colision;
} 
void affiche_tetros() //affiche le tetrominos
{
    for (char y = 0; y < 4; y++) {
		for (char x = 0; x < 4; x++) {
		    if (y+joueury+4>3 && y+joueury+4<24 && x+joueurx+14>13 && x+joueurx+14<25)
            {
                if (actuel.forme[y][x]==1) {affchr(x+joueurx+14,y+joueury+4,196,15,actuel.couleur);}
            }
            }
		}
}
void efface(char haut,char bas,bool supprime)// effacement du tetros ou de la grille 
{
    if (!supprime){bas++;}
    for (char y = bas; y > haut; y--) {
		for (char x = 1; x < 11; x++) {
		    if (y+4>3 && y+4<24)
            {
                if (supprime){grille[x][y]=grille[x][y-1];}
                if (grille[x][y]!=0) {affchr(x+14,y+4,196,15,grille[x][y]);}
                else {affchr(x+14,y+4,32,15,0);}
            }
            }
		}
}
void rotate_tetros()// Rotation du tetromono actuel
{
    memcpy(&temporaire, &actuel, sizeof(tetros));
    char temp[4][4],temph,templ,tempc;
    // On effectue une rotation de 90 degrés en échangeant les valeurs dans le tableau temporaire
    for (char y = 0; y < actuel.hauteur; y++) {
        for (char x = 0; x < actuel.largeur; x++) {
            temp[x][(actuel.hauteur-1)-y] = actuel.forme[y][x];
        }
    }
    // memorisation temporaire des dimentions et couleur
    temph=actuel.hauteur;templ=actuel.largeur;tempc=actuel.couleur;
    memset(&actuel, 0, sizeof(tetros));//on efface tout dans actuel
    // rotation hauteur largeur et restauration couleur
    actuel.largeur=temph;actuel.hauteur=templ;actuel.couleur=tempc;

    // On copie les valeurs du tableau temporaire dans le tableau original
    for (char y = 0; y < 4; y++) {
        for (char x = 0; x < 4; x++) {
            if (temp[y][x]==1) actuel.forme[y][x] = 1;
        }
    }
    if (colision(joueurx,joueury)||joueury+actuel.hauteur>19||joueurx+actuel.largeur>11){memcpy(&actuel, &temporaire, sizeof(tetros));}
    else {efface(joueury-1,joueury+temporaire.hauteur-1,false);msleep(100);}    
}*/
void gene_prochain_teros() // chosit aleatoirement le prochaint tetrominos
{
    switch (rand() % 7)
    {
    case 0:
        memcpy(&prochain, &I, sizeof(tetros));break;
    
    case 1:
        memcpy(&prochain, &O, sizeof(tetros));break;
    case 2:
        memcpy(&prochain, &T, sizeof(tetros));break;
    case 3:
        memcpy(&prochain, &S, sizeof(tetros));break;
    case 4:
        memcpy(&prochain, &Z, sizeof(tetros));break;
    case 5:
        memcpy(&prochain, &L, sizeof(tetros));break;
    case 6:
        memcpy(&prochain, &J, sizeof(tetros));break;
    default:
        break;
    }
}
void changetetro()// spawn un nouveau tetrominos en haut de la zone de jeu
{   
    if (prochain.hauteur==0){gene_prochain_teros();}
    memcpy(&actuel, &prochain, sizeof(tetros));
    joueurx=((10-actuel.largeur)/2)+1 ,joueury=-1;
    gene_prochain_teros();
    for (char y = 0; y < 4; y++) {
		for (char x = 0; x < 4; x++) {
		        if (prochain.forme[y][x]==1) {affchr(x+32,y+2,196,15,prochain.couleur);}
                else {affchr(x+32,y+2,32,15,0);}
            }
            }
		}
void cadre(char xpos,char ypos,char largeur, char hauteur) // desine cadre pour titre et autre
{
    char txt[40];memset(txt, 0, sizeof(txt));
    memset(txt, 255, largeur-4);sprintf(texte,"%c%s%c", 203,txt,219);print(xpos+1,ypos,texte,14,0);print(xpos+1,(ypos+hauteur)-1,texte,14,0);
    memset(txt, 255, largeur-2);sprintf(texte,"%c%s%c", 203,txt,219);
    for (temp1=ypos+1;temp1!=(ypos+hauteur)-1;temp1++){print(xpos,temp1,texte,14,0);}

}
void dercojeu()//desine le decor du jeu.
{
    cls(208,15,0);
    ligne(14,4,14,23,195,0,14);ligne(25,4,25,23,151,14,0);
    cadre(13,0,14,4);
    print(-1,1,"AQUATRIS",7,14);print(-1,2,"2023",15,14);affchr(14,4,142,14,0);affchr(25,4,158,14,0);
    for (temp1=4;temp1!=24;temp1++){print(-1,temp1,"          ",14,0);}
    cadre(30,0,8,8);
    //ligne(32,1,35,1,163,14,0);ligne(32,6,35,6,240,14,0);
    //ligne(31,2,31,5,181,14,0);ligne(36,2,36,5,234,14,0);affchr(31,1,183,14,0);affchr(36,1,235,14,0);affchr(31,6,245,14,0);affchr(36,6,250,14,0);
    print(32,1,"NEXT",7,14);
    cadre(29,10,10,3);print(31,10,"SCORE:",7,14);
    cadre(29,15,10,3);print(31,15,"LEVEL:",7,14);
    cadre(30,20,8,3);print(32,20,"LINE",7,14);
    print(3,22,"POSSAMAI",15,0);print(2,23,"Christophe",15,0);
}
void pose_tetros() //affiche le tetrominos
{
    for (char y = 0; y < 4; y++) {
		for (char x = 0; x < 4; x++) {
		    if (y+joueury+4>3 && y+joueury+4<24)
            {
                if (actuel.forme[y][x]==1) {grille[x+joueurx][y+joueury]=actuel.couleur;}
            }
            }
		}
}
void aff_score(unsigned char ligne)// calcul le score et affiche score et level
{
    if(ligne>0){score=score+((table_score[ligne]*level));}
    while (nbligne >= level * 10) {level++;}
    sprintf(texte,"%06d", score);print(31,11,texte,7,0);
    sprintf(texte,"%04d", level-1);print(32,16,texte,7,0);
    sprintf(texte,"%04d", nbligne);print(32,21,texte,7,0);
    
}
void verif_ligne()// verifier si une ou plusieur ligne son complete
{
    temp1=0;//temp1 = nombre de case occuper par ligne
    temp2=0;//temp2 = nombre de ligne complete
    for (char y = joueury; y < joueury+actuel.hauteur; y++) {
		for (char x = 0; x < 11; x++) {if (grille[x][y]!=0) {temp1++;}}
            if (temp1>=10) {temp2++;efface(1,y,true);}
            temp1=0;
		}
    // score selon le nombre de ligne
    nbligne=nbligne+temp2;aff_score(temp2);
}
void main(){
debut:
char key;

//MENU
dercojeu();
print(-1,12,"Ready?",3,0);
while (1)
{
	key = getk();compteur++;
	if (key!=0) {break;}
}
// debut partie
srand(compteur);compteur=0;score=0;level=1;nbligne=0;
memset(&grille, 0, sizeof(grille));//on efface tout dans grille
dercojeu();aff_score(0);
changetetro();
while (1)
{
	key = getk();compteur++; 
	if (compteur > 37 - (2 * level) | key=='v') {efface(joueury-1,joueury+actuel.hauteur-1,false);joueury++;compteur=0;}
    
	if (key=='l') {rotate_tetros();}
    if (key=='c' && joueurx>1 && !colision(joueurx-1,joueury)) {efface(joueury-1,joueury+actuel.hauteur-1,false);joueurx--;}
    if (key=='b' && joueurx<(11-actuel.largeur) && !colision(joueurx+1,joueury)) {efface(joueury-1,joueury+actuel.hauteur-1,false);joueurx++;}
    affiche_tetros();
    if (key=='p') {msleep(200);fgetc_cons();}
    if (joueury>=20-actuel.hauteur || colision(joueurx,joueury+1)) {
        if (joueury<=0) {break;}
        pose_tetros();verif_ligne();changetetro();}
}
//fin
msleep(500);
print(15,14,"GAME  OVER",1,0);    
fgetc_cons();msleep(500); // attente touche
goto debut;

}