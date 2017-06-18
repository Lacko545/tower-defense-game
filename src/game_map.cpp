#include "game_map.h"

using namespace std;


game_map::game_map(const string & path):tile_width(16),enemy_n(0)
{
	ifstream in_file(path);
	string line;

	if( in_file.is_open() ){
		while( getline(in_file, line) ){
			line.erase(remove(line.begin(), line.end(), '\n'), line.end());
			map_data.push_back(line);
		}
		in_file.close();
	}
	else{
		throw MapDoesntExistException();
	}

	this->rows=map_data.size();
	this->cols=map_data.begin()->length();


	for(auto i=map_data.begin(); i != map_data.end(); i++){
		if(cols != (int) i->length()){
			// if the size of every line isn't the same
			// the map is obviously not correct
			throw MapFormatException();
		}
	}

	char tmp;
	this->tile_map=new tile**[rows];
	for(int i=0; i<(int)rows; i++){
		this->tile_map[i]=new tile*[cols];
		for( int j=0; j<(int)cols; j++){
			tmp=map_data[i][j];
			if(tmp=='#'){
				// # is a wall
				// the map has to be wrapped by an extra layer of #
				// otherwise it's not correct
				tile_map[i][j] = new wall_tile(i,j);
			} else 
			if (tmp==' '){
				// space is an empty space which can be travelled through
				tile_map[i][j] = new empty_tile(i,j);
			} else
			if (tmp=='_'){
				// _ is a spot where you can build a tower
				tile_map[i][j]=new tower_tile(i,j);
			} else
			if (tmp=='S'){
				// S is the spawn point of enemies
				tile_map[i][j]=new empty_tile(i,j,'S');
			} else
			if (tmp=='E'){
				// E is the exit enemies try to get to
				tile_map[i][j]=new empty_tile(i,j,'E');
			} else {
				// throws exception if map uses undefined characters
				throw MapFormatException();
			}
		}
	}

	this->shortest_path = this->find_path();

	for(auto i = this->shortest_path.begin(); i!= this->shortest_path.end(); i++){
		printf("%d %d ->",i->first, i->second);
		fflush(stdout);
		// outputs the path enemies will take
	}
	printf("\n");
}

game_map::~game_map()
{
	for(int i=0; i<(int)rows; i++){
		for(int j=0; j<(int)cols; j++){
			delete tile_map[i][j];
		}
	}
}


void game_map::draw_map(SDL_Surface *& screen)
{

	// draw map simply traverses all tiles
	// blits their image to their respective square
	for(int i=0; i<this->rows; i++ ){
		for(int j=0; j<this->cols; j++){
			SDL_Rect pos;
			pos.x = j*tile_width;
			pos.y = i*tile_width;
			SDL_BlitSurface(tile_map[i][j]->get_surface(),NULL,screen, &pos);
		}
	}
}


// find path implements a simple BFS algorithm which finds the shortest path 
// enemies can take to reach the exit from their spawn point

vector<pair<int,int>> game_map::find_path() const
{
	pair<int, int> start;
	pair<int, int> end;
	vector<pair<int, int>> ret;

	for(int i=0; i<this->rows; i++){
		for(int j=0; j<this->cols; j++){
			if(tile_map[i][j]->is_end()){
				end.first  = i;
				end.second = j;
			} else if(tile_map[i][j]->is_start()){
				start.first  = i;
				start.second = j;
			}
		}
	
	}

	pair<int, int> act;
	queue<pair<int, int>> q;
	map<pair<int, int>, pair<int, int> > bfs_tree;
	pair<int, int> prev;
	set<pair<int, int>> visited;
	
	q.push(end);
	visited.insert(end);
	
	fflush(stdout);
	while(!q.empty()){
		act=q.front();
		
		q.pop();
		if(act == start){
			break;

		}

		if( !tile_map[act.first + 1][act.second]->occupied() &&
		    visited.find(make_pair(act.first+1, act.second) ) == visited.end() ){
			visited.insert(make_pair(act.first+1,act.second));
			bfs_tree.insert( make_pair(make_pair(act.first+1, act.second), act) );
			q.push(make_pair(act.first+1, act.second ));
		}
		if( !tile_map[act.first][act.second + 1]->occupied() &&
		    visited.find(make_pair(act.first, act.second+1) ) == visited.end()){
			visited.insert(make_pair(act.first,act.second+1));
			bfs_tree.insert( make_pair(make_pair(act.first, act.second+1), act ));
			q.push(make_pair(act.first, act.second+1 ));
		}
		
		if( !tile_map[act.first - 1][act.second]->occupied() &&
		    visited.find(make_pair(act.first-1, act.second) ) == visited.end() ){
			visited.insert(make_pair(act.first-1,act.second));
			bfs_tree.insert(make_pair( make_pair(act.first-1, act.second), act ));
			q.push(make_pair(act.first-1, act.second ));
		}
		if( !tile_map[act.first][act.second - 1]->occupied() &&
		    visited.find(make_pair(act.first, act.second-1) ) == visited.end() ){
			visited.insert(make_pair(act.first,act.second-1));
			bfs_tree.insert(make_pair( make_pair(act.first, act.second-1), act ));
			q.push(make_pair(act.first, act.second-1 ));
		}
	}

	ret.push_back(start);
	for(auto i=bfs_tree.find( start ); i->second != end; i=bfs_tree.find(i->second) ){
		ret.push_back(i->second);
	}
	ret.push_back(end);
	
	return ret;
}

int game_map::move(bool spawn)
{
	int ret=0;
	auto i = this->shortest_path.rbegin();// take last index from path;
	ret = tile_map[i->first][i->second]->remove(); // remove if someone is on finish
	if(ret == 1 ) enemy_n--;
	for(i=next(i); i!=this->shortest_path.rend(); i++){

		if(tile_map[i->first][i->second]->occupied()){
			tile_map[i->first][i->second]->move(*tile_map[prev(i)->first][prev(i)->second]);
		}
	}

	if(spawn){
		enemy_n++;
		auto s_point = this->shortest_path.begin();
		tile_map[s_point->first][s_point->second]->spawn();
	}
	return ret;
}

int game_map::click(int x, int y, int money)
{
	int x_pos, y_pos;
	x_pos = x/16;
	y_pos = y/16;
	printf("clicked here: %d %d, rows: %d cols: %d\n", x_pos, y_pos, rows, cols);
	if(y_pos < rows && x_pos < cols){
		printf("was in bounds\n");
		return tile_map[y_pos][x_pos]->build(money);
	}
	return 0;
}

int game_map::enemy_cnt()
{
	return enemy_n;
}

int game_map::shoot()
{
	int ret=0;
	for(int i=0; i<this->rows; i++){
		for(int j=0; j<this->cols; j++){
			int range = tile_map[i][j]->get_range();
			if(range <= 0) continue;
			for(int ii = i-range; ii <= i+range; ii++){
				if(ii<0 || ii>=this->rows) continue;
				bool breaker = false;
				for(int jj=j-range; jj<=j+range; jj++){
					if(jj<0 || jj>=this->cols ) continue;
					
					if(tile_map[ii][jj]->take_damage(1)){
						if(tile_map[ii][jj]->occupied() == false){
							ret++;
							enemy_n--;
						}
						breaker=true;
						break;
					}
				}
				if(breaker) break;
			}
		}
	}
	return ret;
}


tile::~tile()
{
	if(img != NULL){
		SDL_FreeSurface(img);
	}
}

empty_tile::empty_tile(int x, int y): tile(x,y), occupant(NULL), start(false), end(false){
	this->img=SDL_LoadBMP("src/assets/grass.bmp");
}

empty_tile::empty_tile(int x, int y, char c): tile(x,y), occupant(NULL){
	start = c=='S'?true:false;
	end   = c=='E'?true:false;
	if(start) this->img=SDL_LoadBMP("src/assets/start.bmp");
	if(end) this->img=SDL_LoadBMP("src/assets/end.bmp");
}

void empty_tile::spawn()
{
	this->occupant = new enemy();
}

bool empty_tile::occupied()
{ 
	return occupant==NULL?false:true;
}

bool empty_tile::is_end()
{
	return end;
}

bool empty_tile::is_start(){
	return start;
}

SDL_Surface * empty_tile::get_surface()
{
	return occupant==NULL?this->img:occupant->get_surface();
}

int empty_tile::remove()
{
	int ret=0;
	if(occupant != NULL && this->end==true){
		ret = 1;
		delete occupant;
		occupant=NULL;
	}
	return ret;
}

void empty_tile::move(tile & dest)
{
	dest.occupy( this->occupant );
	this->occupant = NULL;
}

void empty_tile::occupy(enemy * in_occupant)
{
	this->occupant = in_occupant;
}

bool empty_tile::take_damage(int amount)
{
	bool kill=false;
	if(occupant==NULL) return false;
	else kill=occupant->take_damage(amount);
	if(kill){
		delete occupant;
		occupant=NULL;
	}
	return true;
}


wall_tile::wall_tile(int x,int y):tile(x,y)
{
	this->img=SDL_LoadBMP("src/assets/wall.bmp");
}


tower_tile::tower_tile(int x, int y): tile(x,y), building(NULL)
{
	this->img=SDL_LoadBMP("src/assets/no_tower.bmp");
}

tower_tile::~tower_tile()
{
	if(building != NULL) delete building;
}

int tower_tile::build(int money)
{	
	printf("building\n");
	if(building==NULL){
		building = new tower("src/cfg/tower.txt");

		if( building->get_price() <= money ){
			SDL_FreeSurface(img);
			img=load_surface(building->get_img_path());
			return building->get_price();
		} else {
			delete building;
			building = NULL;
			return -1;
		}
	}
	return 0;
}

int tower_tile::get_range()
{
	if(building==NULL) return 0;
	return building->get_range();
}
