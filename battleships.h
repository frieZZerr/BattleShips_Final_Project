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

//	Constructor
	Coordinates( char X, int Y ) {

		x = X;
		y = Y;
	}

	Coordinates() {}

	int getX() {							//	X coordinate

		if( (int)x >= 97 && (int)x <= 106 )
			return (int)x - 97;
		else
			return (int)x - 65;

	}

	int getY() { return y; }				//	Y coordinate

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

//	Constructor
	Ship( vector<int> X, vector<int> Y, int size_, string name_, int way_ ) {

		x    = X;
		y    = Y;
		size = size_;
		name = name_;
		way  = way_;
		hit  = 0;
	}

//	----------   Getters   -----------
	int getSize() { return size; }
	int getWay()  { return way;  }

	vector<int> getX() { return x; }
	vector<int> getY() { return y; }

	string getName() { return name; }
//	-------------------------------

//	Checking if the ship has sunk
	bool isSunk() { return hit >= size; }

//	Ship gets damaged
	void getShot() { hit++; }

//	Check if there's any ship at the passed coordinates
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

//	Constructor
	Board() {

//		"Clearing" the boards
		for( int i = 0; i < SIZE; i++ )
			for( int j = 0; j < SIZE; j++ ) {
				playerSquare[i][j] 	   = ' ';
				enemySquare[i][j]  	   = ' ';
				enemySquareShown[i][j] = ' ';
			}

		makeShips( playerSquare, playerShipsVector, playerSeaMine );	//	Player's ships and seamine setup
		makeShips( enemySquare,  enemyShipsVector,  enemySeaMine  );	//	Enemy's ships and seamine setup
	}

//	Creating all the needed ships in random positions that are compatible with the rules
	void makeShips( char square[SIZE][SIZE], vector<Ship> &vec, Coordinates &seaMine ) {

//		Array with taken positions
		bool placedShips[SIZE][SIZE];
		bool whereShips[SIZE][SIZE];
		for( int i = 0; i < SIZE; i++ )
			for( int j = 0; j < SIZE; j++ ) {

				placedShips[i][j] = false;
				whereShips[i][j]  = false;
			}

//		shipNumber defines the biggest possible ship
		for( int shipNumber = 4; shipNumber > 0; shipNumber-- ) {

//			Setting an amount of ships depending on it's type and it's name
			int howManyShips = 5 - shipNumber;
			string shipName = setShipName( shipNumber );

			bool good = true;
			while( good ) {

				int whichWay = rand() % 2;

				if( whichWay == 0 ) {	//	Horizontal

//					Random coordinates
					char X = 'A' + rand() % ( SIZE-shipNumber );
					int  Y = rand() % SIZE;
					Coordinates cords( X, Y );

//					Checking if a ship can be placed at these coordinates
					if( canPlace( placedShips, cords, shipNumber, whichWay ) ) {

//						Creating a ship
						vector<int> vecX;
						vector<int> vecY;
						for( int i = cords.getX(); i < cords.getX()+shipNumber; i++ ) {
							vecX.push_back(i);
							vecY.push_back(Y);
						}
						string name_ = shipName + to_string( howManyShips );
						Ship ship( vecX, vecY, shipNumber, name_, whichWay );

//						Placing the ship on the board
						placeShip( square, cords, shipNumber, whichWay, whereShips );
						vec.push_back( ship );

//						Stop the loop if there're enough ships placed
						howManyShips--;
						if( howManyShips == 0 )
							good = false;
					}
				}
				else {					//	Vertical

//					Random coordinates
					char X = 'A' + rand() % SIZE;
					int  Y = rand() % ( SIZE-shipNumber );
					Coordinates cords( X, Y );

//					Checking if a ship can be placed at these coordinates
					if( canPlace( placedShips, cords, shipNumber, whichWay ) ) {

//						Creating a ship
						vector<int> vecX;
						vector<int> vecY;
						for( int i = Y; i < Y+shipNumber; i++ ) {
							vecX.push_back( cords.getX() );
							vecY.push_back(i);
						}
						string name_ = shipName + to_string( howManyShips );
						Ship ship( vecX, vecY, shipNumber, name_, whichWay );

//						Placing the ship on the board
						placeShip( square, cords, shipNumber, whichWay, whereShips );
						vec.push_back( ship );

//						Stop the loop if there're enough ships placed
						howManyShips--;
						if( howManyShips == 0 )
							good = false;
					}
				}
			}
		}

//		Generating seamines
		generateSeaMine( square, whereShips, seaMine );
	}

//	Generating seamines
	void generateSeaMine( char square[SIZE][SIZE], bool placed[SIZE][SIZE], Coordinates &seaMine ) {

		int quantity = 1;
		bool isPlaced;

		for( int i = 0; i < quantity; i++ ) {

			isPlaced = false;

			while( !isPlaced ) {

	//			Random coordinates excluding the "coast" ones
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

//	Checking if a specific ship can be placed at the passed coordinates
	bool canPlace( bool placed[SIZE][SIZE], Coordinates &cords, int shipSize, int way ) {

		int x = cords.getX();
		int y = cords.getY();

		if( way == 0 ) {		//	Horizontal

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

		else {					//	Vertical

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

//	Placing the ship on the board
	void placeShip( char square[SIZE][SIZE], Coordinates &cords, int shipSize, int way, bool ships[SIZE][SIZE] ) {
		
		int col = cords.getX();
		int row = cords.getY();
		char ship = shipSize + 48;

		if( way == 0 ) {		//	Horizontal

			for( int i = 0; i < shipSize; i++ ) {

//				Ships are drawn on board as their size number
				square[col][row] = ship;

//				Ships are drawn on board as 'O'
//				square[col][row] = 'O';

				ships[col][row] = true;

				col++;
			}
		}
		else {					//	Vertical

			for( int i = 0; i < shipSize; i++ ) {

//				Ships are drawn on board as their size number
				square[col][row] = ship;

//				Ships are drawn on board as 'O'
//				square[col][row] = 'O';

				ships[col][row] = true;

				row++;
			}
		}
	}

//	Ship's name setup depending on it's size
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

//	Generating random coordinates
	Coordinates randomShot() {

		char X = 'A' + rand() % SIZE;
		int Y  = rand() % SIZE;

		Coordinates shot( X, Y );

		return shot;
	}

//	Getting input coordinates where to shoot
	Coordinates wait4move() {

		char x;
		int y;
		bool allGood = false;

		while( !allGood ) {

			cout << setw(3) << "Enter coordinates: ";
			cin >> x >> y;
			cout << endl;

			if( ( ( (int)x >= 65 && (int)x <= 74 ) || ( (int)x >= 97 && (int)x <= 106 ) )
					&& y >= 1 && y <= 10 ) {
				allGood = true;
				break;
			}

			cout << "Wrong values!" << endl << endl;
			assert( ( ( (int)x >= 65 && (int)x <= 74 ) || ( (int)x >= 106 && (int)x <= 97 ) )
						&& y >= 1 && y <= 10 ) ;
		}

		Coordinates shotCoord( x, y-1 );

		return shotCoord;
	}

//	Firing at passed coordinates
	void fire( Coordinates &shot, string turn ) {

		int x = shot.getX();
		int y = shot.getY();

//		Player's turn
		if( turn == "Player" ) {

//			Checking if the seamine has been hit
			if( x == enemySeaMine.getX() && y == enemySeaMine.getY() ) {

//				Setting the action status
				status = "SeaMine";

				for( int i = x-1; i <= x+1; i++ ) {
					for( int j = y-1; j <= y+1; j++ ) {

						Coordinates cords( (char)i+65, j );

//						Checking if the seamine has damaged any ships in it's explosion radius
						checkHitShips( enemySquareShown, enemyShipsVector, cords );
					}
				}

//				End turn
				playing = false;
			}
			else {

//				Checking if any ship has been hit
				checkHitShips( enemySquareShown, enemyShipsVector, shot );

//				Next shot
				if( enemySquareShown[x][y] != 'x' )
					playing = false;
				else
					playing = true;
			}
		}

//		AI's turn
		if( turn == "AI" ) {

//			Checking if the seamine has been hit
			if( x == playerSeaMine.getX() && y == playerSeaMine.getY() ) {

//				Setting the action status
				status = "SeaMine";

				for( int i = x-1; i <= x+1; i++ ) {
					for( int j = y-1; j <= y+1; j++ ) {

						Coordinates cords( i, j );

//						Checking if the seamine has damaged any ships in it's explosion radius
						checkHitShips( playerSquare, playerShipsVector, cords );
					}
				}

//				End turn
				playing = true;
			}
			else {

//				Checking if any ship has been hit
				checkHitShips( playerSquare, playerShipsVector, shot );

//				Next shot
				if( playerSquare[x][y] != 'x' )
					playing = true;
				else
					playing = false;
			}
		}
	}

//	Checking if any ship has been hit
	void checkHitShips( char square[SIZE][SIZE], vector<Ship> &ships, Coordinates &shot ) {

		int x = shot.getX();
		int y = shot.getY();
		string whatGotHit = "";

//		Looking for a hit ship
		for( int i = 0; i < (int)ships.size(); i++ ) {

			
//			Saving the hit ship's name
			whatGotHit = ships.at(i).getShip( shot );
			
			if( whatGotHit != "" ) {

//				Setting the action status
				if( status != "SeaMine" )
					status = whatGotHit;

//				Ships gets damaged
				ships.at(i).getShot();

//				Board update
				square[x][y] = 'x';

//				Checking if the ship has been sunk
				if( ships.at(i).isSunk() ) {

//					Board update
					updateAfterSunk( square, ships.at(i) );

//					Removing the sunk ship from the vector
					ships.erase( ships.begin()+i );
					didSunk = true;
				}

				break;
			}
		}

//		If nothing has been hit
		if( whatGotHit == "" ) {

//			Board update
			square[x][y] = '.';
		}
	}

//	Board update after any ship has been sunk
	void updateAfterSunk( char square[SIZE][SIZE], Ship &ship ) {

		if( ship.getWay() == 0 ) {				//	Horizontal

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
		else {									//	Vertical

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

//	Displaying the active game status
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

//	Who's playing
	bool isPlaying() { return playing; }

//	Checking if the game is over
	bool gameOverCheck() { return ( playerShipsVector.empty() || enemyShipsVector.empty() ); }

//	Checking the victory condition
	bool getWin() { return enemyShipsVector.empty(); }

//	Displaying the board
	void displayBoard() {

		cout << endl;

		cout << setw(3) << "" << " |" << setw(10) << "~~~~~~~~~~~~~" << "YOUR SHIPS"
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

		cout << setw(3) << "" << " |" << setw(10) << "~~~~~~~~~~~~~" << "ENEMY SHIPS"
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
