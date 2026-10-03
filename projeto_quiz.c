#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ARQUIVO    "perguntas.csv"
#define TEMPORARIO "temp.csv"
#define TAM_LINHA  600

/* =========================================================
 * ESTRUTURA DA PERGUNTA
 * ========================================================= */

typedef struct {
    int id;
    char texto[250];
    char categoria[50];
    char curso[10];
    char resposta[4];   /* "SIM" ou "NAO" */
} Pergunta;


/* =========================================================
 * FUNCOES AUXILIARES (texto)
 * ========================================================= */

/* Remove \n e \r (o \r aparece em arquivos criados no Windows) */
void removerEnter(char *texto) {
    texto[strcspn(texto, "\r\n")] = '\0';
}

/* Remove espacos no inicio e no fim */
void trim(char *s) {
    int ini = 0;
    size_t fim;

    while (isspace((unsigned char)s[ini])) ini++;
    if (ini > 0) memmove(s, s + ini, strlen(s + ini) + 1);

    fim = strlen(s);
    while (fim > 0 && isspace((unsigned char)s[fim - 1])) {
        s[--fim] = '\0';
    }
}

void maiusculo(char *s) {
    for (; *s; s++) *s = (char)toupper((unsigned char)*s);
}

/* =========================================================
 * FUNCOES DE INTERFACE
 * ========================================================= */

void limparTela(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/* Espera o usuario apertar ENTER */
void pausar(void) {
    char buf[100];

    printf("\nPressione ENTER para voltar ao menu...");
    if (fgets(buf, sizeof(buf), stdin) != NULL && strchr(buf, '\n') == NULL) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

/* Cabecalho padrao de cada tela */
void cabecalho(const char *titulo) {
    printf("\n=========================================\n");
    printf("  [ %s ]\n", titulo);
    printf("=========================================\n");
}

/* Compara dois textos sem diferenciar maiuscula/minuscula */
int iguaisIgnorandoCaixa(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++;
        b++;
    }
    return *a == '\0' && *b == '\0';
}

/* Copia sem estourar o tamanho do destino */
void copiar(char *dest, int tam, const char *orig) {
    strncpy(dest, orig, tam - 1);
    dest[tam - 1] = '\0';
}

/* Le uma linha do teclado com seguranca */
void lerTexto(const char *rotulo, char *dest, int tam) {
    printf("%s", rotulo);

    if (fgets(dest, tam, stdin) == NULL) {
        dest[0] = '\0';
        return;
    }

    /* Se a linha era maior que o buffer, descarta o resto */
    if (strchr(dest, '\n') == NULL) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }

    removerEnter(dest);
    trim(dest);
}


/* =========================================================
 * VALIDACOES
 * ========================================================= */

int cursoValido(const char *curso) {
    return strcmp(curso, "CC") == 0 ||
           strcmp(curso, "ES") == 0 ||
           strcmp(curso, "ADS") == 0;
}

int respostaValida(const char *resp) {
    return strcmp(resp, "SIM") == 0 || strcmp(resp, "NAO") == 0;
}

/* Texto nao pode ser vazio nem conter ';' (quebraria o CSV) */
int textoValido(const char *s) {
    return s[0] != '\0' && strchr(s, ';') == NULL;
}

/* Le um codigo inteiro positivo. Retorna -1 se for invalido */
int lerCodigo(const char *rotulo) {
    char buf[50];
    char *fim;
    long valor;

    lerTexto(rotulo, buf, sizeof(buf));

    if (buf[0] == '\0') return -1;

    valor = strtol(buf, &fim, 10);

    if (*fim != '\0' || valor <= 0 || valor > 1000000) return -1;

    return (int)valor;
}

void lerCampoTexto(const char *rotulo, const char *nomeCampo, char *dest, int tam) {
    do {
        lerTexto(rotulo, dest, tam);
        if (!textoValido(dest)) {
            printf("%s invalido! Nao pode ser vazio nem conter ponto e virgula (;).\n", nomeCampo);
        }
    } while (!textoValido(dest));
}

void lerCurso(char *dest) {
    do {
        lerTexto("Curso (CC/ES/ADS): ", dest, 10);
        maiusculo(dest);
        if (!cursoValido(dest)) {
            printf("Curso invalido! Digite somente CC, ES ou ADS.\n");
        }
    } while (!cursoValido(dest));
}

void lerResposta(const char *rotulo, char *dest) {
    char buf[20];

    do {
        lerTexto(rotulo, buf, sizeof(buf));
        maiusculo(buf);
        if (!respostaValida(buf)) {
            printf("Resposta invalida! Digite somente SIM ou NAO.\n");
        }
    } while (!respostaValida(buf));

    strcpy(dest, buf);
}


/* =========================================================
 * FUNCOES DE ARQUIVO
 * ========================================================= */

/* Converte uma linha do CSV em uma struct. Retorna 1 se OK */
int parseLinha(const char *linhaOriginal, Pergunta *p) {
    char linha[TAM_LINHA];
    char *campos[5];
    int n = 0;
    char *token;

    copiar(linha, sizeof(linha), linhaOriginal);

    token = strtok(linha, ";");
    while (token != NULL && n < 5) {
        campos[n++] = token;
        token = strtok(NULL, ";");
    }

    if (n != 5) return 0;

    p->id = atoi(campos[0]);
    if (p->id <= 0) return 0;

    if (strlen(campos[4]) > 3) return 0;

    copiar(p->texto, sizeof(p->texto), campos[1]);
    copiar(p->categoria, sizeof(p->categoria), campos[2]);
    copiar(p->curso, sizeof(p->curso), campos[3]);
    copiar(p->resposta, sizeof(p->resposta), campos[4]);

    return 1;
}

void gravarPergunta(FILE *arquivo, const Pergunta *p) {
    fprintf(arquivo, "%d;%s;%s;%s;%s\n",
            p->id, p->texto, p->categoria, p->curso, p->resposta);
}

/* Procura uma pergunta pelo codigo. Retorna 1 se achou */
int buscarPorId(int id, Pergunta *resultado) {
    FILE *arquivo = fopen(ARQUIVO, "r");
    char linha[TAM_LINHA];
    Pergunta p;

    if (arquivo == NULL) return 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        removerEnter(linha);
        if (parseLinha(linha, &p) && p.id == id) {
            if (resultado != NULL) *resultado = p;
            fclose(arquivo);
            return 1;
        }
    }

    fclose(arquivo);
    return 0;
}

/* Reescreve o arquivo usando um arquivo temporario.
 * nova == NULL  -> exclui a pergunta com esse id
 * nova != NULL  -> substitui a pergunta com esse id
 * Retorna 1 se deu certo */
int reescreverArquivo(int id, const Pergunta *nova) {
    FILE *original = fopen(ARQUIVO, "r");
    FILE *temp;
    char linha[TAM_LINHA];
    Pergunta p;

    if (original == NULL) return 0;

    temp = fopen(TEMPORARIO, "w");
    if (temp == NULL) {
        fclose(original);
        return 0;
    }

    while (fgets(linha, sizeof(linha), original) != NULL) {
        removerEnter(linha);

        if (linha[0] == '\0') continue;

        if (parseLinha(linha, &p) && p.id == id) {
            if (nova != NULL) gravarPergunta(temp, nova);
        } else {
            fprintf(temp, "%s\n", linha);
        }
    }

    fclose(original);
    fclose(temp);

    /* No Windows o rename falha se o destino existir, entao remove antes */
    if (remove(ARQUIVO) != 0) return 0;
    if (rename(TEMPORARIO, ARQUIVO) != 0) return 0;

    return 1;
}


/* =========================================================
 * CADASTRO DE PERGUNTA
 * ========================================================= */

void cadastrarPergunta(void) {
    Pergunta p;
    FILE *arquivo;

    cabecalho("CADASTRAR PERGUNTA");

    p.id = lerCodigo("Codigo: ");
    if (p.id == -1) {
        printf("\nCodigo invalido! Digite um numero inteiro positivo.\n");
        return;
    }

    if (buscarPorId(p.id, NULL)) {
        printf("\nJa existe uma pergunta com o codigo %d.\n", p.id);
        return;
    }

    lerCampoTexto("Pergunta: ", "Pergunta", p.texto, sizeof(p.texto));
    lerCampoTexto("Categoria: ", "Categoria", p.categoria, sizeof(p.categoria));
    lerCurso(p.curso);
    lerResposta("Resposta (SIM/NAO): ", p.resposta);

    /* "a" = acrescenta no final sem apagar o que ja existe */
    arquivo = fopen(ARQUIVO, "a");
    if (arquivo == NULL) {
        printf("\nErro ao abrir o arquivo %s para gravacao!\n", ARQUIVO);
        return;
    }

    gravarPergunta(arquivo, &p);
    fclose(arquivo);

    printf("\nPergunta cadastrada com sucesso!\n");
}


/* =========================================================
 * LISTAR PERGUNTAS
 * ========================================================= */

void listarPerguntas(void) {
    FILE *arquivo = fopen(ARQUIVO, "r");
    char linha[TAM_LINHA];
    Pergunta p;
    int encontrou = 0;

    if (arquivo == NULL) {
        printf("\nErro: nao foi possivel abrir o arquivo %s (ele ainda nao existe?).\n", ARQUIVO);
        return;
    }

    cabecalho("LISTA DE TODAS AS PERGUNTAS");
    printf("%-5s %-6s %-16s %-5s %s\n", "COD", "CURSO", "CATEGORIA", "RESP", "PERGUNTA");
    printf("-------------------------------------------------------------------------\n");

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        removerEnter(linha);

        if (parseLinha(linha, &p)) {
            printf("%-5d %-6s %-16s %-5s %s\n",
                   p.id, p.curso, p.categoria, p.resposta, p.texto);
            encontrou = 1;
        }
    }

    fclose(arquivo);

    if (!encontrou) {
        printf("\nNenhuma pergunta cadastrada.\n");
    }
}


/* =========================================================
 * CONSULTAS
 * ========================================================= */

void consultarPorCategoria(void) {
    char busca[50];
    FILE *arquivo;
    char linha[TAM_LINHA];
    Pergunta p;
    int encontrou = 0;

    cabecalho("CONSULTAR POR CATEGORIA");
    lerTexto("Categoria desejada: ", busca, sizeof(busca));

    if (busca[0] == '\0') {
        printf("\nCategoria nao informada.\n");
        return;
    }

    arquivo = fopen(ARQUIVO, "r");
    if (arquivo == NULL) {
        printf("\nErro: nao foi possivel abrir o arquivo %s (ele ainda nao existe?).\n", ARQUIVO);
        return;
    }

    printf("\nPerguntas encontradas:\n");

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        removerEnter(linha);

        if (parseLinha(linha, &p) && iguaisIgnorandoCaixa(p.categoria, busca)) {
            printf("[%d] %s - %s - %s\n", p.id, p.texto, p.curso, p.resposta);
            encontrou = 1;
        }
    }

    fclose(arquivo);

    if (!encontrou) {
        printf("Nenhuma pergunta encontrada nessa categoria.\n");
    }
}

void consultarPorCurso(void) {
    char busca[10];
    FILE *arquivo;
    char linha[TAM_LINHA];
    Pergunta p;
    int encontrou = 0;

    cabecalho("CONSULTAR POR CURSO");
    lerTexto("Curso (CC/ES/ADS): ", busca, sizeof(busca));
    maiusculo(busca);

    if (!cursoValido(busca)) {
        printf("\nCurso invalido! Digite somente CC, ES ou ADS.\n");
        return;
    }

    arquivo = fopen(ARQUIVO, "r");
    if (arquivo == NULL) {
        printf("\nErro: nao foi possivel abrir o arquivo %s (ele ainda nao existe?).\n", ARQUIVO);
        return;
    }

    printf("\nPerguntas relacionadas ao curso %s:\n", busca);

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        removerEnter(linha);

        if (parseLinha(linha, &p) && strcmp(p.curso, busca) == 0) {
            printf("[%d] %s - %s - %s\n", p.id, p.texto, p.categoria, p.resposta);
            encontrou = 1;
        }
    }

    fclose(arquivo);

    if (!encontrou) {
        printf("Nenhuma pergunta encontrada nesse curso.\n");
    }
}


/* =========================================================
 * ATUALIZAR PERGUNTA
 * ========================================================= */

void atualizarPergunta(void) {
    int id;
    Pergunta atual, nova;

    cabecalho("ATUALIZAR PERGUNTA");

    id = lerCodigo("Codigo da pergunta: ");
    if (id == -1) {
        printf("\nCodigo invalido! Digite um numero inteiro positivo.\n");
        return;
    }

    if (!buscarPorId(id, &atual)) {
        printf("\nPergunta nao encontrada.\n");
        return;
    }

    printf("\nDados atuais:\n");
    printf("Texto: %s\nCategoria: %s\nCurso: %s\nResposta: %s\n\n",
           atual.texto, atual.categoria, atual.curso, atual.resposta);

    nova.id = id;
    lerCampoTexto("Novo texto: ", "Texto", nova.texto, sizeof(nova.texto));
    lerCampoTexto("Nova categoria: ", "Categoria", nova.categoria, sizeof(nova.categoria));
    lerCurso(nova.curso);
    lerResposta("Nova resposta (SIM/NAO): ", nova.resposta);

    if (reescreverArquivo(id, &nova)) {
        printf("\nPergunta atualizada com sucesso!\n");
    } else {
        printf("\nErro ao atualizar o arquivo!\n");
    }
}


/* =========================================================
 * EXCLUIR PERGUNTA
 * ========================================================= */

void excluirPergunta(void) {
    int id;
    Pergunta alvo;
    char confirma[10];

    cabecalho("EXCLUIR PERGUNTA");

    id = lerCodigo("Codigo da pergunta: ");
    if (id == -1) {
        printf("\nCodigo invalido! Digite um numero inteiro positivo.\n");
        return;
    }

    if (!buscarPorId(id, &alvo)) {
        printf("\nPergunta nao encontrada.\n");
        return;
    }

    printf("\nPergunta: %s\n", alvo.texto);
    lerTexto("Tem certeza que deseja excluir? (S/N): ", confirma, sizeof(confirma));
    maiusculo(confirma);

    if (strcmp(confirma, "S") != 0) {
        printf("\nExclusao cancelada.\n");
        return;
    }

    if (reescreverArquivo(id, NULL)) {
        printf("\nPergunta excluida com sucesso!\n");
    } else {
        printf("\nErro ao atualizar o arquivo!\n");
    }
}


/* =========================================================
 * RESUMO ESTATISTICO POR CURSO (extra)
 * ========================================================= */

void resumoPorCurso(void) {
    FILE *arquivo = fopen(ARQUIVO, "r");
    char linha[TAM_LINHA];
    Pergunta p;
    int cc = 0, es = 0, ads = 0;

    if (arquivo == NULL) {
        printf("\nErro: nao foi possivel abrir o arquivo %s (ele ainda nao existe?).\n", ARQUIVO);
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        removerEnter(linha);

        if (parseLinha(linha, &p)) {
            if (strcmp(p.curso, "CC") == 0) cc++;
            else if (strcmp(p.curso, "ES") == 0) es++;
            else if (strcmp(p.curso, "ADS") == 0) ads++;
        }
    }

    fclose(arquivo);

    printf("\n=========================================\n");
    printf("      RESUMO ESTATISTICO POR CURSO\n");
    printf("=========================================\n");
    printf("Ciencia da Computacao (CC)     : %d pergunta(s)\n", cc);
    printf("Engenharia de Software (ES)    : %d pergunta(s)\n", es);
    printf("Analise e Des. Sistemas (ADS)  : %d pergunta(s)\n", ads);
    printf("-----------------------------------------\n");
    printf("Total                          : %d pergunta(s)\n", cc + es + ads);
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

    do {
        limparTela();
        mostrarMenu();

        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            break;
        }

        removerEnter(entrada);
        trim(entrada);

        /* Aceita exatamente um caractere */
        if (strlen(entrada) == 1) {
            opcao = entrada[0];
        } else {
            opcao = 'x';
        }

        switch (opcao) {
            case '1': cadastrarPergunta();    break;
            case '2': listarPerguntas();      break;
            case '3': consultarPorCategoria(); break;
            case '4': consultarPorCurso();    break;
            case '5': atualizarPergunta();    break;
            case '6': excluirPergunta();      break;
            case '7': resumoPorCurso();       break;
            case '0': printf("\nEncerrando o programa. Ate logo!\n"); break;
            default:  printf("\nOpcao invalida! Digite um numero de 0 a 7.\n");
        }

        if (opcao != '0') {
            pausar();
        }

    } while (opcao != '0');

    return 0;
}
