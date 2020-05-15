#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

using namespace std;

struct Record {
	string id;
	string title;
	string user;
	int timestamp;
	int karma;
};

class Database {
public:

	bool Put(const Record& record)
	{
		if (maps_id.find(record.id) == maps_id.end())
		{
			maps_id.emplace(record.id, record);
			auto it = maps_id.find(record.id);

			maps_timestamp.emplace(record.timestamp, it );
			maps_karma.emplace( record.karma, it );
			maps_user.emplace( record.user, it );

			return true;
		}

		return false;
	};

	const Record* GetById(const string& id) const
	{
		if (maps_id.find(id) == maps_id.end())
		{
			return nullptr;
		}
		return &maps_id.at(id);
	};

	bool Erase(const string& id)
	{
		auto record = maps_id.find(id);

		if (maps_id.find(id) != maps_id.end())
		{
			maps_timestamp.extract(record->second.timestamp);
			maps_karma.extract(record->second.karma);
			maps_user.extract(record->second.user);
			maps_id.extract(record);

			return true;
		}

		return false;
	};

	template <typename Callback>
	void RangeByTimestamp(int low, int high, Callback callback) const
	{
		for (auto timestamp = low; timestamp <= high; timestamp++)
		{
			auto elements = maps_timestamp.equal_range(timestamp);

			for (auto el = elements.first; el != elements.second; el++)
			{
				if (!callback(el->second->second))
					return;
			}

		}
	};

	template <typename Callback>
	void RangeByKarma(int low, int high, Callback callback) const
	{
		for (auto karma = low; karma <= high; karma++)
		{
			auto elements = maps_karma.equal_range(karma);

			for (auto el = elements.first; el != elements.second; el++)
			{
				if (!callback(el->second->second))
					return;
			}

		}
	};

	template <typename Callback>
	void AllByUser(const string& user, Callback callback) const
	{
		auto elements = maps_user.equal_range(user);

		for (auto el = elements.first; el != elements.second; el++)
		{
			if (!callback(el->second->second))
				return;
		}
	};

private:

	unordered_map<string, Record> maps_id;
	multimap<int, unordered_map<string, Record>::const_iterator> maps_timestamp;
	multimap<int, unordered_map<string, Record>::const_iterator> maps_karma;
	multimap<string, unordered_map<string, Record>::const_iterator> maps_user;

};

void TestRangeBoundaries() {
	const int good_karma = 1000;
	const int bad_karma = -10;

	Database db;
	db.Put({ "id1", "Hello there", "master", 1536107260, good_karma });
	db.Put({ "id2", "O>>-<", "general2", 1536107260, bad_karma });

	int count = 0;
	db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
		++count;
		return true;
		});

	ASSERT_EQUAL(2, count);
}

void TestSameUser() {
	Database db;
	db.Put({ "id1", "Don't sell", "master", 1536107260, 1000 });
	db.Put({ "id2", "Rethink life", "master", 1536107260, 2000 });

	int count = 0;
	db.AllByUser("master", [&count](const Record&) {
		++count;
		return true;
		});

	ASSERT_EQUAL(2, count);
}

void TestReplacement() {
	const string final_body = "Feeling sad";

	Database db;
	db.Put({ "id", "Have a hand", "not-master", 1536107260, 10 });
	db.Erase("id");
	db.Put({ "id", final_body, "not-master", 1536107260, -10 });

	auto record = db.GetById("id");
	ASSERT(record != nullptr);
	ASSERT_EQUAL(final_body, record->title);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestRangeBoundaries);
	RUN_TEST(tr, TestSameUser);
	RUN_TEST(tr, TestReplacement);
	return 0;
}