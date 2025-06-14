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
    vector<list<User>> table; //������ ������� ��� �������� ������
    size_t size; // ������ ���-�������
    int count; // ���������� ��������� � ���-�������

    const int PRIME_CONST = 31;//��� ���-�-���
    // ���-������� (�������������� �����������)
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
    

    int getIndex(string name) {// �������� ������ �� �����
        return hashFunction(name, size);
    }
    // �������������
    void rehash() {
        size_t oldSize = size;
        size *= 2; // ����������� ������ ���-������� � 2 ����
        vector<list<User>> oldTable = table;
        table.clear();//�������� ��-��� �� ������� �������
        table.resize(size);
        count = 0; // ����� �������� 
        // ��������� �������� � ����� �������
        for (size_t i = 0; i < oldSize; ++i) {
            for (const auto& user : oldTable[i]) {
                int newIndex = hashFunction(user.name, size);
                table[newIndex].push_back(user);
                count++; // ����������� ������� ��� ��������
            }
        }
    }

    // ����� ������� ��������
    void insert(const User& user) {
        if (count > size * 0.7) { // ���� �������� ������ 70%, ����������
            rehash();
        }
        int index = getIndex(user.name);
        table[index].push_back(user); // ��������� ��-� � ��������������� ������
        count++; // ����������� ������� ��-��� � ������
    }

    // ����� ������ ��������
    bool find(string name, int &id){
        int index = getIndex(name);
        for (const auto& user : table[index]) {
            if (user.name == name) {
                id = user.id; // ������� id ������������
                return true; // id ������
            }
        }
        return false; // id �� ������
    }

    // ����� �������� ��������
    bool remove(string name) {
        int index = getIndex(name);
        auto& userList = table[index];
        for (auto iter = userList.begin(); iter != userList.end(); ++iter) {
            if (iter->name == name) {
                userList.erase(iter); // ������� ������������
                count--; // ��������� �������
                return true; //���� �������
            }
        }
        return false; // ������������ �� ������ ��� ��������
    }

    // ����� ������ ���-�������
    void print(){
        for (size_t i = 0; i < size; ++i) {
            cout << "������ " << i << ": ";
            for (const auto& user : table[i]) {
                cout << "{" << user.name << ", " << user.id << "} ";
            }
            cout << endl;
        }
    }
};

int main() {
    HashTable hashTable(10); 

   
    hashTable.insert(User(1, "����"));
    hashTable.insert(User(2, "���"));
    hashTable.insert(User(12, "����")); // �������� � id=2
    hashTable.insert(User(3, "���"));

   
    cout << "��� �������:" << endl;
    hashTable.print();

    int id = 0;
    if (hashTable.find("���", id)) {
        cout << "������������ ������: " << id << endl;
    }
    else {
        cout << "������������ �� ������" << endl;
    }

    if (hashTable.remove("���")) {
        cout << "������������ ��� ������" << endl;
    }
    else {
        cout << "������������ �� ������" << endl;
    }
    return 0;
}