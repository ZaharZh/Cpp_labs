#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

class Computer {
public:
    string processor;
    int vram;
    int vdisk;
    string video_adapter;
    int cost;
    int purchase_year;

    void input() {
        cout << "Процессор     : ";
        getline(cin, processor);
        cout << "Объем оперативной памяти, GB  : ";
        cin >> vram;
        cin.ignore();
        cout << "Объем жесткого диска, GB : ";
        cin >> vdisk;
        cin.ignore();
        cout << "Видеоадаптер    : ";
        getline(cin, video_adapter);
        cout << "Цена          : ";
        cin >> cost;
        cin.ignore();
        cout << "Год закупки : ";
        cin >> purchase_year;
        cin.ignore();
    }

    void print() const {
        cout << "Процессор     : " << processor << endl;
        cout << "Объем оперативной памяти, GB  : " << vram << endl;
        cout << "Объем жесткого диска, GB : " << vdisk << endl;
        cout << "Видеоадаптер    : " << video_adapter << endl;
        cout << "Цена          : " << cost << endl;
        cout << "Год закупки   : " << purchase_year << endl;
    }

    void fromStr(const string& str) {
        istringstream iss(str);
        getline(iss, processor, '#');
        iss >> vram;
        iss.ignore();
        iss >> vdisk;
        iss.ignore();
        getline(iss, video_adapter, '#');
        iss >> cost;
        iss.ignore();
        iss >> purchase_year;
    }

    string toStr() const {
        ostringstream oss;
        oss << processor << "#" << vram << "#" << vdisk << "#" << video_adapter << "#" << cost << "#" << purchase_year;
        return oss.str();
    }
};

class ComputerList {
private:
    int alloc;
    int size;
    Computer** comps;

    void resize() {
        alloc = alloc ? alloc * 2 : 1;
        Computer** olds = comps;
        comps = new Computer*[alloc];
        for (int i = 0; i < size; ++i) {
            comps[i] = olds[i];
        }
        delete[] olds;
    }

public:
    ComputerList() : alloc(0), size(0), comps(nullptr) {}

    ~ComputerList() {
        for (int i = 0; i < size; ++i) {
            delete comps[i];
        }
        delete[] comps;
    }

    void add(const Computer& comp) {
        if (size == alloc) {
            resize();
        }
        comps[size++] = new Computer(comp);
    }

    void remove(int pos) {
        if (pos >= 0 && pos < size) {
            delete comps[pos];
            for (int i = pos; i + 1 < size; ++i) {
                comps[i] = comps[i + 1];
            }
            --size;
        }
    }

    int getSize() const {
        return size;
    }

    void printUpdate(int year) const {
        int pos = 0;
        for (; pos < size; ++pos) {
            if (comps[pos]->purchase_year <= year) {
                break;
            }
        }
        if (pos == size) {
            cout << "Обновления не требуются!" << endl;
            return;
        }

        int plen = 9, vlen = 13;
        for (int i = pos; i < size; ++i) {
            plen = max(plen, static_cast<int>(comps[i]->processor.length()));
            vlen = max(vlen, static_cast<int>(comps[i]->video_adapter.length()));
        }

        string stars(plen + vlen + 58, '*');

        cout << " " << stars << endl;
        cout << "| " << setw(2) << "#" << " | " << setw(plen) << "Процессор" << " | "
             << setw(9) << "ОП, GB" << " | " << setw(9) << "ЖД, GB" << " | "
             << setw(vlen) << "Видеокарта" << " | " << setw(9) << "Цена" << " | "
             << setw(9) << "Год" << " |" << endl;
        cout << " " << stars << endl;
        for (int i = pos, cnt = 0; i < size; ++i) {
            if (comps[i]->purchase_year > year) {
                continue;
            }
            ++cnt;
cout << "| " << setw(2) << cnt << " | " << setw(plen) << comps[i]->processor << " | "
                 << setw(9) << comps[i]->vram << " | " << setw(9) << comps[i]->vdisk << " | "
                 << setw(vlen) << comps[i]->video_adapter << " | " << setw(9) << comps[i]->cost << " | "
                 << setw(9) << comps[i]->purchase_year << " |" << endl;
        }
        cout << " " << stars << endl;
    }

    void print() const {
        printUpdate(2000000000);
    }

    void read(const string& fname) {
        ifstream file(fname);
        if (!file) {
            return;
        }
        string line;
        while (getline(file, line)) {
            Computer* comp = new Computer();
            comp->fromStr(line);
            add(*comp);
        }
        file.close();
    }

    void write(const string& fname) const {
        ofstream file(fname);
        for (int i = 0; i < size; ++i) {
            file << comps[i]->toStr() << endl;
        }
        file.close();
    }
};

void printCommands() {
    cout << "1) Добавить информацию" << endl;
    cout << "2) Вывести всю информацию" << endl;
    cout << "3) Удалить информацию" << endl;
    cout << "4) Проверить обновления" << endl;
    cout << "0) Выйти" << endl;
    cout << "\n>> ";
}

int main() {
    string file = "computer.txt";
    int command, number, year;
    ComputerList list;

    list.read(file);
    do {
        printCommands();
        cin >> command;
        cin.ignore();
        Computer comp;
        switch (command) {
        case 1:
            cout << "\nВведите информацию о компьютере:" << endl;
            comp.input();
            list.add(comp);
            cout << "\nИнформация обновлена!" << endl;
            break;
        case 2:
            if (list.getSize() == 0) {
                cout << "\nСписок пуст!" << endl;
                break;
            }
            cout << "\nСписок компьютеров:" << endl;
            list.print();
            cout << endl;
            break;
        case 3:
            if (list.getSize() == 0) {
                cout << "\nСписок пуст!" << endl;
                break;
            }
            cout << "\nВведите номер[1, " << list.getSize() << "] -> ";
            cin >> number;
            cin.ignore();
            if (number < 1 || number > list.getSize()) {
                cout << "\nНеправильный номер!" << endl;
                break;
            }
            list.remove(number - 1);
            cout << "\nИнформация удалена" << endl;
            break;
        case 4:
            if (list.getSize() == 0) {
                cout << "\nСписок пуст!" << endl;
                break;
            }
            cout << "\nВведите год -> ";
            cin >> year;
            cin.ignore();
            cout << "\nКомпьютеры для обновления:" << endl;
            list.printUpdate(year);
            cout << endl;
            break;
        case 0:
            cout << "\nРабота завершена!" << endl;
            break;
        default:
            cout << "\nНеверная команда!" << endl;
            break;
        }
    } while (command != 0);

    list.write(file);
    return 0;
}