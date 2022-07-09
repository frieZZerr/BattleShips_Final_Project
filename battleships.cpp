#include "battleships.h"

using namespace std;

int opcja = 0;

void mainScreen() {

	cout << setw(23) << "=============================================" << endl;
	cout << "|" << setw(21) << "" << "|" << setw(21) << "" << "|" << endl;
	cout << "|" << setw(10) << "~~~~~~~~~~~~~~~~" << "BATTLESHIPS"
				<< setw(10) << "~~~~~~~~~~~~~~~~" << "|" << endl;
	cout << "|" << setw(21) << "" << "|" << setw(21) << "" << "|" << endl;
	cout << setw(23) << "=============================================" << endl << endl;

	cout << "|" << setw(10) << "~~~~~~~~~~~~~~~~~" << "MAIN MENU"
				<< setw(10) << "~~~~~~~~~~~~~~~~~" << "|" << endl;
	cout << "|" << setw(3) << "" << "--> 1. PLAY GAME" << setw(24) << "" << "|" << endl;
	cout << "|" << setw(3) << "" << "--> 2. QUIT GAME" << setw(24) << "" << "|" << endl;
	cout << "|" << setw(21) << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << "|" << endl << endl;

	cout << "Choice: ";
	cin >> opcja;
	cout << endl;
	assert( opcja < 3 && opcja > 0 );
}

//  Czyszczenie ekranu
void clrscr() {

    system("@cls||clear");
}

int main( int argc, char *argv[] ) {

	srand( time( NULL ) );

	bool end = false, gameOver = false;

	while( !end ) {

		clrscr();
		mainScreen();

		switch( opcja ) {

			case 1: {

				Board board;

				while( !gameOver ) {

					//	Player's turn
					while( board.isPlaying() ) {

						clrscr();

						board.displayBoard();
						Coordinates shot = board.wait4move();
						board.fire( shot, "Player" );

						if( board.gameOverCheck() ) {

							gameOver = true;

							if( !board.getWin() ) {

								clrscr();
								board.displayBoard();

								cout << "YOU LOST !!!" << endl << endl;
							}
							else {

								clrscr();
								board.displayBoard();

								cout << "YOU WON !!!" << endl << endl;
							}
							break;
						}
					}

					//	AI's turn
					while( !board.isPlaying() ) {

						clrscr();

						board.displayBoard();
						sleep(1);
						Coordinates ai = board.randomShot();
						board.fire( ai, "AI" );

						if( board.gameOverCheck() ) {

							gameOver = true;

							if( !board.getWin() ) {

								clrscr();
								board.displayBoard();

								cout << "YOU LOST !!!" << endl << endl;
							}
							else {

								clrscr();
								board.displayBoard();

								cout << "YOU WON !!!" << endl << endl;
							}
							break;
						}
					}
				}

				end = true;
			} break;

			case 2: {

				clrscr();
				end = true;
			} break;

			default: {
				
				clrscr();
				cout << "Input ERROR !!!" << endl << endl;
				mainScreen();
			}
		}
	}

	return 0;
}
