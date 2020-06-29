#pragma once

#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "json.h"
#include "BusRoute.h"
#include "BusStopInfo.h"
#include "TransportGuide.h"

class TransportJson
{
public:

	explicit TransportJson(std::ostream& ou = std::cout);
	
	void Set(std::istream& data);
	std::ostream& Get();

	void ExecuteStringRequest(const std::string& request);
	void WriteResponseByStringRequest(const std::string& request);
	void SetRouteStringDetails(const std::string& request);
	void CreateTransportGuide();

	void ExecuteRequest(const Json::Node& request);
	void WriteResponseByRequest(const Json::Node& request);
private:

	struct Requests
	{
		std::vector<Json::Node> base_requests;
		std::vector<Json::Node> stat_requests;
	};

	std::ostream& out;

	BusRouteData busRoute;
	BusStopsData busStops;
	RouteDetails routeDetails;
	TransportGuide transportGuide;

	Requests GetInfo(std::istream& data);

	void SetInfo(const std::vector<Json::Node>& data);
	void SetRouteDetails(const std::map<std::string, Json::Node> &map);
	void WriteResponse(const std::vector<Json::Node>& data);

	void WriteBusResponse(const std::map<std::string, Json::Node>& request);
	void WriteStopResponse(const std::map<std::string, Json::Node>& request);
	void WriteRouteResponse(const std::map<std::string, Json::Node>& request);

	BusStopInfo& GetBusStopData(std::string name);

	void SetBusStopsActualDistance(std::string_view name, const BusStopInfo::DistanceData& distanceData);

};