# Sistema de Gerenciamento de Biblioteca (C)

## Linguagem Utilizada

- C (linguagem estruturada)
- Manipulação de arquivos `.txt`
- Estruturas: listas encadeadas e tabelas hash

## Funcionalidades

Este projeto simula um sistema de gerenciamento de biblioteca com as seguintes funções principais:

### Estruturas de dados
- Lista encadeada para armazenamento dos livros.
- Tabelas hash para busca rápida por:
  - Título
  - Autor
  - Ano de publicação

### Menu do sistema

1. Exibir livros cadastrados
2. Inserir novo livro
3. Buscar livro por título
4. Buscar livro por autor
5. Buscar livro por ano
6. Registrar empréstimo
7. Registrar devolução
8. Relatório de livros cadastrados
0. Sair

### Arquivo de dados

- O sistema carrega automaticamente os livros a partir do arquivo `livrosED.txt`, no formato:
  ```
  id;título;autor;ano;gênero
  ```

## Como Executar no Windows

### Requisitos

- Compilador C (GCC via MinGW ou Code::Blocks)
- Editor de texto (VS Code, Code::Blocks, etc.)
- Arquivo `livrosED.txt` com dados

### Passo a passo com GCC (MinGW)

1. Salve `Trabalho.c` e `livrosED.txt` na mesma pasta.
2. Abra o Prompt de Comando e navegue até a pasta:
   ```bash
   cd caminho\da\pasta
   ```
3. Compile:
   ```bash
   gcc Trabalho.c -o biblioteca.exe
   ```
4. Execute:
   ```bash
   biblioteca.exe
   ```

### Com Code::Blocks

1. Crie um novo projeto e cole o código de `Trabalho.c`.
2. Certifique-se de que `livrosED.txt` está na mesma pasta que o `.exe`.
3. Compile e execute pelo próprio Code::Blocks.

## Observações

Este projeto pode ser expandido para controle de acervos, locadoras e inventários com acesso rápido e ordenado.

## Autor

**Igor Resende Brito**
