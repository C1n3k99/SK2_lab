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
#include <stos.h>

int kolej = 0; //0-3
int gracze = 0;
bool wygrana = false;
char** talia;
pthread_mutex_t talia_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ruch_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t desc_mutex = PTHREAD_MUTEX_INITIALIZER;
struct karta* dobierane=NULL;
struct karta* zagrane=NULL; 
bool ruch_jak_zegar = true;
int dobieranie = 1;
bool zagrana = false;
int tab_desc[4];
int ile_na_rece = {7, 7, 7, 7};

struct thread_data_t
{
    int my_socket;
    int my_turn;
};

void bubblesort(int table[], int size)
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
}

void tasowanie (char** talia)
{
    int ilosc = sizeof(talia)/sizeof(talia[0]);
    int T[ilosc];
    int T2[ilosc];
    srand(time(0));
    for (int i=0; i<ilosc; i++)
    {
        T[i]=rand()%10000;
        T2[i]=T[i];
    }
    bubblesort(&T, ilosc);
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
                free(temp);
                break;
            }
        }
    }
}

void wysylanie_komunikatu(char* komunikat)
{
    for (int i=0; i<4; i++)
    {
        write(tab_desc[i], komunikat, 4);
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

bool sprawdzenie_komunikatu (char* komunikat)
{
    char* wierzch=malloc(3*sizeof(char));
    strcpy(wierzch, pop(zagrane));
    char* zagrana_karta=malloc(3*sizeof(char));
    strcpy(zagrana_karta, komunikat[1]+komunikat[2]+komunikat[3]);
    zagrana = false;
    //zagranie zwykłej
    if ((dobieranie==1 && komunikat[2]=='0' && komunikat[1]==wierzch[0] && atoi(wierzch[2])==(atoi(komunikat[3])+1)%10)|| (dobieranie==1 && komunikat[2]=='0' && atoi(komunikat[0])==kolej && (komunikat[3]==wierzch[2] || komunikat[1]==wierzch[0])) ) 
    {
        zagrane=push(zagrane, wierzch);
        zagrane=push(zagrane, zagrana_karta);
        wysylanie_komunikatu();
        zagrana = true;
    }
    //zagranie zmiany kolejności tury
    else if (komunikat[2]=='1' && komunikat[3]=='0' && dobieranie==1 && atoi(komunikat[0])==kolej && ((wierzch[1]==komunikat[2] && wierzch[2]==komunikat[3]) || komunikat[1]==wierzch[0])) 
    {
        if (ruch_jak_zegar) ruch_jak_zegar=false;
        else ruch_jak_zegar=true;
        zagrane=push(zagrane, wierzch);
        zagrane=push(zagrane, zagrana_karta);
        wysylanie_komunikatu();
        zagrana = true;
    }
    //zagranie stopu
    else if (komunikat[2]=='1' && komunikat[3]=='1' && dobieranie==1 && atoi(komunikat[0])==kolej && ((wierzch[1]==komunikat[2] && wierzch[2]==komunikat[3]) || komunikat[1]==wierzch[0]))
    {
        zagrane=push(zagrane, wierzch);
        zagrane=push(zagrane, zagrana_karta);
        wysylanie_komunikatu();
        zagrana = true;
    }
    //zagranie +2
    else if (komunikat[2]=='1' && komunikat[3]=='2' && atoi(komunikat[0])==kolej && ((wierzch[1]==komunikat[2] && wierzch[2]==komunikat[3]) || (komunikat[1]==wierzch[0] && dobieranie==1)))
    {
        zagrane=push(zagrane, wierzch);
        zagrane=push(zagrane, zagrana_karta);
        if (dobieranie==1)
        {
            dobieranie+=1;
        }
        else dobieranie+=2;
        zagrana = true;
    }
    //zagranie +4
    else if (komunikat[2]=='1' && komunikat[3]=='4' && atoi(komunikat[0])==kolej && (komunikat[2]!='1' || komunikat[3]!='2' || dobieranie==1))
    {
        zagrane=push(zagrane, wierzch);
        zagrane=push(zagrane, zagrana_karta);
        if (dobieranie==1)
        {
            dobieranie+=3;
        }
        else dobieranie+=4;
        zagrana = true;
    }
    //zagranie zmiany koloru
    else if (komunikat[2]=='1' && komunikat[3]=='3' && dobieranie==1 && atoi(komunikat[0])==kolej)
    {
        zagrane=push(zagrane, wierzch);
        zagrana = true;
    }
    //komunikat o dobraniu kart
    else if (atoi(komunikat[0])==kolej)
    {
        zagrane=push(zagrane, wierzch);
        zagrana = true;
        wysylanie_komunikatu();
        dobieranie=1;
    }
    if (zagrana) {
        if (ruch_jak_zegar) kolej+=1;
        else kolej -=1;}
    free(wierzch);
    //trzeba jakoś sprawdzić czy jest wygrana
    //trzeba ogarnac jaki komunikat gdy lezy czarna karta
}

void *ThreadBehavior(void *t_data)
{ 
    pthread_detach(pthread_self());
    struct thread_data_t *th_data = (struct thread_data_t*)t_data;
    int desc = th_data->my_socket;
    pthread_mutex_lock(&talia_mutex);
    if (th_data->my_turn==0)
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
    }
    write(desc, talia[28], 3);
    for (int i=th_data->my_turn*7; i<(th_data->my_turn+1)*7;i++)
    {
        write(desc, talia[i], 3);
    }
    if (th_data->my_turn==3)
    {   
        for (int i=29;i<68;i++)
        {
            dobierane=push(dobierane, talia[i]);
        }
    }
    write(desc, nick[0], 20);
    pthread_mutex_unlock(&talia_mutex);
    char* komunikat=malloc(4*sizeof(char));
    while(!wygrana)
    {
        read(desc, komunikat, 4);
        pthread_mutex_lock(&ruch_mutex);
        if (!wygrana)
        {
            if (sprawdzenie_komunikatu(komunikat))
            {

            }
        }
        pthread_mutex_unlock(&ruch_mutex);
    }
    
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
    tasowanie(talia);
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