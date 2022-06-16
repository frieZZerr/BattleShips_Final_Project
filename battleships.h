#pragma once

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <string>
#include <unistd.h>
#include <assert.h>

#define SIZE 10

using namespace std;

class Coordinates {

	char x;
	int y;

public:

//	Konstruktor
	Coordinates( char X, int Y ) {

		x = X;
		y = Y;
	}

	Coordinates() {}

	int getX() {							//	Wspolrzedna X

		if( (int)x >= 97 && (int)x <= 106 )
			return (int)x - 97;
		else
			return (int)x - 65;

	}

	int getY() { return y; }				//	Wspolrzedna Y

	void setX( char X ) { x = X; }
	void setY( int Y  ) { y = Y; }

	Coordinates& operator= ( const Coordinates& cords ) {

		x = cords.x;
		y = cords.y;

		return *this;
	}
};

class Ship {

	vector<int> x;
	vector<int> y;
	int size;
	int hit;
	int way;
	string name;

public:

//	Konstruktor
	Ship( vector<int> X, vector<int> Y, int size_, string name_, int way_ ) {

		x    = X;
		y    = Y;
		size = size_;
		name = name_;
		way  = way_;
		hit  = 0;
	}

//	----------	Gettery	-----------
	int getSize() { return size; }
	int getWay()  { return way;  }

	vector<int> getX() { return x; }
	vector<int> getY() { return y; }

	string getName() { return name; }
//	-------------------------------

//	Sprawdzanie czy dany statek zostal zatopiony
	bool isSunk() { return hit >= size; }

//	Trafienie statku
	void getShot() { hit++; }

//	Sprawdz nazwe statku na danych koordynatach
	string getShip( Coordinates c ) {

		for( int i = 0; i < size; i++ ) {
			if( x.at(i) == c.getX() && y.at(i) == c.getY() )
				return name;
		}

		return "";
	}
};

class Board {

	char playerSquare[SIZE][SIZE];
	char enemySquare[SIZE][SIZE];
	char enemySquareShown[SIZE][SIZE];

	bool playing = true;
	bool didSunk = false;

	vector<Ship> playerShipsVector;
	vector<Ship> enemyShipsVector;

	string status = "";

	Coordinates playerSeaMine;
	Coordinates enemySeaMine;

public:

//	Konstruktor
	Board() {

//		"Zerowanie" tablic
		for( int i = 0; i < SIZE; i++ )
			for( int j = 0; j < SIZE; j++ ) {
				playerSquare[i][j] 	   = ' ';
				enemySquare[i][j]  	   = ' ';
				enemySquareShown[i][j] = ' ';
			}

		makeShips( playerSquare, playerShipsVector, playerSeaMine );	//	Ustawianie statkow i miny morskiej dla gracza
		makeShips( enemySquare,  enemyShipsVector,  enemySeaMine  );	//	Ustawianie statkow i miny morskiej dla AI
	}

//	Tworzenie statkow
	void makeShips( char square[SIZE][SIZE], vector<Ship> &vec, Coordinates &seaMine ) {

//		Tablica przechowujaca zajete pozycje
		bool placedShips[SIZE][SIZE];
		bool whereShips[SIZE][SIZE];
		for( int i = 0; i < SIZE; i++ )
			for( int j = 0; j < SIZE; j++ ) {

				placedShips[i][j] = false;
				whereShips[i][j]  = false;
			}

//		Najwiekszy rozmiar statku to 4 pola
		for( int shipNumber = 4; shipNumber > 0; shipNumber-- ) {

//			Ilosc statkow danego rozmiaru
			int howManyShips = 5 - shipNumber;

//			Ustawianie nazwy statku w zaleznosci od jego rozmiaru
			string shipName = setShipName( shipNumber );

			bool good = true;
			while( good ) {

				int whichWay = rand() % 2;

				if( whichWay == 0 ) {	//	horizontal

//					Losowe koordynaty
					char X = 'A' + rand() % ( SIZE-shipNumber );
					int  Y = rand() % SIZE;
					Coordinates cords( X, Y );

//					Sprawdzanie czy na tym polu mozna postawic statek o danym rozmiarze
					if( canPlace( placedShips, cords, shipNumber, whichWay ) ) {

//						Tworzenie statku
						vector<int> vecX;
						vector<int> vecY;
						for( int i = cords.getX(); i < cords.getX()+shipNumber; i++ ) {
							vecX.push_back(i);
							vecY.push_back(Y);
						}
						string name_ = shipName + to_string( howManyShips );
						Ship ship( vecX, vecY, shipNumber, name_, whichWay );

//						Polozenie statku na planszy
						placeShip( square, cords, shipNumber, whichWay, whereShips );
						vec.push_back( ship );

//						Jesli utworzono potrzebna ilosc statkow --> skoncz ich tworzenie
						howManyShips--;
						if( howManyShips == 0 )
							good = false;
					}
				}
				else {					//	vertical

//					Losowe koordynaty
					char X = 'A' + rand() % SIZE;
					int  Y = rand() % ( SIZE-shipNumber );
					Coordinates cords( X, Y );

//					Sprawdzanie czy na tym polu mozna postawic statek o danym rozmiarze
					if( canPlace( placedShips, cords, shipNumber, whichWay ) ) {

//						Tworzenie statku
						vector<int> vecX;
						vector<int> vecY;
						for( int i = Y; i < Y+shipNumber; i++ ) {
							vecX.push_back( cords.getX() );
							vecY.push_back(i);
						}
						string name_ = shipName + to_string( howManyShips );
						Ship ship( vecX, vecY, shipNumber, name_, whichWay );

//						Polozenie statku na planszy
						placeShip( square, cords, shipNumber, whichWay, whereShips );

//						Dodanie statku do wektora
						vec.push_back( ship );

//						Jesli utworzono potrzebna ilosc statkow --> skoncz ich tworzenie
						howManyShips--;
						if( howManyShips == 0 )
							good = false;
					}
				}
			}
		}

//		Generowanie min morskich
		generateSeaMine( square, whereShips, seaMine );
	}

//	Generowanie min morskich
	void generateSeaMine( char square[SIZE][SIZE], bool placed[SIZE][SIZE], Coordinates &seaMine ) {

		int quantity = 1;
		bool isPlaced;

		for( int i = 0; i < quantity; i++ ) {

			isPlaced = false;

			while( !isPlaced ) {

	//			Losowe koordynaty z wykluczeniem wybrzezy
				char X = 'B' + rand() % ( SIZE-1 );
				int  Y = rand() % ( SIZE-1 )+1;
				Coordinates cords( X, Y );

				if( !placed[ cords.getX() ][ cords.getY() ] ) {

					square[ cords.getX() ][ cords.getY() ] = 'X';
					
					seaMine.setX(X);
					seaMine.setY(Y);

					isPlaced = true;
				}
			}
		}
	}

//	Sprawdzenie czy na danych koordynatach mozna postawic statek o danym rozmiarze
//	FIXED -------------------------------------------
	bool canPlace( bool placed[SIZE][SIZE], Coordinates &cords, int shipSize, int way ) {

		int x = cords.getX();
		int y = cords.getY();

		if( way == 0 ) {		//	horizontal

			for( int i = x; i < x+shipSize; i++ ) {

				if( placed[i][y] )
					return false;
			}

			for( int i = x-1; i <= x+shipSize; i++ ) {

				if( i < 0 )
					i++;
				else if( i > SIZE-1 )
					break;
				else {

					placed[i][y] = true;

					if( y == 0 ) {
						placed[i][y+1] = true;
					}
					else if( y == SIZE-1 ) {
						placed[i][y-1] = true;
					}
					else {
						placed[i][y+1] = true;
						placed[i][y-1] = true;
					}
				}
			}
		}

		else {					//	vertical

			for( int i = y; i < y+shipSize; i++ ) {

				if( placed[x][i] )
					return false;
			}

			for( int i = y-1; i <= y+shipSize; i++ ) {

				if( i < 0 )
					i++;
				else if( i > SIZE-1 )
					break;
				else {

					placed[x][i] = true;

					if( x == 0 ) {
						placed[x+1][i] = true;
					}
					else if( x == SIZE-1 ) {
						placed[x-1][i] = true;
					}
					else {
						placed[x+1][i] = true;
						placed[x-1][i] = true;
					}
				}
			}
		}

		return true;
	}

//	Stawianie statku na planszy
	void placeShip( char square[SIZE][SIZE], Coordinates &cords, int shipSize, int way, bool ships[SIZE][SIZE] ) {
		
		int col = cords.getX();
		int row = cords.getY();
		char ship = shipSize + 48;

		if( way == 0 ) {		//	horizontal

			for( int i = 0; i < shipSize; i++ ) {

//				Statki sa wyswietlanie na planszy numerem swojego rozmiaru
				square[col][row] = ship;

//				Statki sa wyswietlane na planszy jako znak 'O'
//				square[col][row] = 'O';

				ships[col][row] = true;

				col++;
			}
		}
		else {					//	vertical

			for( int i = 0; i < shipSize; i++ ) {

//				Statki sa wyswietlanie na planszy numerem swojego rozmiaru
				square[col][row] = ship;

//				Statki sa wyswietlane na planszy jako znak 'O'
//				square[col][row] = 'O';

				ships[col][row] = true;

				row++;
			}
		}
	}

//	Ustawianie nazwy statku w zaleznosci od jego rozmiaru
	string setShipName( int shipSize ) {

		string name = "";

		switch( shipSize ) {
			case 4: {
				name = "Carrier";
			} break;
			case 3: {
				name = "Battleship";
			} break;
			case 2: {
				name = "Cruiser";
			} break;
			case 1: {
				name = "Destroyer";
			} break;
			default:
				break;
		}

		return name;
	}

//	Tworzenie losowych koordynatow do strzalu
	Coordinates randomShot() {

		char X = 'A' + rand() % SIZE;
		int Y  = rand() % SIZE;

		Coordinates shot( X, Y );

		return shot;
	}

//	Pobieranie wspolrzednych do strzalu
	Coordinates wait4move() {

		char x;
		int y;
		bool allGood = false;

		while( !allGood ) {

			cout << setw(3) << "Podaj wspolrzedne do strzalu: ";
			cin >> x >> y;
			cout << endl;

			if( ( ( (int)x >= 65 && (int)x <= 74 ) || ( (int)x >= 97 && (int)x <= 106 ) )
					&& y >= 1 && y <= 10 ) {
				allGood = true;
				break;
			}

			cout << "Podano nieprawidlowe wspolrzedne!" << endl << endl;
			assert( ( ( (int)x >= 65 && (int)x <= 74 ) || ( (int)x >= 106 && (int)x <= 97 ) )
						&& y >= 1 && y <= 10 ) ;
		}

		Coordinates shotCoord( x, y-1 );

		return shotCoord;
	}

//	Strzelanie w zadane koordynaty
//	FIXED -------------------------------------------
	void fire( Coordinates &shot, string turn ) {

		int x = shot.getX();
		int y = shot.getY();

		//test = shot;

//		Kolej gracza
		if( turn == "Player" ) {

//			Jesli zostala trafiona mina morska
			if( x == enemySeaMine.getX() && y == enemySeaMine.getY() ) {

//				Ustawianie statusu akcji
				status = "SeaMine";

				for( int i = x-1; i <= x+1; i++ ) {
					for( int j = y-1; j <= y+1; j++ ) {

						Coordinates cords( (char)i+65, j );

//						Sprawdzenie czy mine trafila jakis statek
						checkHitShips( enemySquareShown, enemyShipsVector, cords );
					}
				}

//				Zakonczenie tury
				playing = false;
			}
			else {

//				Sprawdzenie czy jakis statek zostal trafiony
				checkHitShips( enemySquareShown, enemyShipsVector, shot );

//				Kolejny strzal
				if( enemySquareShown[x][y] != 'x' )
					playing = false;
				else
					playing = true;
			}
		}

//		Kolej AI
		if( turn == "AI" ) {

//			Jesli zostala trafiona mina morska
			if( x == playerSeaMine.getX() && y == playerSeaMine.getY() ) {

//				Ustawianie statusu akcji
				status = "SeaMine";

				for( int i = x-1; i <= x+1; i++ ) {
					for( int j = y-1; j <= y+1; j++ ) {

						Coordinates cords( i, j );

//						Sprawdzenie czy mine trafila jakis statek
						checkHitShips( playerSquare, playerShipsVector, cords );
					}
				}

//				Zakonczenie tury
				playing = true;
			}
			else {

//				Sprawdzenie czy jakis statek zostal trafiony
				checkHitShips( playerSquare, playerShipsVector, shot );

//				Kolejny strzal
				if( playerSquare[x][y] != 'x' )
					playing = true;
				else
					playing = false;
			}
		}
	}

//	Sprawdzenie czy statek zostal trafiony
//	FIXED
	void checkHitShips( char square[SIZE][SIZE], vector<Ship> &ships, Coordinates &shot ) {

		int x = shot.getX();
		int y = shot.getY();
		string whatGotHit = "";

//		Szukanie czy statek zostal trafiony
		for( int i = 0; i < (int)ships.size(); i++ ) {

			whatGotHit = ships.at(i).getShip( shot );

//			Jesli tak to zapisuje nazwe tego statku
			if( whatGotHit != "" ) {

//				Ustawianie statusu akcji
				if( status != "SeaMine" )
					status = whatGotHit;

//				Statek otrzymuje "obrazenia"
				ships.at(i).getShot();

//				Zaktualizowanie planszy
				square[x][y] = 'x';

//				Sprawdzanie czy statek zostal zatopiony
				if( ships.at(i).isSunk() ) {

//					Zaktualizowanie planszy
					updateAfterSunk( square, ships.at(i) );

//					Usuniecie zatopionego statku z wektora
					ships.erase( ships.begin()+i );
					didSunk = true;
				}

				break;
			}
		}

//		Jesli nic nie zostalo trafione
		if( whatGotHit == "" ) {

//			Zaktualizowanie planszy
			square[x][y] = '.';
		}
	}

//	Aktualizowanie planszy po zatopieniu statku
	void updateAfterSunk( char square[SIZE][SIZE], Ship &ship ) {

		if( ship.getWay() == 0 ) {				//	horizontal

			int x = ship.getX().front();
			int y = ship.getY().front();

			for( int i = x-1; i <= x+ship.getSize(); i++ ) {

				if( i < -1 )
					i++;
				else if( i > SIZE-1 )
					break;
				else {

					if( i == x+ship.getSize() || i == x-1 )
						square[i][y] = '.';
					else 
						square[i][y] = 'x';

					if( y == 0 ) {
						square[i][y+1] = '.';
					}
					else if( y == SIZE-1 ) {
						square[i][y-1] = '.';
					}
					else {
						square[i][y+1] = '.';
						square[i][y-1] = '.';
					}
				}
			}
		}
		else {									//	vertical

			int x = ship.getX().front();
			int y = ship.getY().front();

			for( int i = y-1; i <= y+ship.getSize(); i++ ) {

				if( i < -1 )
					i++;
				else if( i > SIZE-1 )
					break;
				else {

					if( i == y+ship.getSize() || i == y-1 )
						square[x][i] = '.';
					else
						square[x][i] = 'x';

					if( x == 0 ) {
						square[x+1][i] = '.';
					}
					else if( x == SIZE-1 ) {
						square[x-1][i] = '.';
					}
					else {
						square[x+1][i] = '.';
						square[x-1][i] = '.';
					}
				}
			}
		}
	}

//	Wyswietlanie informacji o aktualnym stanie akcji
	void getStatus() {

		bool firstShot = true;

		for( int i = 0; i < SIZE; i++ )
			for( int j = 0; j < SIZE; j++ )
				if( enemySquareShown[j][i] != ' ' )
					firstShot = false;

		if( !firstShot ) {
			if( status != "" ) {

				if( didSunk ) {

					cout << status << " sunk!!" << endl << endl;
					didSunk = false;
				}
				else if( status == "SeaMine" ) {

					cout << "SeaMine has been hit..." << endl << endl;
					status = "";
				}
				else
					cout << "Hit..." << endl << endl;
			}
			else {
				cout << "Miss..." << endl << endl;
			}
		}
	}

//	Kto ma teraz ruch
	bool isPlaying() { return playing; }

//	Sprawdzanie warunku konczacego gre
	bool gameOverCheck() { return ( playerShipsVector.empty() || enemyShipsVector.empty() ); }

//	Sprawdzenie warunku zwyciestwa
	bool getWin() { return enemyShipsVector.empty(); }

//	Wyswietlanie planszy
	void displayBoard() {

		cout << endl;

		cout << setw(3) << "" << " |" << setw(10) << "~~~~~~~~~~~~~" << "TWOJE STATKI"
					<< setw(10) << "~~~~~~~~~~~~~~" << "|" << endl;

		cout << setw(3) << "" << " | "
			<< "A" << " | "
			<< "B" << " | "
			<< "C" << " | "
			<< "D" << " | "
			<< "E" << " | "
			<< "F" << " | "
			<< "G" << " | "
			<< "H" << " | "
			<< "I" << " | "
			<< "J" << " | " << endl;

		cout << setw(23) << "=============================================" << endl;

		for (int i = 0; i < SIZE; i++) {

			cout << setw(3) << i + 1 << " | ";

			for (int j = 0; j < SIZE; j++) {
				cout << setw(1) << playerSquare[j][i] << " | ";
			}
			cout << endl;
		}

		cout << setw(23) << "=============================================" << endl << endl;

		cout << setw(3) << "" << " |" << setw(10) << "~~~~~~~~~~~~~" << "STATKI WROGA"
					<< setw(10) << "~~~~~~~~~~~~~~" << "|" << endl;

		cout << setw(3) << "" << " | "
			<< "A" << " | "
			<< "B" << " | "
			<< "C" << " | "
			<< "D" << " | "
			<< "E" << " | "
			<< "F" << " | "
			<< "G" << " | "
			<< "H" << " | "
			<< "I" << " | "
			<< "J" << " | " << endl;

		cout << setw(23) << "=============================================" << endl;

		for (int i = 0; i < SIZE; i++) {

			cout << setw(3) << i + 1 << " | ";

			for (int j = 0; j < SIZE; j++) {
				cout << setw(1) << enemySquareShown[j][i] << " | ";
			}
			cout << endl;
		}

		cout << setw(23) << "=============================================" << endl;

		cout << setw(3) << "" << " | " << "Player: " << setw(3) << playerShipsVector.size()
			 << setw(16) << "" << "Enemy: " << setw(3) << enemyShipsVector.size() << " | " << endl << endl;

		getStatus();
	}
};