#include <iostream>
#include <fstream>
#include <string>
//#include <string.h>
#include <locale>
#include <cmath>
#include <ctime>
#include <locale.h>
#include <limits>

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

int qtdObjetos(char arquivo_leitura[], int qtdItensPorObjeto){

    ifstream inFile; // cria variavel para o arquivo para leitura de dados
    inFile.open(arquivo_leitura, ios::in); // abre o arquivo em modo de leitura

    if(!inFile){
        cerr << "Erro ao abrir o arquivo " << arquivo_leitura << ". (Possível erro na iniciação. Verifique se o arquivo existe no diretório correto.)" << endl;
        return ERR_OPEN;
    }

    char linha[MAXCHAR];
    int linhas = 0;
    while (inFile.getline(linha,MAXCHAR)) linhas++; // conta a quantidade de linhas no arquivo
    inFile.close();

    if((int)(ceil(linhas/qtdItensPorObjeto)) == 0){
        grava(arquivo_leitura, 2);
        return 1;
    }

    return (int)(ceil(linhas/qtdItensPorObjeto));
}

int leitura(char arquivo_leitura[], int *primos){

    ifstream inFile; // cria variavel para o arquivo para leitura de dados
    inFile.open(arquivo_leitura, ios::in); // abre o arquivo em modo de leitura

    if(!inFile){ // testa se o arquivo foi aberto corretamente
        cerr << "Erro ao abrir o arquivo " << arquivo_leitura << endl;
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

bool testaPrimo(int *primos, int qtdPrimos, int testar, bool motivo){
    bool primo = true;
    if(testar == 0){
        cout << "um número não pode ser dividido por 0." << endl;
        return false;
    }
    #ifdef DEBUGGING
    motivo = true;
    #endif // DEBUGGING
    for(int i = 0; i < qtdPrimos; i++){
        if(testar % primos[i] == 0 && testar != primos[i]){

            if(motivo){
                cout << testar << " é divisivel por " << primos[i] << endl;
                primo = false;
            }
            else return false;
        }
    }

    return primo;
}

int qtdPrimosIntervalo(int inicio, int fim, int qtdPrimos, int *primos){
    if(fim > primos[qtdPrimos-1]){
        cout << "Erro ao calcular a quantidade de primos entre " << inicio << " e " << fim << ": o maior número primo registrado (" << primos[qtdPrimos-1] << ") é menor do que o último número pedido (" << fim << "). Execute mais registros ou teste outro intervalo." << endl;
        return ERR_INPUT;
    }

    int quantidade = 0;
    for(int i = 0; i < qtdPrimos; i++){
        if(primos[i] >= inicio && primos[i] <= fim) quantidade++;
        if(primos[i] >= fim) break;
    }

    cout << "Quantidade de primos entre " << inicio << " e " << fim << ": " << quantidade << endl;
    return quantidade;
}

int main(){
    setlocale(LC_ALL, "Portuguese");
    //system("color e0");
    system("color 02");

    char arquivo[] = "primos.txt";

    int temp = -1;
    while(temp != 9){

        /// leitura dos primos no arquivo
        int qtdPrimos = qtdObjetos(arquivo, 1);
        if(qtdPrimos == ERR_OPEN)
            return ERR_OPEN;

        if(qtdPrimos == 0){
            if(grava(arquivo, 2) == ERR_OPEN)
                return ERR_OPEN;

            qtdPrimos++;
        }

        int primos[qtdPrimos];
        if(leitura(arquivo, primos) == ERR_OPEN)
            return ERR_OPEN;
        /// /////////////////////
        temp = -1;

        while(temp < 1 || temp > 9){

            cout << endl
                 << "Digite 1 para listar TODOS os " << qtdPrimos << " números primos registrados, 2 para testar um número primo" << endl
                 << "Digite 3 para adicionar números primos à base de dados, 4 para ver estatísticas" << endl
                 << "Digite 8 para criar um novo arquivo para registros, 9 para encerrar o programa" << endl;

            cin >> temp;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        cout << endl;

        if(temp == 1){

            cout << "Primos registrados: " << endl;
            for(int i = 0; i < qtdPrimos; i++)
                cout << primos[i] << "     ";
            cout << endl << "------------------- " << endl;
        }

        if(temp == 2){
            int testar;

            do{
                cout << "digite qual numero primo deve ser testado: "; cin >> testar;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if(testar > primos[qtdPrimos-1]){
                    cout << "erro: o número a ser testado é maior do que a base de numeros primos registrada." << endl
                         << "o maior número registrado é: " << primos[qtdPrimos-1] << endl
                         << "o número testado seria: " << testar << endl
                         << "registre mais primos antes de testar números altos." << endl;
                }

                if(testar < 2){
                    cout << "erro: o número a ser testado é menor do que o menor número primo (2)." << endl;
                }

            } while(testar > primos[qtdPrimos-1] || testar < 2);

            if(testaPrimo(primos, qtdPrimos, testar, true))
                cout << testar << " é um número primo." << endl;

            else
                cout << testar << " não é um número primo." << endl;

            cout << endl;

        }

        if(temp == 3){
            int adicionar, adicionados = 0;

            do{
                cout << "Digite a quantidade de números primos a serem adicionados (altas quantidades podem demorar para serem concluidas): ";
                cin >> adicionar;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if(adicionar < 1){
                    cout << "A quantidade de números adicionados não pode ser menor que 1." << endl;
                }

                if(adicionar > 100000){
                    cout << "Uma muito alta de números pode demorar para ser finalizada. Evite valores muito altos." << endl;
                }

            } while(adicionar < 1 || adicionar > 100000);

            int testa = primos[qtdPrimos-1]+1; // numero inicial para testar (nao queremos testar numeros ja testados

            while(adicionar > adicionados){

                qtdPrimos = qtdObjetos(arquivo, 1);
                if(qtdPrimos == ERR_OPEN)
                    return ERR_OPEN;

                int primos[qtdPrimos];
                if(leitura(arquivo, primos) == ERR_OPEN)
                    return ERR_OPEN;

                if(testaPrimo(primos, qtdPrimos, testa, false)){ // caso seja primo
                    if(grava(arquivo, testa) == ERR_OPEN){
                        cout << "erro abrindo o arquivo " << arquivo << endl;
                        return ERR_OPEN;
                    }

                    adicionados++;
                    cout << "Número primo adicionado: " << testa << "(" << adicionados << " de " << adicionar << " adicionados)" << endl;
                }

                testa++;
            }
        }

        if(temp == 4){
            cout << "------------------" << endl;
            cout << "Números primos registrados: " << qtdPrimos << endl;

            qtdPrimosIntervalo(1, 10, qtdPrimos, primos);
            qtdPrimosIntervalo(10, 100, qtdPrimos, primos);
            qtdPrimosIntervalo(100, 1000, qtdPrimos, primos);
            qtdPrimosIntervalo(1000, 10000, qtdPrimos, primos);
            qtdPrimosIntervalo(1000000, 10000000, qtdPrimos, primos);

        }

        if(temp == 8){
            remove(arquivo);
            if(grava(arquivo, 2) == ERR_OPEN)
                return ERR_OPEN;

            cout << "Novo arquivo criado!" << endl;

        }


    }
}
