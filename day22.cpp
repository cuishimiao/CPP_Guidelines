#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <thread>
#include <mutex>

using namespace std;

enum Direction { UP, DOWN, LEFT, RIGHT };

class Food {
public:
    COORD position;
    Food() {
        generate();
    }
    void generate() {
        position.X = rand() % 40;
        position.Y = rand() % 20;
    }
};

class Snake {
public:
    vector<COORD> body;
    Direction dir;
    Snake() {
        COORD start = { 20, 10 };
        body.push_back(start);
        dir = RIGHT;
    }
    void move() {
        COORD newHead = body.front();
        switch (dir) {
            case UP: newHead.Y--; break;
            case DOWN: newHead.Y++; break;
            case LEFT: newHead.X--; break;
            case RIGHT: newHead.X++; break;
        }
        body.insert(body.begin(), newHead);
        body.pop_back();
    }
    void grow() {
        COORD newHead = body.front();
        switch (dir) {
            case UP: newHead.Y--; break;
            case DOWN: newHead.Y++; break;
            case LEFT: newHead.X--; break;
            case RIGHT: newHead.X++; break;
        }
        body.insert(body.begin(), newHead);
    }
    bool checkCollision() {
        COORD head = body.front();
        if (head.X < 0 || head.X >= 40 || head.Y < 0 || head.Y >= 20) return true;
        for (size_t i = 1; i < body.size(); i++) {
            if (body[i].X == head.X && body[i].Y == head.Y) return true;
        }
        return false;
    }
};

class Game {
public:
    Snake snake;
    Food food;
    bool gameOver;
    mutex mtx;
    Game() : gameOver(false) {}
    void draw() {
        system("cls");
        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 40; x++) {
                if (x == food.position.X && y == food.position.Y) {
                    cout << "F";
                } else {
                    bool isBody = false;
                    for (auto& part : snake.body) {
                        if (part.X == x && part.Y == y) {
                            cout << "O";
                            isBody = true;
                            break;
                        }
                    }
                    if (!isBody) cout << " ";
                }
            }
            cout << endl;
        }
    }
    void input() {
        while (!gameOver) {
            if (_kbhit()) {
                char ch = _getch();
                mtx.lock();
                switch (ch) {
                    case 'w': if (snake.dir != DOWN) snake.dir = UP; break;
                    case 's': if (snake.dir != UP) snake.dir = DOWN; break;
                    case 'a': if (snake.dir != RIGHT) snake.dir = LEFT; break;
                    case 'd': if (snake.dir != LEFT) snake.dir = RIGHT; break;
                }
                mtx.unlock();
            }
        }
    }
    void logic() {
        while (!gameOver) {
            mtx.lock();
            snake.move();
            if (snake.body.front().X == food.position.X && snake.body.front().Y == food.position.Y) {
                snake.grow();
                food.generate();
            }
            if (snake.checkCollision()) {
                gameOver = true;
            }
            mtx.unlock();
            draw();
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }
    void run() {
        thread t1(&Game::input, this);
        thread t2(&Game::logic, this);
        t1.join();
        t2.join();
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}