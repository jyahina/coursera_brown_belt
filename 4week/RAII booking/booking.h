#pragma once

namespace RAII
{
	template<typename Provider>
	class Booking
	{
	public:
		Booking(Provider* p, int)
			: provider(p)
		{};

		Booking() = delete;
		Booking(const Booking& other) = delete;
		Booking& operator=(const Booking& other) = delete;
		Booking(Booking&& other)
		{
			deleter();
			provider = other.provider;
			other.provider = nullptr;
		};
		
		Booking& operator=(Booking&& other)
		{
			deleter();
			provider = other.provider;
			other.provider = nullptr;

			return *this;
		};

		~Booking()
		{
			deleter();
		};

	private:
		Provider* provider = nullptr;

		void deleter()
		{
			if (provider)
			{
				if (provider->counter > 0)
					provider->CancelOrComplete(*this);

				provider = nullptr;
			}
		};
	};
}