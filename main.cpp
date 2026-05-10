#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <string>

using namespace std;

const int N = 3;

struct State {

    vector<vector<int>> board;

    vector<int> stars;

    string stateStr;

    int dist;
};

// ----------------------------------------------------
// Convert board + stars to unique string
// ----------------------------------------------------
string boardToString(
        const vector<vector<int>>& board,
        const vector<int>& stars
) {

    string s;

    for(int i = 0; i < N; i++) {

        for(int j = 0; j < N; j++) {

            bool isStar = false;

            for(int x : stars) {

                if(board[i][j] == x) {
                    isStar = true;
                    break;
                }
            }

            if(isStar)
                s += "*,";
            else
                s += to_string(board[i][j]) + ",";
        }
    }

    return s;
}

// ----------------------------------------------------
// Pretty board print
// ----------------------------------------------------
string boardToReadableString(
        const vector<vector<int>>& board,
        const vector<int>& stars
) {

    string s;

    for(int i = 0; i < N; i++) {

        for(int j = 0; j < N; j++) {

            bool isStar = false;

            for(int x : stars) {

                if(board[i][j] == x)
                    isStar = true;
            }

            if(board[i][j] == 0)
                s += " . ";

            else if(isStar)
                s += " * ";

            else
                s += " " +
                     to_string(board[i][j]) +
                     " ";

            if(j < N - 1)
                s += "\t";
        }

        s += "\n";
    }

    return s;
}

// ----------------------------------------------------
// Find zero
// ----------------------------------------------------
pair<int,int> findZero(
        const vector<vector<int>>& board
) {

    for(int i = 0; i < N; i++) {

        for(int j = 0; j < N; j++) {

            if(board[i][j] == 0)
                return {i, j};
        }
    }

    return {-1, -1};
}

// ----------------------------------------------------
// Read stars
// ----------------------------------------------------
vector<int> readStars() {

    vector<int> stars;

    int starCount;

    cout << "How many stars (*) ? ";
    cin >> starCount;

    if(starCount > 0) {

        cout << "Enter star values:\n";

        for(int i = 0; i < starCount; i++) {

            int x;
            cin >> x;

            stars.push_back(x);
        }
    }

    return stars;
}

// ----------------------------------------------------
// Read board
// ----------------------------------------------------
vector<vector<int>> readBoard() {

    vector<vector<int>> board(
            N,
            vector<int>(N)
    );

    cout << "Enter 3x3 board:\n";
    cout << "(Use 0 for empty)\n\n";

    for(int i = 0; i < N; i++) {

        for(int j = 0; j < N; j++) {

            cin >> board[i][j];
        }
    }

    return board;
}

// ----------------------------------------------------
// Generate heuristic database
// ----------------------------------------------------
void generateHeuristicDatabase(
        vector<vector<int>> goalBoard,
        vector<int> stars
) {

    queue<State> q;

    unordered_set<string> visited;

    ofstream dbFile("heuristic.txt");

    if(!dbFile.is_open()) {

        cerr << "Error opening heuristic.txt\n";
        return;
    }

    State start;

    start.board = goalBoard;

    start.stars = stars;

    start.dist = 0;

    start.stateStr =
            boardToString(
                    start.board,
                    start.stars
            );

    q.push(start);

    visited.insert(start.stateStr);

    dbFile
            << start.stateStr
            << " "
            << start.dist
            << "\n";

    int count = 1;

    int dx[4] = {-1, 1, 0, 0};

    int dy[4] = {0, 0, -1, 1};

    cout << "\nGenerating ALL states...\n";

    while(!q.empty()) {

        State current = q.front();

        q.pop();

        auto [zx, zy] =
                findZero(current.board);

        for(int i = 0; i < 4; i++) {

            int nx = zx + dx[i];

            int ny = zy + dy[i];

            if(nx >= 0 &&
               nx < N &&
               ny >= 0 &&
               ny < N) {

                State newState = current;

                swap(
                        newState.board[zx][zy],
                        newState.board[nx][ny]
                );

                newState.dist =
                        current.dist + 1;

                newState.stateStr =
                        boardToString(
                                newState.board,
                                newState.stars
                        );

                if(
                        visited.count(
                                newState.stateStr
                        ) == 0
                        ) {

                    visited.insert(
                            newState.stateStr
                    );

                    q.push(newState);

                    dbFile
                            << newState.stateStr
                            << " "
                            << newState.dist
                            << "\n";

                    count++;

                    if(count % 1000 == 0) {

                        cout
                                << count
                                << " states generated...\n";
                    }
                }
            }
        }
    }

    dbFile.close();

    cout << "\nDONE!\n";

    cout << "Total states = "
         << count
         << endl;

    cout << "Saved in heuristic.txt\n";
}

// ----------------------------------------------------
// Load heuristic database
// ----------------------------------------------------
unordered_map<string,int>
loadHeuristicDatabase() {

    unordered_map<string,int> db;

    ifstream file("heuristic.txt");

    if(!file.is_open()) {

        cerr << "Could not open heuristic.txt\n";

        return db;
    }

    string state;

    int dist;

    while(file >> state >> dist) {

        db[state] = dist;
    }

    file.close();

    return db;
}

// ----------------------------------------------------
// Heuristic function
// ----------------------------------------------------
int heuristic(
        const vector<vector<int>>& board,
        const vector<int>& stars,
        unordered_map<string,int>& db
) {

    string s =
            boardToString(board, stars);

    if(db.count(s))
        return db[s];

    return -1;
}

// ----------------------------------------------------
// Test heuristic
// ----------------------------------------------------
void testHeuristic(
        unordered_map<string,int>& db,
        const vector<int>& stars
) {

    cout << "\n====================\n";
    cout << "TEST A STATE\n";
    cout << "====================\n";

    vector<vector<int>> board =
            readBoard();

    cout << "\nYour State:\n\n";

    cout
            << boardToReadableString(
                    board,
                    stars
            );

    string s =
            boardToString(board, stars);

    if(db.count(s)) {

        cout
                << "\nExact Distance To Goal = "
                << db[s]
                << endl;
    }
    else {

        cout
                << "\nThis state is NOT reachable "
                << "from the goal state.\n";
    }
}

// ----------------------------------------------------
// MAIN
// ----------------------------------------------------
int main() {

    cout << "====================\n";
    cout << "GOAL STATE\n";
    cout << "====================\n\n";

    vector<vector<int>> goalBoard =
            readBoard();

    vector<int> stars =
            readStars();

    cout << "\nGoal State:\n\n";

    cout
            << boardToReadableString(
                    goalBoard,
                    stars
            );

    // Generate database
    generateHeuristicDatabase(
            goalBoard,
            stars
    );

    // Load database
    unordered_map<string,int> db =
            loadHeuristicDatabase();

    cout << "\nDatabase loaded.\n";

    // Testing loop
    while(true) {

        testHeuristic(db, stars);

        cout
                << "\nTest another state?\n";
        cout
                << "1 = YES\n";
        cout
                << "0 = NO\n\n";

        int x;

        cin >> x;

        if(x == 0)
            break;
    }

    return 0;
}