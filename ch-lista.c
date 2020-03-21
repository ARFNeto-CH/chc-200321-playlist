#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>                               
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define TAMANHO 15

// so o que temos: a musica e a lista 
struct musica
{
    char    Faixa[TAMANHO];
    char    Artista[TAMANHO];
    int     min;
    int     seg;
};  // musica
typedef struct musica Musica;

struct no
{
    void* dados;
    struct no* proxima;
};  // no
typedef struct no Lista;

// as funcoes que escrevemos primeiro

Lista*  apagar(Lista*);
Musica* fabrica(int);
Lista*  inserir(void*, Lista*);
int     listar(Lista*);

// fase 2 :) bonus gravar e ler do disco e editar playlist

Lista*  apagar_da_playlist(int, Lista*);
int     gravar_no_arquivo(char*, Lista*);
Lista*  inserir_do_arquivo(char*, Lista*);
Lista*  inserir_na_playlist(Musica*, Lista*);
void    mostra_musica(Musica*, char*);

// as funcoes do menu, que escrevemos DEPOIS que as 
// declaradas acima estavam ok

Lista*  m_apagar(Lista*);
Lista*  m_excluir_faixa(Lista*);
Lista*  m_incluir_faixa(Lista*);
int     m_mostrar(Lista*);
void    m_salvar(Lista*);
int     m_show(int);
Lista*  m_somar(Lista*);

// as funcoes chatas para a console ja que precisamos do
// menu: cls() pause() e flush() para Windows 
// funces para a console. Escrevi muitas outras mas nao
// vou arrastar pra ca
int			cls();
void        flush();
void		pause();

int main(int argc, char** argv)
{
    int nMusicas = 0;
    fabrica(0);
    Lista* playlist = NULL;
    char c = 0;
    int n = 0;
    do
    {
        c = m_show(nMusicas);
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

Lista*  m_apagar(Lista* playlist)
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

Lista*  m_excluir_faixa(Lista* playlist)
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
    playlist = apagar_da_playlist(n - 1, playlist);
    return playlist;
};  // m_excluir_faixa()

Lista*  m_incluir_faixa(Lista* playlist)
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
    pStr = fgets(pStr, TAMANHO + 1, stdin);
    n = strlen(pStr);
    if (n < 1) return playlist;
    if (n < TAMANHO)
        pStr[n - 1] = 0;
    else
        flush();

    // tudo certo agora le o artista
    printf("Faixa:\t'%s'\n", pStr);
    memset(artista, 0, TAMANHO + 1);
    printf("Artista [%d letras MAX]:", TAMANHO);
    pStr = artista;
    pStr = fgets(pStr, TAMANHO + 1, stdin);
    n = strlen(pStr);
    if (n < 1) return playlist;
    if (n < TAMANHO)
        pStr[n - 1] = 0;
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
    printf("Duracao:\t%02d:%02d\n", min, seg);

    // preenche o resto do nome com brancos
    n = strlen(musica);
    if (n <= TAMANHO)
    {
        int q = TAMANHO - n;
        memset(musica + n, 32, q);
    };  // if()

    // preenche o resto do nome do artista com brancos
    n = strlen(artista);
    if (n <= TAMANHO)
    {
        int q = TAMANHO - n;
        memset(artista + n, 32, q);
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

int     m_mostrar(Lista* playlist)
{
    int n = 0;
    cls();
    n = listar(playlist);
    if (n > 0) printf("[%d musica (s)]\n\n", n);
    pause();
    return n;
};

void    m_salvar(Lista* playlist)
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

int     m_show(int nMusicas)
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
    {
        lido = fgets(linha, 70, entrada);
        if (lido != NULL)printf("%s", linha);
    } while (lido != NULL);
    fclose(entrada);
    printf("Sua Opcao:_");
    opt = getc(stdin);
    return opt;
};  // show()

Lista*  m_somar(Lista* playlist)
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

//////////////////////////////////////////////////////////////////////

// as funcoes para a console

int		cls()
{	// limpa a tela no windows, do jeito oficial
    CONSOLE_SCREEN_BUFFER_INFO		info;
    HANDLE		H = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD		origem = { 0,0 };
    int			total;
    if (H == INVALID_HANDLE_VALUE) return -1;
    GetConsoleScreenBufferInfo(H, &info);
    FillConsoleOutputCharacter(H, (TCHAR)' ',
        info.dwSize.X * info.dwSize.Y,
        origem, (LPDWORD)&total);
    FillConsoleOutputAttribute(
        H, info.wAttributes,
        info.dwSize.X * info.dwSize.Y,
        origem, (LPDWORD)&total);
    SetConsoleCursorPosition(H, origem);
    return 0;
};	// end cls()

void    flush()
{
    int ch = 0;
    do { ch = getc(stdin); } while ((ch != '\n') && (ch != EOF));
};	// flush()

void    pause()
{
    int ch = 0;
    do { ch = getc(stdin); } while (ch != '\n');
    printf("Tecle ENTER para prosseguir...");
    do { ch = getc(stdin); } while (ch != '\n');
};  // pause()

//////////////////////////////////////////////////////////////////////

// as funcoes da playlist

Lista*  apagar(Lista* l)
{   // apagar e como listar()
    if (l == NULL)
    {
        printf("Lista nao iniciada\n");
        return NULL;
    }
    if (l->dados == NULL)
    {
        printf("Lista vazia\n");
        return NULL;
    }
    // nao esta vazia entao varre a lista e apaga 
    // os caras todos
    Lista* aux = l;
    Lista* temp = NULL;
    do
    {
        temp = aux->proxima; // salva o endereco da proxima
        free(aux->dados); // apaga essa musica
        free(aux); // apaga o no da lista
        aux = temp;
    } while (aux != NULL); // do-while()
    return NULL;
};  // apagar()

Musica* fabrica(int parm)
{
    // a cada chamada retorna uma musica novinha
    static int n = 0;
    if (parm == 0)
    {
        srand(200401);
        n = 1;
        return NULL;
    }
    Musica* m = (Musica*)malloc(sizeof(Musica));
    sprintf(m->Faixa, "Musica 1234 %03d", n);
    sprintf(m->Artista, "Artista ABC %03d", n);
    m->min = rand() % 10;
    m->seg = rand() % 60;
    n = n + 1;
    return m;
};  // fabrica()

Lista*  inserir(void* valor, Lista* l)
{   // insere dados: musica nesse caso
    //printf("***** inserir() *****\n");
    // cria um elemento
    Lista* novo = (Lista*)malloc(sizeof(Lista));
    novo->proxima = l;
    novo->dados = valor;
    return novo;
};  // inserir()

int     listar(Lista* l)
{
    int total = 0;
    Lista* aux = l;
    if (l == NULL)
    {
        printf("Lista nao iniciada\n");
        return -1;
    }
    if (l->dados == NULL)
    {
        printf("Lista vazia\n");
        return 0;
    }
    // nao esta vazia entao cria um ponteiro para
    // o tipo de dados que estamos usando

    printf(
        "              #            faixa            artista    HH:MM\n\n"
    );
    do
    {
        total = total + 1;
        char pos[TAMANHO];
        sprintf(pos, "%14d", total);
        mostra_musica(aux->dados, pos);
        aux = aux->proxima;
    } while (aux != NULL); // do-while()
    printf("\n");
    return total;
};  // listar()

// fase 2 :)
Lista*  apagar_da_playlist(int n, Lista* playlist)
{   // baseado em listar()
    int pos = 0;
    Lista* pL = playlist;
    Lista* pAnt = NULL; // vai apontar para a faixa anterior
    if (pL == NULL) return pL; // vazio
    if (pL->dados == NULL) return pL; // vai que...
    // agora sim vamos procurar
    do
    {   // procura a faixa por posicao
        if (pos == n)
        {   // vai apagar essa
            if (pAnt != NULL)
            {   // tinha uma musica antes dessa
                pAnt->proxima = pL->proxima;
                free(pL->dados);
                free(pL);
                return playlist;
            }
            else
            {   // vai apagar a primeira
                playlist = pL->proxima;
                free(pL->dados);
                free(pL);
                return playlist;
            };  // if()
        };  // if()
        pos = pos + 1; // pos atual
        pAnt = pL;
        pL = pL->proxima;
    } while (pL != NULL); // do-while()
    return playlist; // nao achou tal faixa
};  // apagar_da_playlist()

int     gravar_no_arquivo(char* arquivo, Lista* playlist)
{
    // vai gravar em 'arquivo' a playlist em 'playlist'
    int n = 0;
    FILE* saida;
    saida = fopen(arquivo, "w");
    if (saida == NULL)
    {
        printf("Erro tentando criar '%s'\n", arquivo);
        return -1;
    };  // if()
    // se a playlist esta vazia faz o simples
    if (playlist == NULL)
    {
        fclose(saida);
        return 0; // sim, normal. So esta vazia
    };  // if()
    if (playlist->dados == NULL)
    {
        fclose(saida);
        return 0; // sim, normal. So esta vazia
    };  // if()
    // tem musicas, e o arquivo esta aberto
    // e como listar() so que ao inves de mostrar na tela grava no disco
    // entao cou copiar o codigo de listar() que ja esta ok
    int total = 0;
    Lista* aux = playlist;
    Musica* m = NULL;
    // faixa e artista nao sao strings...
    char musica[TAMANHO + 1];
    char artista[TAMANHO + 1];
    memset(musica, 0, TAMANHO + 1);
    memset(artista, 0, TAMANHO + 1);
    do
    {
        total = total + 1;
        m = (Musica*)aux->dados;
        memcpy(musica, m->Faixa, TAMANHO); // ja tem o 0 no fim
        memcpy(artista, m->Artista, TAMANHO);
        n = fprintf(saida, "%15s %15s %02d %02d\n",
            musica, artista, m->min, m->seg); // 15 + 15 + 2 + 2 + 3 espacos e o \n = 38
        aux = aux->proxima;
    } while (aux != NULL); // do-while()
    fclose(saida);
    return total;
};

Lista*  inserir_do_arquivo(char* arquivo, Lista* playlist_inicial)
{
    const char* mascara = "%15c%*1c%15c%*1c%2d%*1c%2d%*1c";
    const char* arq_entrada = arquivo;
    int n = 0;
    Lista* pL = playlist_inicial; // a playlist no comeco
    FILE* entrada;
    // o que vai ler de cada linha
    char musica[TAMANHO + 1];
    char artista[TAMANHO + 1];
    int min = 0;
    int seg = 0;
    // zera os campos de musica e artista
    memset(musica, 0, TAMANHO + 1);
    memset(artista, 0, TAMANHO + 1);
    entrada = fopen(arq_entrada, "r");
    if (entrada == NULL)
    {
        printf("Erro tentando abrir '%s'\n", arq_entrada);
        return pL;
    };  // if()
    // agora le os caras e poe na lista
    while (!feof(entrada))
    {
        n = fscanf(entrada, mascara, musica, artista, &min, &seg);
        if (n != 4) break;
        // leu ok entao cria a musica e poe na lista
        // atencao: nome e artista nao sao strings
        Musica* nova = (Musica*)malloc(sizeof(Musica)); // cria a faixa
        memcpy(nova->Faixa, musica, TAMANHO);
        memcpy(nova->Artista, artista, TAMANHO);
        nova->min = min;
        nova->seg = seg;
        pL = inserir((void*)(nova), pL); // insere a musica
    }   // while()
    fclose(entrada);
    return pL;
};  // inserir_do_arquivo()

Lista*  inserir_na_playlist(Musica* musica, Lista* playlist)
{
    // inserir na playlist eh algo besta. Como nao tem qualquer restricao
    // tipo duplicar musicas ou inserir em certa posicao, e igual a inserir()
    if (musica == NULL) return playlist; // nao tinha musica
    Lista* pL = (Lista*)malloc(sizeof(Lista)); // cria o registro na lista
    Musica* nova = (Musica*)malloc(sizeof(Musica)); // cria a faixa
    memcpy(nova->Artista, musica->Artista, TAMANHO);
    memcpy(nova->Faixa, musica->Faixa, TAMANHO);
    nova->min = musica->min;
    nova->seg = musica->seg;
    // em *nova esta a musica nova. grava em *pL
    pL->proxima = playlist; // insere no inicio
    pL->dados = (void*)nova;
    return pL; // novo inicio
    return NULL;
};  // inserir_na_playlist()

void    mostra_musica(Musica* m, char* titulo)
{
    char    t_musica[TAMANHO + 1];
    char    t_artista[TAMANHO + 1];
    t_musica[TAMANHO] = 0;
    t_artista[TAMANHO] = 0;
    memcpy(t_musica, m->Faixa, TAMANHO);
    memcpy(t_artista, m->Artista, TAMANHO);

    printf(
        "%15s: %15s    %15s    %02d:%02d\n",
        titulo,
        t_musica,
        t_artista,
        m->min,
        m->seg
    );
    return;
};  // mostra_musica()

    // fim do texto