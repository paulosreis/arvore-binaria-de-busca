/* Estruturas de Dados - Trabalho pratico

Professor: Diego P. Rubert
Aluno(s): Paulo Guilherme Silva dos Santos Reis

*/

#include <cstdio>
#include <string>

using std::string;

class No
{
    friend class ArvBinBusca;

public:
    No(const int chave);

    void escreve(const char *sep = "");

private:
    int chave;
    No *pai;
    No *esq;
    No *dir;
};

class ArvBinBusca
{
public:
    ArvBinBusca();
    ArvBinBusca(const ArvBinBusca &outro); // construtor de cópia
    ~ArvBinBusca();
    ArvBinBusca &operator=(const ArvBinBusca &outro); // operador de atribuição

    void escreve_ordenado(); // escreve em percurso em-ordem
    void escreve();

    No *get_raiz();         // devolve a raiz
    No *busca(int k);       // devolve o ponteiro para um elemento, se achar, ou NULL
    No *minimo();           // devolve o menor elemento da árvore
    No *maximo();           // devolve o maior elemento da árvore
    No *sucessor(No *x);    // devolve o sucessor de um elemento
    No *predecessor(No *x); // devolve o predecessor de um elemento

    void insere(int chave); // insere um elemento
    bool remove(int chave); // busca e remove um elemento

    void limpa(); // remove todos elementos da árvore, liberando a memoria

private:
    No *raiz;

    void escreve_ordenado(No *x); // escreve em percurso em-ordem
    void escreve(const string &prefixo, No *x);

    No *busca(No *x, int k); // Parte recursiva da busca
    No *minimo(No *x);       // Parte recursiva de minimo
    No *maximo(No *x);       // Parte recursiva de maximo

    void insere(No *z);             // Insercao, mas ja recebe pronto o no (nao a chave) a ser inserido
    void transplante(No *u, No *v); // Transplante
    void remove(No *z);             // Recebe o no a ser removido (deve tratar todos os casos e fazer transplante, se necessario)

    void limpa(No *x); // dado um nó x, remove recursivamente elementos abaixo e deleta x

    void copia(const ArvBinBusca &T); // copia uma árvore T para a atual a partir da raiz,
                                      // usada no construtor de cópia e no operador de atribuição
    void copia(No *dest, No *orig);   // copia um nó e os descendentes recursivamente
};

int main(void)
{
    ArvBinBusca T; // construtor ArvBinBusca()
    int v[] = {10, 25, 0, 16, 20, 9, 15, 6, 14, 7, 18, 12, 22, 19, 3, 13};

    for (const auto &x : v)
        T.insere(x); // TODO: implemente primeiro o metodo insere

    printf("T:\n");
    T.escreve();
    printf("Valores de T em ordem crescente: ");
    T.escreve_ordenado(); // TODO: implemente depois escreve_ordenado

    No *raiz = T.get_raiz();
    printf("Raiz: ");
    raiz->escreve("\n");

    No *pre = T.predecessor(raiz);
    printf("Predecessor da raiz: ");
    pre->escreve("\n");
    No *suc = T.sucessor(raiz);
    printf("Sucessor da raiz: ");
    suc->escreve("\n");

    printf("Sucessor do predecessor da raiz (== raiz): ");
    T.sucessor(pre)->escreve("\n");
    printf("Predecessor do sucessor da raiz (== raiz): ");
    T.predecessor(suc)->escreve("\n");

    No *minimo = T.minimo();
    No *maximo = T.maximo();
    printf("Mínimo: ");
    minimo->escreve("\n");
    printf("Máximo: ");
    maximo->escreve("\n");

    T.remove(0);  // Caso 1
    T.remove(13); // Caso 2
    T.remove(10); // Caso 3b + 3a

    printf("T:\n");
    T.escreve();


    ArvBinBusca T2(T); // construtor de cópia
    T2.insere(30);
    printf("T:\n");
    T.escreve();
    printf("T2:\n");
    T2.escreve();

    ArvBinBusca T3 = T; // construtor de cópia
    T3.insere(-8);
    printf("T:\n");
    T.escreve();
    printf("T3:\n");
    T3.escreve();

    T3 = T; // operador de atribuição
    T3.insere(100);
    printf("T:\n");
    T.escreve();
    printf("T3:\n");
    T3.escreve();

    return 0;
}

//***********************************
//*** IMPLEMENTAÇÕES DA CLASSE NO ***
//***********************************

No::No(const int chave) : chave(chave),
                          pai(NULL),
                          esq(NULL),
                          dir(NULL)
{
}

void No::escreve(const char *sep)
{
    printf("%2d%s", chave, sep);
}

//********************************************
//*** IMPLEMENTAÇÕES DA CLASSE ARVBINBUSCA ***
//********************************************

ArvBinBusca::ArvBinBusca()
{
    raiz = NULL;
}

ArvBinBusca::ArvBinBusca(const ArvBinBusca &outro)
{
    copia(outro);
}

ArvBinBusca::~ArvBinBusca()
{
    limpa();
}

ArvBinBusca &ArvBinBusca::operator=(const ArvBinBusca &outro)
{
    limpa();
    copia(outro);
    return *this;
}

void ArvBinBusca::escreve_ordenado()
{
    escreve_ordenado(raiz);
    putchar('\n');
}

void ArvBinBusca::escreve_ordenado(No *x)
{
    // TODO: implementar (escrever em percurso em-ordem em uma única linha)
    if (x != NULL)
    {
        escreve_ordenado(x->esq);
        x->escreve(" ");
        escreve_ordenado(x->dir);
    }
}

void ArvBinBusca::escreve()
{
    escreve("", raiz);
}

void ArvBinBusca::escreve(const string &prefixo, No *x)
{
    if (x == NULL)
        return;

    bool ehDireito = x->pai and x->pai->dir == x;
    bool temIrmaoEsq = x->pai and x->pai->esq;

    printf("%s", prefixo.c_str());
    printf(ehDireito and temIrmaoEsq ? "├──" : "└──");

    if (x->pai == NULL) // raiz
        x->escreve("\n");
    else
        x->escreve(ehDireito ? "d\n" : "e\n");

    escreve(prefixo + (ehDireito and temIrmaoEsq ? "│   " : "    "), x->dir);
    escreve(prefixo + (ehDireito and temIrmaoEsq ? "│   " : "    "), x->esq);
}

No *ArvBinBusca::get_raiz()
{
    return raiz;
}

No *ArvBinBusca::busca(int k)
{
    return busca(raiz, k);
}

No *ArvBinBusca::busca(No *x, int k)
{
    // TODO: implementar
    if (x == NULL || k == x->chave)
    {
        return x;
    }

    if (k < x->chave)
    {
        return busca(x->esq, k);
    }
    else
    {
        return busca(x->dir, k);
    }
}

No *ArvBinBusca::minimo()
{
    return raiz ? minimo(raiz) : NULL;
}

No *ArvBinBusca::minimo(No *x)
{
    // TODO: implementar
    while (x->esq != NULL)
    {
        x = x->esq;
    }
    return x;
}

No *ArvBinBusca::maximo()
{
    return raiz ? maximo(raiz) : NULL;
}

No *ArvBinBusca::maximo(No *x)
{
    // TODO: implementar
    while (x->dir != NULL)
    {
        x = x->dir;
    }
    return x;
}

No *ArvBinBusca::sucessor(No *x)
{
    // TODO: implementar
    No *y;
    if (x->dir != NULL)
    {
        return minimo(x->dir);
    }
    y = x->pai;

    while (y != NULL && x == y->dir)
    {
        x = y;
        y = y->pai;
    }

    return y;
}

No *ArvBinBusca::predecessor(No *x)
{
    // TODO: implementar
    No *y;
    if (x->esq != NULL)
    {
        return maximo(x->esq);
    }
    y = x->pai;
    while (y != NULL && x == y->esq)
    {
        x = y;
        y = y->pai;
    }

    return y;
}

void ArvBinBusca::insere(int chave)
{
    No *z = new No(chave);
    insere(z);
}

void ArvBinBusca::insere(No *z)
{
    // TODO: implementar
    No *novoNo = z;
    No *noAtual = NULL;
    No *noPai = raiz;

    while (noPai != NULL)
    {
        noAtual = noPai;
        if (novoNo->chave < noPai->chave)
            noPai = noPai->esq;
        else
            noPai = noPai->dir;
    }

    novoNo->pai = noAtual;

    if (noAtual == NULL)
        raiz = novoNo;
    else if (novoNo->chave < noAtual->chave)
        noAtual->esq = novoNo;
    else
        noAtual->dir = novoNo;
}

void ArvBinBusca::transplante(No *u, No *v)
{
    // TODO: implementar
    if (u->pai == NULL)
    {
        raiz = v;
    }
    else if (u == u->pai->esq)
    {
        u->pai->esq = v;
    }
    else
    {
        u->pai->dir = v;
    }
    if (v != NULL)
    {
        v->pai = u->pai;
    }
}

bool ArvBinBusca::remove(int chave)
{
    No *z = busca(raiz, chave);
    if (z == NULL)
        return false;

    remove(z);
    delete z;
    return true;
}

void ArvBinBusca::remove(No *z)
{
    // TODO: implementar
    if (z->esq == NULL)
    {
        transplante(z, z->dir);
    }
    else if (z->dir == NULL)
    {
        transplante(z, z->esq);
    }
    else
    {
        No *y = minimo(z->dir);
        if (y->pai != z)
        {
            transplante(y, y->dir);
            y->dir = z->dir;
            y->dir->pai = y;
        }
        transplante(z, y);
        y->esq = z->esq;
        y->esq->pai = y;
    }
}

void ArvBinBusca::limpa()
{
    limpa(raiz);
    raiz = NULL;
}

void ArvBinBusca::limpa(No *x)
{
    // TODO: implementar
    if (x != NULL)
    {
        limpa(x->esq);
        limpa(x->dir);
        delete x;
    }
}

void ArvBinBusca::copia(const ArvBinBusca &T)
{
    if (T.raiz == NULL)
        raiz = NULL;
    else
    {
        raiz = new No(T.raiz->chave);
        copia(raiz, T.raiz);
    }
}

void ArvBinBusca::copia(No *dest, No *orig)
{
    // TODO: implementar
    if (orig == NULL)
        return;

    dest->chave = orig->chave;

    if (orig->esq != NULL)
    {
        dest->esq = new No(orig->esq->chave);
        dest->esq->pai = dest;
        copia(dest->esq, orig->esq);
    }

    if (orig->dir != NULL)
    {
        dest->dir = new No(orig->dir->chave);
        dest->dir->pai = dest;
        copia(dest->dir, orig->dir);
    }
}
