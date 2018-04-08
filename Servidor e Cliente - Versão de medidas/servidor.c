#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>

#define N 50 //Tamanho do nome do arquivo enviado

//Declaração da função que subtrai as duas estruturas de timeval. Função ao final do código.
void subtracao_timeval (struct timeval *resultado, struct timeval *final, struct timeval *inicial);

int main(int argc, char	 *argv[]){

	int n = 100;
	int numero_mensagens = 0;
	float vetor_desvio[n];
	float media_tempos = 0;
	float tempo_global = 0;
	

	for(int i = 0; i<n; i++){ //proporciona várias iterações do código

	//Processamento dos argumentos da linha de comando
	int porta_servidor = atoi(argv[1])+i;
	int tamanho_buffer = atoi(argv[2]);
	struct timeval tempo_inicial, tempo_final, resultado;

	//Início contagem de tempo
	gettimeofday(&tempo_inicial, NULL);
	

	//Outras variáveis importantes	
	struct sockaddr_in endereco_servidor, endereco_cliente;
	socklen_t size = sizeof(endereco_cliente);

	int socket_servidor, socket_cliente;

	char *buffer = malloc(tamanho_buffer);
	char nome_arquivo_enviado[N];
	memset(buffer, 0, tamanho_buffer);
	memset(nome_arquivo_enviado, 0, N);


	//Criação de socket para o servidor
	socket_servidor = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_servidor == -1){
		printf("Erro na criação do socket.");
		free(buffer);
		return 0;
	}


	//Abertura passiva aguardando conexão
	endereco_servidor.sin_family = AF_INET;
	endereco_servidor.sin_port = htons(porta_servidor);	
	memset(endereco_servidor.sin_zero, 0, 8);

	int erro_bind = bind(socket_servidor, (struct sockaddr*) &endereco_servidor, sizeof(endereco_servidor)); 

	if (erro_bind == -1){
		printf("Erro no bind.\n");
		close(socket_servidor);
		free(buffer);
		return 0;
	}

	listen(socket_servidor, 1); //O segundo parâmetro diz quantas conexões são aceitas
	socket_cliente = accept(socket_servidor, (struct sockaddr*) &endereco_cliente, &size);

	if(socket_cliente == -1){
		printf("Erro ao aceitar conexão.\n");
		close(socket_servidor);
		free(buffer);
		return 0;
	}

	

	//Envia ACK
	strcpy(buffer, "OK.");
	send(socket_cliente, buffer, strlen(buffer), 0);
	memset(buffer, 0, sizeof(buffer));
	

	//Espera ACK e nome do arquivo	
	int tamanho_resposta_recebida;
	do{
		tamanho_resposta_recebida = recv(socket_cliente, nome_arquivo_enviado, N, 0);
	}while(tamanho_resposta_recebida < 0);

	//Abertura do arquivo
	FILE *f = fopen((const char*)nome_arquivo_enviado, "r");
	if (f == NULL){
		printf("Erro ao abrir o arquivo.");
		close(socket_cliente);
		close(socket_servidor);
		free(buffer);
		return 0;
	}

		
		//Leitura e envio do arquivo
		int tamanho_string, bytes_enviados = 0;
		tamanho_string = fread(buffer, sizeof(char), tamanho_buffer, f);
		while(tamanho_string > 0){
			send(socket_cliente, buffer, tamanho_string, 0);
			memset(buffer, 0, sizeof(buffer));
			bytes_enviados += tamanho_string;
			tamanho_string = fread(buffer, sizeof(char), tamanho_buffer, f);


			//calculo do número de mensagens enviadas
			numero_mensagens = numero_mensagens+1; 


		}


	


		//Final contagem de tempo
		gettimeofday(&tempo_final, NULL);
		subtracao_timeval(&resultado, &tempo_final, &tempo_inicial);
	
		int tempo_total_segundos = resultado.tv_sec;
		int tempo_total_microsegundos = resultado.tv_usec;
		float tempo_total = tempo_total_segundos + (float) tempo_total_microsegundos/10000000;

		//Calculo do tempo de execução de todas as iterações
		tempo_global = tempo_global+tempo_total;

		//Calculo do desvio padrão
		vetor_desvio[i] = tempo_total;
		media_tempos = media_tempos+tempo_total;

		

	
		

		printf("Arquivo: %s. Bytes enviados: %i Tempo gasto: %f\n", nome_arquivo_enviado, bytes_enviados, tempo_total);	
		

	

	//Fechar arquivo e conexões	
	fclose(f);
	close(socket_cliente);
	close(socket_servidor);
	free(buffer);

}
	
	media_tempos = media_tempos/n;
	double soma_quadratica = 0;

	for(int j=0; j<n; j++){
		soma_quadratica = soma_quadratica + pow((vetor_desvio[j]-media_tempos),2);
	}

	double desvio = sqrt(soma_quadratica/n);






	printf("Número total de mensagens enviadas:%i\n", numero_mensagens);
	numero_mensagens = numero_mensagens/n;
	printf("Número de mensagens enviadas por iteração:%i\n", numero_mensagens);
	printf("Tempo de todas as iterações:%f\n", tempo_global);
	printf("Média dos tempos de cada iteração:%f\n", media_tempos);
	printf("Desvio padrão do tempo:%f\n", desvio);

	
	
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