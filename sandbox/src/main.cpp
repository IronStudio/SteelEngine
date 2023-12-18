#include <chrono>
#include <iostream>
#include <smmintrin.h>

#include <se/utils/assert.hpp>
#include <se/math/vector.hpp>
#include <se/math/vec4.hpp>


#define PRINT_VECTOR(vector, D) for (size_t i {0}; i < D; ++i) std::cout << vector[i] << ", "; std::cout << std::endl;

struct Clock
{
	Clock() :
		start {std::chrono::high_resolution_clock::now()}
	{

	}

	~Clock()
	{
		std::cout << "CLOCK : " << std::chrono::duration_cast<std::chrono::duration<float, std::milli>> (std::chrono::high_resolution_clock::now() - start).count() << " ms" << std::endl;
	}


	std::chrono::high_resolution_clock::time_point start;
};


int main(int, char **)
{
	try
	{
		srand(time(nullptr));

		se::Vector<int, 3> vector {};
		PRINT_VECTOR(vector, 3)
		se::Vector<float, 4> vector2 {1.};
		PRINT_VECTOR(vector2, 4)
		se::Vector<double, 5> vector3 {1.5f, 2, -4., 5.f, 0};
		PRINT_VECTOR(vector3, 5)
		se::Vector<double, 5> vector4 {vector2};
		PRINT_VECTOR(vector4, 5)
		se::Vector<float, 4> vector5 {0.5f, 10.f, 291.f, -38.f};
		PRINT_VECTOR(vector5, 4)

		vector4 = vector3;
		PRINT_VECTOR(vector4, 5)

		vector[1] = 42;
		PRINT_VECTOR(vector, 3)

		vector2 += vector5;
		PRINT_VECTOR(vector2, 4)

		vector3 += vector4;
		PRINT_VECTOR(vector3, 5)

		vector *= 2.f;
		PRINT_VECTOR(vector, 3)

		PRINT_VECTOR(vector2, 4)
		PRINT_VECTOR(vector5, 4)
		std::cout << "Dot : " << se::dot(vector2, vector5) << std::endl;

		se::Vector<float, 2> polar {cosf(2.3f), sinf(2.3f)};
		std::cout << polar.length2() << ", " << polar.length() << std::endl;

		
		se::Vec4<float> vec4 {1.f};
		vec4.x = 10.f;
		vec4.y = -3.f;
		vec4.z = -19.42f;
		vec4.w = 24.29f;
		std::cout << vec4.r << ", " << vec4.g << ", " << vec4.b << ", " << vec4.a << std::endl;



		std::vector<float> vectors1 {};
		vectors1.resize(1024 * 1024);
		std::vector<float> vectors2 {};
		vectors2.resize(1024 * 1024);
		std::vector<float> results1 {};
		results1.resize(1024 * 1024 / 4);
		std::vector<float> results2 {};
		results2.resize(1024 * 1024 / 4);

		for (auto &vec : vectors1)
			vec = (float)rand() / (float)RAND_MAX * 100.f;

		for (auto &vec : vectors2)
			vec = (float)rand() / (float)RAND_MAX * 100.f;


		{
			Clock _ {};

			for (size_t i {0}; i < (1024 * 1024); i += 4)
			{
				results1[i / 4] = 0;

				for (size_t j {0}; j < 4; ++j)
					results1[i / 4] += vectors1[i + j] * vectors2[i + j];
			}
		}


		{
			Clock _ {};

			for (size_t i {0}; i < (1024 * 1024); i += 4)
			{
				__m128 dst {_mm_load_ps(vectors1.data() + i)};
				__m128 src {_mm_load_ps(vectors2.data() + i)};
				dst = _mm_dp_ps(dst, src, 0b11111111);
				float output[4];
				_mm_store_ps(output, dst);
				results2[i / 4] = output[0];
			}
		}
	}

	catch (const se::Assert &assert)
	{
		std::cout << assert.getMessage() << std::endl;
	}

	return 0;
}