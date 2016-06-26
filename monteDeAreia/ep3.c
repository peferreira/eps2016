#include <stdio.h>
#define MAX  128

/*funcoes pedidas no enunciado*/
void zere_tabuleiro(int tabuleiro[MAX][MAX], int nlin, int ncol);
void leia_configuracao_inicial(int tabuleiro[MAX][MAX], int *nlin, int *ncol);
void imprima_tabuleiro(int tabuleiro[MAX][MAX], int nlin, int ncol);
void copie_matriz(int origem[MAX][MAX], int destino[MAX][MAX],
                   int nlin, int ncol);
int espalhe(int tabuleiro[MAX][MAX], int ativacao[MAX][MAX],
             int nlin, int ncol, int instante, int *novosativados);


/*funcoes extras*/
int conta_num_graos(int tabuleiro[MAX][MAX], int nlin, int ncol);
void imprima_tabuleiro_borda(int tabuleiro[MAX+2][MAX+2], int nlin, int ncol);
void gera_tabuleiro_com_borda(int tabuleiro[MAX][MAX], int tabuleiroBORDA[MAX+2][MAX+2],int nlin, int ncol);
void retira_borda_tabuleiro(int tabuleiroBORDA[MAX+2][MAX+2], int tabuleiro[MAX][MAX],int nlin, int ncol);
void printf_tracejado(int ncol);
void printf_ncol(int ncol);
int conta_vizinhos(int nlin, int ncol, int i, int j);
void espalhe_celula(int tabuleiro[MAX][MAX], int i, int j, int nlin, int ncol);
void inicializa_tabuleiro(int tabuleiro[MAX][MAX], int nlin, int ncol, int valor);

/*recebe uma matriz atual, uma matriz para adicionar os valores de deslocamento
para posicoes instaveis uma posicao e devolve 1 se a posicao estiver instavel*/

/*cria matriz com borda -1*/


int main()
{
  int instante;
  int espalhamentos, instaveis;
  int novosativados, nelementos;
  int nlin, ncol;
  int tabuleiro[MAX][MAX];
  int ativacao[MAX][MAX];
  float frac;
  instaveis = 0;
  novosativados = espalhamentos = 0;
  zere_tabuleiro(tabuleiro,MAX,MAX);
  leia_configuracao_inicial(tabuleiro,&nlin,&ncol);
  inicializa_tabuleiro(ativacao,nlin,ncol, -1);
  nelementos = nlin*ncol;
  instante = 0;
  printf("Simulador de monte de areia\n");
  printf("Configuracao inicial\n");
  printf("Instante %d:\n", instante);
  imprima_tabuleiro(tabuleiro,nlin,ncol);
  instaveis = espalhe(tabuleiro, ativacao, nlin, ncol, instante, &novosativados);
  espalhamentos+= instaveis;
  while(nelementos - novosativados > 0 && instaveis > 0){
    instante++;
    printf("Instante %d:\n", instante);
    imprima_tabuleiro(tabuleiro,nlin,ncol);
    instaveis = 0;
    instaveis = espalhe(tabuleiro, ativacao, nlin, ncol, instante, &novosativados);
    espalhamentos += instaveis;
  }
  instante++;
  printf("Instante %d:\n", instante);
  imprima_tabuleiro(tabuleiro,nlin,ncol);
  frac = (nelementos-novosativados)/nelementos;
  printf("Tabuleiro %d x %d, %d graos.\n", nlin,ncol,conta_num_graos(tabuleiro,nlin,ncol));
  printf("Simulacao encerrada no instante %d.\n", instante);
  printf("Total de espalhamentos ao longo do processo: %d\n", espalhamentos);
  printf("Total de casas que nunca foram ativadas: %d (%.1f%%)\n",nelementos-novosativados,frac);
  if(nelementos - novosativados > 0){
    printf("Sistema finito.\n");
  } else{
    printf("Sistema infinito.\n");
  }
  printf("Configuracao quando detectado:\n");
  imprima_tabuleiro(tabuleiro,nlin,ncol);
  printf("Primeiro instante de ativacao de cada casa:\n");
  imprima_tabuleiro(ativacao,nlin,ncol);

  return 0;
}


int conta_num_graos(int tabuleiro[MAX][MAX], int nlin, int ncol){
  int i,j,num_graos;
  i = j = num_graos = 0;

  for (i = 0; i < nlin; i++)
  {
    for (j = 0; j < ncol; j++)
    {
      num_graos+= tabuleiro[i][j];
    }
  }
  return num_graos;
}


/*no comeco todos vizinhos estao habilitados
checa esq, dir, cim, baixo e desabilita caso necessario*/
void espalhe_celula(int tabuleiro[MAX][MAX], int i, int j, int nlin, int ncol)
{
    int esq,dir,cima,baixo;
    esq = dir = cima = baixo = 1;
    if(i == 0)
      cima = 0;
    if(j == 0)
      esq = 0;
    if(i == nlin-1)
      baixo = 0;
    if(j == ncol-1)
      dir = 0;
    if(dir == 1){
      tabuleiro[i][j+1]++;
      tabuleiro[i][j]--;
    }
    if(esq == 1){
      tabuleiro[i][j-1]++;
      tabuleiro[i][j]--;
    }
    if(cima == 1){
      tabuleiro[i-1][j]++;
      tabuleiro[i][j]--;
    }
    if(baixo == 1){
      tabuleiro[i+1][j]++;
      tabuleiro[i][j]--;
    }
}
int ativa_posicao(int ativacao[MAX][MAX], int i, int j, int instante)
{
  if(ativacao[i][j] == -1){
    ativacao[i][j] = instante;
    return 1;
  }
  return 0;
}
int espalhe(int tabuleiro[MAX][MAX], int ativacao[MAX][MAX],
             int nlin, int ncol, int instante, int *novosativados){
    int i,j,instaveis;
    int copia_tabuleiro[MAX][MAX];
    instaveis = 0;
    copie_matriz(tabuleiro,copia_tabuleiro, nlin, ncol);

    for(i = 0; i < nlin; i++)
    {
      for(j = 0; j < ncol; j++)
      {
          if(copia_tabuleiro[i][j] >= conta_vizinhos(nlin,ncol,i,j))
          {
            instaveis++;
            *novosativados += ativa_posicao(ativacao,i,j,instante);
            espalhe_celula(tabuleiro,i,j,nlin,ncol);
          }
      }
    }
    return instaveis;
}

int conta_vizinhos(int nlin, int ncol, int i, int j){
  int m,n,vizinhos;
  vizinhos = 0;
  m = n = 0;
  if(i == 0 || i == nlin-1)
      m=1;
  if(j == 0 || j == ncol-1)
      n=1;
  vizinhos = 4 - m - n;
  return vizinhos;
}

void gera_tabuleiro_com_borda(int tabuleiro[MAX][MAX], int tabuleiroBORDA[MAX+2][MAX+2], int nlin, int ncol){
  int i,j;
  i = 0; j = 0;
  for (i = 0; i < nlin+2; i++)
  {
    for (j = 0; j < ncol+2; j++)
    {
      if(i == 0 || j == 0 || i == nlin+1|| j == ncol+1){
          tabuleiroBORDA[i][j] = -1;
      }
      else{
        tabuleiroBORDA[i][j] = tabuleiro[i-1][j-1];
      }
    }
  }
}

void retira_borda_tabuleiro(int tabuleiroBORDA[MAX+2][MAX+2], int tabuleiro[MAX][MAX],int nlin, int ncol){
  int i,j;
  i = 0; j = 0;
  for (i = 0; i < nlin; i++)
  {
    for (j = 0; j < ncol; j++)
    {
      tabuleiro[i][j] = tabuleiroBORDA[i+1][j+1];
    }
  }
}
void inicializa_tabuleiro(int tabuleiro[MAX][MAX], int nlin, int ncol, int valor)
{
    int i,j;
    i = 0; j = 0;
    for (i = 0; i < nlin; i++)
    {
      for (j = 0; j < ncol; j++)
      {
        tabuleiro[i][j] = valor;
      }
    }
}


void zere_tabuleiro(int tabuleiro[MAX][MAX], int nlin, int ncol)
{
    inicializa_tabuleiro(tabuleiro, nlin, ncol, 0);
}


void imprima_tabuleiro(int tabuleiro[MAX][MAX], int nlin, int ncol)
{
  int i,j;
  i = 0; j = 0;
  printf_ncol(ncol);
  printf_tracejado(ncol);

  for (i = 0; i < nlin; i++)
  {
    if(i < 10){
      printf("  %d ",i);
    } else if(i < 100){
      printf("  %d ",i);
    } else {
      printf(" %d ",i);
    }
    for (j = 0; j < ncol; j++)
    {
      if(tabuleiro[i][j] < 10){
          printf("|  %d ", tabuleiro[i][j]);
      } else if (tabuleiro[i][j] < 100){
          printf("| %d ", tabuleiro[i][j]);
      } else{
          printf("|%d ", tabuleiro[i][j]);
      }
    }
    printf("|\n");
    printf_tracejado(ncol);
  }
  printf("\n");
}

void imprima_tabuleiro_borda(int tabuleiro[MAX+2][MAX+2], int nlin, int ncol)
{
  int i,j;
  i = 0; j = 0;
  for (i = 0; i < nlin+2; i++)
  {
    for (j = 0; j < ncol+2; j++)
    {
      printf("%d  ", tabuleiro[i][j]);
    }
    printf("\n");
  }
}

void printf_ncol(int ncol){
    int i;
    printf("    ");

    for (i = 0; i < ncol; i++){
        if(i < 10){
          printf("   %d ",i);
        } else if(i < 100){
          printf("  %d ",i);
        } else {
          printf(" %d ",i);
        }
    }
    printf("\n");
}

void printf_tracejado(int ncol){
    int i;
    printf("    +");
    for (i = 0; i < ncol; i++){
        printf("----+");
    }
    printf("\n");
}


void leia_configuracao_inicial(int tabuleiro[MAX][MAX], int *nlin, int *ncol)
{
  int i,j;
  scanf("%d", nlin);
  scanf("%d", ncol);
  scanf("%d", &i);
  while(i != -1)
  {
    scanf("%d", &j);
    scanf("%d", &tabuleiro[i][j]);
    scanf("%d", &i);
  }
}


void copie_matriz(int origem[MAX][MAX], int destino[MAX][MAX],
                   int nlin, int ncol){
   int i,j;
   i = 0; j = 0;
   for (i = 0; i < nlin; i++)
   {
     for (j = 0; j < ncol; j++)
     {
       destino[i][j] = origem[i][j];
     }
   }
}
