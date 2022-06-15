
#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>
#include <SFML/Graphics.hpp>


using namespace sf;
using namespace std;

class Gem;

class GemsField
{
public:
	vector <vector <unique_ptr <Gem>>> gems;
	vector <unique_ptr<Gem>> dead_gems;
	vector<Vector2i> combination;
	vector<Vector2i> burst_combination;

	Vector2i chosen1;
	Vector2i chosen2;
	vector <int> count_of_deleted_blocks;
	int scaling_stage;
	int deleting_stage;
	int updating_stage;
	int current_color;
	int is_burst;
	Texture burst;
	vector <Vector2i> collector;
	vector <vector<int>> gems_colors;
	
	GemsField();
	

	void draw(RenderWindow& win);
	

	Vector2i clicked_gem_number(Vector2i mouse);

	void make_chosen(Vector2i first);
	
	void make_unchosen(Vector2i first);
	
	bool is_near(Vector2i first, Vector2i second)
	{
		int differ_x = abs(first.x - second.x);
		int differ_y = abs(first.y - second.y);
		return differ_x == 1 && differ_y == 0 || differ_x == 0 && differ_y == 1;
	}

	void swap(RenderWindow& win, Clock& clc);
	
	void choosing(RenderWindow& win, Clock clc);
	

	void set_count_of_deleting_blocks();


	bool is_in_combination(Vector2i x);

	bool is_in_burst_combination(Vector2i x);

	void delete_combination(Clock& clc);
	

	
	void test_gem(int x, int y);
	
	void search_combination();
	

	void update(Clock& clc);
	
};


