/*
Copyright 2016 Tyler Winters

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "../include/Decimal.hpp"

#include <assert.h>
#include <iostream>

#define STR_ASSERT(left, oper, right) if(!((left) oper (right))) throw std::runtime_error(__FILE__ ":" STR__LINE__ "- expr(" #left " " #oper " " #right ")  vals(" +(left)+ " " #oper " " +(right)+ ")")
#define DEC_ASSERT(left, oper, right) if(!((left) oper (right))) throw std::runtime_error(__FILE__ ":" STR__LINE__ "- expr(" #left " " #oper " " #right ")  vals(" +to_string(left)+ " " #oper " " +to_string(right)+ ")")

void decimalUnitTests()
{
	using namespace tylawin::DataTypes;

	STR_ASSERT( to_string(Decimal(0)),		==,	std::string("0")	);
	STR_ASSERT( to_string(Decimal(222)),		==,	std::string("222")	);
	STR_ASSERT( to_string(Decimal(-222)),		==,	std::string("-222")	);
	STR_ASSERT( to_string(Decimal(222.222L)),	==,	std::string("222.222")	);
	STR_ASSERT( to_string(Decimal(-222.222L)),	==,	std::string("-222.222")	);
	STR_ASSERT( to_string(Decimal("0")),		==,	std::string("0")	);
	STR_ASSERT( to_string(Decimal("1")),		==,	std::string("1")	);
	STR_ASSERT( to_string(Decimal("-1")),		==,	std::string("-1")	);
	STR_ASSERT( to_string(Decimal(".5")),		==,	std::string("0.5")	);
	STR_ASSERT( to_string(Decimal("-.5")),		==,	std::string("-0.5")	);
	STR_ASSERT( to_string(Decimal("1.1")),		==,	std::string("1.1")	);
	STR_ASSERT( to_string(Decimal("-1.1")),		==,	std::string("-1.1")	);
	STR_ASSERT( to_string(Decimal("0.01")),		==,	std::string("0.01")	);
	STR_ASSERT( to_string(Decimal("9999999999.99999999")),	==,	std::string("9999999999.99999999")	);

	DEC_ASSERT( Decimal("0"),	==,	Decimal("0")	);
	DEC_ASSERT( Decimal("1"),	==,	Decimal("1")	);
	DEC_ASSERT( Decimal("-1"),	==,	Decimal("-1")	);
	DEC_ASSERT( Decimal("1.1"),	==,	Decimal("1.1")	);
	DEC_ASSERT( Decimal("-1.1"),	==,	Decimal("-1.1")	);

	DEC_ASSERT( Decimal("1"),	!=,	Decimal("0")	);
	DEC_ASSERT( Decimal("0"),	!=,	Decimal("1")	);
	DEC_ASSERT( Decimal("-1"),	!=,	Decimal("0")	);
	DEC_ASSERT( Decimal("0"),	!=,	Decimal("-1")	);
	DEC_ASSERT( Decimal("1.1"),	!=,	Decimal("0")	);
	DEC_ASSERT( Decimal("0"),	!=,	Decimal("1.1")	);
	DEC_ASSERT( Decimal("-1.1"),	!=,	Decimal("0")	);
	DEC_ASSERT( Decimal("0"),	!=,	Decimal("-1.1")	);
	DEC_ASSERT( Decimal("1"),	!=,	Decimal("1.1")	);
	DEC_ASSERT( Decimal("1.1"),	!=,	Decimal("1")	);
	DEC_ASSERT( Decimal("-1"),	!=,	Decimal("-1.1")	);
	DEC_ASSERT( Decimal("-1.1"),	!=,	Decimal("-1")	);

	DEC_ASSERT( Decimal("0"),	>=,	Decimal("0")	);
	DEC_ASSERT( Decimal("1"),	>=,	Decimal("1")	);
	DEC_ASSERT( Decimal("-1"),	>=,	Decimal("-1")	);
	DEC_ASSERT( Decimal("1.1"),	>=,	Decimal("1.1")	);
	DEC_ASSERT( Decimal("-1.1"),	>=,	Decimal("-1.1")	);
	DEC_ASSERT( Decimal("1"),	>=,	Decimal("0")	);
	DEC_ASSERT( Decimal("0"),	>=,	Decimal("-1")	);
	DEC_ASSERT( Decimal("1"),	>=,	Decimal("0.5")	);
	DEC_ASSERT( Decimal("1.5"),	>=,	Decimal("1")	);
	DEC_ASSERT( Decimal("-1"),	>=,	Decimal("-5")	);
	DEC_ASSERT( Decimal("-1.5"),	>=,	Decimal("-1.6")	);
	
	DEC_ASSERT( Decimal("0"),	<=,	Decimal("0")	);
	DEC_ASSERT( Decimal("1"),	<=,	Decimal("1")	);
	DEC_ASSERT( Decimal("-1"),	<=,	Decimal("-1")	);
	DEC_ASSERT( Decimal("1.1"),	<=,	Decimal("1.1")	);
	DEC_ASSERT( Decimal("-1.1"),	<=,	Decimal("-1.1")	);
	DEC_ASSERT( Decimal("0"),	<=,	Decimal("1")	);
	DEC_ASSERT( Decimal("-1"),	<=,	Decimal("0")	);
	DEC_ASSERT( Decimal("0.5"),	<=,	Decimal("1")	);
	DEC_ASSERT( Decimal("1"),	<=,	Decimal("1.5")	);
	DEC_ASSERT( Decimal("-5"),	<=,	Decimal("-1")	);
	DEC_ASSERT( Decimal("-1.6"),	<=,	Decimal("-1.5")	);

	DEC_ASSERT( Decimal("1"),	>,	Decimal("0")	);
	DEC_ASSERT( Decimal("0"),	>,	Decimal("-1")	);
	DEC_ASSERT( Decimal("1"),	>,	Decimal("0.5")	);
	DEC_ASSERT( Decimal("1.5"),	>,	Decimal("1")	);
	DEC_ASSERT( Decimal("-1"),	>,	Decimal("-5")	);
	DEC_ASSERT( Decimal("-1.5"),	>,	Decimal("-1.6")	);

	DEC_ASSERT( Decimal("0"),	<,	Decimal("1")	);
	DEC_ASSERT( Decimal("-1"),	<,	Decimal("0")	);
	DEC_ASSERT( Decimal("0.5"),	<,	Decimal("1")	);
	DEC_ASSERT( Decimal("1"),	<,	Decimal("1.5")	);
	DEC_ASSERT( Decimal("-5"),	<,	Decimal("-1")	);
	DEC_ASSERT( Decimal("-1.6"),	<,	Decimal("-1.5")	);

	DEC_ASSERT( Decimal("0"),	==,	-Decimal("0")		);
	DEC_ASSERT( Decimal("1"),	==,	-Decimal("-1")		);
	DEC_ASSERT( Decimal("-1"),	==,	-Decimal("1")		);
	DEC_ASSERT( Decimal("1.1"),	==,	-Decimal("-1.1")	);
	DEC_ASSERT( Decimal("-1.1"),	==,	-Decimal("1.1")		);

	STR_ASSERT( to_string(Decimal("0") + Decimal("0")),		==,	std::string("0")	);
	STR_ASSERT( to_string(Decimal("1") + Decimal("0")),		==,	std::string("1")	);
	STR_ASSERT( to_string(Decimal("0") + Decimal("1")),		==,	std::string("1")	);
	STR_ASSERT( to_string(Decimal("1") + Decimal("1")),		==,	std::string("2")	);
	STR_ASSERT( to_string(Decimal("1.1") + Decimal("0")),		==,	std::string("1.1")	);
	STR_ASSERT( to_string(Decimal("0") + Decimal("1.1")),		==,	std::string("1.1")	);
	STR_ASSERT( to_string(Decimal("1.1") + Decimal("1.1")),		==,	std::string("2.2")	);
	STR_ASSERT( to_string(Decimal("-1") + Decimal("0")),		==,	std::string("-1")	);
	STR_ASSERT( to_string(Decimal("0") + Decimal("-1")),		==,	std::string("-1")	);
	STR_ASSERT( to_string(Decimal("-1") + Decimal("-1")),		==,	std::string("-2")	);
	STR_ASSERT( to_string(Decimal("-1.1") + Decimal("0")),		==,	std::string("-1.1")	);
	STR_ASSERT( to_string(Decimal("0") + Decimal("-1.1")),		==,	std::string("-1.1")	);
	STR_ASSERT( to_string(Decimal("-1.1") + Decimal("-1.1")),	==,	std::string("-2.2")	);
	STR_ASSERT( to_string(Decimal("1.9") + Decimal("1.9")),		==,	std::string("3.8")	);
	STR_ASSERT( to_string(Decimal("1.9") + Decimal("-1.9")),	==,	std::string("0")	);
	STR_ASSERT( to_string(Decimal("-1.9") + Decimal("1.9")),	==,	std::string("0")	);
	STR_ASSERT( to_string(Decimal("-1.9") + Decimal("-1.9")),	==,	std::string("-3.8")	);
	STR_ASSERT( to_string(Decimal("1.9") + Decimal("-1.5")),	==,	std::string("0.4")	);
	STR_ASSERT( to_string(Decimal("1.5") + Decimal("-1.9")),	==,	std::string("-0.4")	);
	STR_ASSERT( to_string(Decimal("-1.9") + Decimal("1.5")),	==,	std::string("-0.4")	);
	STR_ASSERT( to_string(Decimal("-1.5") + Decimal("1.9")),	==,	std::string("0.4")	);

	STR_ASSERT( to_string(Decimal("0") - Decimal("0")),		==,	std::string("0")	);
	STR_ASSERT( to_string(Decimal("1") - Decimal("0")),		==,	std::string("1")	);
	STR_ASSERT( to_string(Decimal("0") - Decimal("1")),		==,	std::string("-1")	);
	STR_ASSERT( to_string(Decimal("1") - Decimal("1")),		==,	std::string("0")	);
	STR_ASSERT( to_string(Decimal("1.1") - Decimal("0")),		==,	std::string("1.1")	);
	STR_ASSERT( to_string(Decimal("0") - Decimal("1.1")),		==,	std::string("-1.1")	);
	STR_ASSERT( to_string(Decimal("1.1") - Decimal("1.1")),		==,	std::string("0")	);
	STR_ASSERT( to_string(Decimal("-1") - Decimal("0")),		==,	std::string("-1")	);
	STR_ASSERT( to_string(Decimal("0") - Decimal("-1")),		==,	std::string("1")	);
	STR_ASSERT( to_string(Decimal("-1") - Decimal("-1")),		==,	std::string("0")	);
	STR_ASSERT( to_string(Decimal("-1.1") - Decimal("0")),		==,	std::string("-1.1")	);
	STR_ASSERT( to_string(Decimal("0") - Decimal("-1.1")),		==,	std::string("1.1")	);
	STR_ASSERT( to_string(Decimal("-1.1") - Decimal("-1.1")),	==,	std::string("0")	);
	STR_ASSERT( to_string(Decimal("1.9") - Decimal("1.9")),		==,	std::string("0")	);
	STR_ASSERT( to_string(Decimal("1.9") - Decimal("-1.9")),	==,	std::string("3.8")	);
	STR_ASSERT( to_string(Decimal("-1.9") - Decimal("1.9")),	==,	std::string("-3.8")	);
	STR_ASSERT( to_string(Decimal("-1.9") - Decimal("-1.9")),	==,	std::string("0")	);
	STR_ASSERT( to_string(Decimal("1.9") - Decimal("-1.5")),	==,	std::string("3.4")	);
	STR_ASSERT( to_string(Decimal("1.5") - Decimal("-1.9")),	==,	std::string("3.4")	);
	STR_ASSERT( to_string(Decimal("-1.9") - Decimal("1.5")),	==,	std::string("-3.4")	);
	STR_ASSERT( to_string(Decimal("-1.5") - Decimal("1.9")),	==,	std::string("-3.4")	);

	STR_ASSERT( to_string(Decimal("0") * Decimal("0")),		==,	std::string("0")	);
	STR_ASSERT( to_string(Decimal("1") * Decimal("0")),		==,	std::string("0")	);
	STR_ASSERT( to_string(Decimal("0") * Decimal("1")),		==,	std::string("0")	);
	STR_ASSERT( to_string(Decimal("1") * Decimal("1")),		==,	std::string("1")	);
	STR_ASSERT( to_string(Decimal("1.1") * Decimal("0")),		==,	std::string("0")	);
	STR_ASSERT( to_string(Decimal("0") * Decimal("1.1")),		==,	std::string("0")	);
	STR_ASSERT( to_string(Decimal("1.1") * Decimal("1.1")),		==,	std::string("1.21")	);
	STR_ASSERT( to_string(Decimal("-1") * Decimal("0")),		==,	std::string("0")	);
	STR_ASSERT( to_string(Decimal("0") * Decimal("-1")),		==,	std::string("0")	);
	STR_ASSERT( to_string(Decimal("-1") * Decimal("-1")),		==,	std::string("1")	);
	STR_ASSERT( to_string(Decimal("-1.1") * Decimal("0")),		==,	std::string("0")	);
	STR_ASSERT( to_string(Decimal("0") * Decimal("-1.1")),		==,	std::string("0")	);
	STR_ASSERT( to_string(Decimal("-1.1") * Decimal("-1.1")),	==,	std::string("1.21")	);
	STR_ASSERT( to_string(Decimal("1.9") * Decimal("1.9")),		==,	std::string("3.61")	);
	STR_ASSERT( to_string(Decimal("1.9") * Decimal("-1.9")),	==,	std::string("-3.61")	);
	STR_ASSERT( to_string(Decimal("-1.9") * Decimal("1.9")),	==,	std::string("-3.61")	);
	STR_ASSERT( to_string(Decimal("-1.9") * Decimal("-1.9")),	==,	std::string("3.61")	);
	STR_ASSERT( to_string(Decimal("1.9") * Decimal("-1.5")),	==,	std::string("-2.85")	);
	STR_ASSERT( to_string(Decimal("1.5") * Decimal("-1.9")),	==,	std::string("-2.85")	);
	STR_ASSERT( to_string(Decimal("-1.9") * Decimal("1.5")),	==,	std::string("-2.85")	);
	STR_ASSERT( to_string(Decimal("-1.5") * Decimal("1.9")),	==,	std::string("-2.85")	);
	STR_ASSERT( to_string(Decimal("1.9999999999999999999") * Decimal("1.9999999999999999999")),	==,	std::string("3.9999999999999999996")	);
	STR_ASSERT( to_string(Decimal("1.9999999999999999999") * Decimal("-1.9999999999999999999")),	==,	std::string("-3.9999999999999999996")	);
	STR_ASSERT( to_string(Decimal("-1.9999999999999999999") * Decimal("1.9999999999999999999")),	==,	std::string("-3.9999999999999999996")	);
	STR_ASSERT( to_string(Decimal("-1.9999999999999999999") * Decimal("-1.9999999999999999999")),	==,	std::string("3.9999999999999999996")	);

	STR_ASSERT( to_string(Decimal("0") / Decimal("1")),	==,	std::string("0")	);
	STR_ASSERT( to_string(Decimal("1") / Decimal("1")),	==,	std::string("1")	);
	STR_ASSERT( to_string(Decimal("3") / Decimal("1")),	==,	std::string("3")	);
	STR_ASSERT( to_string(Decimal("1") / Decimal("3")),	==,	std::string("0.3333333333333333333")	);
	STR_ASSERT( to_string(Decimal("1") / Decimal("-3")),	==,	std::string("-0.3333333333333333333")	);
	STR_ASSERT( to_string(Decimal("4") / Decimal("3")),	==,	std::string("1.3333333333333333333")	);
	STR_ASSERT( to_string(Decimal("-4") / Decimal("3")),	==,	std::string("-1.3333333333333333333")	);
	STR_ASSERT( to_string(Decimal("1.3") / Decimal("1")),	==,	std::string("1.3")	);
	STR_ASSERT( to_string(Decimal("-1.3") / Decimal("-1")),	==,	std::string("1.3")	);
	STR_ASSERT( to_string(Decimal("1.3") / Decimal("1.3")),	==,	std::string("1")	);
	STR_ASSERT( to_string(Decimal("-1.3") / Decimal("1.3")),==,	std::string("-1")	);
	STR_ASSERT( to_string(Decimal("1") / Decimal("0.5")),	==,	std::string("2")	);
	STR_ASSERT( to_string(Decimal("1") / Decimal("-0.5")),	==,	std::string("-2")	);
	STR_ASSERT( to_string(Decimal("5.55555555") / Decimal("6")),	==,	std::string("0.925925925")		);
	STR_ASSERT( to_string(Decimal("6") / Decimal("5.55555555")),	==,	std::string("1.080000001080000001")	);
	STR_ASSERT( to_string(Decimal("7.5555") / Decimal("3.3333")),	==,	std::string("2.2666726667266672666")	);

	Decimal t1, t2;

	t1 = Decimal("2");
	std::stringstream ss;
	ss << t1;
	ss >> t2;
	assert(t1 == t2 && "istream ostream test");
	std::stringstream ss2;
	t1 = Decimal("-2.2");
	ss2 << t1;
	ss2 >> t2;
	assert(t1 == t2 && "istream ostream test2");
	std::stringstream ss3;
	t1 = Decimal("-20000.99999999");
	ss3 << t1;
	ss3 >> t2;
	assert(t1 == t2 && "istream ostream test3");
}

int main(int argc, char **argv)
{
	decimalUnitTests();
	std::cout << "success!" << std::endl;
	return 0;
}
