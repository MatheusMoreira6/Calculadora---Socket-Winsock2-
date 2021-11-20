#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <conio.h>
#include <locale.h>

#define PORTA 2021
#define T_MAX 1024

//Função para retornar o resultado do calculo.
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

    //Iniciliazação do Winsock.
    WSADATA winsock;
    if(WSAStartup(MAKEWORD(2, 2), &winsock) != 0){
        printf("Falha na inicialização da biblioteca winsock!\n");
        return(1);
    }

    //Criação do Socket.
    SOCKET servidor_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(servidor_socket == INVALID_SOCKET){
        printf("Falha na criação do socket. Erro: %d\n", WSAGetLastError());
        WSACleanup();
        return(1);
    }

    //Configuração do servidor.
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

    //Configurando o servidor para "escutar" e definindo quantas conexões poderam ser feitas (Nesse caso apenas uma).
    if(listen(servidor_socket, 1) == SOCKET_ERROR){
        printf("Falha!\nO servidor não pode ficar escutando. Erro: %d\n", WSAGetLastError());
        closesocket(servidor_socket);
        WSACleanup();
        return(1);
    }

    //Criação do socket do cliente.
    SOCKET cliente_socket;
    memset(&cliente_socket, 0, sizeof(cliente_socket));

    //Criação da estrutura.
    struct sockaddr_in client;
    int tamanho_client = sizeof(client);

    //Aguardando o cliente para aceitar a conexão.
    printf("| Aguardando por uma aplicação cliente...\n");
    cliente_socket = accept(servidor_socket, (struct sockaddr *)&client, &tamanho_client);
    if(cliente_socket == INVALID_SOCKET){
        printf("\nA conexão com o cliente não pode ser aceita. Erro: %d\n", WSAGetLastError());
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

        //Variavel de controle e operação.
        verificador='n';
        int operando=0;
        float numero_1=0;
        float numero_2=0;
        float resultado=0;

        //Enviar menu.
        strcpy(mensagem, "|--------------------|\n| Calculadora Socket |\n|--------------------|\n| 1 - Soma           |\n| 2 - Subtração      |\n| 3 - Divisão        |\n| 4 - Multiplicação  |\n|--------------------|\n| Informe uma opção: ");
        if(send(cliente_socket, mensagem, strlen(mensagem), 0) < 0){
            printf("\nFalha no envio da mensagem. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return(1);
        }
        memset(mensagem, 0x0, T_MAX);

        //Aguardando o tipo de operação.
        if(recv(cliente_socket, mensagem, T_MAX, 0) <= 0){
            printf("\nErro ao receber a messagem do cliente. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return(1);
        }
        mensagem[strlen(mensagem)] = '\0';

        //Converter número para inteiro.
        operando=atof(mensagem);
        memset(mensagem, 0x0, T_MAX);

        printf("\n| Operando: %d\n", operando);

        //Enviar solicitação do primeiro número.
        strcpy(mensagem, "\n| Informe o primeiro número: ");
        if(send(cliente_socket, mensagem, strlen(mensagem), 0) < 0){
            printf("\nFalha no envio da mensagem. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return(1);
        }
        memset(mensagem, 0x0, T_MAX);

        //Aguardando o primeiro número.
        if(recv(cliente_socket, mensagem, T_MAX, 0) <= 0){
            printf("\nErro ao receber a messagem do cliente. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return(1);
        }
        mensagem[strlen(mensagem)] = '\0';

        //Converter primeiro número para float.
        numero_1=atof(mensagem);
        memset(mensagem, 0x0, T_MAX);

        printf("\n| Primeiro Número: %g\n", numero_1);

        //Enviar solicitação do segundo número.
        strcpy(mensagem, "\n| Informe o segundo número: ");
        if(send(cliente_socket, mensagem, strlen(mensagem), 0) < 0){
            printf("\nFalha no envio da mensagem. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return(1);
        }
        memset(mensagem, 0x0, T_MAX);

        //Aguardando o segundo número.
        if(recv(cliente_socket, mensagem, T_MAX, 0) <= 0){
            printf("\nErro ao receber a messagem do cliente. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return(1);
        }
        mensagem[strlen(mensagem)] = '\0';

        //Converter segundo número para float.
        numero_2=atof(mensagem);
        memset(mensagem, 0x0, T_MAX);

        printf("\n| Segundo Número: %g\n", numero_2);

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

        //Verificação de nova operação.
        strcpy(mensagem, "\n| Deseja realizar uma nova operação (S/N)?");
        if(send(cliente_socket, mensagem, strlen(mensagem), 0) < 0){
            printf("\nFalha no envio da mensagem. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return(1);
        }
        memset(mensagem, 0x0, T_MAX);

        //Aguardando resposta da decisão.
        if(recv(cliente_socket, mensagem, T_MAX, 0) <= 0){
            printf("\nErro ao receber a messagem do cliente. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return(1);
        }
        mensagem[strlen(mensagem)] = '\0';
        verificador=mensagem[0];

        //Verificação para repetir.
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
