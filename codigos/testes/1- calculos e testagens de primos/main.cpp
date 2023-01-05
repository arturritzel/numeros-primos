#include <iostream>
#include <fstream>
#include <string>
//#include <string.h>
#include <locale>
#include <cmath>
#include <ctime>
#include <locale.h>

using namespace std;

///  opção de análise profunda durante o programa
/*
    utilizar essas definições fazem com que o programa funcione com velocidade extremamente reduzida.
    certifique-se de usá-la apenas para propósitos de teste
*/

//#define DEBUGGING
//<- mostra porque X número não é primo durante geração

//#define DEBUGGING2
//<- logs de leitura

#define MAXCHAR 200

enum biblioErro{

    ERR_SUCESSO = -10, // sucesso
    ERR_OPEN, // erro de abertura do arquivo
    ERR_INPUT, // erro de entrada do usuario

};

/*
Modos de abertura Descrição
ios::in Abre para leitura (default de ifstream).
ios::out Abre para gravação (default de ofstream),
ios::ate Abre e posiciona no final do arquivo.
(Este modo trabalha com leitura e gravação)
ios::app Grava a partir do fim do arquivo
ios::trunc Abre e apaga todo o conteúdo do arquivo
ios::nocreate Erro de abertura se o arquivo não existe
ios::noreplace Erro de abertura se o arquivo existir
ios::binary Abre em binário (default é texto)
*/

int qtdObjetos(char arquivo_leitura[], int qtdItensPorObjeto){

    ifstream inFile; // cria variavel para o arquivo para leitura de dados
    inFile.open(arquivo_leitura, ios::in); // abre o arquivo em modo de leitura

    if(!inFile){
        cerr << "erro ao abrir o arquivo " << arquivo_leitura << " (Possível erro na iniciação. Verifique se o arquivo existe no diretório correto.)" << endl;
        return ERR_OPEN;
    }

    char linha[MAXCHAR];
    int linhas = 0;
    while (inFile.getline(linha,MAXCHAR)) linhas++; // conta a quantidade de linhas no arquivo

    return (int)(ceil(linhas/qtdItensPorObjeto));
}

int leitura(char arquivo_leitura[], int *primos){

    ifstream inFile; // cria variavel para o arquivo para leitura de dados
    inFile.open(arquivo_leitura, ios::in); // abre o arquivo em modo de leitura

    if(!inFile){ // testa se o arquivo foi aberto corretamente
        cerr << "erro ao abrir o arquivo " << arquivo_leitura << endl;
        return ERR_OPEN;
    }

    char linha[MAXCHAR];
    int contagem = 0;

    while(inFile.getline(linha,MAXCHAR)){
        primos[contagem] = atoi(linha);
        contagem++;


        #ifdef DEBUGGING2
        cout << "lendo linha " << contagem << "; conteúdo: '" << linha << "'" << endl;
        #endif // DEBUGGING2
    }

    return ERR_SUCESSO;
}

int grava(char arquivo_gravacao[], int numero){

    ofstream outFile; // cria variavel para o arquivo para leitura de dados
    outFile.open(arquivo_gravacao, ios::out|ios::app); // abre o arquivo em modo de leitura

    if(!outFile){ // testa se o arquivo foi aberto corretamente
        cerr << "erro ao abrir o arquivo " << arquivo_gravacao << endl;
        return ERR_OPEN;
    }

    outFile << numero << endl;

    return ERR_SUCESSO;
}

bool testaPrimo(int *primos, int qtdPrimos, int testar){
    bool primo = true;
    for(int i = 0; i < qtdPrimos; i++){
        if(testar % primos[i] == 0){

            #ifdef DEBUGGING
            cout << testar << " divisivel por " << primos[i] << endl;
            primo = false;
            #endif // DEBUGGING

            #ifndef DEBUGGING
            return false;
            #endif // DEBUGGING
        }
    }

    return primo;
}

int main(){
    setlocale(LC_ALL, "Portuguese");
    system("color e0");

    char arquivo[] = "primos.txt";

    int temp = -1;
    while(temp != 9){

        /// leitura dos primos no arquivo
        int qtdPrimos = qtdObjetos(arquivo, 1);
        if(qtdPrimos == ERR_OPEN)
            return ERR_OPEN;

        int primos[qtdPrimos];
        if(leitura(arquivo, primos) == ERR_OPEN)
            return ERR_OPEN;
        /// /////////////////////

        cout << endl
             << "Digite 0 para listar TODOS os " << qtdPrimos << " números primos registrados, 1 para testar um número primo" << endl
             << "2 para adicionar números primos à base de dados, 3 para ver estatísticas" << endl;

        cin >> temp; cout << endl;

        if(temp == 0){

            cout << "Primos registrados: " << endl;
            for(int i = 0; i < qtdPrimos; i++)
                cout << primos[i] << "     ";
            cout << "------------------- " << endl;
        }

        if(temp == 1){
            int testar;
            cout << "digite qual numero primo deve ser testado: "; cin >> testar;

            if(testar > primos[qtdPrimos-1]){
                cout << "erro: o número a ser testado é maior do que a base de numeros primos registrada." << endl
                     << "o maior número registrado é: " << primos[qtdPrimos-1] << endl
                     << "o número testado seria: " << testar << endl
                     << "registre mais primos antes de testar números altos." << endl;

                return ERR_INPUT;
            }

            else if(testaPrimo(primos, qtdPrimos, testar))
                cout << testar << " é um número primo." << endl;

            else
                cout << testar << " não é um número primo." << endl;

            cout << endl;

        }

        if(temp == 2){
            int adicionar, adicionados = 0;
            cout << "Digite a quantidade de números primos a serem adicionados (altas quantidades podem demorar para serem concluidas): ";
            cin >> adicionar;

            if(adicionar < 1){
                cout << "a quantidade testada não pode ser menor que 1." << endl;
                return ERR_INPUT;
            }

            int testa = primos[qtdPrimos-1]+1; // numero inicial para testar (nao queremos testar numeros ja testados

            while(adicionar > adicionados){

                if(testaPrimo(primos, qtdPrimos, testa)){ // caso seja primo
                    if(grava(arquivo, testa) == ERR_OPEN){
                        cout << "erro abrindo o arquivo " << arquivo << endl;
                        return ERR_OPEN;
                    }
                    adicionados++;
                    cout << "numero primo adicionado: " << testa << "(" << adicionados << " de " << adicionar << " adicionados)" << endl;
                }

                testa++;
            }
        }


    }
}
