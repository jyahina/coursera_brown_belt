#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include "test_runner.h"

using namespace std;

bool reversedLess(const string_view& a, const string_view& b)
{
	size_t minLength = min(a.size(), b.size());
	auto itA = a.rbegin(), itB = b.rbegin();

	for (size_t i = 0; i < minLength; ++i, ++itA, ++itB)
	{
		if (*itA != *itB)
		{
			return *itA < *itB;
		}

		
	}

	return a.size() < b.size();
}

bool reversedLess2(const string_view& a, const string_view& b)
{
	size_t minLength = min(a.size(), b.size());
	auto itA = a.rbegin(), itB = b.rbegin();

	for (size_t i = 0; i < minLength; ++i, ++itA, ++itB)
	{
		if (*itA != *itB)
		{
			return *itA < *itB;
		}


	}

	return a.size() < b.size();
}

bool IsSubdomain(const string_view& subdomain, const string_view& domain) {

	if (subdomain.size() > domain.size()) return false;
	if (subdomain.size() == domain.size()) { if (subdomain == domain) return true; }

	auto i = subdomain.size();
	auto j = domain.size();

	while (i > 0 && j > 0)
	{
 		if (subdomain[--i] != domain[--j])
		{
			return false;
		}
	}

	return (subdomain.front() != '.') ? domain[--j] == '.' : true;
}


vector<string> ReadDomains() {
	size_t count;
	cin >> count;

	vector<string> domains(count);
	string domain;

	for (size_t i = 0; i < count; ++i)
	{
		cin >> domain;
		domains[i] = move(domain);
	}

	return domains;
}

string getStatus(const vector<string>& subdomains, const string_view& domain)
{
	if (domain.empty() || subdomains.empty())
	{
		return "Good";
	}

	vector<string_view> subs;

	for (size_t i = domain.size()-1; i > 0; --i)
	{
		if (domain[i] == '.')
		{
			subs.push_back({ &domain[i + 1], domain.size() - i - 1 });
		}
	}

	vector<string>::const_iterator first, last;
	last = upper_bound(subdomains.begin(), subdomains.end(), domain, reversedLess);
	first = subdomains.begin();

	for (auto& tt : subs)
	{
		first = lower_bound(first, last, tt, reversedLess);
		
		if (first == subdomains.end()) return "Good";

		if (IsSubdomain(*first, domain))
		{
			return "Bad";
		}

	}

	return (*prev(last) != domain) ? "Good" : "Bad";
}

string CheckDomains(vector<string>& banned_domains, const vector<string>& domains_to_check)
{
	string answered;
	sort(banned_domains.begin(), banned_domains.end(), reversedLess); //сортируем с конца домена

	for (const string_view& domain : domains_to_check)
	{
		answered += move(getStatus(banned_domains, domain)) + "\n";
	}

	return answered;
}

void CheckDomains(ostream& out, vector<string>& banned_domains, const vector<string>& domains_to_check)
{
	sort(banned_domains.begin(), banned_domains.end(), reversedLess); //сортируем с конца домена

	for (const auto& domain : domains_to_check)
	{
		out << move(getStatus(banned_domains, domain)) << "\n";
	}
}

void TestSimpleWithPoint()
{
	vector<string> banned_domains = {
		"com"
	};

	vector<string> domains_to_check =
	{
		"com.ru", "ru.com.en", "ru.com"
	};

	ASSERT_EQUAL(CheckDomains(banned_domains, domains_to_check), "Good\nGood\nBad\n");
}

void TestSimple()
{
	vector<string> banned_domains = {
		"ya.ru", "maps.me", "m.ya.ru", "com"
	};

	vector<string> domains_to_check =
	{
		"ya.ru", "ya.com", "m.maps.me", "moscow.m.ya.ru", "maps.com", "maps.ru", "ya.ya"
	};

	ASSERT_EQUAL(CheckDomains(banned_domains, domains_to_check), "Bad\nBad\nBad\nBad\nBad\nGood\nGood\n");
}

void TestEmpty()
{
	vector<string> banned_domains = {};

	vector<string> domains_to_check =
	{
		"ya.ru", "ya.com", "m.maps.me", "moscow.m.ya.ru", "maps.com", "maps.ru", "ya.ya"
	};

	ASSERT_EQUAL(CheckDomains(banned_domains, domains_to_check), "Good\nGood\nGood\nGood\nGood\nGood\nGood\n");

	banned_domains = {};
	domains_to_check = {};

	ASSERT_EQUAL(CheckDomains(banned_domains, domains_to_check), "");
}

void TestSubdomain()
{
	ASSERT_EQUAL(IsSubdomain("ya.ru", "fhdjf.ya.ru"), true);
	ASSERT_EQUAL(IsSubdomain("yg.ru", "fhdjf.ya.ru"), false);
	ASSERT_EQUAL(IsSubdomain("ya.ru", "ya.ru"), true);
	ASSERT_EQUAL(IsSubdomain("ru", "ya.ru"), true);
	ASSERT_EQUAL(IsSubdomain("ru", "ru.ya"), false);
	ASSERT_EQUAL(IsSubdomain("ya.ru", "fhdjfya.ru"), false);
	ASSERT_EQUAL(IsSubdomain("such.domain.ru", "wow.such.domain.ru"), true);
}

void TestDomainBanned()
{
	vector<string> banned = { "ya.ru", "mail.mail.com", "tv", "ru.tv", "", "mail.ru.tv" };
	sort(banned.begin(), banned.end(), reversedLess); //сортируем с конца домена

	ASSERT_EQUAL(getStatus(banned, "mail.ya.ru"), "Bad");
	ASSERT_EQUAL(getStatus(banned, "mail.mail.com"), "Bad");
	ASSERT_EQUAL(getStatus(banned, "bazooka.tv"), "Bad");
	ASSERT_EQUAL(getStatus(banned, "wow.mail.mail.com"), "Bad");
	ASSERT_EQUAL(getStatus(banned, "wow.mail.mail.ru.tv"), "Bad");
	ASSERT_EQUAL(getStatus(banned, "wow.mail.tu.tv"), "Bad");

	ASSERT_EQUAL(getStatus(banned, "mail.ya.rf"), "Good");
	ASSERT_EQUAL(getStatus(banned, "mail.muil.com"), "Good");
	ASSERT_EQUAL(getStatus(banned, "bazooka.ttv"), "Good");
	ASSERT_EQUAL(getStatus(banned, "wow.ru.mail.com"), "Good");
	ASSERT_EQUAL(getStatus(banned, ""), "Good");

	vector<string> bannedEmpty;
	ASSERT_EQUAL(getStatus(bannedEmpty, "wow.mail.mail.com"), "Good");
}

void TestS()
{
	vector<string> banned_domains = {
		"b.c", "a.b.c"
	};

	vector<string> domains_to_check =
	{
		"d.b.c"
	};

	ASSERT_EQUAL(CheckDomains(banned_domains, domains_to_check), "Bad\n");
}

void Tests()
{
	TestRunner tr;
	RUN_TEST(tr, TestSimple);
	RUN_TEST(tr, TestSimpleWithPoint);
	RUN_TEST(tr, TestEmpty);
	RUN_TEST(tr, TestSubdomain);
	RUN_TEST(tr, TestDomainBanned);
	RUN_TEST(tr, TestS);
}

int main() {

	Tests();

	vector<string> banned_domains = move(ReadDomains());
	vector<string> domains_to_check = move(ReadDomains());

	CheckDomains(cout, banned_domains, domains_to_check);

	return 0;
}
