#include "Transport.h"
#include <sstream>
#include <math.h>
#include <algorithm>

using namespace std;


Transport::Transport(ostream& out_) 
	:  out(out_)
{}

void Transport::MainProcess(std::istream& in)
{
	ios::sync_with_stdio(false);
	in.tie(nullptr);
	out.precision(6);

	string request;
	int query_count;

	for (auto i : { 1, 2 })
	{
		 in >> query_count;
		 getline(in, request);

		for (int query_id = 0; query_id < query_count; ++query_id)
		{
			getline(in, request);
			ExecuteRequest(request);
		}
	}

}
void Transport::ExecuteRequest(const std::string& request)
{
	string_view line(request);
	auto query_type = utilities::getPartLine(line);
	auto nameBusStop = utilities::getPartLine(line, ':');

	if (query_type == "Stop")
	{
		if (line.empty()) out << GetInfoBusStops(nameBusStop) << "\n";
		else SetBusStopsCoordinates(nameBusStop, line);
	}
	else if (query_type == "Bus")
	{
		if (line.empty()) out << GetInfoBus(nameBusStop) << "\n";
		else SetBusStops(nameBusStop, line);
	}
}

void Transport::SetBusStops(std::string_view name, std::string_view stops)
{
	InfoRoute route(name);
	route.SetRingParameter(stops.find_first_of(">") != std::string_view::npos);

	while (!stops.empty())
	{
		utilities::getPartLine(stops);

		auto busStopName = utilities::getPartLine(stops, stops.find_first_of(">-"));
		route.SetBusStop(busStopName);
		
		auto busStopNameStr = string(busStopName);

		if (auto itBusStop = busStops.find(busStopNameStr); itBusStop == busStops.end())
		{
			auto& busStop = busStops[move(busStopNameStr)];
			busStop = BusStopInfo(busStopName);
			busStop.setBus(route.GetName());
		} else 
			itBusStop->second.setBus(route.GetName());
	}
	
	busRoute[move(string(name))] = move(route);
}

void Transport::SetBusStopsCoordinates(std::string_view name, std::string_view details)
{
	auto nameBusStop = string(name);
	auto busStopsIt = busStops.find(nameBusStop);

	if (busStopsIt != busStops.end())
	{
		busStopsIt->second.setCoordinate(details);
	}
	else {
		busStops[move(nameBusStop)] = BusStopInfo(name, details);
	}
}

string Transport::GetInfoBus(std::string_view name)
{
	const auto& bus = busRoute.find(string(name));

	return (bus == busRoute.end())
		? "Bus " + move(string(name)) + ": not found"
		: bus->second.GetDetail(busStops);
}

std::string Transport::GetInfoBusStops(std::string_view name)
{
	const auto& busStop = busStops.find(string(name));

	return (busStop == busStops.end())
		? "Stop " + move(string(name)) + ": not found"
		: busStop->second.GetDetail();
}

