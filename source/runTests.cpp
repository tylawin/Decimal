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

using namespace tylawin::DataTypes;

#define DASSERT(expr, msg) if(!(expr)) throw std::runtime_error(__FILE__ ":" STR__LINE__ "- expr(" #expr ")  msg(" + (msg) + ")")

void decimalUnitTests()
{
	assert(Decimal(0).asString() == std::string("0"));
	assert(Decimal(222).asString() == std::string("222"));
	assert(Decimal(-222).asString() == std::string("-222"));
	DASSERT(Decimal(222.222L).asString() == std::string("222.222"), Decimal(222.222L).asString());
	assert(Decimal(-222.222L).asString() == std::string("-222.222"));
	assert(Decimal("0").asString() == std::string("0"));
	assert(Decimal("1").asString() == std::string("1"));
	assert(Decimal("-1").asString() == std::string("-1"));
	assert(Decimal(".5").asString() == std::string("0.5"));
	assert(Decimal("-.5").asString() == std::string("-0.5"));
	assert(Decimal("1.1").asString() == std::string("1.1"));
	assert(Decimal("-1.1").asString() == std::string("-1.1"));
	assert(Decimal("0.01").asString() == std::string("0.01"));
	assert(Decimal("9999999999.99999999").asString() == std::string("9999999999.99999999"));

	assert(Decimal("0") == Decimal("0"));
	assert(Decimal("1") == Decimal("1"));
	assert(Decimal("-1") == Decimal("-1"));
	assert(Decimal("1.1") == Decimal("1.1"));
	assert(Decimal("-1.1") == Decimal("-1.1"));

	assert(Decimal("1") != Decimal("0"));
	assert(Decimal("0") != Decimal("1"));
	assert(Decimal("-1") != Decimal("0"));
	assert(Decimal("0") != Decimal("-1"));
	assert(Decimal("1.1") != Decimal("0"));
	assert(Decimal("0") != Decimal("1.1"));
	assert(Decimal("-1.1") != Decimal("0"));
	assert(Decimal("0") != Decimal("-1.1"));
	assert(Decimal("1") != Decimal("1.1"));
	assert(Decimal("1.1") != Decimal("1"));
	assert(Decimal("-1") != Decimal("-1.1"));
	assert(Decimal("-1.1") != Decimal("-1"));

	assert(Decimal("0") >= Decimal("0"));
	assert(Decimal("1") >= Decimal("1"));
	assert(Decimal("-1") >= Decimal("-1"));
	assert(Decimal("1.1") >= Decimal("1.1"));
	assert(Decimal("-1.1") >= Decimal("-1.1"));
	assert(Decimal("1") >= Decimal("0"));
	assert(Decimal("0") >= Decimal("-1"));
	assert(Decimal("1") >= Decimal("0.5"));
	assert(Decimal("1.5") >= Decimal("1"));
	assert(Decimal("-1") >= Decimal("-5"));
	assert(Decimal("-1.5") >= Decimal("-1.6"));
	
	assert(Decimal("0") <= Decimal("0"));
	assert(Decimal("1") <= Decimal("1"));
	assert(Decimal("-1") <= Decimal("-1"));
	assert(Decimal("1.1") <= Decimal("1.1"));
	assert(Decimal("-1.1") <= Decimal("-1.1"));
	assert(Decimal("0") <= Decimal("1"));
	assert(Decimal("-1") <= Decimal("0"));
	assert(Decimal("0.5") <= Decimal("1"));
	assert(Decimal("1") <= Decimal("1.5"));
	assert(Decimal("-5") <= Decimal("-1"));
	assert(Decimal("-1.6") <= Decimal("-1.5"));

	assert(Decimal("1") > Decimal("0"));
	assert(Decimal("0") > Decimal("-1"));
	assert(Decimal("1") > Decimal("0.5"));
	assert(Decimal("1.5") > Decimal("1"));
	assert(Decimal("-1") > Decimal("-5"));
	assert(Decimal("-1.5") > Decimal("-1.6"));

	assert(Decimal("0") < Decimal("1"));
	assert(Decimal("-1") < Decimal("0"));
	assert(Decimal("0.5") < Decimal("1"));
	assert(Decimal("1") < Decimal("1.5"));
	assert(Decimal("-5") < Decimal("-1"));
	assert(Decimal("-1.6") < Decimal("-1.5"));

	assert(Decimal("0") == -Decimal("0"));
	assert(Decimal("1") == -Decimal("-1"));
	assert(Decimal("-1") == -Decimal("1"));
	assert(Decimal("1.1") == -Decimal("-1.1"));
	assert(Decimal("-1.1") == -Decimal("1.1"));

	assert((Decimal("0") + Decimal("0")).asString() == std::string("0"));
	assert((Decimal("1") + Decimal("0")).asString() == std::string("1"));
	assert((Decimal("0") + Decimal("1")).asString() == std::string("1"));
	assert((Decimal("1") + Decimal("1")).asString() == std::string("2"));
	assert((Decimal("1.1") + Decimal("0")).asString() == std::string("1.1"));
	assert((Decimal("0") + Decimal("1.1")).asString() == std::string("1.1"));
	assert((Decimal("1.1") + Decimal("1.1")).asString() == std::string("2.2"));
	assert((Decimal("-1") + Decimal("0")).asString() == std::string("-1"));
	assert((Decimal("0") + Decimal("-1")).asString() == std::string("-1"));
	assert((Decimal("-1") + Decimal("-1")).asString() == std::string("-2"));
	assert((Decimal("-1.1") + Decimal("0")).asString() == std::string("-1.1"));
	assert((Decimal("0") + Decimal("-1.1")).asString() == std::string("-1.1"));
	assert((Decimal("-1.1") + Decimal("-1.1")).asString() == std::string("-2.2"));
	assert((Decimal("1.9") + Decimal("1.9")).asString() == std::string("3.8"));
	assert((Decimal("1.9") + Decimal("-1.9")).asString() == std::string("0"));
	assert((Decimal("-1.9") + Decimal("1.9")).asString() == std::string("0"));
	assert((Decimal("-1.9") + Decimal("-1.9")).asString() == std::string("-3.8"));
	assert((Decimal("1.9") + Decimal("-1.5")).asString() == std::string("0.4"));
	assert((Decimal("1.5") + Decimal("-1.9")).asString() == std::string("-0.4"));
	assert((Decimal("-1.9") + Decimal("1.5")).asString() == std::string("-0.4"));
	assert((Decimal("-1.5") + Decimal("1.9")).asString() == std::string("0.4"));

	assert((Decimal("0") - Decimal("0")).asString() == std::string("0"));
	assert((Decimal("1") - Decimal("0")).asString() == std::string("1"));
	assert((Decimal("0") - Decimal("1")).asString() == std::string("-1"));
	assert((Decimal("1") - Decimal("1")).asString() == std::string("0"));
	assert((Decimal("1.1") - Decimal("0")).asString() == std::string("1.1"));
	assert((Decimal("0") - Decimal("1.1")).asString() == std::string("-1.1"));
	assert((Decimal("1.1") - Decimal("1.1")).asString() == std::string("0"));
	assert((Decimal("-1") - Decimal("0")).asString() == std::string("-1"));
	assert((Decimal("0") - Decimal("-1")).asString() == std::string("1"));
	assert((Decimal("-1") - Decimal("-1")).asString() == std::string("0"));
	assert((Decimal("-1.1") - Decimal("0")).asString() == std::string("-1.1"));
	assert((Decimal("0") - Decimal("-1.1")).asString() == std::string("1.1"));
	assert((Decimal("-1.1") - Decimal("-1.1")).asString() == std::string("0"));
	assert((Decimal("1.9") - Decimal("1.9")).asString() == std::string("0"));
	assert((Decimal("1.9") - Decimal("-1.9")).asString() == std::string("3.8"));
	assert((Decimal("-1.9") - Decimal("1.9")).asString() == std::string("-3.8"));
	assert((Decimal("-1.9") - Decimal("-1.9")).asString() == std::string("0"));
	assert((Decimal("1.9") - Decimal("-1.5")).asString() == std::string("3.4"));
	assert((Decimal("1.5") - Decimal("-1.9")).asString() == std::string("3.4"));
	assert((Decimal("-1.9") - Decimal("1.5")).asString() == std::string("-3.4"));
	assert((Decimal("-1.5") - Decimal("1.9")).asString() == std::string("-3.4"));

	assert((Decimal("0") * Decimal("0")).asString() == std::string("0"));
	assert((Decimal("1") * Decimal("0")).asString() == std::string("0"));
	assert((Decimal("0") * Decimal("1")).asString() == std::string("0"));
	assert((Decimal("1") * Decimal("1")).asString() == std::string("1"));
	DASSERT((Decimal("1.1") * Decimal("0")).asString() == std::string("0"), (Decimal("1.1") * Decimal("0")).asString());
	assert((Decimal("0") * Decimal("1.1")).asString() == std::string("0"));
	DASSERT((Decimal("1.1") * Decimal("1.1")).asString() == std::string("1.21"), (Decimal("1.1") * Decimal("1.1")).asString());
	assert((Decimal("-1") * Decimal("0")).asString() == std::string("0"));
	assert((Decimal("0") * Decimal("-1")).asString() == std::string("0"));
	assert((Decimal("-1") * Decimal("-1")).asString() == std::string("1"));
	assert((Decimal("-1.1") * Decimal("0")).asString() == std::string("0"));
	assert((Decimal("0") * Decimal("-1.1")).asString() == std::string("0"));
	assert((Decimal("-1.1") * Decimal("-1.1")).asString() == std::string("1.21"));
	DASSERT((Decimal("1.9") * Decimal("1.9")).asString() == std::string("3.61"), (Decimal("1.9") * Decimal("1.9")).asString());
	assert((Decimal("1.9") * Decimal("-1.9")).asString() == std::string("-3.61"));
	assert((Decimal("-1.9") * Decimal("1.9")).asString() == std::string("-3.61"));
	assert((Decimal("-1.9") * Decimal("-1.9")).asString() == std::string("3.61"));
	assert((Decimal("1.9") * Decimal("-1.5")).asString() == std::string("-2.85"));
	assert((Decimal("1.5") * Decimal("-1.9")).asString() == std::string("-2.85"));
	assert((Decimal("-1.9") * Decimal("1.5")).asString() == std::string("-2.85"));
	assert((Decimal("-1.5") * Decimal("1.9")).asString() == std::string("-2.85"));
	assert((Decimal("1.9999999999999999999") * Decimal("1.9999999999999999999")).asString() == std::string("3.9999999999999999996"));
	assert((Decimal("1.9999999999999999999") * Decimal("-1.9999999999999999999")).asString() == std::string("-3.9999999999999999996"));
	assert((Decimal("-1.9999999999999999999") * Decimal("1.9999999999999999999")).asString() == std::string("-3.9999999999999999996"));
	assert((Decimal("-1.9999999999999999999") * Decimal("-1.9999999999999999999")).asString() == std::string("3.9999999999999999996"));

	assert((Decimal("0") / Decimal("1")).asString() == std::string("0"));
	assert((Decimal("1") / Decimal("1")).asString() == std::string("1"));
	assert((Decimal("3") / Decimal("1")).asString() == std::string("3"));
	assert((Decimal("1") / Decimal("3")).asString() == std::string("0.3333333333333333333"));
	assert((Decimal("1") / Decimal("-3")).asString() == std::string("-0.3333333333333333333"));
	assert((Decimal("4") / Decimal("3")).asString() == std::string("1.3333333333333333333"));
	assert((Decimal("-4") / Decimal("3")).asString() == std::string("-1.3333333333333333333"));
	assert((Decimal("1.3") / Decimal("1")).asString() == std::string("1.3"));
	assert((Decimal("-1.3") / Decimal("-1")).asString() == std::string("1.3"));
	assert((Decimal("1.3") / Decimal("1.3")).asString() == std::string("1"));
	assert((Decimal("-1.3") / Decimal("1.3")).asString() == std::string("-1"));
	assert((Decimal("1") / Decimal("0.5")).asString() == std::string("2"));
	assert((Decimal("1") / Decimal("-0.5")).asString() == std::string("-2"));
	DASSERT((Decimal("5.55555555") / Decimal("6")).asString() == std::string("0.925925925"), (Decimal("5.55555555") / Decimal("6")).asString());
	DASSERT((Decimal("6") / Decimal("5.55555555")).asString() == std::string("1.080000001080000001"), (Decimal("6") / Decimal("5.55555555")).asString());
	DASSERT((Decimal("7.5555") / Decimal("3.3333")).asString() == std::string("2.2666726667266672666"), (Decimal("7.5555") / Decimal("3.3333")).asString());

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
