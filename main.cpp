/*
	PTC-2017-1 Projeto de protocolos - Professor Marcelo Maia Sobral
	Engenharia de telecomunicações - 8ª fase, IFSC - Instituto Federal de Santa Catarina

	Aluno: Andre Felippe Weber

	*********************** TAREFA 1 - Um protocolo imaginário ******************
	********************* Protocolo de transferência de arquivos ****************

	PARA COMPILAR O PROGRAMA AS SEGUINTES FLAGS DEVEM SER USADAS:

	g++ *.cpp -lpthread -std=c++11 -std=c++0x -o protocolo_FTP_type
*/

#include "protocolo.h"

int main(int argc, char *argv[])
{
	//Protocolo(string IPlocal, string IPdestino)
	Protocolo protocolo("192.168.88.108", "192.168.88.106");

	//Lado do sender
	protocolo.send("CP.jpg", "./socker.jpg");

	//Lado do receiver
	//protocolo.receive();
	return 0;
}

