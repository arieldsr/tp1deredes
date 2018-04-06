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

	//Sockets cliente-servidor
	int socket_server;
	int socket_client;
	
	//Buffer de recepção do servidor, com tamanho len
	char buffer[len]; //Buffer de recepção do servidor, com tamanho len
	char name[30]; // Variável auxiliar, nome do arquivo buscado, tamanho arbitrário
	char *out_data = malloc(len*sizeof(char));//Variável auxiliar de envio de dados do arquivo ao cliente
	
	memset(buffer, 0x0, len); // limpa o buffer de qualquer lixo
	memset(name, 0x0, 30);//Limpa a variável do nome
	memset(out_data, 0x0, len);//limpa a variável de saída
	//As limpezas são necessárias porque o valor recebido pode ser menor que o tamanho das variáveis, guardando lixo

	socket_server = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_server == -1){
		printf("Erro na criação do socket");
	}
	printf("teste1: criou o socket\n");
	
		server.sin_family= AF_INET;
		server.sin_port = htons(port);
		memset(server.sin_zero, 0x0, 8);
		printf("%s", &server.sin_zero);	

	int aux1 = bind(socket_server, (struct sockaddr*)&server, sizeof(server)); 

	if( aux1 == -1){
		printf("Erro no bind\n");
	}
	printf("teste2:fez o bind\n");

	listen(socket_server, 1); 

	socket_client = accept(socket_server, (struct sockaddr*)&client, &client);

	if(socket_client == -1){
		printf("Erro np accept\n");
	}
	printf("teste3: fez o accept\n");
	
	
	aux2 = recv(socket_client, name, 30, 0);
	while(aux2<0); // espera até uma mensagem ser enviada
	
	printf("teste4 - Nome do arquivo: %s/n", name);
		
	FILE *f = fopen((constant char*)name, "r"); // abre o arquivo especificado para leitura
	int size_string;
	
	while(size_string = fread(out_data, sizeof(char), len, f){
		send(socket_client, out_data, size_string, 0);
		memset(out_data,0x0,0);//limpa a variável de saída a cada iteração
	}
	printf("teste5: arquivo enviado ao cliente);
		
	fclose(f);
	close(socket_client);
	close(socket_server);
	free(data_out);
	
return = 0; 
}
