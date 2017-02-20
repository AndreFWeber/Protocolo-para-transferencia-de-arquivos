#ifndef PROTOCOLO
#define PROTOCOLO
#include "receiver.h"
#include "sender.h"


using namespace std;
class Protocolo {

public:

	//Construtor da Classe Protocolo
	Protocolo();
	Protocolo(string IPlocal, string IPdestino);
	//Destrutor da Classe Protocolo 
	virtual ~Protocolo(void);

	void set_connFd(int value);
	//envia o arquivo
	int send(string fileName, string filePath);

	int receive();



private:
	//Estrutura para endereco
	struct sockaddr_in	addr; 

	//Porta padrao deste protocolo
	unsigned int		porta_TCP = 2017; 
 	
	// descriptor do socket
	int			sockFd;


	//Verificador de conexao (Verifica se a conexao com o receptor foi OK)
	int 			checker;

	//Tamanho do arquivo
	int 			fileSize;

	//Nome do arquivo
	string 			fileName;

	//Um objeto para enviar mensagens e o arquivo
	Sender sender;	

	//Recebe as mensagens trocadas durante a operacao do protocolo
	int receiveMsg(int option);


}; // Protocolo {

#endif
