
#include <stdio.h>
#include <string.h>
#include <ctype.h>

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
 * FUNÇÕES AUXILIARES
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
 * CADASTRO DE PERGUNTA
 * ========================================================= */

Pergunta cadastro_pergunta(void) {
    static int proximoId = 1;
    Pergunta p1;

    p1.id = proximoId++;

    printf("\nDigite o texto da pergunta: ");
    fgets(p1.texto, sizeof(p1.texto), stdin);
    removerEnter(p1.texto);

    categoria_escolha(p1.categoria);
    curso_escolha(p1.curso);

    do {
        printf("\nResposta correta [S/N]: ");
        scanf(" %c", &p1.resposta);
        while (getchar() != '\n');

        p1.resposta = toupper((unsigned char)p1.resposta);

        if (p1.resposta != 'S' && p1.resposta != 'N') {
            printf("Resposta invalida, digite S ou N.\n");
        }

    } while (p1.resposta != 'S' && p1.resposta != 'N');

    return p1;
}

/* =========================================================
 * CADASTRAR PERGUNTA
 * ========================================================= */

void cadastrarPergunta(void) {
    Pergunta p1 = cadastro_pergunta();

    printf("\nPergunta cadastrada!\n");
    printf("-----------------------------------------\n");
    printf("ID: %d\n", p1.id);
    printf("Texto: %s\n", p1.texto);
    printf("Categoria: %s\n", p1.categoria);
    printf("Curso: %s\n", p1.curso);
    printf("Resposta: %c\n", p1.resposta);
    printf("-----------------------------------------\n");

    /* Aqui, a pergunta pode ser armazenada
       no vetor ou banco de dados do projeto. */
}

/* =========================================================
 * DECLARACAO DAS DEMAIS FUNCOES
 * ========================================================= */

/* Mantenha aqui as implementacoes dessas funcoes
   que voce ja possui no seu projeto. */

void listarPerguntas(void);
void consultarPorCategoria(void);
void consultarPorCurso(void);
void atualizarPergunta(void);
void excluirPergunta(void);

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
