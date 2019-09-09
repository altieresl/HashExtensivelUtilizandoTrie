/* 
    Projeto Pratico - Hashing Extensivel
    Estrutura de dados
    Copyright 2018 by Altieres Pereira
    Tabela Hash utilizando o conceito de trie
*/
#include "TabelaHash.hpp"
#include <bitset>

using namespace std;

TabelaHash::TabelaHash()
{
    ifstream arquivoTextoIn("arquivo_texto.txt");
    if (arquivoTextoIn)// Verifica se o arquivo texto existe
    {
        arquivoTextoIn.close();
    }else// Senao cria
    {
        arquivoTextoIn.close();
        ofstream arquivoTextoOut("arquivo_texto.txt");
        arquivoTextoOut.close();
    }
    fstream arquivoBinario("arquivo_binario.bin", ios::binary | ios::in);// Aberto para leitura
    if (!arquivoBinario)// Se nao existe fecha a leitura e abre como escrita para criar o arquivo
    {
        arquivoBinario.close();
        arquivoBinario.open("arquivo_binario.bin", ios::binary | ios::out);
    }
    arquivoBinario.close();
}

string TabelaHash::ConverteParaBinario(unsigned long int _id)// Funcao que converte um unsigned long int em uma string que representa um binario
{
    bitset<64> binario{_id};// Bitset que representa numeros apenas com 0 e 1
    string binAux = binario.to_string<char,string::traits_type,string::allocator_type>();// Convertendo bitset pra string a funcao da classe bitset
    int posicaoUltimo1 = -1;
    for (int i = (int)binAux.length(); i >= 0; i--)
    {
        if (binAux[i] == '1')
        {
            posicaoUltimo1 = i;// Assim como um inteiro imprime apenas os numeros apos o ultimo 1, encontramos a posicao do ultimo para fazer o mesmo em uma representacao com string
        }
    }
    string bin = "";    
    for (unsigned int i = posicaoUltimo1; i < binAux.length(); i++)// Copia do ultimo 1 ate o final da string
    {
        bin += binAux[i];
    }
    if (_id == 0)
    {
        bin = "0";// Caso o id seja 0 temos que definir seu binario manualmente, ja que nos estamos trabalhando apenas com os numeros apos o ultimo 1 (nesse caso nao haveria nenhum)
    }
    return bin;
}

float TabelaHash::ComparaCompatibilidade(string _aux, string _buscado)// Funcao utilizada para definir o grau de compatibilidade entre dois binarios representados em string
{
    /*
        Percorrendo do tamanho do menor binario ate 1 e comparamos do ultimo bit das duas strings ate terminar a menor delas,
        caso o bit de alguma string seja diferente do bit da outra string (relativamente falando) o percorrimento e encerrado e
        sabemos que as strings nao sao compativeis, senao, o nivel de compatibilidade e dado pela quantidade de caracteres compativeis
        dividido pelo tamanho da maior
    */
    bool compativeis = true;
    int j = int(_aux.length()-1), k = int(_buscado.length()-1), quantidadeCaracteresCompativeis = 0, tamanhoDoMaiorBinario = (int)max(_aux.length(),_buscado.length());
    for (int i = int(min(_aux.length(),_buscado.length())); i > 0 and compativeis; i--, j--, k--)
    {
        if (_aux[j] != _buscado[k])
        {
            compativeis = false;
        }else
        {
            quantidadeCaracteresCompativeis++;
        }
    }
    float retorno;
    if (compativeis)
    {
        retorno = ((float)quantidadeCaracteresCompativeis/(float)tamanhoDoMaiorBinario);
    }else
    {
        retorno = 0;
    }
    return retorno;
}

PosicaoNoArquivo TabelaHash::BuscaParaInserir(unsigned long int _id)// Funcao que busca uma posicao para a insercao de acordo com o id passado, utiliza o grau de compatibilidade para definir qual o melhor lugar para a insercao
{
    ifstream arquivoTxt("arquivo_texto.txt");
    PosicaoNoArquivo posicaoNoArquivo, posicaoNoArquivoAux; // O primeiro objeto e utilizado para ser o a posicao final no arquivoTxt, enquanto a outra serve para armazenar posicoes auxiliares e comparar em busca da melhor insercao
    posicaoNoArquivo.posicaoNaTabela = -1;
    posicaoNoArquivo.qtdRegistros = 0;
    int i = 0;
    string aux;
    bool encontradaColisao = false;
    int posicaoQuantidadeRegistros = -1;
    bool igual = false;
    float grauCompatibilidade = 0;
    float maiorCompatibilidade = 0;
    while (arquivoTxt >> aux)
    {
        if (i % 3 == 0)// Campo da posicao do bloco
        {
            posicaoNoArquivoAux.posicaoNaTabela = stoi(aux);// Convertendo a string para inteiro
        }else if (i % 3 == 1)// Campo da faixa de IDs abrigados pelo bloco
        {
            grauCompatibilidade = ComparaCompatibilidade(aux,ConverteParaBinario(_id));// Comparando cada faixa de ID do arquivoTxt texto para encontrar a que o novo ID mais chega perto
            if (grauCompatibilidade > 0)// Caso seja 0 significa que os ids tem bits diferentes em uma mesma "posicao"
            {
                encontradaColisao = true;// Significa que o novo id tem algum grau de compatibilidade com alguma das faixas ja criadas no arquivoTxt texto, assim sabemos que mesmo que seja necessario dividir uma faixa, nao sera necessario criar uma nova apenas para ele
                if (grauCompatibilidade > maiorCompatibilidade)
                {
                    maiorCompatibilidade = grauCompatibilidade;// Armazena o maior para comparar com os outros
                    posicaoQuantidadeRegistros = i + 1;
                    posicaoNoArquivoAux.id = aux;// Salva no auxiliar o atual maior, para o caso dos posteriores serem menores
                }else if (grauCompatibilidade == maiorCompatibilidade)
                {
                    posicaoQuantidadeRegistros = i + 1;
                    posicaoNoArquivoAux.id = aux;
                    igual = true;// Caso a compatibilidade seja igual, escolheremos um que tenha espaco para a insercao, caso nenhum deles tenha, permanece o primeiro
                }
            }
        }else if (i % 3 == 2 and i == posicaoQuantidadeRegistros)
        {
            if (!igual)// Caso nao seja igual apenas define o atual como a posicao desejada, pode ser alterado caso seja encontrado um maior
            {
                posicaoNoArquivo.posicaoNaTabela = posicaoNoArquivoAux.posicaoNaTabela;
                posicaoNoArquivo.qtdRegistros = stoi(aux);
                posicaoNoArquivo.id = posicaoNoArquivoAux.id;
            }else
            {
                igual = false;
                if (stoi(aux) < TAM_BLOCO)// Para o caso de entre os dois 'maiores' ate o momento serem iguais, optaremos sempre pelo que tem espaco para facilitar a insercao 
                {
                    posicaoNoArquivo.posicaoNaTabela = posicaoNoArquivoAux.posicaoNaTabela;
                    posicaoNoArquivo.qtdRegistros = stoi(aux);
                    posicaoNoArquivo.id = posicaoNoArquivoAux.id;
                }
            }
        }
        i++;
    }
    arquivoTxt.close();
    if (!encontradaColisao)// Caso nao seja encontrada colisao, retorna registro com valores -1, informando que deve ser criado um novo bloco
    {
        PosicaoNoArquivo posicaoInvalida;
        posicaoInvalida.posicaoNaTabela = -1;
        posicaoInvalida.qtdRegistros = -1;
        return posicaoInvalida;
    }else
    {
        return posicaoNoArquivo;
    }
}

PosicaoNoArquivo TabelaHash::BuscaAux(unsigned long int _id)// Funcao de busca que nao compara o grau de compatibilidade, apenas busca um id em todas as faixas que tal id tenha alguma compatibilidade
{
    ifstream arquivoTxt("arquivo_texto.txt");
    fstream arquivoBin("arquivo_binario.bin", ios::binary | ios::out | ios::in);
    int i = 0, j;
    string aux;
    bool encontrado = false;
    PosicaoNoArquivo posicaoNoArquivo;
    InstrumentoMusical* instrumento = new InstrumentoMusical;
    while (arquivoTxt >> aux and !encontrado)
    {
        if (i % 3 == 0)// Campo da posicao do bloco
        {
            posicaoNoArquivo.posicaoNaTabela = stoi(aux);
        }
        if (i % 3 == 1)// Campo da faixa de IDs abrigados pelo bloco
        {
            if (ComparaCompatibilidade(aux,ConverteParaBinario(_id)) > 0)// Quando a compatibilidade for maior que zero, percorre o arquivo binario em busca de um registro com id igual
            {
                j = 0;
                arquivoBin.seekg(posicaoNoArquivo.posicaoNaTabela * sizeof(Bloco), arquivoBin.beg);
                while (!encontrado and arquivoBin.read(reinterpret_cast<char *>(instrumento), sizeof(InstrumentoMusical)) and j < TAM_BLOCO)
                {
                    if (_id == instrumento->id)
                    {
                        encontrado = true;// Quando encontrado interrompe a busca e armazena os dados
                        posicaoNoArquivo.id = aux;
                        posicaoNoArquivo.posicaoDoIdNoBloco = j;
                    }
                    j++;
                }
            }
        }
        i++;
    }
    arquivoBin.close();
    arquivoTxt.close();
    if (encontrado)
    {
        return posicaoNoArquivo;// Retornando os dados encontrados
    }else// Caso nada seja encontrado retorna um registro com dados -1 indicando que nao foi encontrado
    {
        PosicaoNoArquivo posicaoNaoEncontrada;
        posicaoNaoEncontrada.id = "-1";
        posicaoNaoEncontrada.qtdRegistros = -1;
        posicaoNaoEncontrada.posicaoNaTabela = -1;
        posicaoNaoEncontrada.posicaoDoIdNoBloco = -1;
        return posicaoNaoEncontrada;
    }
}

void TabelaHash::ArrumaOrdemNoArquivo(fstream* _arquivoBin, PosicaoNoArquivo _posicaoNoArquivo)// Funcao que arruma um bloco no arquivo, depois de um bloco se dividir ou de uma remocao, por exemplo, quando se removeria o primeiro item de um bloco e ficaria um espaco vazio antes do segundo
{
    InstrumentoMusical* instrumento = new InstrumentoMusical;
    InstrumentoMusical* instrumentoVazio = new InstrumentoMusical;// Cria um objeto de InstrumentoMusical que sera utilizado para simular um elemento 'removido'
    instrumentoVazio->id = 0;// Inicializando ele como invalido
    strcpy(instrumentoVazio->nome,"");
    strcpy(instrumentoVazio->tipo, "");
    instrumentoVazio->anoCriacao = 0;
    strcpy(instrumentoVazio->claves, "");
    strcpy(instrumentoVazio->descricao, "");
    bool houveTroca = true;
    int k = 0, j, i;
    while (k < TAM_BLOCO and houveTroca)// Quando nao houver troca significa que nao existem espacos em branco no bloco
    {
        _arquivoBin->seekg(_posicaoNoArquivo.posicaoNaTabela * sizeof(Bloco), _arquivoBin->beg);// Posiciona o cursor no incio do bloco
        i = 0, j = 0;
        unsigned long int posicaoVazia[TAM_BLOCO], k = 0, contVazio = 0, passaInstrumento[TAM_BLOCO], passaVazio[TAM_BLOCO];
        InstrumentoMusical* aux2 = new InstrumentoMusical[TAM_BLOCO];// Cria vetores auxiliares do tamanho TAM_BLOCO, para garantir que nao faltaria espaco, ao inves disso poderiamos utilizar a classe vector ou uma fila
        while (_arquivoBin->read(reinterpret_cast<char *> (instrumento), sizeof(InstrumentoMusical)) and i < TAM_BLOCO)// Um instrumento para a memoria
        {
            string nomeStr(instrumento->nome);// Criamos uma string com o campo de nome do instrumento (que era um vetor de char) para ter acesso a funcao de tamanho
            if (nomeStr.length() == 0)// Caso aquele instrumento esteja 'vazio'
            {
                posicaoVazia[j] = i;// Armazenamos o mesmo em um vetor que guardara vazios
                contVazio++;// Incrementamos o contador
                j++;// Utilizado para armazenar o proximo 'vazio' em uma nova posicao do vetor
            }else if (nomeStr.length() > 0)// Quando o instrumento esta valido
            {
                if (contVazio > 0)// Se existe algum instrumento vazio antes desse, salvamos sua posicao para trocarmos depois
                {
                    passaInstrumento[k] = _posicaoNoArquivo.posicaoNaTabela * sizeof(Bloco) + posicaoVazia[k] * sizeof(InstrumentoMusical);// Posicao (que esta vazia) onde o instrumento encontrado ficara
                    aux2[k] = *instrumento;// Armazenando o tal instrumento
                    passaVazio[k] = _posicaoNoArquivo.posicaoNaTabela * sizeof(Bloco) + i * sizeof(InstrumentoMusical);// Posicao onde o instrumento vazio ficara
                    k++;
                    contVazio--;
                }
            }
            i++;
        }
        _arquivoBin->clear();// Para o caso do arquivo ter sido lido ate o final e ficado inutilizado
        for (unsigned int j = 0; j < k; j++)// Percorre de 0 ate a quantidade de elementos validos apos elementos vazios fazendo a troca
        {
            _arquivoBin->seekp(passaInstrumento[j], _arquivoBin->beg);
            _arquivoBin->write(reinterpret_cast<const char *> (&aux2[j]), sizeof(InstrumentoMusical));
            _arquivoBin->seekp(passaVazio[j], _arquivoBin->beg);
            _arquivoBin->write(reinterpret_cast<const char *> (instrumentoVazio), sizeof(InstrumentoMusical));
        }
        _arquivoBin->clear();// Para o caso do arquivo ter sido lido ate o final e ficado inutilizado
        if (k == 0)// Caso k seja 0 significa que nao existe nenhum instrumento valido apos um vazio, entao nao houve troca
        {
            houveTroca = false;// Caso nao houve troca significa que todas as posicoes estao ok
        }
        k++;
        delete[] aux2;
    }
    delete instrumento;
    delete instrumentoVazio;
}

void TabelaHash::BubbleSort(InstrumentoMusical _instrumento[], int tam)// Para a ordenacao de InstrumentosMusicais
{
    InstrumentoMusical aux;
    for (int i = 1; i < tam; i++)
    {
        for (int j = 0; j < tam-i; j++)
        {
            if (_instrumento[j].id > _instrumento[j+1].id)// Utilizando o id como criterio
            {
                aux = _instrumento[j];
                _instrumento[j] = _instrumento[j+1];
                _instrumento[j+1] = aux;
            }
        }
    }
}

void TabelaHash::Remove(unsigned long int _id)// Funcao que remove um instrumento do arquivo, pelo ID
{
    fstream arquivoTxt("arquivo_texto.txt", ios::in | ios::out);
    fstream arquivoBin("arquivo_binario.bin", ios::binary | ios::out | ios::in);
    PosicaoNoArquivo posicaoNoArquivo = BuscaAux(_id);// A busca retorna a o registro que inclui a posicao dele na tabela e outros dados
    cout << "Posicao na tabela & " << posicaoNoArquivo.posicaoNaTabela << endl;
    if (posicaoNoArquivo.posicaoNaTabela != -1)// Caso a posicao seja valida
    {
        InstrumentoMusical* instrumento = new InstrumentoMusical;
        InstrumentoMusical* instrumentoVazio = new InstrumentoMusical;// Criando um instrumento vazio e atribuindo valores de elemento vazio nele
        instrumentoVazio->id = 0;
        strcpy(instrumentoVazio->nome,"");
        strcpy(instrumentoVazio->tipo, "");
        instrumentoVazio->anoCriacao = 0;
        strcpy(instrumentoVazio->claves, "");
        strcpy(instrumentoVazio->descricao, "");
        int i = 0, posicaoParaRemover = -1;
        bool encontradoParaRemover = false;
        arquivoBin.seekg(posicaoNoArquivo.posicaoNaTabela * sizeof(Bloco), arquivoBin.beg);// A posicao na tabela hash dada pelo registro e utilizada para posicionaar o cursor no arquivo binario
        while (arquivoBin.read(reinterpret_cast<char *> (instrumento) , sizeof(InstrumentoMusical)) and i < TAM_BLOCO and !encontradoParaRemover)// Traz para a memoria instrumentos um por um, ate TAM_BLOCO vezes ou caso seja encontrado o elemento
        {
            if (instrumento->id == _id)// Caso o id seja encontrado no arquivo binario
            {
                encontradoParaRemover = true;
                posicaoParaRemover = (int)arquivoBin.tellg()-(int)sizeof(InstrumentoMusical);// A posicao onde o instrumento se encontra para a remocao e dada pela posicao do cursor atual menos o tamanho de um instrumento
            }
            i++;
        }
        arquivoBin.clear();// Para o caso do arquivo ter sido lido ate o final e ficado inutilizado
        arquivoBin.seekp(posicaoParaRemover, arquivoBin.beg);
        arquivoBin.write(reinterpret_cast<const char *> (instrumentoVazio), sizeof(InstrumentoMusical));//Escrevendo um instrumento vazio em cima do elemento que desejamos remover
        arquivoBin.clear();
        bool encontradoNoTxt = false;
        int aux;
        i = 0;
        int posicaoqtdRegistros = -1;
        while (arquivoTxt >> aux)// Percorrendo o arquivo texto ate encontrar a posicao que desejamos alterar
        {
            if (i % 3 == 0 and aux == posicaoNoArquivo.posicaoNaTabela)
            {
                encontradoNoTxt = true;
            }else if (i % 3 == 2 and encontradoNoTxt)
            {
                encontradoNoTxt = false;
                posicaoqtdRegistros = (int)arquivoTxt.tellg(); // A posicao que indica a quantidade de registros no arquivo texto e que queremos alterar por ter removido um instrumento
                posicaoNoArquivo.qtdRegistros = aux;
            }
            i++;
        }
        arquivoTxt.clear();
        arquivoTxt.seekp(posicaoqtdRegistros-1, arquivoBin.beg);
        arquivoTxt << --posicaoNoArquivo.qtdRegistros;// Decrementando antes de escrever
        ArrumaOrdemNoArquivo(&arquivoBin, posicaoNoArquivo);// Funcao que arruma o bloco, pro do instrumento removido nao ser o ultimo do bloco
        delete instrumento;
        delete instrumentoVazio;
    }else// Caso o registro indique uma posicao invalida
    {
        cerr << "Elemento nao encontrado para a remocao\n";
    }
    arquivoBin.close();
    arquivoTxt.close();
}

void TabelaHash::Insere(InstrumentoMusical* _instrumentoMusical)// Funcao de insercao no arquivo
{
    if (Recupera(_instrumentoMusical->id) != NULL)// Caso seja possivel recuperar um instrumento com tal ID no arquivo significa que o mesmo ja existe
    {
        cerr << "Id ja existente na tabela hash\n";
    }else
    {
        fstream arquivoBin("arquivo_binario.bin", ios::binary | ios::out | ios::in);
        fstream arquivoTxt("arquivo_texto.txt", ios::in | ios::out);
        int i = 0;
        unsigned long int aux;
        PosicaoNoArquivo posicaoNoArquivo = BuscaParaInserir(_instrumentoMusical->id);// Utilizando a busca que leva em consideracao o grau de compatibilidade
        posicaoNoArquivo.posicaoProximaInsercao = -1;
        if (posicaoNoArquivo.posicaoNaTabela == -1)// Caso a busca retorne uma posicao invalida significa que um novo bloco deve ser criado
        {
            while (arquivoTxt >> aux)
            {
                if (i % 3 == 0)
                {
                    posicaoNoArquivo.posicaoProximaInsercao = (int)aux;// Buscando a posicao para a insercao
                }
                i++;
            }
            arquivoTxt.clear();
            arquivoTxt << ++posicaoNoArquivo.posicaoProximaInsercao << " ";// Incrementando a posicao encontrada (que no caso era a ultima inserida)
            string idInstrumento = ConverteParaBinario(_instrumentoMusical->id);// Convertendo para binario
            arquivoTxt << idInstrumento[idInstrumento.length()-1] << " ";// Escrevendo o ultimo digito do binario, para esse novo bloco (que sera 0 ou 1)
            arquivoTxt << 1 << endl;// Quantidade de registros (que comeca em 1)
            arquivoBin.seekp((posicaoNoArquivo.posicaoProximaInsercao) * sizeof(Bloco), arquivoBin.beg);// Posicionando o cursor na proxima posicao de insercao
            Bloco* bloco = new Bloco;// Criando um bloco para escrever no arquivo
            arquivoBin.write(reinterpret_cast<const char *> (bloco), sizeof(Bloco));
            delete bloco;
            arquivoBin.seekp(-sizeof(Bloco), arquivoBin.cur);// Posicionando o cursor um bloco atras (posicao relativa)
            arquivoBin.write(reinterpret_cast<const char *> (_instrumentoMusical), sizeof(InstrumentoMusical));
        }else if (posicaoNoArquivo.posicaoNaTabela != -1 and posicaoNoArquivo.qtdRegistros < TAM_BLOCO and posicaoNoArquivo.qtdRegistros >= 0)// Como nao e uma posicao invalida e a quantidade de registros e menor que TAM_BLOCO significa que podemos inserir nesse bloco
        {
            int i = 0, posicaoqtdRegistros = -1, bitsAteqtdRegistros = -1;
            unsigned long int aux;
            bool encontrado = false;
            while (arquivoTxt >> aux)// Buscando a posicao para alterar no arquivo texto (acrescentar o elemento)
            {
                if (i % 3 == 0 and (int)aux == posicaoNoArquivo.posicaoNaTabela)
                {
                    encontrado = true;
                    posicaoqtdRegistros = i + 2;
                }
                if (encontrado and i == posicaoqtdRegistros)
                {
                    bitsAteqtdRegistros = (int)arquivoTxt.tellg();
                }
                i++;
            }
            arquivoTxt.clear();
            arquivoTxt.seekp(bitsAteqtdRegistros - 1, arquivoTxt.beg);// Posicao dada pelo tellg dentro da busca
            arquivoTxt << ++posicaoNoArquivo.qtdRegistros;// Escrevendo ja incrementando
            arquivoBin.seekp((posicaoNoArquivo.posicaoNaTabela * sizeof(Bloco)) + ((posicaoNoArquivo.qtdRegistros - 1) * sizeof(InstrumentoMusical)));// Posicionando o cursor do arquivo binario na posicao de insercao
            arquivoBin.write(reinterpret_cast<const char *> (_instrumentoMusical), sizeof(InstrumentoMusical));// Escrevendo o instrumento no arquivo binario
        }else if (posicaoNoArquivo.posicaoNaTabela != -1 and posicaoNoArquivo.qtdRegistros == TAM_BLOCO)// Criar novo bloco e dividir
        {
            int i = 0;
            while (arquivoTxt >> aux)
            {
                if (i % 3 == 0)
                {
                    posicaoNoArquivo.posicaoProximaInsercao = (int)aux;
                }
                i++;
            }
            string novaChave;// Cria o que sera a chave do novo bloco
            string idInseridoBin = ConverteParaBinario(_instrumentoMusical->id);// Armazenamos o id recem incerido
            novaChave = idInseridoBin[idInseridoBin.length()-posicaoNoArquivo.id.length()-1];// A nova chave recebe o digito na posicao referente a um bit posterior da faixa de id que causou a colisao, exemplo: novo id inserido 0101, id antigo 01, tal bit seria o 1 (que seria o terceiro digito da direita pra esquerda)
            novaChave += posicaoNoArquivo.id;// Concatena a antiga faixa nessa nova, ainda utilizando o exemplo anterior, resultaria em 101
            cout << "Estouro no bloco " << posicaoNoArquivo.id << ", agora temos uma chave de " << novaChave.length() << " bits : " << novaChave << endl;
            arquivoBin.clear();
            PosicaoNoArquivo posicaoNovoBloco;// Posicao no arquivo referente ao novo bloco
            posicaoNovoBloco.posicaoNaTabela = ++posicaoNoArquivo.posicaoProximaInsercao;
            posicaoNovoBloco.qtdRegistros = 1;// O bloco inicia com um elemento
            arquivoBin.seekp(posicaoNovoBloco.posicaoNaTabela * sizeof(Bloco), arquivoBin.beg);// Posicionando onde sera o novo bloco
            Bloco* bloco = new Bloco;
            arquivoBin.write(reinterpret_cast<const char *> (bloco), sizeof(Bloco));// Passando o bloco pro arquivo
            delete bloco;
            arquivoBin.seekp(posicaoNovoBloco.posicaoNaTabela * sizeof(Bloco), arquivoBin.beg);// Voltando pro inicio do bloco recem criado (poderia ter sido com (-sizeof(Bloco), arquivoBeg.cur) utilizando posicao relativa)
            arquivoBin.write(reinterpret_cast<const char *> (_instrumentoMusical), sizeof(InstrumentoMusical));// Escrevendo o instrumento no novo bloco
            arquivoBin.clear();
            bool precisaTrocarDeLugar = false;
            if (novaChave != posicaoNoArquivo.id)// Verificando se a nova chave nao e a mesma da anterior, pois nesse caso nao buscariamos elementos do primeiro bloco que estao aptos a ir para o segundo
            {
                arquivoBin.seekg(posicaoNoArquivo.posicaoNaTabela * sizeof(Bloco), arquivoBin.beg);// Posicionando o cursor no inicio do bloco inicial para buscar elementos aptos a ir para o novo bloco
                InstrumentoMusical* instrumento = new InstrumentoMusical;
                i = 0;
                unsigned long int qtdAIrParaNovoBloco = 0, posicaoNovo[TAM_BLOCO], posicaoAntigo[TAM_BLOCO];
                float grauCompatibilidade = 0, grauCompatibilidade2 = 0;
                InstrumentoMusical* aux = new InstrumentoMusical[TAM_BLOCO];
                while (arquivoBin.read(reinterpret_cast<char *> (instrumento), sizeof(InstrumentoMusical)) and i < TAM_BLOCO and posicaoNoArquivo.qtdRegistros > 0 and posicaoNovoBloco.qtdRegistros < TAM_BLOCO)// Busca instrumentos no arquivo binario enquanto a quantidade de registros do antigo for maior que 0 e a quantidade do novo for menor que TAM_BLOCO
                {
                    grauCompatibilidade = ComparaCompatibilidade(novaChave, ConverteParaBinario(instrumento->id));// Comparando compatibilidade do id do instrumento do bloco inicial com a nova chave
                    if (grauCompatibilidade > 0)// Caso seja maior que 0 significa que e compativel
                    {
                        grauCompatibilidade2 = ComparaCompatibilidade(posicaoNoArquivo.id, ConverteParaBinario(instrumento->id));
                        if (grauCompatibilidade > grauCompatibilidade2)// Comparando se o id do bloco inicial e mais compativel com a antiga ou a nova chave, caso ele seja mais compativel com a nova ele sera movido para o novo bloco
                        {
                            posicaoNovo[qtdAIrParaNovoBloco] = posicaoNovoBloco.posicaoNaTabela * sizeof(Bloco) + (posicaoNovoBloco.qtdRegistros) * sizeof(InstrumentoMusical);
                            aux[qtdAIrParaNovoBloco] = *instrumento;
                            posicaoAntigo[qtdAIrParaNovoBloco] = (unsigned long int)arquivoBin.tellg() - sizeof(InstrumentoMusical);
                            precisaTrocarDeLugar = true;// Quando ao menos um elemento e trocado de lugar, precisamos verificar se existe algum instrumento em branco antes de um instrumento valido
                            qtdAIrParaNovoBloco++;
                            posicaoNoArquivo.qtdRegistros--;// Decrementando a quantidade de registros do antigo bloco para escrever no arquivo texto depois
                            posicaoNovoBloco.qtdRegistros++;// Incrementando a quantidade de registros do novo bloco para escrever no arquivo texto depois
                        }
                    }
                    i++;
                }
                delete instrumento;
                arquivoBin.clear();
                InstrumentoMusical* instrumentoVazio = new InstrumentoMusical;// Criando um registro vazio que sera escrito onde removemos um elemento para inserir no outro bloco
                instrumentoVazio->id = 0;
                strcpy(instrumentoVazio->nome,"");
                strcpy(instrumentoVazio->tipo, "");
                instrumentoVazio->anoCriacao = 0;
                strcpy(instrumentoVazio->claves, "");
                strcpy(instrumentoVazio->descricao, "");
                for (unsigned int i = 0; i < qtdAIrParaNovoBloco; i++)// Passando os elementos aptos a ir para o novo bloco
                {
                    arquivoBin.seekp(posicaoNovo[i], arquivoBin.beg);
                    arquivoBin.write(reinterpret_cast<const char *> (&aux[i]), sizeof(InstrumentoMusical));
                    arquivoBin.seekp(posicaoAntigo[i], arquivoBin.beg);
                    arquivoBin.write(reinterpret_cast<const char *> (instrumentoVazio), sizeof(InstrumentoMusical));
                }
                arquivoBin.clear();
                delete[] aux;
                delete instrumentoVazio;
                if (precisaTrocarDeLugar)// Caso algum elemento foi para o novo bloco, verificamos se a ordem esta ok
                {
                    ArrumaOrdemNoArquivo(&arquivoBin, posicaoNoArquivo);
                }
            }
            arquivoTxt.clear();
            arquivoTxt << posicaoNoArquivo.posicaoProximaInsercao << " ";// Escrevendo o novo bloco no arquivo texto
            arquivoTxt << novaChave << " ";
            arquivoTxt << posicaoNovoBloco.qtdRegistros << endl;
            int bitsAteqtdRegistros = 0, posicaoqtdRegistros = -1;
            bool encontrado = false;
            i = 0;
            arquivoTxt.clear();
            arquivoTxt.seekg(arquivoTxt.beg);
            while (arquivoTxt >> aux)// Modificando o bloco anterior no arquivo texto
            {
                if (i % 3 == 0 and (int)aux == posicaoNoArquivo.posicaoNaTabela)
                {
                    encontrado = true;
                    posicaoqtdRegistros = i + 2;
                }
                if (encontrado and i == posicaoqtdRegistros)
                {
                    bitsAteqtdRegistros = (int)arquivoTxt.tellg();
                    encontrado = false;
                }
                i++;
            }
            arquivoTxt.clear();
            arquivoTxt.seekp(bitsAteqtdRegistros - 1, arquivoTxt.beg);// A posicao da quantidade de registros que queremos alterar (do antigo bloco)
            arquivoTxt << posicaoNoArquivo.qtdRegistros;
        }
        arquivoTxt.close();
        arquivoBin.close();
    }
}

InstrumentoMusical* TabelaHash::Recupera(unsigned long int _id)// Funcao que busca um instrumento associado a um ID
{
    ifstream arquivoTxt("arquivo_texto.txt");
    fstream arquivoBin("arquivo_binario.bin", ios::binary | ios::out | ios::in);
    int i = 0, j;
    string aux;
    int localizacaoNoArqAux;
    bool encontrado = false;
    InstrumentoMusical* instrumento = new InstrumentoMusical;
    while (arquivoTxt >> aux and !encontrado)// Percorrendo o arquivoTxt texto
    {
        if (i % 3 == 0)// Campo da localizacao do bloco no arquivoTxt binario
        {
            localizacaoNoArqAux = stoi(aux);
        }
        if (i % 3 == 1)// Campo da faixa dos IDS
        {
            if (ComparaCompatibilidade(aux,ConverteParaBinario(_id)) > 0)// Se e compativel, nao importa o quao compativel, busca no arquivoTxt binario a partir da localizacao do campo anterior
            {
                j = 0;
                arquivoBin.seekg(localizacaoNoArqAux * sizeof(Bloco), arquivoBin.beg);
                while (!encontrado and arquivoBin.read(reinterpret_cast<char *>(instrumento), sizeof(InstrumentoMusical)) and j < TAM_BLOCO)
                {
                    if (_id == instrumento->id)// Caso seja encontrado um instrumento com id identico ao buscado, para a busca
                    {
                        encontrado = true;
                    }
                    j++;
                }
            }
        }
        i++;
    }
    arquivoTxt.close();
    arquivoBin.close();
    if (encontrado)
    {
        return instrumento;// Retorna o elemento encontrado, nao e possivel deletar o instrumento pois esta sendo passado como retorno
    }else
    {
        return NULL;// Caso nao seja encontrado retorna nulo
    }
}

void TabelaHash::Altera(InstrumentoMusical* _instrumento)// Funcao que altera os dados de um determinado ID
{
    PosicaoNoArquivo posicaoNoArquivo = BuscaAux(_instrumento->id);// Utiliza a funcao de busca que retorna uma posicao no arquivo
    if (posicaoNoArquivo.posicaoNaTabela == -1)// Se nao for valido avisa ao usuario que nao foi possivel encontrar
    {
        cerr << "Id nao encontrado para alteracao\n";
    }else// Caso seja valido ja posiciona exatamente no bloco e instrumento que desejamos alterar
    {
        ofstream arquivoBin("arquivo_binario.bin", ios::binary | ios::in | ios::out);
        arquivoBin.seekp(posicaoNoArquivo.posicaoNaTabela * sizeof(Bloco) + posicaoNoArquivo.posicaoDoIdNoBloco * sizeof(InstrumentoMusical), arquivoBin.beg);
        arquivoBin.write(reinterpret_cast<const char*> (_instrumento), sizeof(InstrumentoMusical));// Escrevemos o novo instrumento, onde o ID nao sera alterado
        arquivoBin.close();
    }
}

void TabelaHash::ImprimeElementoAtual(InstrumentoMusical* _instrumento)// Funcao que recebe um InstrumentoMusical e imprime seus atributos
{
    if (_instrumento != NULL)
    {
        cout << "Instrumento encontrado: "  << endl;
        cout << _instrumento->id            << endl;
        cout << _instrumento->nome          << endl;
        cout << _instrumento->tipo          << endl;
        cout << _instrumento->descricao     << endl;
        cout << _instrumento->anoCriacao    << endl;
        cout << _instrumento->claves        << endl;
    }else// Caso seja nulo, informa que nao foi encontrado
    {
        cerr << "Instrumento nao encontrado" << endl;
    }
}

void TabelaHash::Percorre()
{
    ifstream arquivoBin("arquivo_binario.bin", ios::binary);
    InstrumentoMusical* instrumento = new InstrumentoMusical;
    cout << "Lista de registros:"  << endl;
    int i = 0;
    while (arquivoBin.read(reinterpret_cast<char *>(instrumento), sizeof(InstrumentoMusical)))
    {
        if (i % TAM_BLOCO == 0)
        {
            cout << "Inicio do bloco\n";
        }
        cout << instrumento->id            << endl;
        cout << instrumento->nome          << endl;
        cout << instrumento->tipo          << endl;
        cout << instrumento->descricao     << endl;
        cout << instrumento->anoCriacao    << endl;
        cout << instrumento->claves        << endl << endl;
        i++;
    }
    delete instrumento;
    arquivoBin.close();
}

void TabelaHash::ImprimeBlocoOrdenado(int _numDoBloco)
{
    ifstream arquivoBin("arquivo_binario.bin", ios::binary);
    ifstream arquivoTxt("arquivo_texto.txt", ios::binary);
    int i = 0;
    int aux;
    bool encontrado = false;
    while (arquivoTxt >> aux and !encontrado)// Busca o bloco no arquivo texto, o qual estaria no primeiro campo
    {
        if (i % 3 == 0 and aux == _numDoBloco)// Campo da localizacao do bloco no arquivo
        {
            encontrado = true;
        }
        i++;
    }
    if (encontrado)// Como o bloco foi encontrado
    {
        i = 0;
        InstrumentoMusical* instrumentoVet = new InstrumentoMusical[TAM_BLOCO];
        arquivoBin.seekg(_numDoBloco * sizeof(Bloco), arquivoBin.beg);// Posicionando o cursor na parte do bloco que desejamos imprimir
        while (arquivoBin.read(reinterpret_cast<char*> (&instrumentoVet[i]), sizeof(InstrumentoMusical)) and i < TAM_BLOCO)// Armazenamos todos os instrumentos desse bloco num vetor da posicao 0 ate TAM_BLOCO-1
        {
            i++;
        }
        int quantidadeValidos = 0;// Contamos os validos para utilizar como parametro da ordenação posteriormente
        for (int i = 0; i < TAM_BLOCO; i++)
        {
            string nomeAux(instrumentoVet[i].nome);
            if (nomeAux.length() > 0)// Utilizamos um tamanho com quantidade de caracteres superior a 0 como a definicao de um instrumento existente
            {
                quantidadeValidos++;
            }
        }
        BubbleSort(instrumentoVet, quantidadeValidos);// Como nunca existira um elemento vazio no meio de um bloco seguido por um elemento valido, podemos utilizar esse contador como tamanho do vetor para ordenar
        if (quantidadeValidos > 0)
        {
            cout << "Bloco ordenado:\n";
            for (int k = 0; k < TAM_BLOCO; k++)
            {
                cout << instrumentoVet[k].id            << endl;
                cout << instrumentoVet[k].nome          << endl;
                cout << instrumentoVet[k].tipo          << endl;
                cout << instrumentoVet[k].descricao     << endl;
                cout << instrumentoVet[k].anoCriacao    << endl;
                cout << instrumentoVet[k].claves        << endl << endl;
            }
        }else
        {
            cerr << "Bloco vazio:\n";
        }
        delete[] instrumentoVet;
    }else
    {
        cerr << "Bloco nao encontrado\n";
    }
    arquivoBin.close();
    arquivoTxt.close();
}
