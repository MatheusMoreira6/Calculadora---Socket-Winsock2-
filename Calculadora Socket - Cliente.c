#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <conio.h>
#include <locale.h>

#define PORTA 2021
#define T_MAX 1024

int main(){
    setlocale(LC_ALL, "Portuguese");
    system("title Cliente - Calculadora Socket");
    system("color F0");

    //Iniciliaza��o do Winsock.
    WSADATA winsock;
    if(WSAStartup(MAKEWORD(2, 2), &winsock) != 0) {
        printf("\nFalha na inicializa��oo da biblioteca winsock!\n");
        return(1);
    }

    //Cria��o do Socket.
    SOCKET socket_aplicacao = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_aplicacao == INVALID_SOCKET) {
        printf("\nFalha na cria��o do socket. Erro: %d\n", WSAGetLastError());
        WSACleanup();
        return(1);
    }

    //Configura��o do servidor.
    struct sockaddr_in servidor;
    memset(&servidor, 0, sizeof(servidor));
    servidor.sin_addr.s_addr = inet_addr("127.0.0.1");
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(PORTA);

    //Conex�o com o servidor.
    if(connect(socket_aplicacao, (struct sockaddr *)&servidor, sizeof(servidor)) < 0){
        printf("\nFalha na conex�o com o servidor. Erro: %d\n", WSAGetLastError());
        closesocket(socket_aplicacao);
        WSACleanup();
        return(1);
    }

    //Receber uma mensagem.
    char mensagem[T_MAX];
    if(recv(socket_aplicacao, mensagem, T_MAX, 0) <= 0){
        printf("\nFalha ao receber a messagem do servidor. Erro: %d\n", WSAGetLastError());
        closesocket(socket_aplicacao);
        WSACleanup();
        return(1);
    }
    mensagem[strlen(mensagem)] = '\0';
    printf("Mensagem recebida: %s", mensagem);

    printf("\nPressione qualquer tecla para continuar!");
    getche();

    //Loop para o envio e recebimento de mensagens.
    char verificador;
    do {
        //Limpando a estrutura.
        memset(mensagem, 0x0, T_MAX);
        system("cls");

        //Variavel de controle.
        verificador='n';

        //Aguardando por menu.
        if(recv(socket_aplicacao, mensagem, T_MAX, 0) <= 0){
            printf("\nFalha ao receber a messagem do servidor. Erro: %d\n", WSAGetLastError());
            closesocket(socket_aplicacao);
            WSACleanup();
            return(1);
        }
        mensagem[strlen(mensagem)] = '\0';

        //Exibir menu.
        printf("%s", mensagem);
        memset(mensagem, 0x0, T_MAX);

        //Pegar op��o do menu.
        fflush(stdin);
        gets(mensagem);

        //Enviar op��o.
        if(send(socket_aplicacao, mensagem, strlen(mensagem), 0) < 0){
            printf("\nFalha ao enviar a mensagem. Erro: %d\n", WSAGetLastError());
            closesocket(socket_aplicacao);
            WSACleanup();
            return(1);
        }
        memset(mensagem, 0x0, T_MAX);

        //Aguardando mensagem solicitando o primeiro n�mero.
        if(recv(socket_aplicacao, mensagem, T_MAX, 0) <= 0){
            printf("\nFalha ao receber a messagem do servidor. Erro: %d\n", WSAGetLastError());
            closesocket(socket_aplicacao);
            WSACleanup();
            return(1);
        }
        mensagem[strlen(mensagem)] = '\0';

        //Exibir mensagem.
        printf("%s", mensagem);
        memset(mensagem, 0x0, T_MAX);

        //Pegar primeiro n�mero.
        fflush(stdin);
        gets(mensagem);

        //Enviar primeiro n�mero.
        if(send(socket_aplicacao, mensagem, strlen(mensagem), 0) < 0){
            printf("\nFalha ao enviar a mensagem. Erro: %d\n", WSAGetLastError());
            closesocket(socket_aplicacao);
            WSACleanup();
            return(1);
        }
        memset(mensagem, 0x0, T_MAX);

        //Aguardando mensagem solicitando o segundo n�mero.
        if(recv(socket_aplicacao, mensagem, T_MAX, 0) <= 0){
            printf("\nFalha ao receber a messagem do servidor. Erro: %d\n", WSAGetLastError());
            closesocket(socket_aplicacao);
            WSACleanup();
            return(1);
        }
        mensagem[strlen(mensagem)] = '\0';

        //Exibir mensagem.
        printf("%s", mensagem);
        memset(mensagem, 0x0, T_MAX);

        //Pegar segundo n�mero.
        fflush(stdin);
        gets(mensagem);

        //Enviar segundo n�mero.
        if(send(socket_aplicacao, mensagem, strlen(mensagem), 0) < 0){
            printf("\nFalha ao enviar a mensagem. Erro: %d\n", WSAGetLastError());
            closesocket(socket_aplicacao);
            WSACleanup();
            return(1);
        }
        memset(mensagem, 0x0, T_MAX);

        //Aguardando mensagem com o resultado.
        if(recv(socket_aplicacao, mensagem, T_MAX, 0) <= 0){
            printf("\nFalha ao receber a messagem do servidor. Erro: %d\n", WSAGetLastError());
            closesocket(socket_aplicacao);
            WSACleanup();
            return(1);
        }
        mensagem[strlen(mensagem)] = '\0';

        //Exibir mensagem.
        printf("%s", mensagem);
        memset(mensagem, 0x0, T_MAX);

        //Aguardando mensagem se deseja realizar uma nova opera��o.
        if(recv(socket_aplicacao, mensagem, T_MAX, 0) <= 0){
            printf("\nFalha ao receber a messagem do servidor. Erro: %d\n", WSAGetLastError());
            closesocket(socket_aplicacao);
            WSACleanup();
            return(1);
        }
        mensagem[strlen(mensagem)] = '\0';

        //Exibir mensagem.
        printf("%s", mensagem);
        memset(mensagem, 0x0, T_MAX);

        //Pegar resposta da decis�o.
        fflush(stdin);
        verificador=getche();

        //Enviar resposta da decis�o.
        mensagem[0]=verificador;
        mensagem[strlen(mensagem)]='\0';
        if(send(socket_aplicacao, mensagem, strlen(mensagem), 0) < 0){
            printf("\nFalha ao enviar a mensagem. Erro: %d\n", WSAGetLastError());
            closesocket(socket_aplicacao);
            WSACleanup();
            return(1);
        }
        memset(mensagem, 0x0, T_MAX);

        //Verifica��o para repetir.
    }while(verificador=='s' || verificador=='S');

    //Fechando o socket.
    closesocket(socket_aplicacao);
    WSACleanup();
    return(0);
}
