#include "ini.h"

Ini::Section& Ini::Document::AddSection(string name)
{
	sections.insert({ name, Section() });

	return sections[name];

}

const Ini::Section& Ini::Document::GetSection(const string& name) const
{
	return sections.at(name);
}

size_t Ini::Document::SectionCount() const
{
	return sections.size();
}

pair<string_view, string_view> Split(string_view line, char by) {
	size_t pos = line.find(by);
	string_view left = line.substr(0, pos);

	if (pos < line.size() && pos + 1 < line.size()) {
		return { left, line.substr(pos + 1) };
	}
	else {
		return { left, string_view() };
	}
}

string_view Unquote(string_view value) {
	if (!value.empty() && value.front() == '"') {
		value.remove_prefix(1);
	}
	if (!value.empty() && value.back() == '"') {
		value.remove_suffix(1);
	}
	return value;
}

Ini::Document Ini::Load(istream& input)
{
	Document doc;

	Section* newSecion = nullptr;

	for (string line; getline(input, line);)
	{
		size_t startSection = line.find_first_of("[");

		if (startSection != string::npos)
		{
			size_t endSection = line.find_last_of("]") - 1;
			newSecion = &doc.AddSection(move(line.substr(startSection + 1, endSection)));
			
			continue;
		}

		auto [head, tail] = Split(line, ' ');
		auto [name, value] = Split(line, '=');
		if (!name.empty() && !value.empty()) {
			(*newSecion)[move(string(Unquote(name)))] = move(string(Unquote(value)));
		}
		
	}

	return doc;
}
