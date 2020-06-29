#pragma once

#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "BusRoute.h"
#include "BusStopInfo.h"

class Transport
{
public:
	explicit Transport(std::ostream& out_ = std::cout);
	void MainProcess(std::istream& in = std::cin);
	void ExecuteRequest(const std::string& request);

	void SetBusStops(std::string_view name, std::string_view stops);
	void SetBusStopsDetails(std::string_view name, std::string_view details);

	std::string GetInfoBus(std::string_view name);
	std::string GetInfoBusStops(std::string_view name);

private:
	using BusRouteData = std::unordered_map<std::string, InfoRoute>;
	using BusStopsData = std::unordered_map<std::string, BusStopInfo>;

	std::ostream& out;

	BusRouteData busRoute;
	BusStopsData busStops;

	void SetBusStopsActualDistance(std::string_view name, const BusStopInfo::DistanceData& distanceData);
	BusStopInfo& GetBusStopData(std::string name);
};