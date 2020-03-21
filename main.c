#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>                               
#include <stdlib.h>
#include <string.h>

#include "lista.h"
#include "util.h"

int show(int);
int teste1();
int teste2();

// as funcoes do menu
Lista*  m_apagar(Lista*);
Lista*  m_excluir_faixa(Lista*);
Lista*  m_incluir_faixa(Lista*);
int     m_mostrar(Lista*);
void    m_salvar(Lista*);
Lista*  m_somar(Lista*);
Lista*  m_trocar(Lista*);

int main(int argc, char** argv)
{
    int nMusicas = 0;
    fabrica(0);
    Lista* playlist = NULL;
    char c = 0;
    int n = 0;
    do
    {   c = show(nMusicas);
        switch (c)
        {
        case '0':
            return 0;
        case '1':
            playlist = m_incluir_faixa(playlist);
            break;
        case '2':
            playlist = m_excluir_faixa(playlist);
            break;
        case '3':
            nMusicas = m_mostrar(playlist);
            break;
        case '4':
            playlist = m_apagar(playlist);
            if (playlist == NULL) nMusicas = 0;
            break;
        case '5':
            m_salvar(playlist);
            break;
        case '6':
            playlist = m_somar(playlist);
            break;
        case '7':
            playlist = m_apagar(playlist);
            playlist = m_somar(playlist);
            break;
        case 'x':
            // cria e insere 10 musicas
            for (int i = 0; i < 10; i = i + 1)
                playlist = inserir((void*)fabrica(1), playlist);
            nMusicas = nMusicas + 10;
            break;
        default:
            nMusicas = m_mostrar(playlist);
            break;
        };  // switch()
    } while (c != 'q'); // while()
    return 0;
};  // main()


int show(int nMusicas)
{
    const char* v_programa = "PlayList";
    const char* v_versao = "v 0.90";
    cls();
    printf("\n%s %s\n", v_programa, v_versao);
    // temos pressa entao o menu vem de menu.txt e vai para tela
    const char* arq_entrada = "menu.txt";
    char linha[71];
    char* lido = NULL;
    char opt = 0;
    FILE* entrada;
    entrada = fopen(arq_entrada, "r");
    if (entrada == NULL)
    {
        printf("Erro tentando abrir menu '%s'\n", arq_entrada);
        return 'q';
    };  // if()
    // agora le os caras e poe na tela
    do
    {   lido = fgets(linha, 70, entrada);
        if(lido != NULL)printf("%s", linha);
    } while (lido != NULL);
    fclose(entrada);
    printf("Sua Opcao:_");
    opt = getc(stdin);
    return opt;
};  // show()

// um teste mais simples
// cria e lista 10 musicas
int  teste1()
{
    int n;
    fabrica(0); // inicia a fabrica de musicas
    Lista* playlist = NULL;
    // cria e insere 10 musicas
    for (int i = 0; i < 10; i = i + 1) playlist = inserir((void*)fabrica(1), playlist);
    n = listar(playlist);
    printf("listar() retornou %d\n", n);
    playlist = apagar(playlist);
    n = listar(playlist);
    return 0;
};  // teste1()


int teste2()
{
    int n;
    fabrica(0);
    Lista* playlist = NULL;
    playlist = inserir_do_arquivo("gabarito.txt", playlist);
    n = listar(playlist);
    printf("listar() retornou %d\n", n);

    // insere na playlist
    playlist = inserir_na_playlist(fabrica(1), playlist);
    n = listar(playlist);
    printf("listar() retornou %d\n", n);

    // apaga a musica 10 que nao tem
    playlist = apagar_da_playlist(9, playlist);
    n = listar(playlist);
    printf("listar() retornou %d\n", n);

    // apaga a segunda musica
    playlist = apagar_da_playlist(1, playlist);
    n = listar(playlist);
    printf("listar() retornou %d\n", n);

    // apaga a primeira musica
    playlist = apagar_da_playlist(0, playlist);
    n = listar(playlist);
    printf("listar() retornou %d\n", n);

    // listar() acaba de reporta n musicas
    // entao apaga todas
    int total = n;
    for (int x = 0; x < total; x = x + 1)
    {   // apaga a primeira musica
        playlist = apagar_da_playlist(0, playlist);
        n = listar(playlist);
        printf("listar() retornou %d\n", n);
    };  // for

    // agora cria umas musicas e grava na saida
    for (int i = 0; i < 10; i = i + 1) playlist = inserir((void*)fabrica(1), playlist);
    n = listar(playlist);
    printf("listar() retornou %d\n", n);

    n = gravar_no_arquivo("teste.txt", playlist);
    printf("gravar_no_arquivo() retornou %d\n", n);

    // e apaga a playlist aqui
    playlist = apagar(playlist);
    n = listar(playlist);

    return 0;
};  // teste2()

Lista* m_apagar(Lista* playlist)
{
    char arquivo[50];
    char* pStr = arquivo;
    int n = 0;
    flush();
    printf("Entre 's' ou 'S' para apagar a playlist: ");
    pStr = fgets(pStr, 2, stdin);
    if ((*pStr != 's') && (*pStr != 's')) return playlist;
    playlist = apagar(playlist);
    return playlist;
};  // m_apagar() 


Lista* m_excluir_faixa(Lista* playlist)
{
    int ch = 0;
    char arquivo[50];
    char* pStr = arquivo;
    int n = 0;
    flush();
    printf("Numero da Faixa que quer apagar: ");
    ch = scanf(" %d", &n);
    if (ch != 1) return playlist;
    if (n < 1) return playlist;
    playlist = apagar_da_playlist(n-1,playlist);
    return playlist;
};  // m_excluir_faixa()


Lista* m_incluir_faixa(Lista* playlist)
{
    // esse e o mais chato mas similar a incluir do arquivo
    // afinal e a mesma coisa entao vamos copiar de la
    // prepara as variaveis
    int n = 0;
    // o que vai ler de cada linha:
    char* pStr = NULL;
    char musica[TAMANHO + 1];
    char artista[TAMANHO + 1];
    int min = 0;
    int seg = 0;

    // tudo certo agora le a faixa
    flush();
    printf("Faixa [%d letras MAX]:", TAMANHO);
    memset(musica, 0, TAMANHO + 1);
    pStr = musica;
    pStr = fgets(pStr, TAMANHO+1, stdin);
    n = strlen(pStr);
    if (n<1) return playlist;
    if(n<TAMANHO)
        pStr[n-1] = 0;
    else
        flush();

    // tudo certo agora le o artista
    printf("Faixa:\t'%s'\n", pStr);
    memset(artista, 0, TAMANHO + 1);
    printf("Artista [%d letras MAX]:", TAMANHO);
    pStr = artista;
    pStr = fgets(pStr, TAMANHO+1, stdin);
    n = strlen(pStr);
    if (n < 1) return playlist;
    if (n<TAMANHO)
        pStr[n-1] = 0;
    else
        flush();

    // ok agora le a duracao: minutos
    printf("Faixa:  \t'%s'\n", musica);
    printf("Artista:\t'%s'\n", artista);
    printf("Duracao da faixa: os minutos:");
    n = scanf("%d", &min);
    if (n != 1) return playlist;
    printf("Faixa:  \t'%s'\n", musica);
    printf("Artista:\t'%s'\n", artista);
    printf("Min:\t%02d\n", min);

    // ok agora le a duracao: segundos
    flush();
    printf("Duracao da faixa: os segundos:");
    n = scanf(" %d", &seg);
    if (n != 1) return playlist;
    if (seg > 59) return playlist;
    printf("Faixa:  \t'%s'\n", musica);
    printf("Artista:\t'%s'\n", artista);
    printf("Duracao:\t%02d:%02d\n", min,seg);
    
    // preenche o resto do nome com brancos
    n = strlen(musica);
    if (n <= TAMANHO)
    {   
        int q = TAMANHO - n;
        memset(musica+n, 32, q);
    };  // if()

    // preenche o resto do nome do artista com brancos
    n = strlen(artista);
    if (n <= TAMANHO)
    {   
        int q = TAMANHO - n;
        memset(artista+n, 32, q);
    };  // if()

    // finalmente insere
    Musica* nova = (Musica*)malloc(sizeof(Musica)); // cria a faixa
    memcpy(nova->Faixa, musica, TAMANHO);
    memcpy(nova->Artista, artista, TAMANHO);
    nova->min = min;
    nova->seg = seg;
    playlist = inserir((void*)(nova), playlist); // insere a musica
    return playlist;
};  // m_incluir_faixa()

int m_mostrar(Lista* playlist)
{
    int n = 0;
    cls();
    n = listar(playlist);
    if(n>0) printf("[%d musica (s)]\n\n", n);
    pause();
    return n;
};

void m_salvar(Lista* playlist)
{   // salva a playlist em disco
    char arquivo[50];
    char* pStr = arquivo;
    int n = 0;
    flush();
    printf("Arquivo a ser gravado: ");
    pStr = fgets(pStr, 40, stdin);
    printf("Arquivo: %s len = %d\n", arquivo, strlen(arquivo));
    arquivo[strlen(arquivo) - 1] = 0;
    n = gravar_no_arquivo(arquivo, playlist);
    printf("gravar_no_arquivo() retornou %d\n", n);
    return;
};

Lista* m_somar(Lista* playlist)
{   // le do disco faixas para a lista
    char arquivo[50];
    char* pStr = arquivo;
    flush();
    printf("Arquivo com as faixas: ");
    pStr = fgets(pStr, 40, stdin);
    printf("Arquivo: %s len = %d\n", arquivo, strlen(arquivo));
    arquivo[strlen(arquivo) - 1] = 0;
    playlist = inserir_do_arquivo(arquivo, playlist);
    return playlist;
};

Lista* m_trocar(Lista* playlist)
{
    return playlist;
};
