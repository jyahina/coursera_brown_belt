#pragma once
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "Utilities.hpp"
#include "BusStopInfo.h"

struct RouteDetails
{
	double bus_wait_time = 0.;
	double bus_velocity = 0.;
};


class InfoRoute
{
public:
	InfoRoute() = default;

	InfoRoute(std::string_view numberRoute);

	std::string GetName() const;

	void SetRingParameter(bool ringParameter);

	void SetBusStop(std::string_view busStopInfo);
	
	size_t GetBusStopCount() const;
	
	size_t GetUniqueBuStopCont() const;
	
	long double GetLengthGeographicalRoute(const std::unordered_map<std::string, BusStopInfo>& busStopDetails) const;

	double GetLengthRoadRoute(const std::unordered_map<std::string, BusStopInfo>& busStopDetails) const;

	std::string GetDetail(const std::unordered_map<std::string, BusStopInfo>& busStopDetails) const;

	std::vector<const std::string*> GetRoute() const;

	bool isRingRoute() const;
private:
	std::string number;

	bool isRing = false;
	std::vector<const std::string*> busStops;
	std::unordered_set<std::string> uniqueBusStops;

};

using BusRouteData = std::unordered_map<std::string, InfoRoute>;
