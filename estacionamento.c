#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define CORREDORES 3
#define VAGAS_POR_CORREDOR 5

typedef struct {
    char placa[10];
    char modelo[30];
    char cor[15];
    time_t entrada;
} Veiculo;

typedef struct {
    int numero;
    int ocupada; // 0 = livre, 1 = ocupada
    Veiculo veiculo;
} Vaga;

Vaga estacionamento[CORREDORES][VAGAS_POR_CORREDOR];

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

char letraCorredor(int c) {
    return 'A' + c;
}

/* ===================== CADASTRO E CONTROLE DAS VAGAS ===================== */

void inicializarVagas() {
    int c, v;
    for (c = 0; c < CORREDORES; c++) {
        for (v = 0; v < VAGAS_POR_CORREDOR; v++) {
            estacionamento[c][v].numero = v + 1;
            estacionamento[c][v].ocupada = 0;
        }
    }
}

void mostrarMapa() {
    int c, v;
    printf("\n--- MAPA DO ESTACIONAMENTO ---\n");
    printf("[ ] livre   [X] ocupada\n\n");
    for (c = 0; c < CORREDORES; c++) {
        printf("Corredor %c: ", letraCorredor(c));
        for (v = 0; v < VAGAS_POR_CORREDOR; v++) {
            if (estacionamento[c][v].ocupada)
                printf("%c%d[X] ", letraCorredor(c), estacionamento[c][v].numero);
            else
                printf("%c%d[ ] ", letraCorredor(c), estacionamento[c][v].numero);
        }
        printf("\n");
    }
}

/* ===================== BUSCA E COMPARACAO DE VAGAS ===================== */

int contarLivresCorredor(int c) {
    int v, livres = 0;
    for (v = 0; v < VAGAS_POR_CORREDOR; v++) {
        if (!estacionamento[c][v].ocupada)
            livres++;
    }
    return livres;
}

// Escolhe o corredor com mais vagas livres e pega a primeira vaga livre dele.
// Retorna 1 se achou, 0 se o estacionamento esta cheio.
int buscarVagaLivre(int *corredor, int *vaga) {
    int c, v;
    int melhor = -1, maiorLivres = 0;

    printf("\nComparando corredores:\n");
    for (c = 0; c < CORREDORES; c++) {
        int livres = contarLivresCorredor(c);
        printf("Corredor %c -> %d vaga(s) livre(s)\n", letraCorredor(c), livres);
        if (livres > maiorLivres) {
            maiorLivres = livres;
            melhor = c;
        }
    }

    if (melhor == -1)
        return 0;

    for (v = 0; v < VAGAS_POR_CORREDOR; v++) {
        if (!estacionamento[melhor][v].ocupada) {
            *corredor = melhor;
            *vaga = v;
            return 1;
        }
    }
    return 0;
}

// Procura um veiculo pela placa. Retorna 1 se encontrou.
int buscarPlaca(char placa[], int *corredor, int *vaga) {
    int c, v;
    for (c = 0; c < CORREDORES; c++) {
        for (v = 0; v < VAGAS_POR_CORREDOR; v++) {
            if (estacionamento[c][v].ocupada &&
                strcmp(estacionamento[c][v].veiculo.placa, placa) == 0) {
                *corredor = c;
                *vaga = v;
                return 1;
            }
        }
    }
    return 0;
}

/* ===================== OCUPACAO E SAIDA DO VEICULO ===================== */

void ocuparVaga(int c, int v, Veiculo carro) {
    estacionamento[c][v].veiculo = carro;
    estacionamento[c][v].ocupada = 1;
}

void liberarVaga(int c, int v) {
    estacionamento[c][v].ocupada = 0;
    memset(&estacionamento[c][v].veiculo, 0, sizeof(Veiculo));
}

void registrarSaida() {
    char placa[10];
    int c, v, i;
    double minutos;

    printf("\nPlaca do veiculo: ");
    scanf("%9s", placa);
    limparBuffer();
    for (i = 0; placa[i] != '\0'; i++)
        placa[i] = toupper(placa[i]);

    if (!buscarPlaca(placa, &c, &v)) {
        printf("Veiculo nao encontrado no estacionamento.\n");
        return;
    }

    minutos = difftime(time(NULL), estacionamento[c][v].veiculo.entrada) / 60.0;

    printf("Veiculo %s saindo da vaga %c%d.\n", placa, letraCorredor(c), estacionamento[c][v].numero);
    printf("Tempo de permanencia: %.0f minuto(s)\n", minutos);

    liberarVaga(c, v);
    printf("Vaga liberada.\n");
}

/* ===================== ENTRADA DO VEICULO ===================== */

void cadastrarVeiculo(Veiculo *carro) {
    int i;

    memset(carro, 0, sizeof(Veiculo));

    printf("Placa: ");
    scanf("%9s", carro->placa);
    limparBuffer();
    for (i = 0; carro->placa[i] != '\0'; i++)
        carro->placa[i] = toupper(carro->placa[i]);

    printf("Modelo: ");
    scanf("%29[^\n]", carro->modelo);
    limparBuffer();

    printf("Cor: ");
    scanf("%14[^\n]", carro->cor);
    limparBuffer();

    carro->entrada = time(NULL);
}

void registrarEntrada() {
    Veiculo carro;
    int c, v;
    char hora[10];

    printf("\n--- ENTRADA DE VEICULO ---\n");
    cadastrarVeiculo(&carro);

    if (buscarPlaca(carro.placa, &c, &v)) {
        printf("Esse veiculo ja esta no estacionamento (vaga %c%d).\n",
               letraCorredor(c), estacionamento[c][v].numero);
        return;
    }

    if (!buscarVagaLivre(&c, &v)) {
        printf("Estacionamento lotado! Nao ha vagas livres.\n");
        return;
    }

    ocuparVaga(c, v, carro);

    strftime(hora, sizeof(hora), "%H:%M", localtime(&carro.entrada));
    printf("\nVeiculo %s estacionado na vaga %c%d as %s.\n",
           carro.placa, letraCorredor(c), estacionamento[c][v].numero, hora);
}

/* ===================== INDICADORES ===================== */

void mostrarIndicadores() {
    int c, v, totalLivres = 0;
    int total = CORREDORES * VAGAS_POR_CORREDOR;

    for (c = 0; c < CORREDORES; c++)
        totalLivres += contarLivresCorredor(c);

    printf("\n--- INDICADORES ---\n");
    printf("Total de vagas: %d\n", total);
    printf("Vagas livres:   %d\n", totalLivres);
    printf("Vagas ocupadas: %d\n", total - totalLivres);
    printf("Ocupacao:       %.1f%%\n", (total - totalLivres) * 100.0 / total);

    printf("\nVagas livres por corredor:\n");
    for (c = 0; c < CORREDORES; c++) {
        printf("Corredor %c (%d): ", letraCorredor(c), contarLivresCorredor(c));
        for (v = 0; v < VAGAS_POR_CORREDOR; v++) {
            if (!estacionamento[c][v].ocupada)
                printf("%c%d ", letraCorredor(c), estacionamento[c][v].numero);
        }
        printf("\n");
    }
}

void listarVeiculos() {
    int c, v, achou = 0;
    char hora[10];

    printf("\n--- VEICULOS ESTACIONADOS ---\n");
    for (c = 0; c < CORREDORES; c++) {
        for (v = 0; v < VAGAS_POR_CORREDOR; v++) {
            if (estacionamento[c][v].ocupada) {
                Veiculo *carro = &estacionamento[c][v].veiculo;
                strftime(hora, sizeof(hora), "%H:%M", localtime(&carro->entrada));
                printf("%c%d - %s - %s %s - entrada %s\n",
                       letraCorredor(c), estacionamento[c][v].numero,
                       carro->placa, carro->modelo, carro->cor, hora);
                achou = 1;
            }
        }
    }
    if (!achou)
        printf("Nenhum veiculo estacionado.\n");
}

/* ===================== MENU E INTEGRACAO ===================== */

int main() {
    int opcao;

    inicializarVagas();

    do {
        printf("\n===== CONTROLE DE ESTACIONAMENTO =====\n");
        printf("1 - Registrar entrada\n");
        printf("2 - Registrar saida\n");
        printf("3 - Mapa de vagas\n");
        printf("4 - Indicadores\n");
        printf("5 - Veiculos estacionados\n");
        printf("0 - Sair\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1)
            opcao = -1;
        limparBuffer();

        switch (opcao) {
            case 1: registrarEntrada(); break;
            case 2: registrarSaida(); break;
            case 3: mostrarMapa(); break;
            case 4: mostrarIndicadores(); break;
            case 5: listarVeiculos(); break;
            case 0: printf("Encerrando o sistema...\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}
