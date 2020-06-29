#include "BusRoute.h"
#include <sstream>
#include <iostream>
#include <iomanip>

InfoRoute::InfoRoute(std::string_view numberBus)
{
	number = move(std::string(numberBus));
}

void InfoRoute::setRingParameter(bool ringParameter)
{
	isRing = ringParameter;
}

void InfoRoute::SetBusStop(std::string_view busStopInfo)
{
	utilities::removeSpaceInEndLine(busStopInfo);

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

long double InfoRoute::GetLengthRoute(const utilities::BusStopInfo& busStopDetails) const
{
	long double lengthRoute = 0.;
	
	for (size_t num = 0; num < busStops.size() - 1; ++num )
	{
		lengthRoute += utilities::GetLength(
			busStopDetails.at(*busStops[num]),
			busStopDetails.at(*busStops[num + 1])
		);
	}

	if (!isRingRoute()) lengthRoute *= 2.;

	return lengthRoute;
}

std::string InfoRoute::GetDetail(const utilities::BusStopInfo& busStopDetails) const
{
	std::stringstream result;
	result.precision(6);

	result << "Bus " << number << ": "
		<< GetBusStopCount() << " stops on route, "
		<< GetUniqueBuStopCont() << " unique stops, "
		<< GetLengthRoute(busStopDetails) << " route length";

	return result.str();
}

bool InfoRoute::isRingRoute() const
{
	return isRing;
}
