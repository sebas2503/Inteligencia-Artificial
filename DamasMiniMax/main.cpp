#include <iostream>
#include <vector>
#include <utility>
using namespace std;
#define INF 12345
#define TamTabla 8
int Depth;
 int tablaDamas[TamTabla][TamTabla] = {
    {0, -1, 0, -1, 0, -1, 0, -1},
    {-1, 0, -1, 0, -1, 0, -1, 0},
    {0, -1, 0, -1, 0, -1, 0, -1},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0}
     
 };
/*int tablaDamas[TamTabla][TamTabla] = {
   {0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0},
   {1, 0, 1, 0, 1, 0, 1, 0},
   {0, 1, 0, 1, 0, 1, 0, 1},
   {1, 0, 1, 0, 1, 0, 1, 0}
    
};*/
  bool RevisarPersona(){
    bool movimiento=false;
    for (int i = 0; i < TamTabla;++i)
    {
      for (int j = 0; j < TamTabla; ++j)
      {
        if(tablaDamas[i][j] == 1)
        {
          if(tablaDamas[i-1][j-1]==0 && i-1<TamTabla && i-1>=0 && j-1<TamTabla && j-1>=0)
              movimiento=true;
          if(tablaDamas[i-1][j+1]==0 && i-1<TamTabla && i-1>=0 && j+1<TamTabla && j+1>=0)
              movimiento=true;
          if(tablaDamas[i-2][j-2]==0 && i-2<TamTabla && i-2>=0 && j-2<TamTabla && j-2>=0)
              movimiento=true;
          if(tablaDamas[i-2][j+2]==0 && i-2<TamTabla && i-2>=0 && j+2<TamTabla && j+2>=0)
              movimiento=true;
        }
      }
    }
      //cout<<"Mov: "<<movimiento<<endl;
    return movimiento;
  }
void PasarTabla(int t[TamTabla][TamTabla], int t2[TamTabla][TamTabla])
{
    for (int i = 0; i < TamTabla;i++)
    {
        for(int j = 0; j < TamTabla; j++)
        {
            t[i][j] = t2[i][j];
        }
    }
}
/*class movimiento
{
public:
    int tablero[TamTabla][TamTabla];
    movimiento(){}
  movimiento(int t[TamTabla][TamTabla]){
    PasarTabla(tablero,t);
  }
};

vector<movimiento> JugadasHechas;
*/

 int FuncionUtilidad(int tablaDama[TamTabla][TamTabla]) {
   int sumaPieza = 0;

  for (int i = 0; i < TamTabla; ++i)
    for (int j = 0; j < TamTabla; ++j)
      sumaPieza += -tablaDama[i][j];

  return sumaPieza;
}
/*class MovPersoBot
{
public:
    int x;
    int y;
    MovPersoBot(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
    int GetX()
    {
        return x;
    }
    int GetY()
    {
        return y;
    }
};
vector<MovPersoBot> Persona;
vector<MovPersoBot> Bot;*/
pair< int,  int> MoviBot[2] = {{1, -1}, {1, 1}};
pair< int,  int> MoviPersona[2] = {{-1, -1}, {-1, 1}};


void SaberGanador() {
   int sumaPieza = FuncionUtilidad(tablaDamas);

  if (sumaPieza < 0)
  {
    cout<<"Ganaste"<<endl;
  }
  else if (sumaPieza == 0)
  {
    cout<<"Empate"<<endl;
  }
  else
  {
    cout<<"Gano el bot"<<endl;
  }
}
class NodoMiniMax
{
public:
    int MejorTablaDama[TamTabla][TamTabla];
    int tablaDama[TamTabla][TamTabla];
    int depth;
    bool minimax;
    NodoMiniMax(int tabla[TamTabla][TamTabla],int depth)
    {
        this->depth=depth;
        PasarTabla(tablaDama,tabla);
        minimax = depth & 1; //Saber si se minimiza o maximiza
    }
   int MinMax()
   {
        if (depth == Depth) //Se alcanzo los maximos niveles de profundidad
          return FuncionUtilidad(tablaDama);
       
        int mejor = minimax ? INF : -INF; //Si se maximiza INF, caso contrario -INF
        int PersoBot = minimax ? 1 : -1; //Saber q jugador sera 1 = Persona, -1 = Bot
        pair<  int,   int>* MovPersoBot = minimax ? MoviPersona : MoviBot; //Saber q movimientos hare gracias al minimize
        //Ver posibles jugadas, es decir hacer nodo hijos del arbol minimax
        for (int i = 0; i < TamTabla;i++) {
          for (int j = 0; j < TamTabla;j++) {
            if (tablaDama[i][j] == PersoBot) {
              for (int l = 0; l < 2;l++) {
                  int posx = i + MovPersoBot[l].first;
                  int posy = j + MovPersoBot[l].second;

                if (posx >= 0 && posx < TamTabla && posy >= 0 && posy < TamTabla && tablaDama[posx][posy] == 0) {
                  // movimiento
                    tablaDama[i][j] = 0;
                    tablaDama[posx][posy] = PersoBot;
                    NodoMiniMax child(tablaDama, depth + 1);
                    int sumaFicha = child.MinMax();

                  if (minimax && sumaFicha < mejor) {
                    mejor = sumaFicha;
                    PasarTabla(MejorTablaDama, tablaDama);
                  }

                  if (!minimax && sumaFicha > mejor) {
                    mejor = sumaFicha;
                    PasarTabla(MejorTablaDama, tablaDama);
                  }

                    tablaDama[i][j] = PersoBot;
                    tablaDama[posx][posy] = 0;
                }

                  posx = posx + MovPersoBot[l].first;
                  posy = posy + MovPersoBot[l].second;

                if ( posx >= 0 && posx < TamTabla && posy >= 0 && posy < TamTabla && tablaDama[posx][posy] == 0 &&
                    tablaDama[posx-MovPersoBot[l].first][posy-MovPersoBot[l].second] == -PersoBot) {
                  // comer una pieza
                    tablaDama[i][j] = 0;
                    tablaDama[posx-MovPersoBot[l].first][posy-MovPersoBot[l].second] = 0;
                    tablaDama[posx][posy] = PersoBot;
                    NodoMiniMax child(tablaDama, depth + 1);
                    int sumaFicha = child.MinMax();

                  if (minimax && sumaFicha < mejor) {
                    mejor = sumaFicha;
                    PasarTabla(MejorTablaDama,tablaDama);
                  }

                  if (!minimax && sumaFicha > mejor) {
                    mejor = sumaFicha;
                    PasarTabla(MejorTablaDama,tablaDama);
                  }

                    tablaDama[i][j] = PersoBot;
                    tablaDama[posx-MovPersoBot[l].first][posy-MovPersoBot[l].second] = -PersoBot;
                    tablaDama[posx][posy] = 0;
                }
              }
            }
          }
        }
       return mejor;
   }

};
void Imprimir()
{
    cout<<"  +-0---1---2---3---4---5---6---7-+"<<endl;
    
    for (int i=0; i < TamTabla;i++)
    {
        cout<<i<<" |";
        for (int j=0; j < TamTabla;j++)
        {
            if(tablaDamas[i][j] == -1){
              cout << " * |";
            }
            else if(tablaDamas[i][j]== 1){
              cout << " . |";
            }
            else
            {
              cout << "   |";
            }
            
        }
        cout<<endl;
        cout<<"  +-------------------------------+"<<endl;
    }
    cout<<endl;
}
bool MovimientoValidoJugador(int x, int y)
{
    if(y < 0)
    {return false;}
    else if(x < 0 && y > 7)
    {return false;}
    return true;
}
void MovDerecha(int x, int y)
{
    if (MovimientoValidoJugador(x-1,y+1))
    {
        //cout<<"Nueva pos ("<<x-1<<","<<y+1<<")"<<endl;
        if(tablaDamas[x-1][y+1]==0)
        {
            tablaDamas[x][y] = 0;
            tablaDamas[x-1][y+1] = 1;
            //movimiento judada(tablaDamas);
            //JugadasHechas.push_back(judada);
        }
    }
}
void MovIzquierda(int x, int y)
{
    if (MovimientoValidoJugador(x-1,y-1))
    {
        //cout<<"Nueva pos ("<<x-1<<","<<y-1<<")"<<endl;
        if(tablaDamas[x-1][y-1]==0)
        {
            tablaDamas[x][y] = 0;
            tablaDamas[x-1][y-1] = 1;
            //movimiento judada(tablaDamas);
            //JugadasHechas.push_back(judada);
        }
    }
}
void ComerIzquierda(int x, int y)
{
    if (MovimientoValidoJugador(x-2,y-2))
    {
        //cout<<"Nueva pos ("<<x-1<<","<<y-1<<")"<<endl;
        if(tablaDamas[x-2][y-2]==0 && tablaDamas[x-1][y-1]==-1)
        {
            tablaDamas[x][y]=0;
            tablaDamas[x-2][y-2]=1;
            tablaDamas[x-1][y-1]=0;
            //movimiento judada(tablaDamas);
            //JugadasHechas.push_back(judada);
        }
    }
}
void ComerDerecha(int x,int y)
{
    if (MovimientoValidoJugador(x-2,y+2))
    {
        //cout<<"Nueva pos ("<<x-1<<","<<y-1<<")"<<endl;
        if(tablaDamas[x-2][y+2]==0 && tablaDamas[x-1][y+1]==-1)
        {
            tablaDamas[x][y]=0;
            tablaDamas[x-2][y+2]=1;
            tablaDamas[x-1][y+1]=0;
            //movimiento judada(tablaDamas);
            //JugadasHechas.push_back(judada);
        }
    }
}
int main(){

  bool PersoBot;
  cout << "¿Quien empiza? (1)Jugador (0)Bot: ";
  cin >> PersoBot;
  cout << endl << "Depth del MiniMax: ";
  cin >> Depth;

  while(true)
  {
    
    if(PersoBot)
    {
      int x,y;
      int movi;
      if(!RevisarPersona()){break;}

      while(true)
      {
        
        Imprimir();
        cout<<"Ingrese las coordenadas de la ficha a mover: ";
        cin>>x>>y;
        if(tablaDamas[x][y]!=1)
        {
            cout << "Error casilla equivocada\n";
            continue;
        }
        cout<<"Movimiento: \n (1)Izquierda\n (2)Derecha\n (3)Comer Izquierda\n (4)Comer Derecha\n";
          cin>>movi;
          switch (movi)
          {
              case 1:
                  MovIzquierda(x,y);
                  break;
              case 2:
                  MovDerecha(x,y);
                  break;
              case 3:
                  ComerIzquierda(x,y);
                  break;
              case 4:
                  ComerDerecha(x,y);
                  break;
              default:
                  break;
          }
          break;
      }
    }
    else
    {
        NodoMiniMax root(tablaDamas,0);
        int result;
        result = root.MinMax();
        if (result == -INF) //No hubo posible movimiento
        {
          break;
        }
        PasarTabla(tablaDamas,root.MejorTablaDama);
        //Imprimir();
        //movimiento judada(tablaDamas);
        //JugadasHechas.push_back(judada);
    }
      PersoBot = (!PersoBot); //CAMBIAR TURNOS
      //cout<<"TamTabla: "<<JugadasHechas.size()<<endl;
  }

  SaberGanador();

}
