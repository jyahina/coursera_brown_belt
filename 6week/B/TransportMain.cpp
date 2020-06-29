#pragma once

#include <cstdio>
#include <iostream>
#include "Transport.h"
#include "test_runner.h"
using namespace std;

void TestBus()
{
	Transport transport(cout);
	transport.ExecuteRequest("Stop Tolstopaltsevo: 55.611087, 37.20829");
	transport.ExecuteRequest("Stop Marushkino: 55.595884, 37.209755");
	transport.ExecuteRequest("Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye");
	transport.ExecuteRequest("Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka");
	transport.ExecuteRequest("Stop Rasskazovka: 55.632761, 37.333324");
	transport.ExecuteRequest("Stop Biryulyovo Zapadnoye: 55.574371, 37.6517");
	transport.ExecuteRequest("Stop Biryusinka: 55.581065, 37.64839");
	transport.ExecuteRequest("Stop Universam: 55.587655, 37.645687");
	transport.ExecuteRequest("Stop Biryulyovo Tovarnaya: 55.592028, 37.653656");
	transport.ExecuteRequest("Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164");
	transport.ExecuteRequest("Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye");
	transport.ExecuteRequest("Stop Rossoshanskaya ulitsa : 55.595579, 37.605757");
	transport.ExecuteRequest("Stop Prazhskaya : 55.611678, 37.603831");

	string answer;
	answer = transport.GetInfoBus("256");
	ASSERT_EQUAL(answer, "Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length");

	answer = transport.GetInfoBus("750");
	ASSERT_EQUAL(answer, "Bus 750: 5 stops on route, 3 unique stops, 20939.5 route length");

	answer = transport.GetInfoBus("751");
	ASSERT_EQUAL(answer, "Bus 751: not found");

	answer = transport.GetInfoBusStops("Samara");
	ASSERT_EQUAL(answer, "Stop Samara: not found");

	answer = transport.GetInfoBusStops("Prazhskaya");
	ASSERT_EQUAL(answer, "Stop Prazhskaya: no buses");

	answer = transport.GetInfoBusStops("Biryulyovo Zapadnoye");
	ASSERT_EQUAL(answer, "Stop Biryulyovo Zapadnoye: buses 256 828");
}

void TestsMainProcessA()
{
	std::stringstream out;
	Transport transport(out);

	std::stringstream in(R"(10
Stop Tolstopaltsevo: 55.611087, 37.20829
Stop Marushkino: 55.595884, 37.209755
Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka
Stop Rasskazovka: 55.632761, 37.333324
Stop Biryulyovo Zapadnoye: 55.574371, 37.6517
Stop Biryusinka: 55.581065, 37.64839
Stop Universam: 55.587655, 37.645687
Stop Biryulyovo Tovarnaya: 55.592028, 37.653656
Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164
3
Bus 256
Bus 750
Bus 751
)");

	std::string expected(R"(Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length
Bus 750: 5 stops on route, 3 unique stops, 20939.5 route length
Bus 751: not found
)");

	transport.MainProcess(in);

	ASSERT_EQUAL(expected, out.str());
};

void TestsMainProcessB()
{
	std::stringstream out;
	Transport transport(out);

	std::stringstream in(R"(13
Stop Tolstopaltsevo: 55.611087, 37.20829
Stop Marushkino: 55.595884, 37.209755
Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka
Stop Rasskazovka: 55.632761, 37.333324
Stop Biryulyovo Zapadnoye: 55.574371, 37.6517
Stop Biryusinka: 55.581065, 37.64839
Stop Universam: 55.587655, 37.645687
Stop Biryulyovo Tovarnaya: 55.592028, 37.653656
Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164
Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye
Stop Rossoshanskaya ulitsa: 55.595579, 37.605757
Stop Prazhskaya: 55.611678, 37.603831
6
Bus 256
Bus 750
Bus 751
Stop Samara
Stop Prazhskaya
Stop Biryulyovo Zapadnoye
)");

	std::string expected(R"(Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length
Bus 750: 5 stops on route, 3 unique stops, 20939.5 route length
Bus 751: not found
Stop Samara: not found
Stop Prazhskaya: no buses
Stop Biryulyovo Zapadnoye: buses 256 828
)");

	transport.MainProcess(in);

	ASSERT_EQUAL(expected, out.str());
};

void TestsMainProcessB1()
{
	std::stringstream out;
	Transport transport(out);

	std::stringstream in(R"(13
Stop Tolstopaltsevo: 55.611087, 37.20829
Stop Marushkino: 55.595884, 37.209755
Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka
Stop Rasskazovka: 55.632761, 37.333324
Stop Biryulyovo Zapadnoye: 55.574371, 37.6517
Stop Biryusinka: 55.581065, 37.64839
Stop Universam: 55.587655, 37.645687
Stop Biryulyovo Tovarnaya: 55.592028, 37.653656
Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164
Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye
Stop Rossoshanskaya ulitsa: 55.595579, 37.605757
Stop Prazhskaya: 55.611678, 37.603831
6
Bus 256
Bus 750
Bus 751
Stop Samara
Stop Prazhskaya
Stop Biryulyovo Zapadnoye
)");

	std::string expected(R"(Bus 256: 6 stops on route, 5 unique stops, 4371.017249 route length
Bus 750: 5 stops on route, 3 unique stops, 20939.483047 route length
Bus 751: not found
Stop Samara: not found
Stop Prazhskaya: no buses
Stop Biryulyovo Zapadnoye: buses 256 828
)");

	transport.MainProcess(in);

	ASSERT_EQUAL(expected, out.str());
};

void Tests()
{
	TestRunner tr;
	RUN_TEST(tr, TestBus);
	RUN_TEST(tr, TestsMainProcessA);
	RUN_TEST(tr, TestsMainProcessB);
	RUN_TEST(tr, TestsMainProcessB1);
}

int main()
{
	//Tests();

	auto transport = Transport();
	transport.MainProcess();
	return 0;
};