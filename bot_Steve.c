/********************************************************************
  Bot-exemplo

  Após receber as informações iniciais do jogo, a cada rodada esse
  bot irá se movimentar para esquerda.
  Cabe a você agora aprimorar sua estratégia!!!
 ********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define MAX_STR 50
/* ADAPTAR EM FUNÇÃO DE COMO OS DADOS SERÃO ARMAZENADOS NO SEU BOT */

typedef struct Dados
{
  char id[MAX_STR];
  int alturaBot;
  int larguraBot;
  int bolsa;
  int widthPesca;
  int heightPesca;

} Dados;

typedef struct Porto
{
  int posX;
  int posY;
} Porto;

void Vender(Dados *barco)
{
  printf("SELL\n");
}

void Pescar(Dados *barco)
{

  printf("FISH\n");
}

void ladoesquerdo(Dados *barco)
{

  printf("LEFT\n");
}

void ladodireito(Dados *barco)
{

  printf("RIGHT\n");
}

void ladoemaixo(Dados *barco)
{

  printf("DOWN\n");
}

void ladoemcima(Dados *barco)
{
  printf("UP\n");
}

bool truefish(int **areapesca, int objetivowidth, int objetivoheight)
{
  int areabotvar = areapesca[objetivoheight][objetivowidth];

  if (areabotvar > 11 && areabotvar < 40)
  {
    return true;
  }

  return false;
}

void mudardados(int **areapesca, int w)
{
  for (int i = 0; i < w; i++)
  {
    free(areapesca[i]);
  }
  free(areapesca);
}

void andarlinha(Dados barco, bool pescabool, Porto porto)
{
  if (pescabool)
  {
    if (barco.larguraBot < barco.widthPesca)
    {
      ladodireito(&barco);
    }
    else
    {
      ladoesquerdo(&barco);
    }
  }
  else
  {
    if (barco.larguraBot < porto.posX)
    {
      ladodireito(&barco);
    }
    else
    {
      ladoesquerdo(&barco);
    }
  }
}

void andarcoluna(Dados barco, bool pescabool, Porto porto)
{
  if (pescabool)
  {
    if (barco.alturaBot < barco.heightPesca)
    {
      ladoemaixo(&barco);
    }
    else
    {
      ladoemcima(&barco);
    }
  }
  else
  {
    if (barco.alturaBot < porto.posY)
    {
      ladoemaixo(&barco);
    }
    else
    {
      ladoemcima(&barco);
    }
  }
}

void nextfish(int **areapesca, Dados *barco, int h, int w)
{
  bool spawnpeixe = truefish(areapesca, barco->larguraBot, barco->alturaBot);
  barco->widthPesca = -1;
  barco->heightPesca = -1;

  int caminhoaltura = h - 1;

  int caminholargura = w - 1;

  int contador = 1;

  if (spawnpeixe && truefish(areapesca, barco->larguraBot, barco->alturaBot))
  {
    barco->widthPesca = barco->larguraBot;
    barco->heightPesca = barco->alturaBot;
    return;
  }
  else
  {
    while (barco->heightPesca == -1 || barco->widthPesca == -1)
    {
      if (!(barco->alturaBot - contador < 0 || barco->alturaBot - contador > caminhoaltura))
      {
        if (truefish(areapesca, barco->larguraBot, barco->alturaBot - contador))
        {
          barco->widthPesca = barco->larguraBot;
          barco->heightPesca = barco->alturaBot - contador;

          return;
        }
      }
      if (!(barco->larguraBot + contador < 0 || barco->larguraBot + contador > caminholargura))
      {
        if (truefish(areapesca, barco->larguraBot + contador, barco->alturaBot))
        {
          barco->widthPesca = barco->larguraBot + contador;
          barco->heightPesca = barco->alturaBot;

          return;
        }
      }
      if (!(barco->alturaBot + contador < 0 || barco->alturaBot + contador > caminhoaltura))
      {
        if (truefish(areapesca, barco->larguraBot, barco->alturaBot + contador))
        {
          barco->widthPesca = barco->larguraBot;
          barco->heightPesca = barco->alturaBot + contador;

          return;
        }
      }
      if (!(barco->larguraBot - contador < 0 || barco->larguraBot - contador > caminholargura))
      {
        if (truefish(areapesca, barco->larguraBot - contador, barco->alturaBot))
        {
          barco->widthPesca = barco->larguraBot - contador;
          barco->heightPesca = barco->alturaBot;

          return;
        }
      }
      contador++;
    }
  }
}

void escolhaBot(Dados *barco, Porto *porto, int **areapesca, bool *pescabool)
{
  if (*pescabool && barco->bolsa < 10)
  {
    if (barco->larguraBot != barco->widthPesca)
    {
      andarlinha(*barco, *pescabool, *porto);
    }
    else if (barco->alturaBot != barco->heightPesca)
    {
      andarcoluna(*barco, *pescabool, *porto);
    }
    else
    {
      Pescar(barco);
      barco->bolsa++;
      if (barco->bolsa == 10)
      {
        *pescabool = false;
        barco->widthPesca = -1;
        barco->heightPesca = -1;
      }
    }
  }
  else
  {
    if (barco->larguraBot != porto->posX)
    {
      andarlinha(*barco, *pescabool, *porto);
    }
    else if (barco->alturaBot != porto->posY)
    {
      andarcoluna(*barco, *pescabool, *porto);
    }
    else
    {
      Vender(barco);
      barco->bolsa = 0;
      *pescabool = true;
    }
  }
}

void readData(int h, int w, int **areapesca, Dados *barco, Porto *porto, bool portoBool) //portobool = ver se o porto foi lido
{
  char id[MAX_STR];
  int v, n, x, y;
  // lê os dados da área de pesca
  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      scanf("%i", &v);
      areapesca[i][j] = v;
      if (areapesca[i][j] == 11 || areapesca[i][j] == 21 || areapesca[i][j] == 31)
      {
        areapesca[i][j] = 0;
      }
      if (!portoBool)
      {
        if (areapesca[i][j] == 1)
        {
          porto->posX = j;
          porto->posY = i;
        }
      }
    }
  }
  // lê os dados dos bots
  scanf(" BOTS %i", &n);
  // o " " antes de BOTS é necessário para ler o '\n' da linha anterior
  for (int i = 0; i < n; i++)
  {
    scanf("%s %i %i", id, &y, &x);
    if (strcmp(barco->id, id) == 0)
    {
      barco->alturaBot = y;
      barco->larguraBot = x;
      nextfish(areapesca, barco, h, w);
    }
  }
}

int main()
{
  char line[MAX_STR]; // dados temporários
  char myId[MAX_STR]; // identificador do bot em questão

  setbuf(stdin, NULL);  // stdin, stdout e stderr não terão buffers
  setbuf(stdout, NULL); // assim, nada é "guardado temporariamente"
  setbuf(stderr, NULL);
  srand(time(NULL));

  // === INÍCIO DA PARTIDA ===
  int h, w;
  scanf("AREA %i %i", &h, &w); // lê a dimensão da área de pesca: altura (h) x largura (w)
  int hmax = h;
  int wmax = w;
  scanf(" ID %s", myId); // ...e o id do bot
  // obs: o " " antes de ID é necessário para ler o '\n' da linha anterior

  // Para "debugar", é possível enviar dados para a saída de erro padrão (stderr).
  // Esse dado não será enviado para o simulador, apenas para o terminal.
  // A linha seguinte é um exemplo. Pode removê-la se desejar.
  int **areapesca = (int **)malloc(sizeof(int *) * h);
  for (int i = 0; i < w; i++)
    areapesca[i] = malloc(sizeof(int) * w);

  fprintf(stderr, "Meu id = %s\n", myId);

  Dados barco;
  Porto porto;
  bool portoBool = false;
  strcpy(barco.id, myId);
  barco.bolsa = 0; // bolsa = onde ficará os peixes
  barco.heightPesca = -1;
  barco.widthPesca = -1;

  bool pescabool = true;

  // === PARTIDA ===
  // O bot entra num laço infinito, mas não se preocupe porque o simulador irá matar
  // o processo quando o jogo terminar.

  // State state = GOING_TO_FISHING_POINT;
  while (1)
  {

    // LÊ OS DADOS DO JOGO E ATUALIZA OS DADOS DO BOT
    readData(h, w, areapesca, &barco, &porto, portoBool);
    bool portoBool = true;
    // INSIRA UMA LÓGICA PARA ESCOLHER UMA AÇÃO A SER EXECUTADA
    escolhaBot(&barco, &porto, areapesca, &pescabool);
    // lê qual foi o resultado da ação (e eventualmente atualiza os dados do bot).
    scanf("%s", line);
  }

  mudardados(areapesca, w);

  return 0;
}