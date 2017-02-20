# Protocolo-para-transferencia-de-arquivos
Projeto de um protocolo de transferência de arquivos desenvolvido durante a primeira semana da matéria de Projeto de Protocolos 

Palavras-chave: Protocolo; Transferência de arquivos; C++, sockets; 
**********************************************************************************************************************************

PTC-2017-1 Projeto de protocolos - Professor Marcelo Maia Sobral
Engenharia de telecomunicações - 8ª fase, IFSC 

Aluno: Andre Felippe Weber
Data: 19 de fevereiro de 2017.

TAREFA 1 - Um protocolo imaginário
Protocolo de transferência de arquivos

Serviço: Realiza transferência de arquivos entre dois computadores utilizando o protocolo de transporte TCP.

Ambiente de execução do protocolo: Os dois computadores devem estar conectados à internet e devem estar preparados para receber e enviar dados na porta 2017. 

Vocabulário: Frames de dados e de controle.
Frames de controle:
ACK: Confirma que algo ocorreu como esperado;
NACK: Informa que algo não ocorreu como esperado;
RQST: Remetente envia uma requisição de envio de arquivo para o destinatário
REJ: Conexão para envio de arquivo foi rejeitada pelo destinatário;
ACC: Conexão para envio de arquivo foi aceita pelo destinatário;
DONE: Remetente envia DONE para o destinatário indicando que a transferência foi concluída;
FAIL: Remetente envia FAIL para o destinatário indicando que houve uma falha durante a transferência;

Codificação: Mensagens são textuais para os frames de controle e os dados são enviados bit à bit.

Comportamento: Um IP de destino e o local deve ser provido ao protocolo. Para enviar um arquivo, o nome do arquivo deve ser fornecido, assim como o caminho até ele. Se o destinatário não for alcançável então uma mensagem de erro será retornada. 
	O computador que vai receber o arquivo deve estar preparado para a transferência antes que o remetente inicie o envio.
	O remetente requisitará uma conexão para que a transferência de um arquivo seja feita através do envio de um RQST. O destinatário aceitará o arquivo respondendo com um ACC ou rejeitará através de um REJ. Em caso de rejeição o protocolo terminará a conexão, caso contrário o nome do arquivo é enviado ao destinatário. Que por sua vez responde ACK para confirmar que o nome foi recebido ou NACK se o nome não foi interpretado corretamente. 
	O destinatário envia então o tamanho do arquivo seguido pelo arquivo em si. Ao final desta transmissão uma mensagem DONE indicando o final da transmissão é enviado ao destinatário que responde com um ACK se a recepção foi um sucesso ou NACK caso contrário.

Figura 1 - Exemplo de transmissão de arquivo bem sucedida.

 
