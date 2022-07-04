#include <iostream>
#include <thread>//для потоков
#include <chrono>//для времени
#include <vector>
#include <string_view>
#include <mutex>

//atomic
//многопоточность

//управляющий поток
//атомарные операции
//гонки данных
//векторизованные вычисления
using namespace std::chrono;

void Fill(std::vector<double>* courses) {
	for (size_t i = 0; i < courses->capacity(); i++)
	{
		std::this_thread::sleep_for(milliseconds(15));
		courses->emplace_back(15 + (double)::rand() / (1 + ::rand()));
	}
}

std::mutex mt;

void Print(std::vector<double>::iterator start,
	std::vector<double>::iterator stop,
	std::string_view name) {
	while (start <= stop)
	{
		std::lock_guard lg(mt);
		std::cout << name << "Value " << *start << '\n';
		
		++start;
	}
}

int main() {

	std::this_thread::get_id();//get_id() возвращает дискриптер потока
	std::this_thread::sleep_for(milliseconds(10));//принимает время для
	//this_thread::sleep_until();
	std::vector<double> udsCource;
	udsCource.reserve(540);
	std::vector<double> euroCource;
	euroCource.reserve(540);
	std::vector<double> phoundCource;
	phoundCource.reserve(540);
	std::thread tr1(Fill, &udsCource);
	std::thread tr2(Fill, &euroCource);
	std::thread tr3(Fill, &phoundCource);
	//Fill(&udsCource);
	//Fill(&euroCource);
	//Fill(&phoundCource);
	tr1.join();
	std::thread tr4(Print, udsCource.begin(),
		udsCource.begin() + 100,
		"USD ");
	std::thread tr5{
		[&tr2,&euroCource]() {
			tr2.join();
			Print(euroCource.begin(),
				euroCource.begin() + 100,
				"Euro ");
		}
	};
	tr3.join();
	Print(phoundCource.begin(),
		phoundCource.begin() + 100,
		"Phound ");
	tr4.join();
	tr5.join();


	return 0;
}