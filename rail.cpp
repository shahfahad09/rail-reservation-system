#include <iostream>
#include <queue>
#include <string>
#include <iomanip>
#include <limits>
#include <fstream>
using namespace std;

class Train {
public:
    int train_number;
    string train_name;
    int available_seats;

    Train(int number = 0, string name = "", int seats = 0)
        : train_number(number), train_name(name), available_seats(seats) {}
};

class Booking {
public:
    int train_number;
    string passenger_name;
    int passenger_age;
    bool is_active;
    string cancellation_reason;

    Booking(int number = 0, string name = "", int age = 0, bool active = true, string reason = "")
        : train_number(number), passenger_name(name), passenger_age(age),
          is_active(active), cancellation_reason(reason) {}
};

class RailwayReservationSystem {
private:
    queue<Train> trains;
    queue<Booking> bookings;

public:
    RailwayReservationSystem() {
        loadTrainsFromCSV();
        loadBookingsFromCSV();
    }

    void saveTrainsToCSV() {
        ofstream file("trains.csv");
        file << "TrainNumber,TrainName,AvailableSeats\n";

        queue<Train> temp = trains;
        while (!temp.empty()) {
            Train t = temp.front(); temp.pop();
            file << t.train_number << "," << t.train_name << "," << t.available_seats << "\n";
        }
        file.close();
    }

    void saveBookingsToCSV() {
        ofstream file("bookings.csv");
        file << "TrainNumber,PassengerName,Age,Status,CancellationReason\n";

        queue<Booking> temp = bookings;
        while (!temp.empty()) {
            Booking b = temp.front(); temp.pop();
            file << b.train_number << "," << b.passenger_name << "," << b.passenger_age << ","
                 << (b.is_active ? "Active" : "Cancelled") << "," << b.cancellation_reason << "\n";
        }
        file.close();
    }

    void loadTrainsFromCSV() {
        ifstream file("trains.csv");
        if (!file) return;

        string line;
        getline(file, line); // header skip

        while (getline(file, line)) {
            int num, seats;
            string name;

            size_t p1 = line.find(",");
            size_t p2 = line.find(",", p1 + 1);

            num = stoi(line.substr(0, p1));
            name = line.substr(p1 + 1, p2 - (p1 + 1));
            seats = stoi(line.substr(p2 + 1));

            trains.push(Train(num, name, seats));
        }
        file.close();
    }

    void loadBookingsFromCSV() {
        ifstream file("bookings.csv");
        if (!file) return;

        string line;
        getline(file, line); // header skip

        while (getline(file, line)) {
            int num, age;
            string pname, status, reason;

            size_t p1 = line.find(",");
            size_t p2 = line.find(",", p1 + 1);
            size_t p3 = line.find(",", p2 + 1);
            size_t p4 = line.find(",", p3 + 1);

            num = stoi(line.substr(0, p1));
            pname = line.substr(p1 + 1, p2 - (p1 + 1));
            age = stoi(line.substr(p2 + 1, p3 - (p2 + 1)));
            status = line.substr(p3 + 1, p4 - (p3 + 1));
            reason = line.substr(p4 + 1);

            bool active = (status == "Active");

            bookings.push(Booking(num, pname, age, active, reason));
        }
        file.close();
    }

    void add_train(int number, string name, int seats) {
        trains.push(Train(number, name, seats));
        saveTrainsToCSV();
        cout << "Train added successfully.\n";
    }

    void view_trains() {
        cout << setw(15) << "Train Number" << setw(30) << "Train Name"
             << setw(20) << "Available Seats\n";
        cout << "----------------------------------------------------------------\n";

        queue<Train> temp = trains;
        while (!temp.empty()) {
            Train t = temp.front(); temp.pop();
            cout << setw(15) << t.train_number
                 << setw(30) << t.train_name
                 << setw(20) << t.available_seats << "\n";
        }
    }

    void book_ticket(int train_number) {
        queue<Train> temp = trains;
        bool found = false;
        Train selected;

        while (!temp.empty()) {
            Train t = temp.front(); temp.pop();
            if (t.train_number == train_number) {
                found = true;
                selected = t;
            }
        }

        if (!found) {
            cout << "Train not found.\n";
            return;
        }

        if (selected.available_seats <= 0) {
            cout << "No seats available.\n";
            return;
        }

        cin.ignore();
        string name;
        int age;
        cout << "Enter Passenger Name: ";
        getline(cin, name);
        cout << "Enter Passenger Age: ";
        cin >> age;

        bookings.push(Booking(train_number, name, age, true, ""));
        updateSeats(train_number, -1);
        saveBookingsToCSV();

        cout << "Ticket booked successfully!\n";
    }

    void updateSeats(int number, int diff) {
        queue<Train> temp, newq;

        while (!trains.empty()) {
            Train t = trains.front(); trains.pop();
            if (t.train_number == number) t.available_seats += diff;
            newq.push(t);
        }
        trains = newq;
        saveTrainsToCSV();
    }

    void cancel_ticket(string name, int age) {
        queue<Booking> temp;
        bool found = false;
        int tn = 0;

        cin.ignore();

        while (!bookings.empty()) {
            Booking b = bookings.front(); bookings.pop();
            if (b.passenger_name == name && b.passenger_age == age && b.is_active) {
                found = true;
                b.is_active = false;
                cout << "Enter cancellation reason: ";
                getline(cin, b.cancellation_reason);
                tn = b.train_number;
            }
            temp.push(b);
        }
        bookings = temp;

        if (found) {
            updateSeats(tn, +1);
            saveBookingsToCSV();
            cout << "Ticket cancelled.\n";
        } else {
            cout << "Booking not found.\n";
        }
    }

    void view_all_tickets() {
        cout << setw(15) << "Train No" << setw(20) << "Name" << setw(10) << "Age"
             << setw(15) << "Status" << setw(25) << "Reason\n";
        cout << "--------------------------------------------------------------------------------\n";

        queue<Booking> temp = bookings;
        while (!temp.empty()) {
            Booking b = temp.front(); temp.pop();
            cout << setw(15) << b.train_number
                 << setw(20) << b.passenger_name
                 << setw(10) << b.passenger_age
                 << setw(15) << (b.is_active ? "Active" : "Cancelled")
                 << setw(25) << (b.is_active ? "N/A" : b.cancellation_reason) << "\n";
        }
    }
};

int main() {
    RailwayReservationSystem system;
    int ch;

    do {
        cout << "\n--- Railway Reservation System ---\n";
        cout << "1. Add Train\n2. View Trains\n3. Book Ticket\n4. Cancel Ticket\n5. View All Tickets\n6. Exit\n";
        cout << "Enter choice: ";
        cin >> ch;

        if (ch == 1) {
            int num, seats;
            string name;
            cout << "Train Number: "; cin >> num;
            cout << "Train Name: "; cin.ignore(); getline(cin, name);
            cout << "Seats: "; cin >> seats;
            system.add_train(num, name, seats);
        }
        else if (ch == 2) system.view_trains();
        else if (ch == 3) {
            int num;
            cout << "Enter Train Number: ";
            cin >> num;
            system.book_ticket(num);
        }
        else if (ch == 4) {
            string name;
            int age;
            cout << "Passenger Name: "; cin.ignore(); getline(cin, name);
            cout << "Age: "; cin >> age;
            system.cancel_ticket(name, age);
        }
        else if (ch == 5) system.view_all_tickets();

    } while (ch != 6);

    return 0;
}
