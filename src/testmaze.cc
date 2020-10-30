#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <utility>
using namespace std;

static const int MAP_HEIGHT = 17;
static const int MAP_WIDTH = 17;

int main() {
    srand(time(NULL));
    array<array<bool, MAP_WIDTH>, MAP_HEIGHT> _map;

    for (auto & row : _map) {
        for (auto & col : row) {
            col = false;
        }
    }

    vector<pair<int, int>> dirs {
        make_pair(-1,0),
        make_pair(1,0),
        make_pair(0,-1),
        make_pair(0,1)
    };

    int done = 0;

    do {
        // this code is used to make sure the numbers are odd

        int row = 1 + rand() % ((MAP_HEIGHT - 1) / 2) * 2;
        int col = 1 + rand() % ((MAP_WIDTH - 1) / 2) * 2;

        if (done == 0) {
            _map[row][col] = true;
        }

        if(_map[row][col]) {

            //Randomize Directions
            random_shuffle(dirs.begin(), dirs.end());

            bool blocked = true;

            do {
                if (rand() % 5 == 0) {
                    random_shuffle(dirs.begin(), dirs.end());
                }
                blocked = true;
                for (int i = 0; i < 4; i++) {
                    // Determine which direction the tile is
                    int r = row + dirs[i].first * 2;
                    int c = col + dirs[i].second * 2;
                    //Check to see if the tile can be used
                    if (r >= 1 && r < MAP_HEIGHT - 1 && c >= 1 && c < MAP_WIDTH - 1) {
                        if (!_map[r][c]) {
                            //create destination location
                            _map[r][c] = true;
                            //create intermediate location
                            _map[row + dirs[i].first][col + dirs[i].second] = true;
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

    vector<int> freeCols;
    for (int i = 1; i < MAP_WIDTH - 1; i++) {
        if (_map[1][i]) {
            freeCols.push_back(i);
        }
    }
    _map[0][freeCols[rand() % freeCols.size()]] = true;

    freeCols.clear();
    for (int i = 1; i < MAP_WIDTH - 1; i++) {
        if (_map[MAP_HEIGHT - 2][i]) {
            freeCols.push_back(i);
        }
    }
    _map[MAP_HEIGHT - 1][freeCols[rand() % freeCols.size()]] = true;

    for (auto & row : _map) {
        for (auto & col : row) {
            putchar(col ? '.' : '#');
        }
        putchar('\n');
    }

    return 0;
}
