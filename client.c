#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORTA 2000
#define TAM 300
#define TRUE 1
#define FALSE 0

struct sockaddr_in conexao;

int main(){

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);           //criação do socket (IPV4, TCP/IP, 0)
    
    socklen_t len = sizeof(conexao);                        //recupera o tamanho em bytes da estrutura do socket
    int slen, rec, sair = FALSE;
    char mensagem[TAM], resposta[TAM];
    char ip[32];
    strcpy(ip, "127.0.0.1");

    if(sockfd == -1){
        perror("socket ");
        exit(1);
    }else{
        printf("Socket criado!\n");
    }


    conexao.sin_family = AF_INET;                            //define a familia de protocolos
    conexao.sin_port = htons(PORTA);                        //endereço da porta da camada de transporte

    //inet_pton == p -> string pointer  +  to -> para  + n -> network byte order
    inet_pton(AF_INET, ip, &conexao.sin_addr);              //converte o endereço de ip para um network byte order  (IPV4, IP, Endereço de rede de 32 bits )
    memset(conexao.sin_zero, 0x0, 8);                       //copia o caracter 0 para os n primeiros caracteres da string apontada

    if(connect(sockfd, (struct sockaddr *)&conexao, len) < 0){      //testa a conexão do socket
        perror("connect ");
        exit(1);
    }

    rec = recv(sockfd, resposta, TAM, 0);                   //recebimento de dados pelo socket
    resposta[rec] = '\0';
    printf("Servidor: %s\n", resposta);
    //printf("Ponha * para mandar uma mensagem. Escreva # para terminar a conexão\n");


    do{
        printf("Client: ");
        fgets(mensagem, TAM, stdin);            //leitura da mensagem (string a ser lida, tamanho maxímo, entrada - teclado)
        mensagem[strlen(mensagem)-1] = '\0';

        send(sockfd, mensagem, strlen(mensagem), 0);    //envia a mensagem


        rec = recv(sockfd, resposta, TAM, 0);
        resposta[rec] = '\0';
        printf("Servidor: %s\n", resposta);

        if(*mensagem == '#' || *resposta == '#'){
                sair = TRUE;
        }
    }while(!sair);

    close(sockfd);
    printf("Cliente Encerrado! \n");
    return 0;
}   