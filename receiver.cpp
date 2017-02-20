/* The port number is passed as an argument */
//g++ *.cpp -lpthread -std=c++11 -std=c++0x -o protocolo_FTP_type


#include "receiver.h"


//Construtor da Classe C_Conexao
Receiver::Receiver(){
	
}

Receiver::Receiver(string IPlocal){
	meuIP=IPlocal;
}


//Destrutor da Classe C_Conexao 
Receiver::~Receiver(void){}

int Receiver::open_socket(){
	cout << "Receiver: Abrindo o socket. IP: " << meuIP << endl;

	// Variaveis Diversas
	int				h; // Verificador de erros
	socklen_t			size_addr; // tamanho do socket em bytes

	// cria socket TCP
	sockFd = (int) socket (AF_INET, SOCK_STREAM, 0); 
	if (sockFd <= 0) {
		perror("Receiver: Erro ao abrir o socket");
	}
		
	// preenche estrutura sockaddr_in
	size_addr = sizeof(addr); 
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(meuIP.c_str());// IP
       // convert integer value for port must be converted into network byte order
	addr.sin_port = htons(porta_TCP); 
	
	// bind na porta 2017 e no ip da interface local 
	h = bind (sockFd, (sockaddr*) &addr, size_addr);
	if (h == -1) {
		perror("Receiver: Erro ao efetuar o bind");
		return -1;
	}

	cout << "Receiver: Sucesso ao abrir socket. IP: " << meuIP << endl;
	

}

//Aceita uma conexao de outro PC
int Receiver::acceptConnection(){
	//Abre um socket
	if(open_socket()<1) return 0;

	listen(sockFd, 5);
	socklen_t clilen = sizeof(cli_addr);
	connFd = accept(sockFd, (struct sockaddr *)&cli_addr, &clilen);
	if (connFd < 0)
        {
            cerr << "Receiver: Falha ao receber um conexao (accept)" << endl;
            return 0;
        }
        else
        {
            cout << "Receiver: Uma nova conexao foi aceita (accept)" << endl;
        }
	
	//A partir de agora usará apenas o connFd
	close(sockFd);

	return connFd;
}


int Receiver::receiveFile(int fileSize, string fileName){

	char currentDir[FILENAME_MAX];
	if(!getcwd(currentDir, sizeof(currentDir)));


	string current_dir(currentDir);
	string filePath_str=current_dir+"/"+fileName;

	File = fopen(filePath_str.c_str(), "wb");
    	char *Buffer_file = new char[fileSize];

	int received = 0;
	while (fileSize > received)
	{
		int Amount = recv(connFd, Buffer_file + received, fileSize, 0);
		if (Amount <= 0)
		{
			cout <<"Receiver: Falha na recepcao do arquivo" << endl;
			return -1;
            		
        	}
        	else
        	{
            		received += Amount;
           		cout << "Recebido: "<< received <<"/"<<fileSize << endl;
        	}
    	}         


    	fwrite(Buffer_file, 1, fileSize, File);
	fflush(File);
    	fclose(File);
        delete[] Buffer_file;


	return 0;
}







