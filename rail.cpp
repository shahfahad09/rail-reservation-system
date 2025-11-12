#include <iostream>
#include <queue>
#include <string>
#include <iomanip>
#include <limits>
using namespace std;

class Train {
public:
    int train_number;
    string train_name;
    int available_seats;

    Train(int number, string name, int seats)
        : train_number(number), train_name(name), available_seats(seats) {}
};

class Booking {
public:
    int train_number;
    string passenger_name;
    int passenger_age;
    string cancellation_reason;

    Booking(int number, string name, int age)
        : train_number(number), passenger_name(name), passenger_age(age), cancellation_reason("") {}
};

class RailwayReservationSystem {
private:
    queue<Train> trains;
    queue<Booking> bookings;

public:
    void add_train(int number, string name, int seats) {
        Train new_train(number, name, seats);
        trains.push(new_train);
        cout << "Train added successfully." << endl;
    }

    void view_trains() {
        cout << setw(15) << "Train Number" << setw(30) << "Train Name" << setw(20) << "Available Seats" << endl;
        cout << "-------------------------------------------------------------" << endl;

        queue<Train> temp = trains;
        while (!temp.empty()) {
            Train train = temp.front();
            cout << setw(15) << train.train_number 
                 << setw(30) << train.train_name 
                 << setw(20) << train.available_seats << endl;
            temp.pop();
        }
    }

    void book_ticket(int train_number) {
        queue<Train> temp = trains;
        bool train_found = false;

        while (!temp.empty()) {
            Train train = temp.front();
            if (train.train_number == train_number) {
                train_found = true;
                if (train.available_seats > 0) {
                    string passenger_name;
                    int passenger_age;

                    cout << "Enter Passenger Name: ";
                    cin.ignore();
                    getline(cin, passenger_name);
                    cout << "Enter Passenger Age: ";
                    cin >> passenger_age;

                    Booking new_booking(train_number, passenger_name, passenger_age);
                    bookings.push(new_booking);

                    queue<Train> temp_trains;
                    while (!trains.empty()) {
                        Train t = trains.front();
                        if (t.train_number == train_number) {
                            t.available_seats--;
                        }
                        temp_trains.push(t);
                        trains.pop();
                    }
                    trains = temp_trains;

                    cout << "Ticket booked successfully for " << passenger_name 
                         << " (Age: " << passenger_age << ") on Train " << train.train_name << "." << endl;
                    return;
                } else {
                    cout << "Seat not available on Train " << train.train_name << "." << endl;
                    return;
                }
            }
            temp.pop();
        }

        if (!train_found) {
            cout << "Train not found. Please check the train number." << endl;
        }
    }

    void view_all_tickets() {
        cout << setw(15) << "Train Number" << setw(20) << "Passenger Name" << setw(10) << "Age" 
             << setw(15) << "Status" << setw(25) << "Cancellation Reason" << endl;
        cout << "----------------------------------------------------------------------------------------" << endl;

        queue<Booking> temp = bookings;
        while (!temp.empty()) {
            Booking booking = temp.front();
            cout << setw(15) << booking.train_number 
                 << setw(20) << booking.passenger_name 
                 << setw(10) << booking.passenger_age;

            if (booking.cancellation_reason.empty()) {
                cout << setw(15) << "Active" << setw(25) << "N/A" << endl;
            } else {
                cout << setw(15) << "Deactivated" << setw(25) << "N/A" << endl;
                cout << setw(55) << " " << setw(25) << booking.cancellation_reason << endl;
            }
            temp.pop();
        }
    }

    void cancel_ticket(string passenger_name, int passenger_age) {
        queue<Booking> temp;
        bool found = false;
        int train_number = 0;

        while (!bookings.empty()) {
            Booking booking = bookings.front();
            bookings.pop();

            if (booking.passenger_name == passenger_name && booking.passenger_age == passenger_age) {
                found = true;
                string reason;
                cout << "Enter reason for ticket cancellation: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, reason);
                booking.cancellation_reason = reason;
                train_number = booking.train_number;
                cout << "Ticket canceled successfully for " << passenger_name << ". Reason: " << booking.cancellation_reason << endl;
            }
            temp.push(booking);
        }

        bookings = temp;

        if (found && train_number != 0) {
            queue<Train> temp_trains;
            bool seat_updated = false;

            while (!trains.empty()) {
                Train train = trains.front();
                trains.pop();

                if (train.train_number == train_number && !seat_updated) {
                    train.available_seats++;
                    cout << "Seat availability updated for Train " << train.train_name << endl;
                    seat_updated = true;
                }

                temp_trains.push(train);
            }

            trains = temp_trains;
        } else if (!found) {
            cout << "No booking found for " << passenger_name << " with age " << passenger_age << "." << endl;
        }
    }
};

int main() {
    RailwayReservationSystem system;
    int choice;

    do {
        cout << "\nRailway Reservation System" << endl;
        cout << "1. Add Train" << endl;
        cout << "2. View Trains" << endl;
        cout << "3. Book Ticket" << endl;
        cout << "4. Cancel Ticket" << endl;
        cout << "5. View All Tickets" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        
        while (!(cin >> choice) || choice < 1 || choice > 6) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please enter a number between 1 and 6: ";
        }

        switch (choice) {
            case 1: {
                int number, seats;
                string name;
                cout << "Enter Train Number: ";
                cin >> number;
                cout << "Enter Train Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Available Seats: ";
                cin >> seats;
                system.add_train(number, name, seats);
                break;
            }
            case 2:
                system.view_trains();
                break;
            case 3: {
                int number;
                cout << "Enter Train Number: ";
                while (!(cin >> number)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a valid train number: ";
                }
                system.book_ticket(number);
                break;
            }
            case 4: {
                string name;
                int age;
                cout << "Enter Passenger Name to Cancel Ticket: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Passenger Age: ";
                cin >> age;
                system.cancel_ticket(name, age);
                break;
            }
            case 5:
                system.view_all_tickets();
                break;
            case 6:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 6);

    return 0;
}
