#include "../pch.h"
#include <chrono>

/* Public Methods */
Motor::Motor(const int &port1, const int &port2, const int &port3, const int &port4) { p1 = port1; p2 = port2; p3 = port3; p4 = port4; }

void Motor::setmimic(Motor *mimi) { mimic = mimi; }
void Motor::setmirror(Motor *mirr) { mirror = mirr; }

void Motor::queue(const long double &centimeters) { Biker::push(QueuedAction(centimeters, this)); }
void Motor::moveint(const long double &centimeters) {
	for (long long ind = (long long) std::round(std::abs(centimeters)*100); ind > 0; --ind) {
		using namespace std::literals::chrono_literals;
		moveone(centimeters < 0 ? true : false);
		//if (mimic) mimic->moveone(centimeters < 0 ? true : false); //null pointers become FALSE automatically
		if (mirror) mirror->moveone(centimeters < 0 ? false : true);
		std::this_thread::sleep_for(1ms);
	}

	//Cleaup after ourselves
	GPIO::set(p1, false); GPIO::set(p2, false); GPIO::set(p3, false); GPIO::set(p4, false);
}

void Motor::moveone(const bool &reversed) {
	if (step == 0) {
		//1 0 0 0
		GPIO::set(p1, true); GPIO::set(p2, false); GPIO::set(p3, false); GPIO::set(p4, false);
	} else if (step == 1) {
		//1 1 0 0
		GPIO::set(p1, true); GPIO::set(p2, true); GPIO::set(p3, false); GPIO::set(p4, false);
	} else if (step == 2) {
		//0 1 0 0
		GPIO::set(p1, false); GPIO::set(p2, true); GPIO::set(p3, false); GPIO::set(p4, false);
	} else if (step == 3) {
		//0 1 1 0
		GPIO::set(p1, false); GPIO::set(p2, true); GPIO::set(p3, true); GPIO::set(p4, false);
	} else if (step == 4) {
		//0 0 1 0
		GPIO::set(p1, false); GPIO::set(p2, false); GPIO::set(p3, true); GPIO::set(p4, false);
	} else if (step == 5) {
		//0 0 1 1
		GPIO::set(p1, false); GPIO::set(p2, false); GPIO::set(p3, true); GPIO::set(p4, true);
	} else if (step == 6) {
		//0 0 0 1
		GPIO::set(p1, false); GPIO::set(p2, false); GPIO::set(p3, false); GPIO::set(p4, true);
	} else if (step == 7) {
		//1 0 0 1
		GPIO::set(p1, true); GPIO::set(p2, false); GPIO::set(p3, false); GPIO::set(p4, true);
	}
	if(reversed) step--;
	else step++;

	if (step >= 8) step = 0;
	if (step < 0) step = 7;
}