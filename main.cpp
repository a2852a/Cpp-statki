#include <iostream>
#include <conio.h>
#include <windows.h>
#include <iomanip> 
#include <string>
#include <sstream>
#include <queue>
#include <time.h>

#include "Header.h"

using namespace std;


//KLASA DO OBSLUGI STATKOW

class ships { // klasa przechowujaca statki oraz metody operowania nimi

private:

	struct ship_cont {
		COORD pkt_A;
		COORD ship_body[5];
		bool ship_hp[5];
		int type;
		int hp;
	};

public:
	ship_cont * fleet;
	int sinked_ships = 0;


	void navigate_ship(int ship_id , COORD ship_root_cord, bool col_flag , int MAXseaX , int MAXseaY ) { // Rysuje w konsoli statek przed dodaniem
		COORD tempXY;
		COORD retXY;

		retXY.X = 0;
		retXY.Y = 3 + MAXseaY;
		SetConsoleCoords(retXY);


		cout << "Wspolrzedne punktu A: [" << ship_root_cord.X - 1 << ":" << (char)(ship_root_cord.Y - 2 + 65) << "]"; for (int i = 0; i < MAXseaX + 1 - ship_root_cord.X; i++) cout << " "; cout << endl;
		for (int j = 0; j < MAXseaX + 1; j++) cout << "-";


		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		if (col_flag == true) { SetConsoleTextAttribute(hConsole, 12); }
		else {
			SetConsoleTextAttribute(hConsole, 2);

		}


		if (fleet[ship_id].type == 1) {   // Jezeli statek jest w poziomie
			tempXY = ship_root_cord;

			for (int i = 0; i < fleet[ship_id].hp; i++) {

				SetConsoleCoords(tempXY);

				if (tempXY.X < 0 || tempXY.X > MAXseaX) continue; // Nie rysuj tego co wychodzi poza plansze
				cout  << "+";
				tempXY.X++;

			}
		};

		if (fleet[ship_id].type == 3) {   // Jezeli statek jest w pionie
			tempXY = ship_root_cord;

			for (int i = 0; i < fleet[ship_id].hp; i++) {

				SetConsoleCoords(tempXY);
				if (tempXY.Y < 0 || tempXY.Y > MAXseaY+1) continue;
				cout << "+";
				tempXY.Y++;

			}
		};

		if (fleet[ship_id].type == 2) {   // Jezeli statek jest w ukosie
			tempXY = ship_root_cord;

			for (int i = 0; i < fleet[ship_id].hp; i++) {

				SetConsoleCoords(tempXY);

				if (tempXY.X < 0 || tempXY.X > MAXseaX) continue; // Nie rysuj tego co wychodzi poza plansze
				if (tempXY.Y < 0 || tempXY.Y > MAXseaY + 1) continue;

				cout << "+";
				tempXY.X++; tempXY.Y++;

			}
		};

		if (fleet[ship_id].type == 4) {   // Jezeli statek jest w ukosie
			tempXY = ship_root_cord;

			for (int i = 0; i < fleet[ship_id].hp; i++) {

				SetConsoleCoords(tempXY);

				if (tempXY.X <= 0 || tempXY.X > MAXseaX) continue; // Nie rysuj tego co wychodzi poza plansze
				if (tempXY.Y < 0 || tempXY.Y > MAXseaY + 1) continue;

				cout << "+";
				tempXY.X--; tempXY.Y++;

			}
		};


		fleet[ship_id].pkt_A = ship_root_cord;
		SetConsoleTextAttribute(hConsole, 7);


	};

	bool hp_change_ship(int ship_id , bool op) { //op=true zwieksz hp op=false zmniejsz hp

		if(op == true)
		if (fleet[ship_id].hp == 5) { fleet[ship_id].hp = 2; }
		else { fleet[ship_id].hp++; }


		if(op == false)
		if (fleet[ship_id].hp == 2) { fleet[ship_id].hp = 5; }
		else { fleet[ship_id].hp--; }


		return 1;
	};

	void type_change_ship(int ship_id ) {

		if (fleet[ship_id].type == 4) { fleet[ship_id].type = 1; }
		else { fleet[ship_id].type++; }

	};


	ships(int ship_count);
};

ships::ships(int ship_count) {
	fleet = new ship_cont[ship_count+1];
	
	for (int i = 0; i < ship_count; i++) {

		fleet[i].hp = 2;
		fleet[i].type = 1;

		for (int j = 0; j < 5; j++) {
			fleet[i].ship_hp[j] = true; // Nadane punktu zycia statkowi
		}
	}
}

//-----------------------------------

// KLASA DO OBSLUGI PLANSZY

class battlesea { //Klasa przechowujaca pole gry oraz metody operujace nim

private:

	bool add_ship_2_sea(ships * ship , int * ship_id) { // Dodaje do morza statek - wsp poprawione o interfejs

		if ((*ship).fleet[*ship_id].type == 1) {   // Jezeli statek jest w poziomie

			for (int i = 0; i < (*ship).fleet[*ship_id].hp; i++) {

				(*ship).fleet[*ship_id].ship_body[i].X = (*ship).fleet[*ship_id].pkt_A.X + i - 1; // Ustanowienie ciala statku
				(*ship).fleet[*ship_id].ship_body[i].Y = (*ship).fleet[*ship_id].pkt_A.Y -2; // Ustanowienie ciala statku

				sea[(*ship).fleet[*ship_id].pkt_A.Y-2][(*ship).fleet[*ship_id].pkt_A.X+i-1] = '*';
				taken_points[taken_points_ctr].X = (*ship).fleet[*ship_id].pkt_A.X + i - 1;
				taken_points[taken_points_ctr++].Y = (*ship).fleet[*ship_id].pkt_A.Y - 2;

			}
		};

		if ((*ship).fleet[*ship_id].type == 3) {   // Jezeli statek jest w pionie
			
			for (int i = 0; i < (*ship).fleet[*ship_id].hp; i++) {

				(*ship).fleet[*ship_id].ship_body[i].X = (*ship).fleet[*ship_id].pkt_A.X - 1; // Ustanowienie ciala statku
				(*ship).fleet[*ship_id].ship_body[i].Y = (*ship).fleet[*ship_id].pkt_A.Y + i - 2; // Ustanowienie ciala statku

				sea[(*ship).fleet[*ship_id].pkt_A.Y + i - 2][(*ship).fleet[*ship_id].pkt_A.X - 1] = '*';
				taken_points[taken_points_ctr].X = (*ship).fleet[*ship_id].pkt_A.X - 1;
				taken_points[taken_points_ctr++].Y = (*ship).fleet[*ship_id].pkt_A.Y + i - 2;


			}
		};

		if ((*ship).fleet[*ship_id].type == 2) {   // Jezeli statek jest w ukosie
			
			for (int i = 0; i < (*ship).fleet[*ship_id].hp; i++) {

				(*ship).fleet[*ship_id].ship_body[i].X = (*ship).fleet[*ship_id].pkt_A.X + i - 1; // Ustanowienie ciala statku
				(*ship).fleet[*ship_id].ship_body[i].Y = (*ship).fleet[*ship_id].pkt_A.Y + i - 2; // Ustanowienie ciala statku

				sea[(*ship).fleet[*ship_id].pkt_A.Y + i - 2][(*ship).fleet[*ship_id].pkt_A.X + i - 1] = '*';
				taken_points[taken_points_ctr].X = (*ship).fleet[*ship_id].pkt_A.X + i - 1;
				taken_points[taken_points_ctr++].Y = (*ship).fleet[*ship_id].pkt_A.Y + i - 2;

			}
		};

		if ((*ship).fleet[*ship_id].type == 4) {   // Jezeli statek jest w ukosie
			

			for (int i = 0; i < (*ship).fleet[*ship_id].hp; i++) {


				(*ship).fleet[*ship_id].ship_body[i].X = (*ship).fleet[*ship_id].pkt_A.X - i - 1; // Ustanowienie ciala statku
				(*ship).fleet[*ship_id].ship_body[i].Y = (*ship).fleet[*ship_id].pkt_A.Y + i - 2; // Ustanowienie ciala statku

				sea[(*ship).fleet[*ship_id].pkt_A.Y + i - 2][(*ship).fleet[*ship_id].pkt_A.X - i - 1] = '*';
				taken_points[taken_points_ctr].X = (*ship).fleet[*ship_id].pkt_A.X - i - 1;
				taken_points[taken_points_ctr++].Y = (*ship).fleet[*ship_id].pkt_A.Y + i - 2;

			}
		};
		

		return true;
	};

	void draw_ships_av_setup() {
		cout << "\n\n\n| Ilosc dostepnych statkow" << endl;
		cout << "| Statki typu 2: " << ships_count[0] << endl;
		cout << "| Statki typu 3: " << ships_count[1] << endl;
		cout << "| Statki typu 4: " << ships_count[2] << endl;
		cout << "| Statki typu 5: " << ships_count[3] << endl;
		for (int j = 0; j < MAXseaX + 1; j++) cout << "-";
		cout << "\n|LEGENDA:\n"; 
		cout << "|-/+ - Zwieksz / zmniejsz rozmiar statku\n";
		cout << "|r - Obroc statek\n";
		cout << "|a - Automatyczne ustawienie statkow\n";

	}

	void draw_sea_av_setup() {

		curs_def.X = 0;
		curs_def.Y = 0;
		char temp_c = 'A';


		SetConsoleCoords(curs_def);

		cout << player_name << " - Ustawia statki" << endl;
		for (int j = 0; j < MAXseaX+1; j++) cout << "-"; cout << endl;

		for (int j = 0; j < MAXseaY; j++)
		{
			cout << "|";
			for (int i = 0; i < MAXseaX; i++) cout << sea[j][i];
			cout <<"|"<< temp_c++;
			cout << endl;
		}

		for (int j = 0; j < MAXseaX+1; j++) cout << "-";
	};

	bool check_cords_colision(COORD temp_cords) { // Sprawdza czy dany punkt nie zawiera sie w zajetych punktach na mapie false - nie koliduje true -  koliduje
		for (int i = 0; i < taken_points_ctr; i++) {
			if ((temp_cords.X == taken_points[i].X) && (temp_cords.Y == taken_points[i].Y)) return true;
		}
		return false;

	};

	bool check_colision(ships ship , int * ship_id ,  COORD crd ) { // sprawdza czy dany statek nie koliduje z zadnym innym statkiem false - nie koliduje true - koliduje
		COORD temp_cords;
		ship.fleet[*ship_id].pkt_A = crd;


		if (ship.fleet[*ship_id].type == 1) {   // Jezeli statek jest w poziomie

			for (int i = 0; i < ship.fleet[*ship_id].hp; i++) {

				for (int k = -1; k < 2; k++) {
					for (int l = -1; l < 2; l++) {
						temp_cords.X = ship.fleet[*ship_id].pkt_A.X + i - 1 - k;
						temp_cords.Y = ship.fleet[*ship_id].pkt_A.Y - 2 - l;

						if (temp_cords.X < -1 || temp_cords.Y < -1) return true; // -1 -  moze dotykac scian jak nie to 0
						if (temp_cords.X == MAXseaX + 1 || temp_cords.Y == MAXseaY + 1) return true; // +1 moze dotykac scian jak nie to 0
						if (check_cords_colision(temp_cords) == true) return true;

					}	
				};
			}
			return false;
		};

		if (ship.fleet[*ship_id].type == 3) {   // Jezeli statek jest w pionie

			for (int i = 0; i < ship.fleet[*ship_id].hp; i++) {

				for (int k = -1; k < 2; k++) {
					for (int l = -1; l < 2; l++) {
						temp_cords.X = ship.fleet[*ship_id].pkt_A.X - 1 - k;
						temp_cords.Y = ship.fleet[*ship_id].pkt_A.Y + i - 2 - l;

						if (temp_cords.X < -1 || temp_cords.Y < -1) return true; // -1 -  moze dotykac scian jak nie to 0
						if (temp_cords.X == MAXseaX + 1 || temp_cords.Y == MAXseaY + 1) return true; // +1 moze dotykac scian jak nie to 0
						if (check_cords_colision(temp_cords) == true) return true;

					}
				};
			}
			return false;
		};

		if (ship.fleet[*ship_id].type == 2) {   // Jezeli statek jest w ukosie

			for (int i = 0; i < ship.fleet[*ship_id].hp; i++) {

				
				for (int k = -1; k < 2; k++) {
					for (int l = -1; l < 2; l++) {
						temp_cords.X = ship.fleet[*ship_id].pkt_A.X + i - 1 - k;
						temp_cords.Y = ship.fleet[*ship_id].pkt_A.Y + i - 2 - l;

						if (temp_cords.X < -1 || temp_cords.Y < -1) return true; // -1 -  moze dotykac scian jak nie to 0
						if (temp_cords.X == MAXseaX + 1 || temp_cords.Y == MAXseaY + 1) return true; // +1 moze dotykac scian jak nie to 0
						if (check_cords_colision(temp_cords) == true) return true;

					}
				};

			}
			return false;
		};

		if (ship.fleet[*ship_id].type == 4) {   // Jezeli statek jest w ukosie


			for (int i = 0; i < ship.fleet[*ship_id].hp; i++) {


				for (int k = -1; k < 2; k++) {
					for (int l = -1; l < 2; l++) {
						temp_cords.X = ship.fleet[*ship_id].pkt_A.X - i - 1 - k;
						temp_cords.Y = ship.fleet[*ship_id].pkt_A.Y + i - 2 - l;

						if (temp_cords.X < -1 || temp_cords.Y < -1) return true; // -1 -  moze dotykac scian jak nie to 0
						if (temp_cords.X == MAXseaX + 1 || temp_cords.Y == MAXseaY + 1 ) return true; // +1 moze dotykac scian jak nie to 0
						if (check_cords_colision(temp_cords) == true) return true;

					}
				};
			}
			return false;
		};
		return true;
	}

	void draw_prebattle_shipstats() {
		ClearScreen();
		cout << "Wyznacz ilosc statkow uzytych w grze:" << endl;
		cout << "<>Statki typu 2: " << ships_count[0] << endl;
		cout << "  Statki typu 3: " << ships_count[1] << endl;
		cout << "  Statki typu 4: " << ships_count[2] << endl;
		cout << "  Statki typu 5: " << ships_count[3] << endl;

	}

	int prebattle_shipnavigation(int limit = 0) { // Wybor opcji
		char wyb;
		int option;
		option = 0;
		curXY.X = 0; // reset menu
		curXY.Y = 1; // reset menu

		SetConsoleCoords(curXY);


		while ((wyb = _getch()) != 13) {   //Menu strzalkowe
			if (wyb == 72) { if (option == 0)continue; cout << "  "; curXY.Y--;   SetConsoleCoords(curXY); cout << "<>";  SetConsoleCoords(curXY); option--; }
			if (wyb == 80) { if (option == 4 - limit)continue; cout << "  "; curXY.Y++;   SetConsoleCoords(curXY); cout << "<>";  SetConsoleCoords(curXY); option++; }
			if (wyb == 75) { if (ships_count[option] == 0)continue; ships_count[option]--; curXY.X = 17; SetConsoleCoords(curXY); cout << ships_count[option]; curXY.X = 0; SetConsoleCoords(curXY);  continue; };
			if (wyb == 77) { if (ships_count[option] == 5)continue; ships_count[option]++; curXY.X = 17; SetConsoleCoords(curXY); cout << ships_count[option]; curXY.X = 0; SetConsoleCoords(curXY); continue; };
		
		};

		return option;

	}

	void draw_prebattle_seastats() {
		ClearScreen();
		cout << "Wyznacz dlugosci planszy:" << endl;
		cout << "<>Dlugosc Y: " << MAXseaY << endl;
		cout << "  Dlugosc X: " << MAXseaX << endl;


	}

	int prebattle_seanavigation(int limit = 0) { // Wybor opcji
		char wyb;
		int option;
		option = 0;
		curXY.X = 0; // reset menu
		curXY.Y = 1; // reset menu

		SetConsoleCoords(curXY);


		while ((wyb = _getch()) != 13) {   //Menu strzalkowe
			if (wyb == 72) { if (option == 0)continue; cout << "  "; curXY.Y--;   SetConsoleCoords(curXY); cout << "<>";  SetConsoleCoords(curXY); option--; }
			if (wyb == 80) { if (option == 4 - limit)continue; cout << "  "; curXY.Y++;   SetConsoleCoords(curXY); cout << "<>";  SetConsoleCoords(curXY); option++; }
			if (wyb == 75) { if (option == 0) { if (MAXseaY == 1)continue; MAXseaY--; } else { if (MAXseaX == 1)continue; MAXseaX--; } curXY.X = 13; SetConsoleCoords(curXY); if (option == 0)cout << MAXseaY <<" "; else cout << MAXseaX << "    "; curXY.X = 0; SetConsoleCoords(curXY);  continue; };
			if (wyb == 77) { if (option == 0) { if (MAXseaY == 26)continue; MAXseaY++; } else { if (MAXseaX == 10000)continue; MAXseaX++; } curXY.X = 13; SetConsoleCoords(curXY); if (option == 0)cout << MAXseaY << " "; else cout << MAXseaX << "    "; curXY.X = 0; SetConsoleCoords(curXY); continue; };

		};

		return option;

	}

	public:

		string player_name;

		char **sea;
		char **enemy_sea;

		int MAXseaY = 12;
		int MAXseaX = 30;

		COORD * taken_points;

		int taken_points_ctr = 0;
		int fleet_hp;

		COORD curs_def;
		COORD curXY;

		queue <string> log_que;

		bool PLAYER_CHEATMODE = false;


		int ships_count[4];
		int ships_number = 0;


	void main_setup(ships ship, bool PC_player) {

		int ship_index = 0;

		curXY.X = MAXseaX / 2 + 1;
		curXY.Y = MAXseaY / 2 + 2;

		char wyb;
		bool col_flag = false;

		draw_sea_av_setup(); draw_ships_av_setup();
		col_flag = check_colision(ship, &ship_index, curXY);
		ship.navigate_ship(ship_index, curXY, col_flag, MAXseaX, MAXseaY);


		while (1) {
			ShowConsoleCursor(false);

			if (PC_player == false)
				while ((wyb = _getch()) != 13) {   //Menu strzalkowe

					if (wyb == 97) {
						break;
					};

					if (wyb == 72) { // STRZALKA W GORE

						draw_sea_av_setup(); draw_ships_av_setup();

						if (curXY.Y == 2) { ship.navigate_ship(ship_index, curXY, col_flag, MAXseaX, MAXseaY); continue; }

						curXY.Y--;

						SetConsoleCoords(curXY);
						col_flag = check_colision(ship, &ship_index, curXY);
						if (col_flag == false && (ships_count[ship.fleet[ship_index].hp - 2] == 0))col_flag = true;
						ship.navigate_ship(ship_index, curXY, col_flag, MAXseaX, MAXseaY);
					};


					if (wyb == 80) { // STRZALKA W DOL

						draw_sea_av_setup(); draw_ships_av_setup();

						if (curXY.Y == MAXseaY + 1) { ship.navigate_ship(ship_index, curXY, col_flag, MAXseaX, MAXseaY); continue; }

						curXY.Y++;

						SetConsoleCoords(curXY);
						col_flag = check_colision(ship, &ship_index, curXY);
						if (col_flag == false && (ships_count[ship.fleet[ship_index].hp - 2] == 0))col_flag = true;
						ship.navigate_ship(ship_index, curXY, col_flag, MAXseaX, MAXseaY);
					};


					if (wyb == 75) { // STRZALKA W LEWO
						draw_sea_av_setup(); draw_ships_av_setup();

						if (curXY.X == 1) { ship.navigate_ship(ship_index, curXY, col_flag, MAXseaX, MAXseaY); continue; }

						curXY.X--;

						SetConsoleCoords(curXY); col_flag = check_colision(ship, &ship_index, curXY);
						if (col_flag == false && (ships_count[ship.fleet[ship_index].hp - 2] == 0))col_flag = true;
						ship.navigate_ship(ship_index, curXY, col_flag, MAXseaX, MAXseaY);
					};


					if (wyb == 77) { // STRZALKA W PRAWO
						draw_sea_av_setup(); draw_ships_av_setup();

						if (curXY.X == MAXseaX) { ship.navigate_ship(ship_index, curXY, col_flag, MAXseaX, MAXseaY); continue; }

						curXY.X++;

						SetConsoleCoords(curXY); col_flag = check_colision(ship, &ship_index, curXY);
						if (col_flag == false && (ships_count[ship.fleet[ship_index].hp - 2] == 0))col_flag = true;
						ship.navigate_ship(ship_index, curXY, col_flag, MAXseaX, MAXseaY);
					};


					if (wyb == 114) { // ZNAK r  - zmiana obrot
						draw_sea_av_setup(); draw_ships_av_setup();

						ship.type_change_ship(ship_index); SetConsoleCoords(curXY);
						col_flag = check_colision(ship, &ship_index, curXY);

						if (col_flag == false && (ships_count[ship.fleet[ship_index].hp - 2] == 0))col_flag = true;
						ship.navigate_ship(ship_index, curXY, col_flag, MAXseaX, MAXseaY);
					};


					if (wyb == 43) { // ZNAK +  - zmiana rozmiaru (zwieksz)
						draw_sea_av_setup(); draw_ships_av_setup();

						ship.hp_change_ship(ship_index, true); SetConsoleCoords(curXY);
						col_flag = check_colision(ship, &ship_index, curXY);

						if (col_flag == false && (ships_count[ship.fleet[ship_index].hp - 2] == 0))col_flag = true;
						ship.navigate_ship(ship_index, curXY, col_flag, MAXseaX, MAXseaY);
					};


					if (wyb == 45) { // ZNAK -  - zmiana rozmiaru (zmniejsz)
						draw_sea_av_setup(); draw_ships_av_setup();

						ship.hp_change_ship(ship_index, false); SetConsoleCoords(curXY);
						col_flag = check_colision(ship, &ship_index, curXY);

						if (col_flag == false && (ships_count[ship.fleet[ship_index].hp - 2] == 0))col_flag = true;
						ship.navigate_ship(ship_index, curXY, col_flag, MAXseaX, MAXseaY);
					};

				};

			if (PC_player == true || wyb == 97) {

				int buf_ships_count;
				for (int i = 0; i < 4; i++) { // Pobranie typu danego statku
					buf_ships_count = ships_count[i];
					for (int j = 0; j < buf_ships_count; j++) { // Wykonuj petle tyle razy ile jest statkow danego typu
						draw_sea_av_setup(); draw_ships_av_setup();
						for (int k = 0; k < i; k++) ship.hp_change_ship(ship_index, true); //Ustaw hp statku

						while (1) {
							curXY.X = rand() % (MAXseaX + 1) + 0;  // Losowanie punku A statku (plus o 1 wieksza dla losowania)
							curXY.Y = rand() % (MAXseaY + 1) + 0;
							ship.fleet[ship_index].type = rand() % 5 + 1;  // Losowanie typu dla statku

							col_flag = check_colision(ship, &ship_index, curXY);
							if (ships_count[ship.fleet[ship_index].hp - 2] != 0)
								if (!col_flag) break;

						}
						ship.navigate_ship(ship_index, curXY, col_flag, MAXseaX, MAXseaY); // Wywolanie do zaktualizowania cordow
						

						ships_count[ship.fleet[ship_index].hp - 2]--;
						add_ship_2_sea(&ship, &ship_index);
						ship_index++;

						Sleep(600);
						draw_sea_av_setup();
						draw_ships_av_setup();
					}
				}
				if (ships_count[0] == 0 && ships_count[1] == 0 && ships_count[2] == 0 && ships_count[3] == 0) { cout << "\nROZMIESZCZONO... Wcisnij dowolny znak aby kontynuowac... \n";  if (PC_player == false) { wyb = _getch(); } else { Sleep(600); } return; }
				else { cout << "WYSTAPIL BLAD KRYTYCZNY\n Sprawdz ustawienia gry\n" << endl; system("pause"); exit(-1); }

			}



			if (ships_count[ship.fleet[ship_index].hp - 2] != 0)
				if (!col_flag) {
					ships_count[ship.fleet[ship_index].hp - 2]--;
					add_ship_2_sea(&ship, &ship_index);
					ship_index++;

					draw_sea_av_setup();
					draw_ships_av_setup();
				}

			if (ships_count[0] == 0 && ships_count[1] == 0 && ships_count[2] == 0 && ships_count[3] == 0) { cout << "\nROZMIESZCZONO... Wcisnij dowolny znak aby kontynuowac... \n";  wyb = _getch(); return; } // Jezeli rozstawiono wszystkie statki, wroc

		}

	}

	void prepare_sea(bool copy = false) { // Przygotuj pole sea jezeli copy = true nie pobieraj danych od uzytkownika (zatwierdzone zostaja dane w obiekcie - do kopiowania pola)
		

		if (copy == false) {
			draw_prebattle_shipstats();
			prebattle_shipnavigation(1);

			draw_prebattle_seastats();
			prebattle_seanavigation(3);

			
		}

		ships_number = ships_count[0] + ships_count[1] + ships_count[2] + ships_count[3];
		if (ships_number == 0) { ships_number++; ships_count[0] = 1; }

		fleet_hp = (ships_count[0] * 2) + (ships_count[1] * 3) + (ships_count[2] * 4) + (ships_count[3] * 5) + 5;
		taken_points = new COORD[fleet_hp];

		//Tworzenie tablicy gracza

		char** ary = new char*[MAXseaY]; // rezerwowanie miejsca na sea
		for (int i = 0; i < MAXseaY; ++i)
			ary[i] = new char[MAXseaX];

		sea = ary; // przepisanie wskaznika do wskaznika w klasie
		for (int j = 0; j < MAXseaY; j++) // inicjacja sea
		{
			for (int i = 0; i < MAXseaX; i++) sea[j][i] = ' '; 
			cout << endl;
		}
		//-----------------------

		//Tworzenie tablicy przeciwnika
		char** ary_e = new char*[MAXseaY]; // rezerwowanie miejsca na sea
		for (int i = 0; i < MAXseaY; ++i)
			ary_e[i] = new char[MAXseaX];

		enemy_sea = ary_e; // przepisanie wskaznika do wskaznika w klasie
		for (int j = 0; j < MAXseaY; j++) // inicjacja sea
		{
			for (int i = 0; i < MAXseaX; i++) enemy_sea[j][i] = '@';
			cout << endl;
		}
		//-----------------------

		ClearScreen();

	};

	battlesea(); // konstr.

};
battlesea::battlesea() { 

	for (int i = 0; i < 4; i++)ships_count[i] = 1; 

}

//------------------------------------

//KLASA DO OBSLUGI PRZEBIEGU ROZGRYWKI

class gameconnection {


private:
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int MAX_log_size = 6;

	int oX_logs;
	int oY_logs = 1;

	bool verify_victory(battlesea * current_player_sea , ships * enemy_player_ships , battlesea * enemy_player_sea) {

		if ((*enemy_player_ships).sinked_ships == (*current_player_sea).ships_number) {
			
			ClearScreen();
			COORD vic_cord;
			vic_cord.X = 0;
			vic_cord.Y = 0;

			//Wyswietl ekrany graczy
			display_ally_sea(current_player_sea, enemy_player_sea , false);
			display_ally_sea(enemy_player_sea, current_player_sea);

			cout << "\n\nWcisnij dowolny przycisk aby zakonczyc" << endl;
			SetConsoleCoords(vic_cord);
			cout << "ZWYCIEZYL GRACZ - " << current_player_sea->player_name << "!" << endl;
			_getch();
			exit(0);
			return true;
		}

		return false;
	}
	
	bool sink(ships ship, int ship_number) {

		for (int i = 0; i < ship.fleet[ship_number].hp; i++)
			if (ship.fleet[ship_number].ship_hp[i] == true) return false;

		return true;

	}

	void shot_result(COORD shot, battlesea * current_player_sea , battlesea * enemy_player_sea , ships * current_player_ship , ships * enemy_player_ship) {

		bool result = false;
		string buf_s;
		stringstream ss;

		for (int i = 0; i < (*enemy_player_sea).fleet_hp; i++) {

			if (shot.X == (*enemy_player_sea).taken_points[i].X && shot.Y == (*enemy_player_sea).taken_points[i].Y) result = true; // TRAFIONO

		}
		
		switch (result) {

		case true: {
		
			(*current_player_sea).enemy_sea[shot.Y][shot.X] = 'X';  
			(*enemy_player_sea).sea[shot.Y][shot.X] = 'X';

			for (int i = 0; i < (*enemy_player_sea).ships_number; i++) { // Sprawdz kazdy statek
				for (int j = 0; j < (*enemy_player_ship).fleet[i].hp; j++) // Sprawdz kazdy punkt statku (do punktu trafienia)
					if (shot.X == (*enemy_player_ship).fleet[i].ship_body[j].X && shot.Y == (*enemy_player_ship).fleet[i].ship_body[j].Y) {
						(*enemy_player_ship).fleet[i].ship_hp[j] = false; // strzel

						if (sink(*enemy_player_ship, i) == true) {
							(*enemy_player_ship).sinked_ships++;
							ss << "ZATOPIONO~STATEK~~";
							ss << "[" << shot.X << ":" << (char)(shot.Y + 65) << "]HP=" << (*enemy_player_ship).fleet[i].hp;
							ss >> buf_s;
							(*current_player_sea).log_que.push(buf_s); 
							buf_s = "";
							for (int l = 0; l < (*enemy_player_ship).fleet[i].hp; l++) {

								(*current_player_sea).enemy_sea[(*enemy_player_ship).fleet[i].ship_body[l].Y][(*enemy_player_ship).fleet[i].ship_body[l].X] = '^'; // Gdy zatopiono zmien ikonke statku

							};

							return;
						}

						ss << "TRAFIONO~~";
						ss << "[" << shot.X << ":" << (char)(shot.Y + 65) << "]";
						ss >> buf_s; ss.clear(); ss.sync();
						(*current_player_sea).log_que.push(buf_s); 
						buf_s = "";

						return;
					} 
			}
			return;
	
		}

		case false: {

		(*current_player_sea).enemy_sea[shot.Y][shot.X] = ' ';
		ss << "PUDLO~~";
		ss <<"["<< shot.X << ":" << (char)(shot.Y + 65) << "]";
		ss >> buf_s;
		(*current_player_sea).log_que.push(buf_s); //log_ctr++;
		buf_s = "";

		return;

		}

		};

	};

	void display_ally_sea(battlesea * current_player, battlesea * enemy_player , bool apply_offset = true) {

		int oX_ally_sea = 12 + current_player->MAXseaX;
		if (apply_offset == false) oX_ally_sea = 0;

		(*current_player).curs_def.X = 0 ;
		(*current_player).curs_def.Y = 0;
		char temp_c = 'A';
		
		SetConsoleCoords((*current_player).curs_def);

		move_cursor_offset_X(oX_ally_sea); cout << endl;
		move_cursor_offset_X(oX_ally_sea);
		for (int j = 0; j < (*current_player).MAXseaX + 1; j++) cout << "-";  move_cursor_offset_X(oX_ally_sea); cout << endl;
		for (int j = 0; j < (*current_player).MAXseaY; j++)
		{
			move_cursor_offset_X(oX_ally_sea);
			cout << "|";
			for (int i = 0; i < (*current_player).MAXseaX; i++) {
				if ((*enemy_player).enemy_sea[j][i] == 'X') SetConsoleTextAttribute(hConsole, 12); // Jezeli wykryto czesc statku ktory jeszcze zyje zmien kolor na czerwony
				if ((*enemy_player).enemy_sea[j][i] == '^') SetConsoleTextAttribute(hConsole, 9);// Jezeli wykryto zatopiony statek zmien kolor na czerwony				
				
				cout << (*current_player).sea[j][i];
				SetConsoleTextAttribute(hConsole, 7);
			}

			
			cout << "|" << temp_c++;
			cout << endl;
		}
		move_cursor_offset_X(oX_ally_sea);

		int tempi = (*current_player).MAXseaX + 1 - (4 + current_player->player_name.length());
		if (!(tempi < 0)) {
		cout << "----" << current_player->player_name;
		for (int j = 0; j < tempi ; j++) cout << "-";
		}
		else { for (int i = 0; i < (*current_player).MAXseaX+2; i++) cout << current_player->player_name[i]; }

	}

	void display_enemy_sea(battlesea * current_player , battlesea * enemy_player_sea) {

		(*current_player).curs_def.X = 0;
		(*current_player).curs_def.Y = 0;
		char temp_c = 'A';

		SetConsoleCoords((*current_player).curs_def);
		 
		cout << "Ruch gracza - " << (*current_player).player_name; if ((*current_player).PLAYER_CHEATMODE == true) { SetConsoleTextAttribute(hConsole, 2); cout << " CHEATMODE - Oszukiwac w statkach... niewiarygodne...\n";   SetConsoleTextAttribute(hConsole, 7); } else {cout <<endl;}
		for (int j = 0; j < (*current_player).MAXseaX + 1; j++) cout << "-"; cout << endl;
		for (int j = 0; j < (*current_player).MAXseaY; j++)
		{
			cout << "|";
			for (int i = 0; i < (*current_player).MAXseaX; i++) {
				if((*current_player).enemy_sea[j][i] == 'X') SetConsoleTextAttribute(hConsole, 12); // Jezeli wykryto czesc statku ktory jeszcze zyje zmien kolor na czerwony
				if ((*current_player).enemy_sea[j][i] == '^') SetConsoleTextAttribute(hConsole, 9);// Jezeli wykryto zatopiony statek zmien kolor na czerwony				
				if ((*current_player).PLAYER_CHEATMODE == true) { cout <<(*enemy_player_sea).sea[j][i] ; }else{   cout << (*current_player).enemy_sea[j][i];}
				SetConsoleTextAttribute(hConsole, 7);
				}
				

			cout << "|" << temp_c++;
			cout << endl;
		}

		int tempi = (*current_player).MAXseaX + 1 - (4 + enemy_player_sea->player_name.length());
		if (!(tempi < 0)) {
			cout << "----" << enemy_player_sea->player_name;
			for (int j = 0; j < tempi; j++) cout << "-";
		}
		else { for (int i = 0; i < (*current_player).MAXseaX+2; i ++) cout << current_player->player_name[i] ; }


	}

	COORD sea_aim(battlesea * current_player_sea, battlesea * enemy_player_sea) {


		COORD curXY;
		COORD retXY;

		curXY.X = (*current_player_sea).MAXseaX / 2 + 1;
		curXY.Y = (*current_player_sea).MAXseaY / 2 + 2;//curs_def;

		char wyb;
		bool col_flag = false;

		SetConsoleCoords(curXY);
		cout << "+";

		retXY.X = 0;
		retXY.Y = 3 + current_player_sea->MAXseaY ;
		SetConsoleCoords(retXY);
		move_cursor_offset_XY(oX_logs,oY_logs);
		cout << "|Wspolrzedne punktu: [" << curXY.X - 1 << ":" << (char)(curXY.Y - 2 + 65) << "]"; for (int i = 0; i < current_player_sea->MAXseaX + 1 - curXY.X; i++) cout << " ";



		while (1) {
			while ((wyb = _getch()) != 13) {   //Menu strzalkowe

				if (wyb == 72) { // STRZALKA W GORE

					if (curXY.Y == 2) {
						/*nic nie rob;*/ continue;
					}

					display_enemy_sea(current_player_sea, enemy_player_sea);
					curXY.Y--;
					SetConsoleCoords(curXY);
					cout << "+";
				};


				if (wyb == 80) { // STRZALKA W DOL

					if (curXY.Y == (*current_player_sea).MAXseaY +1) {
						/*nic nie rob;*/ continue;
					}

					display_enemy_sea(current_player_sea, enemy_player_sea);
					curXY.Y++;
					SetConsoleCoords(curXY);
					cout << "+" ;
					
				};


				if (wyb == 75) { // STRZALKA W LEWO

					if (curXY.X == 1) {
						/*nic nie rob;*/ continue;
					}

					display_enemy_sea(current_player_sea, enemy_player_sea);
					curXY.X--;
					SetConsoleCoords(curXY);
					cout << "+";
					


				};


				if (wyb == 77) { // STRZALKA W PRAWO

					if (curXY.X == (*current_player_sea).MAXseaX) {
						/*nic nie rob;*/ continue;
					}

					display_enemy_sea(current_player_sea, enemy_player_sea);
					curXY.X++;
					SetConsoleCoords(curXY);
					cout << "+";

				};


				if (wyb == 99) { // ZNAK c - CHEATMODE

					if (current_player_sea->PLAYER_CHEATMODE == true) continue;

					current_player_sea->PLAYER_CHEATMODE = true;
					display_enemy_sea(current_player_sea, enemy_player_sea);
					SetConsoleCoords(curXY);
					cout << "+";
					continue;
				};

				retXY.X = 0;
				retXY.Y = 3 + current_player_sea->MAXseaY;
				SetConsoleCoords(retXY);
				move_cursor_offset_XY(oX_logs,oY_logs);
				cout << "|Wspolrzedne punktu: [" << curXY.X - 1 << ":" << (char)(curXY.Y - 2 + 65) << "]"; for (int i = 0; i < current_player_sea->MAXseaX +1 - curXY.X; i++) cout << " ";



			}

			retXY.X = curXY.X - 1;
			retXY.Y = curXY.Y - 2;

			//Ktore pola beda ignorowane przy strzale (zakomentowane mozna strzelac ponownie)
			if ((*current_player_sea).enemy_sea[retXY.Y][retXY.X] == ' ' || (*current_player_sea).enemy_sea[retXY.Y][retXY.X] == '^' || (*current_player_sea).enemy_sea[retXY.Y][retXY.X] == 'X') continue; // Jezeli puste miejsce (strzelono nie pozwol strzelic ponownie)


			return retXY;
		}



	}
	
	COORD OPTIMALIZATION_find_coords(battlesea current_player_sea) {

		bool tempb = true;
		int temp_ile_razy = 0;
		COORD point;
		point.X = -1; point.Y = -1;
		int recog_ship_type = -1;

		for (int i = 0; i < current_player_sea.MAXseaY; i++) {
			for (int j = 0; j < current_player_sea.MAXseaX; j++) { // Znajdz na planszy przeciwnika znak X
				if (current_player_sea.enemy_sea[i][j] == 'X') {
					//cout << "ZNALEZIONO X = [" << i << ":" << j << "]" << endl; system("pause");
					point.X = j;
					point.Y = i;
					return point;
				}
			}
		}
		return point;
	}

	int OPTIMALIZATION_find_type(COORD point, battlesea * current_player_sea) {

		int y = point.Y;
		int x = point.X;

		//Sprawdz czy wykryto typ 1
		if (x != 0) {

			if (x != current_player_sea->MAXseaX - 1) { if (current_player_sea->enemy_sea[y][x - 1] == 'X' || current_player_sea->enemy_sea[y][x + 1] == 'X') return 1; }
			else { if (current_player_sea->enemy_sea[y][x - 1] == 'X') return  1; }

		}
		else { if (current_player_sea->enemy_sea[y][x + 1] == 'X') return 1; }


		//Sprawdz czy wykryto typ 3

		if (y != 0) {

			if (y != current_player_sea->MAXseaY - 1) { if (current_player_sea->enemy_sea[y - 1][x] == 'X' || current_player_sea->enemy_sea[y + 1][x] == 'X') return 3; }
			else { if (current_player_sea->enemy_sea[y - 1][x] == 'X') return 3; }

		}
		else { if (current_player_sea->enemy_sea[y + 1][x] == 'X') return 3; }


		//Sprawdz czy wykryto typ 2

		if (x != 0 && y != 0) { // Nigdy nie powinno przejsc (Jezeli mozesz isc w gore) - dodatkowe bezpieczenstwo
			if (current_player_sea->enemy_sea[y - 1][x - 1] == 'X') return 2;
		};


		if ((x != current_player_sea->MAXseaX - 1) && (y != current_player_sea->MAXseaY - 1)) { // Jezeli mozna isc w dol
			if (current_player_sea->enemy_sea[y + 1][x + 1] == 'X') return 2;
		};


		//Sprawdz czy wykryto typ 4

		if ((x != current_player_sea->MAXseaX - 1 ) && (y != 0) ) { // Jezeli mozna isc w gore
			if (current_player_sea->enemy_sea[y - 1][x + 1] == 'X') return 4;
		};


		if ((x != 0) && (y != current_player_sea->MAXseaY - 1)) { // Jezeli mozna isc w dol
			if (current_player_sea->enemy_sea[y + 1][x - 1] == 'X') return 4;
		};

		//Jezeli nie ma typu zwroc 0
		return 0;

	}

	COORD OPTIMALIZATION(battlesea * current_player_sea) {

		bool tempb = true;
		int temp_ile_razy = 0;
		COORD point;
		int recog_ship_type = -1;

		// WYSZUKIWANIE PUNKTU

		point = OPTIMALIZATION_find_coords(*current_player_sea);
		if (point.Y == -1 && point.X == -1) return point;
					

		// WYSZUKIWANIE TYPU STATKU
		recog_ship_type = OPTIMALIZATION_find_type(point, current_player_sea);



		switch (recog_ship_type) {

		case 1: {
			//cout << "WYKRYTO TYP 1  [" << point.X << ":" << point.Y << "] " << endl; system("pause");
			if (!(point.X - 1 < 0)) { if (current_player_sea->enemy_sea[point.Y][point.X - 1] == '@' ) { point.X--; return point; } }
			if (point.X + 1 !=  current_player_sea->MAXseaX) {
				if (current_player_sea->enemy_sea[point.Y][point.X + 1] == '@') { point.X++; return point; }
				else {
					if (current_player_sea->enemy_sea[point.Y][point.X + 1] == 'X') { while (current_player_sea->enemy_sea[point.Y][point.X + 1] != '@')point.X++; point.X++; return point; }
				
				}
			}
		}



		case 3: {
			//cout << "WYKRYTO TYP 3  [" << point.X << ":" << point.Y << "] " << endl; system("pause");
			if (!(point.Y - 1 < 0)) { if (current_player_sea->enemy_sea[point.Y-1][point.X] == '@') { point.Y--; return point; } }
			if (point.Y + 1 != current_player_sea->MAXseaY) {
				if (current_player_sea->enemy_sea[point.Y + 1][point.X] == '@') { point.Y++; return point; }
				else {
					if (current_player_sea->enemy_sea[point.Y+1][point.X] == 'X') { while (current_player_sea->enemy_sea[point.Y+1][point.X] != '@')point.Y++; point.Y++; return point; }

				}
			}
		}




		case 2: {
			//cout << "WYKRYTO TYP 2  [" << point.X << ":" << point.Y << "] " << endl; system("pause");
			if (point.X != 0 && point.Y != 0) { if (current_player_sea->enemy_sea[point.Y - 1][point.X - 1] == '@') { point.Y--; point.X--; return point; } } // Jezeli mozesz isc w gore

			if ((point.X != current_player_sea->MAXseaX - 1) && (point.Y != current_player_sea->MAXseaY - 1)) {

				if (current_player_sea->enemy_sea[point.Y + 1][point.X + 1] == '@') { point.Y++; point.X++; return point; }

				while (1) {
					if ((point.X != current_player_sea->MAXseaX - 1) && (point.Y != current_player_sea->MAXseaY - 1)) {
						if (current_player_sea->enemy_sea[point.Y + 1][point.X + 1] == '@') { point.Y++; point.X++; return point; } // Jezeli jest @ (mozna strzelac zwroc ten pukt
						else { point.Y++; point.X++; }// Jezeli jest tam X przejdz dalej

					} else { break; }
				}
			}
		}



		case 4: {
			//cout << "WYKRYTO TYP 4  [" << point.X << ":" << point.Y <<"] "<< endl; system("pause");
			if ((point.X != current_player_sea->MAXseaX - 1) && (point.Y != 0)) { if (current_player_sea->enemy_sea[point.Y - 1][point.X + 1] == '@') { point.Y--; point.X++; return point; } } // Jezeli mozesz isc w gore

			if ((point.X != 0) && (point.Y != current_player_sea->MAXseaY - 1)) {

				if (current_player_sea->enemy_sea[point.Y + 1][point.X - 1] == '@') { point.Y++; point.X--; return point; }

				while (1) {
					if ((point.X != 0) && (point.Y != current_player_sea->MAXseaY - 1)) {
						if (current_player_sea->enemy_sea[point.Y + 1][point.X - 1] == '@') { point.Y++; point.X--; return point; } // Jezeli jest @ (mozna strzelac zwroc ten pukt
						else { point.Y++; point.X--; }// Jezeli jest tam X przejdz dalej
					} else { break; }
				}

			}

		}



		default: {
		//Jezeli nie wykryto typu statku strzelaj wokol punktu

		//Okreslanie limitu strzelania
			int temp_max_i = 1; int temp_max_j = 1;
			int i = 0; int j = 0;

			if (!(point.X - 1 < 0)) i = -1; // Sprawdz lewy limit
			if (!(point.Y - 1 < 0)) j = -1; // Sprawdz gorny limit

			if (point.X + 1 != current_player_sea->MAXseaX) temp_max_i = 2; // Sprawdz prawy limit
			if (point.Y + 1 != current_player_sea->MAXseaY) temp_max_j = 2; // Sprawdz dolny limit

			int j_buf = j;

			for (i; i < temp_max_i; i++) {
				for (j; j < temp_max_j; j++) {

					if (current_player_sea->enemy_sea[point.Y + j][point.X + i] == '@') { point.X += i; point.Y += j; return point; }
				}
				j = j_buf;
			}
		}
			}


		point.X = -1;
		point.Y = -1;
		return point;


	}

	COORD sea_aim_auto(battlesea * current_player_sea, battlesea * enemy_player_sea) {

		COORD curXY;
		COORD retXY;
		

		curXY.X = (*current_player_sea).MAXseaX / 2 + 1;
		curXY.Y = (*current_player_sea).MAXseaY / 2 + 2;

		bool col_flag = false;

		SetConsoleCoords(curXY);
		cout << "+";

		retXY.X = 0;
		retXY.Y = 3 + current_player_sea->MAXseaY;
		SetConsoleCoords(retXY);
		move_cursor_offset_XY(oX_logs, oY_logs);
		cout << "|Wspolrzedne punktu: [" << curXY.X - 1 << ":" << (char)(curXY.Y - 2 + 65) << "]"; for (int i = 0; i < current_player_sea->MAXseaX + 1 - curXY.X; i++) cout << " ";

		// MAGIA AUTOMATYCZNEGO STRZELANIA

		while (1) {
		retXY = OPTIMALIZATION(current_player_sea);
		if (retXY.X >= 0 && retXY.Y >= 0) { curXY.X = retXY.X + 1; curXY.Y = retXY.Y + 2; break; };

		curXY.X = (rand() % (enemy_player_sea->MAXseaX) + 0) + 1;  // Losowanie punku statku (offset plus o 1 wieksza dla losowania)
		curXY.Y = (rand() % (enemy_player_sea->MAXseaY) + 0) + 2;

		retXY.X = curXY.X - 1; // Bez offsetow
		retXY.Y = curXY.Y - 2;

		if ((*current_player_sea).enemy_sea[retXY.Y][retXY.X] == ' ' || (*current_player_sea).enemy_sea[retXY.Y][retXY.X] == '^' || (*current_player_sea).enemy_sea[retXY.Y][retXY.X] == 'X') continue; // Jezeli puste miejsce (strzelono nie pozwol strzelic ponownie)
		


		break;
	    }

		display_enemy_sea(current_player_sea, enemy_player_sea);

		//curXY.X = retXY.X + 1;
		//curXY.Y = retXY.Y + 2;

		SetConsoleCoords(curXY);
		cout << "+";


		curXY.X = 0;
		curXY.Y = 3 + current_player_sea->MAXseaY;
		SetConsoleCoords(curXY);
		move_cursor_offset_XY(oX_logs, oY_logs);

		cout << "|Wspolrzedne punktu: [" << retXY.X  << ":" << (char)(retXY.Y + 65) << "]"; for (int i = 0; i < current_player_sea->MAXseaX + 1 - retXY.X; i++) cout << " ";


		return retXY;
		}

	void draw_logs(battlesea * current_player_sea , ships * current_player_ship, ships * enemy_player_ship) {

		string buf_s;

		COORD temp;
		temp.X = 0;
		temp.Y = 3 + current_player_sea->MAXseaY +1;

		SetConsoleCoords(temp); move_cursor_offset_XY(oX_logs, oY_logs);

		for (int j = 0; j < (*current_player_sea).MAXseaX + 1; j++) cout << "-";
		cout << endl; move_cursor_offset_X(oX_logs);
		cout << "|Zatopione wrogie statki: " << (*enemy_player_ship).sinked_ships << "/" << (*current_player_sea).ships_number << endl; move_cursor_offset_X(oX_logs);
		cout << "|Zatopione sojusznicze statki: " << (*current_player_ship).sinked_ships << "/" << (*current_player_sea).ships_number << endl; move_cursor_offset_X(oX_logs);

		for (int j = 0; j < (*current_player_sea).MAXseaX + 1; j++) cout << "-";
		cout << endl;
		move_cursor_offset_X(oX_logs); cout << "|LOGI:" << endl;

		temp.X = 0;
		temp.Y = 3 + current_player_sea->MAXseaY +2 + 4;
		SetConsoleCoords(temp);
		move_cursor_offset_XY(oX_logs, oY_logs);


		for (int i = 0; i < ((int) (*current_player_sea).log_que.size()); i++) {
			cout <<">"<< (*current_player_sea).log_que.front() << endl;
			move_cursor_offset_X(oX_logs);
			buf_s = (*current_player_sea).log_que.front();

			(*current_player_sea).log_que.pop(); //shufle
			(*current_player_sea).log_que.push(buf_s);

		}
	
		if ( ((int) (*current_player_sea).log_que.size()) >= MAX_log_size)  for (int i = 0; i < MAX_log_size; i++) { if (i + 1 == MAX_log_size) { buf_s = (*current_player_sea).log_que.front();(*current_player_sea).log_que.push(buf_s); } (*current_player_sea).log_que.pop(); } // Jezeli przekroczono max wiadomosci wyczysc kolejke zachowujac ostatni element

	}

	public:

	void main_game_interface(battlesea * player1_sea, battlesea * player2_sea ,ships * player1_ships , ships * player2_ships , bool PC_player1 , bool PC_player2) {

		ShowConsoleCursor(false); // Dodatkowe wylaczenie kursora(ochrona przed manipulacja ekranu)
		
		
		while (1) {
			ShowConsoleCursor(false);
			//TURA GRACZA 1

			ClearScreen();

			display_enemy_sea(player1_sea, player2_sea);
			display_ally_sea(player1_sea, player2_sea);
			//system("pause");

			draw_logs(player1_sea, player1_ships, player2_ships);

			if (PC_player1 == false) { shot_result(sea_aim(player1_sea, player2_sea), player1_sea, player2_sea, player1_ships, player2_ships); }
			else { shot_result(sea_aim_auto(player1_sea, player2_sea), player1_sea, player2_sea, player1_ships, player2_ships); }


			draw_logs(player1_sea, player1_ships, player2_ships);
			verify_victory(player1_sea,player2_ships , player2_sea);

			Sleep(1500);



			//TURA GRACZA 2
			ClearScreen();
			display_enemy_sea(player2_sea, player1_sea);
			if(( PC_player1 == true && PC_player2 == true) || ((PC_player1 != true && PC_player2 != true)))
				display_ally_sea(player2_sea, player1_sea);

			draw_logs(player2_sea, player2_ships, player1_ships);

			if (PC_player2 == false) { shot_result(sea_aim(player2_sea, player1_sea), player2_sea, player1_sea, player2_ships, player1_ships); }
			else { shot_result(sea_aim_auto(player2_sea, player1_sea), player2_sea, player1_sea, player2_ships, player1_ships); }

			draw_logs(player2_sea, player2_ships, player1_ships);
			verify_victory(player2_sea, player1_ships , player1_sea);

			Sleep(1500);



		}
		


		
	}




	gameconnection(int MAXseaX);


};

gameconnection::gameconnection(int MAXseaX) {

};

//------------------------------------

//KLASA DO OBSLUGI MENU GLOWNEGO

class menu { // Klasa menu glownego
public:
	int option;
	COORD curs_pos;
	COORD curXY;


	void author() {
		ClearScreen();
		char c;
		cout << "--------------------------------\n";
		cout << "Program - Statki \n";
		cout << "Autor: Sebastian Jozwiak grupa I6X6S1 2017 v1.0\n";
		cout << "Wojskowa Akademia Techniczna - Wydzial Cybernetyki\n";
		cout << "Napisano na potrzeby zajec labolatoryjnych z przedmiotu \"Programowanie Obiektowe\" \n";
		cout << "--------------------------------\n";
		cout << "\nWcisnij dowolny przycisk aby wrocic..." << endl;
		c = getchar();

	}

	void instruction() {
		ClearScreen();
		char c;
		cout << "--------------------------------\n";
		cout << "Instrukcja\n";
		cout << "--------------------------------\n";
		cout << "Dostepne tryby: PVP PVPc oraz PcVPc " << endl;
		cout << "Uzytkownik sam musi zadbac o poprawne dopasowanie ilosci statkow do rozmiaru planszy" << endl;
		cout << "W przypadku gdy nie wybrano zadnego statku program poda automatycznie 2 masztowiec" << endl;
		cout << "Bledne wypelnienie planszy skutkuje zapetleniem sie programu (tryb auto) lub uniemozliwa polozenie statku\n" << endl;
		cout << "Podczas rozmieszczania statkow uzytkownik jest informowany o mozliwosci polozenia statku w danym miejscu poprzez kolory:" << endl;
		cout << "Zielony - mozna polozyc" << endl;
		cout << "Czerwony - nie mozna polozyc (wykorzystano statki tego typu lub wystepuja kolizje)\n" << endl;
		cout << "Sterowanie statkami podczas rozmieszczania:\n";
		cout << "-/+ - Zwieksz / zmniejsz rozmiar statku\n";
		cout << "r - Obroc statek\n";
		cout << "a - Automatyczne ustawienie statkow\n";
		cout << "strzalki - poruszanie statkiem \n" << endl;
		cout << "Podczas rozgrywki nastepujace kolory informuja uzytkownika o statusie statku:\n";
		cout << "Niebieski - zatopiony" << endl;
		cout << "Czerwony - czesc statku trafiona ale nie zatopiona\n" << endl;
		cout << "Lewy ekran informuje uzytkownika o stanie wrogiego pola natomiast prawy o statusie jego wlasnego\n";
		cout << "W przypadku problemow z wyswietlaniem wiekszy rozmiarow planszy nalezy zwiekszyc bufor konsoli windowsa" << endl;
		cout << "CHEATMODE jest wlaczany podczas rozgrywki klawiszem - c" << endl;

		cout << "--------------------------------\n";
		cout << "\nWcisnij dowolny przycisk aby wrocic..." << endl;
		c = getchar();

	}

	void draw_main_menu() {
		ClearScreen();
		cout << "Gra STATKI" << endl;
		for (int j = 0; j < 14; j++) cout << "~"; cout << endl;
		cout << "Menu Glowne:" << endl;
		cout << ">1. Nowa Gra" << endl;
		cout << " 2. Instrukcja" << endl;
		cout << " 3. Autor" << endl;

	};

	void draw_sub1_main_menu() {
		ClearScreen();
		cout << "Gra STATKI" << endl;
		for (int j = 0; j < 14; j++) cout << "~"; cout << endl;
		cout << "Wybierz tryb gry:" << endl;
		cout << ">1. P V P" << endl;
		cout << " 2. P v PC" << endl;
		cout << " 3. PC v PC [symulacja]" << endl;
		cout << " 4. Wstecz" << endl;

	};

	int menu_navigation(int limit = 0) { // Wybor opcji
		char wyb;
		option = 0;
		curXY = curs_pos; // reset menu
		SetConsoleCoords(curXY);


		while ((wyb = _getch()) != 13) {   //Menu strzalkowe
			if (wyb == 72) { if (option == 0)continue; cout << " "; curXY.Y--;   SetConsoleCoords(curXY); cout << ">";  SetConsoleCoords(curXY); option--; }
			if (wyb == 80) { if (option == 4 - limit)continue; cout << " "; curXY.Y++;   SetConsoleCoords(curXY); cout << ">";  SetConsoleCoords(curXY); option++; }
		};

		return option;

	}

	int menu_go() {

			draw_main_menu();
		
			switch (menu_navigation(2)) {

			case 0: {draw_sub1_main_menu(); return menu_navigation(1); };
			case 1: {instruction(); return 3; };
			case 2: {author(); return 3; };

			};
	}



	menu(); //konstruktor
};
menu::menu() {
	option = 0;

	curXY.X = 0;  curXY.Y = 3;

	curs_pos.X = 0; curs_pos.Y = 3; // pozycja menu glownego
};

//------------------------------


int main() {
	srand(time(NULL));
	ShowConsoleCursor(false);

	int option = 0;

	menu menu;


	while(1)
	{
		option = menu.menu_go();
		cout << "option " <<option<< endl;
		if (option != 3) break;
	}
	//Tworzenie graczy i wpisanie parametrów gry
	battlesea plansza_player1;
	battlesea plansza_player2;

	ClearScreen();
	plansza_player1.prepare_sea();

	ships statek_player1(plansza_player1.ships_number);
	//------------------
	ClearScreen();

	if (option != 2) {
		cout << "Wpisz imie gracza 1" << endl;
		cin >> plansza_player1.player_name;
	}
	else {
		plansza_player1.player_name = "Pc1";

	};

	if (option == 0) {
		cout << "Wpisz imie gracza 2" << endl;
		cin >> plansza_player2.player_name;
	}
	else {
		plansza_player2.player_name = "Pc2";

	};

	//Skopiowanie ustawien z planszy 1

	plansza_player2.MAXseaX = plansza_player1.MAXseaX;
	plansza_player2.MAXseaY = plansza_player1.MAXseaY;
	for (int i = 0; i < 5; i ++)plansza_player2.ships_count[i] = plansza_player1.ships_count[i];
	plansza_player2.prepare_sea(true);
	ships statek_player2(plansza_player2.ships_number);

	//-----------------

	//Ustawienie flag AI graczy
	bool is_player1_pc = false; bool is_player2_pc = false;
	switch (option) {
	case 1: {is_player1_pc = false; is_player2_pc = true;  break; };
	case 2: {is_player1_pc = true; is_player2_pc = true;  break; };

	}

	plansza_player1.main_setup(statek_player1, is_player1_pc); // Rozstawienie statkow przez 1 gracza
	ClearScreen();
	plansza_player2.main_setup(statek_player2, is_player2_pc); // rozstawienie statkow przez 2 gracza
	//ClearScreen();
	gameconnection thegame(plansza_player1.MAXseaX);
	thegame.main_game_interface(&plansza_player1, &plansza_player2 , &statek_player1 , &statek_player2, is_player1_pc , is_player2_pc); // uruchomienie gry


	//system("pause");
	return 0;
}