#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <utility>

constexpr int MAP_HEIGHT = 17;
constexpr int MAP_WIDTH = 17;

using Maze = std::array<std::array<bool, MAP_WIDTH>, MAP_HEIGHT>;

struct Position {
    int row_;
    int col_;
};

Position makeEntrance(const Maze&);
Position makeExit(const Maze&);

Maze makeMaze() {
    Maze maze;

    for (auto & row : maze) {
        for (auto & col : row) {
            col = false;
        }
    }

    std::array<Position, 4> dirs {
        Position{-1, 0}, // North
        Position{1,  0}, // South
        Position{0, -1}, // West
        Position{0,  1}  // East
    };

    int done = 0;

    do {
        // this code is used to make sure the numbers are odd

        int row = 1 + rand() % ((MAP_HEIGHT - 1) / 2) * 2;
        int col = 1 + rand() % ((MAP_WIDTH - 1) / 2) * 2;

        if (done == 0) {
            maze[row][col] = true;
        }

        if(maze[row][col]) {

            //Randomize Directions
            std::random_shuffle(dirs.begin(), dirs.end());

            bool blocked = true;

            do {
                if (rand() % 5 == 0) {
                    std::random_shuffle(dirs.begin(), dirs.end());
                }
                blocked = true;
                for (auto i = 0; i < 4; i++) {
                    // Determine which direction the tile is
                    auto r = row + dirs[i].row_ * 2;
                    auto c = col + dirs[i].col_ * 2;
                    //Check to see if the tile can be used
                    if (r >= 1 && r < MAP_HEIGHT - 1 && c >= 1 && c < MAP_WIDTH - 1) {
                        if (!maze[r][c]) {
                            //create destination location
                            maze[r][c] = true;
                            //create intermediate location
                            maze[row + dirs[i].row_][col + dirs[i].col_] = true;
                            row = r;
                            col = c;
                            blocked = false;
                            done++;
                            break;
                        }
                    }
                }
                //recursive, no directions found, loop back a node
            } while (!blocked);
        }

    } while (done + 1 < ((MAP_HEIGHT - 1) * (MAP_WIDTH - 1)) / 4);

    auto entrance = makeEntrance(maze);
    maze[entrance.row_][entrance.col_] = true;

    auto exit = makeExit(maze);
    maze[exit.row_][exit.col_] = true;

    return maze;
}

// Set the entrance on the top row.  it has to be above an empty cell on the
// next row.  The freeCols vector contains the list of empty cells.
Position makeEntrance(const Maze& maze) {
    std::vector<int> freeCols;

    for (auto i = 1; i < MAP_WIDTH - 1; i++) {
        if (maze[1][i]) {
            freeCols.push_back(i);
        }
    }

    return {0, freeCols[rand() % freeCols.size()] };
}

// Set the exit on the bottom row.  it has to be below an empty cell on the
// previous row.  Once again, freeCols contains the list of empty cells. 
Position makeExit(const Maze& maze) {
    std::vector<int> freeCols;

    for (auto i = 1; i < MAP_WIDTH - 1; i++) {
        if (maze[MAP_HEIGHT - 2][i]) {
            freeCols.push_back(i);
        }
    }

    return {MAP_HEIGHT - 1, freeCols[rand() % freeCols.size()] };
}

void draw(const Maze& maze) {
    for (auto & row : maze) {
        for (auto & col : row) {
            putchar(col ? '.' : '#');
        }
        putchar('\n');
    }
}

int main() {
    srand(time(NULL));

    auto maze = makeMaze();

    draw(maze);

    return 0;
}
