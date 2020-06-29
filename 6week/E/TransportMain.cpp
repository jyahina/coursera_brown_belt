#pragma once

#include <cstdio>
#include <iostream>
#include "test_runner.h"
#include "TransportJson.h"

using namespace std;

void TestRequestsJson()
{
	std::stringstream out;
	TransportJson transport(out);

	stringstream in(R"(
{"routing_settings": {"bus_wait_time": 6, "bus_velocity": 40}, "base_requests": [{"type": "Bus", "name": "297", "stops": ["Biryulyovo Zapadnoye", "Biryulyovo Tovarnaya", "Universam", "Biryulyovo Zapadnoye"], "is_roundtrip": true}, {"type": "Bus", "name": "635", "stops": ["Biryulyovo Tovarnaya", "Universam", "Prazhskaya"], "is_roundtrip": false}, {"type": "Stop", "name": "Biryulyovo Zapadnoye", "latitude": 55.574371, "longitude": 37.6517, "road_distances": {"Biryulyovo Tovarnaya": 2600}}, {"type": "Stop", "name": "Universam", "latitude": 55.587655, "longitude": 37.645687, "road_distances": {"Biryulyovo Tovarnaya": 1380, "Biryulyovo Zapadnoye": 2500, "Prazhskaya": 4650}}, {"type": "Stop", "name": "Biryulyovo Tovarnaya", "latitude": 55.592028, "longitude": 37.653656, "road_distances": {"Universam": 890}}, {"type": "Stop", "name": "Prazhskaya", "latitude": 55.611717, "longitude": 37.603938, "road_distances": {}}], "stat_requests": [{"id": 1, "type": "Bus", "name": "297"}, {"id": 2, "type": "Bus", "name": "635"}, {"id": 3, "type": "Stop", "name": "Universam"}, {"id": 4, "type": "Route", "from": "Biryulyovo Zapadnoye", "to": "Universam"}, {"id": 5, "type": "Route", "from": "Biryulyovo Zapadnoye", "to": "Prazhskaya"}]}

)");
//    stringstream in(R"({
//  "routing_settings": {
//    "bus_wait_time": 6,
//    "bus_velocity": 40
//  },
//  "base_requests": [
//    {
//      "type": "Bus",
//      "name": "297",
//      "stops": [
//        "Biryulyovo Zapadnoye",
//        "Biryulyovo Tovarnaya",
//        "Universam",
//        "Biryulyovo Zapadnoye"
//      ],
//      "is_roundtrip": true
//    },
//    {
//      "type": "Bus",
//      "name": "635",
//      "stops": [
//        "Biryulyovo Tovarnaya",
//        "Universam",
//        "Prazhskaya"
//      ],
//      "is_roundtrip": false
//    },
//    {
//      "type": "Stop",
//      "road_distances": {
//        "Biryulyovo Tovarnaya": 2600
//      },
//      "longitude": 37.6517,
//      "name": "Biryulyovo Zapadnoye",
//      "latitude": 55.574371
//    },
//    {
//      "type": "Stop",
//      "road_distances": {
//        "Prazhskaya": 4650,
//        "Biryulyovo Tovarnaya": 1380,
//        "Biryulyovo Zapadnoye": 2500
//      },
//      "longitude": 37.645687,
//      "name": "Universam",
//      "latitude": 55.587655
//    },
//    {
//      "type": "Stop",
//      "road_distances": {
//        "Universam": 890
//      },
//      "longitude": 37.653656,
//      "name": "Biryulyovo Tovarnaya",
//      "latitude": 55.592028
//    },
//    {
//      "type": "Stop",
//      "road_distances": {},
//      "longitude": 37.603938,
//      "name": "Prazhskaya",
//      "latitude": 55.611717
//    }
//  ],
//  "stat_requests": [
//    {
//      "type": "Bus",
//      "name": "297",
//      "id": 1
//    },
//    {
//      "type": "Bus",
//      "name": "635",
//      "id": 2
//    },
//    {
//      "type": "Stop",
//      "name": "Universam",
//      "id": 3
//    },
//    {
//      "type": "Route",
//      "from": "Biryulyovo Zapadnoye",
//      "to": "Universam",
//      "id": 4
//    },
//    {
//      "type": "Route",
//      "from": "Biryulyovo Zapadnoye",
//      "to": "Prazhskaya",
//      "id": 5
//    }
//  ]
//})");


	std::string expected(R"([{ "request_id": 1, "route_length": 5990, "curvature": 1.42963, "stop_count": 4, "unique_stop_count": 3 }, { "request_id": 2, "route_length": 11570, "curvature": 1.30156, "stop_count": 5, "unique_stop_count": 3 }, { "request_id": 3, "buses": ["297", "635"] }, { "request_id": 4, "items": [{"time": 6, "type": "Wait", "stop_name": "Biryulyovo Zapadnoye"},{"span_count": 2, "bus": "297", "type": "Bus", "time": 5.235}], "total_time": 11.235 }, { "request_id": 5, "items": [{"time": 6, "type": "Wait", "stop_name": "Biryulyovo Zapadnoye"},{"span_count": 1, "bus": "297", "type": "Bus", "time": 3.9}, {"time": 6, "type": "Wait", "stop_name": "Biryulyovo Tovarnaya"},{"span_count": 2, "bus": "635", "type": "Bus", "time": 8.31}], "total_time": 24.21 }])");

	transport.Set(in);

	ASSERT_EQUAL(expected, out.str());
}

void TestRequests()
{
	std::stringstream out;
	TransportJson transport(out);
	std::string expected;

	transport.ExecuteStringRequest(R"({"type": "Stop", "road_distances": { "Marushkino": 3900  }, "longitude": 37.20829, "name": "Tolstopaltsevo", "latitude": 55.611087 })");
	transport.ExecuteStringRequest(R"({"type": "Stop","road_distances": {  "Rasskazovka": 9900},"longitude": 37.209755,"name": "Marushkino","latitude": 55.595884})");
	transport.ExecuteStringRequest(R"({"type": "Bus","name": "256","stops": [  "Biryulyovo Zapadnoye",  "Biryusinka",  "Universam",  "Biryulyovo Tovarnaya",  "Biryulyovo Passazhirskaya",  "Biryulyovo Zapadnoye"],"is_roundtrip": true })");
	transport.ExecuteStringRequest(R"({"type": "Bus", "name": "750", "stops": [   "Tolstopaltsevo",   "Marushkino",   "Rasskazovka" ], "is_roundtrip": false})");
	transport.ExecuteStringRequest(R"({"type": "Stop", "road_distances": {}, "longitude": 37.333324, "name": "Rasskazovka", "latitude": 55.632761})");
	transport.ExecuteStringRequest(R"({"type": "Stop", "road_distances": {"Rossoshanskaya ulitsa": 7500, "Biryusinka": 1800, "Universam": 2400},"longitude": 37.6517,"name": "Biryulyovo Zapadnoye","latitude": 55.574371 })");
	transport.ExecuteStringRequest(R"({"type": "Stop", "road_distances": {   "Universam": 750 }, "longitude": 37.64839, "name": "Biryusinka", "latitude": 55.581065 })");
	transport.ExecuteStringRequest(R"({"type": "Stop","road_distances" : {"Rossoshanskaya ulitsa": 5600,"Biryulyovo Tovarnaya" : 900},"longitude" : 37.645687,"name" : "Universam","latitude" : 55.587655	})");
	transport.ExecuteStringRequest(R"({"type": "Stop","road_distances" : {"Biryulyovo Passazhirskaya": 1300},"longitude" : 37.653656,"name" : "Biryulyovo Tovarnaya","latitude" : 55.592028	 })");
    transport.ExecuteStringRequest(R"({"type": "Stop","road_distances" : {"Biryulyovo Zapadnoye": 1200},"longitude" : 37.659164,"name" : "Biryulyovo Passazhirskaya","latitude" : 55.580999	})");
	transport.ExecuteStringRequest(R"({"type": "Bus","name" : "828","stops" : ["Biryulyovo Zapadnoye","Universam","Rossoshanskaya ulitsa","Biryulyovo Zapadnoye"] ,	  "is_roundtrip": true	  })");
	transport.ExecuteStringRequest(R"({"type": "Stop","road_distances" : {},"longitude" : 37.605757,"name" : "Rossoshanskaya ulitsa","latitude" : 55.595579})");
    transport.ExecuteStringRequest(R"({"type": "Stop","road_distances" : {},"longitude" : 37.603831,"name" : "Prazhskaya","latitude" : 55.611678	})");

	transport.WriteResponseByStringRequest(R"({"type": "Bus","name" : "256","id" : 1965312327})");
	expected = (R"({ "request_id": 1965312327, "route_length": 5950, "curvature": 1.36124, "stop_count": 6, "unique_stop_count": 5 })");
	ASSERT_EQUAL(expected, out.str());
    out.str("");

	transport.WriteResponseByStringRequest(R"({"type": "Bus", "name" : "750", "id" : 519139350})");
	expected = (R"({ "request_id": 519139350, "route_length": 27600, "curvature": 1.31808, "stop_count": 5, "unique_stop_count": 3 })");
	ASSERT_EQUAL(expected, out.str());
    out.str("");

	transport.WriteResponseByStringRequest(R"({ "type": "Bus", "name" : "751", "id" : 194217464})");
	expected = (R"({ "request_id": 194217464, "error_message": "not found" })");
	ASSERT_EQUAL(expected, out.str());
    out.str("");

	transport.WriteResponseByStringRequest(R"({ "type": "Stop", "name" : "Samara", "id" : 746888088})");
	expected = (R"({ "request_id": 746888088, "error_message": "not found" })");
	ASSERT_EQUAL(expected, out.str());
    out.str("");

	transport.WriteResponseByStringRequest(R"({ "type": "Stop", "name" : "Prazhskaya", "id" : 65100610})");
	expected = (R"({ "request_id": 65100610, "buses": [] })");
	ASSERT_EQUAL(expected, out.str());
    out.str("");

	transport.WriteResponseByStringRequest(R"({ "type": "Stop", "name" : "Biryulyovo Zapadnoye", "id" : 1042838872})");
	expected = (R"({ "request_id": 1042838872, "buses": ["256", "828"] })");
	ASSERT_EQUAL(expected, out.str());
    out.str("");
}

void TestRequestsE()
{
    std::stringstream out;
    TransportJson transport(out);
    std::string expected;

    transport.SetRouteStringDetails(R"({"routing_settings": { "bus_wait_time": 6, "bus_velocity": 40 } })");
    transport.ExecuteStringRequest(R"({ "type": "Bus","name" : "297","stops" : ["Biryulyovo Zapadnoye", "Biryulyovo Tovarnaya", "Universam", "Biryulyovo Zapadnoye"] , "is_roundtrip": true })");
    transport.ExecuteStringRequest(R"({ "type": "Bus", "name" : "635", "stops" : [   "Biryulyovo Tovarnaya",   "Universam",   "Prazhskaya"] ,"is_roundtrip" : false})");
    transport.ExecuteStringRequest(R"({"type": "Stop","road_distances" : {  "Biryulyovo Tovarnaya": 2600},"longitude" : 37.6517,"name" : "Biryulyovo Zapadnoye","latitude" : 55.574371   })");
    transport.ExecuteStringRequest(R"({"type": "Stop","road_distances" : {  "Prazhskaya": 4650,  "Biryulyovo Tovarnaya" : 1380,  "Biryulyovo Zapadnoye" : 2500},"longitude" : 37.645687,"name" : "Universam","latitude" : 55.587655 })");
    transport.ExecuteStringRequest(R"({"type": "Stop","road_distances" : {  "Universam": 890},"longitude" : 37.653656,"name" : "Biryulyovo Tovarnaya","latitude" : 55.592028})");
    transport.ExecuteStringRequest(R"({  "type": "Stop",  "road_distances" : {},  "longitude" : 37.603938,  "name" : "Prazhskaya",  "latitude" : 55.611717})");
    transport.CreateTransportGuide();
    transport.WriteResponseByStringRequest(R"({ "type": "Bus", "name" : "297", "id" : 1})");
    expected = (R"({ "request_id": 1, "route_length": 5990, "curvature": 1.42963, "stop_count": 4, "unique_stop_count": 3 })");
    ASSERT_EQUAL(expected, out.str());
    out.str("");

    transport.WriteResponseByStringRequest(R"({  "type": "Bus", "name" : "635",  "id" : 2 })");
    expected = (R"({ "request_id": 2, "route_length": 11570, "curvature": 1.30156, "stop_count": 5, "unique_stop_count": 3 })");
    ASSERT_EQUAL(expected, out.str());
    out.str("");

    transport.WriteResponseByStringRequest(R"({"type": "Stop","name" : "Universam","id" : 3    })");
    expected = (R"({ "request_id": 3, "buses": ["297", "635"] })");
    ASSERT_EQUAL(expected, out.str());
    out.str("");

    transport.WriteResponseByStringRequest(R"({"type": "Route","from" : "Biryulyovo Zapadnoye","to" : "Universam","id" : 4     })");
    expected = (R"({ "request_id": 4, "items": [{"time": 6, "type": "Wait", "stop_name": "Biryulyovo Zapadnoye"},{"span_count": 2, "bus": "297", "type": "Bus", "time": 5.235}], "total_time": 11.235 })");
    ASSERT_EQUAL(expected, out.str());
    out.str("");

    transport.WriteResponseByStringRequest(R"({"type": "Route","from" : "Biryulyovo Zapadnoye","to" : "Prazhskaya","id" : 5     })");
    expected = (R"({ "request_id": 5, "items": [{"time": 6, "type": "Wait", "stop_name": "Biryulyovo Zapadnoye"},{"span_count": 1, "bus": "297", "type": "Bus", "time": 3.9}, {"time": 6, "type": "Wait", "stop_name": "Biryulyovo Tovarnaya"},{"span_count": 2, "bus": "635", "type": "Bus", "time": 8.31}], "total_time": 24.21 })");
    ASSERT_EQUAL(expected, out.str());
    out.str("");
}

void Tests()
{
	TestRunner tr;
	RUN_TEST(tr, TestRequests);
    RUN_TEST(tr, TestRequestsE);
	RUN_TEST(tr, TestRequestsJson);

}

int main()
{
  //Tests();

    auto transport = TransportJson();
    transport.Set(cin);
	return 0;
};