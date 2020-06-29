#include "TransportGuide.h"

TransportGuide::TransportGuide(double waitTimeOnStopBus, double velocity)
	: waitTime(waitTimeOnStopBus)
	, busVelocity(velocity)
{}

void TransportGuide::createGuied(const BusStopsData & busStops, const BusRouteData & busRoute)
{
	SetDirectRoute(busStops, busRoute);
	SetVerticesAndStops(busStops);

	graph = std::make_unique<Graph::DirectedWeightedGraph<long double>>(verticesToStops.size());
	SetWeightEdge();
	
	router = std::make_unique<Graph::Router<long double>>(*graph);
}

std::variant<std::string, bool> TransportGuide::GetOptimalRoute(const std::string& from, const std::string& to)
{
	Graph::VertexId fromV = stopsToVertices.at(from);
	Graph::VertexId toV = stopsToVertices.at(to);

	auto optimalRoute = router->BuildRoute(fromV, toV);

	if (!optimalRoute) return false;
	
	std::stringstream result;
	double totalTime = 0.;
	const auto& route = optimalRoute.value();

	result << "\"items\": [" ;

	for (size_t i = 0; i < route.edge_count; ++i)
	{
		auto edgeId = router->GetRouteEdge(route.id, i);
		auto edge = graph->GetEdge(edgeId);
		result << "{";
		result << "\"time\": " << waitTime << ", ";
		result << "\"type\": " << "\"Wait\"" << ", ";
		result << "\"stop_name\": " << "\"" << verticesToStops.at(edge.from) << "\"";
		result << "},";

		const auto& dRoute = edgesToRoutes.at(edgeId);
		result << "{";
		result << "\"span_count\": " << dRoute.spanCount << ", ";
		result << "\"bus\": " << "\"" << dRoute.bus << "\"" << ", ";
		result << "\"type\": " << "\"Bus\"" << ", ";
		result << "\"time\": " << edge.weight - waitTime ;

		result << "}";

		if(i != route.edge_count - 1) result << ", ";

		totalTime += edge.weight;
	}

	result << "], ";
	result << "\"total_time\": " << totalTime;

	return result.str();
}

void TransportGuide::SetDirectRoute(const BusStopsData& busStops, const BusRouteData& busRoutes)
{
	for (const auto& [name, route] : busRoutes)
	{
		auto busStopsRoute = route.GetRoute();
		for (size_t num_i = 0; num_i < busStopsRoute.size() - 1; ++num_i)
		{
			long double distance = 0.;

			for (size_t num_j = num_i + 1; num_j < busStopsRoute.size(); ++num_j)
			{
				distance += busStops.at(*busStopsRoute[num_j - 1]).GetDistance(*busStopsRoute[num_j]);

				if (busStopsRoute[num_i] != busStopsRoute[num_j])
					SetDirectRouteBetweenBusStop(busStopsRoute[num_i], busStopsRoute[num_j], distance, num_j - num_i, name, busStops);
			}
		}
		
	/*	if (route.isRingRoute()) 
			SetDirectRouteBetweenBusStop(busStopsRoute.back(), busStopsRoute.front(), 1, name, busStops);
		else
		{*/
		if (!route.isRingRoute())
			for (int num_i = busStopsRoute.size() - 1; num_i != 0; --num_i)
			{
				long double distance = 0.;
				for (int num_j = num_i - 1; num_j >= 0; --num_j)
				{
					distance += busStops.at(*busStopsRoute[num_j + 1]).GetDistance(*busStopsRoute[num_j]);

					if (busStopsRoute[num_i] != busStopsRoute[num_j])
						SetDirectRouteBetweenBusStop(busStopsRoute[num_i], busStopsRoute[num_j], distance, num_i - num_j, name, busStops);
				}
			}
		//}
			
	}
}

void TransportGuide::SetDirectRouteBetweenBusStop(const std::string* from, const std::string* to, long double distance, size_t spanCount, std::string nameBus, const BusStopsData& busStops)
{
	auto stopsPair = std::make_pair(*from, *to);
	auto& direct = directRoute[move(stopsPair)];

	if (direct.distance > distance)
	{
		direct.distance = distance;
		direct.spanCount = spanCount;
		direct.bus = nameBus;
	}
}

void TransportGuide::SetVerticesAndStops(const BusStopsData& busStops)
{
	Graph::VertexId currentId = 0;
	for (const auto i : busStops)
	{
		auto stopView = std::string_view(verticesToStops.insert(std::make_pair(currentId, i.first)).first->second);
		stopsToVertices[stopView] = currentId;
		++currentId;
	}
}

void TransportGuide::SetWeightEdge()
{
	if (!graph) return;

	for (const auto& [stops, route] : directRoute)
	{
		auto edge = Graph::Edge<long double>{
			/*from */	stopsToVertices[stops.first],
			/*to */		stopsToVertices[stops.second],
			/*weight */	(route.distance * 3.0l / 50 / busVelocity) + waitTime
		};

		edgesToRoutes[graph->AddEdge(edge)] = route;
	}
}
