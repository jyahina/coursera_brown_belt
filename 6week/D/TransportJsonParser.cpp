#include "TransportJsonParser.h"

TransportParser::Requests TransportParser::TJson::GetInfo(std::istream& data)
{
	return {Json::splitJson(data), Json::splitJson(data) };
}
