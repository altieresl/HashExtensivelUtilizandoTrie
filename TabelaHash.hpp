/* 
    Projeto Pratico - Hashing Extensivel
    Estrutura de dados
    Copyright 2018 by Altieres Pereira
    Tabela Hash utilizando o conceito de trie
*/
#ifndef TABELAHASH_HPP
#define TABELAHASH_HPP
#include "Arquivo.hpp"
#include <fstream>
#include <iostream>
#include <string.h>

class TabelaHash
{
    public:
        TabelaHash();
        void Insere(InstrumentoMusical* _instrumentoMusical);
        InstrumentoMusical* Recupera(unsigned long int _id);
        void ImprimeElementoAtual(InstrumentoMusical* _instrumento);
        void Percorre();
        void ImprimeBlocoOrdenado(int _bloco);
        void Remove(unsigned long int _id);
        void Altera(InstrumentoMusical* _instrumento);
    private:
        std::string ConverteParaBinario(unsigned long int _id);
        float ComparaCompatibilidade(std::string _aux, std::string _buscado);
        PosicaoNoArquivo BuscaParaInserir(unsigned long int _id);
        PosicaoNoArquivo BuscaAux(unsigned long int _id);
        void ArrumaOrdemNoArquivo(std::fstream* _arquivoBin, PosicaoNoArquivo _posicaoNoArquivo);
        void BubbleSort(InstrumentoMusical _instrumento[], int tamanho);
};

#endif
