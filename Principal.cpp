/* 
    Projeto Pratico - Hashing Extensivel
    Estrutura de dados
    Copyright 2018 by Altieres Pereira
    Metodo principal - Interface
*/
#include "TabelaHash.hpp"
#include <limits>

using namespace std;

int main()
{
    TabelaHash tabelaHash;
    InstrumentoMusical* instrumento = new InstrumentoMusical;
    int opcao, numeroDoBloco;
    string id, nome, tipo, descricao, claves;
    unsigned long int idInt;
    bool idInserido;
    do
    {
        idInserido = false;
        cout << "Escolha uma opcao:\n1. Inserir\n2. Recuperar valor pelo id\n3. Percorre\n4. Remove\n5. Imprime ordenado\n6. Alterar\n0. Sair\n";
        cin >> opcao;
        switch (opcao)
        {
            case 1 :
                cout << "Digite o ID" << endl;
                while (!idInserido)
                {
                    try
                    {
                        cin >> instrumento->id;
                        if (instrumento->id > numeric_limits<unsigned int>::max() or instrumento->id == 0)// Tratamento de excecoes para caso o usuario digite algum numero maior que o limite de um unsigned int, dado pelo template class numeric_limits
                        {
                            throw instrumento->id;
                        }else
                        {
                            idInserido = true;
                        }
                    }catch (unsigned long int const)
                    {
                        cin.clear();
                        cin.ignore(100,'\n');
                        cout << "Digite um ID entre 1 e 4294967295\n";
                    }
                }
                cin.ignore();
                cout << "Digite o nome" << endl;
                getline(cin, nome);// Primeiro armazenando em uma string para nao gerar problemas no caso do usuario digitar um texto maior do que o campo suporta
                strncpy(instrumento->nome, nome.c_str(), 30);// Copiando os 30 primeiros caracteres para o char que sera inserido
                cout << "Digite o tipo (Cordas, Sopro ou Percussao): " << endl;
                getline(cin, tipo);
                strncpy(instrumento->tipo, tipo.c_str(), 20);
                cout << "Digite o ano de criacao" << endl;
                cin >> instrumento->anoCriacao;
                cin.ignore();
                cout << "Digite a descricao" << endl;
                getline(cin, descricao);
                strncpy(instrumento->descricao, descricao.c_str(), 300);
                cout << "Digite as claves que o instrumento usa:" << endl;
                getline(cin,claves);
                strncpy(instrumento->claves, claves.c_str(), 20);
                tabelaHash.Insere(instrumento);
                break;
            case 2 :
                cout << "Digite o id" << endl;
                while (!idInserido)
                {
                    try
                    {
                        cin >> idInt;
                        if (idInt > numeric_limits<unsigned int>::max() or idInt == 0)
                        {
                            throw idInt;
                        }else
                        {
                            idInserido = true;
                        }
                        
                    }catch (unsigned long int const)
                    {
                        cin.clear();
                        cin.ignore(100,'\n');
                        cout << "Digite um ID entre 1 e 4294967295\n";
                    }
                }
                tabelaHash.ImprimeElementoAtual(tabelaHash.Recupera(idInt));//funcao da classe TabelaHash
                break;
            case 3:
                tabelaHash.Percorre();
                break;
            case 4:
                cout << "Digite o id" << endl;
                while (!idInserido)
                {
                    try
                    {
                        cin >> idInt;
                        if (idInt > numeric_limits<unsigned int>::max() or idInt == 0)
                        {
                            throw idInt;
                        }else
                        {
                            idInserido = true;
                        }
                        
                    }catch (unsigned long int const)
                    {
                        cin.clear();
                        cin.ignore(100,'\n');
                        cout << "Digite um ID entre 1 e 4294967295\n";
                    }
                }
                tabelaHash.Remove(idInt);
                break;
            case 5:
                cout << "Digite o numero do bloco que deseja imprimir ordenado\n";
                cin >> numeroDoBloco;
                tabelaHash.ImprimeBlocoOrdenado(numeroDoBloco);
                break;
            case 6:
                cout << "Digite o ID que deseja substituir" << endl;
                while (!idInserido)
                {
                    try
                    {
                        cin >> instrumento->id;
                        if (instrumento->id > numeric_limits<unsigned int>::max() or instrumento->id == 0)
                        {
                            throw instrumento->id;
                        }else
                        {
                            idInserido = true;
                        }
                        
                    }catch (unsigned long int const)
                    {
                        cin.clear();
                        cin.ignore(100,'\n');
                        cout << "Digite um ID entre 1 e 4294967295\n";
                    }
                }
                cin.ignore();
                cout << "Digite o nome" << endl;
                getline(cin, nome);
                strncpy(instrumento->nome, nome.c_str(), 30);
                cout << "Digite o tipo (Cordas, Sopro ou Percussao): " << endl;
                getline(cin, tipo);
                strncpy(instrumento->tipo, tipo.c_str(), 20);
                cout << "Digite o ano de criacao" << endl;
                cin >> instrumento->anoCriacao;
                cin.ignore();
                cout << "Digite a descricao" << endl;
                getline(cin, descricao);
                strncpy(instrumento->descricao, descricao.c_str(), 300);
                cout << "Digite as claves que o instrumento usa:" << endl;
                getline(cin,claves);
                strncpy(instrumento->claves, claves.c_str(), 20);
                tabelaHash.Altera(instrumento);
                break;
            case 0 :
                break;
            default :
                cerr << "Comando nao encontrado\n";
                break;
        }
    }
    while (opcao != 0);
    delete instrumento;
    return 0;
}
