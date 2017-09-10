/* Заголовочный файл в котором мы создаем новые классы Date, FullName и Database, 
а также перегружаем операторы ввода, вывода и сравнения для них */

#pragma once
#include "stdafx.h"
#include <iostream>
#include <string>
#include <map>
#include <exception>
#include <algorithm>
#include <set>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

class FullName;
class Date;
class Database;
bool operator<(const FullName& lhs, const FullName& rhs);
bool operator<(const Date& lhs, const Date& rhs);
istream& operator>>(istream& stream, FullName& name);
istream& operator>>(istream& stream, Date& date);
ostream& operator<<(ostream& stream, const FullName& name);
ostream& operator<<(ostream& stream, const Date& date);

/*Создаем новые классы FullName, Date, Database*/

//Класс для полного имени
class FullName {
public:
	//Парсим имя из потока
	void ParseName(const string& new_name) {
		stringstream temp(new_name);
		getline(temp, first_name, '-');
		CheckNextSymbolAndSkip(temp, new_name);
		getline(temp, last_name);
		full_name = first_name + " " + last_name;
	}
	//Методы для вывода имени и фамилии по отдельности
	string GetFirstName() const {
		return first_name;
	}
	string GetLastName() const {
		return last_name;
	}
	//Метод для вывода имени и фамилии вместе
	string GetFullName() const {
		return full_name;
	}
	//Возвращаем размер переменной
	int size() const {
		return full_name.size();
	}

private:
	string first_name, last_name, full_name;
	//Проверяем на правильность ввода и убираем тире
	void CheckNextSymbolAndSkip(stringstream &input, const string& new_name) const {
		input.unget();
		if ( input.get() != '-')
			throw logic_error("Wrong name format: " + new_name);
	}
};

//Класс даты рождения
class Date {
public:
	//Парсим дату из потока
	void ParseDate(const string& new_date) {
		int temp_year, temp_month, temp_day;
		stringstream temp(new_date);
		string trash;
		if (temp) {
			temp >> temp_year;
			CheckNextSymbolAndSkip(temp, new_date);
			temp >> temp_month;
			CheckNextSymbolAndSkip(temp, new_date);
			temp >> temp_day;
			temp >> trash;
			if (trash.size() != 0)
				throw logic_error("Wrong date format: " + new_date);
		}
		//Записываем данные в переменные из приватного поля
		year = temp_year;
		if (temp_month < 1 || temp_month > 12)
			throw logic_error("Month value is invalid: " + to_string(temp_month));
		else month = temp_month;
		if (temp_day < 1 || temp_day > 31)
			throw logic_error("Day value is invalid: " + to_string(temp_day));
		else day = temp_day;
		//Обращаемся к приватному методу для формирования даты в строковой переменной
		SetDate(year, month, day);
	}
	//Методы для выводы даты по частям(год или месяц или день)
	int GetYear() const {
		return year;
	}
	int GetMonth() const {
		return month;
	}
	int GetDay() const {
		return day;
	}
	//Метод для получения даты	
	string GetDate() const {
		return date;
	}
	//Возвращаем размер переменной
	int size() const {
		return date.size();
	}

private:
	int year, month, day;
	string date;

	//Формируем дату в строковой переменной в нужном формате
	void SetDate(const int& year, const int& month, const int& day) {
		stringstream temp2;
		temp2 << setw(4) << setfill('0') << year << '.';
		temp2 << setw(2) << setfill('0') << month << '.';
		temp2 << setw(2) << setfill('0') << day;
		temp2 >> date;
	}
	//Делаем проверку на наличие тире и пропускаем его.
	void CheckNextSymbolAndSkip(stringstream &input, const string& date) const {
		if (input.peek() == '-')
			input.ignore(1);
		else
			throw logic_error("Wrong date format: " + date);
	}
};

//Создаём основной класс для хранения базы данных дат рождений с полными именами
class Database {
public:
	//Метод для добавления новой даты рождения
	void AddName(const Date& date, const FullName& name) {
		birthdays_db[date].insert(name);
	}
	//Метод для удаления имении из определенной даты
	bool DeleteName(const Date& date, const FullName& name) {
		if (birthdays_db.count(date) != 0 && birthdays_db.at(date).count(name) != 0) {
			birthdays_db[date].erase(name);
			return true;
		}
		else return false;
	}
	//Метод для удаления даты со всеми днями рождения
	int DeleteDate(const Date& date) {
		if (birthdays_db.count(date) != 0) {
			const int number_of_names = birthdays_db[date].size();
			birthdays_db.erase(date);
			return number_of_names;
		}
		else return 0;
	}
	//Метод для поиска имен в базе событий на конкретную дату рождения
	set<FullName> Findnames(const Date& date) const {
		if (birthdays_db.count(date) != 0)
			return birthdays_db.at(date);
		else return {};
	}

	//Метод для поиска имен в базе событий на конкретную дату рождения
	Date Finddate(const FullName& name) const {
		for (const auto& names : birthdays_db) {
			if (names.second.count(name) != 0)
				return names.first;
			else return {};
		}
	}
	//Метод для вывода всех дней рождения и имен
	void PrintAll() const {
		for (const auto& date : birthdays_db) {
			for (const auto& name : date.second)
				cout << date.first << " " << name << endl;
		}
	}

private:
	//Для хранения данных используем контейнеры map и set
	map<Date, set<FullName>> birthdays_db;
};


/*ПЕРЕГРУЖАЕМ ОПЕРАТОРЫ ввода, вывода и сравнения для классов Date и FullName*/

//Перегружаем оператор меньше для автоматической сортировки в map и set для класса FullName
bool operator<(const FullName& lhs, const FullName& rhs) {
	// сравним даты с помощью вектора
	return vector<string>{lhs.GetLastName(), lhs.GetFirstName()} <
		vector<string>{rhs.GetLastName(), rhs.GetFirstName()};
}

//Перегружаем оператор ввода из потока для класса FullName
istream& operator>>(istream& stream, FullName& name) {
	string new_name;
	stream >> new_name;
	name.ParseName(new_name);
	return stream;
}

//Перегружаем оператор вывода в поток для класса FullName
ostream& operator<<(ostream& stream, const FullName& name) {
	//const string to_print = name.GetFullName();
	stream << name.GetFullName();
	return stream;
}

//Перегружаем оператор ввода из потока для класса Date
istream& operator>>(istream& stream, Date& date) {
	string new_date;
	stream >> new_date;
	date.ParseDate(new_date);
	return stream;
}

//Перегружаем оператор вывода в поток для класса Date
ostream& operator<<(ostream& stream, const Date& date) {
//	const string to_print = date.GetDate();
	stream << date.GetDate();
	return stream;
}

//Перегружаем оператор меньше для автоматической сортировки в map и set для класса Date
bool operator<(const Date& lhs, const Date& rhs) {
	// сравним даты с помощью вектора
	return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <
		vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

