#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "stos.h"

int kolej = 0; //0-3
int gracze = 0;
bool wygrana = false;
char** talia;
char** nowa_talia;
char** nick;
pthread_mutex_t talia_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ruch_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t desc_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t start_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t start_cond = PTHREAD_COND_INITIALIZER;
struct karta* dobierane=NULL;
struct karta* zagrane=NULL; 
bool ruch_jak_zegar = true;
bool mode = false;
int dobieranie = 1;
bool zagrana = false;
bool uno = false;
int uno_gracz;
int tab_desc[4];
int ile_na_rece[4] = {7, 7, 7, 7};
int czyja_kolej;
char kolor[2];
int sprawdzenie;
int rozmiar_zagrane=0;
int rozmiar_dobierane=0;
char nic[2];


struct thread_data_t
{
    int my_socket;
    int my_turn;
};


/*void bubblesort(int table[], int size)
{
	int i, j, temp;
	for (i = 0; i<size-1; i++)
        {
		for (j=0; j<size-1-i; j++)
		{
			if (table[j] > table[j+1])
			{
				temp = table[j+1];
				table[j+1] = table[j];
				table[j] = temp;
			}
		}
        }
}*/
void nowe_tasowanie (int rozmiar)
{
    int ilosc = rozmiar;
    int T[ilosc];
    int T2[ilosc];
    srand(time(0));
    for (int i=0; i<ilosc; i++)
    {
        T[i]=rand()%10000;
        T2[i]=T[i];
    }
    //bubblesort(T, ilosc);
    int temp1;
    for (int i = 0; i<ilosc-1; i++)
        {
		for (int j=0; j<ilosc-1-i; j++)
		{
			if (T[j] > T[j+1])
			{
				temp1 = T[j+1];
				T[j+1] = T[j];
				T[j] = temp1;
			}
		}
        }
    char* temp2=malloc(3*sizeof(char));
    for (int j=0;j<ilosc;j++)
    {
        for (int k=0; k<ilosc;k++)
        {
            if (T[j]==T2[k])
            {
                strcpy(temp2, nowa_talia[j]);
                strcpy(nowa_talia[j], nowa_talia[k]);
                strcpy(nowa_talia[k], temp2);
                break;
            }
        }
    }
    free(temp2);
}
void tasowanie (int rozmiar)
{
    int ilosc = rozmiar;
    int T[ilosc];
    int T2[ilosc];
    srand(time(0));
    for (int i=0; i<ilosc; i++)
    {
        T[i]=rand()%10000;
        T2[i]=T[i];
    }
    //bubblesort(T, ilosc);
    int temp1;
    for (int i = 0; i<ilosc-1; i++)
        {
		for (int j=0; j<ilosc-1-i; j++)
		{
			if (T[j] > T[j+1])
			{
				temp1 = T[j+1];
				T[j+1] = T[j];
				T[j] = temp1;
			}
		}
        }
    char* temp=malloc(3*sizeof(char));
    for (int j=0;j<ilosc;j++)
    {
        for (int k=0; k<ilosc;k++)
        {
            if (T[j]==T2[k])
            {
                strcpy(temp, talia[j]);
                strcpy(talia[j], talia[k]);
                strcpy(talia[k], temp);
                break;
            }
        }
    }
    free(temp);
}

void wysylanie_komunikatu(char* komunikat)
{
    for (int i=0; i<4; i++)
    {
        write(tab_desc[i], komunikat, strlen(komunikat));
    }
}

void przygotowanie_talii ()
{
    talia = malloc(68*sizeof(char*));
    for (int i=0; i<68;i++)
    {
        talia[i]=malloc(3*sizeof(char));
    }
    strcpy(talia[0], "y00");
    strcpy(talia[1], "y01");
    strcpy(talia[2], "y02");
    strcpy(talia[3], "y03");
    strcpy(talia[4], "y04");
    strcpy(talia[5], "y05");
    strcpy(talia[6], "y06");
    strcpy(talia[7], "y07");
    strcpy(talia[8], "y08");
    strcpy(talia[9], "y09");
    strcpy(talia[10], "y10");
    strcpy(talia[11], "y10"); //->zmiana kolejnosci ruchu
    strcpy(talia[12], "y11"); //->blokowanie
    strcpy(talia[13], "y11");
    strcpy(talia[14], "y12"); //->+2
    strcpy(talia[15], "y12");
    strcpy(talia[16], "n00");
    strcpy(talia[17], "n01");
    strcpy(talia[18], "n02");
    strcpy(talia[19], "n03");
    strcpy(talia[20], "n04");
    strcpy(talia[21], "n05");
    strcpy(talia[22], "n06");
    strcpy(talia[23], "n07");
    strcpy(talia[24], "n08");
    strcpy(talia[25], "n09");
    strcpy(talia[26], "n10");
    strcpy(talia[27], "n10"); //->zmiana kolejnosci ruchu
    strcpy(talia[28], "n11"); //->blokowanie
    strcpy(talia[29], "n11");
    strcpy(talia[30], "n12"); //->+2
    strcpy(talia[31], "n12");
    strcpy(talia[32], "c00");
    strcpy(talia[33], "c01");
    strcpy(talia[34], "c02");
    strcpy(talia[35], "c03");
    strcpy(talia[36], "c04");
    strcpy(talia[37], "c05");
    strcpy(talia[38], "c06");
    strcpy(talia[39], "c07");
    strcpy(talia[40], "c08");
    strcpy(talia[41], "c09");
    strcpy(talia[42], "c10");
    strcpy(talia[43], "c10"); //->zmiana kolejnosci ruchu
    strcpy(talia[44], "c11"); //->blokowanie
    strcpy(talia[45], "c11");
    strcpy(talia[46], "c12"); //->+2
    strcpy(talia[47], "c12");
    strcpy(talia[48], "z00");
    strcpy(talia[49], "z01");
    strcpy(talia[50], "z02");
    strcpy(talia[51], "z03");
    strcpy(talia[52], "z04");
    strcpy(talia[53], "z05");
    strcpy(talia[54], "z06");
    strcpy(talia[55], "z07");
    strcpy(talia[56], "z08");
    strcpy(talia[57], "z09");
    strcpy(talia[58], "z10");
    strcpy(talia[59], "z10"); //->zmiana kolejnosci ruchu
    strcpy(talia[60], "z11"); //->blokowanie
    strcpy(talia[61], "z11");
    strcpy(talia[62], "z12"); //->+2
    strcpy(talia[63], "z12");
    strcpy(talia[64], "b13"); //->zmiana koloru
    strcpy(talia[65], "b13"); //->zmiana koloru
    strcpy(talia[66], "b14"); //->+4
    strcpy(talia[67], "b14"); //->+4
}

int sprawdzenie_komunikatu (char* komunikat)
{
    char* wierzch=malloc(3*sizeof(char));
    strcpy(wierzch, zagrane->rodzaj);
    char* zagrana_karta=malloc(3*sizeof(char));
    for (int i=1; i<4; i++)
        zagrana_karta[i-1]=komunikat[i];
    zagrana = false;
    char* komunikat_wys=malloc(20*sizeof(char));
    //zagranie zwykłej
    if (!mode){
        if ((dobieranie==1 && komunikat[2]=='0' && komunikat[1]==wierzch[0] && (int)(wierzch[2]-'0')==((int)(komunikat[3]-'0')+1)%10)|| (dobieranie==1 && komunikat[2]=='0' && (int)(komunikat[0]-'0')==kolej && (komunikat[3]==wierzch[2] || komunikat[1]==wierzch[0])) ) 
        {
            zagrane=push(zagrane, zagrana_karta);
            rozmiar_zagrane+=1;
            if (kolej!=(int)(komunikat[0]-'0')) kolej=(int)(komunikat[0]-'0');
            ile_na_rece[kolej]--;
            if (uno){
                uno=false;
                ile_na_rece[kolej]+=2;
                for (int i=0; i<2;i++)
                {   
                    if (dobierane==NULL)
                    {
                        char gorna[3];
                        strcpy(gorna, zagrane->rodzaj);
                        zagrane=pop(zagrane);
                        nowa_talia = malloc(rozmiar_zagrane*sizeof(char*));
                        for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            nowa_talia[i]=malloc(3*sizeof(char));
                        }
                        for (int i=0;zagrane!=NULL;i++)
                        {
                            strcpy(nowa_talia[i], zagrane->rodzaj);
                            zagrane=pop(zagrane);
                        }
                        nowe_tasowanie(rozmiar_zagrane);
                        rozmiar_dobierane=0;
                        int rozmiar = sizeof(nowa_talia)/sizeof(nowa_talia[0]);
                        for (int i=0;i<rozmiar;i++)
                        {
                            dobierane=push(dobierane, nowa_talia[i]);
                            rozmiar_dobierane+=1;
                        }
                        zagrane=push(zagrane, gorna);
                        rozmiar_zagrane=1;
                    for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            free(nowa_talia[i]);
                        }
                    free(nowa_talia);
                    }
                    write(tab_desc[uno_gracz],dobierane->rodzaj,3);
                    dobierane=pop(dobierane);
                }
            }
            for (int i=0;i<4;i++)
            {
                if (ile_na_rece[i]==1) {
                    uno=true;
                    uno_gracz=i;
                }
            }
            if (ruch_jak_zegar) czyja_kolej = (kolej+1)%4;
            else czyja_kolej = kolej-1;
            if (czyja_kolej<0) czyja_kolej=czyja_kolej+4;
            sprintf(komunikat_wys, "%d;%s;%d;%d;%d;%d", czyja_kolej, zagrana_karta, ile_na_rece[0], ile_na_rece[1], ile_na_rece[2], ile_na_rece[3]);
            write(tab_desc[(int)(komunikat[0]-'0')],"??", 2);
            read(tab_desc[(int)(komunikat[0]-'0')], nic, 2);
            wysylanie_komunikatu(komunikat_wys);
            zagrana = true;
        }
        //zagranie zmiany kolejności tury
        else if (komunikat[2]=='1' && komunikat[3]=='0' && dobieranie==1 && (int)(komunikat[0]-'0')==kolej && ((wierzch[1]==komunikat[2] && wierzch[2]==komunikat[3]) || komunikat[1]==wierzch[0])) 
        {
            if (ruch_jak_zegar) ruch_jak_zegar=false;
            else ruch_jak_zegar=true;
            zagrane=push(zagrane, zagrana_karta);
            rozmiar_zagrane+=1;
            ile_na_rece[kolej]--;
            if (uno){
                uno=false;
                ile_na_rece[kolej]+=2;
                for (int i=0; i<2;i++)
                {   
                    if (dobierane==NULL)
                    {
                        char gorna[3];
                        strcpy(gorna, zagrane->rodzaj);
                        zagrane=pop(zagrane);
                        nowa_talia = malloc(rozmiar_zagrane*sizeof(char*));
                        for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            nowa_talia[i]=malloc(3*sizeof(char));
                        }
                        for (int i=0;zagrane!=NULL;i++)
                        {
                            strcpy(nowa_talia[i], zagrane->rodzaj);
                            zagrane=pop(zagrane);
                        }
                        nowe_tasowanie(rozmiar_zagrane);
                        rozmiar_dobierane=0;
                        int rozmiar = sizeof(nowa_talia)/sizeof(nowa_talia[0]);
                        for (int i=0;i<rozmiar;i++)
                        {
                            dobierane=push(dobierane, nowa_talia[i]);
                            rozmiar_dobierane+=1;
                        }
                        zagrane=push(zagrane, gorna);
                        rozmiar_zagrane=1;
                    for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            free(nowa_talia[i]);
                        }
                    free(nowa_talia);
                    }
                    write(tab_desc[uno_gracz],dobierane->rodzaj,3);
                    dobierane=pop(dobierane);
                }
            }
            for (int i=0;i<4;i++)
            {
                if (ile_na_rece[i]==1) {
                    uno=true;
                    uno_gracz=i;
                }
            }
            if (ruch_jak_zegar) czyja_kolej = (kolej+1)%4;
            else czyja_kolej = kolej-1;
            if (czyja_kolej<0) czyja_kolej=czyja_kolej+4;
            sprintf(komunikat_wys, "%d;%s;%d;%d;%d;%d", czyja_kolej, zagrana_karta, ile_na_rece[0], ile_na_rece[1], ile_na_rece[2], ile_na_rece[3]);
            write(tab_desc[(int)(komunikat[0]-'0')],"??", 2);
            read(tab_desc[(int)(komunikat[0]-'0')], nic, 2);
            wysylanie_komunikatu(komunikat_wys);
            zagrana = true;
        }
        //zagranie stopu
        else if (komunikat[2]=='1' && komunikat[3]=='1' && dobieranie==1 && (int)(komunikat[0]-'0')==kolej && ((wierzch[1]==komunikat[2] && wierzch[2]==komunikat[3]) || komunikat[1]==wierzch[0]))
        {
            zagrane=push(zagrane, zagrana_karta);
            rozmiar_zagrane+=1;
            ile_na_rece[kolej]--;
            if (uno){
                uno=false;
                ile_na_rece[kolej]+=2;
                for (int i=0; i<2;i++)
                {   
                    if (dobierane==NULL)
                    {
                        char gorna[3];
                        strcpy(gorna, zagrane->rodzaj);
                        zagrane=pop(zagrane);
                        nowa_talia = malloc(rozmiar_zagrane*sizeof(char*));
                        for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            nowa_talia[i]=malloc(3*sizeof(char));
                        }
                        for (int i=0;zagrane!=NULL;i++)
                        {
                            strcpy(nowa_talia[i], zagrane->rodzaj);
                            zagrane=pop(zagrane);
                        }
                        nowe_tasowanie(rozmiar_zagrane);
                        rozmiar_dobierane=0;
                        int rozmiar = sizeof(nowa_talia)/sizeof(nowa_talia[0]);
                        for (int i=0;i<rozmiar;i++)
                        {
                            dobierane=push(dobierane, nowa_talia[i]);
                            rozmiar_dobierane+=1;
                        }
                        zagrane=push(zagrane, gorna);
                        rozmiar_zagrane=1;
                    for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            free(nowa_talia[i]);
                        }
                    free(nowa_talia);
                    }
                    write(tab_desc[uno_gracz],dobierane->rodzaj,3);
                    dobierane=pop(dobierane);
                }
            }
            for (int i=0;i<4;i++)
            {
                if (ile_na_rece[i]==1) {
                    uno=true;
                    uno_gracz=i;
                }
            }
            if (ruch_jak_zegar) kolej = (kolej+1)%4;
            else kolej = kolej-1;
            if (kolej<0) kolej=kolej+4;
            if (ruch_jak_zegar) czyja_kolej = (kolej+1)%4;
            else czyja_kolej = kolej-1;
            if (czyja_kolej<0) czyja_kolej=czyja_kolej+4;
            sprintf(komunikat_wys, "%d;%s;%d;%d;%d;%d", czyja_kolej, zagrana_karta, ile_na_rece[0], ile_na_rece[1], ile_na_rece[2], ile_na_rece[3]);
            write(tab_desc[(int)(komunikat[0]-'0')],"??", 2);
            read(tab_desc[(int)(komunikat[0]-'0')], nic, 2);
            wysylanie_komunikatu(komunikat_wys);
            zagrana = true;
        }
        //zagranie +2
        else if (komunikat[2]=='1' && komunikat[3]=='2' && (int)(komunikat[0]-'0')==kolej && ((wierzch[1]==komunikat[2] && wierzch[2]==komunikat[3]) || (komunikat[1]==wierzch[0] && dobieranie==1)))
        {
            zagrane=push(zagrane, zagrana_karta);
            rozmiar_zagrane+=1;
            if (dobieranie==1)
            {
                dobieranie+=1;
            }
            else dobieranie+=2;
            ile_na_rece[kolej]--;
            if (uno){
                uno=false;
                ile_na_rece[kolej]+=2;
                for (int i=0; i<2;i++)
                {   
                    if (dobierane==NULL)
                    {
                        char gorna[3];
                        strcpy(gorna, zagrane->rodzaj);
                        zagrane=pop(zagrane);
                        nowa_talia = malloc(rozmiar_zagrane*sizeof(char*));
                        for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            nowa_talia[i]=malloc(3*sizeof(char));
                        }
                        for (int i=0;zagrane!=NULL;i++)
                        {
                            strcpy(nowa_talia[i], zagrane->rodzaj);
                            zagrane=pop(zagrane);
                        }
                        nowe_tasowanie(rozmiar_zagrane);
                        rozmiar_dobierane=0;
                        int rozmiar = sizeof(nowa_talia)/sizeof(nowa_talia[0]);
                        for (int i=0;i<rozmiar;i++)
                        {
                            dobierane=push(dobierane, nowa_talia[i]);
                            rozmiar_dobierane+=1;
                        }
                        zagrane=push(zagrane, gorna);
                        rozmiar_zagrane=1;
                    for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            free(nowa_talia[i]);
                        }
                    free(nowa_talia);
                    }
                    write(tab_desc[uno_gracz],dobierane->rodzaj,3);
                    dobierane=pop(dobierane);
                }
            }
            for (int i=0;i<4;i++)
            {
                if (ile_na_rece[i]==1) {
                    uno=true;
                    uno_gracz=i;
                }
            }
            if (ruch_jak_zegar) czyja_kolej = (kolej+1)%4;
            else czyja_kolej = kolej-1;
            if (czyja_kolej<0) czyja_kolej=czyja_kolej+4;
            sprintf(komunikat_wys, "%d;%s;%d;%d;%d;%d", czyja_kolej, zagrana_karta, ile_na_rece[0], ile_na_rece[1], ile_na_rece[2], ile_na_rece[3]);
            write(tab_desc[(int)(komunikat[0]-'0')],"??", 2);
            read(tab_desc[(int)(komunikat[0]-'0')], nic, 2);
            wysylanie_komunikatu(komunikat_wys);
            zagrana = true;
        }
        //zagranie +4
        else if (komunikat[2]=='1' && komunikat[3]=='4' && (int)(komunikat[0]-'0')==kolej && (komunikat[2]!='1' || komunikat[3]!='2' || dobieranie==1))
        {
            zagrane=push(zagrane, zagrana_karta);
            rozmiar_zagrane+=1;
            if (dobieranie==1)
            {
                dobieranie+=3;
            }
            else dobieranie+=4;
            ile_na_rece[kolej]--;
            if (uno){
                uno=false;
                ile_na_rece[kolej]+=2;
                for (int i=0; i<2;i++)
                {   
                    if (dobierane==NULL)
                    {
                        char gorna[3];
                        strcpy(gorna, zagrane->rodzaj);
                        zagrane=pop(zagrane);
                        nowa_talia = malloc(rozmiar_zagrane*sizeof(char*));
                        for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            nowa_talia[i]=malloc(3*sizeof(char));
                        }
                        for (int i=0;zagrane!=NULL;i++)
                        {
                            strcpy(nowa_talia[i], zagrane->rodzaj);
                            zagrane=pop(zagrane);
                        }
                        nowe_tasowanie(rozmiar_zagrane);
                        rozmiar_dobierane=0;
                        int rozmiar = sizeof(nowa_talia)/sizeof(nowa_talia[0]);
                        for (int i=0;i<rozmiar;i++)
                        {
                            dobierane=push(dobierane, nowa_talia[i]);
                            rozmiar_dobierane+=1;
                        }
                        zagrane=push(zagrane, gorna);
                        rozmiar_zagrane=1;
                    for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            free(nowa_talia[i]);
                        }
                    free(nowa_talia);
                    }
                    write(tab_desc[uno_gracz],dobierane->rodzaj,3);
                    dobierane=pop(dobierane);
                }
            }
            for (int i=0;i<4;i++)
            {
                if (ile_na_rece[i]==1) {
                    uno=true;
                    uno_gracz=i;
                }
            }
            if (ruch_jak_zegar) czyja_kolej = (kolej+1)%4;
            else czyja_kolej = kolej-1;
            if (czyja_kolej<0) czyja_kolej=czyja_kolej+4;
            sprintf(komunikat_wys, "%d;%s;%d;%d;%d;%d", czyja_kolej, zagrana_karta, ile_na_rece[0], ile_na_rece[1], ile_na_rece[2], ile_na_rece[3]);
            write(tab_desc[(int)(komunikat[0]-'0')],"??", 2);
            read(tab_desc[(int)(komunikat[0]-'0')], nic, 2);
            wysylanie_komunikatu(komunikat_wys);
            read(tab_desc[kolej], kolor, 2);
            wysylanie_komunikatu(kolor);
            zagrana = true;
            mode=true;
        }
        //zagranie zmiany koloru
        else if (komunikat[2]=='1' && komunikat[3]=='3' && dobieranie==1 && (int)(komunikat[0]-'0')==kolej)
        {
            zagrane=push(zagrane, zagrana_karta);
            rozmiar_zagrane+=1;
            ile_na_rece[kolej]--;
            if (uno){
                uno=false;
                ile_na_rece[kolej]+=2;
                for (int i=0; i<2;i++)
                {   
                    if (dobierane==NULL)
                    {
                        char gorna[3];
                        strcpy(gorna, zagrane->rodzaj);
                        zagrane=pop(zagrane);
                        nowa_talia = malloc(rozmiar_zagrane*sizeof(char*));
                        for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            nowa_talia[i]=malloc(3*sizeof(char));
                        }
                        for (int i=0;zagrane!=NULL;i++)
                        {
                            strcpy(nowa_talia[i], zagrane->rodzaj);
                            zagrane=pop(zagrane);
                        }
                        nowe_tasowanie(rozmiar_zagrane);
                        rozmiar_dobierane=0;
                        int rozmiar = sizeof(nowa_talia)/sizeof(nowa_talia[0]);
                        for (int i=0;i<rozmiar;i++)
                        {
                            dobierane=push(dobierane, nowa_talia[i]);
                            rozmiar_dobierane+=1;
                        }
                        zagrane=push(zagrane, gorna);
                        rozmiar_zagrane=1;
                    for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            free(nowa_talia[i]);
                        }
                    free(nowa_talia);
                    }
                    write(tab_desc[uno_gracz],dobierane->rodzaj,3);
                    dobierane=pop(dobierane);
                }
            }
            for (int i=0;i<4;i++)
            {
                if (ile_na_rece[i]==1) {
                    uno=true;
                    uno_gracz=i;
                }
            }
            if (ruch_jak_zegar) czyja_kolej = (kolej+1)%4;
            else czyja_kolej = kolej-1;
            if (czyja_kolej<0) czyja_kolej=czyja_kolej+4;
            sprintf(komunikat_wys, "%d;%s;%d;%d;%d;%d", czyja_kolej, zagrana_karta, ile_na_rece[0], ile_na_rece[1], ile_na_rece[2], ile_na_rece[3]);
            write(tab_desc[(int)(komunikat[0]-'0')],"??", 2);
            read(tab_desc[(int)(komunikat[0]-'0')], nic, 2);
            wysylanie_komunikatu(komunikat_wys);
            read(tab_desc[kolej], kolor, 2);
            wysylanie_komunikatu(kolor);
            zagrana = true;
            mode=true;
        }
        //komunikat o dobraniu kart
        else if ((int)(komunikat[0]-'0')==kolej && komunikat[1]=='p')
        {
            ile_na_rece[kolej]+=dobieranie;
            for (int i=0; i<dobieranie;i++)
            {   
                if (dobierane==NULL)
                {
                    char gorna[3];
                    strcpy(gorna, zagrane->rodzaj);
                    zagrane=pop(zagrane);
                    nowa_talia = malloc(rozmiar_zagrane*sizeof(char*));
                    for (int i=0; i<rozmiar_zagrane;i++)
                    {
                        nowa_talia[i]=malloc(3*sizeof(char));
                    }
                    for (int i=0;zagrane!=NULL;i++)
                    {
                        strcpy(nowa_talia[i], zagrane->rodzaj);
                        zagrane=pop(zagrane);
                    }
                    nowe_tasowanie(rozmiar_zagrane);
                    rozmiar_dobierane=0;
                    int rozmiar = sizeof(nowa_talia)/sizeof(nowa_talia[0]);
                    for (int i=0;i<rozmiar;i++)
                    {
                        dobierane=push(dobierane, nowa_talia[i]);
                        rozmiar_dobierane+=1;
                    }
                    zagrane=push(zagrane, gorna);
                    rozmiar_zagrane=1;
                for (int i=0; i<rozmiar_zagrane;i++)
                    {
                        free(nowa_talia[i]);
                    }
                free(nowa_talia);
                }
                write(tab_desc[(int)(komunikat[0]-'0')],dobierane->rodzaj,3);
                dobierane=pop(dobierane);
            }
            if (ruch_jak_zegar) czyja_kolej = (kolej+1)%4;
            else czyja_kolej = kolej-1;
            if (czyja_kolej<0) czyja_kolej=czyja_kolej+4;
            sprintf(komunikat_wys, "%d;%s;%d;%d;%d;%d", czyja_kolej, wierzch, ile_na_rece[0], ile_na_rece[1], ile_na_rece[2], ile_na_rece[3]);
            write(tab_desc[(int)(komunikat[0]-'0')],"??", 2);
            read(tab_desc[(int)(komunikat[0]-'0')], nic, 2);
            wysylanie_komunikatu(komunikat_wys);
            dobieranie=1;
            zagrana = true;
        }
        //komunikat o UNO
        else if (komunikat[1]=='u')
        {
            if ((int)(komunikat[0]-'0')==uno_gracz) uno = false;
        }
    } else //czarna
        {
        if ((dobieranie==1 && komunikat[2]=='0' && kolor[0]==wierzch[0] && (int)(wierzch[2]-'0')==((int)(komunikat[3]-'0')+1)%10)|| (dobieranie==1 && komunikat[2]=='0' && (int)(komunikat[0]-'0')==kolej && (komunikat[3]==wierzch[2] || kolor[0]==wierzch[0])) ) 
        {
            zagrane=push(zagrane, zagrana_karta);
            rozmiar_zagrane+=1;
            if (kolej!=(int)(komunikat[0]-'0')) kolej=(int)(komunikat[0]-'0');
            ile_na_rece[kolej]--;
            if (uno){
                uno=false;
                ile_na_rece[kolej]+=2;
                for (int i=0; i<2;i++)
                {   
                    if (dobierane==NULL)
                    {
                        char gorna[3];
                        strcpy(gorna, zagrane->rodzaj);
                        zagrane=pop(zagrane);
                        nowa_talia = malloc(rozmiar_zagrane*sizeof(char*));
                        for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            nowa_talia[i]=malloc(3*sizeof(char));
                        }
                        for (int i=0;zagrane!=NULL;i++)
                        {
                            strcpy(nowa_talia[i], zagrane->rodzaj);
                            zagrane=pop(zagrane);
                        }
                        nowe_tasowanie(rozmiar_zagrane);
                        rozmiar_dobierane=0;
                        int rozmiar = sizeof(nowa_talia)/sizeof(nowa_talia[0]);
                        for (int i=0;i<rozmiar;i++)
                        {
                            dobierane=push(dobierane, nowa_talia[i]);
                            rozmiar_dobierane+=1;
                        }
                        zagrane=push(zagrane, gorna);
                        rozmiar_zagrane=1;
                    for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            free(nowa_talia[i]);
                        }
                    free(nowa_talia);
                    }
                    write(tab_desc[uno_gracz],dobierane->rodzaj,3);
                    dobierane=pop(dobierane);
                }
            }
            for (int i=0;i<4;i++)
            {
                if (ile_na_rece[i]==1) {
                    uno=true;
                    uno_gracz=i;
                }
            }
            if (ruch_jak_zegar) czyja_kolej = (kolej+1)%4;
            else czyja_kolej = kolej-1;
            if (czyja_kolej<0) czyja_kolej=czyja_kolej+4;
            sprintf(komunikat_wys, "%d;%s;%d;%d;%d;%d", czyja_kolej, zagrana_karta, ile_na_rece[0], ile_na_rece[1], ile_na_rece[2], ile_na_rece[3]);
            write(tab_desc[(int)(komunikat[0]-'0')],"??", 2);
            read(tab_desc[(int)(komunikat[0]-'0')], nic, 2);
            wysylanie_komunikatu(komunikat_wys);
            mode=false;
            zagrana = true;
        }
        //zagranie zmiany kolejności tury
        else if (komunikat[2]=='1' && komunikat[3]=='0' && dobieranie==1 && (int)(komunikat[0]-'0')==kolej && ((wierzch[1]==komunikat[2] && wierzch[2]==komunikat[3]) || kolor[0]==wierzch[0])) 
        {
            if (ruch_jak_zegar) ruch_jak_zegar=false;
            else ruch_jak_zegar=true;
            zagrane=push(zagrane, zagrana_karta);
            rozmiar_zagrane+=1;
            ile_na_rece[kolej]--;
            if (uno){
                uno=false;
                ile_na_rece[kolej]+=2;
                for (int i=0; i<2;i++)
                {   
                    if (dobierane==NULL)
                    {
                        char gorna[3];
                        strcpy(gorna, zagrane->rodzaj);
                        zagrane=pop(zagrane);
                        nowa_talia = malloc(rozmiar_zagrane*sizeof(char*));
                        for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            nowa_talia[i]=malloc(3*sizeof(char));
                        }
                        for (int i=0;zagrane!=NULL;i++)
                        {
                            strcpy(nowa_talia[i], zagrane->rodzaj);
                            zagrane=pop(zagrane);
                        }
                        nowe_tasowanie(rozmiar_zagrane);
                        rozmiar_dobierane=0;
                        int rozmiar = sizeof(nowa_talia)/sizeof(nowa_talia[0]);
                        for (int i=0;i<rozmiar;i++)
                        {
                            dobierane=push(dobierane, nowa_talia[i]);
                            rozmiar_dobierane+=1;
                        }
                        zagrane=push(zagrane, gorna);
                        rozmiar_zagrane=1;
                    for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            free(nowa_talia[i]);
                        }
                    free(nowa_talia);
                    }
                    write(tab_desc[uno_gracz],dobierane->rodzaj,3);
                    dobierane=pop(dobierane);
                }
            }
            for (int i=0;i<4;i++)
            {
                if (ile_na_rece[i]==1) {
                    uno=true;
                    uno_gracz=i;
                }
            }
            if (ruch_jak_zegar) czyja_kolej = (kolej+1)%4;
            else czyja_kolej = kolej-1;
            if (czyja_kolej<0) czyja_kolej=czyja_kolej+4;
            sprintf(komunikat_wys, "%d;%s;%d;%d;%d;%d", czyja_kolej, zagrana_karta, ile_na_rece[0], ile_na_rece[1], ile_na_rece[2], ile_na_rece[3]);
            write(tab_desc[(int)(komunikat[0]-'0')],"??", 2);
            read(tab_desc[(int)(komunikat[0]-'0')], nic, 2);
            wysylanie_komunikatu(komunikat_wys);
            zagrana = true;
            mode=false;
        }
        //zagranie stopu
        else if (komunikat[2]=='1' && komunikat[3]=='1' && dobieranie==1 && (int)(komunikat[0]-'0')==kolej && ((wierzch[1]==komunikat[2] && wierzch[2]==komunikat[3]) || kolor[0]==wierzch[0]))
        {
            zagrane=push(zagrane, zagrana_karta);
            rozmiar_zagrane+=1;
            ile_na_rece[kolej]--;
            if (uno){
                uno=false;
                ile_na_rece[kolej]+=2;
                for (int i=0; i<2;i++)
                {   
                    if (dobierane==NULL)
                    {
                        char gorna[3];
                        strcpy(gorna, zagrane->rodzaj);
                        zagrane=pop(zagrane);
                        nowa_talia = malloc(rozmiar_zagrane*sizeof(char*));
                        for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            nowa_talia[i]=malloc(3*sizeof(char));
                        }
                        for (int i=0;zagrane!=NULL;i++)
                        {
                            strcpy(nowa_talia[i], zagrane->rodzaj);
                            zagrane=pop(zagrane);
                        }
                        nowe_tasowanie(rozmiar_zagrane);
                        rozmiar_dobierane=0;
                        int rozmiar = sizeof(nowa_talia)/sizeof(nowa_talia[0]);
                        for (int i=0;i<rozmiar;i++)
                        {
                            dobierane=push(dobierane, nowa_talia[i]);
                            rozmiar_dobierane+=1;
                        }
                        zagrane=push(zagrane, gorna);
                        rozmiar_zagrane=1;
                    for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            free(nowa_talia[i]);
                        }
                    free(nowa_talia);
                    }
                    write(tab_desc[uno_gracz],dobierane->rodzaj,3);
                    dobierane=pop(dobierane);
                }
            }
            for (int i=0;i<4;i++)
            {
                if (ile_na_rece[i]==1) {
                    uno=true;
                    uno_gracz=i;
                }
            }
            if (ruch_jak_zegar) kolej = (kolej+1)%4;
            else kolej = kolej-1;
            if (kolej<0) kolej=kolej+4;
            if (ruch_jak_zegar) czyja_kolej = (kolej+1)%4;
            else czyja_kolej = kolej-1;
            if (czyja_kolej<0) czyja_kolej=czyja_kolej+4;
            sprintf(komunikat_wys, "%d;%s;%d;%d;%d;%d", czyja_kolej, zagrana_karta, ile_na_rece[0], ile_na_rece[1], ile_na_rece[2], ile_na_rece[3]);
            write(tab_desc[(int)(komunikat[0]-'0')],"??", 2);
            read(tab_desc[(int)(komunikat[0]-'0')], nic, 2);
            wysylanie_komunikatu(komunikat_wys);
            zagrana = true;
            mode=false;
        }
        //zagranie +2
        else if (komunikat[2]=='1' && komunikat[3]=='2' && (int)(komunikat[0]-'0')==kolej && ((wierzch[1]==komunikat[2] && wierzch[2]==komunikat[3]) || (kolor[0]==wierzch[0] && dobieranie==1)))
        {
            zagrane=push(zagrane, zagrana_karta);
            rozmiar_zagrane+=1;
            if (dobieranie==1)
            {
                dobieranie+=1;
            }
            else dobieranie+=2;
            ile_na_rece[kolej]--;
            if (uno){
                uno=false;
                ile_na_rece[kolej]+=2;
                for (int i=0; i<2;i++)
                {   
                    if (dobierane==NULL)
                    {
                        char gorna[3];
                        strcpy(gorna, zagrane->rodzaj);
                        zagrane=pop(zagrane);
                        nowa_talia = malloc(rozmiar_zagrane*sizeof(char*));
                        for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            nowa_talia[i]=malloc(3*sizeof(char));
                        }
                        for (int i=0;zagrane!=NULL;i++)
                        {
                            strcpy(nowa_talia[i], zagrane->rodzaj);
                            zagrane=pop(zagrane);
                        }
                        nowe_tasowanie(rozmiar_zagrane);
                        rozmiar_dobierane=0;
                        int rozmiar = sizeof(nowa_talia)/sizeof(nowa_talia[0]);
                        for (int i=0;i<rozmiar;i++)
                        {
                            dobierane=push(dobierane, nowa_talia[i]);
                            rozmiar_dobierane+=1;
                        }
                        zagrane=push(zagrane, gorna);
                        rozmiar_zagrane=1;
                    for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            free(nowa_talia[i]);
                        }
                    free(nowa_talia);
                    }
                    write(tab_desc[uno_gracz],dobierane->rodzaj,3);
                    dobierane=pop(dobierane);
                }
            }
            for (int i=0;i<4;i++)
            {
                if (ile_na_rece[i]==1) {
                    uno=true;
                    uno_gracz=i;
                }
            }
            if (ruch_jak_zegar) czyja_kolej = (kolej+1)%4;
            else czyja_kolej = kolej-1;
            if (czyja_kolej<0) czyja_kolej=czyja_kolej+4;
            sprintf(komunikat_wys, "%d;%s;%d;%d;%d;%d", czyja_kolej, zagrana_karta, ile_na_rece[0], ile_na_rece[1], ile_na_rece[2], ile_na_rece[3]);
            write(tab_desc[(int)(komunikat[0]-'0')],"??", 2);
            read(tab_desc[(int)(komunikat[0]-'0')], nic, 2);
            wysylanie_komunikatu(komunikat_wys);
            zagrana = true;
            mode=false;
        }
        //zagranie +4
        else if (komunikat[2]=='1' && komunikat[3]=='4' && (int)(komunikat[0]-'0')==kolej && (komunikat[2]!='1' || komunikat[3]!='2' || dobieranie==1))
        {
            zagrane=push(zagrane, zagrana_karta);
            rozmiar_zagrane+=1;
            if (dobieranie==1)
            {
                dobieranie+=3;
            }
            else dobieranie+=4;
            ile_na_rece[kolej]--;
            if (uno){
                uno=false;
                ile_na_rece[kolej]+=2;
                for (int i=0; i<2;i++)
                {   
                    if (dobierane==NULL)
                    {
                        char gorna[3];
                        strcpy(gorna, zagrane->rodzaj);
                        zagrane=pop(zagrane);
                        nowa_talia = malloc(rozmiar_zagrane*sizeof(char*));
                        for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            nowa_talia[i]=malloc(3*sizeof(char));
                        }
                        for (int i=0;zagrane!=NULL;i++)
                        {
                            strcpy(nowa_talia[i], zagrane->rodzaj);
                            zagrane=pop(zagrane);
                        }
                        nowe_tasowanie(rozmiar_zagrane);
                        rozmiar_dobierane=0;
                        int rozmiar = sizeof(nowa_talia)/sizeof(nowa_talia[0]);
                        for (int i=0;i<rozmiar;i++)
                        {
                            dobierane=push(dobierane, nowa_talia[i]);
                            rozmiar_dobierane+=1;
                        }
                        zagrane=push(zagrane, gorna);
                        rozmiar_zagrane=1;
                    for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            free(nowa_talia[i]);
                        }
                    free(nowa_talia);
                    }
                    write(tab_desc[uno_gracz],dobierane->rodzaj,3);
                    dobierane=pop(dobierane);
                }
            }
            for (int i=0;i<4;i++)
            {
                if (ile_na_rece[i]==1) {
                    uno=true;
                    uno_gracz=i;
                }
            }
            if (ruch_jak_zegar) czyja_kolej = (kolej+1)%4;
            else czyja_kolej = kolej-1;
            if (czyja_kolej<0) czyja_kolej=czyja_kolej+4;
            sprintf(komunikat_wys, "%d;%s;%d;%d;%d;%d", czyja_kolej, zagrana_karta, ile_na_rece[0], ile_na_rece[1], ile_na_rece[2], ile_na_rece[3]);
            write(tab_desc[(int)(komunikat[0]-'0')],"??", 2);
            read(tab_desc[(int)(komunikat[0]-'0')], nic, 2);
            wysylanie_komunikatu(komunikat_wys);
            read(tab_desc[kolej], kolor, 2);
            wysylanie_komunikatu(kolor);
            zagrana = true;
        }
        //zagranie zmiany koloru
        else if (komunikat[2]=='1' && komunikat[3]=='3' && dobieranie==1 && (int)(komunikat[0]-'0')==kolej)
        {
            zagrane=push(zagrane, zagrana_karta);
            rozmiar_zagrane+=1;
            ile_na_rece[kolej]--;
            if (uno){
                uno=false;
                ile_na_rece[kolej]+=2;
                for (int i=0; i<2;i++)
                {   
                    if (dobierane==NULL)
                    {
                        char gorna[3];
                        strcpy(gorna, zagrane->rodzaj);
                        zagrane=pop(zagrane);
                        nowa_talia = malloc(rozmiar_zagrane*sizeof(char*));
                        for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            nowa_talia[i]=malloc(3*sizeof(char));
                        }
                        for (int i=0;zagrane!=NULL;i++)
                        {
                            strcpy(nowa_talia[i], zagrane->rodzaj);
                            zagrane=pop(zagrane);
                        }
                        nowe_tasowanie(rozmiar_zagrane);
                        rozmiar_dobierane=0;
                        int rozmiar = sizeof(nowa_talia)/sizeof(nowa_talia[0]);
                        for (int i=0;i<rozmiar;i++)
                        {
                            dobierane=push(dobierane, nowa_talia[i]);
                            rozmiar_dobierane+=1;
                        }
                        zagrane=push(zagrane, gorna);
                        rozmiar_zagrane=1;
                    for (int i=0; i<rozmiar_zagrane;i++)
                        {
                            free(nowa_talia[i]);
                        }
                    free(nowa_talia);
                    }
                    write(tab_desc[uno_gracz],dobierane->rodzaj,3);
                    dobierane=pop(dobierane);
                }
            }
            for (int i=0;i<4;i++)
            {
                if (ile_na_rece[i]==1) {
                    uno=true;
                    uno_gracz=i;
                }
            }
            if (ruch_jak_zegar) czyja_kolej = (kolej+1)%4;
            else czyja_kolej = kolej-1;
            if (czyja_kolej<0) czyja_kolej=czyja_kolej+4;
            sprintf(komunikat_wys, "%d;%s;%d;%d;%d;%d", czyja_kolej, zagrana_karta, ile_na_rece[0], ile_na_rece[1], ile_na_rece[2], ile_na_rece[3]);
            write(tab_desc[(int)(komunikat[0]-'0')],"??", 2);
            read(tab_desc[(int)(komunikat[0]-'0')], nic, 2);
            wysylanie_komunikatu(komunikat_wys);
            read(tab_desc[kolej], kolor, 2);
            wysylanie_komunikatu(kolor);
            zagrana = true;
        }
        //komunikat o dobraniu kart
        else if ((int)(komunikat[0]-'0')==kolej && komunikat[1]=='p')
        {
            ile_na_rece[kolej]+=dobieranie;
            for (int i=0; i<dobieranie;i++)
            {   
                if (dobierane==NULL)
                {
                    char gorna[3];
                    strcpy(gorna, zagrane->rodzaj);
                    zagrane=pop(zagrane);
                    nowa_talia = malloc(rozmiar_zagrane*sizeof(char*));
                    for (int i=0; i<rozmiar_zagrane;i++)
                    {
                        nowa_talia[i]=malloc(3*sizeof(char));
                    }
                    for (int i=0;zagrane!=NULL;i++)
                    {
                        strcpy(nowa_talia[i], zagrane->rodzaj);
                        zagrane=pop(zagrane);
                    }
                    nowe_tasowanie(rozmiar_zagrane);
                    rozmiar_dobierane=0;
                    int rozmiar = sizeof(nowa_talia)/sizeof(nowa_talia[0]);
                    for (int i=0;i<rozmiar;i++)
                    {
                        dobierane=push(dobierane, nowa_talia[i]);
                        rozmiar_dobierane+=1;
                    }
                    zagrane=push(zagrane, gorna);
                    rozmiar_zagrane=1;
                for (int i=0; i<rozmiar_zagrane;i++)
                    {
                        free(nowa_talia[i]);
                    }
                free(nowa_talia);
                }
                write(tab_desc[(int)(komunikat[0]-'0')],dobierane->rodzaj,3);
                dobierane=pop(dobierane);
                //tu sprawdzic czy nie jest pusta talia do dobierania
            }
            if (ruch_jak_zegar) czyja_kolej = (kolej+1)%4;
            else czyja_kolej = kolej-1;
            if (czyja_kolej<0) czyja_kolej=czyja_kolej+4;
            sprintf(komunikat_wys, "%d;%s;%d;%d;%d;%d", czyja_kolej, wierzch, ile_na_rece[0], ile_na_rece[1], ile_na_rece[2], ile_na_rece[3]);
            write(tab_desc[(int)(komunikat[0]-'0')],"??", 2);
            read(tab_desc[(int)(komunikat[0]-'0')], nic, 2);
            wysylanie_komunikatu(komunikat_wys);
            dobieranie=1;
            zagrana = true;
        }
        //komunikat o UNO
        else if (komunikat[1]=='u')
        {
            if ((int)(komunikat[0]-'0')==uno_gracz) uno = false;
        }
    }
    if (zagrana) {
        if (ruch_jak_zegar) kolej = (kolej+1)%4;
        else kolej = kolej-1;}
    if (kolej<0) kolej=kolej+4;
    free(wierzch);
    free(komunikat_wys);
    if (ile_na_rece[0]==0) return 0;
    if (ile_na_rece[1]==0) return 1;
    if (ile_na_rece[2]==0) return 2;
    if (ile_na_rece[3]==0) return 3;
    return 4;
}

void *ThreadBehavior(void *t_data)
{ 
    pthread_detach(pthread_self());
    struct thread_data_t *th_data = (struct thread_data_t*)t_data;
    int desc = th_data->my_socket;
    nick = malloc(4*sizeof(char*));
    if (th_data->my_turn<3)
    {
        pthread_mutex_lock(&start_mutex);

        pthread_cond_wait(&start_cond, &start_mutex);

        pthread_mutex_unlock(&start_mutex);
    }
    else
    {
        pthread_cond_broadcast(&start_cond);
    }
    pthread_mutex_lock(&talia_mutex);
    char ktory[2];
    ktory[0] = th_data->my_turn+'0';
    ktory[1]='!';
    write(desc, ktory,2);
    for (int i=0; i<4;i++)
    {
        nick[i]=malloc(20*sizeof(char));
    }
    if (th_data->my_turn==3)
    {
        int indeks=28;
        while(talia[indeks][1]!='0')
        {
            indeks++;
        }
        if (indeks!=28)
        {
            char* swap=malloc(3*sizeof(char));
            strcpy(swap, talia[indeks]);
            strcpy(talia[indeks], talia[28]);
            strcpy(talia[28], swap);
            free(swap);
        }
        zagrane=push(zagrane, talia[28]);
        rozmiar_zagrane+=1;
    }
    char proba[3];
    strcpy(proba, talia[28]);
    write(desc, proba, 3);
    for (int i=th_data->my_turn*7; i<(th_data->my_turn+1)*7;i++)
    {
        write(desc, talia[i], 3);
    }
    /*if (th_data->my_turn==0)
    {
        char wiadomosc1[27];
        sprintf(wiadomosc1, "%s;%s;%s;%s;%s;%s;%s", talia[0], talia[1], talia[2], talia[3], talia[4], talia[5], talia[6]);
        write(desc, wiadomosc1, 27);
    }
    if (th_data->my_turn==1)
    {
        char wiadomosc2[27];
        sprintf(wiadomosc2, "%s;%s;%s;%s;%s;%s;%s", talia[7], talia[8], talia[9], talia[10], talia[11], talia[12], talia[13]);
        write(desc, wiadomosc2, 27);
    }
    if (th_data->my_turn==2)
    {
        char wiadomosc3[27];
        sprintf(wiadomosc3, "%s;%s;%s;%s;%s;%s;%s", talia[14], talia[15], talia[16], talia[17], talia[18], talia[19], talia[20]);
        write(desc, wiadomosc3, 27);
    }
    if (th_data->my_turn==3)
    {
        char wiadomosc4[27];
        sprintf(wiadomosc4, "%s;%s;%s;%s;%s;%s;%s", talia[21], talia[22], talia[23], talia[24], talia[25], talia[26], talia[27]);
        write(desc, wiadomosc4, 27);
    }*/
    if (th_data->my_turn==3)
    {   
        for (int i=29;i<68;i++)
        {
            dobierane=push(dobierane, talia[i]);
            rozmiar_dobierane+=1;
        }
        
    }
    char* global_nick=malloc(83*sizeof(char));
    sprintf(global_nick, "%s;%s;%s;%s", nick[0], nick[1], nick[2], nick[3]);
    //write(desc, global_nick, 83);
    free(global_nick);
    pthread_mutex_unlock(&talia_mutex);
    char* komunikat=malloc(4*sizeof(char));
    while(!wygrana)
    {
        //przyjęcie jaka karta
        read(desc, komunikat, 4);
        pthread_mutex_lock(&ruch_mutex);
        if (!wygrana)
        {   
            sprawdzenie=sprawdzenie_komunikatu(komunikat);
            if (sprawdzenie<4)
            {
                wygrana=true;
                char zwyciezca = sprawdzenie + '0';
                char* komunikat_koniec=malloc(2*sizeof(char));
                komunikat_koniec[0]='!';
                komunikat_koniec[1]=zwyciezca;
                wysylanie_komunikatu(komunikat_koniec);
                free(komunikat_koniec);
            }
        }
        pthread_mutex_unlock(&ruch_mutex);
    }

    return NULL;
}

void handleConnection(int connection_socket_descriptor, int turn)
{
    int create_result = 0;
    pthread_t thread1;

    struct thread_data_t* t_data = malloc(sizeof(struct thread_data_t));
    t_data->my_socket = connection_socket_descriptor;
    t_data->my_turn = turn;
    create_result = pthread_create(&thread1, NULL, ThreadBehavior, (void *)t_data);
    if (create_result){
       printf("Błąd przy próbie utworzenia wątku, kod błędu: %d\n", create_result);
       exit(-1);
    }
}

int main(int argc, char* argv[])
{
    struct sockaddr_in server_address;

    memset(&server_address, 0, sizeof(struct sockaddr));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi(argv[1]));

    int server_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_descriptor < 0)
    {
        fprintf(stderr, "%s: Błąd przy próbie utworzenia gniazda..\n", argv[0]);
        exit(1);
    }
    char reuse_addr_val = 1;
    setsockopt(server_socket_descriptor, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse_addr_val, sizeof(reuse_addr_val));

    int bind_result = bind(server_socket_descriptor, (struct sockaddr*)&server_address, sizeof(struct sockaddr));
    if (bind_result < 0)
    {
        fprintf(stderr, "%s: Błąd przy próbie dowiązania adresu IP i numeru portu do gniazda.\n", argv[0]);
        exit(1);
    }

    int listen_result = listen(server_socket_descriptor, 4);
    if (listen_result < 0) {
        fprintf(stderr, "%s: Błąd przy próbie ustawienia wielkości kolejki.\n", argv[0]);
        exit(1);
    }
    
    int connection_socket_descriptor;
    przygotowanie_talii();
    tasowanie(68);
    while(1)
    {
        connection_socket_descriptor = accept(server_socket_descriptor, NULL, NULL);
        if (connection_socket_descriptor < 0)
        {
            fprintf(stderr, "%s: Błąd przy próbie utworzenia gniazda dla połączenia.\n", argv[0]);
            exit(1);
        }
        pthread_mutex_lock(&desc_mutex);
        tab_desc[gracze]=connection_socket_descriptor;
        pthread_mutex_unlock(&desc_mutex);
        handleConnection(connection_socket_descriptor, gracze);
        gracze++;
    }
}