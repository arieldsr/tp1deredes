#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>

#define N 50 //Tamanho do nome do arquivo

struct sockaddr_in servidor;
struct sockaddr_in cliente;
struct timeval tempo_inicio, tempo_final;
socklen_t size = sizeof(cliente);

int main(int argc, char	 *argv[]){
	//Início contagem de tempo
	gettimeofday(&tempo_inicio, NULL);
	
	int porta_servidor = atoi(argv[1]);
	int tamanho_buffer = atoi(argv[2]);

	int socket_servidor;
	int socket_cliente;

	char nome_arquivo_enviado[N];
	char *buffer = malloc(tamanho_buffer);
	
	//Limpeza das variáveis
	memset(&nome_arquivo_enviado, 0, N);
	memset(buffer, 0, tamanho_buffer);

	socket_servidor = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_servidor == -1){
		printf("Erro na criação do socket.");
		return 0;
	}

	printf("Teste 1: Criou o socket.\n");

	servidor.sin_family = AF_INET;
	servidor.sin_port = htons(porta_servidor);	
	memset(servidor.sin_zero, 0, 8);

	int erro_bind = bind(socket_servidor, (struct sockaddr*) &servidor, sizeof(servidor)); 

	if (erro_bind == -1){
		printf("Erro no bind.\n");
		return 0;
	}

	printf("Teste 2: Fez o bind.\n");

	listen(socket_servidor, 1); //O segundo parâmetro diz quantas conexões são aceitas

	socket_cliente = accept(socket_servidor, (struct sockaddr*) &cliente, &size);

	if(socket_cliente == -1){
		printf("Erro no accept.\n");
		return 0;
	}

	printf("Teste 3: Fez o accept.\n");
	
	char mensagem_enviada[25];
	strcpy(mensagem_enviada, "Conectado ao servidor. Insira o nome do arquivo:\0");
	
	if (send(socket_cliente, mensagem_enviada, strlen(mensagem_enviada), 0))
		printf("Aguardando resposta do cliente.\n");
	else{
		printf("Erro ao enviar mensagem de sucesso ao cliente.\n");
	}
	
	int resposta_foi_recebida;
	do{
		resposta_foi_recebida = recv(socket_cliente, nome_arquivo_enviado, porta_servidor, 0);
	}while(resposta_foi_recebida < 0);

	strtok(nome_arquivo_enviado, "\n"); //Retorna apenas o pedaço da string anterior ao '\n'.

	printf("Teste 4: O nome do arquivo é %s.\n", nome_arquivo_enviado);
		
	FILE *f = fopen((const char*)nome_arquivo_enviado, "r");
	
	if (f == NULL){
		printf("Erro ao abrir o arquivo. Conexão encerrada.");
		close(socket_cliente);
		close(socket_servidor);
		free(buffer);
		return 0;
	}

	int tamanho_string, bytes_enviados = 0;
	printf("Arquivo aberto.\n");
	tamanho_string = fread(buffer, sizeof(char), tamanho_buffer, f);
	
	while(tamanho_string > 0){
		send(socket_cliente, buffer, tamanho_string, 0);
		memset(buffer, 0, sizeof(buffer));
		bytes_enviados += tamanho_string;
		tamanho_string = fread(buffer, sizeof(char), tamanho_buffer, f);
	}
	printf("Teste 5: Arquivo enviado ao cliente.");
		
	fclose(f);
	close(socket_cliente);
	close(socket_servidor);
	free(buffer);

	gettimeofday(&tempo_final, NULL);

	int tempo_total = tempo_final.tv_sec - tempo_inicio.tv_sec;

	printf("\nArquivo: %s. Bytes enviados: %i. Tempo gasto em segundos: %i.", nome_arquivo_enviado, bytes_enviados, tempo_total);	
	
	return 1;
}