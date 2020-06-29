#pragma once

#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "BusRoute.h"

class Transport
{
public:
	explicit Transport(std::ostream& out_ = std::cout);
	void MainProcess(std::istream& in = std::cin);
	void ExecuteRequest(const std::string& request);

	void SetBusStops(std::string_view name, std::string_view stops);
	void SetBusStopsCoordinates(std::string_view name, std::string_view details);
	std::string GetInfoBusStops(std::string_view name);

private:
	std::ostream& out;
	utilities::BusStopInfo busStops;
	std::unordered_map<std::string, InfoRoute> busRoute;
};