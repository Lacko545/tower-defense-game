#include "engine.h"

using namespace std;


engine::engine(int width, int height, string path):map(path),money(15),lives(3),enemy_n(20)
{
	ifstream in_file("src/cfg/cfg.txt");
	if( in_file.is_open() ){
		string line;
		while( getline(in_file, line) ){
			//line.erase(remove(line.begin(), line.end(), '\n'), line.end());
			vector<string> tokens;
			printf("%s",line.c_str());
			fflush(stdout);
			size_t sep = line.find(" ");
			tokens.push_back(line.substr(0,sep));
			tokens.push_back(line.substr(sep+1));

			if(tokens.size()< 2) break;
			     if(tokens[0] == "money") money=stoi(tokens[1]);
			else if(tokens[0] == "lives") lives=stoi(tokens[1]);
			else if(tokens[0] == "enemy_n") enemy_n=stoi(tokens[1]);
		}
		in_file.close();
	}
	printf("#####\nmoney:%d\nlives:%d\nenemy_n:%d\n#######\n",money,lives,enemy_n);


	screen_width=width;
	screen_height=height;
		
	if(this->init() == 0){
		printf("Couldn't init SDL.\n");
		throw SDLErrorException();
	}

	win=load_surface("src/assets/win.bmp");
	lose=load_surface("src/assets/lose.bmp");
}

engine::~engine()
{
	//Quit SDL
	SDL_FreeSurface(screen_surface);
	SDL_DestroyWindow(main_win);
	SDL_Quit();
}


//initializes SDL
bool engine::init()
{
	if( SDL_Init(SDL_INIT_VIDEO)<0 ){
		printf("Could not initialize window: %s\n", SDL_GetError() );
		return false;
	}
	else{
		main_win = SDL_CreateWindow("Tower Defense",
					SDL_WINDOWPOS_UNDEFINED,
					SDL_WINDOWPOS_UNDEFINED,
					this->screen_width,
					this->screen_height,
					SDL_WINDOW_SHOWN );
	}
	if( main_win == NULL ){
		return false;
	}
	else{
		screen_surface = SDL_GetWindowSurface( main_win );
	}
	return true;
}


void engine::game_loop()
{
	SDL_Event event;
	Uint32 next_turn;
	bool play=1;


	//time when the turn shoud change
	next_turn=SDL_GetTicks()+1000;	
	int enemy_counter = 0;
	while(play){
		//handle user input
		while( SDL_PollEvent(&event)!=0 ){
			if(event.type == SDL_QUIT){
				play=0;
			}

			if(event.type == SDL_MOUSEBUTTONDOWN){
				int x,y;
				int spent=0;
				SDL_GetMouseState(&x,&y);
				spent = this->map.click(x,y,money);
				if(spent == -1){
					printf("###############\nNot enough money.\n###############\n");
				}
				else{
					money-=spent;
				}
			}
		}
			
		
		if( SDL_GetTicks()>next_turn ){
			//End of turn. Move->draw->shoot->draw
			lives-=this->map.move( enemy_counter<enemy_n?enemy_counter%2:false );
			enemy_counter++;
			this->map.draw_map(screen_surface);
			SDL_UpdateWindowSurface( main_win );
			money+=this->map.shoot();
			next_turn=SDL_GetTicks() + 1000;
			printf("lives: %d\nmoney: %d\n",lives,money);

			if(lives<=0){
				printf("You Lose!!!");
				SDL_BlitSurface(lose,NULL,screen_surface,NULL);
				SDL_UpdateWindowSurface( main_win );
				SDL_Delay(1000);
				play=0;
			}
			else if(enemy_counter >= enemy_n && map.enemy_cnt()<=0){
				printf("You Win!!!");
				SDL_BlitSurface(win,NULL,screen_surface,NULL);
				SDL_UpdateWindowSurface( main_win );
				SDL_Delay(1000);
				play=0;
			}
		}


	}
	printf("End of game_loop()\n");
}
