#include "Common.h"
#include <unordered_map>
#include <mutex>
#include <list>
#include <functional>
using namespace std;

class LruCache : public ICache {
public:
	LruCache(
		shared_ptr<IBooksUnpacker> books_unpacker,
		const Settings& settings
	)
		: settingCache(settings)
		, books_ptr(books_unpacker)
	{}

	BookPtr GetBook(const string& book_name) override
	{
		lock_guard<mutex> guard(_locker);
		auto book = cacheBook.find(book_name);

		if (book == cacheBook.end())
		{
			BookPtr newBook = books_ptr->UnpackBook(book_name);
			size_t memoryBook = newBook->GetContent().size();

			/*if (memoryBook < settingCache.max_memory)
			{*/
				cacheBook[book_name] = newBook;
				listBook.push_front(cacheBook.find(book_name));
				current_memory += memoryBook;

				while (current_memory > settingCache.max_memory)
				{
					auto delete_book = listBook.back();

					current_memory -= delete_book->second->GetContent().size();
					cacheBook.erase(delete_book);
					listBook.pop_back();
				}
			//}

			return newBook;
		}

		return book->second;
	};

private:

	const Settings settingCache;
	shared_ptr<IBooksUnpacker> books_ptr;
	unordered_map<string, BookPtr> cacheBook;
	list<unordered_map<string, BookPtr>::iterator> listBook;
	size_t current_memory = 0;
	mutex _locker;
};


unique_ptr<ICache> MakeCache(
	shared_ptr<IBooksUnpacker> books_unpacker,
	const ICache::Settings& settings
)
{
	return make_unique<LruCache>(books_unpacker, settings);
}
