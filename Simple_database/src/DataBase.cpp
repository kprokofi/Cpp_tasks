//simple database in C++

#include <string>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <exception>
#include <iomanip>

using namespace std;

struct Day {
    int value;
    explicit Day(int new_value) {
        if (new_value > 31 || new_value < 0) {
            throw invalid_argument("");
        }
        value = new_value;
    }
};

struct Month {
    int value;
    explicit Month(int new_value) {
        if (new_value > 12 || new_value < 0) {
            throw invalid_argument("");
        }
        value = new_value;
    }
};

struct Year {
    int value;
    explicit Year(int new_value) {
        value = new_value;
    }
};

class Date {
public:
    int GetYear() const {
        return _year;
    }
    int GetMonth() const {
        return _month;
    }
    int GetDay() const {
        return _day;
    }

    Date() {
        _year = 1;
        _month = 1;
        _day = 1;
    }

    Date(Year new_year, Month new_month, Day new_day) {
        _year = new_year.value;
        _month = new_month.value;
        _day = new_day.value;
    }
private:
    int _day, _month, _year;
};

bool operator<(const Date& lhs, const Date& rhs) {
    if (lhs.GetYear() < rhs.GetYear())
        return true;
    else if (lhs.GetYear() > rhs.GetYear()) {
        return false;
    }
    else {
        if (lhs.GetMonth() < rhs.GetMonth())
            return true;
        else if (lhs.GetYear() > rhs.GetYear()) {
            return false;
        }
        else {
            if (lhs.GetDay() < rhs.GetDay())
                return true;
            else {
                return false;
            }
        }
    }
}

istream& operator>>(istream& stream, Date& date) {
    int year = 1, month = 1, day = 1;
    char del1, del2;
    if (stream) {
        stream >> year >> del1 >> month >> del2 >> day;
    }
    if (del1 != '-' || del2 != '-') {
        throw invalid_argument("");
    }

    date = Date(Year(year), Month(month), Day(day));
    return stream;
}

class Database {
public:
    void AddEvent(const Date& date, const string& event) {
        source[date].insert(event);
    }
    bool DeleteEvent(const Date& date, const string& event) {
        if (source[date].count(event)) {
            source[date].erase(event);
            return true;
        }
        return false;
    }

    int  DeleteDate(const Date& date) {
        int N = source[date].size();
        source.erase(date);
        return N;
    }

    set<string> Find(const Date& date) const {
        return source.at(date);
    }

    void Print() const {
        for (auto s : source) {
            cout << setw(4) << setfill('0') << s.first.GetYear() << '-' << setw(2) << setfill('0') << s.first.GetMonth() << '-' << setw(2) << setfill('0') << s.first.GetDay() << ' ';
            for (auto v : s.second) {
                cout << v << ' ';
            }
            cout << endl;
        }
    }

private:
    map <Date, set<string>> source;
};

void Parse_query(string& cmd, string& date, string& event, Database& db) {
    if (cmd == "") {
        return;
    }
    else if (cmd == "Add") {
        stringstream sstr(date);
        Date new_date;
        sstr >> new_date;
        db.AddEvent(new_date, event);
    }
    else if (cmd == "Del") {
        stringstream sstr(date);
        int N;
        bool sucess;
        Date new_date;
        sstr >> new_date;
        if (event.size() > 0) {
            sucess = db.DeleteEvent(new_date, event);
            if (sucess)
                cout << "Deleted successfully" << endl;
            else
                cout << "Event not found" << endl;
        }
        else {
            N = db.DeleteDate(new_date);
            cout << "Deleted " << N << " events" << endl;
        }
    }
    else if (cmd == "Find") {
        stringstream sstr(date);
        set<string> found;
        Date new_date;
        sstr >> new_date;
        found = db.Find(new_date);
        for (auto e : found) {
            cout << e << endl;
        }
    }
    else if (cmd == "Print") {
        db.Print();
    }
    else {
        throw invalid_argument("Unknown command: " + cmd);
    }
}

int main() {
    Database db;
    string command, cmd, date, event;
    while (getline(cin, command)) {
        stringstream command_string(command);
        if (command_string.good()) {
            command_string >> cmd;
            if (command_string.good()) {
                command_string >> date;
                if (command_string.good()) {
                    command_string >> event;
                }
                else {
                    event = "";
                }
            }
        }
        // Считайте команды с потока ввода и обработайте каждую
        try {
            Parse_query(cmd, date, event, db);
        }
        catch (exception) {
            return 0;
        }
    }


    return 0;
}
