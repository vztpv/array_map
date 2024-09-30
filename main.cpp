
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <map>
#include <random>
#include <chrono>
#include <algorithm>
#include "array_map.h"


inline long long Rand() noexcept {
	return rand(); // (rand() & 255) + ((rand() & 255) << 8) + ((rand() & 255) << 16) + ((rand() & 255) << 24);
}

int main(void)
{
	try {
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();


		std::vector<int> x(10240000, 0);
		//std::sort(x.begin(), x.end());
		long long max = 0;

		for (int i = 0; i < x.size(); ++i) {
			x[i] = i;
			if (x[i] > max) {
				max = x[i];
			}
		}

		std::default_random_engine rng(seed);
		std::shuffle(x.begin(), x.end(), rng);

		int a = clock();
		wiz::array_map<int, int> map;
		map.reserve2(x.size());
		for (int i = 0; i < x.size(); ++i) {
			map.lazy_insert(std::make_pair(x[i], x[i] + 1));
			//map[x[i]];
		}
		map[max] = 0;
		int b = clock();

		//map.PrintTree();
		std::cout << "array_map random_insert ";
		std::cout << b - a << "ms\n";

		std::cout << "------------------------\n";
		a = clock();
		std::map<int, int> map2;
		for (int i = 0; i < x.size(); ++i) {
			map2.insert(std::make_pair(x[i], x[i] + 1));
		}
		b = clock();

		//map2[max]= 0;
		std::cout << "std::map random_insert ";
		std::cout << b - a << "ms\n";
		std::cout << "------------------------\n";



		{
			a = clock();
			auto map2 = std::map<int, int>();
			auto pairs = std::vector<std::pair<int, int>>();
			pairs.reserve(x.size());
			for (int i = 0; i < x.size(); ++i) {
				pairs.push_back({ x[i] , x[i] + 1 });
			}
			std::stable_sort(std::execution::par, pairs.begin(), pairs.end());
			int c = clock();
			map2.insert(pairs.begin(), pairs.end());

			b = clock();

			std::cout << "std::map insert2 ";
			std::cout << b - a << "ms\t" << b-c << "ms\t" << c-a <<"ms\n";
			std::cout << map2[max] << " ";
			std::cout << "------------------------\n";

			a = clock();
			for (int i = 0; i < x.size(); ++i) {
				if (map2.end() == map2.find(x[i])) {
					return -2;
				}
			}
			b = clock();

			std::cout << "std::map random_search? ";
			std::cout << b - a << "ms\n";
			std::cout << "------------------------\n";
		}

		std::shuffle(x.begin(), x.end(), std::default_random_engine(seed));

		a = clock();
		for (int i = 0; i < x.size(); ++i) {
			if (map.end() == map.find(x[i])) {
				return -1;
			}
		}
		b = clock();

		std::cout << "array_map random_search ";
		std::cout << b - a << "ms\n";
		std::cout << "------------------------\n";

		a = clock();
		for (int i = 0; i < x.size(); ++i) {
			if (map2.end() == map2.find(x[i])) {
				return -2;
			}
		}
		b = clock();

		std::cout << "std::map random_search ";
		std::cout << b - a << "ms\n";
		std::cout << "------------------------\n";

		{
			a = clock();

				auto map1 = wiz::array_map<int, int>();
				for (int i = 0; i < x.size(); ++i) {
					map1.insert(std::make_pair(i, i));
				}
				map1[max] = 0;
			
			b = clock();

			std::cout << "array_map insert-sorted_order";
			std::cout << b - a << "ms\n";
			std::cout << "------------------------\n";

			a = clock();
			for (int i = 0; i < x.size(); ++i) {
				if (map1.end() == map1.find(x[i])) {
					return -1;
				}
			}
			b = clock();

			std::cout << "array_map random_search ";
			std::cout << b - a << "ms\n";
			std::cout << "------------------------\n";


		}
		{
			a = clock();
				auto map2 = std::map<int, int>();
				for (int i = 0; i < x.size(); ++i) {
					map2.insert(std::make_pair(i, i));
				}
			b = clock();

			std::cout << "std::map insert-sorted_order";
			std::cout << b - a << "ms\n";
			std::cout << "------------------------\n";

			a = clock();
			for (int i = 0; i < x.size(); ++i) {
				if (map2.end() == map2.find(x[i])) {
					return -2;
				}
			}
			b = clock();

			std::cout << "std::map random_search ";
			std::cout << b - a << "ms\n";
			std::cout << "------------------------\n";
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}
	return 0;
}

