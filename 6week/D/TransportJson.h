#pragma once

#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "BusRoute.h"
#include "BusStopInfo.h"
#include "TransportJsonParser.h"

class TransportJson
{
public:

	explicit TransportJson(std::ostream& ou = std::cout);
	
	void Set(std::istream& data);
	std::ostream& Get();

	void ExecuteRequest(const std::string& request);
	void WriteResponseByRequest(const std::string& request);
private:
	using BusRouteData = std::unordered_map<std::string, InfoRoute>;
	using BusStopsData = std::unordered_map<std::string, BusStopInfo>;

	std::ostream& out;
	TransportParser::TJson parser;

	BusRouteData busRoute;
	BusStopsData busStops;

	void SetInfo(const std::vector<std::string> data);
	void WriteResponse(const std::vector<std::string> data);

	void WriteBusResponse(const std::map<std::string, Json::Node>& request);
	void WriteStopResponse(const std::map<std::string, Json::Node>& request);

	BusStopInfo& GetBusStopData(std::string name);
	void SetBusStopsActualDistance(std::string_view name, const BusStopInfo::DistanceData& distanceData);

};