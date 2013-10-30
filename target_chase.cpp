/*
 Insert a target's initial coordinates and its motion parameters and 
 see it chased and hit down by the smart snake.
 Uses graphics.h available in Turbo C. Compiled under tubrboc++ 4.5 
*/

#include<iostream.h>
#include<fstream.h>

#include<conio.h>
#include<bios.h>
#include<dos.h>
#include<graphics.h>
#include<process.h>

#define UP 18432
#define DOWN 20480
#define LEFT 19200
#define RIGHT 19712
#define ESC 283


/***********************************************************************/

struct menues
{     	void quit();
}menu;

void menues::quit()
{	closegraph();
	cout<<"\nexiting\n";
	getch();
	exit(0);
}

/**********************************************************************/

struct point
{   	int x, y;
	point(int, int);
};

point::point(int x1=0, int y1=0)
{	x=x1;
	y=y1;
}


/**********************************************************************/
/*searches new set of coordinates to be moved onto from last position */
point path_search(point);

/**********************************************************************/
struct SNAKES
{
	point head, tail, cell[40];   // cells of body between head and tail

	int color;
	int length;
	int speed;   // !!! high value of "speed" slows down snake !!!

	int update();  //updates body cells on every pixel move of snake
	int move_xy(point);
	int owncollision();

	SNAKES();
}snake;

SNAKES::SNAKES()
{	length=40;     // must be equal to "x" as in cell[x] while creating struct cell[x]
	speed=50;      // !!! high value of "speed" slows down snake !!!
	head.x=250;
	head.y=150;
	color=WHITE;	//since graphics maynot be initialised yet so we are indicating colors using numbers

	/* constructing body */
	for(int i=0; i<length; i++)
	{	cell[i]=head;
	}
	tail=cell[length-1]; //tail is the last cell and head is the first cell
}

SNAKES::owncollision()
{	if(getpixel(head.x, head.y)==color)//collided
		return 1;
	else //dint collide
		return 0;
}

SNAKES::update()
{	for(int i=length-1; i>0; i--)
	{	cell[i]=cell[i-1];
	}

	cell[0]=head;
	tail=cell[length-1];

	return 0;
}


/************************************************************************/

struct game_variables
{       int key_pressed;
	int right_boundry, left_boundry, top_boundry, bottom_boundry;
	int centrex, centrey;  // screen's centre pixels
	int bgcolor;	//background color
	void over(int);
	game_variables();
}game;

game_variables::game_variables()
{	right_boundry=600;   //just setting an initial default value as precaution
	left_boundry=20;
	top_boundry=20;
	bottom_boundry=500;
	bgcolor=RED;
	key_pressed=0;
}

void game_variables::over(int force_quit)
{
	if(force_quit==1)
	{	outtextxy(centrex/2,centrey/2, "u pressed escape key to quit.");
	}

	outtextxy(centrex-50,centrey/2+50, "quitting now");

	getch();
	getch();
	menu.quit();
}

/**********************************************************************/
/* target's functions */

struct targets:public point
{    	void move();
	point param;	//change in positions while moving it. (motion prameters)
	targets();
}target;

targets::targets()
{
	cout<<"enter target parameters (x,y): ";
	cin>>x;
	cout<<"\b\b ";
	cin>>param.y;
	cout<<"enter motion parameters (x,y): ";
	cin>>param.x;
	cout<<"\b\b ";
	cin>>param.y;
}

void targets::move()
{
	start:
	if((x>game.left_boundry+31&&x<game.right_boundry-31) && (y>game.top_boundry+31&&y<game.bottom_boundry-31))
	{	x+=param.x;
		y+=param.y;
	}
	else
	{       if(param.x>=0)
			x=game.left_boundry+32;
		if(param.x<0)
			x=game.right_boundry-32;
		if(param.y>=0)
			y=game.top_boundry+32;
		if(param.y<0)
			y=game.bottom_boundry-32;
		goto start;
	}
}

/************************************************************************/
void draw_path()
{	int lastcolor=getcolor();
	setcolor(BLUE);
	line(snake.head.x, snake.head.y, target.x, target.y);
	setcolor(GREEN);
	putpixel(snake.head.x, snake.head.y, snake.color);
	circle(target.x, target.y, 10);
	setcolor(lastcolor);
}

void erase_path()
{	int lastcolor=getcolor();
	setcolor(DARKGRAY);
	line(snake.head.x, snake.head.y, target.x, target.y);
	putpixel(snake.head.x, snake.head.y, snake.color);
	setcolor(lastcolor);
}
/************************************************************************/
void control_room()
{
	setbkcolor(game.bgcolor);
	game.centrex=getmaxx()/2;
	game.centrey=getmaxy()/2;
	game.left_boundry=10;
	game.right_boundry=game.centrex*2-10;
	game.top_boundry=20;
	game.bottom_boundry=game.centrey*2-5;

	cout<<"head: "<<snake.head.x<<", "<<snake.head.y;
	rectangle(game.left_boundry, game.top_boundry, game.right_boundry, game.bottom_boundry);
	game.key_pressed=RIGHT;			/* to start game for the first time */

	/* to record path moved */
	ofstream opfile;
	opfile.open("path.txt", ios::binary);
	if(!opfile)
	{	cout<<"file not opened";
	}

	/* draw initial path and mark target in a circle */
 //	int lastcolor=getcolor();
	setcolor(YELLOW);
	circle(target.x, target.y, 5);
	snake.move_xy(snake.head);
	point neu_position;

	do
	{	neu_position = path_search(snake.head);
		opfile<<neu_position.x<<neu_position.y;
		//cout<<" "<<neu_position.x<<","<<neu_position.y;
		erase_path();
		snake.move_xy(neu_position);
		target.move();

	}while( !(neu_position.x==-1 && neu_position.y==-1) );


	opfile.close();

	cout<<"\final position: "<<neu_position.x<<", "<<neu_position.y;
	outtextxy(game.centrex-20, game.centrey, "target down dude!!!:-)");

	/*reuse neu to redraw whole path*/
	ifstream ipfile;
	ipfile. open("path.txt", ios::nocreate|ios::noreplace|ios::binary);
	if(!ipfile)
	{	cout<<"ofile not opened";
	}
	point dummy;
	while(ipfile)
	{      	ipfile>>dummy.x>>dummy.y;
		cout<<dummy.x<<dummy.y;
		putpixel(dummy.x, dummy.y, WHITE);
	}

	getch();
	getch();

} //end of control_room()


/*************************************************************************/


SNAKES::move_xy(point neu)
{
		snake.head=neu;
		snake.update();

		if(head.y==game.bottom_boundry)
		{	head.y=game.top_boundry+1;
		}
		else
		{	if(head.y==game.top_boundry)
			{	head.y=game.bottom_boundry-1;
			}
			else
			{	if(head.x==game.right_boundry)
				{	head.y=game.left_boundry+1;
				}
				else
				{	if(head.x==game.left_boundry)
					{	head.x=game.right_boundry-1;
					}
				}
			}
		}

		/* moving snake ahead */
		putpixel(head.x, head.y, color);
		delay(speed);
		/* erasing snakes trailing path */
		putpixel(tail.x, tail.y, game.bgcolor);

/* not		if(owncollision()) //if snake collides to itself
  needed 	{	game.over();
   now		}
*/
		return 0;
}

/************************************************************************/
void main()
{	clrscr();

	/* request auto detection */
	int gdriver = DETECT, gmode;

	/* initialize graphics mode */
	initgraph(&gdriver, &gmode, "");

	control_room();
}

/***************************************************************************/

point path_search(point initial)
{       draw_path();
	//locate the next BLUE coordinateto move onto by searching the all 4 neighbouring points possible in a 2d plane
	//checking left pixel
	int neighbours_color = getpixel(initial.x-1, initial.y);
	if(neighbours_color==BLUE)
	{	point got_bluepixel;
		got_bluepixel.x=initial.x-1;
		got_bluepixel.y=initial.y;
		return(got_bluepixel);
	}

	//checking upper pixel
	neighbours_color = getpixel(initial.x, initial.y-1);
	if(neighbours_color==BLUE)
	{	point got_bluepixel;
		got_bluepixel.x=initial.x;
		got_bluepixel.y=initial.y-1;
		return(got_bluepixel);
	}

	//checking right pixel
	neighbours_color = getpixel(initial.x+1, initial.y);
	if(neighbours_color==BLUE)
	{	point got_bluepixel;
		got_bluepixel.x=initial.x+1;
		got_bluepixel.y=initial.y;
		return(got_bluepixel);
	}

	//checking lower pixel
	neighbours_color = getpixel(initial.x, initial.y+1);
	if(neighbours_color==BLUE)
	{	point got_bluepixel;
		got_bluepixel.x=initial.x;
		got_bluepixel.y=initial.y+1;
		return(got_bluepixel);
	}

	//checking upper-left pixel
	neighbours_color = getpixel(initial.x-1, initial.y-1);
	if(neighbours_color==BLUE)
	{	point got_bluepixel;
		got_bluepixel.x=initial.x-1;
		got_bluepixel.y=initial.y-1;
		return(got_bluepixel);
	}

	//checking upper-right pixel
	neighbours_color = getpixel(initial.x+1, initial.y-1);
	if(neighbours_color==BLUE)
	{	point got_bluepixel;
		got_bluepixel.x=initial.x+1;
		got_bluepixel.y=initial.y-1;
		return(got_bluepixel);
	}

	//checking lower-left pixel
	neighbours_color = getpixel(initial.x-1, initial.y+1);
	if(neighbours_color==BLUE)
	{	point got_bluepixel;
		got_bluepixel.x=initial.x-1;
		got_bluepixel.y=initial.y+1;
		return(got_bluepixel);
	}

	//checking lower-right pixel
	neighbours_color = getpixel(initial.x+1, initial.y+1);
	if(neighbours_color==BLUE)
	{	point got_bluepixel;
		got_bluepixel.x=initial.x+1;
		got_bluepixel.y=initial.y+1;
		return(got_bluepixel);
	}

	//if reached here, that means no blue pixel exists in perifery
	point got_bluepixel;
	got_bluepixel.x=-1;
	got_bluepixel.y=-1;
	return(got_bluepixel);
}
