#include<iostream>
#include<string>
#include<list>
#include<vector>

using namespace std;

struct User
{
    int id;
    string name;

    User(int id, const string& name) : id(id), name(name) {}
};
class HashTable {
private:
    vector<list<User>> table; //Вектор списков для хранения данных
    size_t size; // Размер хэш-таблицы
    int count; // Количество элементов в хэш-таблице

    const int PRIME_CONST = 31;//для хэш-ф-ции
    // Хэш-функция (полиномиальное хэширование)
    int hashFunction(const string& key, size_t table_size) const {
        long long hashCode = 0;
        long long p_pow = 1;
        for (char c : key) {
            hashCode = (hashCode + c * p_pow) % table_size;
            p_pow = (p_pow * PRIME_CONST) % table_size;
        }
        if (hashCode < 0) hashCode = -hashCode; 
        return hashCode % table_size;
    }

public:
    HashTable(size_t size = 10) : size(size), count(0) {
        table.resize(size);
    }
    

    int getIndex(string name) {// получить индекс по ключу
        return hashFunction(name, size);
    }
    // Рехэширование
    void rehash() {
        size_t oldSize = size;
        size *= 2; // увеличиваем размер хэш-таблицы в 2 раза
        vector<list<User>> oldTable = table;
        table.clear();//удаление эл-тов из старого вектора
        table.resize(size);
        count = 0; // сброс счетчика 
        // переносим элементы в новую таблицу
        for (size_t i = 0; i < oldSize; ++i) {
            for (const auto& user : oldTable[i]) {
                int newIndex = hashFunction(user.name, size);
                table[newIndex].push_back(user);
                count++; // увеличиваем счетчик при переносе
            }
        }
    }

    // метод вставки элемента
    void insert(const User& user) {
        if (count > size * 0.7) { // Если загрузка больше 70%, рехэшируем
            rehash();
        }
        int index = getIndex(user.name);
        table[index].push_back(user); // добавляем эл-т в соответствующий список
        count++; // увеличиваем счетчик эл-тов в списке
    }

    // метод поиска элемента
    bool find(string name, int &id){
        int index = getIndex(name);
        for (const auto& user : table[index]) {
            if (user.name == name) {
                id = user.id; // находим id пользователя
                return true; // id найден
            }
        }
        return false; // id не найден
    }

    // метод удаления элемента
    bool remove(string name) {
        int index = getIndex(name);
        auto& userList = table[index];
        for (auto iter = userList.begin(); iter != userList.end(); ++iter) {
            if (iter->name == name) {
                userList.erase(iter); // удаляем пользователя
                count--; // уменьшаем счетчик
                return true; //если удалено
            }
        }
        return false; // пользователь не найден для удаления
    }

    // метод вывода хэш-таблицы
    void print(){
        for (size_t i = 0; i < size; ++i) {
            cout << "индекс " << i << ": ";
            for (const auto& user : table[i]) {
                cout << "{" << user.name << ", " << user.id << "} ";
            }
            cout << endl;
        }
    }
};

int main() {
    HashTable hashTable(10); 

   
    hashTable.insert(User(1, "Женя"));
    hashTable.insert(User(2, "Аня"));
    hashTable.insert(User(12, "Саша")); // коллизия с id=2
    hashTable.insert(User(3, "Ира"));

   
    cout << "хэш таблица:" << endl;
    hashTable.print();

    int id = 0;
    if (hashTable.find("Аня", id)) {
        cout << "пользователь найден: " << id << endl;
    }
    else {
        cout << "пользователь не найден" << endl;
    }

    if (hashTable.remove("Аня")) {
        cout << "пользователь Аня удален" << endl;
    }
    else {
        cout << "пользователь не найден" << endl;
    }
    return 0;
}