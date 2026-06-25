#include <iostream>
#include <string>
#include <clocale>

const unsigned int MIN_F = 128u;
const unsigned int MAX_F = 46847u;
const unsigned long long MAX_UINT = 4294967295ULL;

const unsigned int RAIN_MASK = 1u << 6;
const unsigned int SNOW_MASK = 1u << 5;
const unsigned int THUNDER_MASK = 1u << 4;
const unsigned int FOG_MASK = 1u << 3;
const unsigned int CLOUD_MASK = 1u << 2;
const unsigned int WIND_MASK = 1u << 1;
const unsigned int PRESSURE_MASK = 1u;

void trim(std::string& s);
bool valid_digits(const std::string& s);
bool number(const std::string& s, unsigned int& value);
void get_f(unsigned int& F);
void print_date(int value);
void print_bin(unsigned int value);

int main()
{
    setlocale(LC_ALL, "");

    unsigned int F = 0;
    get_f(F);

    std::cout << "\nF в 16-битном двоичном виде: ";
   
    print_bin(F);
    std::cout << '\n';

    int months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int inp_day = static_cast<int>(F >> 7);

    int day = inp_day;
    int month = 1;

    while (day > months[month - 1]) {
        day -= months[month - 1];
        month++;
    }

    bool rain = F & RAIN_MASK;
    bool snow = F & SNOW_MASK;
    bool thunder = F & THUNDER_MASK;
    bool fog = F & FOG_MASK;
    bool cloudy = F & CLOUD_MASK;
    bool wind = F & WIND_MASK;
    bool pressure = !(F & PRESSURE_MASK);

    bool weather1 = rain && wind && pressure;
    bool weather2 = !cloudy && !wind;

    std::cout << "\nДата в формате DDMM: ";
    print_date(day);
    print_date(month);
    std::cout << '\n';

    std::cout << "\nАтрибуты погоды:\n";
    std::cout << "Дождь: " << (rain ? "да" : "нет") << '\n';
    std::cout << "Снег: " << (snow ? "да" : "нет") << '\n';
    std::cout << "Гроза: " << (thunder ? "да" : "нет") << '\n';
    std::cout << "Туман: " << (fog ? "да" : "нет") << '\n';
    std::cout << "Облачность: " << (cloudy ? "да" : "нет") << '\n';
    std::cout << "Сильный ветер: " << (wind ? "да" : "нет") << '\n';
    std::cout << "Давление: " << (pressure ? "низкое" : "высокое") << '\n';

    std::cout << "\nВ заданную дату ";

    if (weather1) {
        std::cout << "был дождь при сильном ветре и низком давлении.\n";
    }
    else if (weather2) {
        std::cout << "была солнечная погода при отсутствии ветра.\n";
    }
    else {
        std::cout << "не было ни дождя при сильном ветре и низком давлении, ни солнечной погоды при отсутствии ветра.\n";
    }

    return 0;
}

void trim(std::string& s)
{
    while (!s.empty() && (s[0] == ' ' || s[0] == '\t' || s[0] == '\n' || s[0] == '\r')) {
        s.erase(0, 1);
    }

    while (!s.empty() && (s[s.size() - 1] == ' ' || s[s.size() - 1] == '\t' || s[s.size() - 1] == '\n' || s[s.size() - 1] == '\r')) {
        s.erase(s.size() - 1, 1);
    }
}

bool valid_digits(const std::string& s)
{
    if (s.empty()) return false;

    for (int i = 0; i < static_cast<int>(s.size()); i++) {
        if (s[i] < '0' || s[i] > '9') return false;
    }

    return true;
}

bool number(const std::string& s, unsigned int& value)
{
    if (!valid_digits(s)) return false;

    value = 0;

    for (int i = 0; i < static_cast<int>(s.size()); i++) {
        value = value * 10 + (s[i] - '0');
        if (value > MAX_F) return false;
    }

    return true;
}

void get_f(unsigned int& F)
{
    std::string inp;
    bool valid = false;

    std::cout << "Ограничения для F:\n";
    std::cout << "В десятичном виде: " << MIN_F << " <= F <= " << MAX_F << '\n';

    std::cout << "В двоичном виде: ";
    print_bin(MIN_F);
    std::cout << " <= F <= ";
    print_bin(MAX_F);
    std::cout << "\n\n";

    do {
        std::cout << "Введите десятичное беззнаковое целое число F: ";
        std::getline(std::cin, inp);
        trim(inp);

        valid = true;

        if (!number(inp, F)) {
            std::cout << "Введите корректное беззнаковое целое число!\n\n";
            valid = false;
        }
        else if (F < MIN_F || F > MAX_F) {
            std::cout << "Число F должно быть в диапазоне от " << MIN_F << " до " << MAX_F << "!\n\n";
            valid = false;
        }
    } while (!valid);
}

void print_date(int value)
{
    if (value < 10) std::cout << '0';
    std::cout << value;
}

void print_bin(unsigned int value)
{
    for (int i = 15; i >= 0; i--) {
        std::cout << ((value & (1u << i)) != 0 ? '1' : '0');
    }
}