#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <conio.h>
#include <locale.h>

#define PORTA 2021
#define T_MAX 1024

int main() {
    // Definindo o programa e o prompt em UTF-8;
    setlocale(LC_ALL, "Portuguese_Brasil");
    system("chcp 65001");
    system("cls");

    // Configurações da tela.
    system("title Cliente - Calculadora Socket");
    system("color F0");

    // Iniciliazação do Winsock.
    WSADATA winsock;
    if (WSAStartup(MAKEWORD(2, 2), &winsock) != 0)
    {
        printf("\nFalha na iniciliazação do winsock!\n");
        return (1);
    }

    // Criação do Socket.
    SOCKET socket_aplicacao = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_aplicacao == INVALID_SOCKET)
    {
        printf("\nFalha na criação do socket. Erro: %d\n", WSAGetLastError());
        WSACleanup();
        return (1);
    }

    // Configuração do servidor.
    struct sockaddr_in servidor;
    memset(&servidor, 0, sizeof(servidor));
    servidor.sin_addr.s_addr = inet_addr("127.0.0.1");
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(PORTA);

    // Conexão com o servidor.
    if (connect(socket_aplicacao, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
    {
        printf("\nFalha na conexão com o servidor. Erro: %d\n", WSAGetLastError());
        closesocket(socket_aplicacao);
        WSACleanup();
        return (1);
    }

    // Receber uma mensagem.
    char mensagem[T_MAX];
    if (recv(socket_aplicacao, mensagem, T_MAX, 0) <= 0)
    {
        printf("\nFalha ao receber a messagem do servidor. Erro: %d\n", WSAGetLastError());
        closesocket(socket_aplicacao);
        WSACleanup();
        return (1);
    }
    mensagem[strlen(mensagem)] = '\0';
    printf("Mensagem recebida: %s", mensagem);

    printf("\nPressione qualquer tecla para continuar!");
    getche();

    // Loop para o envio e recebimento de mensagens.
    char verificador;
    char opcao;
    do {
        do {
            // Limpando as variáveis.
            memset(mensagem, 0x0, T_MAX);
            verificador = 'n';
            opcao = '1';
            system("cls");

            // Exibir menu.
            printf("|--------------------|\n");
            printf("| Calculadora Socket |\n");
            printf("|--------------------|\n");
            printf("| 1 - Soma           |\n");
            printf("| 2 - Subtração      |\n");
            printf("| 3 - Divisão        |\n");
            printf("| 4 - Multiplicação  |\n");
            printf("|--------------------|\n");
            printf("| Informe uma opção: ");

            // Enviar opção.
            fflush(stdin);
            opcao = getche();
        } while (!(opcao == '1' || opcao == '2' || opcao == '3' || opcao == '4'));
        mensagem[0] = opcao;

        if (send(socket_aplicacao, mensagem, strlen(mensagem), 0) < 0)
        {
            printf("\n\nFalha ao enviar a mensagem. Erro: %d\n", WSAGetLastError());
            closesocket(socket_aplicacao);
            WSACleanup();
            return (1);
        }
        memset(mensagem, 0x0, T_MAX);

        // Enviar primeiro número.
        printf("\n\n| Informe o primeiro número: ");
        fflush(stdin);
        gets(mensagem);

        if (send(socket_aplicacao, mensagem, strlen(mensagem), 0) < 0)
        {
            printf("\nFalha ao enviar a mensagem. Erro: %d\n", WSAGetLastError());
            closesocket(socket_aplicacao);
            WSACleanup();
            return (1);
        }
        memset(mensagem, 0x0, T_MAX);

        // Enviar segundo número.
        printf("\n| Informe o segundo número: ");
        fflush(stdin);
        gets(mensagem);

        if (send(socket_aplicacao, mensagem, strlen(mensagem), 0) < 0)
        {
            printf("\nFalha ao enviar a mensagem. Erro: %d\n", WSAGetLastError());
            closesocket(socket_aplicacao);
            WSACleanup();
            return (1);
        }
        memset(mensagem, 0x0, T_MAX);

        // Aguardando mensagem com o resultado.
        if (recv(socket_aplicacao, mensagem, T_MAX, 0) <= 0)
        {
            printf("\nFalha ao receber a messagem do servidor. Erro: %d\n", WSAGetLastError());
            closesocket(socket_aplicacao);
            WSACleanup();
            return (1);
        }
        mensagem[strlen(mensagem)] = '\0';

        printf("\n| Resultado: %s\n", mensagem);
        memset(mensagem, 0x0, T_MAX);

        // Aguardando mensagem se deseja realizar uma nova operação.
        if (recv(socket_aplicacao, mensagem, T_MAX, 0) <= 0)
        {
            printf("\nFalha ao receber a messagem do servidor. Erro: %d\n", WSAGetLastError());
            closesocket(socket_aplicacao);
            WSACleanup();
            return (1);
        }
        mensagem[strlen(mensagem)] = '\0';

        printf("%s", mensagem);
        memset(mensagem, 0x0, T_MAX);

        // Enviar resposta da decisão.
        fflush(stdin);
        verificador = getche();

        mensagem[0] = verificador;
        mensagem[strlen(mensagem)] = '\0';
        if (send(socket_aplicacao, mensagem, strlen(mensagem), 0) < 0)
        {
            printf("\nFalha ao enviar a mensagem. Erro: %d\n", WSAGetLastError());
            closesocket(socket_aplicacao);
            WSACleanup();
            return (1);
        }
        memset(mensagem, 0x0, T_MAX);

        // Verificação para repetir.
    } while (verificador == 's' || verificador == 'S');

    // Fechando o socket.
    closesocket(socket_aplicacao);
    WSACleanup();
    return (0);
}
