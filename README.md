# Gerenciador de Perguntas - Quiz de TI

**Turma:** TI - 2º semestre - Manhã

**Integrantes:**
1. [Gabriel Oliveira dos Santos]
2. [Gustavo de Oliveira Andrade]
3. [Gabriel Venancio Lima]
4. [Isabelle Boato de Souza ]

**Apresentacao em video:** [[link do YouTube](https://youtu.be/-DWLz1PWaDM)]

## Descricao

Sistema em linguagem C, executado no terminal, para cadastrar e organizar o banco de perguntas de um futuro quiz de orientacao para estudantes do Ensino Medio interessados nos cursos de Ciencia da Computacao (CC), Engenharia de Software (ES) e Analise e Desenvolvimento de Sistemas (ADS).

Nesta etapa o sistema faz apenas o **gerenciamento das perguntas**. O quiz nao e aplicado e nao ha calculo de pontuacao ou de resultado de curso.

## Funcionalidades

1. Cadastrar pergunta
2. Listar todas as perguntas
3. Consultar perguntas por categoria
4. Consultar perguntas por curso
5. Atualizar pergunta
6. Excluir pergunta
7. Exibir resumo estatistico por curso (extra)
0. Sair

## Validacoes

- Opcao do menu valida
- Codigo da pergunta: numero inteiro positivo e sem repeticao
- Curso: somente CC, ES ou ADS
- Resposta: somente SIM ou NAO
- Perguntas vazias ou com ponto e virgula nao sao gravadas
- Tratamento de erro na abertura do arquivo
- Mensagem clara quando uma busca nao encontra resultados

## Arquivo de dados

As perguntas ficam em `perguntas.csv`, uma por linha, com campos separados por ponto e vírgula.

Formato de cada linha: código; pergunta; categoria; curso; resposta.

```
1;Voce gosta de resolver problemas de logica?;Raciocinio;CC;SIM
```

Os dados permanecem salvos depois que o programa é encerrado.

## Arquivos do repositório

- `quiz_perguntas.c`: código-fonte completo
- `perguntas.csv`: perguntas usadas nos testes
- `README.md`: este arquivo

