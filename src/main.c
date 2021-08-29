// #include "DataLinkLayer.h"
// #include "Application.h"
// #include "SocketConnection.h"

#include <stdio.h>

// TODO REMOVER INCLUDES
#include <time.h>
#include <mqueue.h>

int main(int argc, char ** argv)
{
    struct mq_attr props_fila;
    props_fila.mq_maxmsg = 10;
    props_fila.mq_maxmsg = 100;
    mqd_t fila = mq_open("/projetofrc", O_CREAT | O_RDWR, O_RDWR, &props_fila);

    printf("E para escrever na fila, L para ler da fila\n");
    char c;
    scanf(" %c", &c);
    if (c == 'E') {
        char msg[100];
        scanf("%s", msg);
        printf("%d\n", mq_send(fila, msg, sizeof(msg), 0));
    } else {
        char msg[100];
        unsigned int m_prio;
        printf("%d\n", mq_receive(fila, msg, sizeof(msg), &m_prio));
        printf("Mensagem recebida da fila: %s\n", msg);
    }
    return 0;
}