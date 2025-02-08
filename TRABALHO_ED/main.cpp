#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "SparseMatrix.h"

using namespace std;

void ReadSparseMatrix(SparseMatrix& m, string nome_arquivo){
    string caminho = nome_arquivo;  

    fstream arquivoLido(caminho); // abre e lê o arquivo

  if(!arquivoLido.is_open()){ // caso não seja  possível abrir o arquivo
    cerr << "Erro ao ler o arquivo\n" << endl; // saída
    return;
  }

  int linhas; 
  int colunas;
  arquivoLido >> linhas >> colunas; // pega o número de linha e colunas da matriz;
  
  if(linhas <= 0 || colunas <= 0){
    cout << "Dimensões inválidas no arquivo\n";
    return ReadSparseMatrix(m, nome_arquivo);
  }

  m.resize(linhas, colunas); // dá um resize nas linhas e colunas

  int lin, col;
  double valor;
  while(arquivoLido >> lin >> col >> valor){ // insere os valores na matriz em seus respectivos locais
    m.insert(lin, col, valor);
  }

  arquivoLido.close(); // fecha o arquivoLido

}

SparseMatrix sum(SparseMatrix& A, SparseMatrix& B){
  return A + B;
}

SparseMatrix multiply (SparseMatrix& A, SparseMatrix& B){
  return A * B;
}

void menu() {
    cout << "Comandos disponíveis";
    cout << "exit -> Fecha o programa\n";
    cout << "create m + n -> Cria uma matriz m x n\n";
    cout << "show + idx -> Exibe a matriz i\n";
    cout << "list -> Lista os índices das matrizes\n";
    cout << "sum i j -> Soma as matrizes i e j\n";
    cout << "mult i j -> Multiplica as matrizes i e j\n";
    cout << "clear + idx -> Limpa a matriz i\n";
    cout << "read 'mX.txt' -> Lê uma matriz de um arquivo\n";
    cout << "insert idx_matriz + linha +  coluna + valor -> Altera a célula (linha, coluna) da matriz m\n";
    cout << "clear all -> Remove todas as matrizes\n";
    cout << "===========================\n";
}


int main() {
    vector<SparseMatrix> matrizes;
    string comando;
    string arquivo;
    int m, n, i, j;
    double value;

    menu();

    while (true){
    
    {
        cout << endl;
        cout << "Digite o comando desejado: ";
        cin >> comando;

        if(comando == "exit"){
            break;
        }
        
        else if(comando == "create"){
            cin >> m >> n;
            if(m > 0 && n > 0){
                SparseMatrix novaMatriz(m, n);
                matrizes.push_back(novaMatriz);
                cout << "\nMatriz criada com sucesso. Índice: " << matrizes.size() - 1 << "\n";
            }
            else {
                cout << "\nDimensões inválidas!\n";
            }
        }
        
        else if(comando == "show"){
            cin >> i;
            if(i >= 0 && i < matrizes.size()){
                cout << "\nMatriz " << i << ":\n";
                matrizes[i].print();
            }
            else {
                cout << "\nÍndice inválido!\n";
            }
        }
        
        else if(comando == "list"){
            if(matrizes.empty()){
              cout << "\n O vetor de matrizes esta vazio.";
            }
            else {
              cout << "\nÍndices das matrizes:\n";
            }
                for(size_t x = 0; x < matrizes.size(); ++x){
                    cout << x << " ";
            }
            cout << "\n";
        }
        
        else if(comando == "sum"){
            cout << "Digite o idx das duas matrizes, exemplo: 1 2" << endl;
            cin >> i >> j;
            if(i >= 0 && i < matrizes.size() && j >= 0 && j < matrizes.size()){
                SparseMatrix resultado = sum(matrizes[i], matrizes[j]);
                matrizes.push_back(resultado);
                cout << "\nResultado da soma armazenado no índice: " << matrizes.size() - 1 << "\n";
            }
            else {
                cout << "\nÍndices inválidos!\n";
            }
        }
        
        else if(comando == "mult"){
            cout << "Digite o idx das duas matrizes, exemplo: 3 4" << endl;
            cin >> i >> j;
            if (i >= 0 && i < matrizes.size() && j >= 0 && j < matrizes.size()) {
                SparseMatrix resultado = multiply(matrizes[i], matrizes[j]);
                matrizes.push_back(resultado);
                cout << "\nResultado da multiplicação armazenado no índice: " << matrizes.size() - 1 << "\n";
            }
            else {
                cout << "\nÍndices inválidos!\n";
            }
        } 
        
        else if(comando == "clear"){
            cout << "Digite o idx da matriz " << endl;
            cin >> i;
            if(i >= 0 && i < matrizes.size()){
                matrizes[i].clear();
                cout << "\nMatriz " << i << " limpa com sucesso.\n";
            }
            else {
                cout << "\nÍndice inválido!\n";
            }
        }
        
        else if(comando == "read"){
            cin >> arquivo;
            if(arquivo.length() < 4 || arquivo.substr(arquivo.length() - 4) != ".txt"){
                cout << "\nO arquivo não é um .txt\n" << endl;
            }
            else {
                SparseMatrix novaMatriz;
                ReadSparseMatrix(novaMatriz, arquivo);
                matrizes.push_back(novaMatriz);
                cout << "\nMatriz carregada com sucesso. Índice: " << matrizes.size() - 1 << "\n";
            }
        }
        
        else if(comando == "insert") {
            cin >> m >> i >> j >> value;
            if (m >= 0 && m < matrizes.size()) {
                matrizes[m].insert(i, j, value);
                cout << "\nValor atualizado com sucesso.\n";
            }
            else {
                cout << "\nÍndice inválido!\n";
            }
        }
        
        else if(comando == "clear all"){
            matrizes.clear();
            cout << "\nTodas as matrizes foram apagadas.\n";
        }
        
        else{
            cout << "\nComando inválido!\n";
        }
    }
    }
}