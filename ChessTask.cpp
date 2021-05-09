#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <set>
using namespace std;


// Reads board"x".txt and fills the 2D chessBoard vector
void readFromFile(vector<vector<string>>& chessBoard, string &currentInputFile) {
	ifstream file(currentInputFile);
	string line;
	// read file line by line
	while (getline(file, line)) {
		// read line word by word
		string word = "";
		vector<string> row;
		for (int i = 0; i < line.size(); i++) {
			if (line[i] != ' ') {
				word += line[i];
			}
			else {
				row.push_back(word);
				word = "";
			}
		}
		row.push_back(word);
		chessBoard.push_back(row);
	}
	// Close file input
	file.close();
}

// Finds knight and elephand coordinates for both sides, fs as, fb ab
void findKnightAndElephandCoordinates(vector<vector<string>>& chessBoard, vector<vector<int>>& knightCoordinatesBlack,
	vector<vector<int>>& elephantCoordinatesBlack, vector<vector<int>>& knightCoordinatesWhite,
	vector<vector<int>>& elephantCoordinatesWhite) {
	for (int i = 0; i < chessBoard.size(); i++) {
		for (int j = 0; j < chessBoard[i].size(); j++) {
			if (chessBoard[i][j] == "fs") {
				elephantCoordinatesBlack.push_back({ i , j });
			}
			else if (chessBoard[i][j] == "as") {
				knightCoordinatesBlack.push_back({ i , j });
			}
			else if (chessBoard[i][j] == "fb") {
				elephantCoordinatesWhite.push_back({ i , j });
			}
			else if (chessBoard[i][j] == "ab") {
				knightCoordinatesWhite.push_back({ i , j });
			}
		}
	}
}

// Prints the current state of the chess board
void printChessBoard(vector< vector<string> >& chessBoard) {
	for (int i = 0; i < chessBoard.size(); i++) {
		for (int j = 0; j < chessBoard[i].size(); j++) {
			cout << chessBoard[i][j] << " ";

		}
		cout << endl;
	}
	cout << endl;
}

// Check if given point is inside
bool inside(int x, int y, vector<vector<string>>& chessBoard) {
	if (x < 0 || x >= chessBoard.size() || y < 0 || y >= chessBoard[x].size()) {
		return false;
	}
	return true;
}

// Move given knight coordinates and fill the map accordingly
void moveKnight(vector<vector<int>>& knightCoordinates, unordered_map<string, int>& stonesRiskCountMap, set<vector<int>>& visited,
	vector<vector<int>>& knightMoves, vector<vector<string>>& chessBoard, unordered_set<string>& stonesSet) {
	// For each knight
	for (vector<int>& currentPoint : knightCoordinates) {
		for (vector<int>& move : knightMoves) {
			int nx = currentPoint[0] + move[0];
			int ny = currentPoint[1] + move[1];
			// Keeps track of current stones that are under threat
			if (inside(nx, ny, chessBoard) && stonesSet.find(chessBoard[nx][ny]) != stonesSet.end()) {
				// Mark current coordiantes as visited
				visited.insert({ nx, ny });
				// Increment current stone by 1
				stonesRiskCountMap[chessBoard[nx][ny]]++;
			}
		}
	}
}

// Move given elephant coordinates and fill the map accordingly
void moveElephant(vector<vector<int>>& elephantCoordinates, unordered_map<string, int>& stonesRiskCountMap, set<vector<int>>& visited,
	vector<vector<int>>& elephantMoves, vector<vector<string>>& chessBoard, unordered_set<string>& stonesSet) {
	stonesSet.insert("xx");
	// For each knight
	for (vector<int>& currentPoint : elephantCoordinates) {
		for (vector<int>& move : elephantMoves) {
			int nx = currentPoint[0] + move[0];
			int ny = currentPoint[1] + move[1];
			// Keep going until we meet the opposite stone
			while (inside(nx, ny, chessBoard) && stonesSet.find(chessBoard[nx][ny]) != stonesSet.end()) {
				if (stonesSet.find(chessBoard[nx][ny]) != stonesSet.end() && chessBoard[nx][ny] != "xx"
					&& visited.find({ nx, ny }) == visited.end()) {
					stonesRiskCountMap[chessBoard[nx][ny]]++;
					break;
				}
				nx = nx + move[0];
				ny = ny + move[1];
			}
		}
	}
}

// Prints the current state of the chess board, Assume color = b -> white, color -> s -> black
void fillCountsMap(vector< vector<string> >& chessBoard, unordered_map<string, int>& countsMap, string color) {
	for (int i = 0; i < chessBoard.size(); i++) {
		for (int j = 0; j < chessBoard[i].size(); j++) {
			string stoneType = "", stoneColor = "";
			stoneType += chessBoard[i][j].front();
			stoneColor += chessBoard[i][j].back();
			if (stoneType != "x" && stoneColor == color) {
				countsMap[stoneType]++;
			}
		}
	}
}

int main() {

	// Data structures
	vector<vector<string>> chessBoard;
	vector<string> inputFiles = { "board1.txt", "board2.txt", "board3.txt" };
	
	// Result.txt
	ofstream score;
	score.open("sonuçlar1.txt");
	score << "Tahta Dosya Adı		Sonuçlar" << endl;

	int inputFileVector = 0;

	// First read each txt file
	for (string &file : inputFiles) {
		// Fill chessBoard from current file
		readFromFile(chessBoard, file);

		// Black Side Knight and Elephant coordinates
		vector<vector<int>> knightCoordinatesBlack; 
		vector<vector<int>> elephantCoordinatesBlack;

		// White Side Knight and Elephant coordinates
		vector<vector<int>> knightCoordinatesWhite;
		vector<vector<int>> elephantCoordinatesWhite;

		// Find Knight and Elephant coordinates for both team
		findKnightAndElephandCoordinates(chessBoard, knightCoordinatesBlack,
			elephantCoordinatesBlack, knightCoordinatesWhite, elephantCoordinatesWhite);

		// WHITE SIDE
		// Create Hash Table for White Stones, each key represent a stone and value represents how many that stone is under risk
		unordered_map<string, int> whiteStonesRiskCountMap, pointsMap; 
		// All types of white stones
		unordered_set<string> whiteStonesSet = { "pb", "kb", "ab", "fb", "vb", "sb" };
		// To check if we have already visited a coordinate point
		set<vector<int>> visited;
		vector<string> whiteStones = { "pb", "kb", "ab", "fb", "vb", "sb" };
		for (string& stone : whiteStones) {
			whiteStonesRiskCountMap.insert({ stone, 0 });
		}

		// Possible moves of the knight
		vector<vector<int>> knightMoves = { {-2, 1}, {-2, -1}, {-1, +2}, {-1, -2}, {2, -1}, {2, 1}, {1, 2}, {1, -2} };

		// Simulate black knight moves and count overlapping stones
		moveKnight(knightCoordinatesBlack, whiteStonesRiskCountMap, visited, knightMoves, chessBoard, whiteStonesSet);


		// Possible moves of the elephant
		vector<vector<int>> elephantMoves = { {1, 1}, {-1, -1}, {-1, 1}, {1, -1} };

		// Simulate black elephant moves and count overlapping stones
		moveElephant(elephantCoordinatesBlack, whiteStonesRiskCountMap, visited, elephantMoves, chessBoard, whiteStonesSet);


		// Generate the points table for risk calculations
		vector<pair<string, int>> stonePoints = { {"p", 1}, {"a", 3}, {"f", 3}, {"k", 5}, {"v", 9}, {"s", 100} };
		for (pair<string, int>& stonePoint : stonePoints) {
			pointsMap.insert(stonePoint);
		}

		// Count all type of stones and fill the countMap
		unordered_map<string, int> whiteStoneCounts;
		fillCountsMap(chessBoard, whiteStoneCounts, "b");


		// Calculate risk table for white side
		double whiteSideTotalPoints = 0;
		for (auto& tuple : whiteStonesRiskCountMap) {
			string stoneType = "";
			stoneType += tuple.first[0];
			int numberOfStonesUnderThreat = tuple.second;
			whiteSideTotalPoints += (1.0 * numberOfStonesUnderThreat) * (1.0 * pointsMap[stoneType] / 2);
			whiteSideTotalPoints += 0.0 + ((whiteStoneCounts[stoneType] - numberOfStonesUnderThreat) * (pointsMap[stoneType]));
			
		}

		cout << "White Side Risk Point: " << whiteSideTotalPoints << endl;

	

		// BLACK SIDE
		// Create Hash Table for Black Stones, each key represent a stone and value represents how many that stone is under risk
		unordered_map<string, int> blackStonesRiskCountMap, pointsMapBlack; // {key, value}
		// All types of black stones
		unordered_set<string> blackStonesSet = { "ps", "ks", "as", "fs", "vs", "ss" };
		// To check if we have already visited a coordinate point
			 
		vector<string> blackStones = { "ps", "ks", "as", "fs", "vs", "ss" };
		for (string& stone : blackStones) {
			blackStonesRiskCountMap.insert({ stone, 0 });
		}

		// Possible moves of the knight
			
		// Simulate black knight moves and count overlapping stones
		moveKnight(knightCoordinatesWhite, blackStonesRiskCountMap, visited, knightMoves, chessBoard, blackStonesSet);


		// Possible moves of the elephant
				
		// Simulate black elephant moves and count overlapping stones
		moveElephant(elephantCoordinatesWhite, blackStonesRiskCountMap, visited, elephantMoves, chessBoard, blackStonesSet);


		// Count all type of stones and fill the countMap
		unordered_map<string, int> blackStoneCounts;
		fillCountsMap(chessBoard, blackStoneCounts, "s");

	
		// Calculate risk table for Black side
		double blackSideTotalPoints = 0;
		for (auto& tuple : blackStonesRiskCountMap) {
			string stoneType = "";
			stoneType += tuple.first[0];
			int numberOfStonesUnderThreat = tuple.second;
			blackSideTotalPoints += (1.0 * numberOfStonesUnderThreat) * (1.0 * pointsMap[stoneType] / 2);
			blackSideTotalPoints += 0.0 + ((blackStoneCounts[stoneType] - numberOfStonesUnderThreat) * (pointsMap[stoneType]));
			
		}

		cout << "Black Side Risk Point: " << blackSideTotalPoints << endl;

		// Print chess board
		printChessBoard(chessBoard);
		// Clear chessBoard for next iteration
		chessBoard.clear();
		
		//Generate points risk table all .txt and both sides
		
		score<<inputFiles[inputFileVector] << "              Siyah=" << blackSideTotalPoints << ",  Beyaz=" << whiteSideTotalPoints<<endl;
		inputFileVector++;
	}
	score.close();
	
}