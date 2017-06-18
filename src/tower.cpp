#include "tower.h"


using namespace std;

tower::tower(const string & path):damage(1),price(10),range(3)
{
	img_path = "src/assets/tower2.bmp";
	
	ifstream in_file(path);
	string line;
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
			     if(tokens[0] == "damage") damage=stoi(tokens[1]);
			else if(tokens[0] == "price") price=stoi(tokens[1]);
			else if(tokens[0] == "range") range=stoi(tokens[1]);
		}
		printf("############\ndamage=%d\nprice=%d\nrange=%d\n###########\n",damage,price,range);
		in_file.close();
	}
	else {
		printf("Couldn't open tower definition file: %s Loading default tower instead.\n", path.c_str() );
	}
}

int tower::get_price()
{
	return price;
}
int tower::get_range()
{
	return range;
}

string tower::get_img_path()
{
	return img_path;
}
