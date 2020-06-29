#pragma once

#include <cstdio>
#include <iostream>
#include "Transport.h"
#include "test_runner.h"
using namespace std;

void TestBus()
{
	Transport transport(cout);
	transport.ExecuteRequest("Stop Tolstopaltsevo: 55.611087, 37.20829, 3900m to Marushkino");
	transport.ExecuteRequest("Stop Marushkino : 55.595884, 37.209755, 9900m to Rasskazovka");
	transport.ExecuteRequest("Bus 256 : Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye");
	transport.ExecuteRequest("Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka");
	transport.ExecuteRequest("Stop Rasskazovka : 55.632761, 37.333324");
	transport.ExecuteRequest("Stop Biryulyovo Zapadnoye : 55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m to Biryusinka, 2400m to Universam");
	transport.ExecuteRequest("Stop Biryusinka : 55.581065, 37.64839, 750m to Universam");
	transport.ExecuteRequest("Stop Universam : 55.587655, 37.645687, 5600m to Rossoshanskaya ulitsa, 900m to Biryulyovo Tovarnaya");
	transport.ExecuteRequest("Stop Biryulyovo Tovarnaya : 55.592028, 37.653656, 1300m to Biryulyovo Passazhirskaya");
	transport.ExecuteRequest("Stop Biryulyovo Passazhirskaya : 55.580999, 37.659164, 1200m to Biryulyovo Zapadnoye");
	transport.ExecuteRequest("Bus 828 : Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye");
	transport.ExecuteRequest("Stop Rossoshanskaya ulitsa : 55.595579, 37.605757");
	transport.ExecuteRequest("Stop Prazhskaya : 55.611678, 37.603831");

	string answer;
	answer = transport.GetInfoBus("256");
	ASSERT_EQUAL(answer, "Bus 256: 6 stops on route, 5 unique stops, 5950 route length, 1.361239 curvature");

	answer = transport.GetInfoBus("750");
	ASSERT_EQUAL(answer, "Bus 750: 5 stops on route, 3 unique stops, 27600 route length, 1.318084 curvature");

	answer = transport.GetInfoBus("751");
	ASSERT_EQUAL(answer, "Bus 751: not found");

	answer = transport.GetInfoBusStops("Samara");
	ASSERT_EQUAL(answer, "Stop Samara: not found");

	answer = transport.GetInfoBusStops("Prazhskaya");
	ASSERT_EQUAL(answer, "Stop Prazhskaya: no buses");

	answer = transport.GetInfoBusStops("Biryulyovo Zapadnoye");
	ASSERT_EQUAL(answer, "Stop Biryulyovo Zapadnoye: buses 256 828");
}

void TestsMainProcessC()
{
	std::stringstream out;
	Transport transport(out);

	std::stringstream in(R"(13
Stop Tolstopaltsevo: 55.611087, 37.20829, 3900m to Marushkino
Stop Marushkino: 55.595884, 37.209755, 9900m to Rasskazovka
Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka
Stop Rasskazovka: 55.632761, 37.333324
Stop Biryulyovo Zapadnoye: 55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m to Biryusinka, 2400m to Universam
Stop Biryusinka: 55.581065, 37.64839, 750m to Universam
Stop Universam: 55.587655, 37.645687, 5600m to Rossoshanskaya ulitsa, 900m to Biryulyovo Tovarnaya
Stop Biryulyovo Tovarnaya: 55.592028, 37.653656, 1300m to Biryulyovo Passazhirskaya
Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164, 1200m to Biryulyovo Zapadnoye
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

	std::string expected(R"(Bus 256: 6 stops on route, 5 unique stops, 5950 route length, 1.361239 curvature
Bus 750: 5 stops on route, 3 unique stops, 27600 route length, 1.318084 curvature
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
	RUN_TEST(tr, TestsMainProcessC);
}

int main()
{
	//Tests();

	auto transport = Transport();
	transport.MainProcess();
	return 0;
};