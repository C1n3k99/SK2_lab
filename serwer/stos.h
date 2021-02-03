#ifndef STOS_H
#define STOS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct karta
{
    char rodzaj[3];
    int rozmiar;
    struct karta* nastepna;
};

typedef struct karta* wskaznik;

wskaznik create()
{
    wskaznik temp; 
    temp = (wskaznik)malloc(sizeof(struct karta)); 
    temp->nastepna = NULL;
    return temp;
}

wskaznik push(wskaznik head, char* rodzaj)
{
    wskaznik temp;
    temp=create();
    strcpy(temp->rodzaj, rodzaj);
    temp->nastepna=head;
    return temp;
}


wskaznik pop(wskaznik head)
{
    if (head == NULL)
        return head;
    wskaznik head2 = head->nastepna;
    free(head);
    return head2;
}
#endif