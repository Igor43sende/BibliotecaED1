#include <string.h> // Inclui a biblioteca para manipulação de strings
#include <stdio.h> // Inclui a biblioteca para entrada e saída padrão
#include <stdlib.h> // Inclui a biblioteca para alocação de memória e funções utilitárias
#include <ctype.h> // Inclui a biblioteca para manipulação de caracteres

#define TABLE_SIZE 100 // Tamanho das tabelas hash

// Estrutura para representar um livro
typedef struct Livro {
    int id; // Identificador do livro
    char titulo[100]; // Título do livro
    char autor[100]; // Autor do livro
    int ano; // Ano de publicação
    char genero[100]; // Gênero do livro
    int emprestado; // 0 = não emprestado, 1 = emprestado
    char emprestadoPara[100]; // Nome da pessoa que pegou emprestado
    struct Livro *proximo; // Ponteiro para o próximo livro na lista
} Livro;

// Variáveis globais
Livro *lista = NULL; // Lista encadeada principal
Livro *hashTitulo[TABLE_SIZE]; // Tabela hash para títulos
Livro *hashAutor[TABLE_SIZE]; // Tabela hash para autores
Livro *hashAno[TABLE_SIZE]; // Tabela hash para anos

// Função hash para strings (usada para título e autor)
unsigned int hashFunction(const char *str) {
    unsigned int hash = 0; // Inicializa o valor do hash
    while (*str) { // Percorre cada caractere da string
        hash = (hash * 31) + *str++; // Calcula o hash
    }
    return hash % TABLE_SIZE; // Retorna o índice da tabela hash
}

// Função hash para inteiros (usada para ano)
unsigned int hashFunctionInt(int num) {
    return num % TABLE_SIZE; // Retorna o índice da tabela hash
}

// Função para criar um novo livro
Livro *criar(int id, const char *titulo, const char *autor, int ano, const char *genero) {
    Livro *novo = (Livro *)malloc(sizeof(Livro)); // Aloca memória para o novo livro
    novo->id = id; // Define o id
    strcpy(novo->titulo, titulo); // Copia o título
    strcpy(novo->autor, autor); // Copia o autor
    novo->ano = ano; // Define o ano
    strcpy(novo->genero, genero); // Copia o gênero
    novo->emprestado = 0; // Inicializa como não emprestado
    strcpy(novo->emprestadoPara, ""); // Inicializa o campo de empréstimo vazio
    novo->proximo = NULL; // Inicializa o ponteiro para o próximo como NULL
    return novo; // Retorna o ponteiro para o novo livro
}

// Função para verificar se um ID já existe
int idExiste(int id) {
    Livro *atual = lista; // Começa do início da lista
    while (atual != NULL) { // Percorre a lista
        if (atual->id == id) { // Se encontrar o id
            return 1; // ID já existe
        }
        atual = atual->proximo; // Vai para o próximo livro
    }
    return 0; // ID não existe
}

// Função para inserir um livro na lista encadeada
void inserirNaLista(int id, const char *titulo, const char *autor, int ano, const char *genero) {
    Livro *novo = criar(id, titulo, autor, ano, genero); // Cria um novo livro
    if (lista == NULL) { // Se a lista estiver vazia
        lista = novo; // O novo livro vira o início da lista
    } else {
        Livro *atual = lista; // Começa do início da lista
        while (atual->proximo != NULL) { // Percorre até o final
            atual = atual->proximo;
        }
        atual->proximo = novo; // Insere no final
    }
}

// Função para inserir um livro nas tabelas hash
void inserirNasHash(int id, const char *titulo, const char *autor, int ano, const char *genero) {
    // Criar um novo livro para cada tabela hash
    Livro *novoTitulo = criar(id, titulo, autor, ano, genero); // Para tabela de títulos
    Livro *novoAutor = criar(id, titulo, autor, ano, genero); // Para tabela de autores
    Livro *novoAno = criar(id, titulo, autor, ano, genero); // Para tabela de anos

    // Inserir na tabela hash de títulos
    unsigned int indexTitulo = hashFunction(titulo); // Calcula o índice
    novoTitulo->proximo = hashTitulo[indexTitulo]; // Insere no início da lista da hash
    hashTitulo[indexTitulo] = novoTitulo; // Atualiza o ponteiro da hash

    // Inserir na tabela hash de autores
    unsigned int indexAutor = hashFunction(autor); // Calcula o índice
    novoAutor->proximo = hashAutor[indexAutor]; // Insere no início da lista da hash
    hashAutor[indexAutor] = novoAutor; // Atualiza o ponteiro da hash

    // Inserir na tabela hash de anos
    unsigned int indexAno = hashFunctionInt(ano); // Calcula o índice
    novoAno->proximo = hashAno[indexAno]; // Insere no início da lista da hash
    hashAno[indexAno] = novoAno; // Atualiza o ponteiro da hash
}

// Função para inserir um livro na lista e nas tabelas hash
void inserir(int id, const char *titulo, const char *autor, int ano, const char *genero) {
    if (idExiste(id)) { // Verifica se o id já existe
        printf("Erro: ID %d já existe.\n", id); // Mensagem de erro
        return; // Sai da função
    }
    inserirNaLista(id, titulo, autor, ano, genero); // Insere na lista
    inserirNasHash(id, titulo, autor, ano, genero); // Insere nas tabelas hash
    printf("Livro inserido com sucesso!\n"); // Mensagem de sucesso
}

// Função para exibir todos os livros
void exibir(Livro *lista) {
    Livro *atual = lista; // Começa do início da lista
    while (atual != NULL) { // Percorre a lista
        printf("ID: %d\nTítulo: %s\nAutor: %s\nAno: %d\nGênero: %s\nEmprestado: %s\nPara: %s\n\n",
               atual->id, atual->titulo, atual->autor, atual->ano, atual->genero,
               atual->emprestado ? "Sim" : "Não", atual->emprestadoPara); // Exibe os dados
        atual = atual->proximo; // Vai para o próximo livro
    }
}

// Função para buscar todos os livros por título
void buscarPorTitulo(const char *titulo) {
    unsigned int index = hashFunction(titulo); // Calcula o índice na hash
    Livro *atual = hashTitulo[index]; // Pega o início da lista na hash
    int encontrados = 0; // Contador de encontrados

    printf("\n--- Resultados da busca por título: '%s' ---\n", titulo); // Cabeçalho
    while (atual != NULL) { // Percorre a lista
        if (strcmp(atual->titulo, titulo) == 0) { // Compara o título
            printf("ID: %d\nTítulo: %s\nAutor: %s\nAno: %d\nGênero: %s\nEmprestado: %s\nPara: %s\n\n",
                   atual->id, atual->titulo, atual->autor, atual->ano, atual->genero,
                   atual->emprestado ? "Sim" : "Não", atual->emprestadoPara); // Exibe os dados
            encontrados++; // Incrementa o contador
        }
        atual = atual->proximo; // Vai para o próximo livro
    }

    if (encontrados == 0) { // Se não encontrou nenhum
        printf("Nenhum livro encontrado com o título '%s'.\n", titulo); // Mensagem
    }
}

// Função para converter uma string para minúsculas
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) { // Percorre cada caractere
        str[i] = tolower(str[i]); // Converte para minúsculo
    }
}

// Função para buscar todos os livros por autor
void buscarPorAutor(const char *autor) {
    unsigned int index = hashFunction(autor); // Calcula o índice na hash
    Livro *atual = hashAutor[index]; // Pega o início da lista na hash
    int encontrados = 0; // Contador de encontrados

    // Converter o autor inserido para minúsculas para comparação
    char autorLower[100]; // Buffer para autor em minúsculas
    strcpy(autorLower, autor); // Copia o autor
    toLowerCase(autorLower); // Converte para minúsculas

    printf("\n--- Resultados da busca por autor: '%s' ---\n", autor); // Cabeçalho

    // Itera sobre todos os livros na posição da tabela hash
    while (atual != NULL) { // Percorre a lista
        // Converter o autor do livro para minúsculas para comparação
        char atualAutorLower[100]; // Buffer para autor do livro em minúsculas
        strcpy(atualAutorLower, atual->autor); // Copia o autor do livro
        toLowerCase(atualAutorLower); // Converte para minúsculas

        // Verifica se o autor é igual (ignorando maiúsculas/minúsculas)
        if (strcmp(atualAutorLower, autorLower) == 0) { // Compara autores
            printf("ID: %d\nTítulo: %s\nAutor: %s\nAno: %d\nGênero: %s\nEmprestado: %s\nPara: %s\n\n",
                   atual->id, atual->titulo, atual->autor, atual->ano, atual->genero,
                   atual->emprestado ? "Sim" : "Não", atual->emprestadoPara); // Exibe os dados
            encontrados++; // Incrementa o contador
        }
        // Move para o próximo livro na lista encadeada
        atual = atual->proximo; // Vai para o próximo livro
    }

    if (encontrados == 0) { // Se não encontrou nenhum
        printf("Nenhum livro encontrado com o autor '%s'.\n", autor); // Mensagem
    }
}

// Função para buscar todos os livros por ano
void buscarPorAno(int ano) {
    unsigned int index = hashFunctionInt(ano); // Calcula o índice na hash
    Livro *atual = hashAno[index]; // Pega o início da lista na hash
    int encontrados = 0; // Contador de encontrados

    printf("\n--- Resultados da busca por ano: %d ---\n", ano); // Cabeçalho

    // Itera sobre todos os livros na posição da tabela hash
    while (atual != NULL) { // Percorre a lista
        // Verifica se o ano é igual
        if (atual->ano == ano) { // Compara o ano
            printf("ID: %d\nTítulo: %s\nAutor: %s\nAno: %d\nGênero: %s\nEmprestado: %s\nPara: %s\n\n",
                   atual->id, atual->titulo, atual->autor, atual->ano, atual->genero,
                   atual->emprestado ? "Sim" : "Não", atual->emprestadoPara); // Exibe os dados
            encontrados++; // Incrementa o contador
        }
        // Move para o próximo livro na lista encadeada
        atual = atual->proximo; // Vai para o próximo livro
    }

    if (encontrados == 0) { // Se não encontrou nenhum
        printf("Nenhum livro encontrado com o ano %d.\n", ano); // Mensagem
    }
}


// Função para registrar empréstimo de um livro
void registrarEmprestimo(const char *titulo, const char *nomePessoa) {
    unsigned int index = hashFunction(titulo); // Calcula o índice na hash
    Livro *atual = hashTitulo[index]; // Pega o início da lista na hash
    int encontrados = 0; // Contador de encontrados

    while (atual != NULL) { // Percorre a lista
        if (strcmp(atual->titulo, titulo) == 0) { // Compara o título
            if (atual->emprestado) { // Se já está emprestado
                printf("O livro '%s' já está emprestado para %s.\n", titulo, atual->emprestadoPara); // Mensagem
            } else {
                atual->emprestado = 1; // Marca como emprestado
                strcpy(atual->emprestadoPara, nomePessoa); // Registra o nome da pessoa
                printf("Empréstimo registrado: '%s' emprestado para %s.\n", titulo, nomePessoa); // Mensagem
            }
            encontrados++; // Incrementa o contador
        }
        atual = atual->proximo; // Vai para o próximo livro
    }

    if (encontrados == 0) { // Se não encontrou nenhum
        printf("Livro com título '%s' não encontrado.\n", titulo); // Mensagem
    }
}

// Função para registrar devolução de um livro
void registrarDevolucao(const char *titulo) {
    unsigned int index = hashFunction(titulo); // Calcula o índice na hash
    Livro *atual = hashTitulo[index]; // Pega o início da lista na hash
    int encontrados = 0; // Contador de encontrados

    while (atual != NULL) { // Percorre a lista
        if (strcmp(atual->titulo, titulo) == 0) { // Compara o título
            if (atual->emprestado) { // Se está emprestado
                atual->emprestado = 0; // Marca como disponível
                strcpy(atual->emprestadoPara, ""); // Limpa o nome da pessoa
                printf("Devolução registrada: '%s' está disponível novamente.\n", titulo); // Mensagem
            } else {
                printf("O livro '%s' já está disponível.\n", titulo); // Mensagem
            }
            encontrados++; // Incrementa o contador
        }
        atual = atual->proximo; // Vai para o próximo livro
    }

    if (encontrados == 0) { // Se não encontrou nenhum
        printf("Livro com título '%s' não encontrado.\n", titulo); // Mensagem
    }
}

// Função para carregar livros de um arquivo
void carregar(const char *arquivoNome) {
    FILE *arquivo = fopen(arquivoNome, "r"); // Abre o arquivo para leitura
    if (arquivo == NULL) { // Se não conseguiu abrir
        printf("Erro ao abrir o arquivo.\n"); // Mensagem de erro
        return; // Sai da função
    }
    int id, ano; // Variáveis auxiliares
    char titulo[100], autor[100], genero[100]; // Buffers para os campos
    while (fscanf(arquivo, "%d;%99[^;];%99[^;];%d;%99[^\n]\n", &id, titulo, autor, &ano, genero) == 5) { // Lê os dados do arquivo
        inserir(id, titulo, autor, ano, genero); // Insere o livro
    }
    fclose(arquivo); // Fecha o arquivo
    printf("Livros carregados com sucesso.\n"); // Mensagem de sucesso
}

// Função para gerar relatório de livros cadastrados
void relatorioLivrosCadastrados() {
    Livro *atual = lista; // Começa do início da lista
    printf("\n--- Relatório de Livros Cadastrados ---\n"); // Cabeçalho
    while (atual != NULL) { // Percorre a lista
        printf("ID: %d\nTítulo: %s\nAutor: %s\nAno: %d\nGênero: %s\nEmprestado: %s\nPara: %s\n\n",
               atual->id, atual->titulo, atual->autor, atual->ano, atual->genero,
               atual->emprestado ? "Sim" : "Não", atual->emprestadoPara); // Exibe os dados
        atual = atual->proximo; // Vai para o próximo livro
    }
}

// Função para liberar a memória alocada
void liberarMemoria() {
    Livro *atual = lista; // Começa do início da lista
    Livro *proximo; // Ponteiro auxiliar
    while (atual != NULL) { // Percorre a lista
        proximo = atual->proximo; // Guarda o próximo
        free(atual); // Libera o atual
        atual = proximo; // Vai para o próximo
    }
}

// Função principal
int main() {
    // Inicializar tabelas hash
    for (int i = 0; i < TABLE_SIZE; i++) { // Percorre todas as posições
        hashTitulo[i] = NULL;
        hashAutor[i] = NULL;
        hashAno[i] = NULL;
    }

    carregar("livrosED.txt"); // Carregar livros do arquivo

    int opcao, id, ano; // Variáveis auxiliares
    char titulo[100], autor[100], genero[100], nomePessoa[100]; // Buffers para os campos

    do {
        printf("\nMENU\n"); // Exibe o menu
        printf("1. Exibir Livros\n");
        printf("2. Inserir Livro\n");
        printf("3. Buscar por Título\n");
        printf("4. Buscar por Autor\n");
        printf("5. Buscar por Ano\n");
        printf("6. Registrar Empréstimo\n");
        printf("7. Registrar Devolução\n");
        printf("8. Relatório de Livros Cadastrados\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao); // Lê a opção do usuário
        getchar(); // Limpar o buffer do teclado após o scanf

        switch (opcao) {
            case 1:
                exibir(lista);
                break;
            case 2:
                printf("Digite o ID: ");
                scanf("%d", &id);
                getchar();
                printf("Digite o título: ");
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = '\0';
                printf("Digite o autor: ");
                fgets(autor, sizeof(autor), stdin);
                autor[strcspn(autor, "\n")] = '\0';
                printf("Digite o ano: ");
                scanf("%d", &ano);
                getchar();
                printf("Digite o gênero: ");
                fgets(genero, sizeof(genero), stdin);
                genero[strcspn(genero, "\n")] = '\0';
                inserir(id, titulo, autor, ano, genero);
                break;
            case 3:
                printf("Digite o título: ");
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = '\0';
                buscarPorTitulo(titulo); 
                break;
            case 4:
                printf("Digite o autor: ");
                fgets(autor, sizeof(autor), stdin);
                autor[strcspn(autor, "\n")] = '\0';
                break;
            case 5:
                printf("Digite o ano: ");
                scanf("%d", &ano);
                buscarPorAno(ano);
                break;
            case 6:
                printf("Digite o título do livro: ");
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = '\0';
                printf("Digite o nome da pessoa: ");
                fgets(nomePessoa, sizeof(nomePessoa), stdin);
                nomePessoa[strcspn(nomePessoa, "\n")] = '\0';
                registrarEmprestimo(titulo, nomePessoa);
                break;
            case 7:
                printf("Digite o título do livro: ");
                fgets(titulo, sizeof(titulo), stdin); 
                titulo[strcspn(titulo, "\n")] = '\0'; 
                registrarDevolucao(titulo); 
                break;
            case 8:
                relatorioLivrosCadastrados();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0); 

    liberarMemoria(); // Liberar memória alocada

    return 0;
}