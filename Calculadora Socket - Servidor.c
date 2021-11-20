#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <conio.h>
#include <locale.h>

#define PORTA 2021
#define T_MAX 1024

//Fun��o para retornar o resultado do calculo.
float calcular(int operando, float numero1, float numero2){
    switch(operando){
        case 1:{
            return(numero1+numero2);
            break;
        }
        case 2:{
            return(numero1-numero2);
            break;
        }
        case 3:{
            return(numero1/numero2);
            break;
        }
        case 4:{
            return(numero1*numero2);
            break;
        }
    }
}

int main(){
    setlocale(LC_ALL, "Portuguese");
    system("title Servidor - Calculadora Socket");
    system("color F0");

    //Iniciliaza��o do Winsock.
    WSADATA winsock;
    if(WSAStartup(MAKEWORD(2, 2), &winsock) != 0){
        printf("Falha na inicializa��o da biblioteca winsock!\n");
        return(1);
    }

    //Cria��o do Socket.
    SOCKET servidor_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(servidor_socket == INVALID_SOCKET){
        printf("Falha na cria��o do socket. Erro: %d\n", WSAGetLastError());
        WSACleanup();
        return(1);
    }

    //Configura��o do servidor.
    struct sockaddr_in servidor;
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(PORTA);

    //Associar o servidor a uma porta local.
    if(bind(servidor_socket, (struct sockaddr *)&servidor, sizeof(servidor)) < 0){
        printf("Falha ao abrir a porta do servidor. Erro: %d\n", WSAGetLastError());
        closesocket(servidor_socket);
        WSACleanup();
        return(1);
    }

    //Configurando o servidor para "escutar" e definindo quantas conex�es poderam ser feitas (Nesse caso apenas uma).
    if(listen(servidor_socket, 1) == SOCKET_ERROR){
        printf("Falha!\nO servidor n�o pode ficar escutando. Erro: %d\n", WSAGetLastError());
        closesocket(servidor_socket);
        WSACleanup();
        return(1);
    }

    //Cria��o do socket do cliente.
    SOCKET cliente_socket;
    memset(&cliente_socket, 0, sizeof(cliente_socket));

    //Cria��o da estrutura.
    struct sockaddr_in client;
    int tamanho_client = sizeof(client);

    //Aguardando o cliente para aceitar a conex�o.
    printf("| Aguardando por uma aplica��o cliente...\n");
    cliente_socket = accept(servidor_socket, (struct sockaddr *)&client, &tamanho_client);
    if(cliente_socket == INVALID_SOCKET){
        printf("\nA conex�o com o cliente n�o pode ser aceita. Erro: %d\n", WSAGetLastError());
        closesocket(servidor_socket);
        WSACleanup();
        return(1);
    }
    printf("\n| Uma aplicacao cliente realizou uma conexao!\n\n");

    //Envio de uma mensagem ao cliente.
    char mensagem[T_MAX] = "Conexao foi estabelecida com sucesso!\n";
    if (send(cliente_socket, mensagem, strlen(mensagem), 0) < 0) {
        printf("\nFalha no envio da mensagem. Erro: %d\n", WSAGetLastError());
        closesocket(servidor_socket);
        closesocket(cliente_socket);
        WSACleanup();
        return(1);
    }

    //Loop para o envio e recebimento de mensagens.
    char verificador;
    do {
        printf("| Servidor - Calculadora Socket\n");
        //Limpando a estrutura.
        memset(mensagem, 0x0, T_MAX);

        //Variavel de controle e opera��o.
        verificador='n';
        int operando=0;
        float numero_1=0;
        float numero_2=0;
        float resultado=0;

        //Enviar menu.
        strcpy(mensagem, "|--------------------|\n| Calculadora Socket |\n|--------------------|\n| 1 - Soma           |\n| 2 - Subtra��o      |\n| 3 - Divis�o        |\n| 4 - Multiplica��o  |\n|--------------------|\n| Informe uma op��o: ");
        if(send(cliente_socket, mensagem, strlen(mensagem), 0) < 0){
            printf("\nFalha no envio da mensagem. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return(1);
        }
        memset(mensagem, 0x0, T_MAX);

        //Aguardando o tipo de opera��o.
        if(recv(cliente_socket, mensagem, T_MAX, 0) <= 0){
            printf("\nErro ao receber a messagem do cliente. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return(1);
        }
        mensagem[strlen(mensagem)] = '\0';

        //Converter n�mero para inteiro.
        operando=atof(mensagem);
        memset(mensagem, 0x0, T_MAX);

        printf("\n| Operando: %d\n", operando);

        //Enviar solicita��o do primeiro n�mero.
        strcpy(mensagem, "\n| Informe o primeiro n�mero: ");
        if(send(cliente_socket, mensagem, strlen(mensagem), 0) < 0){
            printf("\nFalha no envio da mensagem. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return(1);
        }
        memset(mensagem, 0x0, T_MAX);

        //Aguardando o primeiro n�mero.
        if(recv(cliente_socket, mensagem, T_MAX, 0) <= 0){
            printf("\nErro ao receber a messagem do cliente. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return(1);
        }
        mensagem[strlen(mensagem)] = '\0';

        //Converter primeiro n�mero para float.
        numero_1=atof(mensagem);
        memset(mensagem, 0x0, T_MAX);

        printf("\n| Primeiro N�mero: %g\n", numero_1);

        //Enviar solicita��o do segundo n�mero.
        strcpy(mensagem, "\n| Informe o segundo n�mero: ");
        if(send(cliente_socket, mensagem, strlen(mensagem), 0) < 0){
            printf("\nFalha no envio da mensagem. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return(1);
        }
        memset(mensagem, 0x0, T_MAX);

        //Aguardando o segundo n�mero.
        if(recv(cliente_socket, mensagem, T_MAX, 0) <= 0){
            printf("\nErro ao receber a messagem do cliente. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return(1);
        }
        mensagem[strlen(mensagem)] = '\0';

        //Converter segundo n�mero para float.
        numero_2=atof(mensagem);
        memset(mensagem, 0x0, T_MAX);

        printf("\n| Segundo N�mero: %g\n", numero_2);

        //Calculando.
        resultado=calcular(operando, numero_1, numero_2);

        printf("\n| Resultado: %g\n", resultado);

        //Enviando resultado.
        sprintf(mensagem, "\n| Resultado: %g\n", resultado);
        mensagem[strlen(mensagem)] = '\0';
        if(send(cliente_socket, mensagem, strlen(mensagem), 0) < 0){
            printf("\nFalha no envio da mensagem. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return(1);
        }
        memset(mensagem, 0x0, T_MAX);

        //Verifica��o de nova opera��o.
        strcpy(mensagem, "\n| Deseja realizar uma nova opera��o (S/N)?");
        if(send(cliente_socket, mensagem, strlen(mensagem), 0) < 0){
            printf("\nFalha no envio da mensagem. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return(1);
        }
        memset(mensagem, 0x0, T_MAX);

        //Aguardando resposta da decis�o.
        if(recv(cliente_socket, mensagem, T_MAX, 0) <= 0){
            printf("\nErro ao receber a messagem do cliente. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return(1);
        }
        mensagem[strlen(mensagem)] = '\0';
        verificador=mensagem[0];

        //Verifica��o para repetir.
        if(verificador=='s' || verificador=='S'){
            memset(mensagem, 0x0, T_MAX);
            system("cls");
        }
    }while(verificador=='s' || verificador=='S');

    //Fechando os sockets.
    closesocket(servidor_socket);
    closesocket(cliente_socket);
    WSACleanup();
    return(0);
}
