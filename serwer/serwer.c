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

int kolej = 0; //0-3
int gracze = 0;
bool wygrana = false;

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
    int ilosc = sizeof(talia);
    int T[ilosc];
    int T2[ilosc];
    srand(time(0));
    for (int i=0; i<ilosc; i++)
    {
        T[i]=rand()%10000;
        T2[i]=T[i];
    }
    bubblesort(&T, ilosc);
    for (int j=0;j<ilosc;j++)
    {
        for (int k=0; k<ilosc;k++)
        {
            if (T[j]==T2[k])
            {
                char* temp=malloc(3*sizeof(char));
                temp=talia[j];
                talia[j]=talia[k];
                talia[k]=temp;
                break;
            }
        }
    }
}

void *ThreadBehavior(void *t_data)
{ 
    pthread_detach(pthread_self());
    struct thread_data_t *th_data = (struct thread_data_t*)t_data;
    int desc = th_data->my_socket;
    char* buf=malloc(3*sizeof(char));
    char** talia = malloc(68*sizeof(char*));
    for (int i=0; i<68;i++)
    {
        char* talia[i]=malloc(3*sizeof(char*));
    }
    talia[0]="y00";
    talia[1]="y01";
    talia[2]="y02";
    talia[3]="y03";
    talia[4]="y04";
    talia[5]="y05";
    talia[6]="y06";
    talia[7]="y07";
    talia[8]="y08";
    talia[9]="y09";
    talia[10]="y10";
    talia[11]="y10"; //->zmiana kolejnosci ruchu
    talia[12]="y11"; //->blokowanie
    talia[13]="y11";
    talia[14]="y12"; //->+2
    talia[15]="y12";
    talia[16]="n00";
    talia[17]="n01";
    talia[18]="n02";
    talia[19]="n03";
    talia[20]="n04";
    talia[21]="n05";
    talia[22]="n06";
    talia[23]="n07";
    talia[24]="n08";
    talia[25]="n09";
    talia[26]="n10";
    talia[27]="n10"; //->zmiana kolejnosci ruchu
    talia[28]="n11"; //->blokowanie
    talia[29]="n11";
    talia[30]="n12"; //->+2
    talia[31]="n12";
    talia[32]="c00";
    talia[33]="c01";
    talia[34]="c02";
    talia[35]="c03";
    talia[36]="c04";
    talia[37]="c05";
    talia[38]="c06";
    talia[39]="c07";
    talia[40]="c08";
    talia[41]="c09";
    talia[42]="c10";
    talia[43]="c10"; //->zmiana kolejnosci ruchu
    talia[44]="c11"; //->blokowanie
    talia[45]="c11";
    talia[46]="c12"; //->+2
    talia[47]="c12";
    talia[48]="z00";
    talia[49]="z01";
    talia[50]="z02";
    talia[51]="z03";
    talia[52]="z04";
    talia[53]="z05";
    talia[54]="z06";
    talia[55]="z07";
    talia[56]="z08";
    talia[57]="z09";
    talia[58]="z10";
    talia[59]="z10"; //->zmiana kolejnosci ruchu
    talia[60]="z11"; //->blokowanie
    talia[61]="z11";
    talia[62]="z12"; //->+2
    talia[63]="z12";
    talia[64]="s00"; //->zmiana koloru
    talia[65]="s00"; //->zmiana koloru
    talia[66]="s01"; //->+4
    talia[67]="s01"; //->+4
    tasowanie(&talia);
    read(desc,buf,10);
    //write(desc, "HI", 2);
    //char* buf=malloc(10*sizeof(char));
    //read(desc, buf, 10);
    /*
    określenie tury -> kto może zagrać kartę
    odebranie komunikatu jaka karta ma być zagrana i sprawdzenie czy jest dobra -> jak nie to nic sie nie dzieje

    */
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

    while(1)
    {
        connection_socket_descriptor = accept(server_socket_descriptor, NULL, NULL);
        if (connection_socket_descriptor < 0)
        {
            fprintf(stderr, "%s: Błąd przy próbie utworzenia gniazda dla połączenia.\n", argv[0]);
            exit(1);
        }
        handleConnection(connection_socket_descriptor, gracze);
        gracze++;
    }
}