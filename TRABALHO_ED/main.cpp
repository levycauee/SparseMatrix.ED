/*
    Curso: Ciência da Computação;
    Turma: SEG/TER 8:00 ás 10:00;
    Alunos: Mylena Vieira Araújo - 571791;
            Levy Cauê Rodrigues Nascimento - 567546;
*/

#include <iostream> //entrada/saida
#include <fstream> //ler o arquivo
#include <vector> //usar vector
#include "SparseMatrix.h" //inclusão do tad e da classe

using namespace std;


// Função ReadSparseMatrix
// Esta função recebe uma matriz esparsa vazia (referência) e o nome de um arquivo de entrada,
// e preenche a matriz com os elementos diferentes de zero a partir do arquivo.
// O arquivo deve ter a seguinte estrutura:
// - A primeira linha contém dois números inteiros m e n, que representam as dimensões da matriz (número de linhas e colunas).
// - As linhas subsequentes contêm triplas (i, j, valor), onde 'i' é o número da linha, 'j' é o número da coluna e 'valor' é o valor diferente de zero para a célula (i, j).
// O arquivo é lido e os valores são inseridos na matriz esparsa.
// Caso o arquivo não seja aberto corretamente, uma mensagem de erro será exibida.
void ReadSparseMatrix(SparseMatrix& m, string nome_arquivo){
    string caminho = nome_arquivo;  
    fstream arquivoLido(caminho); //abre e lê o arquivo

        if(!arquivoLido.is_open()){ 
            cerr << "Erro ao ler o arquivo" << endl; //se o arquivo não abrir (acho dificil voltar esse erro)
            return;
        } 

    int linhas, colunas;
    arquivoLido >> linhas >> colunas; //pega o número de linha e colunas da matriz
  
        if(linhas < 1 || colunas < 1){
            cout << "Dimensões invalidas" << endl;
            return ReadSparseMatrix(m, nome_arquivo);
        }

    m.redimensionar(linhas, colunas); //redimensiona as linhas e colunas da matriz (aloca memória suficiente)
    //explicamos esse método na própria classe, mas em resumo, dava erro de número de linhas e colunas por não ter memória
    //suficiente pras linhas e colunas que a gente tentava ler...

    double valor;
        while(arquivoLido >> linhas >> colunas >> valor){ //insere os valores na matriz em seus respectivos locais
            m.insert(linhas, colunas, valor);
        }

}


//função sum
//recebe as matrizes esparsas A e B e retorna a matriz esparsa C, que é a soma de A e B.
//a soma é feita elemento a elemento. Se um valor em A ou B for zero, ele é desconsiderado.
//a função assume que as dimensões de A e B são iguais. (verificamos na própria sobrecarga)
SparseMatrix sum(SparseMatrix& A, SparseMatrix& B){
  return A + B;
}


//função multiply
//recebe as matrizes esparsas A e B e retorna a matriz esparsa C, que é o produto de A por B.
//a função assume que o número de colunas de A é igual ao número de linhas de B. (verificamos na própria sobrecarga)
SparseMatrix multiply (SparseMatrix& A, SparseMatrix& B){
  return A * B;
}

//menu inicial
void menu() {
    cout << "Comandos disponíveis";
    cout << endl;
    cout << "help -> Exibe o menu" << endl;
    cout << "exit -> Fecha o programa" << endl;
    cout << "create m + n -> Cria uma matriz m x n" << endl;;
    cout << "show + idx -> Exibe a matriz i" << endl;
    cout << "list -> Lista os índices das matrizes" << endl;
    cout << "sum i j -> Soma as matrizes i e j"<< endl;
    cout << "multiply i j -> Multiplica as matrizes i e j" << endl;
    cout << "clear + idx -> Limpa a matriz i " << endl;
    cout << "read 'mX.txt' -> Lê uma matriz de um arquivo "<< endl;
    cout << "insert idx_matriz + linha +  coluna + valor -> insere na posicao x,y da idx_matriz" << endl;
    cout << endl;
}


int main() {
    vector<SparseMatrix> matrizes; //vector de matrizes
    string comando;
    string arquivo;

    menu();

    while (true){
    
    {
        cout << endl;
        cout << "Digite o comando desejado: ";
        cin >> comando;

        if(comando == "exit"){
            break;
        }

        else if (comando == "help"){
            menu();
        }
        
        else if(comando == "create"){
            int linhas, colunas;

            cin >> linhas >> colunas;
            if(linhas > 0 && colunas > 0){
                SparseMatrix novaMatriz(linhas, colunas);
                matrizes.push_back(novaMatriz);
                cout << "Matriz criada com sucesso. Índice: " << matrizes.size() - 1  << endl;;
            }
            else {
                cout << "Dimensões inválidas" << endl;;
            }
        }
        
        else if(comando == "show"){
            int indice;

            cin >> indice;
            if(indice >= 0 && indice < matrizes.size()){
                cout << "Matriz " << indice << endl;
                matrizes[indice].print();
            }
            else {
                cout << "Índice inválido" << endl;
            }
        }
        
        else if(comando == "list"){
            if(matrizes.empty()){
              cout << "Vector vaio" << endl;
            }
            else {
              cout << "Índices das matrizes:" << endl;
            }
                for(size_t x = 0; x < matrizes.size(); ++x){
                    cout << x << " ";
            }
            cout << endl;
        }
        
        else if(comando == "sum"){
            int idx1, idx2;

            cout << "Digite o idx das duas matrizes, exemplo: 1 2" << endl;
            cin >> idx1 >> idx2;
            if(idx1 >= 0 && idx2 < matrizes.size() && idx2 >= 0 && idx2 < matrizes.size()){
                SparseMatrix resultado = sum(matrizes[idx2], matrizes[idx2]);
                matrizes.push_back(resultado);
                cout << "Resultado da soma armazenado no índice: " << matrizes.size() - 1 << endl;
            }
            else {
                cout << "Índices inválidos" << endl;
            }
        }
        
        else if(comando == "multiply"){
            int idx1, idx2;
    
            cin >> idx1 >> idx2;
            if (idx1 >= 0 && idx1 < matrizes.size() && idx2 >= 0 && idx2 < matrizes.size()) {
                SparseMatrix resultado = multiply(matrizes[idx1], matrizes[idx2]);
                matrizes.push_back(resultado);
                cout << "Resultado da multiplicação armazenado no índice: " << matrizes.size() - 1 << endl;
            }
            else {
                cout << "Índices inválidos!" << endl;
            }
        } 
        
        else if(comando == "clear"){
            int indice;

            cout << "Digite o idx da matriz " << endl;
            cin >> indice;
            if(indice >= 0 && indice < matrizes.size()){
                matrizes[indice].clear();
                cout << "Matriz" << indice << " limpa com sucesso." << endl;
            }
            else {
                cout << "Índice inválido!" << endl;
            }
        }
        
        else if(comando == "read"){
            cin >> arquivo;
            if(arquivo.length() < 4){
                cout << "Arquivo invalido" << endl;
            }
            else {
                SparseMatrix novaMatriz;
                ReadSparseMatrix(novaMatriz, arquivo);
                matrizes.push_back(novaMatriz);
                cout << "Matriz carregada com sucesso.Índice: " << matrizes.size() - 1 << endl;
            }
        }
        
        else if(comando == "insert") {
            int pos_matriz, linha, coluna, valor;

            cin >> pos_matriz >> linha >> coluna >> valor;
            if (pos_matriz >= 0 && pos_matriz < matrizes.size()) {
                matrizes[pos_matriz].insert(linha, coluna, valor);
                cout << "Valor inserido com sucesso." <<  endl;
            }
            else {
                cout << "Índice inválido" << endl;
            }
        }

        
        else{
            cout << "Comando inválido!" << endl;
        }
    }
    }
}
