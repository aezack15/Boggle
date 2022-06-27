//g++ -std=c++20 -o game game.cpp

#include <iostream>
#include <stdio.h>
#include <string>
#include <ctime>
#include <cstdlib>
#include <set>
#include <fstream>
#include <list>
#include <chrono>
#include <thread>
#include <cmath>

using namespace std;

struct cell{
	int x;
	int y;
	string s;
	bool mark;
};

void createBoard();
void printBoard();
void makeMultiset();
void findWordsOnBoard(int, int, string);
void setAllUnVisited();
void playGame();
void usage();

multiset<string> wordsOnBoard;
multiset<string> dictionary;
multiset<string> userWords;
cell gameBoard[4][4];
bool keepPlaying=true;
int points;

int main(){
	usage();
	createBoard();
	cout << endl << "Waiting for game to load..." << endl;
	makeMultiset();
	for(int i=0;i<4;i++){	
		for(int j=0;j<4;j++){
			gameBoard[i][j].mark=true;
			findWordsOnBoard(i,j, gameBoard[i][j].s);
			setAllUnVisited();
		}
	}
	cout << "Game is ready!  Good luck!" << endl;
	printBoard();
	this_thread::sleep_for(chrono::seconds(3));
	while(keepPlaying){
		playGame();
	}
}

void createBoard(){
	srand(time(NULL));
	char letters[]="aaaaabcdeeeeeefghiiiijklmnooooopqrssstttuuuuuvwxyz";
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			gameBoard[i][j].x=i;
			gameBoard[i][j].y=j;
			gameBoard[i][j].s=letters[rand()%50];
			gameBoard[i][j].mark=false;
		}
	}
}

void usage(){
	cout << "Welcome to Captain Zack's game of BOGGLE!" << endl;
	cout << endl <<"How to Play: " << endl;
	cout << "Find words on the board by connecting letters" << endl;
	cout << "Letters UP, DOWN, LEFT, RIGHT, or DIAGONAL can be combined to create a word" << endl;
	cout << "Points awarded are based on the length of the word: 2 ^ (length of word)" << endl;
	cout << "Enter '1' to quit the game and show all possible words on the board" << endl;
}

void printBoard(){
	cout << endl;
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			cout << " " << gameBoard[i][j].s << " ";
		}
		cout << endl;
	}
	cout << "Points: " << points << endl;
	multiset<string>::iterator it;
	int i=0;
	for(it = userWords.begin(); it != userWords.end(); it++){
		cout << *it << ", ";
		if(i>0 && i%5 == 0){
			cout << endl;
		}
		i++;
	}
	cout << endl;
}

void makeMultiset(){
	ifstream file;
	file.open("dictionary.txt");
	while(file.good()){
		string temp;
		file >> temp;
		dictionary.insert(temp);
	}
	file.close();
}


void playGame(){
	system("clear");
	printBoard();
	string userInput;
	cout << "> ";
	cin >> userInput;
	if(wordsOnBoard.contains(userInput)){
		if(userWords.contains(userInput)){
			cout << "You've already gotten that word!" << endl;
		}
		else{
			userWords.insert(userInput);
			int newPoints = pow(2, userInput.size());
			points = points + newPoints;
			cout << "+" << newPoints << " points!" << endl;
		}
		this_thread::sleep_for(chrono::seconds(2));
	}
	else{
		if(userInput != "1"){
			cout << "NOT A WORD" << endl;
			this_thread::sleep_for(chrono::seconds(2));
		}
	}
	if(userInput == "1"){
		keepPlaying=false;
		multiset<string>::iterator it;
		int i=0;
		for(it = wordsOnBoard.begin(); it != wordsOnBoard.end(); it++){
			cout << *it << ", ";
			if(i > 0 && i%10 == 0){
				cout << endl;
			}
			i++;
		}
		cout << endl;
	}
}

void findWordsOnBoard(int x, int y, string possibleWord){
	if(dictionary.contains(possibleWord) && possibleWord.size() >= 2){
			//cout << possibleWord << endl;
			wordsOnBoard.insert(possibleWord);
	}
	/*	
	0,0 | 0,1 | 0,2 | 0,3 | 0,4
	1,0 | 1,1 | 1,2 | 1,3 | 1,4
	2,0 | 2,1 | 2,2 | 2,3 | 2,4
	3,0 | 3,1 | 3,2 | 3,3 | 3,4
	*/
		//to the left
		if(y-1>=0 && gameBoard[x][y-1].mark == false){
				gameBoard[x][y-1].mark=true;
				findWordsOnBoard(x, y-1, possibleWord + gameBoard[x][y-1].s);
				gameBoard[x][y-1].mark=false;
		}
		//to the right
		if(y+1<4 && gameBoard[x][y+1].mark == false){
				gameBoard[x][y+1].mark=true;
				findWordsOnBoard(x, y+1, possibleWord + gameBoard[x][y+1].s);
				gameBoard[x][y+1].mark=false;
		}
		//up
		if(x-1>=0 && gameBoard[x-1][y].mark == false){
				gameBoard[x-1][y].mark=true;
				findWordsOnBoard(x-1, y, possibleWord + gameBoard[x-1][y].s);
				gameBoard[x-1][y].mark=false;
		}
		//down
		if(x+1<4 && gameBoard[x+1][y].mark==false){
				gameBoard[x+1][y].mark=true;
				findWordsOnBoard(x+1, y, possibleWord + gameBoard[x+1][y].s);
				gameBoard[x+1][y].mark=false;
		}
		//up and to the right
		if(x-1>=0 && y+1<4 && gameBoard[x-1][y+1].mark == false){
			gameBoard[x-1][y+1].mark=true;
			findWordsOnBoard(x-1, y+1, possibleWord + gameBoard[x-1][y+1].s);
			gameBoard[x-1][y+1].mark=false;
		}
		//up and to the left
		if(x-1>=0 && y-1>=0 && gameBoard[x-1][y-1].mark == false){
			gameBoard[x-1][y-1].mark=true;
			findWordsOnBoard(x-1, y-1, possibleWord + gameBoard[x-1][y-1].s);
			gameBoard[x-1][y-1].mark=false;
		}
		//down and to the right
		if(x+1<4 && y+1<4 && gameBoard[x+1][y+1].mark == false){
			gameBoard[x+1][y+1].mark=true;
			findWordsOnBoard(x+1, y+1, possibleWord + gameBoard[x+1][y+1].s);
			gameBoard[x+1][y+1].mark=false;
		}
		//down and to the left
		if(x+1<4 && y-1>=0 && gameBoard[x+1][y-1].mark == false){
			gameBoard[x+1][y-1].mark=true;
			findWordsOnBoard(x+1, y-1, possibleWord + gameBoard[x+1][y-1].s);
			gameBoard[x+1][y-1].mark=false;
		}
		gameBoard[x][y].mark=false;
}

void setAllUnVisited(){
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			gameBoard[i][j].mark=false;
		}
	}
}
