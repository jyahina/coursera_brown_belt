#pragma once
#include <string_view>
#include <unordered_set>
#include <set>
#include "Utilities.hpp"

class BusStopInfo
{
public:
	BusStopInfo() = default;

	BusStopInfo(std::string_view nameBusStop, std::string_view details);

	BusStopInfo(std::string_view nameBusStop);

	void setBus(std::string nameBus);

	void setCoordinate(std::string_view coord);

	const utilities::Coordinates& GetCoordinates() const;

	std::string GetDetail() const;

private:
	std::string name;
	utilities::Coordinates coordinates;
	std::set<std::string> buses;
};