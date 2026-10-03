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
    char curso[10];
    char resposta[5];
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
        printf("1 - Analise e Desenvolvimento de Sistemas (ADS)\n");
        printf("2 - Engenharia de Software (ES)\n");
        printf("3 - Ciencia da Computacao (CC)\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao_curso) != 1) {
            while (getchar() != '\n');
            opcao_curso = 0;
        } else {
            while (getchar() != '\n');
        }

        switch (opcao_curso) {
            case 1: strcpy(curso, "ADS"); break;
            case 2: strcpy(curso, "ES"); break;
            case 3: strcpy(curso, "CC"); break;
            default: printf("Opcao invalida, tente novamente.\n");
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
            strcpy(perguntas[total_perguntas].resposta, token);
        }

        total_perguntas++;
    }
    
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

        fprintf(arquivo, "%d;%s;%s;%s;%s\n",
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


       int resp_opcao;
    do {
        printf("\nResposta correta:\n1 - SIM\n2 - NAO\nOpcao: ");
        if (scanf("%d", &resp_opcao) != 1) {
            while (getchar() != '\n');
            resp_opcao = 0;
        } else {
            while (getchar() != '\n');
        }

        if (resp_opcao == 1) strcpy(p1.resposta, "SIM");
        else if (resp_opcao == 2) strcpy(p1.resposta, "NAO");
        else printf("Opcao invalida! Escolha 1 ou 2.\n");

    } while (resp_opcao != 1 && resp_opcao != 2);



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
    printf("Resposta: %s\n", p1.resposta);
    printf("-----------------------------------------\n");
}


/* =========================================================
 * LISTAR PERGUNTAS
 * ========================================================= */

void listarPerguntas(void) {
    FILE *arquivo = fopen("perguntas.csv", "r");
    if (arquivo == NULL) {
        printf("\nErro: nao foi possivel abrir o arquivo perguntas.csv\n");
        return;
    }

    char linha[500];
    int encontrou = 0;

    printf("\n");
    printf("=================================================================================\n");
    printf("                    LISTA DE TODAS AS PERGUNTAS\n");
    printf("=================================================================================\n");

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        removerEnter(linha);

        char *id = strtok(linha, ";");
        char *texto = strtok(NULL, ";");
        char *categoria = strtok(NULL, ";");
        char *curso = strtok(NULL, ";");
        char *resposta = strtok(NULL, ";");

        if (id != NULL && texto != NULL) {
            printf("\nID: %s\n", id);
            printf("Pergunta: %s\n", texto);
            printf("Categoria: %s\n", categoria ? categoria : "-");
            printf("Curso: %s\n", curso ? curso : "-");
            printf("Resposta: %s\n", resposta ? resposta : "-");
            printf("---------------------------------------------------------------------------------\n");
            encontrou = 1;
        }
    }

    if (!encontrou) {
        printf("\nNenhuma pergunta cadastrada.\n");
    }

    fclose(arquivo);
}

/* =========================================================
 * CONSULTAR POR CATEGORIA
 * ========================================================= */

void consultarPorCategoria(void) {
    char categoriaBusca[50];
    int encontrou = 0;

    printf("\nDigite a categoria desejada: ");
    fgets(categoriaBusca, sizeof(categoriaBusca), stdin);
    removerEnter(categoriaBusca);

    FILE *arquivo = fopen("perguntas.csv", "r");
    if (arquivo == NULL) {
        printf("\nErro: nao foi possivel abrir o arquivo perguntas.csv\n");
        return;
    }

    char linha[500];

    printf("\n=========================================\n");
    printf(" PERGUNTAS DA CATEGORIA: %s\n", categoriaBusca);
    printf("=========================================\n");

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        removerEnter(linha);

        char *id = strtok(linha, ";");
        char *texto = strtok(NULL, ";");
        char *categoria = strtok(NULL, ";");
        char *curso = strtok(NULL, ";");
        char *resposta = strtok(NULL, ";");

        if (id != NULL && texto != NULL && categoria != NULL) {
            if (strcmp(categoria, categoriaBusca) == 0) {
                printf("\n[%s] %s - %s - %s\n", id, texto, curso ? curso : "-", resposta ? resposta : "-");
                encontrou = 1;
            }
        }
    }

    if (!encontrou) {
        printf("\nNenhuma pergunta encontrada nessa categoria.\n");
    }

    fclose(arquivo);
}


/* =========================================================
 * CONSULTAR POR CURSO
 * ========================================================= */

void consultarPorCurso(void) {
    char cursoBusca[50];
    int encontrou = 0;

    printf("\nDigite o curso desejado (CC, ES ou ADS): ");
    fgets(cursoBusca, sizeof(cursoBusca), stdin);
    removerEnter(cursoBusca);

    FILE *arquivo = fopen("perguntas.csv", "r");
    if (arquivo == NULL) {
        printf("\nErro: nao foi possivel abrir o arquivo perguntas.csv\n");
        return;
    }

    char linha[500];

    printf("\n=========================================\n");
    printf(" PERGUNTAS DO CURSO: %s\n", cursoBusca);
    printf("=========================================\n");

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        removerEnter(linha);

        char *id = strtok(linha, ";");
        char *texto = strtok(NULL, ";");
        char *categoria = strtok(NULL, ";");
        char *curso = strtok(NULL, ";");
        char *resposta = strtok(NULL, ";");

        if (id != NULL && texto != NULL && curso != NULL) {
            if (strcmp(curso, cursoBusca) == 0) {
                printf("\n[%s] %s - %s - %s\n", id, texto, categoria ? categoria : "-", resposta ? resposta : "-");
                encontrou = 1;
            }
        }
    }

    if (!encontrou) {
        printf("\nNenhuma pergunta encontrada nesse curso.\n");
    }

    fclose(arquivo);
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


            int resp_opcao;
            do {
                printf("\nNova resposta correta:\n1 - SIM\n2 - NAO\nOpcao: ");
                if (scanf("%d", &resp_opcao) != 1) {
                    while (getchar() != '\n');
                    resp_opcao = 0;
                } else {
                    while (getchar() != '\n');
                }

                if (resp_opcao == 1) strcpy(perguntas[i].resposta, "SIM");
                else if (resp_opcao == 2) strcpy(perguntas[i].resposta, "NAO");
                else printf("Opcao invalida! Escolha 1 ou 2.\n");

            } while (resp_opcao != 1 && resp_opcao != 2);


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
 * EXIBIR RESUMO ESTATÍSTICO POR CURSO
 * ========================================================= */
void resumoPorCurso(void) {
    FILE *arquivo = fopen("perguntas.csv", "r");
    if (arquivo == NULL) {
        printf("\nErro: nao foi possivel abrir o arquivo perguntas.csv\n");
        return;
    }

    int cc = 0, es = 0, ads = 0, outros = 0;
    char linha[500];

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        removerEnter(linha);
        if (strlen(linha) == 0) continue;

        char *id = strtok(linha, ";");
        char *texto = strtok(NULL, ";");
        char *categoria = strtok(NULL, ";");
        char *curso = strtok(NULL, ";");
        char *resposta = strtok(NULL, ";"); // Quinta chamada obrigatória para isolar o curso

        if (curso != NULL) {
            if (strcmp(curso, "CC") == 0) {
                cc++;
            } else if (strcmp(curso, "ES") == 0) {
                es++;
            } else if (strcmp(curso, "ADS") == 0) {
                ads++;
            } else {
                outros++;
            }
        }
    }

    fclose(arquivo);

    printf("\n=========================================\n");
    printf("         RESUMO ESTATISTICO POR CURSO\n");
    printf("=========================================\n");
    printf("Ciencia da Computacao (CC)     : %d pergunta(s)\n", cc);
    printf("Engenharia de Software (ES)    : %d pergunta(s)\n", es);
    printf("Analise e Des. Sistemas (ADS)  : %d pergunta(s)\n", ads);
    if (outros > 0) {
        printf("Outros/Nao identificados       : %d pergunta(s)\n", outros);
    }
    printf("-----------------------------------------\n");
    printf("Total Geral no Banco           : %d pergunta(s)\n", cc + es + ads + outros);
    printf("=========================================\n");
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
    printf("7 - Exibir resumo estatistico por curso\n"); 
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
            
            case '7':
                resumoPorCurso();
                break;
    
            case '0':
                printf("\nEncerrando o programa. Ate logo!\n");
                break;


            default:
                printf("\nOpcao invalida! Digite um numero de 0 a 7.\n");
        }

    } while (opcao != '0');


    return 0;
}
