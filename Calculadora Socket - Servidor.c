#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <conio.h>
#include <locale.h>

#define PORTA 2021
#define T_MAX 1024

// Função para retornar o resultado do calculo.
float calcular(int operando, float numero1, float numero2) {
    switch (operando)
    {
        case 1:
        {
            return (numero1 + numero2);
            break;
        }
        case 2:
        {
            return (numero1 - numero2);
            break;
        }
        case 3:
        {
            return (numero1 / numero2);
            break;
        }
        case 4:
        {
            return (numero1 * numero2);
            break;
        }
    }
}

int main() {
    // Definindo o programa e o prompt em UTF-8;
    setlocale(LC_ALL, "Portuguese_Brasil");
    system("chcp 65001");
    system("cls");

    // Configurações da tela.
    system("title Servidor - Calculadora Socket");
    system("color F0");

    // Iniciliazação do Winsock.
    WSADATA winsock;
    if (WSAStartup(MAKEWORD(2, 2), &winsock) != 0)
    {
        printf("Falha na inicialização do winsock!\n");
        return (1);
    }

    // Criação do Socket.
    SOCKET servidor_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (servidor_socket == INVALID_SOCKET)
    {
        printf("Falha na criação do socket. Erro: %d\n", WSAGetLastError());
        WSACleanup();
        return (1);
    }

    // Configuração do servidor.
    struct sockaddr_in servidor;
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(PORTA);

    // Associar o servidor a uma porta local.
    if (bind(servidor_socket, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
    {
        printf("Falha ao abrir a porta do servidor. Erro: %d\n", WSAGetLastError());
        closesocket(servidor_socket);
        WSACleanup();
        return (1);
    }

    // Configurando o servidor para "escutar" e definindo quantas conexões poderam ser feitas (Nesse caso apenas uma).
    if (listen(servidor_socket, 1) == SOCKET_ERROR)
    {
        printf("Falha!\nO servidor não pode ficar 'escutando'. Erro: %d\n", WSAGetLastError());
        closesocket(servidor_socket);
        WSACleanup();
        return (1);
    }

    // Criação do socket do cliente.
    SOCKET cliente_socket;
    memset(&cliente_socket, 0, sizeof(cliente_socket));

    // Criação da estrutura.
    struct sockaddr_in client;
    int tamanho_client = sizeof(client);

    // Aguardando o cliente para aceitar a conexão.
    printf("| Aguardando por uma aplicação cliente...\n");
    cliente_socket = accept(servidor_socket, (struct sockaddr *)&client, &tamanho_client);
    if (cliente_socket == INVALID_SOCKET)
    {
        printf("\nA conexão com o cliente não pode ser aceita. Erro: %d\n", WSAGetLastError());
        closesocket(servidor_socket);
        WSACleanup();
        return (1);
    }
    printf("\n| Uma aplicacao cliente realizou uma conexao!\n\n");

    // Envio de uma mensagem ao cliente.
    char mensagem[T_MAX] = "Conexao foi estabelecida com sucesso!\n";
    if (send(cliente_socket, mensagem, strlen(mensagem), 0) < 0)
    {
        printf("\nFalha no envio da mensagem. Erro: %d\n", WSAGetLastError());
        closesocket(servidor_socket);
        closesocket(cliente_socket);
        WSACleanup();
        return (1);
    }

    // Loop para o envio e recebimento de mensagens.
    char verificador;
    do {
        // Limpando as variáveis.
        memset(mensagem, 0x0, T_MAX);
        verificador = 'n';
        int operando = 0;
        float numero_1 = 0;
        float numero_2 = 0;
        float resultado = 0;
        system("cls");

        printf("| Servidor - Calculadora Socket\n");

        // Aguardando o tipo de operação.
        if (recv(cliente_socket, mensagem, T_MAX, 0) <= 0)
        {
            printf("\nErro ao receber a messagem do cliente. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return (1);
        }
        mensagem[strlen(mensagem)] = '\0';

        operando = atoi(mensagem);
        memset(mensagem, 0x0, T_MAX);
        printf("\n| Operando: %d\n", operando);

        // Aguardando o primeiro número.
        if (recv(cliente_socket, mensagem, T_MAX, 0) <= 0)
        {
            printf("\nErro ao receber a messagem do cliente. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return (1);
        }
        mensagem[strlen(mensagem)] = '\0';

        numero_1 = atof(mensagem);
        memset(mensagem, 0x0, T_MAX);
        printf("\n| Primeiro Número: %g\n", numero_1);

        // Aguardando o segundo número.
        if (recv(cliente_socket, mensagem, T_MAX, 0) <= 0)
        {
            printf("\nErro ao receber a messagem do cliente. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return (1);
        }
        mensagem[strlen(mensagem)] = '\0';

        numero_2 = atof(mensagem);
        memset(mensagem, 0x0, T_MAX);
        printf("\n| Segundo Número: %g\n", numero_2);

        // Calculando.
        resultado = calcular(operando, numero_1, numero_2);
        printf("\n| Resultado: %g\n", resultado);

        // Enviando resultado.
        sprintf(mensagem, "%g", resultado);
        mensagem[strlen(mensagem)] = '\0';
        if (send(cliente_socket, mensagem, strlen(mensagem), 0) < 0)
        {
            printf("\nFalha no envio da mensagem. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return (1);
        }
        memset(mensagem, 0x0, T_MAX);

        // Verificação de nova operação.
        strcpy(mensagem, "\n| Deseja realizar uma nova operação (S/N)?");
        if (send(cliente_socket, mensagem, strlen(mensagem), 0) < 0)
        {
            printf("\nFalha no envio da mensagem. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return (1);
        }
        memset(mensagem, 0x0, T_MAX);

        // Aguardando resposta da decisão.
        if (recv(cliente_socket, mensagem, T_MAX, 0) <= 0)
        {
            printf("\nErro ao receber a messagem do cliente. Erro: %d\n", WSAGetLastError());
            closesocket(servidor_socket);
            closesocket(cliente_socket);
            WSACleanup();
            return (1);
        }
        mensagem[strlen(mensagem)] = '\0';
        verificador = mensagem[0];

        // Verificação para repetir.
    } while (verificador == 's' || verificador == 'S');

    // Fechando os sockets.
    closesocket(servidor_socket);
    closesocket(cliente_socket);
    WSACleanup();
    return (0);
}
