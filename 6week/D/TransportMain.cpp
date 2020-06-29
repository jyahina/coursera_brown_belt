#pragma once

#include <cstdio>
#include <iostream>
#include "Transport.h"
#include "test_runner.h"
#include "TransportJsonParser.h"
#include "TransportJson.h"

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

void TestRequestsJson()
{
	std::stringstream out;
	TransportJson transport(out);

	stringstream in(R"({
  "base_requests": [
    {
      "type": "Stop",
      "road_distances": {
        "Marushkino": 3900
      },
      "longitude": 37.20829,
      "name": "Tolstopaltsevo",
      "latitude": 55.611087
    },
    {
      "type": "Stop",
      "road_distances": {
        "Rasskazovka": 9900
      },
      "longitude": 37.209755,
      "name": "Marushkino",
      "latitude": 55.595884
    },
    {
      "type": "Bus",
      "name": "256",
      "stops": [
        "Biryulyovo Zapadnoye",
        "Biryusinka",
        "Universam",
        "Biryulyovo Tovarnaya",
        "Biryulyovo Passazhirskaya",
        "Biryulyovo Zapadnoye"
      ],
      "is_roundtrip": true
    },
    {
      "type": "Bus",
      "name": "750",
      "stops": [
        "Tolstopaltsevo",
        "Marushkino",
        "Rasskazovka"
      ],
      "is_roundtrip": false
    },
    {
      "type": "Stop",
      "road_distances": {},
      "longitude": 37.333324,
      "name": "Rasskazovka",
      "latitude": 55.632761
    },
    {
      "type": "Stop",
      "road_distances": {
        "Rossoshanskaya ulitsa": 7500,
        "Biryusinka": 1800,
        "Universam": 2400
      },
      "longitude": 37.6517,
      "name": "Biryulyovo Zapadnoye",
      "latitude": 55.574371
    },
    {
      "type": "Stop",
      "road_distances": {
        "Universam": 750
      },
      "longitude": 37.64839,
      "name": "Biryusinka",
      "latitude": 55.581065
    },
    {
      "type": "Stop",
      "road_distances": {
        "Rossoshanskaya ulitsa": 5600,
        "Biryulyovo Tovarnaya": 900
      },
      "longitude": 37.645687,
      "name": "Universam",
      "latitude": 55.587655
    },
    {
      "type": "Stop",
      "road_distances": {
        "Biryulyovo Passazhirskaya": 1300
      },
      "longitude": 37.653656,
      "name": "Biryulyovo Tovarnaya",
      "latitude": 55.592028
    },
    {
      "type": "Stop",
      "road_distances": {
        "Biryulyovo Zapadnoye": 1200
      },
      "longitude": 37.659164,
      "name": "Biryulyovo Passazhirskaya",
      "latitude": 55.580999
    },
    {
      "type": "Bus",
      "name": "828",
      "stops": [
        "Biryulyovo Zapadnoye",
        "Universam",
        "Rossoshanskaya ulitsa",
        "Biryulyovo Zapadnoye"
      ],
      "is_roundtrip": true
    },
    {
      "type": "Stop",
      "road_distances": {},
      "longitude": 37.605757,
      "name": "Rossoshanskaya ulitsa",
      "latitude": 55.595579
    },
    {
      "type": "Stop",
      "road_distances": {},
      "longitude": 37.603831,
      "name": "Prazhskaya",
      "latitude": 55.611678
    }
  ],
  "stat_requests": [
    {
      "type": "Bus",
      "name": "256",
      "id": 1965312327
    },
    {
      "type": "Bus",
      "name": "750",
      "id": 519139350
    },
    {
      "type": "Bus",
      "name": "751",
      "id": 194217464
    },
    {
      "type": "Stop",
      "name": "Samara",
      "id": 746888088
    },
    {
      "type": "Stop",
      "name": "Prazhskaya",
      "id": 65100610
    },
    {
      "type": "Stop",
      "name": "Biryulyovo Zapadnoye",
      "id": 1042838872
    }
  ]
})");

	std::string expected(R"([
  {
    "route_length": 5950,
    "request_id": 1965312327,
    "curvature": 1.36124,
    "stop_count": 6,
    "unique_stop_count": 5
  },
  {
    "route_length": 27600,
    "request_id": 519139350,
    "curvature": 1.31808,
    "stop_count": 5,
    "unique_stop_count": 3
  },
  {
    "request_id": 194217464,
    "error_message": "not found"
  },
  {
    "request_id": 746888088,
    "error_message": "not found"
  },
  {
    "buses": [],
    "request_id": 65100610
  },
  {
    "buses": [
      "256",
      "828"
    ],
    "request_id": 1042838872
  }
]
)");

	transport.Set(in);

	ASSERT_EQUAL(expected, out.str());
}

void TestRequests()
{
	std::stringstream out;
	TransportJson transport(out);
	std::string expected;

	transport.ExecuteRequest(R"({"type": "Stop", "road_distances": { "Marushkino": 3900  }, "longitude": 37.20829, "name": "Tolstopaltsevo", "latitude": 55.611087 })");
	transport.ExecuteRequest(R"({"type": "Stop","road_distances": {  "Rasskazovka": 9900},"longitude": 37.209755,"name": "Marushkino","latitude": 55.595884})");
	transport.ExecuteRequest(R"({"type": "Bus","name": "256","stops": [  "Biryulyovo Zapadnoye",  "Biryusinka",  "Universam",  "Biryulyovo Tovarnaya",  "Biryulyovo Passazhirskaya",  "Biryulyovo Zapadnoye"],"is_roundtrip": true })");
	transport.ExecuteRequest(R"({"type": "Bus", "name": "750", "stops": [   "Tolstopaltsevo",   "Marushkino",   "Rasskazovka" ], "is_roundtrip": false})");
	transport.ExecuteRequest(R"({"type": "Stop", "road_distances": {}, "longitude": 37.333324, "name": "Rasskazovka", "latitude": 55.632761})");
	transport.ExecuteRequest(R"({"type": "Stop", "road_distances": {"Rossoshanskaya ulitsa": 7500, "Biryusinka": 1800, "Universam": 2400},"longitude": 37.6517,"name": "Biryulyovo Zapadnoye","latitude": 55.574371 })");
	transport.ExecuteRequest(R"({"type": "Stop", "road_distances": {   "Universam": 750 }, "longitude": 37.64839, "name": "Biryusinka", "latitude": 55.581065 })");
	transport.ExecuteRequest(R"({"type": "Stop","road_distances" : {"Rossoshanskaya ulitsa": 5600,"Biryulyovo Tovarnaya" : 900},"longitude" : 37.645687,"name" : "Universam","latitude" : 55.587655	})");
	transport.ExecuteRequest(R"({"type": "Stop","road_distances" : {"Biryulyovo Passazhirskaya": 1300},"longitude" : 37.653656,"name" : "Biryulyovo Tovarnaya","latitude" : 55.592028	 })");
    transport.ExecuteRequest(R"({"type": "Stop","road_distances" : {"Biryulyovo Zapadnoye": 1200},"longitude" : 37.659164,"name" : "Biryulyovo Passazhirskaya","latitude" : 55.580999	})");
	transport.ExecuteRequest(R"({"type": "Bus","name" : "828","stops" : ["Biryulyovo Zapadnoye","Universam","Rossoshanskaya ulitsa","Biryulyovo Zapadnoye"] ,	  "is_roundtrip": true	  })");
	transport.ExecuteRequest(R"({"type": "Stop","road_distances" : {},"longitude" : 37.605757,"name" : "Rossoshanskaya ulitsa","latitude" : 55.595579})");
    transport.ExecuteRequest(R"({"type": "Stop","road_distances" : {},"longitude" : 37.603831,"name" : "Prazhskaya","latitude" : 55.611678	})");

	transport.WriteResponseByRequest(R"({"type": "Bus","name" : "256","id" : 1965312327})");
	expected = (R"({ "request_id": 1965312327, "route_length": 5950, "curvature": 1.36124, "stop_count": 6, "unique_stop_count": 5 })");
	ASSERT_EQUAL(expected, out.str());
    out.str("");

	transport.WriteResponseByRequest(R"({"type": "Bus", "name" : "750", "id" : 519139350})");
	expected = (R"({ "request_id": 519139350, "route_length": 27600, "curvature": 1.31808, "stop_count": 5, "unique_stop_count": 3 })");
	ASSERT_EQUAL(expected, out.str());
    out.str("");

	transport.WriteResponseByRequest(R"({ "type": "Bus", "name" : "751", "id" : 194217464})");
	expected = (R"({ "request_id": 194217464, "error_message": "not found" })");
	ASSERT_EQUAL(expected, out.str());
    out.str("");

	transport.WriteResponseByRequest(R"({ "type": "Stop", "name" : "Samara", "id" : 746888088})");
	expected = (R"({ "request_id": 746888088, "error_message": "not found" })");
	ASSERT_EQUAL(expected, out.str());
    out.str("");

	transport.WriteResponseByRequest(R"({ "type": "Stop", "name" : "Prazhskaya", "id" : 65100610})");
	expected = (R"({ "request_id": 65100610, "buses": [] })");
	ASSERT_EQUAL(expected, out.str());
    out.str("");

	transport.WriteResponseByRequest(R"({ "type": "Stop", "name" : "Biryulyovo Zapadnoye", "id" : 1042838872})");
	expected = (R"({ "request_id": 1042838872, "buses": ["256", "828"] })");
	ASSERT_EQUAL(expected, out.str());
    out.str("");
}

void Tests()
{
	TestRunner tr;
	RUN_TEST(tr, TestBus);
	RUN_TEST(tr, TestsMainProcessC);
	RUN_TEST(tr, TestRequests);
	RUN_TEST(tr, TestRequestsJson);
}

int main()
{
	//Tests();

	//auto transport = Transport();
	//transport.MainProcess();

    auto transport = TransportJson();
    transport.Set(cin);
	return 0;
};