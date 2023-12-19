#include <iostream>
#include <ctime>

#include <random>
#include <cmath>

#include <fstream>
#include <algorithm>

#include <limits>
#include <list>

#include <map>
#include <string>

#include <ctime>
#include <iomanip>

using namespace std;

const int playeeetrs = 100;
const int Cells = 0;

int finalTotal = 5;

const int Damaged = 2;
const int lost = 1;


struct score_main {
 string name;
 int score;
};

void scoreRead(score_main boardd[], int& Players, const string& filename) {
 ifstream file(filename);

 Players = 0;
 while (file >> boardd[Players].name >> boardd[Players].score) {
  Players++;
  if (Players >= playeeetrs) {
   break;
  }
 }
}

void Scoreboardwriter(const score_main boardd[], int Players, const string& filename) {
 ofstream file(filename);

 for (int i = 0; i < Players; ++i) {
  file << boardd[i].name << " " << boardd[i].score << "\n";
 }
}

bool comparescore_mains(const score_main& a, const score_main& b) {
 return a.score < b.score;
}

void sortScoreboard(score_main boardd[], int Players) {
 sort(boardd, boardd + Players, comparescore_mains);
}

void waitForEnter() {
 cout << "Press Enter to continue...";
 cin.clear();
 cin.ignore(numeric_limits<streamsize>::max(), '\n');
 cin.get();
 system("clear");
}

void printBoard(int board[10][10]) {
 cout << " ";
 for (int j = 0; j < 10; ++j)
 {
  cout << j << " ";
 }
 cout << "\n";

 for (int i = 0; i < 10; i++) {
  cout << i << " ";
  for (int j = 0; j < 10; j++) {
   if (board[i][j] == Cells) {
    cout << "- ";              
   } else if (board[i][j] == lost) {           
    cout << "* ";                    
   } else if (board[i][j] == Damaged) {             
    cout << "x ";                    
   } else if (board[i][j] == finalTotal) {
    cout << "S ";
   }
  }
  cout << endl;
 }
}

int canPlaceShip(int field[10][10], int start_x, int end_x, int start_y, int end_y) {
 if (start_x < 0 || end_x >= 10 || start_y < 0 || end_y >= 10) {
  return false;
 }

 for (int i = start_x - 1; i <= end_x + 1; i++) {
  for (int j = start_y - 1; j <= end_y + 1; j++) {
   if (i >= 0 && i < 10 && j >= 0 && j < 10 && field[i][j] != 0) {
    return false;
   }
  }
 }

 for (int i = start_x; i <= end_x; i++) {
  for (int j = start_y; j <= end_y; j++) {
   if (field[i][j] != 0) {
    return false;
   }
  }
 }

 for (int i = start_x; i <= end_x; i++) {
  for (int j = start_y; j <= end_y; j++) {
   if ((i > 0 && field[i - 1][j] != 0) ||
     (i < 9 && field[i + 1][j] != 0) ||
     (j > 0 && field[i][j - 1] != 0) ||
     (j < 9 && field[i][j + 1] != 0)) {
    return false;
   }
  }
 }
 return true;
}

void randomPlaceShip(int field[10][10], int shipLength) {
 random_device rd;
 mt19937 gen(rd());
 uniform_int_distribution<int> dist(0, 9);

 while (true) {
  bool isHorizontal = dist(gen) % 2 == 0;
  int start_x, end_x, start_y, end_y;

  if (isHorizontal) {
   start_y = (dist(gen) % 10) - shipLength + 1;
   end_y = start_y + shipLength - 1;
   start_x = dist(gen) % 10;
   end_x = start_x;
  } else {
   start_x = (dist(gen) % 10) - shipLength + 1;
   end_x = start_x + shipLength - 1;
   start_y = dist(gen) % 10;
   end_y = start_y;
  }

  if (canPlaceShip(field, start_x, end_x, start_y, end_y)) {
   for (int i = start_x; i <= end_x; i++) {
    for (int j = start_y; j <= end_y; j++) {
     field[i][j] = finalTotal;
    }
   }
   break;
  }
 }
}

int noShip(int field[10][10], int x, int y) {
 int result = 0;
 for (int i = max(0, x - 1); i <= min(10, x + 1); i++) {
  for (int j = max(0, y - 1); j <= min(10, y + 1); j++) {
   if (field[i][j] == finalTotal) {
    result++;
   }
  }
 }
 return result;
}



void shipDestroyed(int field[10][10], int x, int y) {
 int new_x = 99, new_y = 99;
 for (int i = max(0, x - 1); i <= min(10, x + 1); i++) {
  for (int j = max(0, y - 1); j <= min(10, y + 1); j++) {
   if (field[i][j] == Damaged) {
    new_x = i;
    new_y = j;
    field[i][j] = lost;
   }
   field[i][j] = lost;
  }
 }
 if (new_x != 99) {
  shipDestroyed(field, new_x, new_y);
 }
}

void mainGame(int ships[10][10], int you[10][10], int& score) {
 int win = 0, main_count = 0;
 cout << "\nWelcome to the Battle\n\n";
 while (win == 0) {
  int a, b, pop = 0, count = 0;
  printBoard(you);
  cout << "\nenter the coordinates to an attack: ";
  cin >> a >> b;
  if (b < 0 || b > 10 || a < 0 || a > 10) {
   while (b < 0 || b > 10 || a < 0 || a > 10) {
    cout << "The coordinates are wrong write another coordinates: ";
    cin >> a >> b;
   }
  }
  if (ships[a][b] == 1 || ships[a][b] == 2 || ships[a][b] == 3) {
   while (ships[a][b] == 1 || ships[a][b] == 2 || ships[a][b] == 3) {
    cout << "You already have damaged this point. write another: ";
    cin >> a >> b;
   }
  }
  if (ships[a][b] == 0) {
   cout << "\nMiss!\n" << endl;
   ships[a][b] = lost;
   you[a][b] = lost;
   printBoard(you);
  } else if (ships[a][b] == finalTotal and noShip(ships, a, b) == 1) {
   cout << "\nSunk!\n";
   shipDestroyed(ships, a, b);
   shipDestroyed(you, a, b);
   printBoard(you);
  } else if (ships[a][b] == finalTotal and noShip(ships, a, b) > 1) {
   cout << "\nHit!\n";
   ships[a][b] = Damaged;
   you[a][b] = Damaged;
   printBoard(you);
  }
  main_count++;

  for (int i = 0; i < 10; i++) {
   for (int j = 0; j < 10; j++) {
    if (ships[i][j] == 5) {
     pop++;
    }
   }
  }
  if (pop != 0) {
   waitForEnter();
  } else {
   win = 1;
   if (main_count == 0) {
    main_count = count;
   } else {
    if (count > main_count) {
     main_count = count;
    }
   }
  }
 }

 string answer;
 cout << endl << "The number of your attempts: " << main_count <<"\n\nDo you want to continue? y/n: ";
 cin >> answer;
 if (answer == "y") {
  mainGame(ships, you, score);
 } else {
  score = main_count;
 }
}

int main() {
 int field[10][10], shipsMap[10][10], score = 0;
 for (int i = 0; i < 10; i++)
 {
  for (int j = 0; j < 10; j++)
  {
   field[i][j] = 0;
   shipsMap[i][j] = 0;
  }
 }
  
 int ships[] = {1, 1, 1, 1, 2, 2, 3};
 for (int i = 0; i < 10; i++)
 {
  randomPlaceShip(field, ships[i]);
 }

 score_main boardd[playeeetrs];
 int Players = 0;

 string filename = "players.txt";
 scoreRead(boardd, Players, filename);  

 string name;
 cout << "Input your name: ";
 cin >> name;
 cout << "The rules: \n* - miss \nx-damaged";
 mainGame(field, shipsMap, score);

 if (Players < playeeetrs) {
  boardd[Players].name = name;
  boardd[Players].score = score;
  Players++;
 } else {
  if (score > boardd[playeeetrs - 1].score) {
   boardd[playeeetrs - 1].name = name;
   boardd[playeeetrs - 1].score = score;
  }
 }

 sortScoreboard(boardd, Players);
 Scoreboardwriter(boardd, Players, filename);

 cout << "\nScoreboard:\n";
 for (int i = 0; i < Players; ++i) {
  cout << boardd[i].name << ": " << boardd[i].score << endl;
 }
 return 0;
}
