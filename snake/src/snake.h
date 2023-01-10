#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "winsys.h"
#include "cpoint.h"
#include "screen.h"

#include <ctime>
#include <vector>


class CSnake : public CFramedWindow
{
  
  // General stats
  enum state_of_game
  {
    PAUSE, RUN, WIN, LOSE
  };
  
  int score = 0; // General score
  int tick_counter = 0;
  int speed = 40; // Time for input
  bool help_window = 1; // Help Window is displayed
  state_of_game state = PAUSE; // Initial mode
  
  // Initial move
  CPoint dire = CPoint(1, 0);
  int direction = KEY_RIGHT; // Initial direction
  
  
  // Creating snake
  struct Segment : public CPoint
  {
    char c;
    Segment(int x, int y, char c) : CPoint(x, y), c(c) {}
  };
  vector<Segment> segments;
  
  // Creating fruit
  Segment food = Segment(0,0,'O');
  
  void spawn_food()
  {
    bool taken = 1;
    while(taken)
    {
      food.x = rand() % (geom.size.x - 2) + 1;
      food.y = rand() % (geom.size.y - 2) + 1;
      taken = 0;

      for(auto &segment : segments)
      {
        if(segment.x == food.x && segment.y == food.y)
        {
          taken = 1;
          break;
        }
      }
    } 
  }
  
  // Help
  void Help()
  {
    int x = geom.topleft.x;
    int y = geom.topleft.y;
    gotoyx(y + 2, x + 3);
    printl("h - Toggle help information");
    gotoyx(y + 3, x + 3);
    printl("p - Toggle play/pause mode");
    gotoyx(y + 4, x + 3);
    printl("r - Restart game");
    gotoyx(y + 5, x + 3);
    printl("q - Quit");
    gotoyx(y + 6, x + 3);
    printl("arrows - Move snake (in play mode) or");
    gotoyx(y + 7, x + 12);
    printl("move window (in pause mode)");
  }
  
  // Pause
  void Pause()
  {
    int x = geom.size.x / 2 - 3 + geom.topleft.x;
    int y = geom.size.y / 2 + geom.topleft.y; 
    gotoyx(y, x);
  }
  
  // Reset
  void Reset()
  {
	tick_counter = 0;
	speed = 40;
	score = 0;
	
	dire = CPoint(1, 0);
    direction = KEY_RIGHT;
    
    state = RUN;
    help_window = false;
	
	segments.clear();
	int x = 5, y = 2;
    segments.push_back(Segment(x, y, '*'));
    segments.push_back(Segment(x - 1, y, '+'));
    segments.push_back(Segment(x - 2, y, '+'));
    spawn_food();
 
	  
  }
  
  // Changing mode
  void togglePaused()
  {
    if (state == PAUSE) state = RUN;
    else
    {
		state = PAUSE;
	}
  }
  
  // Tick counter
  bool tick_move()
  {
	tick_counter = (tick_counter + 1)%speed;
	if(tick_counter == 0)
	{
		move();
		return true;
	}
	else
	{
		return false;
	}
  }
  
  // 
  void draw()
  {
    for (int y = 1; y < geom.size.y - 1; y++)
    {
      for (int x = 1; x < geom.size.x - 1; x++)
      {
        gotoyx(geom.topleft.y + y, geom.topleft.x + x);
        printc(' ');
      }
    }

    for (int a = segments.size() - 1; a >= 0; a--)
    {
      gotoyx(geom.topleft.y + segments[a].y, geom.topleft.x + segments[a].x);
      printc(segments[a].c);
    }

    gotoyx(geom.topleft.y + food.y, geom.topleft.x + food.x);
    printc(food.c);
  }
  
  void move()
  {
    for (int a = segments.size() - 1; a > 0; a--)
    {
      segments[a].x = segments[a - 1].x;
      segments[a].y = segments[a - 1].y;
    }

    segments[0] += dire;

    checkHead();

    if (segments[0].x == food.x && segments[0].y == food.y)
    {
      score += 1;
      if (score == (geom.size.x - 2) * (geom.size.y - 2) || segments.size() == (geom.size.x - 2) * (geom.size.y - 2) - 1)
      {
        state = WIN;
        return;
      }

      segments.push_back(Segment(segments[0].x, segments[0].y, '+'));
      
      if(speed >= 3) speed-=2;
      
      move();

      spawn_food();
    }

    for (size_t a = 1; a < segments.size(); a++)
    {
      if (segments[0].x == segments[a].x && segments[0].y == segments[a].y)
      {
        state = LOSE;
        break;
      }
    }
  }
  
  // Winning conditions
  
  void Lose()
  {
    int x = geom.topleft.x + geom.size.x / 2 - 4;
    int y = geom.topleft.y + geom.size.y / 2;
    gotoyx(y, x);
    printl("YOU LOST");
  }

  void Win()
  {
    int x = geom.topleft.x + geom.size.x / 2 - 3;
    int y = geom.topleft.y + geom.size.y / 2;
    gotoyx(y, x);
    printl("YOU WON");
  }
  
  void checkHead() // Coming from other part of the map
  {
    if (segments[0].x == geom.size.x - 1)
      segments[0].x = 1;
    if (segments[0].x == 0)
      segments[0].x = geom.size.x - 2;

    if (segments[0].y == geom.size.y - 1)
      segments[0].y = 1;
    if (segments[0].y == 0)
      segments[0].y = geom.size.y - 2;
  }

public:

  // Generating snake
  CSnake(CRect r) : CFramedWindow(r, ' ')
  {
    int x = 5;
    int y = 2;
    segments.push_back(Segment(x, y, '*'));
    segments.push_back(Segment(x - 1, y, '+'));
    segments.push_back(Segment(x - 2, y, '+'));
  }

  // Overloading function
  bool handleEvent(int key)
  {
    if (tolower(key) == 'p' && state != LOSE && state != WIN && help_window == 0)
    {
      togglePaused();
      return true;
    }

    if (tolower(key) == 'h')
    {
      help_window = !help_window;
      if (help_window && state != LOSE && state  != WIN) state  = PAUSE;
      return true;
    }

    if (tolower(key) == 'r')
    {
      Reset();
      return true;
    }

    if (state  == RUN)
    {
      if (key == direction)
      {
        move();
        return true;
      }

      if (key == KEY_UP && direction != KEY_DOWN)
      {
        direction = KEY_UP;
        dire = CPoint(0, -1);
        move();
      }

      else if (key == KEY_DOWN && direction != KEY_UP)
      {
        direction = KEY_DOWN;
        dire = CPoint(0, 1);
        move();
      }

      else if (key == KEY_RIGHT && direction != KEY_LEFT)
      {
        direction = KEY_RIGHT;
        dire = CPoint(1, 0);
        move();
      }

      else if (key == KEY_LEFT && direction != KEY_RIGHT)
      {
        direction = KEY_LEFT;
        dire = CPoint(-1, 0);
        move();
      }

      else
      {
		  tick_move();
	  }

      return true;
    }

    return CFramedWindow::handleEvent(key);
  }

  void paint()
  {
    CFramedWindow::paint();

    gotoyx(geom.topleft.y - 1, geom.topleft.x);
    printl("| SCORE: %d |", score);

    if (help_window)
      Help();
    else
    {
      draw();
      if (state  == PAUSE)
        Pause();
      if (state  == LOSE)
        Lose();
      if (state  == WIN)
        Win();
    }
  }
};

#endif
