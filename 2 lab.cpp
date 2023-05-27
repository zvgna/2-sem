#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <chrono>
#include <windows.h>

using namespace std;

//Класс, представляющий узел двусвязного списка
class Node {
public:
    int data;
    Node* prev;
    Node* next;

    Node(int value) {
        data = value;
        prev = nullptr;
        next = nullptr;
    }
};

//Класс, представляющий двусвязный список
class LinkedList {
private:
    Node* head;
    Node* tail;
    int size;

public:
    LinkedList() {
        head = nullptr;
        tail = nullptr;
        size = 0;

    }

    //костыль
    Node* getNode(int index) {
        if (index < 0 || index >= size) {
            return nullptr;
        }

        Node* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }

        return current;
    }

    //Вывод списка
    void ShowList() {
        Node* curr = head;
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
        while (curr) {
            cout << curr->data << " ";
            curr = curr->next;
        }
        cout << endl;
        SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }

    //Функция обмена элементов списка
    void swapElements(int index1, int index2) {
        if (index1 < 0 || index1 >= size || index2 < 0 || index2 >= size) {
            cout << "Некорректные индексы элементов." << endl;
            return;
        }

        Node* node1 = getNode(index1);
        Node* node2 = getNode(index2);

        int temp = node1->data;
        node1->data = node2->data;
        node2->data = temp;

        cout << "Элементы успешно обменяны." << endl;
    }

    //Метод для добавления элемента в конец списка
    void append(int value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        }
        else {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    //Метод для вставки элемента по индексу
    void insert(int value, int index) {
        if (index < 0 || index > size) {
            cout << "Некорректный индекс!" << endl;
            return;
        }

        Node* newNode = new Node(value);

        if (index == 0) {
            newNode->next = head;
            if (head != nullptr) {
                head->prev = newNode;
            }
            else {
                tail = newNode;
            }
            head = newNode;
        }
        else if (index == size) {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
        else {
            Node* current = head;
            for (int i = 0; i < index - 1; i++) {
                current = current->next;
            }
            newNode->prev = current;
            newNode->next = current->next;
            current->next->prev = newNode;
            current->next = newNode;
        }
        size++;
    }

    //Метод для удаления элемента по индексу
    void removeAt(int index) {
        if (index < 0 || index >= size) {
            cout << "Некорректный индекс!" << endl;
            return;
        }

        if (index == 0) {
            Node* temp = head;
            head = head->next;
            if (head != nullptr) {
                head->prev = nullptr;
            }
            else {
                tail = nullptr;
            }
            delete temp;
        }
        else if (index == size - 1) {
            Node* temp = tail;
            tail = tail->prev;
            tail->next = nullptr;
            delete temp;
        }
        else {
            Node* current = head;
            for (int i = 0; i < index; i++) {
                current = current->next;
            }
            current->prev->next = current->next;
            current->next->prev = current->prev;
            delete current;
        }
        size--;
    }

    //Метод для удаления элемента по значению (первого вхождения)
    void removeValue(int value) {
        Node* current = head;
        while (current != nullptr) {
            if (current->data == value) {
                if (current == head) {
                    head = head->next;
                    if (head != nullptr) {
                        head->prev = nullptr;
                    }
                    else {
                        tail = nullptr;
                    }
                }
                else if (current == tail) {
                    tail = tail->prev;
                    tail->next = nullptr;
                }
                else {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                }
                delete current;
                size--;
                return;
            }
            current = current->next;
        }
        cout << "Элемент не найден!" << endl;
    }

    //Метод для получения элемента по индексу
    int get(int index) {
        if (index < 0 || index >= size) {
            cout << "Некорректный индекс!" << endl;
            return -1; //Возвращаем значение по умолчанию или можно выбрать другое действие
        }

        Node* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }

    //Метод для получения индекса первого вхождения элемента по значению
    int getIndex(int value) {
        Node* current = head;
        int index = 0;
        while (current != nullptr) {
            if (current->data == value) {
                return index;
            }
            current = current->next;
            index++;
        }
        return -1; //Элемент не найден
    }

    //Метод для вывода содержимого списка
    void display() {
        Node* current = head;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

    //Метод для освобождения памяти, занимаемой списком
    void clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    //Метод для получения размера списка
    int getSize() {
        return size;
    }
};

//Функция для генерации случайных чисел в заданном диапазоне
int generateRandomNumber(int min, int max) {
    return min + rand() % (max - min + 1);
}

//Функция для заполнения списка случайными числами
void fillListRandomly(LinkedList& list, int count, vector<int>& arr) {
    srand(time(nullptr));
    for (int i = 0; i < count; i++) {
        int randomNumber = generateRandomNumber(0, 99);
        list.append(randomNumber);
        arr.push_back(randomNumber);
    }
}

int main() {
    LinkedList list;
    int choice;
    int count;
    int index;
    int value;
    vector<int> arr;
    chrono::high_resolution_clock::time_point startTime;
    chrono::high_resolution_clock::time_point endTime;
    chrono::duration<double> timeTaken;
    setlocale(0, "");

    do {
        cout << "Выберите действие:" << endl;
        cout << "1. Формирование двусвязного списка" << endl;
        cout << "2. Определение скорости создания двусвязного списка" << endl;
        cout << "3. Вставка, удаление, обмен и получение элемента двусвязного списка" << endl;
        cout << "4. Определение скорости вставки, удаления и получения элемента двусвязного списка" << endl;
        cout << "5. Удаление всех узлов и значений" << endl;
        cout << "0. Выход" << endl;
        cout << "Ваш выбор: ";
        cin >> choice;

        switch (choice) {
        case 1:
            //Формирование двусвязного списка
            list.clear();
            cout << "Введите количество элементов в списке: ";
            cin >> count;
            startTime = chrono::high_resolution_clock::now();
            fillListRandomly(list, count, arr);
            endTime = chrono::high_resolution_clock::now();
            timeTaken = endTime - startTime;
            cout << "Список успешно сформирован." << endl;
            list.ShowList();
            break;

        case 2:
            //Определение скорости создания двусвязного списка
            cout << "Время cоздания списка: " << timeTaken.count() << " секунд" << endl;
            break;

        case 3:
            //Вставка, удаление, обмен и получение элемента двусвязного списка
            cout << "Выберите действие:" << endl;
            cout << "1. Вставка элемента" << endl;
            cout << "2. Удаление элемента по индексу" << endl;
            cout << "3. Удаление элемента по значению" << endl;
            cout << "4. Получение элемента по индексу" << endl;
            cout << "5. Получение индекса элемента по значению" << endl;
            cout << "6. Обмен элементов" << endl;
            cout << "0. Выход" << endl;
            cout << "Выберите операцию: ";
            cin >> choice;

            switch (choice) {
            case 1:
                //Вставка элемента
                cout << "Введите значение элемента: ";
                cin >> value;
                cout << "Введите индекс для вставки: ";
                cin >> index;
                startTime = chrono::high_resolution_clock::now();
                list.insert(value, index);
                endTime = chrono::high_resolution_clock::now();
                timeTaken = endTime - startTime;
                cout << "Элемент успешно вставлен." << endl;
                list.ShowList();
                break;

            case 2:
                //Удаление элемента по индексу
                cout << "Введите индекс элемента для удаления: ";
                cin >> index;
                startTime = chrono::high_resolution_clock::now();
                list.removeAt(index);
                endTime = chrono::high_resolution_clock::now();
                timeTaken = endTime - startTime;
                cout << "Элемент успешно удален." << endl;
                list.ShowList();
                break;

            case 3:
                //Удаление элемента по значению
                cout << "Введите значение элемента для удаления: ";
                cin >> value;
                startTime = chrono::high_resolution_clock::now();
                list.removeValue(value);
                endTime = chrono::high_resolution_clock::now();
                timeTaken = endTime - startTime;
                list.ShowList();
                break;

            case 4:
                //Получение элемента по индексу
                cout << "Введите индекс элемента: ";
                cin >> index;
                startTime = chrono::high_resolution_clock::now();
                cout << "Элемент: " << list.get(index) << endl;
                endTime = chrono::high_resolution_clock::now();
                timeTaken = endTime - startTime;
                break;

            case 5:
                //Получение индекса элемента по значению
                cout << "Введите значение элемента: ";
                cin >> value;
                startTime = chrono::high_resolution_clock::now();
                index = list.getIndex(value);
                endTime = chrono::high_resolution_clock::now();
                timeTaken = endTime - startTime;
                if (index != -1) {
                    cout << "Индекс: " << index << endl;
                }
                else {
                    cout << "Элемент не найден!" << endl;
                }
                break;

            case 6:

                //Обмен элементов
                cout << "Введите индексы элементов для обмена: ";
                cin >> index;
                int index2;
                cin >> index2;
                startTime = chrono::high_resolution_clock::now();
                list.swapElements(index, index2);
                endTime = chrono::high_resolution_clock::now();
                timeTaken = endTime - startTime;
                list.ShowList();
                break;

              

            case 0:
                //Вернуться в главное меню
                break;

            default:
                cout << "Некорректный выбор!" << endl;
                break;
            }
            break;

        case 4:
            //Определение скорости вставки, удаления и получения элемента двусвязного списка
            cout << "Время последнего действия: " << timeTaken.count() << " секунд" << endl;
            break;

        case 5:

            //Удаление всех узлов и значений
            startTime = chrono::high_resolution_clock::now();
            list.clear();
            endTime = chrono::high_resolution_clock::now();
            timeTaken = endTime - startTime;
            cout << "Время удаления списка: " << timeTaken.count() << " секунд" << endl;
            startTime = chrono::high_resolution_clock::now();
            arr.clear();
            endTime = chrono::high_resolution_clock::now();
            timeTaken = endTime - startTime;
            cout << "Время удаления массива: " << timeTaken.count() << " секунд" << endl;
            break;

        case 0:
            //Выход из программы
            return 0;
            break;

        default:
            cout << "Некорректный выбор!" << endl;
            break;
        }

        cout << endl;

    } while (choice != 8);

    return 0;
}