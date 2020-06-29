#include "BusRoute.h"
#include <sstream>
#include <iostream>
#include <iomanip>

InfoRoute::InfoRoute(std::string_view numberBus)
{
	number = move(std::string(numberBus));
}

std::string InfoRoute::GetName() const
{
	return number;
}

void InfoRoute::SetRingParameter(bool ringParameter)
{
	isRing = ringParameter;
}

void InfoRoute::SetBusStop(std::string_view busStopInfo)
{
	auto [stopIt, inserted] = uniqueBusStops.insert(move(std::string(busStopInfo)));
	busStops.push_back(&*stopIt);
}

size_t InfoRoute::GetBusStopCount() const
{
	return (isRingRoute()) ? busStops.size() : 2 * busStops.size() - 1;
}

size_t InfoRoute::GetUniqueBuStopCont() const
{
	return uniqueBusStops.size();
}

long double InfoRoute::GetLengthGeographicalRoute(const std::unordered_map<std::string, BusStopInfo>& busStopDetails) const
{
	long double lengthRoute = 0.;
	
	for (size_t num = 0; num < busStops.size() - 1; ++num )
	{
		lengthRoute += utilities::GetLength(
			busStopDetails.at(*busStops[num]).GetCoordinates(),
			busStopDetails.at(*busStops[num + 1]).GetCoordinates()
		);
	}

	if (!isRingRoute()) lengthRoute *= 2.;

	return lengthRoute;
}

double InfoRoute::GetLengthRoadRoute(const std::unordered_map<std::string, BusStopInfo>& busStopDetails) const
{
	double lengthRoute = 0.;

	for (size_t num = 0; num < busStops.size() - 1; ++num)
	{
		lengthRoute += busStopDetails.at(*busStops[num]).GetDistance(*busStops[num + 1]);
	}

	if (!isRingRoute()) {
	
		for (size_t num = busStops.size() - 1; num != 0; --num)
		{
			lengthRoute += busStopDetails.at(*busStops[num]).GetDistance(*busStops[num - 1]);
		}
	}

	return lengthRoute;
}

std::string InfoRoute::GetDetail(const std::unordered_map<std::string, BusStopInfo>& busStopDetails) const
{
	std::stringstream result;
	result.precision(7);
	double lenghtRoute = GetLengthRoadRoute(busStopDetails);

	result << "Bus " << number << ": "
		<< GetBusStopCount() << " stops on route, "
		<< GetUniqueBuStopCont() << " unique stops, "
		<< lenghtRoute << " route length, "
		<< lenghtRoute / GetLengthGeographicalRoute(busStopDetails) << " curvature";

	return result.str();
}

std::vector<const std::string*> InfoRoute::GetRoute() const
{
	return busStops;
}

bool InfoRoute::isRingRoute() const
{
	return isRing;
}
