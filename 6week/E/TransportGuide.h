#pragma once
#include <string>
#include <memory>
#include "router.h"
#include "graph.h"
#include "BusRoute.h"
#include "BusStopInfo.h"
#include <variant>
#include <functional>
#include <unordered_set>
#include <utility>
#include <iomanip>
#include <iostream>
#include <iomanip>

using PairBusStop = std::pair <std::string, std::string>;

struct DirectRoute
{
	std::string bus;        // a bus to ride
	size_t spanCount = 0;       // number of stops between initial and final stop
	long double distance = 1000000000.; // distance
};


class TransportGuide
{
public:
	TransportGuide() = default;
	TransportGuide(double waitTimeOnStopBus, double velocity);

	void createGuied(const BusStopsData& busStops, const BusRouteData& busRoute);

	std::variant<std::string, bool> GetOptimalRoute(const std::string& from, const std::string& to);

private:

	struct StopsPairHasher
	{
		size_t operator()(const std::pair<std::string, std::string>& p) const
		{
			return _shash(p.first) * 1000 + _shash(p.second);
		}
	private:
		std::hash<std::string> _shash;
	};

	double waitTime = 0.;
	double busVelocity = 0.;

	std::unique_ptr<Graph::DirectedWeightedGraph<long double>> graph;
	std::unique_ptr<Graph::Router<long double>> router;

	//utilities
	std::unordered_map<PairBusStop, DirectRoute, StopsPairHasher> directRoute; // Direct route between two busStop
	std::unordered_map<Graph::VertexId, std::string> verticesToStops; // vertices = stops
	std::unordered_map<std::string_view, Graph::VertexId> stopsToVertices; // stops = vertices
	std::unordered_map<Graph::EdgeId, DirectRoute> edgesToRoutes; // вес на ребро

	void SetDirectRoute(const BusStopsData& busStops, const BusRouteData& busRoute);
	void SetDirectRouteBetweenBusStop(const std::string* from, const std::string* to, long double distance, size_t spanCount, std::string nameBus, const BusStopsData& busStops);
	void SetVerticesAndStops(const BusStopsData& busStops);
	void SetWeightEdge();
};