#pragma once

#include <ctime>
#include <iostream>
#include <sstream>
#include <clocale>
#include <cstdio>
#include <vector>
#include <algorithm>
#include "test_runner.h"
#include <optional>

#define SECONDS_IN_DAY 86400

using namespace std;

pair<string_view, optional<string_view>> SplitTwoStrict(string_view s, string_view delimiter = " ")
{
	const size_t pos = s.find(delimiter);
	if (pos == s.npos)
	{
		return { s, nullopt };
	}
	else
	{
		return { s.substr(0, pos), s.substr(pos + delimiter.length()) };
	}
}

pair<string_view, string_view> SplitTwo(string_view s, string_view delimiter = " ")
{
	const auto [lhs, rhs_opt] = SplitTwoStrict(s, delimiter);
	return { lhs, rhs_opt.value_or("") };
}

int ConvertToInt(string_view str)
{
	// use std::from_chars when available to git rid of string copy
	size_t pos;
	const int result = stoi(string(str), &pos);
	if (pos != str.length())
	{
		std::stringstream error;
		error << "string " << str << " contains " << (str.length() - pos) << " trailing chars";
		throw invalid_argument(error.str());
	}
	return result;
}
string_view ReadToken(string_view& s, string_view delimiter = " ")
{
	const auto [lhs, rhs] = SplitTwo(s, delimiter);
	s = rhs;
	return lhs;
}

template <typename Number>
void ValidateBounds(Number number_to_check, Number min_value, Number max_value)
{
	if (number_to_check < min_value || number_to_check > max_value)
	{
		std::stringstream error;
		error << number_to_check << " is out of [" << min_value << ", " << max_value << "]";
		throw out_of_range(error.str());
	}
}

class Date
{
public:
	Date(int day, int month, int year)
		: day_(day)
		, month_(month)
		, year_(year)
	{
		CheckDate();
	};

	Date(string_view date)
	{
		year_ = ConvertToInt(ReadToken(date, "-"));
		month_ = ConvertToInt(ReadToken(date, "-"));
		day_ = ConvertToInt(date);
		CheckDate();
	};

	time_t AsTimestamp() const {
		std::tm t;
		t.tm_sec = 0;
		t.tm_min = 0;
		t.tm_hour = 0;
		t.tm_mday = day_;
		t.tm_mon = month_ - 1;
		t.tm_year = year_ - 1900;
		t.tm_isdst = 0;
		return mktime(&t);
	}
private:
	int day_;
	int month_;
	int year_;

	void CheckDate()
	{
		if (month_ < 1 || month_ > 12)
		{
			throw invalid_argument("Month value is invalid: " + month_);
		}

		if (day_ < 1 || day_ > 31)
		{
			throw invalid_argument("Day value is invalid: " + day_);
		}

	}
};


class PersonalBudgetManager
{
public:

	explicit PersonalBudgetManager()
	{
		initDate();
	};

	double ComputeIncome(const Date& timeFrom, const Date& timeTo)
	{
		double earnings = 0.;
		setData(timeFrom, timeTo, [&](InfoBudget& n) { earnings += n.earnings - n.spendings; });
		return earnings;
	};

	void Earn(const Date& timeFrom, const Date& timeTo, double value)
	{
		double dayBudget = value / (ComputeDaysDiff(timeFrom, timeTo) + 1.);
		setData(timeFrom, timeTo, [&](InfoBudget& n) { n.set(dayBudget); });
	};

	void PayTax(const Date& timeFrom, const Date& timeTo, int percentage = 13)
	{
		setData(timeFrom, timeTo, [&](InfoBudget& n) { n.setPayTax(percentage); });
	};

	void Spend(const Date& timeFrom, const Date& timeTo, double value)
	{
		double daySpend = value / (ComputeDaysDiff(timeFrom, timeTo) + 1.);
		setData(timeFrom, timeTo, [&](InfoBudget& n) { n.setSpend(daySpend); });
	};

private:

	struct InfoBudget
	{
		void set(double money)
		{
			earnings += money;
		};

		void setSpend(double money)
		{
			spendings += money;
		};

		void setPayTax(int percentage)
		{
			double percent = 1.0 - percentage / 100.0;
			earnings = percent * earnings;
		}

		double earnings = 0.;
		double spendings = 0.;
	};

	vector<InfoBudget> budget;

	Date startDate = Date("2000-01-01");
	Date endDate = Date("2100-01-01");
	time_t startDateTimeStamp;


	template <typename T>
	void setData(const Date& timeFrom, const Date& timeTo, const T& f)
	{
		std::for_each(
			budget.begin() + getIndex(timeFrom),
			budget.begin() + getIndex(timeTo) + 1,
			f);
	};

	void initDate()
	{
		budget.resize(ComputeDaysDiff(startDate, endDate) + 1);
		startDateTimeStamp = startDate.AsTimestamp();
	};

	int getIndex(const Date& date)
	{
		return (date.AsTimestamp() - startDateTimeStamp) / SECONDS_IN_DAY;
	};

	int ComputeDaysDiff(const Date& date_from, const Date& date_to)
	{
		const time_t timestamp_to = date_to.AsTimestamp();
		const time_t timestamp_from = date_from.AsTimestamp();
		return (timestamp_to - timestamp_from) / SECONDS_IN_DAY;
	};
};

void GeneralMain()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	auto manager = PersonalBudgetManager();

	int query_count;
	cin >> query_count;
	cout.precision(25);

	for (int query_id = 0; query_id < query_count; ++query_id) {
		string query_type;

		cin >> query_type;

		if (query_type == "ComputeIncome")
		{
			string time_from, time_to;
			cin >> time_from >> time_to;
			cout << manager.ComputeIncome(Date(time_from), Date(time_to)) << "\n";;
		}
		else if (query_type == "Earn")
		{
			string time_from, time_to;
			double value;
			cin >> time_from >> time_to >> value;

			manager.Earn(Date(time_from), Date(time_to), value);
		}
		else if (query_type == "PayTax")
		{
			string time_from, time_to;
			int percentage;
			cin >> time_from >> time_to >> percentage;

			manager.PayTax(Date(time_from), Date(time_to), percentage);
		}
		else if (query_type == "Spend")
		{
			string time_from, time_to;
			double value;
			cin >> time_from >> time_to >> value;

			manager.Spend(Date(time_from), Date(time_to), value);
		}
	}

}

void Tests()
{
	double price = 0.;
	auto manager = PersonalBudgetManager();

	manager.Earn(Date("2000-01-02"), Date("2000-01-06"), 20);
	price = manager.ComputeIncome(Date("2000-01-01"), Date("2001-01-01"));
	ASSERT_EQUAL(price, 20.);

	manager.PayTax(Date("2000-01-02"), Date("2000-01-03"));
	price = manager.ComputeIncome(Date("2000-01-01"), Date("2001-01-01"));
	ASSERT_EQUAL(price, 18.96);

	manager.Earn(Date("2000-01-03"), Date("2000-01-03"), 10);
	price = manager.ComputeIncome(Date("2000-01-01"), Date("2001-01-01"));
	ASSERT_EQUAL(price, 28.96);

	manager.PayTax(Date("2000-01-03"), Date("2000-01-03"));
	price = manager.ComputeIncome(Date("2000-01-01"), Date("2001-01-01"));
	ASSERT_EQUAL(price, 27.2076);

	price = 0.;
	manager = PersonalBudgetManager();

	manager.Earn(Date("2000-01-02"), Date("2000-01-06"), 20.);
	price = manager.ComputeIncome(Date("2000-01-01"), Date("2001-01-01"));
	ASSERT_EQUAL(price, 20.);

	manager.PayTax(Date("2000-01-02"), Date("2000-01-03"), 13);
	price = manager.ComputeIncome(Date("2000-01-01"), Date("2001-01-01"));
	ASSERT_EQUAL(price, 18.96);

	manager.Spend(Date("2000-12-30"), Date("2001-01-02"), 14);
	price = manager.ComputeIncome(Date("2000-01-01"), Date("2001-01-01"));
	ASSERT_EQUAL(price, 8.46);

	manager.PayTax(Date("2000-12-30"), Date("2000-12-30"), 13);
	price = manager.ComputeIncome(Date("2000-01-01"), Date("2001-01-01"));
	ASSERT_EQUAL(price, 8.46);

};

int main()
{
	//Tests();
	GeneralMain();
	return 0;
}