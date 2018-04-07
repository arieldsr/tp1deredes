#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>

//Declaração da função que subtrai as duas estruturas de timeval. Função ao final do código.
void subtracao_timeval (struct timeval *resultado, struct timeval *final, struct timeval *inicial);

//Início da função principal
int main(int argc, char	 *argv[]){
	//Processamento dos argumentos da linha de comando	
	char *host_do_servidor = malloc(sizeof(argv[1]));
	int porta_servidor = atoi(argv[2]);
	char *nome_do_arquivo = malloc(sizeof(argv[3]));
	int tamanho_buffer = atoi(argv[4]);
	struct timeval tempo_inicial, tempo_final, resultado;
	

	//A limpeza das variáveis é feita para eliminar o lixo nas mesmas. 
	memset(host_do_servidor, 0, sizeof(host_do_servidor));
	strcpy(host_do_servidor, argv[1]);
	
	memset(nome_do_arquivo, 0, sizeof(nome_do_arquivo));
	strcpy(nome_do_arquivo, argv[3]);

	
	//Início contagem de tempo
	gettimeofday(&tempo_inicial, NULL);
	
	
	//Outras variáveis importante
	struct sockaddr_in endereco_servidor;
	
	int socket_cliente;

	char *buffer = malloc(tamanho_buffer);
	memset(buffer, 0, tamanho_buffer);


	//Criação de socket para o cliente
	socket_cliente = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_cliente == -1){
		printf("Erro na criação do socket.");
		free(buffer);
		free(nome_do_arquivo);
		free(host_do_servidor);
		return 0;
	}


	//Abertura de conexão com o servidor
	endereco_servidor.sin_family = AF_INET;
	endereco_servidor.sin_addr.s_addr = inet_addr(host_do_servidor); 
	endereco_servidor.sin_port = htons(porta_servidor);
	memset(&endereco_servidor.sin_zero, 0, 8);

	if(connect(socket_cliente, (struct sockaddr*) &endereco_servidor, sizeof(endereco_servidor))){
		printf("Servidor não encontrado.");
		close(socket_cliente);
		free(buffer);
		free(nome_do_arquivo);
		free(host_do_servidor);
		return 0;
	}


	//Espera da resposta/ACK do servidor.	
	int tamanho_resposta_recebida;
	do{
		tamanho_resposta_recebida = recv(socket_cliente, buffer, tamanho_buffer, 0);
	}while(tamanho_resposta_recebida < 0);
	buffer[tamanho_resposta_recebida] = 0x00;
	printf("%s\n", buffer);
	memset(buffer, 0, tamanho_resposta_recebida);


	//Envio do nome do arquivo/ACK para o servidor
	strcpy(buffer, nome_do_arquivo);
	send(socket_cliente, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));


	//Abertura do arquivo
	FILE* f = fopen(nome_do_arquivo, "w+");
	if(f == NULL){
		printf("Erro ao abrir arquivo.");
		close(socket_cliente);
		free(buffer);
		free(nome_do_arquivo);
		free(host_do_servidor);
		return 0;
	}


	//Leitura e inserção dos dados enviados pelo servidor
	int bytes_recebidos = 0;
	while(tamanho_resposta_recebida > 0){
		tamanho_resposta_recebida = recv(socket_cliente, buffer, tamanho_buffer, 0);
		buffer[tamanho_resposta_recebida] = 0x00;
		fwrite(buffer, sizeof(char), tamanho_resposta_recebida, f);
		memset(buffer, 0, tamanho_resposta_recebida);
		bytes_recebidos += tamanho_resposta_recebida;
	}
	

	//Fechar arquivos e conexões
	fclose(f);
	close(socket_cliente);
	free(buffer);
	free(nome_do_arquivo);
	free(host_do_servidor);


	//Final contagem de tempo
	gettimeofday(&tempo_final, NULL);
	subtracao_timeval(&resultado, &tempo_final, &tempo_inicial);

	int tempo_total_segundos = resultado.tv_sec;
	int tempo_total_microsegundos = resultado.tv_usec;
	float tempo_total = (float) tempo_total_segundos + (float) tempo_total_microsegundos/10000000;
	float vazao_kbps = (float) bytes_recebidos/(tempo_total*1000);

	printf("Buffer = %5u byte(s), %10.2f kbps (%u bytes em %3u.%06u s)\n", tamanho_buffer, vazao_kbps, bytes_recebidos, tempo_total_segundos, tempo_total_microsegundos);

	return 1;
}

//Função que subtrai struct timeval
void subtracao_timeval (struct timeval *resultado, struct timeval *final, struct timeval *inicial){
	resultado->tv_sec = final->tv_sec - inicial->tv_sec;
	
	/*Se os microssegundos finais forem menores que os iniciais, pega 1 emprestado*/
  	if (final->tv_usec < inicial->tv_usec) {
		resultado->tv_sec--;
		resultado->tv_usec = (final->tv_usec + 1000000) - inicial->tv_usec;
	}
	else 
		resultado->tv_usec = final->tv_usec - inicial->tv_usec;
}