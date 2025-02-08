#ifndef MATRIX_H
#define MATRIX_H
#include <stdexcept>
#include <iostream>

/*
  Classe que representa uma matriz esparsa.
  aqui temos alguns métodos (TAD) que manipulam uma matriz esparsa.
 */

struct Node {
  Node *direita;
  Node *abaixo;
  int linha;
  int coluna;
  double valor;

  Node() = default;
};

class SparseMatrix{
  
  // Node é uma struct que representa um nó da matriz esparsa.
  private:
  int n;
  int m;

  public:

  Node *head;
  
  // Construtor padrão
  SparseMatrix(){
    this->head = nullptr;
    this->n = 0;
    this->m = 0;
  }

  // Construtor de cópia
  SparseMatrix(const SparseMatrix& matrix){
    this->m = matrix.m;
    this->n = matrix.n;
    this->head = nullptr;
  
    Node* atual = matrix.head;
    while(atual != nullptr){
      this->insert(atual->linha, atual->coluna, atual->valor);
      atual = atual->abaixo;
    }
  }

  // Construtor com parâmetros
  SparseMatrix(int linhas, int colunas){
    // Inicializa a matriz esparsa
    this->head = nullptr;
    this->m = linhas;
    this->n = colunas;
  }

  // Destructor padrão
  ~SparseMatrix() {
    Node* atual = head;
    while (atual != nullptr) {
        Node* temp = atual;
        atual = atual->abaixo;
        delete temp; 
    }
    head = nullptr;
}
  
  // Método que insere um valor na matriz esparsa
  void insert(int i, int j, double value){

    // Validação dos índices
    if(i < 1 || j < 1 || i > this->n || j > this->m){
      throw std::out_of_range("Índices fora do intervalo válido!");
    }
  
    // Ignorar valores iguais a 0
    if(value == 0){
      return;
    }
  
    // Inserir ou atualizar o valor na matriz esparsa
    Node* atual = head;
    Node* anterior = nullptr;
  
    while (atual != nullptr && (atual->linha < i || (atual->linha == i && atual->coluna < j))) {
          anterior = atual;
          atual = atual->abaixo;
    }
  
    if(atual != nullptr && atual->linha == i && atual->coluna == j){
      // Atualizar o valor existente
      atual->valor = value;
    } else {
  
      // Insere um novo nó
      Node* novoNo = new Node();
      novoNo->linha = i;
      novoNo->coluna = j;
      novoNo->valor = value;
      novoNo->abaixo = atual;
  
      if(anterior == nullptr){
        head = novoNo;
      } else {
        anterior->abaixo = novoNo;
      }
    }  
  }

  // Método que vai pegar elemento dada posição (I,J);
  double get(int i, int j){

    // Validação dos índices
   if(i < 1 || j < 1 || i > this->n || j > this->m){
     throw std::out_of_range("Índices fora do intervalo válido!");
   }
 
     // Buscar o valor na matriz esparsa
     Node* atual = head;
 
     // Percorrer a matriz esparsa
     while (atual != nullptr) {
         // Se encontrar o nó com os índices (i, j), retornar o valor
         if (atual->linha == i && atual->coluna == j) {
             return atual->valor;
         }
         // Se passar da linha ou coluna desejada, pode parar (lista ordenada)
         if (atual->linha > i || (atual->linha == i && atual->coluna > j)) {
             break;
         }
         atual = atual->abaixo;
     }
 
     // Se o valor não foi encontrado, retornar 0
     return 0;
   }

  // Método que mostra a matriz esparsa
  void print(){

    // Imprimir todos os elementos, inclusive zeros
    for(int i = 1; i <= this->m; i++){
      for(int j = 1; j <= this->n; j++){
        std::cout << get(i, j) << " ";
      }
      std::cout << std::endl;
    }
  }


  // Método que libera a memória e altera as dimensões da matriz esparsa
  void resize(int linhas, int colunas){
    Node* atual = head;
    while (atual != nullptr) {
      Node* temp = atual;
      atual = atual->abaixo;
      delete temp;
    }

    this->head = nullptr;
    this->m = linhas;
    this->n = colunas;
  }

  // remove elementos das matrizes
  void clear() {
    Node* atual = head;
    while (atual != nullptr) {
      Node* temp = atual;
      atual = atual->abaixo;
      delete temp;
    }
    head = nullptr;
  }

  // Sobrecarga de operador de multiplicação
  SparseMatrix operator*(SparseMatrix& matrix) {
    if(this->n != matrix.m){
      throw std::out_of_range("Não foi possivel somar você precisa inserir uma coordenadas dentro dos limites!");
    }

     SparseMatrix result(this->m, matrix.n);

     // Declara uma variável auxiliar a para armazenar o valor acumulado de cada elemento
     // da matriz resultado
     double a;

    for(int i = 1; i <= this->m; i++){ 
      for(int j = 1; j <= matrix.n; j++){ 
        a = 0.0; 

        // Calculo do elemento resultante
        for(int k = 1; k <= this->n; k++){ // Itera sobre os indices intermediários para calcular o produto escalar
          a += this->get(i, k) * matrix.get(k, j);
        }
          
        if(a != 0.0){
          result.insert(i, j, a);
        }
      }
    }

    // Retorno
    return result;
  }

  SparseMatrix operator+(SparseMatrix& matrix) {

    if(this->m != matrix.m || this->n != matrix.n){
      throw std::out_of_range("Não foi possivel somar você precisa inserir uma coordenadas dentro dos limites!");
    }

    SparseMatrix result(this->m, this->n);
    
    for(int i = 1; i <= this->m; i++){
      for(int j = 1; j <= this->n; j++){
        result.insert(i, j, this->get(i, j) + matrix.get(i, j));
      }
    }

    return result;

  }

  // Sobrecarga do operador de atribuição
  SparseMatrix operator=(const SparseMatrix& matrix){
    if(this != &matrix){
      
      // Limpar memória do objeto atual.
      Node* atual = head;
      while(atual != nullptr){
        Node* temp = atual;
        atual = atual->abaixo;
        delete temp;
      }

      // Copiar os atributos básicos
      this->m = matrix.m;
      this->n = matrix.n;
      this->head = nullptr;

      // Copiando os nós da matriz esparsa
      Node* outroAtual = matrix.head;
      while(outroAtual != nullptr){
        this->insert(outroAtual->linha, outroAtual->coluna, outroAtual->valor);
        outroAtual = outroAtual->abaixo;
      }
    }

    return *this;
  }

};

#endif
