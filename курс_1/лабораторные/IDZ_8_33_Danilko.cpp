#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <clocale>
#include <cwchar>
#include <cwctype>
#include <cstdlib>

const int LIMIT = 150;

struct Date {
    int day;
    int month;
    int year;
};

struct Magazine {
    std::string title;
    int number;
    Date taken;
    Date returned;
    std::string reader;
};

bool valid_digits(const std::string& s);
bool valid_num(const std::string& s, int& value);
bool leap(int year);
int month_days(int month, int year);
void trim(std::string& s);
std::string lowercase(const std::string& s);
bool valid_date(const std::string& s, Date& date, bool allow_zero_date);
bool same_date(const Date& a, const Date& b);
std::string date_string(const Date& date);
void print_record(const Magazine& record);
bool valid_line(const std::string& line, Magazine& record);
bool read_file(const std::string& file_name, Magazine records[], int& count, int& invalid, std::string& invalid_lines, bool& file_empty);
void print_menu();
bool get_date(Date& date);
bool get_reader(std::string& reader);
void find_by_date(const Magazine records[], int total);
void find_not_returned(const Magazine records[], int total);
void find_by_reader(const Magazine records[], int total);

int main()
{
    setlocale(LC_ALL, "");

    Magazine records[LIMIT];
    int total = 0;
    int invalid = 0;
    std::string invalid_lines = "";
    bool file_empty = false;

    if (!read_file("in_Danilko.txt", records, total, invalid, invalid_lines, file_empty)) {
        std::cout << "Не удалось открыть файл 'in_Danilko.txt'!\n";
        return 1;
    }

    if (file_empty) {
        std::cout << "Файл 'in_Danilko.txt' пуст.\n";
        return 0;
    }

    std::cout << "---------- ЗАГРУЗКА БАЗЫ ДАННЫХ ----------\n";
    std::cout << "Файл: 'in_Danilko.txt'\n";
    std::cout << "Корректных записей: " << total << '\n';
    std::cout << "Некорректных строк: " << invalid << '\n';

    if (invalid > 0) std::cout << "Номера некорректных строк: " << invalid_lines << '\n';

    std::string choice;
    std::set<char> options = {'1', '2', '3', '4'};

    bool active = true;

    while (active) {

        print_menu();

        do {
            std::cout << "Выберите действие (1-4): ";
            std::getline(std::cin, choice);
            trim(choice);
            if (choice.size() != 1 || options.count(choice[0]) == 0) std::cout << "Введите число от 1 до 4!\n\n";
        } while (choice.size() != 1 || options.count(choice[0]) == 0);

        switch (choice[0]) {
            case '1': find_by_date(records, total); break;
            case '2': find_not_returned(records, total); break;
            case '3': find_by_reader(records, total); break;
            case '4': active = false; break;
        }
    }

    return 0;
}

bool valid_digits(const std::string& s)
{
    if (s.empty()) return false;

    std::set<char> digits = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    for (int i = 0; i < static_cast<int>(s.size()); i++) {
        if (digits.count(s[i]) == 0) return false;
    }

    return true;
}

bool valid_num(const std::string& s, int& value)
{
    if (!valid_digits(s)) return false;

    value = 0;

    for (int i = 0; i < static_cast<int>(s.size()); i++) value = value * 10 + (s[i] - '0');

    return value > 0;
}

bool leap(int year)
{
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

int month_days(int month, int year)
{
    switch (month) {
        case 1: return 31;
        case 2: if (leap(year)) return 29; else return 28;
        case 3: return 31;
        case 4: return 30;
        case 5: return 31;
        case 6: return 30;
        case 7: return 31;
        case 8: return 31;
        case 9: return 30;
        case 10: return 31;
        case 11: return 30;
        case 12: return 31;
        default: return 0;
    }
}

void trim(std::string& s)
{
    while (!s.empty() && (s[0] == ' ' || s[0] == '\t' || s[0] == '\n' || s[0] == '\r')) s.erase(0, 1);
    while (!s.empty() && (s[s.size() - 1] == ' ' || s[s.size() - 1] == '\t' || s[s.size() - 1] == '\n' || s[s.size() - 1] == '\r')) s.erase(s.size() - 1, 1);
}

std::string lowercase(const std::string& s)
{
    std::string res = "";

    for (int i = 0; i < static_cast<int>(s.size()); i++) {
        unsigned char c = static_cast<unsigned char>(s[i]);

        if (c >= 'A' && c <= 'Z') res += static_cast<char>(c - 'A' + 'a');

        else if (c == 0xD0 && i + 1 < static_cast<int>(s.size())) {
            unsigned char next = static_cast<unsigned char>(s[i + 1]);

            if (next == 0x81) {
                res += static_cast<char>(0xD1);
                res += static_cast<char>(0x91);
                i++;
            }
            else if (next >= 0x90 && next <= 0x9F) {
                res += static_cast<char>(0xD0);
                res += static_cast<char>(next + 0x20);
                i++;
            }
            else if (next >= 0xA0 && next <= 0xAF) {
                res += static_cast<char>(0xD1);
                res += static_cast<char>(next - 0x20);
                i++;
            }
            else res += s[i];
        }

        else res += s[i];
    }

    return res;
}

bool valid_date(const std::string& s, Date& date, bool allow_zero_date)
{
    std::set<char> chars = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.'};

    if (s.size() != 10) return false;

    for (int i = 0; i < 10; i++) {
        if (chars.count(s[i]) == 0) return false;
    }

    if (s[2] != '.' || s[5] != '.') return false;

    if (allow_zero_date && s == "00.00.0000") {
        date.day = 0;
        date.month = 0;
        date.year = 0;
        return true;
    }

    std::string d = s.substr(0, 2);
    std::string m = s.substr(3, 2);
    std::string y = s.substr(6, 4);

    if (!valid_digits(d) || !valid_digits(m) || !valid_digits(y)) return false;

    date.day   = (d[0] - '0') * 10 + (d[1] - '0');
    date.month = (m[0] - '0') * 10 + (m[1] - '0');
    date.year  = (y[0] - '0') * 1000 + (y[1] - '0') * 100 + (y[2] - '0') * 10 + (y[3] - '0');

    if (date.year < 1 || date.month < 1 || date.month > 12) return false;
    if (date.day < 1 || date.day > month_days(date.month, date.year)) return false;

    return true;
}

bool same_date(const Date& a, const Date& b)
{
    return (a.day == b.day) && (a.month == b.month) && (a.year == b.year);
}

std::string date_string(const Date& date)
{
    if (date.day == 0 && date.month == 0 && date.year == 0) return "00.00.0000";

    std::string res = "";

    if (date.day < 10) res += '0';
    res += std::to_string(date.day);
    res += '.';

    if (date.month < 10) res += '0';
    res += std::to_string(date.month);
    res += '.';

    if (date.year < 1000) {
        if (date.year < 100) {
            if (date.year < 10) res += "000";
            else res += "00";
        }
        else res += '0';
    }

    res += std::to_string(date.year);

    return res;
}

void print_record(const Magazine& record)
{
    std::cout << "- " << record.title;
    std::cout << " #" << record.number;
    std::cout << " (выдан " << date_string(record.taken) << ", ";
    if (date_string(record.returned) != "00.00.0000") std::cout << "возвращен " << date_string(record.returned) << "). ";
    else std::cout << "не возвращен). ";
    std::cout << "Читатель: " << record.reader << '\n';
}

bool valid_line(const std::string& line, Magazine& record)
{
    std::string fields[5];
    std::string field = "";

    int count = 0;

    for (int i = 0; i < static_cast<int>(line.size()); i++) {
        if (line[i] == ';') {
            if (count >= 5) return false;
            fields[count] = field;
            field = "";
            count++;
        }
        else field += line[i];
    }

    if (count != 4) return false;

    fields[4] = field;

    for (int i = 0; i < 5; i++) {
        trim(fields[i]);
        if (fields[i].empty()) return false;
    }

    if (
        (!valid_num(fields[1], record.number)) ||
        (!valid_date(fields[2], record.taken, false)) ||
        (!valid_date(fields[3], record.returned, true))
    ) return false;

    record.title = fields[0];
    record.reader = fields[4];

    return true;
}

bool read_file(const std::string& path, Magazine records[], int& total, int& invalid, std::string& invalid_lines, bool& file_empty)
{
    std::ifstream fin(path);

    if (!fin.is_open()) return false;

    total = 0;
    invalid = 0;
    invalid_lines = "";
    file_empty = true;

    std::string line;
    int line_number = 0;

    while (std::getline(fin, line)) {
        line_number++;
        trim(line);

        if (line.empty()) continue;

        file_empty = false;

        Magazine rec;
        if (valid_line(line, rec)) {
            if (total < LIMIT) {
                records[total] = rec;
                total++;
            }
        }
        else {
            invalid++;
            if (!invalid_lines.empty()) invalid_lines += ", ";
            invalid_lines += std::to_string(line_number);
        }
    }

    fin.close();
    return true;
}

void print_menu()
{
    std::cout << "\n\n-------- ДЕЙСТВИЯ С БАЗОЙ ДАННЫХ: --------\n";
    std::cout << "1. Найти журналы, выданные в заданную дату\n";
    std::cout << "2. Найти журналы, которые еще не возвращены\n";
    std::cout << "3. Найти номера и названия журналов по фамилии читателя\n";
    std::cout << "4. Выход\n\n";
}

bool get_date(Date& date)
{
    std::string inp;
    bool valid = false;

    do {
        std::cout << "\nВведите дату в формате DD.MM.YYYY (введите 0 для возврата в меню): ";
        std::getline(std::cin, inp);
        trim(inp);

        if (inp == "0") return false;

        valid = true;

        if (!valid_date(inp, date, false)) {
            std::cout << "Дата введена некорректно!\n";
            valid = false;
        }
    } while (!valid);

    return true;
}

bool get_reader(std::string& reader)
{
    bool valid = false;

    do {
        std::cout << "\nВведите фамилию читателя (введите 0 для возврата в меню): ";
        std::getline(std::cin, reader);
        trim(reader);

        if (reader == "0") return false;

        valid = true;

        if (reader.empty()) {
            std::cout << "Фамилия не должна быть пустой!\n";
            valid = false;
        }
        else if (reader.find(';') != std::string::npos) {
            std::cout << "Фамилия не должна содержать символ ';'!\n";
            valid = false;
        }
    } while (!valid);

    return true;
}

void find_by_date(const Magazine records[], int total)
{
    bool active = true;

    while (active) {
        Date inp_date;

        if (!get_date(inp_date)) active = false;

        else {
            bool found = false;

            for (int i = 0; i < total; i++) {
                if (same_date(records[i].taken, inp_date)) {
                    if (!found) std::cout << "\nЖурналы, выданные " << date_string(inp_date) << ":\n";
                    print_record(records[i]);
                    found = true;
                }
            }

            if (!found) std::cout << "\nСовпадений не найдено.\n";
        }
    }
}

void find_not_returned(const Magazine records[], int total)
{
    bool found = false;
    Date not_returned = {0, 0, 0};

    for (int i = 0; i < total; i++) {
        if (same_date(records[i].returned, not_returned)) {
            if (!found) std::cout << "\nЖурналы, которые еще не возвращены:\n";
            print_record(records[i]);
            found = true;
        }
    }

    if (!found) std::cout << "\nТаких журналов нет.\n";
}

void find_by_reader(const Magazine records[], int total)
{
    bool active = true;

    while (active) {
        std::string reader;

        if (!get_reader(reader)) active = false;

        else {
            std::string mags[LIMIT];

            bool found = false;
            int shown = 0;

            for (int i = 0; i < total; i++) {
                if (lowercase(records[i].reader) == lowercase(reader)) {
                    std::string mag = "- " + records[i].title + " #" + std::to_string(records[i].number);
                    bool repeated = false;

                    for (int j = 0; j < shown; j++) {
                        if (mags[j] == mag) {
                            repeated = true;
                            break;
                        }
                    }

                    if (!repeated) {
                        if (shown == 0) std::cout << "\nЖурналы, которые брал читатель " << records[i].reader << ":\n";
                        std::cout << mag << '\n';
                        mags[shown] = mag;
                        shown++;
                        found = true;
                    }
                }
            }

            if (!found) std::cout << "\nСовпадений не найдено.\n";
        }
    }
}