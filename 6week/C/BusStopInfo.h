#pragma once
#include <string_view>
#include <unordered_set>
#include <set>
#include "Utilities.hpp"

class BusStopInfo
{
public:
	using DistanceData = std::unordered_map<std::string, double>;

	BusStopInfo() = default;

	BusStopInfo(std::string_view nameBusStop, std::string_view coordinatesBusStop);

	BusStopInfo(std::string_view nameBusStop);

	void SetBus(std::string nameBus);

	void SetDetails(std::string_view details);

	void SetDistance(std::string name, double dist);

	void SetDistance(std::string_view& details);

	const DistanceData& GetDistanceData() const;

	const utilities::Coordinates& GetCoordinates() const;

	double GetDistance(const std::string& name) const;

	std::string GetDetail() const;

private:

	std::string name;
	utilities::Coordinates coordinates;
	std::set<std::string> buses;
	DistanceData distance; // map<nameBusStop, distance>
};