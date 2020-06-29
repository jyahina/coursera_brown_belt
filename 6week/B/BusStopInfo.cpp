#include "BusStopInfo.h"

BusStopInfo::BusStopInfo(std::string_view nameBusStop, std::string_view details)
{
	name = move(std::string(nameBusStop));
	setCoordinate(details);
}

BusStopInfo::BusStopInfo(std::string_view nameBusStop)
{
	name = move(std::string(nameBusStop));
}

void BusStopInfo::setBus(std::string nameBus)
{
	buses.emplace(move(nameBus));
}

void BusStopInfo::setCoordinate(std::string_view coord)
{
	coordinates = utilities::Coordinates(coord);
}

const utilities::Coordinates& BusStopInfo::GetCoordinates() const
{
	return coordinates;
}

std::string BusStopInfo::GetDetail() const
{
	std::stringstream result;
	result << "Stop " << name << ": ";

	if (buses.empty()) {
		result << "no buses";
		return result.str();
	}
	result << "buses";

	for (const auto& elem : buses)
	{
		result << " " << elem;
	}

	return result.str();
}
