#include <array>
#include <cstdlib>
#include <ctime>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

constexpr int MAP_HEIGHT = 17;
constexpr int MAP_WIDTH = 17;
constexpr float CELL_HEIGHT = 32;
constexpr float CELL_WIDTH = 32;

using Maze = std::array<std::array<bool, MAP_WIDTH>, MAP_HEIGHT>;

struct Position {
    int row_;
    int col_;
};

class Model {
public:
    Model();

    Maze maze_;
    Position entrance_;
    Position exit_;

private:
    Maze makeMaze();
    Position makeEntrance();
    Position makeExit();
};

Model::Model() : maze_{makeMaze()},entrance_{makeEntrance()}, exit_{makeExit()}
{
    maze_[entrance_.row_][entrance_.col_] = true;
    maze_[exit_.row_][exit_.col_] = true;
}

Maze Model::makeMaze() {
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

    return maze;
}

// Set the entrance on the top row.  it has to be above an empty cell on the
// next row.  The freeCols vector contains the list of empty cells.
Position Model::makeEntrance() {
    std::vector<int> freeCols;

    for (auto i = 1; i < MAP_WIDTH - 1; i++) {
        if (maze_[1][i]) {
            freeCols.push_back(i);
        }
    }

    return {0, freeCols[rand() % freeCols.size()] };
}

// Set the exit on the bottom row.  it has to be below an empty cell on the
// previous row.  Once again, freeCols contains the list of empty cells. 
Position Model::makeExit() {
    std::vector<int> freeCols;

    for (auto i = 1; i < MAP_WIDTH - 1; i++) {
        if (maze_[MAP_HEIGHT - 2][i]) {
            freeCols.push_back(i);
        }
    }

    return {MAP_HEIGHT - 1, freeCols[rand() % freeCols.size()] };
}

class View : public olc::PixelGameEngine {
public:
    explicit View(Model);

    void draw();
    bool OnUserCreate() override;
    bool OnUserUpdate(float) override;

private:
    Model& model_;
    olc::Pixel wall_;
    olc::Pixel floor_;
    olc::Pixel player_;
};

View::View(Model model) : model_{model}, wall_{olc::BLACK},
floor_{olc::WHITE}, player_{olc::MAGENTA} {
    sAppName = "Maze Demo";
}

bool View::OnUserCreate() {
    return true;
}

bool View::OnUserUpdate(float elapsedTime) {
    draw();
    return true;
}

void View::draw() {
    Clear(wall_);

    for (size_t row = 0; row < MAP_HEIGHT; ++row) {
       for (size_t col = 0; col < MAP_WIDTH; ++col) {
           if (model_.maze_[row][col]) {
               Draw(col, row, floor_);
           }
       }
   }
}

int main() {
    srand(time(NULL));

    Model model;
    View view(model);

    if (view.Construct(MAP_WIDTH + 1, MAP_HEIGHT, CELL_WIDTH, CELL_HEIGHT)) {
        view.Start();
    }

    return 0;
}
