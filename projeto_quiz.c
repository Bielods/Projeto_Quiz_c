#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    char texto[250];
    char categoria[50];
    char curso[50];
    char resposta;
} Pergunta;

void categoria_escolha(char *categoria) {
    int opcao;

    do {
        printf("Escolha a categoria:\n1- Raciocinio\n2- Aprendizagem\n3- Interesse\n");
        scanf("%d", &opcao);
        while (getchar() != '\n');  

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
                printf("Opcao invalida, tente de novo.\n");
        }
    } while (opcao < 1 || opcao > 3);
}

void curso_escolha(char *curso) {
    int opcao_curso;

    do {
        printf("Escolha o curso:\n1- Analise e Desenvolvimento de Sistemas\n2- Sistemas de Informacao\n3- Ciencia da Computacao\n");
        scanf("%d", &opcao_curso);
        while (getchar() != '\n');

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
                printf("Opcao invalida, tente de novo.\n");
        }
    } while (opcao_curso < 1 || opcao_curso > 3);
}



Pergunta cadastro_pergunta() {
    Pergunta p1;

    printf("Texto: ");
    fgets(p1.texto, sizeof(p1.texto), stdin);
    p1.texto[strcspn(p1.texto, "\n")] = '\0';  

    categoria_escolha(p1.categoria);
    curso_escolha(p1.curso);
    
    do {
        printf("Resposta correta [S/N]: ");
        scanf(" %c", &p1.resposta);
        while (getchar() != '\n');
        p1.resposta = toupper(p1.resposta);

        if (p1.resposta != 'S' && p1.resposta != 'N') {
            printf("Resposta invalida, digite S ou N.\n");
        }
    } while (p1.resposta != 'S' && p1.resposta != 'N');
	
    return p1;
}

int main() {
    Pergunta p1 = cadastro_pergunta();
    printf("Texto: %s\n", p1.texto);
    printf("Categoria: %s\n", p1.categoria);
    printf("Curso: %s\n", p1.curso);
    printf("Resposta: %c\n", p1.resposta);
    
    return 0;
}
