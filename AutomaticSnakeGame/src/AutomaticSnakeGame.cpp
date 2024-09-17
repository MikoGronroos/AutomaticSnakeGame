#include <iostream>
#include "Pathfinding.h"
#include "AutomaticSnakeGame.h"
#include <random>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

constexpr std::chrono::nanoseconds timestep(50ms);

const int WIDTH = 10;
const int HEIGHT = 10;

int appleXPos = 5;
int appleYPos = 5;

int playerHeadXPos = 1;
int playerHeadYPos = 1;

int playerSize = 1;

Map map = Map(WIDTH, HEIGHT);

bool isGameRunning = true;

std::vector<Node*> route;

class PlayerBodyPart {
public:
    PlayerBodyPart(int x, int y) {
        this->x = x;
        this->y = y;
    }
    int x;
    int y;
};

std::vector<PlayerBodyPart> playerBodyParts;

int main()
{

    using clock = std::chrono::high_resolution_clock;

    std::chrono::nanoseconds lag(0ns);
    auto time_start = clock::now();

    PlayerBodyPart part = PlayerBodyPart(playerHeadXPos, playerHeadYPos);
    playerBodyParts.push_back(part);

    while (isGameRunning) {

        auto delta_time = clock::now() - time_start;
        time_start = clock::now();
        lag += std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time);

        while (lag >= timestep) {
            lag -= timestep;
            update();
            render();
        }

    }

    std::cout << "Game ended" << std::endl;
}

void update() 
{
    if (checkIfPlayerGetsPoint()) {
        clearBoard();
        playerSize++;
        srand((unsigned int)time(NULL));
        appleXPos = rand() % WIDTH;
        appleYPos = rand() % HEIGHT;
        Node* node = map.GetNode(appleXPos, appleYPos);
        while (node->isBlocked == true) {
            appleXPos = rand() % WIDTH;
            appleYPos = rand() % HEIGHT;
            node = map.GetNode(appleXPos, appleYPos);
        }
        route = GetRoute(map.GetNode(playerBodyParts[0].x, playerBodyParts[0].y), map.GetNode(appleXPos, appleYPos));
    }

    if (route.size() > 0) {
        if (route.size() <= 1) {
            if (route[0]->x != appleXPos || route[0]->y != appleYPos) {
                isGameRunning = false;
                return;
            }
        }
        Node* node = route[1];
        int previousPosX = node->x;
        int previousPosY = node->y;
        if (playerSize > playerBodyParts.size()) {
            playerBodyParts.push_back(PlayerBodyPart(0, 0));
        }
        for (int i = 0; i < playerBodyParts.size(); i++)
        {
            int prevX = playerBodyParts[i].x;
            int prevY = playerBodyParts[i].y;
            playerBodyParts[i].x = previousPosX;
            playerBodyParts[i].y = previousPosY;
            map.GetNode(playerBodyParts[i].x, playerBodyParts[i].y)->isBlocked = true;
            map.GetNode(prevX, prevY)->isBlocked = false;
            previousPosX = prevX;
            previousPosY = prevY;
        }
        route = GetRoute(map.GetNode(playerBodyParts[0].x, playerBodyParts[0].y), map.GetNode(appleXPos, appleYPos));
    }
    else
    {
        route = GetRoute(map.GetNode(playerBodyParts[0].x, playerBodyParts[0].y), map.GetNode(appleXPos, appleYPos));
    }
}

void render()
{
    clearBoard();
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            char character = determineNodeCharacter(x, y);
            std::cout << character << "  |";
        }
        std::cout << std::endl;
    }
}


char determineNodeCharacter(int x, int y) 
{
    for (int i = 0; i < playerBodyParts.size(); i++)
    {
        PlayerBodyPart part = playerBodyParts[i];
        if (part.x == x && part.y == y) {
            if (i == 0) {
                return 'A';
            }
            else {
                return 'x';
            }
        }
    }
    if (x == appleXPos && y == appleYPos) {
        return 'o';
    }
    return ' ';
}

bool checkIfPlayerGetsPoint() 
{
    return playerBodyParts[0].x == appleXPos && playerBodyParts[0].y == appleYPos;
}

void clearBoard() {
    system("cls");
}