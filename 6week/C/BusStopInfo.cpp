#include "BusStopInfo.h"

BusStopInfo::BusStopInfo(std::string_view nameBusStop, std::string_view coordinatesBusStop)
{
	name = move(std::string(nameBusStop));
	SetDetails(coordinatesBusStop);
}

BusStopInfo::BusStopInfo(std::string_view nameBusStop)
{
	name = move(std::string(nameBusStop));
}

void BusStopInfo::SetBus(std::string nameBus)
{
	buses.emplace(move(nameBus));
}

void BusStopInfo::SetDetails(std::string_view details)
{
	coordinates = utilities::Coordinates(details);

	while (!details.empty()) SetDistance(details);
}

void BusStopInfo::SetDistance(std::string name, double dist)
{
	if (auto it = distance.find(name); it == distance.end())
	{
		distance[move(name)] = dist;
	}
}

void BusStopInfo::SetDistance(std::string_view& details)
{
	auto dist = utilities::getPartLine(details, 'm');
	utilities::getPartLine(details);
	utilities::getPartLine(details);

	auto nameBusStop = utilities::getPartLine(details, ',');

	std::stringstream(move(std::string(dist))) >> distance[move(std::string(nameBusStop))];
}

const BusStopInfo::DistanceData& BusStopInfo::GetDistanceData() const
{
	return distance;
}

const utilities::Coordinates& BusStopInfo::GetCoordinates() const
{
	return coordinates;
}

double BusStopInfo::GetDistance(const std::string& name) const
{
	return distance.at(name);
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
