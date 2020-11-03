#include <array>
#include <cstdlib>
#include <random>
#include <sstream>
#include <unistd.h>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

constexpr int MAP_HEIGHT = 17;
constexpr int MAP_WIDTH = 17;
constexpr int CELL_HEIGHT = 4;
constexpr int CELL_WIDTH = 4;
constexpr int SCALE = 8;
constexpr float TICK = 1.0f / 60.0f;

using Maze = std::array<std::array<bool, MAP_WIDTH>, MAP_HEIGHT>;

enum class GO { EAST, SOUTH, WEST, NORTH };

struct Position {
    int row_;
    int col_;
};

class Model {
public:
    explicit Model(unsigned);

    void update();

    unsigned seed_;
    std::mt19937 rnd_;
    Maze maze_;
    Position entrance_;
    Position exit_;
    Position player_;
    float tick_;
    GO direction_;

private:

    Maze makeMaze();
    Position makeEntrance();
    Position makeExit();
    void tryNorth();
    void tryEast();
    void tryWest();
    void trySouth();
};

Model::Model(unsigned seed) : seed_{seed}, rnd_{seed_}, maze_{makeMaze()},
entrance_{makeEntrance()}, exit_{makeExit()}, player_{entrance_}, tick_{0},
direction_{GO::SOUTH} {
    maze_[entrance_.row_][entrance_.col_] = true;
    maze_[exit_.row_][exit_.col_] = true;
}

void Model::update() {
    if (player_.row_ == exit_.row_ && player_.col_ == exit_.col_) {
        return;
    }

    switch (direction_) {
        case GO::NORTH:
            player_.row_--;
            tryNorth();
            break;
         case GO::EAST:
            player_.col_++;
            tryEast();
            break;
        case GO::SOUTH:
            player_.row_++;
            trySouth();
            break;
        case GO::WEST:
            player_.col_--;
            tryWest();
            break;
   }
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

    std::uniform_int_distribution<int> height(1, (MAP_HEIGHT / 2));
    std::uniform_int_distribution<int> width(1,  (MAP_WIDTH / 2));
    std::uniform_int_distribution<int> direction(0, 4);

    do {
        // this code is used to make sure rows and columns are odd

        int row = 1 + (height(rnd_) - 1) * 2;
        int col = 1 + (width(rnd_) - 1) * 2;

        if (done == 0) {
            maze[row][col] = true;
        }

        if(maze[row][col]) {

            //Randomize Directions
            std::shuffle(dirs.begin(), dirs.end(), rnd_);

            bool blocked = true;

            do {
                if (direction(rnd_) == 0) {
                    std::shuffle(dirs.begin(), dirs.end(), rnd_);
                }
                blocked = true;
                for (auto i = 0; i < 4; i++) {
                    // Determine which direction the tile is
                    auto r = row + dirs[i].row_ * 2;
                    auto c = col + dirs[i].col_ * 2;
                    //Check to see if the tile can be used
                    if (r >= 1 && r < MAP_HEIGHT - 1 && c >= 1 &&
                    c < MAP_WIDTH - 1) {
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

    std::uniform_int_distribution<int> col(0,freeCols.size() - 1);

    return {0, freeCols[col(rnd_)] };
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

    std::uniform_int_distribution<int> col(0,freeCols.size() - 1);

    return {MAP_HEIGHT - 1, freeCols[col(rnd_)] };
}

void Model::tryNorth() {
    if (maze_[player_.row_][player_.col_ + 1]) {
        direction_ = GO::EAST;
    } else if(maze_[player_.row_ - 1][player_.col_]) {
        direction_ = GO::NORTH;
    } else if (maze_[player_.row_][player_.col_ - 1]) {
        direction_ = GO::WEST;
    } else if(maze_[player_.row_ + 1][player_.col_]) {
        direction_ = GO::SOUTH;
    }
}

void Model::tryEast() {
    if (maze_[player_.row_ + 1][player_.col_]) {
        direction_ = GO::SOUTH;
    } else if(maze_[player_.row_][player_.col_ + 1]) {
        direction_ = GO::EAST;
    } else if(maze_[player_.row_ - 1][player_.col_]) {
        direction_ = GO::NORTH;
    } else if(maze_[player_.row_][player_.col_ - 1]) {
        direction_ = GO::WEST;
    }
}

void Model::trySouth() {
    if (maze_[player_.row_][player_.col_ - 1]) {
        direction_ = GO::WEST;
    } else if (maze_[player_.row_ + 1][player_.col_]) {
        direction_ = GO::SOUTH;
    } else if (maze_[player_.row_][player_.col_ + 1]) {
        direction_ = GO::EAST;
    } else if (maze_[player_.row_ - 1][player_.col_]) {
        direction_ = GO::NORTH;
    }
}

void Model::tryWest() {
    if (maze_[player_.row_ - 1][player_.col_]) {
        direction_ = GO::NORTH;
        player_.row_--;
    } else if (maze_[player_.row_][player_.col_ - 1]) {
        direction_ = GO::WEST;
    } else if (maze_[player_.row_ + 1][player_.col_]) {
        direction_ = GO::SOUTH;
    } else if (maze_[player_.row_][player_.col_ + 1]) {
        direction_ = GO::EAST;
    }
}

class View : public olc::PixelGameEngine {
public:
    explicit View(Model&);

    void draw();
    bool OnUserCreate() override;
    bool OnUserUpdate(float) override;

private:
    Model& model_;
    std::unique_ptr<olc::Sprite> wall_;
    std::unique_ptr<olc::Sprite> floor_;
    std::unique_ptr<olc::Sprite> playerEast_;
    std::unique_ptr<olc::Sprite> playerNorth_;
    std::unique_ptr<olc::Sprite> playerSouth_;
    std::unique_ptr<olc::Sprite> playerWest_;
};

View::View(Model& model) : model_{model},
wall_{std::make_unique<olc::Sprite>(CELL_WIDTH, CELL_HEIGHT)},
floor_{std::make_unique<olc::Sprite>(CELL_WIDTH, CELL_HEIGHT)},
playerEast_{std::make_unique<olc::Sprite>(CELL_WIDTH, CELL_HEIGHT)},
playerNorth_{std::make_unique<olc::Sprite>(CELL_WIDTH, CELL_HEIGHT)},
playerSouth_{std::make_unique<olc::Sprite>(CELL_WIDTH, CELL_HEIGHT)},
playerWest_{std::make_unique<olc::Sprite>(CELL_WIDTH, CELL_HEIGHT)} {
    std::stringstream title;
    title << "Maze Demo | seed = " << model_.seed_;
    sAppName = title.str();
}

bool View::OnUserCreate() {
    for (auto row = 0; row < CELL_HEIGHT; ++row) {
        for (auto col = 0; col < CELL_WIDTH; ++col) {
            wall_->SetPixel(col, row, olc::BLACK);

            floor_->SetPixel(col, row, olc::WHITE);

            if (col < 2 || row == 1 || row == 2) {
                playerEast_->SetPixel(col, row, olc::MAGENTA);
            } else {
                playerEast_->SetPixel(col, row, olc::WHITE);
            }

            if (row > 1 || col == 1 || col == 2) {
                playerNorth_->SetPixel(col, row, olc::MAGENTA);
            } else {
                playerNorth_->SetPixel(col, row, olc::WHITE);
            }

            if (row < 2 || col == 1 || col == 2) {
                playerSouth_->SetPixel(col, row, olc::MAGENTA);
            } else {
                playerSouth_->SetPixel(col, row, olc::WHITE);
            }

            if (col > 1 || row == 1 || row == 2) {
                playerWest_->SetPixel(col, row, olc::MAGENTA);
            } else {
                playerWest_->SetPixel(col, row, olc::WHITE);
            }
        }
    }

    draw();

    return true;
}

bool View::OnUserUpdate(float elapsedTime) {
    model_.tick_ += elapsedTime;

    if (model_.tick_ >= TICK) {
        model_.tick_ = 0;
        model_.update();
        draw();
    }

    return true;
}

void View::draw() {
    Clear(olc::BLACK);

    for (size_t row = 0; row < MAP_HEIGHT; ++row) {
       for (size_t col = 0; col < MAP_WIDTH; ++col) {
           if (model_.maze_[row][col]) {
               DrawSprite(col * CELL_WIDTH, row * CELL_HEIGHT, floor_.get());
           } else {
               DrawSprite(col * CELL_WIDTH, row * CELL_HEIGHT, wall_.get());
           }
       }
    }

    olc::Sprite* player = nullptr;
    switch(model_.direction_) {
        case GO::EAST:
            player = playerEast_.get();
            break;
        case GO::NORTH:
            player = playerNorth_.get();
            break;
        case GO::SOUTH:
            player = playerSouth_.get();
            break;
        case GO::WEST:
            player = playerWest_.get();
            break;
    }
    DrawSprite(model_.player_.col_ * CELL_WIDTH,
        model_.player_.row_ * CELL_HEIGHT, player);
}

void usage(const char* progname, int status) {
    std::cerr <<
        "Usage: " << progname << " [-s seed]|[-h|-?]\n"
        << "    -s Seed for random number generator\n"
        << "    -h,-? Display this help message\n";
    exit(status);
}

int main(int argc, char* argv[]) {
    int opt;
    unsigned seed = 0;

    while ((opt = getopt(argc, argv, "h?s:")) != -1) {
        switch (opt) {
            case 's':
                seed = std::stoul(optarg);
                break;
            case 'h':
            case '?':
                usage(argv[0], EXIT_SUCCESS);
                break;
            default:
                usage(argv[0], EXIT_FAILURE);
                break;
        }
    }

    if (!seed) {
        seed = std::random_device()();
    }

    Model model(seed);
    View view(model);

    if (view.Construct(MAP_WIDTH * CELL_WIDTH, MAP_HEIGHT * CELL_HEIGHT, SCALE,
    SCALE)) {
        view.Start();
    }

    return EXIT_SUCCESS;
}
