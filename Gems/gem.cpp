#include "gem.hpp"
#include "gems.hpp"
class GemsField;

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
	if (abs(gems.gems.at(bonus_position.x).at(bonus_position.y)->fall_pos - gems.gems.at(bonus_position.x).at(bonus_position.y)->get_position().y) < 0.1)
	{
		if (rand() % 2) // bomb
		{
			int color = gems.gems.at(bonus_position.x).at(bonus_position.y)->color;
			gems.gems.at(bonus_position.x).at(bonus_position.y) = make_unique<Bomb>(bonus_position.x, bonus_position.y, 0);
			gems.gems.at(bonus_position.x).at(bonus_position.y)->set_color((MyColor)color);
		}
		else // brush
		{
			gems.gems.at(bonus_position.x).at(bonus_position.y) = make_unique<Brush>(bonus_position.x, bonus_position.y, 0);
			gems.gems.at(bonus_position.x).at(bonus_position.y)->set_color(color);
		}
	}

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
				if (gems.is_in_combination(Vector2i(i, j)) == 0 && gems.is_in_burst_combination(Vector2i(i, j)) == 0 && abs(gems.gems.at(i).at(j)->fall_pos - gems.gems.at(i).at(j)->get_position().y) < 0.1)
				{
					gems.burst_combination.push_back(Vector2i(i, j));
					count++;
				}
			}
		}
	}
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
				if (gems.is_in_combination(Vector2i(i, j)) == 0 && gems.is_in_burst_combination(Vector2i(i, j)) == 0 && abs(gems.gems.at(i).at(j)->fall_pos - gems.gems.at(i).at(j)->get_position().y) < 0.1)
				{
					gems.gems.at(i).at(j)->set_color(color);
					count++;
				}
			}
		}
	}
}

