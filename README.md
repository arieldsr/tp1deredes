# Trabalho Prático 1 - Cliente-servidor TCP
Universidade Federal de Minas Gerais, abril de 2018 </br>
Engenharia de Controle e Automação </br>
Ariel de Souza Ribeiro - http://github.com/arieldsr </br>
Caio Souza Pimentel - https://github.com/caiospimentel</br>

## Problema
  Implementar um par de programas que operem no modelo cliente-servidor e exercitem tanto a transmissão unidirecional quanto a comunicação do tipo requisição-resposta sobre o protocolo TCP. A implementação
deve utilizar a biblioteca de sockets do Unix (Linux).

## Operação
  O processo de comunicação entre cliente e servidor deverá seguir um padrão simples, conforme ilustrado
a seguir:
  ```
  **Cliente:**
  processa argumentos da linha de comando:
  host_do_servidor porta_servidor nome_arquivo tam_buffer
  chama gettimeofday para tempo inicial
  faz abertura ativa a host_do_servidor : porto_servidor
  envia string com nome do arquivo (terminada em zero)
  abre arquivo que vai ser gravado - pode ser fopen(nome,"w+")
  **loop** recv buffer até que receba zero bytes ou valor negativo
  escreve bytes do buffer no arquivo (fwrite)
  atualiza contagem de bytes recebidos
  **fim_loop**
  fecha conexão e arquivo
  chama gettimeofday para tempo final e calcula tempo gasto
  imprime resultado: "Buffer = %5u byte(s), %10.2f kbps (%u bytes em %3u.%06u s)"
  **fim_cliente.**
  ```
  
  ```
  **Servidor:**
  processa argumentos da linha de comando:
  porta_servidor tam_buffer
  faz abertura passiva e aguarda conexão
  recebe, byte a byte até o zero, a string com nome do arquivo
  abre arquivo que vai ser lido – pode ser fopen(nome,"r")
  se deu erro, fecha conexão e termina
  **loop** lê o arquivo, um buffer por vez até fread retornar zero
  envia o buffer lido
  se quiser, contabiliza bytes enviados
  **fim_loop**
  fecha conexão e arquivo
  chama gettimeofday para tempo final e calcula tempo gasto
  se quiser, imprime nome arquivo e número de bytes enviados
  **fim_servidor.**
  ```
  
  De forma resumida, o cliente deve se conectar ao servidor, enviar um string com o nome do arquivo desejado, receber o arquivo um buffer de cada vez e salvar os dados no disco à medida que eles chegam.
Quando não houver mais bytes para ler, o cliente fecha a conexão e o arquivo e gera uma linha com os dados da execução. O servidor por sua vez deve operar de forma complementar.
