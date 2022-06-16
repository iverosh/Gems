#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>


#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

const int window_size = 1000;
const int count_of_gems = 7;
const double scaling_speed = 0.02;
const double falling_speed = 500;

class GemsField;

enum MyColor
{
	Red,
	Green,
	Blue,
	Yellow,
	Cyan,
	Magenta
};

const Color gems_colors[] = { Color::Red, Color::Green, Color::Blue, Color::Yellow, Color::Cyan, Color::Magenta };

class Gem
{
public:

	Sprite sprite;
	Texture tex;
	MyColor color;
	int number_x;
	int number_y;
	double fall_pos;
	bool is_felt;
	int count_of_deleting_blocks_under;
	double size;
	double normal_scale;
	
	Gem(const int x, const int y, const int is_falling);
	
	virtual ~Gem() = default;

	void set_color(const MyColor color);
	void set_position(const double x, const double y);
	void set_position(const Vector2f pos);
	Vector2f get_position() const;
	double get_scale() const;
	void change_scale(const double scale);
	void set_scale(const double scale);
	void set_fall_pos();
	void set_count(const int count);
	int fall_down(const float elapsed);
	void draw(RenderWindow& win) const;

	virtual void death(GemsField& gems);

};

class Bomb : public Gem
{
public:

	Bomb(const int x, const int y, const int is_falling);

	void death(GemsField& gems);

};

class Brush : public Gem
{
public:
	
	Brush(const int x, const int y, const int is_falling);

	void death(GemsField& gems);

};

