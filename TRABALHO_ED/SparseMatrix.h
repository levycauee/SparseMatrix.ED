#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <stdexcept> //exceções
#include <iostream>  //entrada/saida

//REPRESENTA UM NÓ DENTRO DA NOSSA MATRIX
struct Node {
  Node *direita;  //ponteiro para o próximo nó à direita, ou seja, avança para a próxima coluna.
  Node *abaixo;   //ponteiro para o próximo nó abaixo, ou seja, vai para a próxima linha.
  int linha;      //indice da linha do nó
  int coluna;     //indice da coluna do nó.
  double valor;   //valor armazenado no nó.

};

//classe que representa uma matriz esparsa.
class SparseMatrix {
  
private:
  int LINHAS_M; //numero de linhas da matriz.
  int COLUNAS_M; //numero de colunas da matriz.

public:
  Node* m_head; //ponteiro para o primeiro nó da matriz.

  //construtor padrão.
  SparseMatrix() {
    this->m_head = nullptr; // inicializa o primeiro nó da lista como nullptr
    this->LINHAS_M = 0;     // inicializa o número de linhas como 0
    this->COLUNAS_M = 0;    // inicializa o número de colunas como 0

  }

  // Construtor de cópia: cria uma nova matriz copiando outra já existente
  SparseMatrix(const SparseMatrix& matriz) {
    this->LINHAS_M = matriz.LINHAS_M; // copia a quantidade de linhas da matriz passada

    this->COLUNAS_M = matriz.COLUNAS_M; //copia a quantidade de colunas da matriz passada

    this->m_head = nullptr; //começa com a matriz vazia, sem nós

    //agora, percorremos a matriz original para copiar os nós
    Node* atual = matriz.m_head; //começa pelo primeiro nó da matriz original
    while (atual != nullptr) { //enquanto houver nós para copiar...
        this->insert(atual->linha, atual->coluna, atual->valor); //insere o nó na nova matriz que será a copia da que a gente passou, é claro
        atual = atual->abaixo; //vai para o próximo nó na lista (depois volta pra condição do while)
    }
}


  //construtor padrao com parametros
  SparseMatrix (int linhas, int colunas) {
    // Inicializa a matriz esparsa com o número de linhas e colunas especificado.
    this->m_head = nullptr;
    this->LINHAS_M = linhas;
    this->COLUNAS_M = colunas;
  }

  
  // ∼SparseMatrix();
  // Destrutor. Libera toda a memória que foi alocada dinamicamente para a matriz
  ~SparseMatrix() {
    while (m_head != nullptr) { //enquanto houver linhas na matriz
        Node* linhaAtual = m_head; //começa pela primeira linha
        m_head = m_head->abaixo; //move a cabeça para a próxima linha

        //percorre e libera todos os nós da linha atual
        while (linhaAtual != nullptr) {
            Node* temp = linhaAtual;
            linhaAtual = linhaAtual->direita; //move para o próximo nó na linha
            delete temp; //libera a memória do nó atual
        }
    }
}



// void insert(int i, int j, double value);
// Esta função-membro faz o valor na célula (i, j) da matriz ser igual a 'value',
// onde 'i' é a linha e 'j' é a coluna. Se já houver um valor nessa posição,
// ele é atualizado para esse novo valor. A função deve checar se os índices 'i' e 'j'
// passados por parâmetro são válidos; se não forem, uma exceção deve ser lançada.
// Atenção: No caso em que o valor do argumento 'value' for igual a zero,
// a função deve desconsiderar e não fazer nada neste caso. Ou seja,
// chamadas dessa função passando o valor 0 não terão efeito algum.
  void insert(int i, int j, double value) {

    //ignorar valores iguais a 0.
    if (value == 0) {
      return;
    }

    //verificação dos índices
    if (i < 1 || j < 1 || i > this->LINHAS_M || j > this->COLUNAS_M) {
      throw std::out_of_range("Índices fora do intervalo válido!");
    }


    //inserir ou atualizar o valor na matriz esparsa
    Node* atual = m_head;
    Node* anterior = nullptr;

    //percorre a lista para encontrar a posição correta para inserir o novo nó
    while (atual != nullptr && (atual->linha < i || (atual->linha == i && atual->coluna < j))) {
      anterior = atual;
      atual = atual->abaixo;
    }

    //se o nó já existe, atualiza o valor
    if (atual != nullptr && atual->linha == i && atual->coluna == j) {
      atual->valor = value;
    } else {
      //caso contrário, insere um novo nó
      Node* novoNo = new Node();
      novoNo->linha = i;
      novoNo->coluna = j;
      novoNo->valor = value;
      novoNo->abaixo = atual;

      //ajusta os ponteiros para manter a lista ordenada
      if (anterior == nullptr) {
        m_head = novoNo;
      } else {
        anterior->abaixo = novoNo;
      }
    }
  }


  // double get(int i, int j);
  // Devolve o valor na célula (i, j) da matriz, onde 'i' é a linha e 'j' é a coluna.
  // A função deve checar se os índices passados são válidos; se não forem válidos,
  // uma exceção deve ser lançada.
  double get(int i, int j) {
    //alidação dos índices
    if (i < 1 || j < 1 || i > this->LINHAS_M || j > this->COLUNAS_M) {
      throw std::out_of_range("Índices fora do intervalo válido!");
    }

    // Buscar o valor na matriz esparsa.
    Node* atual = m_head;

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

  // void print();
  // Esta função imprime a matriz A no terminal, inclusive os elementos iguais a zero.
  void print() {
    //imprime todos os elementos, inclusive zeros.
    for (int i = 1; i <= this->LINHAS_M; i++) {
      for (int j = 1; j <= this->COLUNAS_M; j++) {
        std::cout << get(i, j) << " ";
      }
      std::cout << std::endl;
    }
  }

  //método que remove todos os elementos da matriz
  void clear() {
    //libera a memória alocada para os nós da matriz
    //também libera os nós, consequentemente apaga os valores
    //isso a gente vai usar muito eu acho
    Node* atual = m_head;
    while (atual != nullptr) {
      Node* temp = atual;
      atual = atual->abaixo;
      delete temp;
    }
    m_head = nullptr; //define a cabeça da lista como nula
  }

  //método que redimensiona a matriz esparsa
  //usamos para garantir que a matriz que a gente redimensionar
  //tenha memoria suficiente para as linhas e colunas que
  //o usuario colocar em uma matrizX
  //(Usamos apenas na função de leitura) por que
  //sempre retornava a exceção de linhas e colunas
  void redimensionar(int linhas, int colunas) {
    //libera a memória alocada para os nós da matriz
    //copiei e colei o clear praticamente
    Node* atual = m_head;
    while (atual != nullptr) {
      Node* temp = atual;
      atual = atual->abaixo;
      delete temp;
    }

    //seta novamente a cabeça da lista e as dimensões da matriz como padrão
    //ou seja, só está liberando memória de acordo com a entrada do usuário
    this->m_head = nullptr;
    this->LINHAS_M = linhas;
    this->COLUNAS_M = colunas;
  }

  

  // Sobrecarga do operador de multiplicação *
  SparseMatrix operator*(SparseMatrix& matriz) {
    //verifica se as dimensões são compatíveis para multiplicação
    if (this->COLUNAS_M != matriz.LINHAS_M) {
      throw std::out_of_range("Não podem ser somadas por conta das dimensões fora do padrão, as colunas de A tem que ser iguais as linhas de B");
    }

    //cria uma nova matriz para armazenar o resultado
    SparseMatrix novaMatriz(this->LINHAS_M, matriz.COLUNAS_M);

    //variável auxiliar para armazenar o valor acumulado de cada elemento da matriz resultado
    int valor;

    //calcula o produto das matrizes
    for (int i = 1; i <= this->LINHAS_M; i++) {
      for (int j = 1; j <= matriz.COLUNAS_M; j++) {
        valor = 0.0;

        //calcula o elemento resultante
        for (int x = 1; x <= this->COLUNAS_M; x++) {
          valor = valor + (this->get(i, x) * matriz.get(x, j));
        }

        //insere o valor na novaMatriz se for diferente de zero
        if (valor != 0.0) {
          novaMatriz.insert(i, j, valor);
        }
      }
    }

    //retorna a novaMatriz
    return novaMatriz;
  }

  //sobrecarga do operador de adição +
  SparseMatrix operator+(SparseMatrix& matriz) {
    //verifica se a conta pode ser feita ou não
    if (this->LINHAS_M != matriz.LINHAS_M || this->COLUNAS_M != matriz.COLUNAS_M) {
      throw std::out_of_range("Não podem ser somadas por conta das dimensões fora do padrão.");
    }

    //cria uma nova matriz C para armazenar o resultado.
    SparseMatrix novaMatriz(this->LINHAS_M, this->COLUNAS_M);

    //calcula a soma das matrizes
    for (int i = 1; i <= this->LINHAS_M; i++) {
      for (int j = 1; j <= this->COLUNAS_M; j++) {
        novaMatriz.insert(i, j, this->get(i, j) + matriz.get(i, j));
      }
    }

    //retorna a novaMatriz 
    return novaMatriz;
  }

};

#endif
