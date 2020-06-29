#include "TransportJson.h"
#define ERROR_MESSAGE "not found"

TransportJson::TransportJson(std::ostream& ou) : out(ou)
{}

void TransportJson::Set(std::istream& data)
{
	auto result = GetInfo(data);
	SetInfo(result.base_requests);
	CreateTransportGuide();
	WriteResponse(result.stat_requests);
}

void TransportJson::CreateTransportGuide()
{
	transportGuide = TransportGuide(routeDetails.bus_wait_time, routeDetails.bus_velocity);
	transportGuide.createGuied(busStops, busRoute);
}

std::ostream& TransportJson::Get()
{
	return out;
}

TransportJson::Requests TransportJson::GetInfo(std::istream& data)
{
	auto doc = Json::Load(data);
	const auto& req = doc.GetRoot().AsMap();

	SetRouteDetails(req.find("routing_settings")->second.AsMap());

	return { req.at("base_requests").AsArray() , req.at("stat_requests").AsArray() };
}

void TransportJson::SetInfo(const std::vector<Json::Node>& data)
{
	for (auto& request : data)
	{
		ExecuteRequest(request);
	}
}

void TransportJson::SetRouteDetails(const std::map<std::string, Json::Node>& map)
{
	routeDetails.bus_velocity = map.at("bus_velocity").AsDouble();
	routeDetails.bus_wait_time = map.at("bus_wait_time").AsDouble();
}

void TransportJson::WriteResponse(const std::vector<Json::Node>& data)
{
	out << "[";

	for (size_t i = 0; i < data.size(); ++i)
	{
		WriteResponseByRequest(data[i]);
		if (i < data.size() - 1) out << ", ";
	}

	out << "]";
}

void TransportJson::ExecuteStringRequest(const std::string& request)
{
	std::stringstream ss_q(request);
	auto doc = Json::Load(ss_q);
	ExecuteRequest(doc.GetRoot());
}

void TransportJson::ExecuteRequest(const Json::Node& request)
{
	const auto& req = request.AsMap();

	auto typeRequest = req.at("type").AsString();

	if (typeRequest == "Bus")
	{
		auto name = req.at("name").AsString();

		InfoRoute route(name);
		route.SetRingParameter(req.at("is_roundtrip").AsBool());

		for (const auto& busStopNameNode : req.at("stops").AsArray())
		{
			auto busStopName = busStopNameNode.AsString();
			route.SetBusStop(busStopName);

			auto& busStop = GetBusStopData(std::move(std::string(busStopName)));
			busStop.SetBus(route.GetName());
		}

		busRoute[std::move(std::string(name))] = std::move(route);
	}
	else if (typeRequest == "Stop")
	{
		auto name = req.at("name").AsString();
		auto& stopBus = GetBusStopData(name);

		stopBus.SetCoordinates(req.at("latitude").AsDouble(), req.at("longitude").AsDouble());

		for (const auto& [k, v] : req.at("road_distances").AsMap())
		{
			stopBus.SetDistance(k, v.AsDouble(), true);
		}
		SetBusStopsActualDistance(name, stopBus.GetDistanceData());
	}
}

void TransportJson::WriteResponseByStringRequest(const std::string& request)
{
	std::stringstream ss_q(request);
	auto doc = Json::Load(ss_q);
	WriteResponseByRequest(doc.GetRoot());
}

void TransportJson::SetRouteStringDetails(const std::string& request)
{
	std::stringstream ss_q(request);
	auto doc = Json::Load(ss_q);
	const auto& req = doc.GetRoot().AsMap();

	SetRouteDetails(req.find("routing_settings")->second.AsMap());
}


void TransportJson::WriteResponseByRequest(const Json::Node& request)
{
	const auto& req = request.AsMap();

	auto typeRequest = req.at("type").AsString();

	if (typeRequest == "Bus") WriteBusResponse(req);
	else if (typeRequest == "Stop") WriteStopResponse(req);
	else if (typeRequest == "Route") WriteRouteResponse(req);
}

void TransportJson::WriteStopResponse(const std::map<std::string, Json::Node>& request)
{
	auto name = request.at("name").AsString();

	const auto& busStop = busStops.find(name);

	out << "{ \"request_id\": " << request.at("id").AsInt() << ", ";

	if (busStop == busStops.end())
	{
		out << "\"error_message\": \"" << ERROR_MESSAGE << '"';
	}
	else {
		out << "\"buses\": [";

		const auto& buses = busStop->second.GetBuses();
		for (auto it = buses.begin(); it != buses.end(); it = next(it))
		{
			out << '"' << *it << '"';

			if (next(it) != buses.end())
			{
				out << ", ";
			}
		}

		out << "]";
	}

	out << " }";
}

void TransportJson::WriteBusResponse(const std::map<std::string, Json::Node>& request)
{
	auto name = request.at("name").AsString();

	const auto& bus = busRoute.find(name);

	out << "{ \"request_id\": " << request.at("id").AsInt() << ", ";

	if (bus == busRoute.end())
	{
		out << "\"error_message\": \"" << ERROR_MESSAGE << '"';
	}
	else {
		out.precision(6);
		double lenghtRoute = bus->second.GetLengthRoadRoute(busStops);

		out << "\"route_length\": " << lenghtRoute << ", " <<
			"\"curvature\": " << lenghtRoute / bus->second.GetLengthGeographicalRoute(busStops) << ", " <<
			"\"stop_count\": " << bus->second.GetBusStopCount() << ", " <<
			"\"unique_stop_count\": " << bus->second.GetUniqueBuStopCont();
	}

	out << " }";
}

void TransportJson::WriteRouteResponse(const std::map<std::string, Json::Node>& request)
{
	auto nameFrom = request.at("from").AsString();
	auto nameTo = request.at("to").AsString();

	out << "{ \"request_id\": " << request.at("id").AsInt() << ", ";

	if (const auto& answer = transportGuide.GetOptimalRoute(nameFrom, nameTo);
		std::holds_alternative<bool>(answer))
	{
		out << "\"error_message\": \"" << ERROR_MESSAGE << '"';
	}
	else {
		out << std::get<std::string>(answer);
	}

	out << " }";
}

void TransportJson::SetBusStopsActualDistance(std::string_view name, const BusStopInfo::DistanceData& distanceData)
{
	for (auto [key, value] : distanceData)
	{
		auto& busStop = GetBusStopData(key);
		busStop.SetDistance(std::string(name), value);
	}
}

BusStopInfo& TransportJson::GetBusStopData(std::string name)
{
	auto itBusStop = busStops.find(name);

	return (itBusStop != busStops.end())
		? itBusStop->second
		: busStops[name] = BusStopInfo(name);
}