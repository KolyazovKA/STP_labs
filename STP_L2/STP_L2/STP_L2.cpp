#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <conio.h>
#include <stdlib.h>
#include <cstdio>
# include <stdlib.h>

using namespace std;

/*
Надо сделать:
* конструктор копирования
* перегруженная операция присваивания ?объектов?


*/



struct CLIENT //структура данных клиента, содержащая номер дома и квартиры, ФИО
{
    int numb_house; //Номер дома
    int numb_apartment; //Номер квартиры
    string FIO; //ФИО 
};

struct Date //структура данных для даты
{
    int d; //день 
    int m; //месяц
    short y; //год
};

struct RECORD { // структура, содержащая клиента, вид платежа, дата платежа, сумма платежа
    int number_of_note;
    CLIENT citizen;
    string payment_type; //Вид платежа
    Date payment_date; //дата платежа
    int payment_sum; //Сумма платежа
};

bool operator>(Date v, Date w)
{
    if (v.y > w.y) return true;
    if ((v.y == w.y) && (v.m > w.m)) return true;
    if ((v.y == w.y) && (v.m == w.m) && (v.d > w.d)) return true;
    return false;
}


std::ostream& operator<<(std::ostream& out, RECORD& z)
{
    out << "\n|" << std::setw(5) << z.number_of_note << "|" << std::setw(5) << z.citizen.numb_house << "|"
        << std::setw(5) << z.citizen.numb_apartment << "|" << std::setw(34) << z.citizen.FIO << "|"
        << std::setw(14) << z.payment_type << "|" << std::setw(4) << z.payment_date.d << "|" << std::setw(5)
        << z.payment_date.m << "|" << std::setw(6) << z.payment_date.y << "|" << std::setw(7)
        << z.payment_sum << "|";
    return out;
}


class Com_payment {
private:
    string data_filename = "data_file.txt";
    string output_filename = "output_file.txt";
    RECORD* px;
    int number_of_lines; //Количество строк
public:
    Com_payment() :px(NULL), number_of_lines(0) {};// конструктор
    Com_payment(Com_payment& copy_c);//конструктор копирования
    ~Com_payment() { if (px != NULL) delete[]px; } //деструктор

    void read_file();
    void save_in_file();
    void output_in_file();
    void add_note();
    void fast_add();
    void delete_note();
    void output_data();

    void sort_by_name();
    void sort_by_payment_type();
    void sort_by_payment();
};


//  конструктор копирования (не проверен)
Com_payment::Com_payment(Com_payment& copy_c)
{
    int i;
    number_of_lines = copy_c.number_of_lines;
    if (number_of_lines == 0)
        px = NULL;
    else {
        px = new RECORD[number_of_lines];
        if (px == NULL) {
            cout << "нет памяти.\n";
            exit(1);
        }
        for (i = 0; i < number_of_lines; i++)
            px[i] = copy_c.px[i];
    }
}


//  чтение данных из файла
void Com_payment::read_file() {
    fstream fin;
    string dop, trash;
    RECORD c;
    fin.open(data_filename);
    if (fin.fail()) {
        cout << "Файл не открывается\n";
        cin >> trash;
        return;
    }

    number_of_lines = 0;
    if (px != NULL) {
        delete[]px;
        px = NULL;
    }

    while (1) {
        fin >> c.number_of_note >> c.citizen.numb_house
            >> c.citizen.numb_apartment >> c.citizen.FIO >> dop
            >> c.payment_type >> c.payment_date.d >> c.payment_date.m
            >> c.payment_date.y >> c.payment_sum;
        c.citizen.FIO += " " + dop;
        if (fin.fail())
            break;
        number_of_lines++;
    }
    fin.close();

    fin.open(data_filename);
    px = new RECORD[number_of_lines];

    if (px == NULL) {
        cout << "Нет памяти.\n";
        fin.close();
        cout << "Ввести фаил не удается.\n";
        _getch();
        number_of_lines = 0;
        return;
    }

    for (int i = 0; i < number_of_lines; i++) {
        fin >> px[i].number_of_note >> px[i].citizen.numb_house
            >> px[i].citizen.numb_apartment >> px[i].citizen.FIO >> dop
            >> px[i].payment_type >> px[i].payment_date.d
            >> px[i].payment_date.m >> px[i].payment_date.y
            >> px[i].payment_sum;
        px[i].citizen.FIO += " " + dop;
    }
    fin.close();

    cout << "Данные из файла введены.\n";
    _getch();
}


//  сохранение данных в файл
void Com_payment::save_in_file() {
    ofstream fout;
    fout.open(data_filename); // очистка файла
    fout.write("", 0);
    //Тестовые данные
/*
1 12 123 Колязов К.А. газ 12 1 2002 2000
2 21 321 Харисов Д.Р. кварплата 21 2 2002 20000
*/

    for (int i = 0; i < number_of_lines; i++) {
        fout << px[i].number_of_note << " " << px[i].citizen.numb_house << " "
            << px[i].citizen.numb_apartment << " " << px[i].citizen.FIO << " "
            << px[i].payment_type << " " << px[i].payment_date.d << " "
            << px[i].payment_date.m << " " << px[i].payment_date.y << " "
            << px[i].payment_sum << "\n";
    }

    cout << "Файл введен";
    _getch();
    fout.close();
}

//  вывод данных в файл
void Com_payment::output_in_file() {
    fstream fout;
    int count = 95;
    for (int i = 0; i < count; i++)
        fout << "_";
    fout << "\n|  №  | № д | №кв |          Фамилия и инициалы      |   Тип плат.  |       Дата      | Сумма |\n";
    fout << "\n|     |     |     |                                  |              |День|Месяц|  Год |       |\n";
    for (int i = 0; i < count; i++)
        fout << "_";
    for (int i = 0; i < number_of_lines; i++) {
        fout << "\n|" << setw(5) << px[i].number_of_note << "|" << setw(5) << px[i].citizen.numb_house << "|"
            << setw(5) << px[i].citizen.numb_apartment << "|" << setw(34) << px[i].citizen.FIO << "|"
            << setw(14) << px[i].payment_type << "|" << setw(4) << px[i].payment_date.d << "|" << setw(5)
            << px[i].payment_date.m << "|" << setw(6) << px[i].payment_date.y << "|" << setw(7)
            << px[i].payment_sum << "|\n";
        for (int i = 0; i < count; i++)
            fout << "_";
    }
    cout << "Вывод завершен";
    _getch();
}


//  добавление записи
void Com_payment::add_note() {
    system("chcp 1251");

    RECORD c, * p;
    p = new RECORD[number_of_lines + 1];
    string dop;

    if (p == NULL) {
        cout << "Нет памяти.\n Добавить новую запись не удается.\n";
        return;
    }
    c.number_of_note = number_of_lines + 1;
    while (1) {
        cout << "1) Номер дома: "; cin >> c.citizen.numb_house; cout << endl << endl;
        if (cin.fail()) //Проверка на правильность введеных данных
        {
            string s;
            cin.clear();
            cin >> s;
            cout << "Неправльные данные. Попробуйте еще раз.\n";
        }
        else if (c.citizen.numb_house <= 0) {
            cout << "Неправльные данные. Попробуйте еще раз.\n";
        }
        else {
            break;
        }
    }

    while (1) {
        cout << "2) Номер квартиры: "; cin >> c.citizen.numb_apartment; cout << endl << endl;
        if (cin.fail()) //Проверка на правильность введеных данных
        {
            string s;
            cin.clear();
            cin >> s;
            cout << "Неправльные данные. Попробуйте еще раз.\n";
        }
        else if (c.citizen.numb_apartment <= 0) {
            cout << "Неправльные данные. Попробуйте еще раз.\n";
        }
        else {
            break;
        }
    }

    cout << "3) Фамилия и инициалы: "; cin >> c.citizen.FIO >> dop; cout << endl << endl; c.citizen.FIO += " " + dop;
    cout << "4) Вид платежа (кварплата, газ, электричество): "; cin >> c.payment_type; cout << endl << endl;
    cout << "5) Дата";
    while (1) {
        cout << "\nа) День: "; cin >> c.payment_date.d;
        if (cin.fail()) //Проверка на правильность введеных данных
        {
            string s;
            cin.clear();
            cin >> s;
            cout << "Неправльные данные. Попробуйте еще раз.\n";
        }
        else if (c.payment_date.d >= 32 or c.payment_date.d <= 0) {
            cout << "Неправльные данные. Попробуйте еще раз.\n";
        }
        else {
            break;
        }
    }
    while (1) {
        cout << "\nб) Месяц: "; cin >> c.payment_date.m;
        if (cin.fail()) //Проверка на правильность введеных данных
        {
            string s;
            cin.clear();
            cin >> s;
            cout << "Неправльные данные. Попробуйте еще раз.\n";
        }
        else if (c.payment_date.m >= 13 or c.payment_date.m <= 0) {
            cout << "Неправльные данные. Попробуйте еще раз.\n";
        }
        else {
            break;
        }
    }
    while (1) {
        cout << "\nв) Год: "; cin >> c.payment_date.y; cout << "\n\n";
        if (cin.fail()) //Проверка на правильность введеных данных
        {
            string s;
            cin.clear();
            cin >> s;
            cout << "Неправльные данные. Попробуйте еще раз.\n";
        }
        else if (c.payment_date.y >= 2023 or c.payment_date.y <= 0) {
            cout << "Неправльные данные. Попробуйте еще раз.\n";
        }
        else {
            break;
        }
    }
    while (1) {
        cout << "6) Сумма платежа: "; cin >> c.payment_sum;
        if (cin.fail()) //Проверка на правильность введеных данных
        {
            string s;
            cin.clear();
            cin >> s;
            cout << "Неправльные данные. Попробуйте еще раз.\n";
        }
        else if (c.payment_sum <= 0) {
            cout << "Неправльные данные. Попробуйте еще раз.\n";
        }
        else {
            break;
        }
    }

    for (int i = 0; i < number_of_lines; i++)
        p[i] = px[i]; //Копирование старового массива
    p[number_of_lines] = c;
    number_of_lines++;
    if (px != NULL)//удаление старого массива
        delete[]px;
    px = p;
    cout << "Запись добавлена.\n\n";
    _getch();
}

//  быстрый ввод данных
void Com_payment::fast_add() {
    system("chcp 1251");
    int count, trash;
    string  dop;
    RECORD c, * p;


    cout << "Сколько добавляем?"; cin >> count;
    for (int i = 0; i < count; i++) {
        cout << i + 1 << ": "; c.number_of_note = number_of_lines + 1;
        cin >> trash >> c.citizen.numb_house >> c.citizen.numb_apartment >> c.citizen.FIO
            >> dop >> c.payment_type >> c.payment_date.d >> c.payment_date.m
            >> c.payment_date.y >> c.payment_sum;
        c.citizen.FIO += " " + dop;
        p = px;
        if (px == NULL)
            delete[]px;
        number_of_lines++;
        px = new RECORD[number_of_lines];
        for (int j = 0; j <= number_of_lines - 2; j++)
            px[j] = p[j];
        px[number_of_lines - 1] = c;
        delete[]p;
    }

}


//  удаление записи
void Com_payment::delete_note() {
    RECORD* c;
    int line, c_lines;
    string choice;
    Com_payment::output_data();
    c_lines = 0;

    while (1) {
        cout << "\nВведите номер строки, которую надо удалить: "; cin >> line; cout << endl;
        if (cin.fail()) //Проверка на правильность введеных данных
        {
            string s;
            cin.clear();
            cin >> s;
            cout << "Нет такой строки. Попробуйте еще раз.\n";
        }
        else if (line < 1 or line > number_of_lines) {
            cout << "Нет такой строки. Попробуйте еще раз.\n";
        }
        else
            break;
    }
    line--;
    cout << px[line].number_of_note << " " << px[line].citizen.numb_house << " "
        << px[line].citizen.numb_apartment << " " << px[line].citizen.FIO << " "
        << px[line].payment_type << " " << px[line].payment_date.d << " "
        << px[line].payment_date.m << " " << px[line].payment_date.y << " "
        << px[line].payment_sum << "\n";
    cout << "Эту строку вы хотите удалить (y/n): "; cin >> choice;

    if (choice == "y" or choice == "Y")
    {
        c = new RECORD[number_of_lines - 1];
        for (int i = 0; i < number_of_lines; i++) {
            if (i != line) {
                c[c_lines] = px[i];
                c[c_lines].number_of_note = c_lines + 1;
                c_lines++;
            }
            else
                continue;
        }

        number_of_lines--;
        if (px != NULL)
            delete[]px;
        px = new RECORD[number_of_lines];
        px = c;
        cout << "Удаление завершено.";
        _getch();
    }
    else {
        cout << "Процесс удаления прерван.";
        _getch();
        return;
    }
}


//  вывод на экран
void Com_payment::output_data() {
    cout << "\n";
    //6 + 6 + 6 + 35 + 13 + 5 + 6 + 7 + 8 = 91
    int count = 95;
    for (int i = 0; i < count; i++)
        cout << "_";
    cout << "\n|  №  | № д | №кв |          Фамилия и инициалы      |   Тип плат.  |       Дата      | Сумма |\n";
    cout << "\n|     |     |     |                                  |              |День|Месяц|  Год |       |\n";
    for (int i = 0; i < count; i++)
        cout << "_";
    for (int i = 0; i < number_of_lines; i++) {
        cout << px[i];
    }
    cout << endl;
    for (int i = 0; i < count; i++)
        cout << "_";

    _getch();
}






// сортировка по ФИО
void Com_payment::sort_by_name() {
    RECORD trash;
    for (int i = 0; i < number_of_lines; i++)
        for (int j = i + 1; j < number_of_lines; j++)
            if (px[i].citizen.FIO > px[j].citizen.FIO) {
                trash = px[i];
                px[i] = px[j];
                px[j] = trash;
            }
    cout << "Сортировка завершена.";
    _getch();
}

//  сортировка по сумме платежа
void Com_payment::sort_by_payment() {
    RECORD trash;
    for (int i = 0; i < number_of_lines; i++)
        for (int j = i + 1; j < number_of_lines; j++)
            if (px[i].payment_sum > px[j].payment_sum) {
                trash = px[i];
                px[i] = px[j];
                px[j] = trash;
            }
    cout << "Сортировка завершена.";
    _getch();
}

//сортировка по типу и по дате
void Com_payment::sort_by_payment_type() {
    RECORD trash;
    for (int i = 0; i < number_of_lines; i++)
        for (int j = i + 1; j < number_of_lines; j++)
            if (px[i].payment_type > px[j].payment_type or (px[i].payment_type == px[j].payment_type and px[i].payment_date > px[j].payment_date)) {
                trash = px[i];
                px[i] = px[j];
                px[j] = trash;
            }
    cout << "Сортировка завершена.";
    _getch();
}


int main()
{
    setlocale(LC_ALL, "");

    Com_payment rec;
    int x = 0;
    int choice;
    string trash;
    Com_payment com;
    bool flag = true;

    while (flag) {
        cout << "_________________________Menu____________________________\n";
        cout << "\t\t1. Загрузить данные из файла\n";
        cout << "\t\t2. Сохранение данных в файл\n";
        cout << "\t\t3. Добавление записи\n";
        cout << "\t\t4. Удаление записи\n";
        cout << "\t\t5. Вывод данных на экран\n";
        cout << "\t\t6. Отсортировать по ФИО\n";
        cout << "\t\t7. Отсортировать по виду платежа\n";
        cout << "\t\t8. Отсортировать по сумме платежа\n";
        cout << "\t\t9. Завершение работы программы\n";
        cout << "\t\t10. Быстрое добавление записи\n";

        cout << "\nВаш выбор: ";
        cin >> choice;

        switch (choice) {
        case 1:
            com.read_file();
            break;
        case 2:
            com.save_in_file();
            break;
        case 3:
            com.add_note();
            break;
        case 4:
            com.delete_note();
            break;
        case 5:
            com.output_data();
            break;
        case 6:
            com.sort_by_name();
            break;
        case 7:
            com.sort_by_payment_type();
            break;
        case 8:
            com.sort_by_payment();
            break;
        case 9:
            flag = false;
            break;
        case 10:
            com.fast_add();
            break;
        default:
            cout << "Нет такого пункта. Попробуйте еще раз.\n\n\n";
        }
        system("cls");
    }
    cout << "Завершение работы.";
    _getch();
    return 0;
}