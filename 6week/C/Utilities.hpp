#pragma once
#include <string_view>
#include <unordered_map>
#include <math.h>
#include <algorithm>
#include <sstream>
#include <utility>
#define PI  3.1415926535

namespace utilities
{
	inline void removeSpaceInEndLine(std::string_view& line)
	{
		if (auto pos = line.find_last_of(' '); 
			pos != std::string_view::npos && pos == line.size() - 1)
		{
			line.remove_suffix(line.size() - pos);
		}
	};

	inline std::string_view getPartLine(std::string_view& line, char delim = ' ')
	{
		auto pos = line.find(delim);
		std::string_view result = line;

		if (pos != std::string_view::npos)
		{
			result.remove_suffix(result.size() - pos);
			line.remove_prefix(pos + 1);
		}
		else
		{
			line.remove_prefix(line.size());
		}

		removeSpaceInEndLine(result);
		return result;
	};

	inline std::string_view getPartLine(std::string_view& line, size_t pos)
	{
		std::string_view result = line;

		if (pos != std::string_view::npos)
		{
			result.remove_suffix(result.size() - pos);
			line.remove_prefix(pos + 1);
		}
		else
		{
			line.remove_prefix(line.size());
		}

		removeSpaceInEndLine(result);
		return result;
	};

	inline long double getRadian(long double degrees)
	{
		return degrees * PI / 180.;
	}

	struct Coordinates
	{
		Coordinates() = default;

		Coordinates(std::string_view& details)
		{
			utilities::getPartLine(details);
			auto latitudePtr = utilities::getPartLine(details, ',');
			std::stringstream(move(std::string(latitudePtr))) >> latitude;

			utilities::getPartLine(details);
			auto longitudePtr = utilities::getPartLine(details, ',');
			std::stringstream(move(std::string(longitudePtr))) >> longitude;

			latitude = utilities::getRadian(latitude);
			longitude = utilities::getRadian(longitude);
		};

		long double latitude = 0.;
		long double longitude = 0.;
	};

	inline long double GetLength(const Coordinates& lhs, const Coordinates& rhs)
	{
		return acos(sin(lhs.latitude) * sin(rhs.latitude) +
			cos(lhs.latitude) * cos(rhs.latitude) *
			cos(abs(lhs.longitude - rhs.longitude))
		) * 6371000.;
	};

	using BusStopInfo = std::unordered_map<std::string, utilities::Coordinates>;

};