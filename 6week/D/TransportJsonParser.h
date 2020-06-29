#pragma once
#include "json.h"

namespace TransportParser
{
	struct Requests
	{
		std::vector<std::string> base_requests;
		std::vector<std::string> stat_requests;
	};

	class TJson
	{
	public:
		TJson() = default;

		Requests GetInfo(std::istream& data);

	private:
	};
};

