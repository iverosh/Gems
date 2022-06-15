#include "gems.hpp"
#include "gem.hpp"

GemsField:: GemsField()
{
	vector <unique_ptr<Gem>> x;
	unique_ptr <Gem> gem;
	for (int i = 0; i < count_of_gems; i++)
	{
		vector<int> vec;
		for (int j = 0; j < count_of_gems; j++)
		{
			gem = make_unique<Gem>(i, j, 0);
			x.push_back(move(gem));
			vec.push_back(0);
		}
		gems.push_back(move(x));
		count_of_deleted_blocks.push_back(0);
		gems_colors.push_back(vec);

	}
	chosen1 = Vector2i(-1, -1);
	chosen2 = Vector2i(-1, -1);
	scaling_stage = -2;
	deleting_stage = 0;
	updating_stage = 1;
	is_burst = 0;
	Image img;
	img.loadFromFile("burst.png");
	burst.loadFromImage(img);
}


void GemsField::draw(RenderWindow& win)
{
	for (int i = 0; i < count_of_gems; i++)
	{
		for (int j = 0; j < count_of_gems; j++)
		{
			if (gems.at(i).at(j) != nullptr)
				gems.at(i).at(j)->draw(win);

		}
	}
}


Vector2i GemsField:: clicked_gem_number(Vector2i mouse)
{
	for (int i = 0; i < count_of_gems; i++)
	{
		for (int j = 0; j < count_of_gems; j++)
		{
			double size = gems.at(i).at(j)->size / 2;
			Vector2f pos = gems.at(i).at(j)->get_position();
			if (pos.x + size >= mouse.x && pos.x - size <= mouse.x && pos.y + size >= mouse.y && pos.y - size <= mouse.y)
				return Vector2i(i, j);
		}
	}
	return Vector2i(-1, -1);
}


void GemsField:: make_chosen(Vector2i first)
{
	double scale = gems.at(first.x).at(first.y)->normal_scale * 1.2;
	gems.at(first.x).at(first.y)->set_scale(scale);
}


void GemsField:: make_unchosen(Vector2i first)
{
	double scale = gems.at(first.x).at(first.y)->normal_scale;
	gems.at(first.x).at(first.y)->set_scale(scale);
}


void GemsField:: choosing(RenderWindow& win, Clock clc)
{

	if (chosen1 == Vector2i(-1, -1))
	{
		chosen1 = clicked_gem_number(Mouse::getPosition(win));
		if (chosen1 != Vector2i(-1, -1))
			make_chosen(chosen1);
	}
	else
	{
		chosen2 = clicked_gem_number(Mouse::getPosition(win));
		if (chosen2 != Vector2i(-1, -1) && is_near(chosen1, chosen2) && chosen1 != chosen2)
			make_chosen(chosen2);
		else
		{
			make_unchosen(chosen1);
			chosen1 = Vector2i(-1, -1);
			chosen2 = Vector2i(-1, -1);

		}
	}
}


void GemsField:: set_count_of_deleting_blocks()
{
	for (int i = 0; i < combination.size(); i++)
	{
		for (int j = 0; j < combination.at(i).y; j++)
		{
			gems.at(combination.at(i).x).at(j)->count_of_deleting_blocks_under++;
			
		}
	}
	for (int i = 0; i < combination.size(); i++)
		gems.at(combination.at(i).x).at(combination.at(i).y)->count_of_deleting_blocks_under = -1;/////
}



void GemsField::swap(RenderWindow& win, Clock& clc)
{

	float elapsed;
	if (scaling_stage == -1)
	{
		if (gems.at(chosen1.x).at(chosen1.y)->get_scale() > 0)
		{
			elapsed = clc.restart().asMilliseconds();
			gems.at(chosen1.x).at(chosen1.y)->change_scale(scaling_speed * elapsed / 50);
			gems.at(chosen2.x).at(chosen2.y)->change_scale(scaling_speed * elapsed / 50);
		}
		else
		{
			scaling_stage = 0;
			gems.at(chosen1.x).at(chosen1.y)->set_scale(0);
			gems.at(chosen2.x).at(chosen2.y)->set_scale(0);
		}
	}
	if (scaling_stage == 0)
	{
		unique_ptr<Gem> tmp = move(gems.at(chosen1.x).at(chosen1.y));
		gems.at(chosen1.x).at(chosen1.y) = move(gems.at(chosen2.x).at(chosen2.y));
		gems.at(chosen2.x).at(chosen2.y) = move(tmp);
		Vector2f pos = gems.at(chosen1.x).at(chosen1.y)->get_position();
		gems.at(chosen1.x).at(chosen1.y)->set_position(gems.at(chosen2.x).at(chosen2.y)->get_position());
		gems.at(chosen2.x).at(chosen2.y)->set_position(pos);
		scaling_stage = 1;
	}
	if (scaling_stage == 1)
	{
		if (gems.at(chosen1.x).at(chosen1.y)->get_scale() < gems.at(chosen1.x).at(chosen1.y)->normal_scale)
		{
			elapsed = clc.restart().asMilliseconds();
			gems.at(chosen1.x).at(chosen1.y)->change_scale(-scaling_speed * elapsed / 200);
			gems.at(chosen2.x).at(chosen2.y)->change_scale(-scaling_speed * elapsed / 200);
		}
		else
		{
			scaling_stage = -2;
			gems.at(chosen1.x).at(chosen1.y)->set_scale(gems.at(chosen1.x).at(chosen1.y)->normal_scale);
			gems.at(chosen2.x).at(chosen2.y)->set_scale(gems.at(chosen1.x).at(chosen1.y)->normal_scale);
			chosen1 = Vector2i(-1, -1);
			chosen2 = Vector2i(-1, -1);

		}

	}

}

bool GemsField:: is_in_combination(Vector2i x)
{
	for (int i = 0; i < combination.size(); i++)
	{
		if (x == combination.at(i))
			return 1;
	}
	return 0;
}

bool GemsField::is_in_burst_combination(Vector2i x)
{
	for (int i = 0; i < burst_combination.size(); i++)
	{
		if (x == burst_combination.at(i))
			return 1;
	}
	return 0;
}

void GemsField::delete_combination(Clock& clc)
{

	float elapsed = clc.restart().asMilliseconds();
	if (deleting_stage == 1)
	{
		if (is_burst == 0)
			sleep(seconds(1));
		deleting_stage = 2;
	}

	if (is_burst)
	{
		for (int i = 0; i < combination.size(); i++)
			gems.at(combination.at(i).x).at(combination.at(i).y)->sprite.setTexture(burst);
		is_burst = 0;
		sleep(seconds(1));
	}
	if (deleting_stage == 2)
	{
		for (int i = 0; i < combination.size(); i++)
		{

			if (gems.at(combination.at(i).x).at(combination.at(i).y)->get_scale() > 0)
				gems.at(combination.at(i).x).at(combination.at(i).y)->change_scale(scaling_speed * elapsed / 300);
			else
			{
				gems.at(combination.at(i).x).at(combination.at(i).y)->set_scale(0);
				deleting_stage = 3;
			}
		}
	}
	if (deleting_stage == 3)
	{
		set_count_of_deleting_blocks();
		for (int i = 0; i < combination.size(); i++)
			count_of_deleted_blocks.at(combination.at(i).x)++;
		for (int i = 0; i < count_of_gems; i++)
		{
			for (int j = 0; j < count_of_gems; j++)
				gems.at(i).at(j)->set_fall_pos();
		}
		deleting_stage = 4;
	}
	if (deleting_stage == 4)
	{
		int flag = 1;
		//float elapsed = clc.restart().asMilliseconds();
		for (int i = 0; i < combination.size(); i++)
		{
			for (int j = 0; j < combination.at(i).y; j++)
			{
				if (gems.at(combination.at(i).x).at(j)->fall_down(elapsed) == 0)
				{
					flag = 0;
					//gems.at(combination.at(i).x).at(j)->count_of_deleting_blocks_under = 0;
				}
			}
		}
		if (flag == 1)
		{
			deleting_stage = 5;
			for (int i = 0; i < count_of_gems; i++)
			{
				for (int j = 0; j < count_of_gems; j++)
				{
					gems.at(i).at(j)->number_y = j;
				}

			}

		}
	}
	if (deleting_stage == 5)
	{
		
		for (int i = 0; i < count_of_gems; i++)
		{
			for (int j = count_of_gems - 1; j >= 0; j--)
			{
				if (gems.at(i).at(j)->count_of_deleting_blocks_under > 0)
					gems.at(i).at(j + gems.at(i).at(j)->count_of_deleting_blocks_under) = move(gems.at(i).at(j));
				else if (gems.at(i).at(j)->count_of_deleting_blocks_under < 0)
				{
					dead_gems.push_back(move(gems.at(i).at(j)));

				}
			}

		}
		deleting_stage = 6;
	}
	if (deleting_stage == 6) // create new gems
	{
		for (int i = 0; i < count_of_gems; i++)
		{
			for (int j = 0; j < count_of_deleted_blocks.at(i); j++)
				gems.at(i).at(count_of_deleted_blocks.at(i) - 1 - j) = make_unique<Gem>(i, count_of_deleted_blocks.at(i) - 1 - j, 1);
		}
		
		for (int i = 0; i < count_of_gems; i++)
		{
			for (int j = 0; j < count_of_gems; j++)
			{
				if (abs(gems.at(i).at(j)->fall_pos - gems.at(i).at(j)->get_position().y) < 0.1)
					gems.at(i).at(j)->count_of_deleting_blocks_under = 0;
			
			}
		
		}

		for (int i = 0; i < dead_gems.size(); i++) 
			dead_gems.at(i)->death(*this);
		dead_gems.clear();
		if (burst_combination.size())
		{
			combination = burst_combination;
			burst_combination.clear();
			deleting_stage = 1;
			is_burst = 1;
		}
		else
		{
			combination.clear();
			deleting_stage = 7;
		}
	}
	if (deleting_stage == 7)
	{
		int flag = 1;
		//float elapsed = clc.restart().asMilliseconds();
		for (int i = 0; i < count_of_gems; i++)
		{
			for (int j = 0; j < count_of_deleted_blocks.at(i); j++)
			{
				if (gems.at(i).at(count_of_deleted_blocks.at(i) - 1 - j)->fall_down(elapsed) == 0)
					flag = 0;
			}
		}

		if (flag == 1)
		{
			
			for (int i = 0; i < count_of_gems; i++)
			{
				count_of_deleted_blocks.at(i) = 0;
				for (int j = 0; j < count_of_gems; j++)
					gems.at(i).at(j)->count_of_deleting_blocks_under = 0;
			}
			
			deleting_stage = 0;
			flag = 0;
		}
	}
	
}


void GemsField::test_gem(int x, int y)
{
	if (gems_colors.at(x).at(y) == -1)
		return;
	if (current_color == -1)
	{
		current_color = gems_colors.at(x).at(y);
		gems_colors.at(x).at(y) = -1;
		collector.push_back(Vector2i(x, y));
	}
	else if (current_color != gems_colors.at(x).at(y))
		return;
	else
	{
		gems_colors.at(x).at(y) = -1;
		collector.push_back(Vector2i(x, y));
	}

	if (x > 0)
		test_gem(x - 1, y);
	if (y > 0)
		test_gem(x, y - 1);
	if (x < count_of_gems - 1)
		test_gem(x + 1, y);
	if (y < count_of_gems - 1)
		test_gem(x, y + 1);

}


void GemsField::search_combination()
{

	current_color = -1;
	collector.clear();

	for (int i = 0; i < count_of_gems; i++)
	{
		for (int j = 0; j < count_of_gems; j++)
			gems_colors.at(i).at(j) = gems.at(i).at(j)->color;
	}

	for (int x = 0; x < count_of_gems; x++)
	{
		for (int y = 0; y < count_of_gems; y++)
		{
			test_gem(x, y);
			if (collector.size() >= 3)
			{
				for (int i = 0; i < collector.size(); i++)
					combination.push_back(collector.at(i));
			}
			current_color = -1;
			collector.clear();
		}

	}
	

}


void GemsField:: update(Clock& clc)
{
	if (deleting_stage == 0)
	{
		search_combination();
		if (combination.size() > 0)
			deleting_stage = 1;
	}
	delete_combination(clc);
}
