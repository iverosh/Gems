#include "gem.hpp"
#include "gems.hpp"
class GemsField;



Gem::Gem(const int x, const int y, const int is_falling)
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

	color = MyColor(rand() % 6);
	sprite.setColor(gems_colors[color]);

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


void Gem::set_color(const MyColor color)
{
	this->color = color;
	sprite.setColor(gems_colors[color]);
}

void Gem::set_position(const double x, const double y)
{
	sprite.setPosition(Vector2f(x, y));
}


void Gem::set_position(const Vector2f pos)
{
	sprite.setPosition(pos);
}

Vector2f Gem::get_position() const
{
	return sprite.getPosition();
}


double Gem::get_scale() const
{
	return sprite.getScale().x;
}


void Gem::change_scale(const double scale)
{
	if (get_scale() - scale > 1)
		sprite.setScale(Vector2f(normal_scale, normal_scale));
	else if (get_scale() - scale < 0)
		sprite.setScale(Vector2f(0, 0));
	else
		sprite.setScale(Vector2f(get_scale() - scale, get_scale() - scale));
}


void Gem::set_scale(const double scale)
{
	sprite.setScale(Vector2f(scale, scale));
}

void Gem::set_fall_pos()
{
	fall_pos = get_position().y + size * 2 * count_of_deleting_blocks_under;
}

void Gem::set_count(const int count)
{
	count_of_deleting_blocks_under = count;
}

int Gem::fall_down(const float elapsed)
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

void Gem::draw(RenderWindow& win)const
{
	win.draw(sprite);
}

void Gem::death(GemsField& gems)
{
	if (rand() % 15 > 1)
		return;
	Vector2i bonus_position;
	int left = number_x > 3 ? 3 : number_x;
	int top = number_y > 3 ? 3 : number_y;

	int right = count_of_gems - number_x > 3 ? 3 : count_of_gems - number_x - 1;
	int bottom = count_of_gems - number_y > 3 ? 3 : count_of_gems - number_y - 1;

	bonus_position.x = number_x + rand() % (left + right + 1) - left;
	bonus_position.y = number_y + rand() % (top + bottom + 1) - top;
	if (abs(gems.gems[bonus_position.x][bonus_position.y]->fall_pos - gems.gems[bonus_position.x][bonus_position.y]->get_position().y) < 0.1)
	{
		if (rand() % 2) // bomb
		{
			int color = gems.gems[bonus_position.x][bonus_position.y]->color;
			gems.gems[bonus_position.x][bonus_position.y] = make_unique<Bomb>(bonus_position.x, bonus_position.y, 0);
			gems.gems[bonus_position.x][bonus_position.y]->set_color((MyColor)color);
		}
		else // brush
		{
			gems.gems[bonus_position.x][bonus_position.y]= make_unique<Brush>(bonus_position.x, bonus_position.y, 0);
			gems.gems[bonus_position.x][bonus_position.y]->set_color(color);
		}
	}

}


Bomb::Bomb(const int x, const int y, const int is_falling) :Gem(x, y, is_falling)
{
	Image img;
	img.loadFromFile("Bomb.png");
	tex.loadFromImage(img);
	sprite.setTexture(tex);
}


void Bomb::death(GemsField& gems)
{
	int count = 0;
	for (int i = 0; i < count_of_gems; i++)
	{
		for (int j = 0; j < count_of_gems; j++)
		{
			if (count == 5)
				return;
			if (rand() % 3 == 0)
			{
				if (gems.is_in_combination(Vector2i(i, j)) == 0 && gems.is_in_burst_combination(Vector2i(i, j)) == 0 && abs(gems.gems[i][j]->fall_pos - gems.gems[i][j]->get_position().y) < 0.1)
				{
					gems.burst_combination.push_back(Vector2i(i, j));
					count++;
				}
			}
		}
	}
}


Brush::Brush(const int x, const int y, const int is_falling) :Gem(x, y, is_falling)
{
	Image img;
	img.loadFromFile("Brush.png");
	tex.loadFromImage(img);
	sprite.setTexture(tex);

}

void Brush::death(GemsField& gems)
{
	int count = 0;
	for (int i = 0; i < count_of_gems; i++)
	{
		for (int j = 0; j < count_of_gems; j++)
		{
			if (count == 2)
				return;
			if (rand() % 3 == 0 && gems.is_near(Vector2i(i, j), Vector2i(number_x, number_y)) == 0 && abs(i - number_x) < 4 && abs(j - number_y) < 4)
			{
				if (gems.is_in_combination(Vector2i(i, j)) == 0 && gems.is_in_burst_combination(Vector2i(i, j)) == 0 && abs(gems.gems[i][j]->fall_pos - gems.gems[i][j]->get_position().y) < 0.1)
				{
					gems.gems[i][j]->set_color(color);
					count++;
				}
			}
		}
	}
}

