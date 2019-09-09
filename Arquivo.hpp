/* 
    Projeto Pratico - Hashing Extensivel
    Estrutura de dados
    Copyright 2018 by Altieres Pereira
    Estrutura armazenada e registro auxiliar utilizado no manuseio de arquivos
*/
#ifndef ARQUIVO_HPP
#define ARQUIVO_HPP
#include <iostream>
#define TAM_BLOCO 4

struct InstrumentoMusical
{
    unsigned long long int id;
    char nome[30];
    char tipo[20];      // Ex: corda
    int  anoCriacao;    // Ano de surgimento
    char descricao[300];
    char claves[20];
};

struct Bloco
{
    InstrumentoMusical instrumento[TAM_BLOCO];// Um bloco com TAM_BLOCO instrumentos
};

struct PosicaoNoArquivo// Registro auxiliar utilizado no manuseio dos arquivos da tabela hash
{
    int posicaoNaTabela;
    int qtdRegistros;
    int posicaoProximaInsercao;
    std::string id;
    int posicaoDoIdNoBloco;
};

#endif
