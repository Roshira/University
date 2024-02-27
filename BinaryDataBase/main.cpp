#include <iostream>
#include <conio.h>
#include <fstream>
#include <stdio.h>
#include <vector>

using namespace std;
//enum transport
//перевірка
//Виведення зупинок по якому ходить транспорт

struct station_info
{
	unsigned int id;
	char name[20];
};

enum TransportType {
	BUS,
	TROLLEY,
	TRAM
};

struct transport {
	unsigned int id;
	TransportType type;
};
struct Route {
	unsigned int idTransport;
	unsigned int idStops;
	unsigned int hours;
	unsigned int minutes;
};

int addTransport() {
	unsigned int id = 1;
	transport part;
	FILE* fp;

	// Відкриття файлу у режимі читання
	if (fopen_s(&fp, "transport.bin", "rb") != 0) {
		cerr << "Could not open file for reading." << endl;
		return 1;
	}

	fseek(fp, 0, SEEK_END);
	long pos = ftell(fp);
	if (pos > 0)
	{
		fseek(fp, pos - sizeof(transport), SEEK_SET);
		fread(&part, sizeof(part), 1, fp);
		id = part.id + 1;
	}


	fclose(fp);

	// Відкриття файлу у режимі додавання
	if (fopen_s(&fp, "transport.bin", "ab") != 0) {
		cerr << "Could not open file for writing." << endl;
		return 1;
	}
	cout << "Enter 0 -exit\n";
	cout << "Enter 'b' for bus, 't' for trolley, or 'h' for tram\n\n";

	char typeInput = 'w';

	while (true) {
		part.id = id;
		cout << "Enter type: ";
		cin >> typeInput;
		if ((typeInput != 'b') && (typeInput != 't') && (typeInput != 'h'))
		{
			break;
		}
		switch (typeInput) {
		case 'b':
			part.type = BUS;
			break;
		case 't':
			part.type = TROLLEY;
			break;
		case 'h':
			part.type = TRAM;
			break;
		}
		fwrite(&part, sizeof(part), 1, fp);
		id++;
	}

		fclose(fp);

		return 0;
	
}

	int addStation() {
		unsigned int id = 1;
		station_info part;
		FILE* fp;
		// Відкриття файлу у режимі читання
		if (fopen_s(&fp, "station.bin", "rb") != 0) {
			cerr << "Could not open file for reading." << endl;
			return 1;
		}

		fseek(fp, 0, SEEK_END);
		long pos = ftell(fp);

		if (pos > 0)
		{
			fseek(fp, pos - sizeof(station_info), SEEK_SET);
			fread(&part, sizeof(part), 1, fp);
			id = part.id + 1;
		}

		fclose(fp);

		// Відкриття файлу у режимі додавання
		if (fopen_s(&fp, "station.bin", "ab") != 0) {
			cerr << "Could not open file for writing." << endl;
			return 1;
		}
		cout << "Enter 0 -exit\n\n\n";

		while (true)
		{
			part.id = id;
			cout << "Enter name: ";
			cin >> part.name;

			if (strcmp(part.name, "0") == 0) {
				break;
			}

			// Запис нового запису в кінець файлу
			fwrite(&part, sizeof(part), 1, fp);
			id++;

		}

		fclose(fp);

		return 0;
	}

	int readAndPrintTransport() {
		transport stop;
		long int ind = 0;
		FILE* fp;
		if (fopen_s(&fp, "transport.bin", "rb") != 0) {
			cerr << "Could not open file for reading." << endl;
			return 1;
		}

		fseek(fp, 0, SEEK_SET);
		while (!feof(fp))
		{
			if (fread(&stop, sizeof(transport), 1, fp)/*&&cur_tree.show_flag*/)
			{
				printf("________________________\n");
				switch (stop.type)
				{
				case 0:
					cout << "Type= Bus\n";
					break;
				case 1:
					cout << "Type= Trolley\n";
					break;
				case 2:
					cout << "Type= Tram\n";
					break;
				}
				printf("Index=%d\n", stop.id);
			}
		}
		fclose(fp);
		return 0;
	}

	int readAndPrintStops() {
		station_info stop;
		long int ind = 0;
		FILE* fp;
		if (fopen_s(&fp, "station.bin", "rb") != 0) {
			cerr << "Could not open file for reading." << endl;
			return 1;
		}

		fseek(fp, 0, SEEK_SET);
		while (!feof(fp))
		{
			if (fread(&stop, sizeof(station_info), 1, fp)/*&&cur_tree.show_flag*/)
			{
				printf("________________________\n");
				printf("Name=%s\n", stop.name);
				printf("Index=%d\n", stop.id);
			}
		}
		fclose(fp);
	}

	int addRoute() {
		Route route;
		FILE* fp;

		if (fopen_s(&fp, "route.bin", "ab") != 0) {
			cerr << "Could not open file for writing." << endl;
			return 1;
		}
		cout << "Enter e -exit\n";


		while (true) {


			cout << "enter ID stops: ";
			cin >> route.idStops;

			if (route.idStops == 0) {
				break;
			}

			cout << "enter ID transport: ";
			cin >> route.idTransport;
			cout << "/Enter time\\\n";
			cout << "hours: ";
			cin >> route.hours;
			cout << "minutes: ";
			cin >> route.minutes;
			station_info stop;
			FILE* file1;
			if (fopen_s(&file1, "station.bin", "rb") != 0) {
				cerr << "Could not open file for reading." << endl;
				return 1;
			}

			fseek(file1, 0, SEEK_SET);
			bool ST = true;
			while (!feof(file1))
			{
				if (fread(&stop, sizeof(station_info), 1, file1)/*&&cur_tree.show_flag*/)
				{
					if (route.idStops == stop.id) {
						ST = false;
					}
				}
			}
			if (ST) {
				cout << "Error stop";
				fclose(fp);
				fclose(file1);
				return 1;
			}
			fclose(file1);

			ST = true;
			transport transp;
			FILE* file2;
			if (fopen_s(&file2, "transport.bin", "rb") != 0) {
				cerr << "Could not open file for reading." << endl;
				return 1;
			}

			fseek(file2, 0, SEEK_SET);
			while (!feof(file2))
			{
				if (fread(&transp, sizeof(transport), 1, file2)/*&&cur_tree.show_flag*/)
				{
					if (route.idTransport == transp.id){
						ST = false;
					}
				}
			}
			if (ST) {
				cout << "ERRROR transport";
				fclose(fp);
				fclose(file2);
				return 1;
			}
			fclose(file2);
			fwrite(&route, sizeof(route), 1, fp);

		}
		
		
		fclose(fp);
		return 0;
	}

	int routePrint() {
		Route route;
		long int ind = 0;
		FILE* fp;
		if (fopen_s(&fp, "route.bin", "rb") != 0) {
			cerr << "Could not open file for reading." << endl;
			return 1;
		}

		fseek(fp, 0, SEEK_SET);
		while (!feof(fp))
		{
			if (fread(&route, sizeof(Route), 1, fp)/*&&cur_tree.show_flag*/)
			{
				printf("________________________\n");
				printf("Id stop=%u\n", route.idStops);
				printf("Id transport=%u\n", route.idTransport);
				printf("hours=%u\n", route.hours);
				printf("minutes=%u\n", route.minutes);
			}
		}
		fclose(fp);
	}

	int clearbased() {
		ofstream file1("transport.bin", ios::binary);

		if (!file1.is_open()) {
			cerr << "error transport" << endl;
			return 1;
		} file1.close();
		ofstream file2("route.bin", ios::binary);

		if (!file2.is_open()) {
			cerr << "error route" << endl;
			return 1;
		} file2.close();
		ofstream file3("station.bin", ios::binary);

		if (!file3.is_open()) {
			cerr << "error stops" << endl;
			return 1;
		} file3.close();
		return 0;
	}

	int printTransportStop() {
		int idTransport;
		cout << "Enter id transport: ";
		cin >> idTransport;

		Route route;
		long int ind = 0;
		FILE* fp;
		if (fopen_s(&fp, "route.bin", "rb") != 0) {
			cerr << "Could not open file for reading." << endl;
			return 1;
		}

		fseek(fp, 0, SEEK_SET);
		while (!feof(fp))
		{
			if (fread(&route, sizeof(Route), 1, fp)/*&&cur_tree.show_flag*/)
			{
				if (idTransport == route.idTransport) {
					printf("________________________\n");
					printf("Id stop=%u\n", route.idStops);
					printf("Id transport=%u\n", route.idTransport);
					printf("hours=%u\n", route.hours);
					printf("minutes=%u\n", route.minutes);
				}
			}
		}
		fclose(fp);
		

	}

	int deleteTransport() {
		int id;
		cout << "Enter the ID of the transport you want to delete: ";
		cin >> id;

		ifstream input_file("transport.bin", ios::binary);
		if (!input_file) {
			cerr << "Could not open input file for reading." << endl;
			return 1;
		}

		ofstream output_file("transportNEW.bin", ios::binary);
		if (!output_file) {
			cerr << "Could not open output file for writing." << endl;
			return 1;
		}

		transport stop;
		while (input_file.read(reinterpret_cast<char*>(&stop), sizeof(stop))) {
			if (stop.id != id) {
				if (id < stop.id)
					stop.id--;
				output_file.write(reinterpret_cast<const char*>(&stop), sizeof(stop));
			}
		}
		input_file.close();
		remove("transport.bin");
		output_file.close();

		if (std::rename("transportNEW.bin", "transport.bin") != 0) { // Виклик rename() і перевірка на помилку
			perror("Error renaming file"); // Вивід повідомлення про помилку, якщо вона виникла
			return 1; // Повернення коду помилки
		}

		return 0;
	}

	int deleteStops() {
		int id;
		cout << "Enter the ID of the stop you want to delete: ";
		cin >> id;

		ifstream input_file("station.bin", ios::binary);
		if (!input_file) {
			cerr << "Could not open input file for reading." << endl;
			return 1;
		}

		ofstream output_file("stationNEW.bin", ios::binary);
		if (!output_file) {
			cerr << "Could not open output file for writing." << endl;
			return 1;
		}

		station_info stop;
		while (input_file.read(reinterpret_cast<char*>(&stop), sizeof(stop))) {
			if (stop.id != id) {
				if (id < stop.id)
					stop.id--;
				output_file.write(reinterpret_cast<const char*>(&stop), sizeof(stop));
			}
		}
		input_file.close();
		remove("station.bin");
		output_file.close();

		if (std::rename("stationNEW.bin", "station.bin") != 0) {
			perror("Error renaming file"); // 
			return 1; // 
		}

		return 0;
	}

	int deleteRoute() {

		int idTransport;
		int idStops;
		int Hours;
		cout << "idTransport: ";
		cin >> idTransport;
		cout << "idStops: ";
		cin >> idStops;
		cout << "Hours: ";
		cin >> Hours;

		ifstream input_file("route.bin", ios::binary);
		if (!input_file) {
			cerr << "Could not open input file for reading." << endl;
			return 1;
		}

		ofstream output_file("routeNEW.bin", ios::binary);
		if (!output_file) {
			cerr << "Could not open output file for writing." << endl;
			return 1;
		}

		Route stop;
		while (input_file.read(reinterpret_cast<char*>(&stop), sizeof(stop))) {
			if ((stop.hours != Hours) && (stop.idStops != idStops) && (stop.idTransport != idTransport)) {
				output_file.write(reinterpret_cast<const char*>(&stop), sizeof(stop));
			}
		}
		input_file.close();
		output_file.close();
		if (remove("route.bin") != 0) {
			cerr << "Error removing existing file." << endl;
			return 1;
		}


		if (std::rename("routeNEW.bin", "route.bin") != 0) {
			perror("Error renaming file"); // 
			return 1; // 
		}

		return 0;

	}
	int main() {
		char number = 'f';

		while (number != '0') {
			cout << "1 - Record the station\n";
			cout << "2 - station list\n";
			cout << "3 - add transport\n";
			cout << "4 - transport list\n";
			cout << "5 - addRoute\n";
			cout << "6 - printRoute\n";
			cout << "7 - clear based\n";
			cout << "8 - print stops transport\n";
			cout << "t - clear id transport\n";
			cout << "s - clear id stop\n";
			cout << "r - clear route\n";
			cout << "0 - Exit\n";
			number = _getch();
			system("cls");
			switch (number) {
			case '1':
				addStation();
				break;
			case '2':
				readAndPrintStops();
				break;
			case '3':
				addTransport();
				break;
			case '4':
				readAndPrintTransport();
				break;
			case '5':
				addRoute();
				break;
			case '6':
				routePrint();
				break;
			case '7':
				clearbased();
				break;
			case '8':
				printTransportStop();
				break;
			case 't':
				deleteTransport();
				break;
			case 's':
				deleteStops();
				break;
			case 'r':
				deleteRoute();
				break;
			case '0':
				cout << "You have exited the program\n";
				break;
			}

			if (number != '0') {
				_getch();
				system("cls");
			}
		}

		return 0;
	}