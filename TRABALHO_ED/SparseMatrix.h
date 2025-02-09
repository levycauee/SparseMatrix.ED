#ifndef MATRIX_H
#define MATRIX_H
// Diretivas de pré-processador para evitar múltiplas inclusões do mesmo arquivo.

#include <stdexcept>
#include <iostream>
// Inclui bibliotecas para exceções e entrada/saída.


// Estrutura que representa um nó da matriz esparsa.
struct Node {
  Node *direita;  // Ponteiro para o próximo nó à direita.
  Node *abaixo;   // Ponteiro para o próximo nó abaixo.
  int linha;      // Índice da linha do nó.
  int coluna;     // Índice da coluna do nó.
  double valor;   // Valor armazenado no nó.

  Node() = default; // Construtor padrão do nó.
};

// Classe que representa uma matriz esparsa.
class SparseMatrix {
  // Node é uma struct que representa um nó da matriz esparsa.
private:
  int n; // Número de colunas da matriz.
  int m; // Número de linhas da matriz.

public:
  Node *head; // Ponteiro para o primeiro nó da matriz.

  // Construtor padrão.
  SparseMatrix() {
    this->head = nullptr; // Inicializa a cabeça da lista como nula.
    this->n = 0;          // Inicializa o número de colunas como 0.
    this->m = 0;          // Inicializa o número de linhas como 0.
  }

  // Construtor de cópia.
  SparseMatrix(const SparseMatrix& matrix) {
    this->m = matrix.m; // Copia o número de linhas.
    this->n = matrix.n; // Copia o número de colunas.
    this->head = nullptr; // Inicializa a cabeça da lista como nula.

    // Copia os nós da matriz original para a nova matriz.
    Node* atual = matrix.head;
    while (atual != nullptr) {
      this->insert(atual->linha, atual->coluna, atual->valor);
      atual = atual->abaixo;
    }
  }

  // Construtor com parâmetros.
  SparseMatrix(int linhas, int colunas) {
    // Inicializa a matriz esparsa com o número de linhas e colunas especificado.
    this->head = nullptr;
    this->m = linhas;
    this->n = colunas;
  }

  // Destrutor padrão.
  ~SparseMatrix() {
    // Libera a memória alocada para os nós da matriz.
    Node* atual = head;
    while (atual != nullptr) {
      Node* temp = atual;
      atual = atual->abaixo;
      delete temp;
    }
    head = nullptr; // Define a cabeça da lista como nula.
  }

  // Método que insere um valor na matriz esparsa.
  void insert(int i, int j, double value) {
    // Validação dos índices.
    if (i < 1 || j < 1 || i > this->n || j > this->m) {
      throw std::out_of_range("Índices fora do intervalo válido!");
    }

    // Ignorar valores iguais a 0.
    if (value == 0) {
      return;
    }

    // Inserir ou atualizar o valor na matriz esparsa.
    Node* atual = head;
    Node* anterior = nullptr;

    // Percorre a lista para encontrar a posição correta para inserir o novo nó.
    while (atual != nullptr && (atual->linha < i || (atual->linha == i && atual->coluna < j))) {
      anterior = atual;
      atual = atual->abaixo;
    }

    // Se o nó já existe, atualiza o valor.
    if (atual != nullptr && atual->linha == i && atual->coluna == j) {
      atual->valor = value;
    } else {
      // Caso contrário, insere um novo nó.
      Node* novoNo = new Node();
      novoNo->linha = i;
      novoNo->coluna = j;
      novoNo->valor = value;
      novoNo->abaixo = atual;

      // Ajusta os ponteiros para manter a lista ordenada.
      if (anterior == nullptr) {
        head = novoNo;
      } else {
        anterior->abaixo = novoNo;
      }
    }
  }

  // Método que retorna o elemento na posição (i, j).
  double get(int i, int j) {
    // Validação dos índices.
    if (i < 1 || j < 1 || i > this->n || j > this->m) {
      throw std::out_of_range("Índices fora do intervalo válido!");
    }

    // Buscar o valor na matriz esparsa.
    Node* atual = head;

    // Percorre a lista para encontrar o nó com os índices (i, j).
    while (atual != nullptr) {
      if (atual->linha == i && atual->coluna == j) {
        return atual->valor; // Retorna o valor se o nó for encontrado.
      }
      // Se passar da linha ou coluna desejada, pode parar (lista ordenada).
      if (atual->linha > i || (atual->linha == i && atual->coluna > j)) {
        break;
      }
      atual = atual->abaixo;
    }

    // Se o valor não foi encontrado, retorna 0.
    return 0;
  }

  // Método que imprime a matriz esparsa.
  void print() {
    // Imprime todos os elementos, inclusive zeros.
    for (int i = 1; i <= this->m; i++) {
      for (int j = 1; j <= this->n; j++) {
        std::cout << get(i, j) << " ";
      }
      std::cout << std::endl;
    }
  }

  // Método que redimensiona a matriz esparsa.
  void resize(int linhas, int colunas) {
    // Libera a memória alocada para os nós da matriz.
    Node* atual = head;
    while (atual != nullptr) {
      Node* temp = atual;
      atual = atual->abaixo;
      delete temp;
    }

    // Redefine a cabeça da lista e as dimensões da matriz.
    this->head = nullptr;
    this->m = linhas;
    this->n = colunas;
  }

  // Método que remove todos os elementos da matriz.
  void clear() {
    // Libera a memória alocada para os nós da matriz.
    Node* atual = head;
    while (atual != nullptr) {
      Node* temp = atual;
      atual = atual->abaixo;
      delete temp;
    }
    head = nullptr; // Define a cabeça da lista como nula.
  }

  // Sobrecarga do operador de multiplicação.
  SparseMatrix operator*(SparseMatrix& matrix) {
    // Verifica se as dimensões são compatíveis para multiplicação.
    if (this->n != matrix.m) {
      throw std::out_of_range("Não foi possível multiplicar: dimensões incompatíveis!");
    }

    // Cria uma nova matriz para armazenar o resultado.
    SparseMatrix result(this->m, matrix.n);

    // Variável auxiliar para armazenar o valor acumulado de cada elemento da matriz resultado.
    double a;

    // Calcula o produto das matrizes.
    for (int i = 1; i <= this->m; i++) {
      for (int j = 1; j <= matrix.n; j++) {
        a = 0.0;

        // Calcula o elemento resultante.
        for (int k = 1; k <= this->n; k++) {
          a += this->get(i, k) * matrix.get(k, j);
        }

        // Insere o valor na matriz resultado se for diferente de zero.
        if (a != 0.0) {
          result.insert(i, j, a);
        }
      }
    }

    // Retorna a matriz resultado.
    return result;
  }

  // Sobrecarga do operador de adição.
  SparseMatrix operator+(SparseMatrix& matrix) {
    // Verifica se as dimensões são compatíveis para adição.
    if (this->m != matrix.m || this->n != matrix.n) {
      throw std::out_of_range("Não foi possível somar: dimensões incompatíveis!");
    }

    // Cria uma nova matriz para armazenar o resultado.
    SparseMatrix result(this->m, this->n);

    // Calcula a soma das matrizes.
    for (int i = 1; i <= this->m; i++) {
      for (int j = 1; j <= this->n; j++) {
        result.insert(i, j, this->get(i, j) + matrix.get(i, j));
      }
    }

    // Retorna a matriz resultado.
    return result;
  }

  // Sobrecarga do operador de atribuição.
  SparseMatrix operator=(const SparseMatrix& matrix) {
    // Verifica se não é uma auto-atribuição.
    if (this != &matrix) {
      // Limpa a memória do objeto atual.
      Node* atual = head;
      while (atual != nullptr) {
        Node* temp = atual;
        atual = atual->abaixo;
        delete temp;
      }

      // Copia os atributos básicos.
      this->m = matrix.m;
      this->n = matrix.n;
      this->head = nullptr;

      // Copia os nós da matriz esparsa.
      Node* outroAtual = matrix.head;
      while (outroAtual != nullptr) {
        this->insert(outroAtual->linha, outroAtual->coluna, outroAtual->valor);
        outroAtual = outroAtual->abaixo;
      }
    }

    // Retorna o objeto atual.
    return *this;
  }
};

#endif
