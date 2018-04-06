#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//inclusões específicas para o tp
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
# include <unistd.h>

struct sockaddr_in server;
socklen_t size = sizeof(server);

int main(int argc, char	 *argv[]){


	int len = atoi(argv[1]); //tamanho Buffer
	int port = atoi(argv[2]); // porta socket
	int host = atoi(argv[3]); //host do servidor
	char name [30]; // nome do arquivo
	memset(name, 0x0,30);//Limpa a variável do nome
	strcpy(name, argv[4]); // salva na variável nome o nome do arquivo

	//Sockets cliente-servidor
	int socket_client;
	int socket_server;
	
;	
	//Buffer de recepção do servidor, com tamanho len
	char buffer[len]; //Buffer de recepção do servidor, com tamanho len
	
	memset(buffer, 0x0, len); // limpa o buffer de qualquer lixo
	
	//As limpezas são necessárias porque o valor recebido pode ser menor que o tamanho das variáveis, guardando lixo

	socket_client = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_client == -1){
		printf("Erro na criação do socket");
	}
	printf("teste1: criou o socket\n");
		
		server.sin_family= AF_INET;
		server.sin_port = htons(port);	
		server.sin_addr.s_addr = inet_addr(host); // não tenho certeza se é isso, mas pega o local do servidor e coloca no endereço de envio do socket
		memset(server.sin_zero, 0x0, 8);

 

	 socket_server = connect(socket_client, (struct sockaddr*)&server, size);

	if( socket_server == -1){
		printf("Erro no connect");
	}
	printf("teste3: fez o accept\n");

		send(socket_server, name, 30, 0); // envia o nome do arquivo para o servidor
		
	
	int aux2 = recv(socket_server, buffer, len, 0); 

	while (aux2 <-1);

	while(aux2 >= 0){
	FILE *f = fopen((const char*)name, "w"); // abre o arquivo especificado para escrita

		fwrite(buffer, sizeof(char), len, f);
		fclose(f);
	aux2 = recv(socket_server, buffer, len, 0); 
	}		
		
	
	close(socket_server);
	close(socket_client);
	
return 0; 
}
