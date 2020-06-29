#include <iostream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <map>
#include <set>
#include <functional>
#include <numeric>
#include <queue>
using namespace std;

class HotelBookingManager {
public:
	HotelBookingManager() = default;

	void Book(int64_t time, const string& hotel_name, int client_id, int room_count)
	{
		auto& hotel = hotel_booking[hotel_name];
		hotel.reservation.push(Reservation{ time, client_id, room_count });
		hotel.clients_rooms[client_id] += room_count;
		hotel.bookedRooms += room_count;

		current_time = time;
	}

	int Clients(const string& hotel_name)
	{
		auto& hotel = hotel_booking[hotel_name];
		hotel.removeResevation(current_time);

		return hotel.clients_rooms.size();
	}

	int Rooms(const string& hotel_name)
	{

		auto& hotel = hotel_booking[hotel_name];
		hotel.removeResevation(current_time);

		return hotel.bookedRooms;
	}

private:

	const int TIME_WINDOW_SIZE = 86'400;

	struct Reservation
	{
		int64_t time;
		int client_id;
		int room_count;
	};

	struct infoHotel
	{
		queue<Reservation> reservation;
		map<int, int> clients_rooms;
		int bookedRooms = 0;

		void removeResevation(int64_t time)
		{
			while (!reservation.empty() && time - reservation.front().time >= 86'400)
			{
				auto& reserv = reservation.front();
				auto& client_room = clients_rooms[reserv.client_id];
				client_room -= reserv.room_count;

				if (client_room <= 0) clients_rooms.erase(reserv.client_id);

				bookedRooms -= reserv.room_count;
				reservation.pop();

			}

		}
	};

	map<string, infoHotel> hotel_booking;
	int64_t current_time;

	
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	HotelBookingManager manager;

	int query_count;
	cin >> query_count;

	for (int query_id = 0; query_id < query_count; ++query_id) {
		string query_type;

		cin >> query_type;

		if (query_type == "BOOK") {
			int64_t time; int client_id, room_count; string hotel_name;
			cin >> time >> hotel_name >> client_id >> room_count;

			manager.Book(time, hotel_name, client_id, room_count);
		}
		else if (query_type == "CLIENTS") {
			string hotel_name;	cin >> hotel_name;

			cout << manager.Clients(hotel_name) << "\n";
		}
		else if (query_type == "ROOMS") {
			string hotel_name;	cin >> hotel_name;

			cout << manager.Rooms(hotel_name) << "\n";
		}
	}

	return 0;
}