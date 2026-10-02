#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PERGUNTAS 50

/* =========================================================
 * ESTRUTURA DA PERGUNTA
 * ========================================================= */

typedef struct {
    int id;
    char texto[250];
    char categoria[50];
    char curso[50];
    char resposta;
} Pergunta;


/* =========================================================
 * VARIAVEIS GLOBAIS
 * ========================================================= */

Pergunta perguntas[MAX_PERGUNTAS];
int total_perguntas = 0;


/* =========================================================
 * FUNCAO PARA REMOVER ENTER
 * ========================================================= */

void removerEnter(char *texto) {
    texto[strcspn(texto, "\n")] = '\0';
}


/* =========================================================
 * ESCOLHA DA CATEGORIA
 * ========================================================= */

void categoria_escolha(char *categoria) {
    int opcao;

    do {
        printf("\nEscolha a categoria:\n");
        printf("1 - Raciocinio\n");
        printf("2 - Aprendizagem\n");
        printf("3 - Interesse\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            opcao = 0;
        } else {
            while (getchar() != '\n');
        }

        switch (opcao) {
            case 1:
                strcpy(categoria, "Raciocinio");
                break;

            case 2:
                strcpy(categoria, "Aprendizagem");
                break;

            case 3:
                strcpy(categoria, "Interesse");
                break;

            default:
                printf("Opcao invalida, tente novamente.\n");
        }

    } while (opcao < 1 || opcao > 3);
}


/* =========================================================
 * ESCOLHA DO CURSO
 * ========================================================= */

void curso_escolha(char *curso) {
    int opcao_curso;

    do {
        printf("\nEscolha o curso:\n");
        printf("1 - Analise e Desenvolvimento de Sistemas\n");
        printf("2 - Sistemas de Informacao\n");
        printf("3 - Ciencia da Computacao\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao_curso) != 1) {
            while (getchar() != '\n');
            opcao_curso = 0;
        } else {
            while (getchar() != '\n');
        }

        switch (opcao_curso) {
            case 1:
                strcpy(curso, "Analise e Desenvolvimento de Sistemas");
                break;

            case 2:
                strcpy(curso, "Sistemas de Informacao");
                break;

            case 3:
                strcpy(curso, "Ciencia da Computacao");
                break;

            default:
                printf("Opcao invalida, tente novamente.\n");
        }

    } while (opcao_curso < 1 || opcao_curso > 3);
}


/* =========================================================
 * CARREGAR PERGUNTAS DO CSV
 * ========================================================= */

void carregarPerguntas(void) {

    FILE *arquivo = fopen("perguntas.csv", "r");

    if (arquivo == NULL) {
        printf("\nNenhum arquivo perguntas.csv encontrado.\n");
        printf("O programa continuara normalmente.\n");
        return;
    }

    char linha[500];

    total_perguntas = 0;

    while (fgets(linha, sizeof(linha), arquivo)
           && total_perguntas < MAX_PERGUNTAS) {

        removerEnter(linha);

        char *token;

        /* ID */
        token = strtok(linha, ";");

        if (token == NULL) {
            continue;
        }

        perguntas[total_perguntas].id = atoi(token);


        /* PERGUNTA */
        token = strtok(NULL, ";");

        if (token != NULL) {
            strcpy(perguntas[total_perguntas].texto, token);
        }


        /* CATEGORIA */
        token = strtok(NULL, ";");

        if (token != NULL) {
            strcpy(perguntas[total_perguntas].categoria, token);
        }


        /* CURSO */
        token = strtok(NULL, ";");

        if (token != NULL) {
            strcpy(perguntas[total_perguntas].curso, token);
        }


        /* RESPOSTA */
        token = strtok(NULL, ";");

        if (token != NULL) {
            perguntas[total_perguntas].resposta =
                toupper((unsigned char)token[0]);
        }

        total_perguntas++;
    }

    fclose(arquivo);

    printf("\n=========================================\n");
    printf(" %d PERGUNTA(S) CARREGADA(S) DO CSV\n",
           total_perguntas);
    printf("=========================================\n");
}


/* =========================================================
 * SALVAR PERGUNTAS NO CSV
 * ========================================================= */

void salvarPerguntas(void) {

    FILE *arquivo = fopen("perguntas.csv", "w");

    if (arquivo == NULL) {
        printf("\nErro ao salvar o arquivo!\n");
        return;
    }

    int i;

    for (i = 0; i < total_perguntas; i++) {

        fprintf(arquivo, "%d;%s;%s;%s;%c\n",
                perguntas[i].id,
                perguntas[i].texto,
                perguntas[i].categoria,
                perguntas[i].curso,
                perguntas[i].resposta);
    }

    fclose(arquivo);
}


/* =========================================================
 * CADASTRO DE PERGUNTA
 * ========================================================= */

void cadastrarPergunta(void) {

    if (total_perguntas >= MAX_PERGUNTAS) {
        printf("\nLimite de perguntas atingido!\n");
        return;
    }

    Pergunta p1;

    /* Cria o ID automaticamente */
    if (total_perguntas == 0) {
        p1.id = 1;
    } else {
        p1.id = perguntas[total_perguntas - 1].id + 1;
    }


    printf("\nDigite o texto da pergunta: ");

    fgets(p1.texto, sizeof(p1.texto), stdin);

    removerEnter(p1.texto);


    categoria_escolha(p1.categoria);

    curso_escolha(p1.curso);


    do {

        printf("\nResposta correta [S/N]: ");

        scanf(" %c", &p1.resposta);

        while (getchar() != '\n');

        p1.resposta =
            toupper((unsigned char)p1.resposta);

        if (p1.resposta != 'S' &&
            p1.resposta != 'N') {

            printf("Resposta invalida, digite S ou N.\n");
        }

    } while (p1.resposta != 'S' &&
             p1.resposta != 'N');


    /* Coloca a pergunta no vetor */
    perguntas[total_perguntas] = p1;

    total_perguntas++;


    /* Salva no CSV */
    salvarPerguntas();


    printf("\nPergunta cadastrada com sucesso!\n");

    printf("-----------------------------------------\n");
    printf("ID: %d\n", p1.id);
    printf("Texto: %s\n", p1.texto);
    printf("Categoria: %s\n", p1.categoria);
    printf("Curso: %s\n", p1.curso);
    printf("Resposta: %c\n", p1.resposta);
    printf("-----------------------------------------\n");
}


/* =========================================================
 * LISTAR PERGUNTAS
 * ========================================================= */

void listarPerguntas(void) {

    int i;

    if (total_perguntas == 0) {
        printf("\nNenhuma pergunta cadastrada.\n");
        return;
    }

    printf("\n");
    printf("=================================================================================\n");
    printf("                    LISTA DE PERGUNTAS (%d)\n", total_perguntas);
    printf("=================================================================================\n");

    for (i = 0; i < total_perguntas; i++) {

        printf("\n[%02d] ID: %d\n",
               i + 1,
               perguntas[i].id);

        printf("CURSO: %s\n",
               perguntas[i].curso);

        printf("CATEGORIA: %s\n",
               perguntas[i].categoria);

        printf("PERGUNTA: %s\n",
               perguntas[i].texto);

        printf("RESPOSTA: %c\n",
               perguntas[i].resposta);

        printf("---------------------------------------------------------------------------------\n");
    }
}


/* =========================================================
 * CONSULTAR POR CATEGORIA
 * ========================================================= */

void consultarPorCategoria(void) {

    char categoria[50];

    categoria_escolha(categoria);

    int i;
    int encontrou = 0;

    printf("\n=========================================\n");
    printf(" PERGUNTAS - %s\n", categoria);
    printf("=========================================\n");

    for (i = 0; i < total_perguntas; i++) {

        if (strcmp(perguntas[i].categoria, categoria) == 0) {

            printf("\nID: %d\n", perguntas[i].id);
            printf("Pergunta: %s\n", perguntas[i].texto);
            printf("Curso: %s\n", perguntas[i].curso);
            printf("Resposta: %c\n", perguntas[i].resposta);

            printf("-----------------------------------------\n");

            encontrou = 1;
        }
    }

    if (!encontrou) {
        printf("\nNenhuma pergunta encontrada nessa categoria.\n");
    }
}


/* =========================================================
 * CONSULTAR POR CURSO
 * ========================================================= */

void consultarPorCurso(void) {

    char curso[50];

    curso_escolha(curso);

    int i;
    int encontrou = 0;

    printf("\n=========================================\n");
    printf(" PERGUNTAS - %s\n", curso);
    printf("=========================================\n");

    for (i = 0; i < total_perguntas; i++) {

        if (strcmp(perguntas[i].curso, curso) == 0) {

            printf("\nID: %d\n", perguntas[i].id);
            printf("Pergunta: %s\n", perguntas[i].texto);
            printf("Categoria: %s\n", perguntas[i].categoria);
            printf("Resposta: %c\n", perguntas[i].resposta);

            printf("-----------------------------------------\n");

            encontrou = 1;
        }
    }

    if (!encontrou) {
        printf("\nNenhuma pergunta encontrada nesse curso.\n");
    }
}


/* =========================================================
 * ATUALIZAR PERGUNTA
 * ========================================================= */

void atualizarPergunta(void) {

    int id;

    printf("\nDigite o ID da pergunta que deseja atualizar: ");
    scanf("%d", &id);

    while (getchar() != '\n');

    int i;
    int encontrou = 0;

    for (i = 0; i < total_perguntas; i++) {

        if (perguntas[i].id == id) {

            encontrou = 1;

            printf("\nDigite o novo texto da pergunta: ");

            fgets(perguntas[i].texto,
                  sizeof(perguntas[i].texto),
                  stdin);

            removerEnter(perguntas[i].texto);

            categoria_escolha(perguntas[i].categoria);

            curso_escolha(perguntas[i].curso);


            do {

                printf("\nNova resposta correta [S/N]: ");

                scanf(" %c", &perguntas[i].resposta);

                while (getchar() != '\n');

                perguntas[i].resposta =
                    toupper((unsigned char)perguntas[i].resposta);

            } while (perguntas[i].resposta != 'S' &&
                     perguntas[i].resposta != 'N');


            salvarPerguntas();

            printf("\nPergunta atualizada com sucesso!\n");

            break;
        }
    }

    if (!encontrou) {
        printf("\nPergunta nao encontrada.\n");
    }
}


/* =========================================================
 * EXCLUIR PERGUNTA
 * ========================================================= */

void excluirPergunta(void) {

    int id;

    printf("\nDigite o ID da pergunta que deseja excluir: ");
    scanf("%d", &id);

    while (getchar() != '\n');

    int i;
    int encontrou = 0;

    for (i = 0; i < total_perguntas; i++) {

        if (perguntas[i].id == id) {

            encontrou = 1;

            /* Move as perguntas seguintes uma posição para trás */
            int j;

            for (j = i; j < total_perguntas - 1; j++) {

                perguntas[j] = perguntas[j + 1];
            }

            total_perguntas--;

            salvarPerguntas();

            printf("\nPergunta excluida com sucesso!\n");

            break;
        }
    }

    if (!encontrou) {
        printf("\nPergunta nao encontrada.\n");
    }
}


/* =========================================================
 * MENU
 * ========================================================= */

void mostrarMenu(void) {

    printf("\n=========================================\n");
    printf(" GERENCIADOR DE PERGUNTAS - QUIZ DE TI\n");
    printf("=========================================\n");

    printf("1 - Cadastrar pergunta\n");
    printf("2 - Listar todas as perguntas\n");
    printf("3 - Consultar perguntas por categoria\n");
    printf("4 - Consultar perguntas por curso\n");
    printf("5 - Atualizar pergunta\n");
    printf("6 - Excluir pergunta\n");
    printf("0 - Sair\n");

    printf("-----------------------------------------\n");
    printf("Escolha uma opcao: ");
}


/* =========================================================
 * MAIN
 * ========================================================= */

int main(void) {

    char entrada[100];
    char opcao;


    /* Carrega as perguntas do CSV ao iniciar */
    carregarPerguntas();


    do {

        mostrarMenu();

        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            break;
        }

        removerEnter(entrada);


        /* Aceita exatamente um caractere */
        if (strlen(entrada) == 1) {
            opcao = entrada[0];
        } else {
            opcao = 'x';
        }


        switch (opcao) {

            case '1':
                cadastrarPergunta();
                break;


            case '2':
                listarPerguntas();
                break;


            case '3':
                consultarPorCategoria();
                break;


            case '4':
                consultarPorCurso();
                break;


            case '5':
                atualizarPergunta();
                break;


            case '6':
                excluirPergunta();
                break;


            case '0':
                printf("\nEncerrando o programa. Ate logo!\n");
                break;


            default:
                printf("\nOpcao invalida! Digite um numero de 0 a 6.\n");
        }

    } while (opcao != '0');


    return 0;
}
