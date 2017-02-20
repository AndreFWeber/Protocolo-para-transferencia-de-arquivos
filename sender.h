#ifndef SENDER
#define SENDER
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>
#include <arpa/inet.h>
#include <string> 
#include <unistd.h>



using namespace std;
class Sender {

public:


	//Construtor da Classe Sender
	Sender();
	Sender(string receiverIP);
	
	//Destrutor da Classe Sender 
	virtual ~Sender(void);

	//envia o arquivo
	int sendFile(string filePath);


	//Inicia uma conexao com o receptor 
	int connectTo();

	//Envia msgs de config. 
	void sendMsg(string msg);


protected:
	//Estrutura para endereco
	struct sockaddr_in		addr; 

	//Porta padrao deste protocolo
	unsigned int			porta_TCP = 2017; 
 	
	// descriptor do socket
	int				sockFd;

	//Verificador de conexao (Verifica se a conexao com o receptor foi OK)
	int 				checker;

	//File
	FILE *File;

	//Caminho ate o arquivo a ser enviado
	string filePath;

	//IP da maquina que recebera o arquivo
	string IP;
	
	
	//Cria socket UDP  
	int open_socket();




private:


}; // Sender {

#endif
