#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <ctime>
#include <iostream>

using namespace std;

const int VERTICAL = 25;
const int HORIZONTAL = VERTICAL*2;

static int score = 2;
static char board_array[VERTICAL][HORIZONTAL], users_input;
static bool game_over;
typedef struct axis_points// START of struct
{
int x, y;
}points;// END of struct


 points randomizer(points *p)// START of func
 {// randomizes random x's and y's for both obstacles and mice
  p->x = rand()%HORIZONTAL;
  p->y = rand()%VERTICAL;
 
  return *p;// pointer, 'coz in class obstacle will be implemented class mouse
 }// END of func


 void setNew_points (points *opoint)// START of method, (obstacles' points)
 {
  for(int i = 0; i < 4; i++)// queue of arrays of datatype points 'rpoint'
   opoint[i] = randomizer(opoint + i);// gives to each x and y random values
 }// END of method


class board // START of class
{
  public:

board ()// START of constructor
 {// fills the board with symbols

 for(int columns = 0; columns <= HORIZONTAL ; columns++)
  {
   board_array[0][columns] = '-';
   board_array[VERTICAL -1][columns] = '_';
  }
 for(int rows = 0; rows < VERTICAL; rows++)
  board_array[rows][0] = board_array[VERTICAL -1][HORIZONTAL] = '|';   
 for(int rows = 1; rows <= VERTICAL - 2; rows++)
 for(int columns = 1; columns <= HORIZONTAL - 1; columns++)
  board_array[rows][columns]= ' '; 
 }// END of constructor
};// END of class


class obstacles// START of class
{
  public:
  points opoint[4];// stands for 'obstacle point'


  void setNew_points()// START of method
 {
   for(int i = 0; i < 4; i++)// queue of arrays of datatype points 'rpoint'
    opoint[i] = randomizer(opoint + i);// gives to each x and y random values
 }// END of method


 void random_types()// START of method      
 {// creates different types of obstacles (there are 3 types of them: '#','+' and '>')
 int num_of_obstacles = 5;
  do
   {
    for(int i = 0; i < 4; i++)// queue of arrays of datatype points 'rpoint'
    {
     int random = rand()%3;// random type of obstacle for 'if operator'
      for(int j = 0; j < 8; j++)// fills columns with 5 symbols: "########", "++++++++", ">>>>>>>>"
      {
       if(random == 0)
        board_array[opoint[i].y][opoint[i].x + j]  = '#';
       if(random == 1)
        board_array[opoint[i].y][opoint[i].x + j]   = '+';
       if(random == 2)
        board_array[opoint[i].y][opoint[i].x + j]   = '>'; 
      }
     }
    num_of_obstacles--;
  } while (num_of_obstacles > 0);// 4 obstacles must be created  
 }// END of method
};// END of class


class snake// START of class
{
  public:
  points spoint;// var of datatype struct points stands for 'snake's points'
  points *tail = (points*)malloc(score*sizeof (points));

  snake ()// START of constructor for initial snake's head values
 {// for them to be in the middle of the board
  spoint.x = HORIZONTAL/2;
  spoint.y = VERTICAL/2;
 }// END of constructor


 void setHead()// START of a method to change the values of snake's head
 {// in accordance with the user's input
	// checks user's input and impacts on snake's head location  
   switch(getch())   
   { 
     case 'W':
      spoint.y--;
     break;

     case 'A':
     spoint.x--;
     break;

     case 'D':
      spoint.x++;
     break;

     case 'S':
      spoint.y++;
     break;

     case 'X':// if player wants to quit the game
      game_over = true;
     break; 
    }  
 }// END of method


 void setTail()// START of method
 {// sets tail for the snake with malloc(), 'coz user's success is something dynamic and not constant
    tail[0].y = spoint.y;
    tail[0].x = spoint.x;
    board_array[((tail)->y)][((tail)->x)] = 'o';

  for(int i = score; i > 0; i--)      
  { 
   tail[i].y = tail[i - 1].y;
   tail[i].x = tail[i - 1].x;  
  board_array[((tail + i)->y)][((tail + i)->x)] = 'o';
   if(i == score)  
    board_array[((tail + i)->y)][((tail + i)->x)] = ' '; 
  // clears old points of tail (for them not be printed again)
  }
  // sets new points of tail in board array
 }// END of method
  

 void check_borders()// START of method
 {// checks if the snake has gone beyond the borders and returns it to opposite side
  if(spoint.y < 0)
   spoint.y = VERTICAL - 1;
  if(spoint.y >= VERTICAL)
   spoint.y = 0;
  if(spoint.x < 0)
   spoint.x = HORIZONTAL - 1;
  if(spoint.x >= HORIZONTAL)
   spoint.x = 0;
 }// END of method


 /*void collision_with_tail()// START of method
 {// checks if the snake ate its tail 
   for(int i = score; i >= 0; i--)
    if(board_array[spoint.y][spoint.x] == board_array[((tail + i)->y)][((tail + i)->x)])
     game_over = true;
 }// END of method
*/

 void collision_with_obstacles(obstacles o)// START of method
 {// checks if the snake hit obstacles
   for(int i = 0; i < 4; i++)// 4 different RANDOM obstacles: '#', '>', '+'
    for(int j = 0; j < 8; j++)// 8 symbols each one: "########", ">>>>>>>>", "++++++++"
     if(board_array[spoint.y][spoint.x] == board_array[o.opoint[i].y][o.opoint[i].x + j])
      game_over = true;
 }// END of method 
};// END of class


class mouse: public board// START of class
{
  public:
  points mpoint;// stands for mouse's points


  mouse()// START of constructor
  {// sets random point for new mouse
   mpoint = randomizer(&mpoint);
  }// END of constructor


  void setNew_mouse(snake s, obstacles o)// START of method
 { 
   if(mpoint.y == s.spoint.y && mpoint.x == s.spoint.x)// checks if snake ate mouse
   { 
    mpoint = randomizer(&mpoint);// sets random point for new mouse  
     for(int i = 0; i < 5; i++)
      if((mpoint.y == VERTICAL && mpoint.x == HORIZONTAL) && (mpoint.y == o.opoint[i].y && mpoint.x == o.opoint[i].x ))
       mpoint = randomizer(&mpoint);// sets random point for new mouse  
       o.setNew_points();// sets random points for new obstacles
       o.random_types();// sets random types for new obstacles
       score++;// increases user's success
    } 
 }// END of method
};// END of class


  void draw(obstacles o,snake s, mouse m, board b)// START of func
 {// draws the filled array board     
  // sets new points of snake in board array  
   board_array[s.spoint.y][s.spoint.x] = 'O'; 
  // sets new points of mouse in board array
   board_array[m.mpoint.y][m.mpoint.x] = '*';
     
  for(int rows = 0; rows <= VERTICAL; rows++)
   {   
    std::cout <<"\t\t\t\t";// for the board to be in the ~~middle 
     for(int columns = 0; columns <= HORIZONTAL; columns++)
      std::cout << board_array[rows][columns]; 
    std::cout << std:: endl;
   }   
  // clears old points of snake (for them not be printed again)
   board_array[s.spoint.y][s.spoint.x] = ' '; 
  //  clears old points of mouse (for them not be printed again)
   board_array[m.mpoint.y][m.mpoint.x] = ' ';
  std::cout << std:: endl << "\t\t\t\t   Press [X] if you want to quit ";
  std::cout << std:: endl << "\t\t\t\t   _   _                                      __";
  std::cout << std:: endl << "\t\t\t\t  (q\\_/p)                                    {0O}";  
  std::cout << std:: endl << "\t\t\t\t   |. .|                                     \\__/";
  std::cout << std:: endl << "\t\t\t\t \\ =\\,/=       ~~~~~~~~~~~~~~~~~~~~~~~~~~~   /^/";
  if(score <= 1)
   std::cout << std:: endl << "\t\t\t\t  )/ _ \\  |\\   | Your score is: " << score-2 << " mouse  |  ( (              ";
  if(score > 1)
   std::cout << std:: endl << "\t\t\t\t  )/ _ \\  |\\   | Your score is: " << score-2 << " mouses |  ( (              ";
   std::cout << std:: endl << "\t\t\t\t (/\\):(/\\  )\\  ~~~~~~~~~~~~~~~~~~~~~~~~~~~   \\_\\_____";
   std::cout << std:: endl << "\t\t\t\t  \\_____/ |Oo\\                               (_______)";
   std::cout << std:: endl << "\t\t\t\t   ^   ^                                    (_________()Oo";    
 }// END of func


int main()
{  
  srand(time(NULL));

 board b;// object of a class 'board' & its constructor
 mouse m;// object of a class 'mouse' & its constructor
 snake s;// object of a class 'snake' & its constructor
 obstacles o;// object of a class 'obstacles' & its constructor
 o.setNew_points();// sets new points for obstacles
 o.random_types();// sets different types of random obstacles

 for(int i = 0; i < 4; i++)// checks if points went through the walls
  if((m.mpoint.y == VERTICAL && m.mpoint.x == HORIZONTAL) && (m.mpoint.y == o.opoint[i].y && m.mpoint.x == o.opoint[i].x))
   m.mpoint = randomizer(&m.mpoint);

 game_over = false;

  while(!game_over)// START of snake-game
  {       
   std::cout << std::endl;   
    draw(o,s,m,b);
     s.setHead();    
     m.setNew_mouse(s,o);
     s.setTail();   
  //s.collision_with_tail();
     s.check_borders();
     s.collision_with_obstacles(o); 
  }// END of while loop 
  for(int i = 0; i < score; i++)
  free(s.tail + i);


 if(game_over == true)
  {
   std::cout << std::endl << "You lost!\nPlay again?\n y/ n\n";
   getch();  
   
   if(getch() == 'Y')
    return main();  
   else
    std::cout <<" Hope to see you soon!\nBye ~";
  }
return 0; 
}
// END of MAIN func
