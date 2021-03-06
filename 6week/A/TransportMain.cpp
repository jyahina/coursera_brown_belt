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

	string answer;
	answer = transport.GetInfoBusStops("256");
	ASSERT_EQUAL(answer, "Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length");

	answer = transport.GetInfoBusStops("750");
	ASSERT_EQUAL(answer, "Bus 750: 5 stops on route, 3 unique stops, 20939.5 route length");

	answer = transport.GetInfoBusStops("751");
	ASSERT_EQUAL(answer, "Bus 751: not found");
}

void TestsMainProcess()
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

void Tests()
{
	TestRunner tr;
	RUN_TEST(tr, TestBus);
	RUN_TEST(tr, TestsMainProcess);

}

int main()
{
	Tests();

	auto transport = Transport();
	transport.MainProcess();
	return 0;
};