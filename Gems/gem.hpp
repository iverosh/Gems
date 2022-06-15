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
	
	Gem(int x, int y, int is_falling)
	{

		number_x = x;
		number_y = y;
		size = (4.0 / 5.0) * window_size / (count_of_gems * 2 - 1);
		is_felt = (bool)is_falling;

		Image img;
		img.loadFromFile("gem.png");
		tex.loadFromImage(img);
		sprite.setTexture(tex);
		normal_scale = size / img.getSize().x;
		sprite.setOrigin(img.getSize().x / 2, img.getSize().y / 2);
		sprite.setScale(normal_scale, normal_scale);

		count_of_deleting_blocks_under = 0;

		switch (rand() % 6)
		{
		case 0:
			sprite.setColor(Color::Red);
			color = Red;
			break;
		case 1:
			sprite.setColor(Color::Green);
			color = Green;
			break;
		case 2:
			sprite.setColor(Color::Blue);
			color = Blue;
			break;
		case 3:
			sprite.setColor(Color::Yellow);
			color = Yellow;
			break;
		case 4:
			sprite.setColor(Color::Cyan);
			color = Cyan;
			break;
		case 5:
			sprite.setColor(Color::Magenta);
			color = Magenta;
			break;

		default:
			break;
		}
		if (is_falling == 0)
		{
			set_position(window_size * (1.0 / 10.0) + x * size * 2 + size / 2, window_size * (1.0 / 10.0) + y * size * 2 + size / 2);
			fall_pos = window_size * (1.0 / 10.0) + y * size * 2 + size / 2;
		}
		else
		{
			set_position(window_size * (1.0 / 10.0) + x * size * 2 + size / 2, -(count_of_gems - y) * size * 2);
			fall_pos = window_size * (1.0 / 10.0) + y * size * 2 + size / 2;

		}
	}
	
	virtual ~Gem() = default;

	void set_color(MyColor color)
	{
		this->color = color;
		switch (color)
		{
		case 0:
			sprite.setColor(Color::Red);
			color = Red;
			break;
		case 1:
			sprite.setColor(Color::Green);
			color = Green;
			break;
		case 2:
			sprite.setColor(Color::Blue);
			color = Blue;
			break;
		case 3:
			sprite.setColor(Color::Yellow);
			color = Yellow;
			break;
		case 4:
			sprite.setColor(Color::Cyan);
			color = Cyan;
			break;
		case 5:
			sprite.setColor(Color::Magenta);
			color = Magenta;
			break;
		default:
			break;
		}
	
	}

	void set_position(double x, double y)
	{
		sprite.setPosition(Vector2f(x, y));
	}

	void set_position(Vector2f pos)
	{
		sprite.setPosition(pos);
	}
	
	Vector2f get_position()
	{
		return sprite.getPosition();
	}
	
	double get_scale()
	{
		return sprite.getScale().x;
	}

	void change_scale(double scale)
	{
		if (get_scale() - scale > 1)
			sprite.setScale(Vector2f(normal_scale, normal_scale));
		else if (get_scale() - scale < 0)
			sprite.setScale(Vector2f(0, 0));
		else
			sprite.setScale(Vector2f(get_scale() - scale, get_scale() - scale));
	}


	void set_scale(double scale)
	{
		sprite.setScale(Vector2f(scale, scale));
	}
	
	
	
	void draw(RenderWindow& win)
	{
		win.draw(sprite);
	}

	virtual void death(GemsField& gems);
	

	void set_fall_pos()
	{
		fall_pos = get_position().y + size * 2 * count_of_deleting_blocks_under;
	}
	void set_count(int count)
	{
		count_of_deleting_blocks_under = count;
	}

	int fall_down(float elapsed)
	{
		if (get_position().y < fall_pos - 1)
		{
			sprite.move(Vector2f(0, falling_speed * elapsed / 1000));
			return 0;
		}
		else
		{
			//cout << "lol";
			set_position(Vector2f(get_position().x, fall_pos));
			return 1;
		}
	}

};

class Bomb : public Gem
{
public:

	Bomb(int x, int y, int is_falling) :Gem(x, y, is_falling)
	{
		Image img;
		img.loadFromFile("Bomb.png");
		tex.loadFromImage(img);
		sprite.setTexture(tex);
			
	}
		

	void death(GemsField& gems);
	

};

class Brush : public Gem
{
public:
	Brush(int x, int y, int is_falling) :Gem(x, y, is_falling)
	{
		Image img;
		img.loadFromFile("Brush.png");
		tex.loadFromImage(img);
		sprite.setTexture(tex);

	}


	void death(GemsField& gems);


};

