#include <iostream> // Biblioteca para entrada e saída de dados
#include <fstream>  // Biblioteca para manipulação de arquivos
#include <vector>   // Biblioteca para uso do container vector
#include <string>   // Biblioteca para manipulação de strings
#include "SparseMatrix.h" // Inclusão do cabeçalho da classe SparseMatrix

using namespace std; // Evita a necessidade de usar std:: antes dos comandos

// Função para ler uma matriz esparsa a partir de um arquivo
void ReadSparseMatrix(SparseMatrix& m, string nome_arquivo){
    string caminho = nome_arquivo;  // Define o caminho do arquivo

    fstream arquivoLido(caminho); // Abre o arquivo para leitura

    if(!arquivoLido.is_open()){ // Verifica se o arquivo foi aberto corretamente
        cerr << "Erro ao ler o arquivo\n" << endl; // Exibe mensagem de erro
        return;
    }

    int linhas; 
    int colunas;
    arquivoLido >> linhas >> colunas; // Lê o número de linhas e colunas da matriz
  
    if(linhas <= 0 || colunas <= 0){ // Verifica se as dimensões são válidas
        cout << "Dimensões inválidas no arquivo\n";
        return ReadSparseMatrix(m, nome_arquivo); // Tenta ler novamente
    }

    m.resize(linhas, colunas); // Redimensiona a matriz

    int lin, col;
    double valor;
    while(arquivoLido >> lin >> col >> valor){ // Lê os valores do arquivo e insere na matriz
        m.insert(lin, col, valor);
    }

    arquivoLido.close(); // Fecha o arquivo
}

// Função para somar duas matrizes esparsas
SparseMatrix sum(SparseMatrix& A, SparseMatrix& B){
    return A + B; // Retorna o resultado da soma
}

// Função para multiplicar duas matrizes esparsas
SparseMatrix multiply (SparseMatrix& A, SparseMatrix& B){
    return A * B; // Retorna o resultado da multiplicação
}

// Exibe o menu de comandos disponíveis
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

// Função principal do programa
int main() {
    vector<SparseMatrix> matrizes; // Vetor para armazenar as matrizes criadas
    string comando; // Variável para armazenar o comando digitado pelo usuário
    string arquivo; // Variável para armazenar o nome do arquivo
    int m, n, i, j; // Variáveis auxiliares
    double value; // Variável para armazenar valores da matriz

    menu(); // Exibe o menu de comandos

    while (true){ // Loop infinito para processar comandos
        cout << endl;
        cout << "Digite o comando desejado: ";
        cin >> comando; // Lê o comando digitado pelo usuário

        if(comando == "exit"){ // Encerra o programa
            break;
        }
        else if(comando == "create"){ // Cria uma nova matriz
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
        else if(comando == "show"){ // Exibe uma matriz
            cin >> i;
            if(i >= 0 && i < matrizes.size()){
                cout << "\nMatriz " << i << ":\n";
                matrizes[i].print();
            }
            else {
                cout << "\nÍndice inválido!\n";
            }
        }
        else if(comando == "list"){ // Lista as matrizes criadas
            if(matrizes.empty()){
                cout << "\nO vetor de matrizes está vazio.";
            }
            else {
                cout << "\nÍndices das matrizes:\n";
            }
            for(size_t x = 0; x < matrizes.size(); ++x){
                cout << x << " ";
            }
            cout << "\n";
        }
        else if(comando == "sum"){ // Soma duas matrizes
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
        else if(comando == "mult"){ // Multiplica duas matrizes
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
        else {
            cout << "\nComando inválido!\n";
        }
    }
}
