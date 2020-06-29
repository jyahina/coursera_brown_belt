#pragma once
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "Utilities.hpp"

class InfoRoute
{
public:
	InfoRoute() = default;

	InfoRoute(std::string_view numberRoute);

	void setRingParameter(bool ringParameter);

	void SetBusStop(std::string_view busStopInfo);
	
	size_t GetBusStopCount() const;
	
	size_t GetUniqueBuStopCont() const;
	
	long double GetLengthRoute(const utilities::BusStopInfo& busStopDetails) const;

	std::string  GetDetail(const utilities::BusStopInfo& busStopDetails) const;
private:
	std::string number;

	bool isRing = false;
	std::vector<const std::string*> busStops;
	std::unordered_set<std::string> uniqueBusStops;

	bool isRingRoute() const;
};
