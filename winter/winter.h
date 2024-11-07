#pragma once

#ifdef WINTER_EXPORTS
#define WINTER_API __declspec(dllexport)
#else
#define WINTER_API __declspec(dllimport)
#endif

#include <random>
#include <vector>

constexpr char up_flag{ 0b00000001 };
constexpr char down_flag{ 0b00000010 };
constexpr char left_flag{ 0b00000100 };
constexpr char right_flag{ 0b00001000 };

constexpr char evil1_flag{ 0b00000001 };
constexpr char evil2_flag{ 0b00000010 };
constexpr char evil3_flag{ 0b00000100 };
constexpr char evil4_flag{ 0b00001000 };
constexpr char evil5_flag{ 0b00010000 };

constexpr char turret1_flag{ 0b00000001 };
constexpr char turret2_flag{ 0b00000010 };
constexpr char turret3_flag{ 0b00000100 };
constexpr char turret4_flag{ 0b00001000 };
constexpr char turret5_flag{ 0b00010000 };
constexpr char turret6_flag{ 0b00100000 };

constexpr float scr_width(800.0f);
constexpr float scr_height(700.0f);
constexpr float sky(50.0f);
constexpr float ground(650.0f);

namespace game
{
	class WINTER_API RANDERER
	{
		private:
			std::random_device rd{};
			std::seed_seq* sequencer = nullptr;
			std::vector<unsigned int> vSeeds;
			std::mt19937* twister = nullptr;
	
		public:
			RANDERER();
			~RANDERER();

			int operator() (int min, int max);
	};

	class WINTER_API SIMPLE
	{
		protected:
			float width{ 1.0f };
			float height{ 1.0f };

		public:
			float x{ 0 };
			float y{ 0 };
			float ex{ 0 };
			float ey{ 0 };

			SIMPLE(float _x = 0, float _y = 0, float _width = 1.0f, float _height = 1.0f);
			virtual ~SIMPLE() {};

			float GetWidth() const;
			float GetHeight() const;
			void SetWidth(float new_width);
			void SetHeight(float new_height);

			void NewDims(float new_width, float new_height);
			void SetEdges();
	};

	class WINTER_API SIMPLE_PACK
	{
		private:
			SIMPLE* base_ptr{ nullptr };
			size_t lenght{ 0 };
			size_t next_position{ 0 };
			bool state_of_pack = false;

		public:

			SIMPLE_PACK(size_t size) :lenght{ size }, base_ptr{ new SIMPLE[size] }
			{
				if (size > 0)state_of_pack = true;
			}
			~SIMPLE_PACK();

			bool IsValid() const;
			size_t FreeCapacity() const;

			void push_back(SIMPLE& element);
			void push_front(SIMPLE& element);
			SIMPLE& operator[](size_t position);
			void operator()(SIMPLE& element, size_t position);
	};


}
