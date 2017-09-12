#include "stdafx.h"
#include "BirthdaysDB_NewClasses.h"


int main()
{
	try {
		Database db;
		string input_stream;
		while (getline(cin, input_stream)) {
			stringstream input(input_stream);
			string command_name;
			//вычленияем из входного потока команду
			input >> command_name;
			Date date;
			FullName name;
			//Записываем в базу дату рождения и имя (Имя и Фамилия)
			if (command_name == "Add") {
				input >> name >> date;
				db.AddName(date, name);
			}
			//Ищем в базе всех людей у которых дата рожения в указанное время
			else if (command_name == "Findnames") {
				input >> date;
				for (const auto& name : db.Findnames(date))
					cout << name << endl;
			}

			//Ищем в базе конкретного человека и находим дату его рождения
			else if (command_name == "Finddate") {
				input >> name;
				cout << db.Finddate(name) << endl;
			}
			//Удаляем или всю дату или конкретное имя
			else if (command_name == "Delete") {
				string temp;
				input >> date >> temp;
				if (temp.size() != 0) {
					name.ParseName(temp);
					if (db.DeleteName(date, name))
						cout << "Deleted successfully" << endl;
					else
						cout << "Name not found" << endl;
				}
				else
					cout << "Deleted " << db.DeleteDate(date) << " names" << endl;
			}
			//Выводим на экран все даты рождений с именами людей
			else if (command_name == "Print") {
				db.PrintAll();
			}
			//Выходим из программы
			else if (command_name == "Exit") {
				return 0;
			}
			//Если на вход подается пустая строка, игнорируем ее
			else if (input_stream.size() == 0)
				break;
			else
				//Если команда не известна - вызываем исключение
				throw logic_error("Unknown command: " + input_stream);
		}
	}
	//Обрабатываем исключения и выводим сообщение с описанием ошибки
	catch (const logic_error& er) {
		cout << er.what() << endl;
	}
	return 0;
}

