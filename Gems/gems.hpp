
#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>
#include <SFML/Graphics.hpp>


using namespace sf;
using namespace std;

class Gem;

enum SwapStage
{
	None,
	Compression,
	Swapping,
	Decompression

};


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
	SwapStage swap_stage;
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
	void make_chosen(const Vector2i first);
	void make_unchosen(const Vector2i first);
	bool is_near(const Vector2i first, const Vector2i second) const;
	
	bool compress(RenderWindow& win, const float elapsed, const Vector2i gem);
	void swap();
	bool decompress(RenderWindow& win, const float elapsed, const Vector2i gem);
	void swap_chosen_gems(RenderWindow& win, Clock& clc);
	void choosing(RenderWindow& win, Clock &clc);
	void set_count_of_deleting_blocks();
	bool is_in_combination(const Vector2i x) const;
	bool is_in_burst_combination(const Vector2i x) const;
	void delete_combination(Clock& clc);
	void test_gem(const int x, const int y);
	void search_combination();
	void update(Clock& clc);
	
};


