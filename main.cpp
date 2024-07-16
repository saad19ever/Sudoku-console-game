#include<windows.h>
#include <iostream>
#include <algorithm>
#include <ctime>
#include<fstream>
#include<string>

#define EMPTY 0
#define EASY 25
#define MEDIUM 40
#define HARD 50
#define INSANE 75

using namespace std;


class Sudoku
{
private :
   int board[9][9];
  int solutionGrid[9][9];
  int RandomNum[9];
  int RandomPos[81];
  int NeedtoSolve[9][9];
  int difficulty;

public:
  Sudoku ();
  void createSeed();
  bool SolveBoard();
  void countSoln(int &number);
  void genPuzzle();
  void InputDifficulty();
  bool Play();
  void printBoard();
  bool SudokuSolved();
  bool InputCoordinates();
  bool EmptyCellCheck(int row, int col);
  bool Menu();
  void PrintSolution();

};

void Sudoku::InputDifficulty()
{
    char difficultylevel;
    cout<<"please choose difficulty:\n";
      cout<<"(press :   E -> easy ; M -> medium ; H -> hard; I -> insane)\n";
    cin>>difficultylevel;
    difficultylevel=toupper(difficultylevel);

    switch(difficultylevel)
    {
    case 'E': this->difficulty=EASY;break;
    case 'M': this->difficulty=MEDIUM;break;
    case 'H':  this->difficulty=HARD;break;
    case 'I':  this->difficulty=INSANE;break;
    default : {cout<<"Invalid input";
                cin.clear();
                cin.ignore();
                system("pause");
                system("cls");
                InputDifficulty();
                break;
    }

    }



    }

//generates random numbers based on computor clock
int genRandNum(int Max)
{
  return rand()%Max;
}

// START: Create seed grid
void Sudoku::createSeed()
{
  this->SolveBoard();

  // Saving the solution grid
  for(int i=0;i<9;i++)
  {
    for(int j=0;j<9;j++)
    {
      this->solutionGrid[i][j] = this->board[i][j];
    }
  }
}
// END: Create seed grid


// START: Intialising
Sudoku::Sudoku()
{

  // Randomly shuffling the array of removing grid positions
  for(int i=0;i<81;i++)
  {
    this->RandomPos[i] = i;
  }

  random_shuffle(RandomPos, RandomPos+81, genRandNum);

  // Randomly shuffling the guessing number array
  for(int i=0;i<9;i++)
  {
    this->RandomNum[i]=i+1;
  }

  random_shuffle(this->RandomNum, (this->RandomNum) + 9, genRandNum);

  // Initialising the grid
  for(int i=0;i<9;i++)
  {
    for(int j=0;j<9;j++)
    {
      this->board[i][j]=0;
      this->NeedtoSolve[i][j]=0;
    }
  }

}
// END: Initialising


 bool Unassignedlocation (int board[9][9],int &row, int &col)
 {
      for (row = 0; row < 9; row++)
    {
        for (col = 0; col < 9; col++)
        {
            if (board[row][col] == EMPTY)
                return true;
        }
    }
    return false;
 }

 bool IsFoundInCol(int board[9][9],int col,int value)
 {
       for (int row = 0; row < 9; row++)
       {  if (board[row][col] == value)
           return true;
       }
       return false;

 }

 bool IsFoundInRow (int board[9][9],int row, int value)
 {
      for (int col = 0; col < 9; col++)
     {
         if (board[row][col] == value)
            return true;
     }
     return false;

 }

 bool IsFoundInBox (int board[9][9],int BoxFirstRow, int BoxFirstCol, int value)
 {
     for(int row=0; row<3;row++)
     {
         for (int col=0 ; col<3; col++)
         if(board[BoxFirstRow+row][BoxFirstCol+col]==value)
            return true;
     }

     return false;
 }


 bool ValueNotFound(int board[9][9], int row, int col, int value)
   {
     return !IsFoundInRow(board, row, value) && !IsFoundInCol(board,col,value) && !IsFoundInBox(board,row-row%3,col-col%3, value);
}

 bool Sudoku::SolveBoard()
 {
     int row , col;
     if (!Unassignedlocation(this->board,row,col))
        return true;


     for (int value=0 ; value < 9 ; value++)
     {
         if (ValueNotFound(this->board, row, col, this->RandomNum[value]))
         {
              this->board[row][col] = this->RandomNum[value];
               if (SolveBoard())
                return true;
                this->board[row][col] = EMPTY;

          }
     }
     return false;
 }

 void Sudoku::countSoln(int &num)
{
  int row, col;

  if(!Unassignedlocation(this->board, row, col))
  {
    num++;
    return ;
  }
   for(int i=0;i<9 && num<2;i++)
  {
      if(ValueNotFound(this->board, row, col, this->RandomNum[i]) )
      {
        this->board[row][col] = this->RandomNum[i];
        countSoln(num);
      }

      this->board[row][col] = EMPTY;
  }

}
bool Sudoku::InputCoordinates()
{
    do{

    int col,value,row;
        cout<<"(press 0 to see the solution || ctrl+c to exit the game)"<<endl;
      cout<<"input cell coordinates:"<<endl;
    cout<<"input row : ";

    cin>>row;

     if (!cin.fail())

      {
           if (row==0)
          {
          system("cls");
          PrintSolution();
          cout<<"GGWP !";
          exit(0);
            }
          cout<<"input column : ";
          cin>>col;

        if(!cin.fail())
                 {
                     if(EmptyCellCheck(row-1,col-1))

                         { cout<<"input value : ";
                                cin>>value;
                                this->board[row-1][col-1]=value;
                                return true;cout<<"cell already full! input again!";
                                system("pause");
                              return false;
                          }
                        else {
                                cout<<"wrong cell! input again!";
                               system("pause");
                               return false;

                             }
                   }
                else
                {
                    cin.clear();
                     cin.ignore();
                     cout<<"Invalid input!";
                     system("pause");return false;
                       }
      }

       else  {
        cin.clear();
         cin.ignore();
        cout<<"Invalid input!";
        system("pause");
        return false;
      }

      }while(!InputCoordinates());

      return true;


}

bool Sudoku::EmptyCellCheck(int row , int col)
{
    if (this->NeedtoSolve[row][col]!=0) return true;
    else return false;
}

// START: Generate puzzle
void Sudoku::genPuzzle()
{
    int count=0;

  for(int i=0;i<81  ;i++)
  {

    int row = (this->RandomPos[i])/9;
    int col = (this->RandomPos[i])%9;
    int temp = this->board[row][col];
    this->board[row][col] = EMPTY;
    // If now more than 1 solution , replace the removed cell back.
    int check=0;
    countSoln(check);
    if(check!=1)
    {
      this->board[row][col] = temp;

      }
      else
      {count++;
       this->NeedtoSolve[row][col]= temp;}
    if (count==(this->difficulty))
        break;

}
}
void Sudoku::PrintSolution()
{
     const char grid [] = "     1   2   3   4   5   6   7   8   9\n";
    const char line [] = "   +---+---+---+---+---+---+---+---+---+";

    cout<< grid<<endl;
    cout<<line<<endl;
    for ( int row = 0 ;row<9 ; row++)
    {
        if(row == 3 || row== 6) cout<<line<<endl;
            cout<<row+1 <<"  | ";


        for (int col = 0 ; col<9 ; col++)
           {

                cout<< solutionGrid[row][col];

                if (col == 2 || col == 5 || col == 8)cout<<" | ";
                 else    cout<<"   ";
           }
           cout<<endl;
           cout<<endl;
    }

     cout<< line<<endl;


}
void Sudoku::printBoard()
{
    const char grid [] = "     1   2   3   4   5   6   7   8   9\n";
    const char line [] = "   +---+---+---+---+---+---+---+---+---+";

    cout<< grid<<endl;
    cout<<line<<endl;
    for ( int row = 0 ;row<9 ; row++)
    {
        if(row == 3 || row== 6) cout<<line<<endl;
            cout<<row+1 <<"  | ";


        for (int col = 0 ; col<9 ; col++)
           {
               if (board[row][col]==EMPTY)
               {
                   cout<<".";
               }
               else
                cout<< board[row][col];

                if (col == 2 || col == 5 || col == 8)cout<<" | ";
                 else    cout<<"   ";
           }
           cout<<endl;
           cout<<endl;
    }

     cout<< line<<endl;

}


bool Sudoku::Play()
{
    if(Menu())
    {

        InputDifficulty();
        system("cls");
        genPuzzle();
   do{
       printBoard();
      InputCoordinates();
      system("cls");
     }while( !SudokuSolved() );
    }

     return SudokuSolved();


}

bool Sudoku::SudokuSolved()
{
    for ( int i = 0; i < 9; ++i)
    {
        for (int  j = 0; j < 9; ++j)
        {
            if (board[i][j] != solutionGrid[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::Menu()
{
    char choice;

cout<<"***********************************************************"<<endl;
cout<<" **                         Sudoku                      *  "<<endl;
cout<<"  **                    main menu :                   **   "<<endl;
cout<<"   **                 ( S )      to start            **    "<<endl;
cout<<"   **                 ( E )      to exit            **     "<<endl;
cout<<"  **                  ( H )     for help             **    "<<endl;
cout<<" *                                                     *   "<<endl;
cout<<"***********************************************************" <<endl;


    cin>>choice;
    choice=toupper(choice);

    switch (choice)
{
        case 'S': return true;break;
        case 'E': cout<<"goodbye :( ";exit(0);

        case 'H':{ system("cls");
                   cout<<endl;
                    cout<<"数独(sudoku)是一种以数字排列为基础的填空游戏（如下图所示）。"<<endl;
                       cout   << " 玩家需要根据9×9盘面上的已知数字，推理出所有剩余空格的数字，"<<endl;
                     cout      << "并满足每一行、每一列、每一个3×3九宫内的数字均含1-9，不重复."<<endl;
                     cout<<endl;
                    cout<<" return to the main menu";
                        system("pause");
                        system("cls");
                        if(Menu())break;

                      }

        default: {cout<<"wrong key! choose again"<<endl;
                      system("pause");
                   system("cls");
                   if(Menu())break;
}
}
return true;
}


int main()
{
    srand(time(NULL));
    Sudoku *puzzle = new Sudoku();
    puzzle->createSeed();
   if( puzzle->Play())
          cout<< "Well congratulations , you won!  ;) ";
    return 0 ;

}
