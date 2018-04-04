#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//inclusões específicas para o tp
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet.h>

struct sockaddr_in server;
struct sockaddr_in client;
int size = sizeof(client);

int main(int argc, chat **argv){


	int len = atoi(argv[1]); //tamanho buffer
	int port = atoi(argv[2]); // porta socket


	int socket_server;
	int socket_client;

	socket_server = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_server == -1){
		printf("Erro na criação do socket");
	}
	printf("teste1: criou o socket\n");


		server.sin_family= AF_INET;
		server.sin_port = htons(port);
		server.sin_zero = 00000000;
		printf("%s", &server.sin_zero);

	int aux1 = bind(socket_server, (struct sockaddr*)&server, sizeof(server)); 

	if( aux1 == -1){
		printf("Erro no bind\n");
	}
	printf("teste2:fez o bind\n");

	listen(socket_server, 1); 

	socket_client = accept(socket_server, (struct sockaddr*)&client, &size);

	if(socket_client == -1){
		printf("Erro np accept\n");
	}
	printf("teste3: fez o accept\n");





return = 0; 
}