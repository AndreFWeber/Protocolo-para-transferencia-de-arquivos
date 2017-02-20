/* The port number is passed as an argument */
//g++ *.cpp -lpthread -std=c++11 -std=c++0x -o protocolo_FTP_type

#include "sender.h"

//Construtor da Classe C_Conexao
Sender::Sender(string receiverIP){
	IP = receiverIP;
}

Sender::Sender(){
}

//Destrutor da Classe C_Conexao 
Sender::~Sender(void){
}

int Sender::open_socket(){
	cout << "Sender: Abrindo o socket. IP: " << IP << endl;

	// Variaveis Diversas
	int				h; // Verificador de erros
	socklen_t			size_addr; // tamanho do socket em bytes

	// cria socket TCP
	sockFd = (int) socket (AF_INET, SOCK_STREAM, 0); 
	if (sockFd <= 0) {
		cout << "Sender: Erro ao abrir o socket" << endl;
		return -1;	
	}
		
	// preenche estrutura sockaddr_in
	size_addr = sizeof(addr); 
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(IP.c_str());// IP
       // convert integer value for port must be converted into network byte order
	addr.sin_port = htons(porta_TCP); 

	cout << "Sender: Sucesso ao abrir socket. IP: " << IP << endl;
	return 0;	

}

int  Sender::connectTo(){

	if(open_socket()<0)return -1;

	checker = connect(sockFd,(struct sockaddr *) &addr, sizeof(addr));
    
   	if (checker < 0)
        {
        	cout << "Sender: Falha ao tentar se conectar com o receiver" << endl;
        	return 1;
        }

	cout << "Sender: Sucesso ao se conectar com o receiver" << endl;
	return 0;	

}


int Sender::sendFile(string filePath){
	char *Buffer_file;
        unsigned long Size;

	//...In order to open a file as a binary file, a "b" character has to be included in the mode string.
	//rb = read as a binary file
        File = fopen(filePath.c_str(), "rb");
        if(!File)
        {
		cout << "Sender: FAIL on opening the file" << endl;
        	return -1;
        }

	//Sets the position indicator associated with the stream to a new position.
	//For streams open in binary mode, the new position is defined by adding offset to a reference position specified by origin.
	//SEEK_END = End of file *
	//http://www.cplusplus.com/reference/cstdio/fseek/
	fseek(File, 0, SEEK_END);

	//Returns the current value of the position indicator of the stream.
	//For binary streams, this is the number of bytes from the beginning of the file.
	//http://www.cplusplus.com/reference/cstdio/ftell/
	Size = ftell(File);

	//O pulo do gato! 
	//Eh necessario "rebobinhar" o arquivo pois o ponteiro esta apontado la pro final! (linha 167)
	//rewind (File); eh uma alternativa ao fseek
	//SEEK_SET = Beginning of file *
	fseek(File, 0, SEEK_SET);

	Buffer_file = (char*)malloc(sizeof(char)*Size);
	
	//http://www.cplusplus.com/reference/cstdio/fread/
	fread(Buffer_file, Size, 1, File);//inteiro

	//Fazendo um malabarismo pra poder enviar o Size!
	string Size_str = std::to_string(Size);
	char const* send_size = Size_str.c_str();
	send(sockFd, send_size, strlen(send_size), 0);

	//Preciso dar um tempo pro receptor respirar??
	//Na maquina local falhas ocorrem sem o sleep!
	usleep(10000);

	// send buffer to client
	int sent = 0;
	while (sent < Size)
	{

	    int n = send(sockFd, Buffer_file + sent, Size - sent, 0);
	    if (n == -1){
		cout <<"Sender: Envio do arquivo falhou"<<endl;
	    	return -1;
	    }
	    sent += n;
	    cout <<"Sender: "<< sent << "/" << Size << " bits enviados"<<endl;
	    usleep(10000);

	}

	cout <<"Sender: Enviou todos os " << Size << " bits do arquivo"<<endl;

	free(Buffer_file);

	close(checker);
	return 0;
}
	
void Sender::sendMsg(string msg){
	char const* msgToSend = msg.c_str();
	send(sockFd, msgToSend, strlen(msgToSend), 0);
}






