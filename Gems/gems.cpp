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
	swap_stage = SwapStage::None;
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
			if (gems[i][j] != nullptr)
				gems[i][j]->draw(win);

		}
	}
}


Vector2i GemsField:: clicked_gem_number(const Vector2i mouse)
{
	for (int i = 0; i < count_of_gems; i++)
	{
		for (int j = 0; j < count_of_gems; j++)
		{
			double size = gems[i][j]->size / 2;
			Vector2f pos = gems[i][j]->get_position();
			if (pos.x + size >= mouse.x && pos.x - size <= mouse.x && pos.y + size >= mouse.y && pos.y - size <= mouse.y)
				return Vector2i(i, j);
		}
	}
	return Vector2i(-1, -1);
}


void GemsField:: make_chosen(const Vector2i first)
{
	double scale = gems[first.x][first.y]->normal_scale * 1.2;
	gems[first.x][first.y]->set_scale(scale);
}


void GemsField:: make_unchosen(const Vector2i first)
{
	double scale = gems[first.x][first.y]->normal_scale;
	gems[first.x][first.y]->set_scale(scale);
}

bool GemsField::is_near(const Vector2i first, const Vector2i second) const
{
	int differ_x = abs(first.x - second.x);
	int differ_y = abs(first.y - second.y);
	return differ_x == 1 && differ_y == 0 || differ_x == 0 && differ_y == 1;
}

void GemsField:: choosing(RenderWindow& win, Clock &clc)
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
		for (int j = 0; j < combination[i].y; j++)
		{
			gems[combination[i].x][j]->count_of_deleting_blocks_under++;
			
		}
	}
	for (int i = 0; i < combination.size(); i++)
		gems[combination[i].x][combination[i].y]->count_of_deleting_blocks_under = -1;/////
}


bool GemsField::compress(RenderWindow& win, const float elapsed, const Vector2i gem)
{
	if (gems[gem.x][gem.y]->get_scale() > 0)
	{
		gems[gem.x][gem.y]->change_scale(scaling_speed * elapsed / 100);
		return 0;
	}
	else
	{
		gems[gem.x][gem.y]->set_scale(0);
		return 1;
	}
}


void GemsField::swap()
{
	unique_ptr<Gem> tmp = move(gems[chosen1.x][chosen1.y]);
	gems[chosen1.x][chosen1.y] = move(gems[chosen2.x][chosen2.y]);
	gems[chosen2.x][chosen2.y]= move(tmp);

	Vector2f pos = gems[chosen1.x][chosen1.y]->get_position();
	gems[chosen1.x][chosen1.y]->set_position(gems[chosen2.x][chosen2.y]->get_position());
	gems[chosen2.x][chosen2.y]->set_position(pos);
	

}


bool GemsField::decompress(RenderWindow& win, const float elapsed, const Vector2i gem)
{
	if (gems[gem.x][gem.y]->get_scale() < gems[gem.x][gem.y]->normal_scale)
	{
		gems[gem.x][gem.y]->change_scale(-scaling_speed * elapsed / 100);
		return 0;
	}
	else
	{
		gems[gem.x][chosen1.y]->set_scale(gems[gem.x][gem.y]->normal_scale);
		return 1;
	}


}

void GemsField::swap_chosen_gems(RenderWindow& win, Clock& clc)
{

	float elapsed;
	if (swap_stage == SwapStage::Compression)
	{
		elapsed = clc.restart().asMilliseconds();
		if (compress(win, elapsed, chosen1) + compress(win, elapsed, chosen2) == 2)
			swap_stage = SwapStage::Swapping;
	}
	if (swap_stage == SwapStage::Swapping)
	{
		swap();
		swap_stage = SwapStage::Decompression;
	}

	if (swap_stage == SwapStage::Decompression)
	{
		elapsed = clc.restart().asMilliseconds();
		if (decompress(win, elapsed, chosen1) + decompress(win, elapsed, chosen2) == 2)
		{
			swap_stage = SwapStage::None;
			chosen1 = Vector2i(-1, -1);
			chosen2 = Vector2i(-1, -1);
		}
		
	}

}

bool GemsField:: is_in_combination(const Vector2i x) const
{
	for (int i = 0; i < combination.size(); i++)
	{
		if (x == combination[i])
			return 1;
	}
	return 0;
}

bool GemsField::is_in_burst_combination(const Vector2i x) const
{
	for (int i = 0; i < burst_combination.size(); i++)
	{
		if (x == burst_combination[i])
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
			gems[combination[i].x][combination[i].y]->sprite.setTexture(burst);
		is_burst = 0;
		sleep(seconds(1));
	}
	if (deleting_stage == 2)
	{
		for (int i = 0; i < combination.size(); i++)
		{

			if (gems[combination[i].x][combination[i].y]->get_scale() > 0)
				gems[combination[i].x][combination[i].y]->change_scale(scaling_speed * elapsed / 300);
			else
			{
				gems[combination[i].x][combination[i].y]->set_scale(0);
				deleting_stage = 3;
			}
		}
	}
	if (deleting_stage == 3)
	{
		set_count_of_deleting_blocks();
		for (int i = 0; i < combination.size(); i++)
			count_of_deleted_blocks[combination[i].x]++;
		for (int i = 0; i < count_of_gems; i++)
		{
			for (int j = 0; j < count_of_gems; j++)
				gems[i][j]->set_fall_pos();
		}
		deleting_stage = 4;
	}
	if (deleting_stage == 4)
	{
		int flag = 1;
			
		for (int i = 0; i < combination.size(); i++)
		{
			for (int j = 0; j < combination[i].y; j++)
			{
				if (gems[combination[i].x][j]->fall_down(elapsed) == 0)
					flag = 0;
		
			}
		}
		if (flag == 1)
		{
			deleting_stage = 5;
			for (int i = 0; i < count_of_gems; i++)
			{
				for (int j = 0; j < count_of_gems; j++)
				{
					gems[i][j]->number_y = j;
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
				if (gems[i][j]->count_of_deleting_blocks_under > 0)
					gems[i][j + gems[i][j]->count_of_deleting_blocks_under] = move(gems[i][j]);
				else if (gems[i][j]->count_of_deleting_blocks_under < 0)
				{
					dead_gems.push_back(move(gems[i][j]));

				}
			}

		}
		deleting_stage = 6;
	}
	if (deleting_stage == 6) // create new gems
	{
		for (int i = 0; i < count_of_gems; i++)
		{
			for (int j = 0; j < count_of_deleted_blocks[i]; j++)
				gems[i][count_of_deleted_blocks[i] - 1 - j] = make_unique<Gem>(i, count_of_deleted_blocks[i] - 1 - j, 1);
		}
		
		for (int i = 0; i < count_of_gems; i++)
		{
			for (int j = 0; j < count_of_gems; j++)
			{
				if (abs(gems[i][j]->fall_pos - gems[i][j]->get_position().y) < 0.1)
					gems[i][j]->count_of_deleting_blocks_under = 0;
			
			}
		
		}

		for (int i = 0; i < dead_gems.size(); i++) 
			dead_gems[i]->death(*this);
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
			for (int j = 0; j < count_of_deleted_blocks[i]; j++)
			{
				if (gems[i][count_of_deleted_blocks[i]- 1 - j]->fall_down(elapsed) == 0)
					flag = 0;
			}
		}

		if (flag == 1)
		{
			
			for (int i = 0; i < count_of_gems; i++)
			{
				count_of_deleted_blocks[i] = 0;
				for (int j = 0; j < count_of_gems; j++)
					gems[i][j]->count_of_deleting_blocks_under = 0;
			}
			
			deleting_stage = 0;
			flag = 0;
		}
	}
	
}


void GemsField::test_gem(const int x, const int y)
{
	if (gems_colors[x][y] == -1)
		return;
	if (current_color == -1)
	{
		current_color = gems_colors[x][y];
		gems_colors[x][y]= -1;
		collector.push_back(Vector2i(x, y));
	}
	else if (current_color != gems_colors[x][y])
		return;
	else
	{
		gems_colors[x][y] = -1;
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
			gems_colors[i][j] = gems[i][j]->color;
	}

	for (int x = 0; x < count_of_gems; x++)
	{
		for (int y = 0; y < count_of_gems; y++)
		{
			test_gem(x, y);
			if (collector.size() >= 3)
			{
				for (int i = 0; i < collector.size(); i++)
					combination.push_back(collector[i]);
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
