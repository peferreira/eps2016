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
void imprima_tabuleiro_borda(int tabuleiro[MAX+2][MAX+2], int nlin, int ncol);
void gera_tabuleiro_com_borda(int tabuleiro[MAX][MAX], int tabuleiroBORDA[MAX+2][MAX+2],int nlin, int ncol);
void retira_borda_tabuleiro(int tabuleiroBORDA[MAX+2][MAX+2], int tabuleiro[MAX][MAX],int nlin, int ncol);
void printf_tracejado(int ncol);
void printf_ncol(int ncol);
int conta_vizinhos(int nlin, int ncol, int i, int j);
void espalhe_celula(int tabuleiro[MAX][MAX], int i, int j, int nlin, int ncol);


/*recebe uma matriz atual, uma matriz para adicionar os valores de deslocamento
para posicoes instaveis uma posicao e devolve 1 se a posicao estiver instavel*/

/*cria matriz com borda -1*/


int main()
{
  int instaveis;
  int novosativados;
  int nlin, ncol;
  int tabuleiro[MAX][MAX];
  instaveis = 0;
  /*int tabuleiroBORDA[MAX+2][MAX+2];*/
  zere_tabuleiro(tabuleiro,MAX,MAX);
  leia_configuracao_inicial(tabuleiro,&nlin,&ncol);
  /*gera_tabuleiro_com_borda(tabuleiro,tabuleiroBORDA,nlin,ncol);
  imprima_tabuleiro_borda(tabuleiroBORDA,nlin,ncol);
  retira_borda_tabuleiro(tabuleiroBORDA, tabuleiro, nlin, ncol);*/

  imprima_tabuleiro(tabuleiro,nlin,ncol);
  instaveis = espalhe(tabuleiro, tabuleiro, nlin, ncol, 3, &novosativados);
  while(instaveis > 0){
    imprima_tabuleiro(tabuleiro,nlin,ncol);
    instaveis = espalhe(tabuleiro, tabuleiro, nlin, ncol, 3, &novosativados);
  }




  return 0;
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

void zere_tabuleiro(int tabuleiro[MAX][MAX], int nlin, int ncol)
{
    int i,j;
    i = 0; j = 0;
    for (i = 0; i < nlin; i++)
    {
      for (j = 0; j < ncol; j++)
      {
        tabuleiro[i][j] = 0;
      }
    }
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
