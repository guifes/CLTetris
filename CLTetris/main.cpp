//
//  main.cpp
//  CLTetris
//
//  Created by Guilherme Fernandes on 21/03/19.
//  Copyright © 2019 Guifes. All rights reserved.
//

#include <iostream>
#include <time.h>
#include <stdlib.h>

#include "formats.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 16
#define PIECE_COUNT 6

typedef bool Board[BOARD_WIDTH][BOARD_HEIGHT];

struct Point
{
    int x, y;
};

struct PieceTemplate
{
    const Point (* const points)[4];
    const int rotations;
};

struct Piece
{
    const PieceTemplate *pieceTemplate;
    Point position;
    int size;
    int rotation;
};

const Point zero =    {  0,  0 };
//const Point up =      {  0, -1 };
const Point down =    {  0,  1 };
const Point left =    { -1,  0 };
const Point right =   {  1,  0 };

Point line[2][4];
Point star[4][4];
Point step[2][4];
Point caneRight[4][4];
Point caneLeft[4][4];
Point bloc[1][4];

const PieceTemplate pieceTypes[] = {
    { line, 2 },
    { star, 4 },
    { step, 2 },
    { caneRight, 4 },
    { caneLeft, 4 },
    { bloc, 1 }
};

void parseFormat(const char format[], const int len, Point* dest);

void startGame();

void printBoard(const Board board);

void generateRandomPiece(Piece& currentPiece);
void spawnPiece(Board board, const Piece& currentPiece);

bool canPieceMoveInDirection(const Board board, const Piece& currentPiece, const Point& direction);
bool canTurnPieceClockwise(const Board board, const Piece& currentPiece);
bool canTurnPieceCounterClockwise(const Board board, const Piece& currentPiece);

void erasePiece(Board board, const Piece& currentPiece);
void writePiece(Board board, const Piece& currentPiece);

bool canSpawnPiece(const Board board, const Piece& currentPiece);
int clearCompleteRows(Board board);
bool isPointEqual(const Point& a, const Point& b);
void movePiece();

int main(int argc, const char * argv[]) {
    
    srand(static_cast<unsigned int>(time(NULL)));
    
    parseFormat(lineFormat, lineFormatLen, (Point *)line);
    parseFormat(starFormat, starFormatLen, (Point *)star);
    parseFormat(stepFormat, stepFormatLen, (Point *)step);
    parseFormat(caneRightFormat, caneRightFormatLen, (Point *)caneRight);
    parseFormat(caneLeftFormat, caneLeftFormatLen, (Point *)caneLeft);
    parseFormat(blocFormat, blocFormatLen, (Point *)bloc);
    
    int score = 0;
    Board board = { 0 };
    Piece currentPiece;
    
    while(true)
    {
        generateRandomPiece(currentPiece);
        
        if(!canSpawnPiece(board, currentPiece))
        {
            // No more room for spawning pieces. Game ended.
            break;
        }
        
        spawnPiece(board, currentPiece);
        
        while(true)
        {
            printBoard(board);
            
            bool pieceMoved = false;
            Point movement = zero;
            int rotationOffset = 0;
            
            // Read input
            
            int input;
            std::cin >> input;
            
            if(input == 6)
            {
                // Quit game
                break;
            }
            
            // Try to move piece with input
            switch(input)
            {
                case 1:
                    if(canPieceMoveInDirection(board, currentPiece, left))
                    {
                        pieceMoved = true;
                        movement.x--;
                    }
                    break;
                case 3:
                    if(canPieceMoveInDirection(board, currentPiece, right))
                    {
                        pieceMoved = true;
                        movement.x++;
                    }
                    break;
                case 4:
                    if(canTurnPieceClockwise(board, currentPiece))
                    {
                        pieceMoved = true;
                        rotationOffset = 1;
                    }
                    break;
                case 5:
                    if(canTurnPieceCounterClockwise(board, currentPiece))
                    {
                        pieceMoved = true;
                        rotationOffset = currentPiece.pieceTemplate->rotations + 1;
                    }
                    break;
            }
            
            erasePiece(board, currentPiece);
            
            if(pieceMoved)
            {
                currentPiece.position.x += movement.x;
                currentPiece.position.y += movement.y;
                currentPiece.rotation = (currentPiece.rotation + rotationOffset) % currentPiece.pieceTemplate->rotations;
            }
            
            // Try to move piece with gravity
            if(canPieceMoveInDirection(board, currentPiece, down))
            {
                currentPiece.position.y += 1;
                writePiece(board, currentPiece);
            }
            else
            {
                writePiece(board, currentPiece);
                score += clearCompleteRows(board);
                
                break;
            }
        }
    }
    
    std::cout << "GAME OVER. SCORE: " << score << std::endl;
    
    return 0;
}

void parseFormat(const char format[], const int len, Point* dest)
{
    int x, y;
    int count = 0;
    Point pivot;
    int c = 0;
    
    for(int i = 0; i < len; ++i)
    {
        x = i % 4;
        y = (i % 16) / 4;
        
        if(format[c] == 32)
        {
            c++;
        }
        
        if(format[c] == 'X' || format[c] == 'P')
        {
            dest[count].x = x;
            dest[count].y = y;
            
            if(format[c] == 'P')
            {
                pivot.x = x;
                pivot.y = y;
            }
            
            count++;
        }
        
        c++;
    }
    
    for(int i = 0; i < count; ++i)
    {
        dest[i].x -= pivot.x;
        dest[i].y -= pivot.y;
        
//        printf("%d, %d\n", dest[i].x, dest[i].y);
    }
    
//    printf("\n");
}

void printBoard(const Board board)
{
    for(int j = 0; j < BOARD_WIDTH + 2; ++j)
    {
        std::cout << "|";
    }
    
    std::cout << std::endl;
    
    for(int i = 0; i < BOARD_HEIGHT; ++i)
    {
        std::cout << "|";
        
        for(int j = 0; j < BOARD_WIDTH; ++j)
        {
            std::cout << (board[j][i] ? "X" : " ");
        }
    
        std::cout << "|" << std::endl;
    }
    
    for(int j = 0; j < BOARD_WIDTH + 2; ++j)
    {
        std::cout << "|";
    }
    
    std::cout << std::endl;
}

void generateRandomPiece(Piece& currentPiece)
{
    // Randomize piece
    int randomPiece = rand() % PIECE_COUNT;
    
    const PieceTemplate *pieceTemplate = &pieceTypes[randomPiece];
    
    int randomRotation = rand() % pieceTemplate->rotations;
    
    currentPiece.pieceTemplate = pieceTemplate;
    currentPiece.position = { 5, 1 };
    currentPiece.size = 4;
    currentPiece.rotation = randomRotation;
}

void spawnPiece(Board board, const Piece& currentPiece)
{
    for(int i = 0; i < currentPiece.size; ++i)
    {
        const Point* piecePoint = &(currentPiece.pieceTemplate->points[currentPiece.rotation][i]);

        Point point = {
            piecePoint->x + currentPiece.position.x,
            piecePoint->y + currentPiece.position.y
        };

        board[point.x][point.y] = true;
    }
}

bool canPieceMoveInDirection(const Board board, const Piece& currentPiece, const Point& direction)
{
    Point newPosition = {
        currentPiece.position.x + direction.x,
        currentPiece.position.y + direction.y
    };
    
    for(int i = 0; i < currentPiece.size; ++i)
    {
        const Point* piecePoint = &currentPiece.pieceTemplate->points[currentPiece.rotation][i];
        
        Point point = {
            piecePoint->x + newPosition.x,
            piecePoint->y + newPosition.y
        };
        
        bool internalPoint = false;
        
        for(int j = 0; j < currentPiece.size; ++j)
        {
            const Point* otherPiecePoint = &currentPiece.pieceTemplate->points[currentPiece.rotation][j];
            
            Point otherPoint = {
                otherPiecePoint->x + currentPiece.position.x,
                otherPiecePoint->y + currentPiece.position.y
            };
            
            if(isPointEqual(point, otherPoint))
            {
                internalPoint = true;
                break;
            }
        }
        
        if(
           !internalPoint &&
           (
                point.x < 0 ||
                point.x >= BOARD_WIDTH ||
                point.y < 0 ||
                point.y >= BOARD_HEIGHT ||
                board[point.x][point.y]
            )
        )
        {
            return false;
        }
    }
    
    return true;
}

void erasePiece(Board board, const Piece& currentPiece)
{
    for(int i = 0; i < currentPiece.size; ++i)
    {
        const Point* piecePoint = &currentPiece.pieceTemplate->points[currentPiece.rotation][i];
        
        Point point = {
            piecePoint->x + currentPiece.position.x,
            piecePoint->y + currentPiece.position.y
        };
        
        board[point.x][point.y] = false;
    }
}

void writePiece(Board board, const Piece& currentPiece)
{
    for(int i = 0; i < currentPiece.size; ++i)
    {
        const Point* piecePoint = &currentPiece.pieceTemplate->points[currentPiece.rotation][i];
        
        Point point = {
            piecePoint->x + currentPiece.position.x,
            piecePoint->y + currentPiece.position.y
        };
        
        board[point.x][point.y] = true;
    }
}

bool canTurnPieceClockwise(const Board board, const Piece& currentPiece)
{
    int nextRotation = (currentPiece.rotation + 1) % currentPiece.pieceTemplate->rotations;
    
    for(int i = 0; i < currentPiece.size; ++i)
    {
        const Point* piecePoint = &currentPiece.pieceTemplate->points[nextRotation][i];
        
        Point point = {
            piecePoint->x + currentPiece.position.x,
            piecePoint->y + currentPiece.position.y
        };
        
        bool internalPoint = false;
        
        for(int j = 0; j < currentPiece.size; ++j)
        {
            const Point* otherPiecePoint = &currentPiece.pieceTemplate->points[currentPiece.rotation][j];
            
            Point otherPoint = {
                otherPiecePoint->x + currentPiece.position.x,
                otherPiecePoint->y + currentPiece.position.y
            };
            
            if(isPointEqual(point, otherPoint))
            {
                internalPoint = true;
                break;
            }
        }
        
        if(
           !internalPoint &&
           (
            point.x < 0 ||
            point.x >= BOARD_WIDTH ||
            point.y < 0 ||
            point.y >= BOARD_HEIGHT ||
            board[point.x][point.y]
            )
           )
        {
            return false;
        }
    }
    
    return true;
}

bool canTurnPieceCounterClockwise(const Board board, const Piece& currentPiece)
{
    int nextRotation = (currentPiece.rotation + currentPiece.pieceTemplate->rotations - 1) % currentPiece.pieceTemplate->rotations;
    
    for(int i = 0; i < currentPiece.size; ++i)
    {
        const Point* piecePoint = &currentPiece.pieceTemplate->points[nextRotation][i];
        
        Point point = {
            piecePoint->x + currentPiece.position.x,
            piecePoint->y + currentPiece.position.y
        };
        
        bool internalPoint = false;
        
        for(int j = 0; j < currentPiece.size; ++j)
        {
            const Point* otherPiecePoint = &currentPiece.pieceTemplate->points[currentPiece.rotation][j];
            
            Point otherPoint = {
                otherPiecePoint->x + currentPiece.position.x,
                otherPiecePoint->y + currentPiece.position.y
            };
            
            if(isPointEqual(point, otherPoint))
            {
                internalPoint = true;
                break;
            }
        }
        
        if(
           !internalPoint &&
           (
            point.x < 0 ||
            point.x >= BOARD_WIDTH ||
            point.y < 0 ||
            point.y >= BOARD_HEIGHT ||
            board[point.x][point.y]
            )
           )
        {
            return false;
        }
    }
    
    return true;
}

int clearCompleteRows(Board board)
{
    int offset = 0;
    int score = 0;
    
    for(int i = BOARD_HEIGHT - 1; i >= 0 ; --i)
    {
        bool rowCleared = true;
        
        for(int j = 0; j < BOARD_WIDTH; ++j)
        {
            if(!board[j][i])
            {
                rowCleared = false;
                break;
            }
        }
        
        if(rowCleared)
        {
            offset++;
            score += offset * 100;
        }
        else if(offset > 0)
        {
            int d = i + offset;
            
            for(int j = 0; j < BOARD_WIDTH; ++j)
            {
                board[j][d] = board[j][i];
            }
        }
    }
    
    return 0;
}

bool canSpawnPiece(const Board board, const Piece& currentPiece)
{
    for(int i = 0; i < currentPiece.size; ++i)
    {
        const Point* piecePoint = &(currentPiece.pieceTemplate->points[currentPiece.rotation][i]);
        
        Point point = {
            piecePoint->x + currentPiece.position.x,
            piecePoint->y + currentPiece.position.y
        };

        if(board[point.x][point.y])
        {
            return false;
        }
    }
    
    return true;
}

bool isPointEqual(const Point& a, const Point& b)
{
    return a.x == b.x && a.y == b.y;
}
