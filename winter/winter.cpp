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