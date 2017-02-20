//g++ *.cpp -lpthread -std=c++11 -std=c++0x -o protocolo_FTP_type

#include "protocolo.h"

//Descriptor do socket referente a conexao aceita
int 			connFd = 0;

//Um objeto para receber mensagens e o arquivo	
Receiver receiver;

void *receive_(void *voiD) {
	connFd = receiver.acceptConnection();
}


//Construtor da Classe Protocolo
Protocolo::Protocolo(){}
Protocolo::Protocolo(string IPlocal, string IPdestino){
	//Um objeto para receber mensagens e o arquivo	

	pthread_t receiverT;
	pthread_create(&receiverT, NULL, receive_, NULL); 
	//pthread_join(receiverT, NULL);


	//Um objeto para enviar mensagens e o arquivo
	Sender aux(IPdestino); 	
	sender = aux;	

	Receiver auxR(IPlocal);
	receiver = auxR;
}
	//Destrutor da Classe Protocolo 
Protocolo::~Protocolo(void){}

/*
	option:
		1 - Recebe a resposta do ACC enviado	
		2 - Recebe a confirmacao de que o nome do arquivo foi recebido e a tranferencia pode ser iniciada	
		3 - Recebe a confirmacao que o arquivo foi recebido com sucesso

		4 - Verifica se um RQST foi recebido
		5 - Recebe o nome do arquivo 
		6 - Verifica se o envio foi terminado (DONE recebido)

*/
int Protocolo::receiveMsg(int option){

	char bufferMsgs[256]="";
	int n = read(connFd,bufferMsgs,255);
	if (n < 0) {
		cout <<"ERRO: Leitura do Socket"<< "na option: " << option<<endl;
		return -1;
	}

	cout<<"Protocolo recebeu: "<<bufferMsgs<<endl;
	switch(option){
		case 1:
			//ACC se o receptor aceitara o arquivo ou REJ se o receptor rejeitou

				if(strcmp (bufferMsgs,"REJ") == 0){
					cout << "Protocolo: Envio rejeitado pelo receptor" << endl;
					return -1;
				}
				if(strcmp (bufferMsgs,"ACC") == 0){
					cout << "Protocolo: Envio aceito pelo receptor" << endl;
					return 0;
				}
				else{
					cout << "Protocolo: Resposta da option "<< option << " nao pode ser interpretada"<<endl;
					return 1;
				}	
								
			break;
		case 2:
			//NACK se o nome nao foi interpretado ou ACK se o nome foi interpretado 
				if(strcmp (bufferMsgs,"NACK") == 0){
					cout << "Protocolo: O nome nao foi interpretado" << endl;
					return -1;
				}
				if(strcmp (bufferMsgs,"ACK") == 0){
					cout << "Protocolo: O nome foi interpretado" << endl;
					return 0;
				}
				else{
					cout << "Protocolo: Resposta da option "<< option << " nao pode ser interpretada"<<endl;
					return 1;
				}	
							
			break;
		case 3:

				if(strcmp (bufferMsgs,"NACK") == 0){
					cout << "Protocolo: O arquivo foi NAO recebido com sucesso" << endl;
					return -1;
				}
				if(strcmp (bufferMsgs,"ACK") == 0){
					cout << "Protocolo: O arquivo foi recebido com sucesso" << endl;
					return 0;
				}
				else{
					cout << "Protocolo: Resposta da option "<< option << " nao pode ser interpretada"<<endl;
					return 1;
				}	
			
			break;


		case 4:
				if(strcmp (bufferMsgs,"RQST") == 0){
					cout << "Protocolo: Requisicao para recebimento de arquivo recebida" << endl;
					return 0;
				}
				else{
					cout << "Protocolo: Resposta da option "<< option << " nao pode ser interpretada"<<endl;
					return 1;
				}	
			
			break;

		case 5:
			fileName = string(bufferMsgs);	
			return 0;		
			break;
		case 6:
			fileSize = atoi((const char*)bufferMsgs);
			return 0;		
			break;
		case 7:
				if(strcmp (bufferMsgs,"DONE") == 0){
					cout << "Protocolo: O remetente concluiu o envio com sucesso" << endl;
					return 0;
				}
				if(strcmp (bufferMsgs,"FAIL") == 0){
					cout << "Protocolo: Falha apontada pelo remetente" << endl;
					return -1;
				}				
				else{
					cout << "Protocolo: Resposta da option "<< option << " nao pode ser interpretada"<<endl;
					return 1;
				}	
			
			break;



	}
}

int Protocolo::send(string fileName, string filePath){

	//Se nao conseguir se conectar, retorna imediatamente
	if(sender.connectTo()<0) return -1;


	//Envia uma requisicao para iniciar uma tranferencia
	sender.sendMsg("RQST");
	cout<<"Protocolo enviou: RQST "<<endl;


	//O baile soh segue quando o outro PC responder
	//IMPLEMENTAR UM TIMEUP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	while(connFd==0);
	if(connFd<0)return -1;

	//Recebe a resposta da requisicao
	if(receiveMsg(1)!=0)return -1;

	//Se a requisicao for aceita, o nome do arquivo sera enviado
	sender.sendMsg(fileName);
	cout<<"Protocolo enviou: "<<fileName<<endl;
	//Recebe a resposta ao envio do nome
	if(receiveMsg(2)!=0)return -1;
	
	//O sender envia o tamanho do arquivo e entao envia o arquivo		
	if(sender.sendFile(filePath) == 0){

		//Se o envio ocorreu bem, entao um DONE eh enviado
		//eh necessario deixar os sockets darem uma respirada antes de continuar
		//Limitacao computacional ou de rede!
		usleep(250000);
		sender.sendMsg("DONE");
		cout<<"Protocolo enviou: DONE "<<endl;
	}
	else{
		//Se houve algum erro durante o envio entao um FAIL eh enviado
		sender.sendMsg("FAIL");
		cout<<"Protocolo enviou: FAIL "<<endl;
	}

	//Recebe a resposta final
	if(receiveMsg(3)!=0)return -1;	

	close(sockFd);
	return 0;
}

int Protocolo::receive(){
	while(connFd==0);
	if(connFd<0)return -1;

	//Se nao conseguir se conectar, retorna imediatamente
	if(sender.connectTo()<0) return -1;

	//Aqui existe a possibilidade de implementar uma opcao de rejeicao
	//do arquivo, baseado no IP ou outro parametro.
	//Aceitarei qualquer pedido a principio ja que so eu tenho acesso a este protocolo
	//nao resultando em nenhum perigo.

	if(receiveMsg(4)==0){
		sender.sendMsg("ACC");
		cout<<"Protocolo enviou: ACC "<<endl;
	}else return -1;

	//Se recebeu o nome do arquivo, entao responde com um ACK
	if(receiveMsg(5)==0){
		sender.sendMsg("ACK");
		cout<<"Protocolo enviou: ACK "<<endl;
	}else{
		sender.sendMsg("NACK");
		cout<<"Protocolo enviou: NACK "<<endl;
	 	return -1;
	}


	//Se recebeu o tamanho do arquivo, se prepara para receber
	if(receiveMsg(6)==0){
		if(receiver.receiveFile(fileSize, fileName)<0);	
	}	


	//Se recebeu o confirmacao de que o remetente terminou o envio...
	if(receiveMsg(7)==0){
		sender.sendMsg("ACK");
		cout<<"Protocolo enviou: ACK "<<endl;
	}else{
		sender.sendMsg("NACK");
		cout<<"Protocolo enviou: NACK "<<endl;
	 	return -1;
	}	

	close(connFd);
	return 0;
}

