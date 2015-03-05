//Bobby Flores
//cse 20211
//final project

#include <stdio.h>
#include <math.h>
#include "gfx3.h"


void update(int *array[2][3]){ 		//set the counter for each pillar back to zero when it gets to the left wall of the window
	int i;
	srand(time(NULL));

	for (i=0;i<3;i++){
		int *a = array[0][i];
		int *b = array[1][i];
		if (*a>=1000){
			*a = 0;		//set counter to zero, respawning the pillar
			*b = (rand()%300); //height of gap between pillar is randomized each time it respawns
		}
	}
}

void half(int x, int y, int r, int d){	//draws a half circle which is used for the wing of the bird
	float theta;
	float dt = (2*M_PI)/100;
	
	if (d==1){
		for (theta=M_PI;theta<=(2*M_PI);theta+=dt){	// upper half of a circle
			int x1 = r*cos(theta) + x;
			int y1 = r*sin(theta) + y;
			int x2 = r*cos(theta + dt) + x;
			int y2 = r*sin(theta + dt) + y;
			gfx_line(x1,y1,x2,y2);
		}
	}
	else if (d==0){
		for (theta=0;theta<=(M_PI);theta+=dt){		//lower half of a circle
			int x1 = r*cos(theta) + x;
			int y1 = r*sin(theta) + y;
			int x2 = r*cos(theta + dt) + x;
			int y2 = r*sin(theta + dt) + y;
			gfx_line(x1,y1,x2,y2);
		}
	}
}			




void bird(int x,int y,int m){	//draws the entire bird
	gfx_ellipse(x,y,15,13);	//body
	gfx_circle(x+10,y-4,4);	//eye
	gfx_point(x+10,y-4);	//pupil
	gfx_triangle(x+14,y-3,x+14,y+3,x+22,y);	//beak
	
	if (m%20<10) half(x-4,y+2,6,0);	//flapping wing, every ten iterations of animation
	else half(x-4,y+2,6,1);
}

void wall(int m, int h, int *i){	//draws a piller with a given randomized gap height
	gfx_color(0,255,0);

	gfx_line(950-m,100,950-m,150+h);	//moves across the screen using the specific counter for each pillar
	gfx_line(1000-m,100,1000-m,150+h);
	gfx_line(950-m,150+h,1000-m,150+h);

	gfx_line(950-m,600,950-m,250+h);
	gfx_line(1000-m,600,1000-m,250+h);
	gfx_line(950-m,250+h,1000-m,250+h);
	
	if ((950-m)==430) (*i)++;		//counts how many pillars that the bird has gone through
}

int wall_pos(int x, int y, int m, int h){	//keeps track of the postion of a given pillar
	if (((935-m)<=x && x<=(1015-m))&&((165+h)>=y || y>=(235+h))){ //if the bird hits the pillar it returns 1, if not returns 0 
		 return 1;
	}
	else return 0;
}

int alive(int x, int y, int m,int n, int p, int h, int g, int k){ //retuns 0 if the bird hits a pillar or the top or bottom wall, if not returns 1
	if (y<110) return 0;
	else if (y>590) return 0;
	else if (wall_pos(x,y,m,h)) return 0;
	
	else if (wall_pos(x,y,n,g)) return 0;
	
	else if (wall_pos(x,y,p,k)) return 0;
	
	else return 1;
}




void play(int *game_state){	//runs animation for game
	char c;
	
	int x;  //position of bird
	int y;

	float v;  //velocity of bird right after a mouse click
	float a;  //constant downward acceleration of bird

	v = 0;
	a = 1;
	x = 500;
	y = 250;
	
	int i = 0; //score counter

	srand(time(NULL));  //random heights of the gaps for each of the three pillars
	int h = (rand()%300);	
	int g = (rand()%300);
	int k = (rand()%300);


	float t; 	//interval by which 'time' changes during each iteration of the animation
	int m=0;	//counters for each pillar that determine their respective positions
	int n=-325;
	int p=-650;

	char str[10]; //character array that score is printed in
	sprintf(str,"%i",i); //converts score from an integer to character

	int *array[2][3] = {{&m,&n,&p},{&h,&g,&k}};
	 


	while(1){
		gfx_clear(); 	//clear the screen before drawing for each iteration
		gfx_color(255,255,255);
		gfx_line(0,600,1000,600); 	//bottom boundary line
		gfx_line(0,100,1000,100);	//top boundary line	
		bird(x,y,m);			//bird
		wall(m,h,&i);			//first pillar
		wall(n,g,&i);			//second pillar
		wall(p,k,&i);			//third pillar
		gfx_flush;
		sprintf(str,"%i",i);		//convert score from integer to character
		gfx_text(50,50,"Your Score:");	//prints the score
		gfx_text(125,50,str);
		y += (v + a*t);			//update position with velocity and acceleration
	
		if (!alive(x,y,m,n,p,h,g,k)) {	//if the bird hits a boundary or pillar, animation stops and player can choose to quit or play again
			*game_state=2;
			break;
		}
		usleep(8000);			//pauses animation for a short period of time

		if (gfx_event_waiting()){
			c = gfx_wait();
		}

		if (c==1){			//if the user presses the left mouse,the bird gets a short burst upwards
			v = -2.5;
			t = 0;
		}
		if (c=='q') break;
		c=0;
		t+=.1;	
		m+=1;		//counters for each pillar
		n+=1;
		p+=1;
		update(array);	//respawns each of the pillar when they reach the left side of the window



	}
}


int main(){
	gfx_open(1000,1000,"game");
	gfx_color(255,255,255);
	
	int game_state = 3;  //value of this integer determines whether the user is playing or in after game menue, starts in pregame menu
	int z = 1;
	char c;
	while(z){
		switch (game_state){
			case 1: play(&game_state); //normal game play
				break;
			case 2: gfx_color(255,0,0);  //if the bird hits a pillar or a boundary the program switches to case two
				gfx_text(490,650,"YOU DIED!");  //prints on the window
				int x = gfx_xpos();
				int y = gfx_ypos();				
				
				gfx_line(250,700,400,700);
				gfx_line(250,800,400,800);
				gfx_line(250,700,250,800);
				gfx_line(400,700,400,800);
				gfx_text(295,750,"PLAY AGAIN"); //box the user can press to play again

				gfx_line(600,700,750,700);
				gfx_line(600,800,750,800);
				gfx_line(600,700,600,800);
				gfx_line(750,700,750,800);
				gfx_text(660,750,"QUIT");  	//box the user can press to quit
				

				if ((x>250 && x<400) && (y>700 && y<800)) game_state = 1;
				else if ((x>600 && x<800) && (y>700 && y<850)) z = 0;
				c = gfx_wait();
				if (c=='q') z=0;
				else if (c=='p') game_state = 1;
				break;
			case 3: gfx_text(460,300,"Flappy Bird");  //opening menu when program is first executed
				gfx_text(200,325,"Use the left mouse key to play to lift the bird and fly through the moving pillars and away from the walls.");  //quick intsructions
				gfx_text(425,500,"Click here to start playing");
				gfx_line(300,400,700,400);
				gfx_line(300,600,700,600);
				gfx_line(300,400,300,600);
				gfx_line(700,400,700,600);	//box the user can press to start gameplay

				int x1 = gfx_xpos();
				int y1 = gfx_ypos();
				if ((300<x1 && x1<700) && (400<y1 && y1<600)) game_state = 1; 
				

				c = gfx_wait();
				if (c=='p') game_state = 1;
				break;

		}
	}
}			
