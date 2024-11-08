#include "pch.h"
#include "winter.h"

//RANDERER ******************

game::RANDERER::RANDERER()
{
	vSeeds.clear();
	for (int i = 0; i < 650; i++)vSeeds.push_back(rd());
	sequencer = new std::seed_seq{ vSeeds.begin(),vSeeds.end() };
	twister = new std::mt19937(*sequencer);
}
game::RANDERER::~RANDERER()
{
	if (sequencer)delete sequencer;
	if (twister)delete twister;
}
int game::RANDERER::operator()(int min, int max)
{
	std::uniform_int_distribution one_number{ min, max };
	return(one_number(*twister));
}

////////////////////////////////

//SIMPLE **********************

game::SIMPLE::SIMPLE(float _x, float _y, float _width, float _height)
{
	x = _x;
	y = _y;
	width = _width;
	height = _height;
	ex = x + width;
	ey = y + height;
}
float game::SIMPLE::GetWidth() const
{
	return width;
}
float game::SIMPLE::GetHeight() const
{
	return height;
}
void game::SIMPLE::SetWidth(float new_width)
{
	width = new_width;
	ex = x + width;
}
void game::SIMPLE::SetHeight(float new_height)
{
	height = new_height;
	ey = y + height;
}
void game::SIMPLE::NewDims(float new_width, float new_height)
{
	width = new_width;
	height = new_height;
	ex = x + width;
	ey = y + height;
}
void game::SIMPLE::SetEdges()
{
	ex = x + width;
	ey = y + height;
}

/////////////////////////////////

//SIMPLE_PACK *******************

bool game::SIMPLE_PACK::IsValid() const
{
	return state_of_pack;
}
size_t game::SIMPLE_PACK::FreeCapacity()const
{
	return (lenght - next_position);
}
size_t game::SIMPLE_PACK::size() const
{
	return lenght;
}
game::SIMPLE_PACK::~SIMPLE_PACK()
{
	if (state_of_pack)delete[]base_ptr;
}
void game::SIMPLE_PACK::push_back(SIMPLE& element)
{
	if (next_position + 1 <= lenght)
	{
		*(base_ptr + next_position) = element;
		++next_position;
	}
}
void game::SIMPLE_PACK::push_front(SIMPLE& element)
{
	if (state_of_pack) *(base_ptr) = element;
}
game::SIMPLE& game::SIMPLE_PACK::operator[](size_t position)
{
	if (state_of_pack && position >= 0 && position < lenght) return *(base_ptr + position);
	else
	{
		SIMPLE err_ret{};
		return err_ret;
	}
}
void game::SIMPLE_PACK::operator()(SIMPLE& element, size_t position)
{
	if (state_of_pack && position >= 0 && position < lenght) *(base_ptr + position) = element;
}

///////////////////////////////////

//CREATURE ***********************

game::CREATURE::CREATURE(float _sx, float _sy) {};
void game::CREATURE::SetPathInfo(float _endx, float _endy)
{
	hor_path = false;
	vert_path = false;

	move_x = x; 
	move_y = y;
	move_ex = _endx;
	move_ey = _endy;

	if (move_ex - move_x == 0)
	{
		vert_path = true;
		return;
	}
	if (move_ey - move_y == 0)
	{
		hor_path = true;
		return;
	}
	
	slope = (move_ey - move_y) / (move_ex - move_x);
	intercept = move_y - slope * move_x;
}
int game::CREATURE::GetDistance(GAME_COORD first_point, GAME_COORD second_point)
{
	int catet1 = abs(second_point.x - first_point.x);
	int catet2 = abs(second_point.y - first_point.y);

	if (catet1 == 0)return catet2;
	if (catet2 == 0)return catet1;

	return static_cast<int>(sqrt(pow(catet1, 2) + pow(catet2, 2)));
}
bool game::CREATURE::GetFlag(unsigned char which_flag) const
{
	return flags & which_flag;
}
void game::CREATURE::SetFlag(unsigned char which_flag)
{
	flags |= which_flag;
}
void game::CREATURE::NullFlag(unsigned char which_flag)
{
	flags &= ~which_flag;
}
GAME_COORD game::CREATURE::SortPack(SIMPLE_PACK& Package)
{
	GAME_COORD ret{};

	if (Package.size() > 2)
	{
		bool is_ok = false;

		while (!is_ok)
		{
			is_ok = true;
			for (size_t pos = 0; pos < Package.size() - 2; ++pos)
			{
				int dist1 = GetDistance(GAME_COORD(Package[pos].x, Package[pos].y), 
					GAME_COORD(Package[pos + 1].x, Package[pos + 1].y));
				int dist2 = GetDistance(GAME_COORD(Package[pos].x, Package[pos].y),
					GAME_COORD(Package[pos + 2].x, Package[pos + 2].y));

				if (dist1 > dist2)
				{
					game::SIMPLE temp_simple{ Package[pos + 1] };
					Package[pos + 1] = Package[pos + 2];
					Package[pos + 2] = temp_simple;
					is_ok = false;
				}
			}
		}
	}
	ret.x = Package[0].x;
	ret.y = Package[0].y;
}


//EVILS **************************

game::EVILS::EVILS(unsigned char _type, float _sx, float _sy):CREATURE(_sx,_sy)
{
	flags |= _type;
	switch (flags)
	{
	case evil1_flag:
		lifes = 50;
		speed = 0.6f;
		strenght = 3;
		max_frames = 7;
		frame_delay = 10;
		attack_delay = 50;
		break;

	case evil2_flag:
		lifes = 60;
		speed = 0.5f;
		strenght = 4;
		max_frames = 12;
		frame_delay = 6;
		attack_delay = 60;
		break;

	case evil3_flag:
		lifes = 70;
		speed = 0.7f;
		strenght = 6;
		max_frames = 35;
		frame_delay = 2;
		attack_delay = 70;
		break;

	case evil4_flag:
		lifes = 80;
		speed = 0.4f;
		strenght = 7;
		max_frames = 13;
		frame_delay = 6;
		attack_delay = 80;
		break;

	case evil5_flag:
		lifes = 90;
		speed = 0.6f;
		strenght = 10;
		max_frames = 15;
		frame_delay = 5;
		attack_delay = 90;
		break;
	}
}
void game::EVILS::Release()
{
	delete this;
}
int game::EVILS::GetFrame()
{
	--frame_delay;
	if (frame_delay < 0)
	{
		switch (flags)
		{
		case evil1_flag:
			frame_delay = 10;
			break;

		case evil2_flag:
			frame_delay = 6;
			break;

		case evil3_flag:
			frame_delay = 2;
			break;

		case evil4_flag:
			frame_delay = 6;
			break;

		case evil5_flag:
			frame_delay = 5;
			break;
		}
		++current_frame;
		if (current_frame > max_frames)current_frame = 0;
	}
	return current_frame;
}
int game::EVILS::Attack()
{
	--attack_delay;
	if (attack_delay < 0)
	{
		switch (flags)
		{
		case evil1_flag:
			attack_delay = 50;
			break;

		case evil2_flag:
			attack_delay = 60;
			break;

		case evil3_flag:
			attack_delay = 70;
			break;

		case evil4_flag:
			attack_delay = 80;
			break;

		case evil5_flag:
			attack_delay = 90;
			break;
		}
		return strenght;
	}
	return 0;
}
void game::EVILS::Transform(unsigned char to_what)
{
	flags |= to_what;
	switch (flags)
	{
	case evil1_flag:
		lifes = 50;
		speed = 0.6f;
		strenght = 3;
		max_frames = 7;
		frame_delay = 10;
		attack_delay = 50;
		break;

	case evil2_flag:
		lifes = 60;
		speed = 0.5f;
		strenght = 4;
		max_frames = 12;
		frame_delay = 6;
		attack_delay = 60;
		break;

	case evil3_flag:
		lifes = 70;
		speed = 0.7f;
		strenght = 6;
		max_frames = 35;
		frame_delay = 2;
		attack_delay = 70;
		break;

	case evil4_flag:
		lifes = 80;
		speed = 0.4f;
		strenght = 7;
		max_frames = 13;
		frame_delay = 6;
		attack_delay = 80;
		break;

	case evil5_flag:
		lifes = 90;
		speed = 0.6f;
		strenght = 10;
		max_frames = 15;
		frame_delay = 5;
		attack_delay = 90;
		break;
	}
}


//FACTORIES ***********************

game::EVILS* game::EvilFactory(unsigned char _type, float _sx, float _sy)
{
	return new game::EVILS(_type, _sx, _sy);
}