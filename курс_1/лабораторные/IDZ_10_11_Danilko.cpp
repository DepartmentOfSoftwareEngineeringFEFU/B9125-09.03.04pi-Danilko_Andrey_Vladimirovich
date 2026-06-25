#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <clocale>

const int LIMIT = 1000;

struct Node {
    Node* prev;
    int data;
    Node* next;
};

bool valid_digits(const std::string& s);
bool valid_num(const std::string& s, int& value);
void trim(std::string& s);

void clear_list(Node*& head, Node*& tail);
int list_len(Node* head);
void print_list(Node* head, Node* tail);
void push_back(Node*& head, Node*& tail, int value);
bool insert_node(Node*& head, Node*& tail, int pos, int value);
bool delete_node(Node*& head, Node*& tail, int pos, int& del_val);
void create_list(Node*& head, Node*& tail, int n);
bool sort(Node*& head, Node*& tail);

void print_menu();
bool get_n(int& n);
int get_choice();
bool get_insert(Node* head, int& pos);
bool get_delete(Node* head, int& pos);

int main()
{
    setlocale(LC_ALL, "");
    std::srand(static_cast<unsigned int>(std::time(0)));

    Node* head = nullptr;
    Node* tail = nullptr;

    bool active = true;

    while (active) {
        print_menu();
        int choice = get_choice();
        switch (choice) {
            case 1:
                if (head != nullptr) std::cout << "\nУдалите текущий список, чтобы создать новый!\n";
                else {
                    int n = 0;
                    if (!get_n(n)) break;
                    create_list(head, tail, n);
                    std::cout << "\nСписок из " << n << " элементов успешно создан.\n";
                }
                break;
            case 2: print_list(head, tail); break;
            case 3:
                if (head == nullptr) std::cout << "\nСписок уже пуст!\n";
                else {
                    clear_list(head, tail);
                    std::cout << "\nСписок полностью удален.\n";
                }
                break;
            case 4: {
                int pos = 0;
                int ins_val = std::rand() % 101;
                if (!get_insert(head, pos)) break;
                if (list_len(head) >= LIMIT) {
                    std::cout << "\nДостигнут максимальный размер списка (" << LIMIT << ").\n";
                }
                else if (insert_node(head, tail, pos, ins_val)) {
                    std::cout << "\nЭлемент " << ins_val << " успешно добавлен в позицию " << pos << ".\n";
                }
                else std::cout << "\nНе удалось добавить элемент!\n";
                break;
            }
            case 5: {
                int pos = 0;
                int del_val = 0;
                if (head == nullptr) {
                    std::cout << "\nСписок пуст, удаление невозможно!\n";
                    break;
                }
                if (!get_delete(head, pos)) break;
                if (delete_node(head, tail, pos, del_val)) {
                    std::cout << "\nЭлемент " << del_val << " удален из позиции " << pos << ".\n";
                }
                else std::cout << "\nНе удалось удалить элемент!\n";
                break;
            }
            case 6:
                if (head == nullptr) std::cout << "\nСписок пуст!\n";
                else {
                    sort(head, tail);
                    std::cout << "\nПреобразование выполнено: сначала четные элементы, затем нечетные.\n";
                }
                break;
            case 7: active = false; break;
        }
    }

    clear_list(head, tail);
    return 0;
}

bool valid_digits(const std::string& s)
{
    if (s.empty()) return false;

    for (int i = 0; i < static_cast<int>(s.size()); i++) {
        if (s[i] < '0' || s[i] > '9') return false;
    }

    return true;
}

bool valid_num(const std::string& s, int& value)
{
    if (!valid_digits(s)) return false;

    value = 0;

    for (int i = 0; i < static_cast<int>(s.size()); i++) {
        value = value * 10 + (s[i] - '0');
    }

    return true;
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

void clear_list(Node*& head, Node*& tail)
{
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }

    tail = nullptr;
}

int list_len(Node* head)
{
    int count = 0;

    while (head != nullptr) {
        count++;
        head = head->next;
    }

    return count;
}

void print_list(Node* head, Node* tail)
{
    std::cout << '\n';

    if (head == nullptr) {
        std::cout << "Список пуст.\n";
        return;
    }

    if (head == tail) {
        std::cout << "От головы к хвосту: " << head->data << '\n';
        std::cout << "От хвоста к голове: " << tail->data << '\n';
        return;
    }

    std::cout << "От головы к хвосту: ";

    Node* current = head;

    while (current != nullptr) {
        std::cout << current->data;
        if (current->next != nullptr) std::cout << " => ";
        current = current->next;
    }

    std::cout << '\n';
    std::cout << "От хвоста к голове: ";

    current = tail;

    while (current != nullptr) {
        std::cout << current->data;
        if (current->prev != nullptr) std::cout << " => ";
        current = current->prev;
    }

    std::cout << '\n';
}

void push_back(Node*& head, Node*& tail, int value)
{
    Node* new_node = new Node;
    new_node->data = value;
    new_node->prev = nullptr;
    new_node->next = nullptr;

    if (head == nullptr) {
        head = new_node;
        tail = new_node;
        return;
    }

    new_node->prev = tail;
    tail->next = new_node;
    tail = new_node;
}

bool insert_node(Node*& head, Node*& tail, int pos, int value)
{
    int size = list_len(head);

    if (pos < 1 || pos > size + 1) return false;

    Node* new_node = new Node;
    new_node->data = value;
    new_node->prev = nullptr;
    new_node->next = nullptr;

    if (head == nullptr) {
        head = new_node;
        tail = new_node;
        return true;
    }

    if (pos == 1) {
        new_node->next = head;
        head->prev = new_node;
        head = new_node;
        return true;
    }

    if (pos == size + 1) {
        new_node->prev = tail;
        tail->next = new_node;
        tail = new_node;
        return true;
    }

    Node* current = head;

    for (int i = 1; i < pos; i++) {
        current = current->next;
    }

    new_node->next = current;
    new_node->prev = current->prev;
    current->prev->next = new_node;
    current->prev = new_node;

    return true;
}

bool delete_node(Node*& head, Node*& tail, int pos, int& del_val)
{
    int size = list_len(head);

    if (head == nullptr || pos < 1 || pos > size) return false;

    if (head == tail) {
        del_val = head->data;
        delete head;
        head = nullptr;
        tail = nullptr;
        return true;
    }

    if (pos == 1) {
        Node* temp = head;
        del_val = temp->data;
        head = head->next;
        head->prev = nullptr;
        delete temp;
        return true;
    }

    if (pos == size) {
        Node* temp = tail;
        del_val = temp->data;
        tail = tail->prev;
        tail->next = nullptr;
        delete temp;
        return true;
    }

    Node* current = head;

    for (int i = 1; i < pos; i++) {
        current = current->next;
    }

    del_val = current->data;
    current->prev->next = current->next;
    current->next->prev = current->prev;
    delete current;

    return true;
}

void create_list(Node*& head, Node*& tail, int n)
{
    clear_list(head, tail);

    for (int i = 0; i < n; i++) {
        push_back(head, tail, std::rand() % 101);
    }
}

bool sort(Node*& head, Node*& tail)
{
    if (head == nullptr) return false;

    Node* even_head = nullptr;
    Node* even_tail = nullptr;
    Node* odd_head = nullptr;
    Node* odd_tail = nullptr;

    Node* current = head;

    while (current != nullptr) {
        Node* next = current->next;

        current->prev = nullptr;
        current->next = nullptr;

        if (current->data % 2 == 0) {
            if (even_head == nullptr) {
                even_head = current;
                even_tail = current;
            }
            else {
                current->prev = even_tail;
                even_tail->next = current;
                even_tail = current;
            }
        }
        else {
            if (odd_head == nullptr) {
                odd_head = current;
                odd_tail = current;
            }
            else {
                current->prev = odd_tail;
                odd_tail->next = current;
                odd_tail = current;
            }
        }

        current = next;
    }

    if (even_head == nullptr) {
        head = odd_head;
        tail = odd_tail;
    }
    else if (odd_head == nullptr) {
        head = even_head;
        tail = even_tail;
    }
    else {
        even_tail->next = odd_head;
        odd_head->prev = even_tail;
        head = even_head;
        tail = odd_tail;
    }

    return true;
}

void print_menu()
{
    std::cout << "\n----- РАБОТА С ДВУСВЯЗНЫМ СПИСКОМ -----\n";
    std::cout << "1. Создать список\n";
    std::cout << "2. Вывести список на экран\n";
    std::cout << "3. Удалить весь список\n";
    std::cout << "4. Добавить элемент в любую точку списка\n";
    std::cout << "5. Удалить элемент из любой точки списка\n";
    std::cout << "6. Переставить элементы по правилу:\n";
    std::cout << "   \"Сначала четные, затем нечетные\"\n";
    std::cout << "7. Выход\n\n";
}

bool get_n(int& n)
{
    std::string inp;
    bool valid = false;

    do {
        std::cout << "\nВведите количество элементов списка n >= 2, '-' для возврата в меню: ";
        std::getline(std::cin, inp);
        trim(inp);

        if (inp == "-") return false;

        valid = true;

        if (!valid_num(inp, n)) {
            std::cout << "Введите корректное натуральное число!\n\n";
            valid = false;
        }
        else if (n < 2) {
            std::cout << "Число n должно быть не меньше 2!\n\n";
            valid = false;
        }
        else if (n > LIMIT) {
            std::cout << "Максимальный размер списка - " << LIMIT << " элементов!\n";
            valid = false;
        }
    } while (!valid);

    return true;
}

int get_choice()
{
    std::string choice;

    do {
        std::cout << "Выберите действие (1-7): ";
        std::getline(std::cin, choice);
        trim(choice);

        if (choice.size() != 1 || choice[0] < '1' || choice[0] > '7') {
            std::cout << "Введите число от 1 до 7!\n\n";
        }
    } while (choice.size() != 1 || choice[0] < '1' || choice[0] > '7');

    return choice[0] - '0';
}

bool get_insert(Node* head, int& pos)
{
    std::string inp;
    int size = list_len(head);
    bool valid = false;

    do {
        std::cout << "\nВведите позицию для вставки (1";
        if (head != nullptr) std::cout << '-' << size + 1;
        std::cout << "), '-' для возврата в меню: ";
        std::getline(std::cin, inp);
        trim(inp);
        if (inp == "-") return false;

        valid = true;
        if (!valid_num(inp, pos)) {
            std::cout << "Введите корректное натуральное число!\n";
            valid = false;
        }
        else if (pos < 1 || pos > size + 1) {
            std::cout << "Позиция должна быть ";
            if (head == nullptr) std::cout << "Элемент можно вставить только на позицию 1!\n";
            else std::cout << "в диапазоне от 1 до " << size + 1 << "!\n";
            valid = false;
        }
    } while (!valid);

    return true;
}

bool get_delete(Node* head, int& pos)
{
    std::string inp;
    int size = list_len(head);
    bool valid = false;

    do {
        std::cout << "\nВведите позицию для удаления (1";
        if (size > 1) std::cout << '-' << size;
        std::cout << "), '-' для возврата в меню: ";

        std::getline(std::cin, inp);
        trim(inp);
        if (inp == "-") return false;

        valid = true;

        if (!valid_num(inp, pos)) {
            std::cout << "Введите корректное натуральное число!\n";
            valid = false;
        }
        else if (pos < 1 || pos > size) {
            std::cout << "Позиция должна быть ";
            if (size == 1) std::cout << "равна 1!\n";
            else std::cout << "в диапазоне от 1 до " << size << "!\n";
            valid = false;
        }

    } while (!valid);

    return true;
}