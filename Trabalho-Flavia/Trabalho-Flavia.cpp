#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

typedef struct
{
    int id;
    char nome[40];
    char quarto[10];
    char data[12];
    char status;
} reg;

int tamanho(FILE* arq)
{
    fseek(arq, 0, SEEK_END); // Move o ponteiro do arquivo para o final
    return ftell(arq) / sizeof(reg);
}

void cadastrar(FILE* arq)
{
    reg reserva;
    fseek(arq, 0, SEEK_END); // Move o ponteiro do arquivo para o final
    int id = tamanho(arq) + 1; // Define o ID da reserva com base na posição
    reserva.id = id;
    printf("\nCadastro de Reserva\n");
    printf("Nome do hóspede: ");
    std::cin.ignore(); // Limpa o buffer de entrada
    fgets(reserva.nome, sizeof(reserva.nome), stdin);
    reserva.nome[strcspn(reserva.nome, "\n")] = 0;
    printf("Número do quarto: ");
    fgets(reserva.quarto, sizeof(reserva.quarto), stdin);
    reserva.quarto[strcspn(reserva.quarto, "\n")] = 0;
    printf("Data da reserva (dd/mm/yyyy): ");
    fgets(reserva.data, sizeof(reserva.data), stdin);
    reserva.data[strcspn(reserva.data, "\n")] = 0;
    reserva.status = ' '; // Define o status da reserva como ativa
    fwrite(&reserva, sizeof(reg), 1, arq); // Escreve a reserva no arquivo
    printf("\nReserva cadastrada com sucesso! Código da reserva: %d\n", reserva.id);
    system("pause");
}

void consultar(FILE* arq)
{
    reg reserva;
    int nr;
    printf("\nConsulta de Reserva pelo Código\n");
    printf("\nInforme o Código...: ");
    scanf_s("%d", &nr);
    if ((nr <= tamanho(arq)) && (nr > 0)) // Verifica se o código é válido
    {
        fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET); // Move o ponteiro do arquivo para o registro correspondente
        fread(&reserva, sizeof(reg), 1, arq); // Lê o registro do arquivo
        if (reserva.status == ' ')
        {
            printf("\nCódigo da reserva..: %d", reserva.id);
            printf("\nNome do hóspede....: %s", reserva.nome);
            printf("\nNúmero do quarto...: %s", reserva.quarto);
            printf("\nData da reserva....: %s\n\n", reserva.data);
        }
        else
            printf("\nReserva cancelada! \n");
    }
    else
    {
        printf("\nNúmero de registro inválido!\n");
    }
    system("pause");
}

void geraarqtxt(FILE* arq)
{
    FILE* arqtxt;
    reg reserva;
    int i, tam;
    if (fopen_s(&arqtxt, "reservas.txt", "w") != 0) // Abre o arquivo de texto para escrita
    {
        printf("Falha ao criar o arquivo de texto!\n");
        system("pause");
        return;
    }
    tam = tamanho(arq); // Obtém o número de registros no arquivo
    fseek(arq, 0, SEEK_SET);
    for (i = 0; i < tam; i++)
    {
        fread(&reserva, sizeof(reg), 1, arq); // Lê cada registro do arquivo
        if (reserva.status == ' ')
        {
            fprintf(arqtxt, "Código da reserva: %d\n", reserva.id);
            fprintf(arqtxt, "Nome do hóspede: %s\n", reserva.nome);
            fprintf(arqtxt, "Número do quarto: %s\n", reserva.quarto);
            fprintf(arqtxt, "Data da reserva: %s\n\n", reserva.data);
        }
    }
    fclose(arqtxt);
    printf("\nArquivo de texto gerado com sucesso!\n");
    system("pause");
}

void excluir(FILE* arq)
{
    reg reserva;
    int nr;
    printf("\nCancelar Reserva\n");
    printf("\nInforme o Código...: ");
    scanf_s("%d", &nr);
    if ((nr <= tamanho(arq)) && (nr > 0)) // Verifica se o código é válido
    {
        fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
        fread(&reserva, sizeof(reg), 1, arq);
        if (reserva.status == ' ')
        {
            reserva.status = '*'; // Marca a reserva como cancelada
            fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
            fwrite(&reserva, sizeof(reg), 1, arq); // Escreve o registro atualizado no arquivo
            printf("\nReserva cancelada com sucesso!\n");
        }
        else
            printf("\nReserva já cancelada!\n");
    }
    else
    {
        printf("\nNúmero de registro inválido!\n");
    }
    system("pause");
}

int main()
{
    system("chcp 65001"); // permitir utf8

    int op;
    FILE* arq;

    // Abre o arquivo binário para leitura e escrita, ou cria um novo se não existir
    if (fopen_s(&arq, "reservas.dat", "rb+") != 0)
    {
        if (fopen_s(&arq, "reservas.dat", "wb+") != 0)
        {
            printf("Falha ao abrir o arquivo!\n");
            system("pause");
            return 1;
        }
    }

    do
    {
        system("CLS");
        printf("                                   /\\\n");
        printf("                              /\\  //\\\\\n");
        printf("                       /\\    //\\\\///\\\\\\        /\\\n");
        printf("                      //\\\\  ///\\////\\\\\\\\  /\\  //\\\\\n");
        printf("         /\\          /  ^ \\/^ ^/^  ^  ^ \\/^ \\/  ^ \\\n");
        printf("        / ^\\    /\\  / ^   /  ^/ ^ ^ ^   ^\\ ^/  ^^  \\\n");
        printf("       /^   \\  / ^\\/ ^ ^   ^ / ^  ^    ^  \\/ ^   ^  \\       *\n");
        printf("      /  ^ ^ \\/^  ^\\ ^ ^ ^   ^  ^   ^   ____  ^   ^  \\     /|\\\n");
        printf("     / ^ ^  ^ \\ ^  _\\___________________|  |_____^ ^  \\   /||o\\\n");
        printf("    / ^^  ^ ^ ^\\  /______________________________\\ ^ ^ \\ /|o|||\\\n");
        printf("   /  ^  ^^ ^ ^  /________________________________\\  ^  /|||||o|\\\n");
        printf("  /^ ^  ^ ^^  ^    ||___|___||||||||||||___|__|||      /||o||||||\\\n");
        printf(" / ^   ^   ^    ^  ||___|___||||||||||||___|__|||          | |\n");
        printf("/ ^ ^ ^  ^  ^  ^   ||||||||||||||||||||||||||||||oooooooooo| |ooooooo\n");
        printf("\n======= RESERVAS DE HOTEL ======= \n");
        printf("1. Cadastrar reserva\n");
        printf("2. Consultar reserva por código\n");
        printf("3. Gerar arquivo de reservas\n");
        printf("4. Cancelar reserva\n");
        printf("5. Sair\n");
        printf("===========Reservas: %d========\n", tamanho(arq));
        printf("Opção: ");
        scanf_s("%d", &op);
        switch (op)
        {
        case 1:
            cadastrar(arq);
            break;
        case 2:
            consultar(arq);
            break;
        case 3:
            geraarqtxt(arq);
            break;
        case 4:
            excluir(arq);
            break;
        case 5:
            fclose(arq);
            break;
        default:
            printf("Opção inválida!\n");
            system("pause");
        }
    } while (op != 5);

    return 0;
}
