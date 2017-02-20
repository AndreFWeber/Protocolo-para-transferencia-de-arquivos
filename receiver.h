#ifndef RECEIVER
#define RECEIVER
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

class Receiver {


public:


	//Construtor da Classe Receiver
	Receiver();
	Receiver(string IPlocal);
	//Destrutor da Classe Receiver 
	virtual ~Receiver(void);


	void *receiver_(void *voiD);

	//Aceita conexao de uma 
	int acceptConnection();

	//Recebe o arquivo
	int receiveFile(int fileSize, string fileName);
protected:
	//Estruturas para endereco
	struct sockaddr_in		cli_addr; 
	struct sockaddr_in		addr; 

	//Porta padrao deste protocolo
	unsigned int			porta_TCP = 2017; 
 	
	// descriptor do socket
	int				sockFd;

	//Descriptor do socket referente a conexao aceita
	int 				connFd;

	//File
	FILE *File;

	//IP da maquina local
	string meuIP = "127.0.0.1"; 
	
	//Buffer onde as mensagens serao recebidas
	char bufferMsgs[256];
	
	//Cria socket UDP  
	int open_socket();



	

private:


}; // Receiver {

#endif
