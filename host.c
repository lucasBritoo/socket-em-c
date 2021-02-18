#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORTA 2000
#define TAM 256
#define TRUE 1
#define FALSE 0

struct sockaddr_in local;
struct sockaddr_in remoto;

int main(){
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	socklen_t len = sizeof(remoto);
	int client, rec;
	int sair = FALSE;
	char mensagem[TAM], resposta[TAM];

	if(sockfd == -1){
		perror("socket ");
		exit(1);
	}else{
		printf("Socket criado!\n");
	}

	local.sin_family = AF_INET;
	local.sin_port = htons(PORTA);
	memset(local.sin_zero, 0x0, 8);

	if(bind(sockfd, (struct sockaddr *)&local, sizeof(local)) == -1){
		perror("bind ");
		exit(1);
	}

	if(listen(sockfd, 1) == -1){
		perror("Listen ");
		exit(1);
	}

	printf("Aguardando cliente...\n");

	client = accept(sockfd, (struct sockaddr *)&remoto, &len);
	if(client == -1){
		perror("accept ");
		exit(1);
	}

	printf("Cliente conectado!\n\n");
	printf("Ponha * para mandar uma mensagem. Escreva # para terminar a conexão\n");
	strcpy(mensagem, "Servidor conectado\n");
	send(client, mensagem, strlen(mensagem), 0);


	do{
		rec = recv(client, resposta, TAM, 0);
		resposta[rec] = '\0';
		printf("Cliente: %s\n", resposta);

		
		printf("Servidor: ");
		fgets(mensagem, TAM, stdin);
		mensagem[strlen(mensagem)-1] = '\0';
		send(client, mensagem, strlen(mensagem),0);	

		if(*mensagem == '#' || *resposta == '#'){
				sair = TRUE;
		}
	}while(!sair);

	close(client);
	close(sockfd);

	printf("Servidor Encerrado! \n");

	return 0;
}