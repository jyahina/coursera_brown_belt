#pragma once
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "Utilities.hpp"
#include "BusStopInfo.h"

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
	
	long double GetLengthRoute(const std::unordered_map<std::string, BusStopInfo>& busStopDetails) const;

	std::string  GetDetail(const std::unordered_map<std::string, BusStopInfo>& busStopDetails) const;
private:
	std::string number;

	bool isRing = false;
	std::vector<const std::string*> busStops;
	std::unordered_set<std::string> uniqueBusStops;

	bool isRingRoute() const;
};
