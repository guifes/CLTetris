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

#include "pieces.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 16
#define PIECE_COUNT 6
#define PIECE_SIZE 4

typedef bool Board[BOARD_WIDTH][BOARD_HEIGHT];

struct Point
{
    int x, y;
};

struct PieceTemplate
{
    const Point (* const points)[PIECE_SIZE];
    const int rotations;
};

struct Piece
{
    const PieceTemplate *pieceTemplate;
    Point position;
    int rotation;
};

const Point zero =    {  0,  0 };
//const Point up =      {  0, -1 };
const Point down =    {  0,  1 };
const Point left =    { -1,  0 };
const Point right =   {  1,  0 };

Point line[2][PIECE_SIZE];
Point star[4][PIECE_SIZE];
Point step[2][PIECE_SIZE];
Point caneRight[4][PIECE_SIZE];
Point caneLeft[4][PIECE_SIZE];
Point bloc[1][PIECE_SIZE];

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

void setPieceInBoard(Board board, const Piece& currentPiece, const bool clear);

bool canPieceMoveInDirection(const Board board, const Piece& currentPiece, const Point& direction);
bool canTurnPiece(const Board board, const Piece& currentPiece, const bool clockwise);

int clearCompleteRows(Board board);

bool isPointEqual(const Point& a, const Point& b);
bool isPointInBoard(const Point& point);
bool checkPieceCollision(const Board board, const PieceTemplate& pieceTemplate, const Point& position, const int rotation);
Point getPointFromPiece(const PieceTemplate& pieceTemplate, const int index, const Point& position, const int rotation);

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
        
        if(!checkPieceCollision(board, *(currentPiece.pieceTemplate), currentPiece.position, currentPiece.rotation))
        {
            // No more room for spawning pieces. Game ended.
            break;
        }
        
        setPieceInBoard(board, currentPiece, false);
        
        while(true)
        {
            printBoard(board);
            
            setPieceInBoard(board, currentPiece, true);
            
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
                    if(canTurnPiece(board, currentPiece, true))
                    {
                        pieceMoved = true;
                        rotationOffset = 1;
                    }
                    break;
                case 5:
                    if(canTurnPiece(board, currentPiece, false))
                    {
                        pieceMoved = true;
                        rotationOffset = currentPiece.pieceTemplate->rotations - 1;
                    }
                    break;
            }
            
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
                setPieceInBoard(board, currentPiece, false);
            }
            else
            {
                setPieceInBoard(board, currentPiece, false);
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
    int c = 0;
    Point pivot;
    
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
    currentPiece.rotation = randomRotation;
}

Point getPointFromPiece(const PieceTemplate& pieceTemplate, const int index, const Point& position, const int rotation)
{
    const Point* point = &(pieceTemplate.points[rotation][index]);
    
    return {
        point->x + position.x,
        point->y + position.y
    };
}

bool canPieceMoveInDirection(const Board board, const Piece& currentPiece, const Point& direction)
{
    Point newPosition = {
        currentPiece.position.x + direction.x,
        currentPiece.position.y + direction.y
    };
    
    return checkPieceCollision(board, *(currentPiece.pieceTemplate), newPosition, currentPiece.rotation);
}

bool canTurnPiece(const Board board, const Piece& currentPiece, bool clockwise)
{
    int rotationOffset = clockwise ? 1 : currentPiece.pieceTemplate->rotations - 1;
    int nextRotation = (currentPiece.rotation + rotationOffset) % currentPiece.pieceTemplate->rotations;
    
    return checkPieceCollision(board, *(currentPiece.pieceTemplate), currentPiece.position, nextRotation);
}

void setPieceInBoard(Board board, const Piece& currentPiece, const bool erase)
{
    for(int i = 0; i < PIECE_SIZE; ++i)
    {
        Point point = getPointFromPiece(*(currentPiece.pieceTemplate), i, currentPiece.position, currentPiece.rotation);
        
        board[point.x][point.y] = !erase;
    }
}

bool checkPieceCollision(const Board board, const PieceTemplate& pieceTemplate, const Point& position, const int rotation)
{
    for(int i = 0; i < PIECE_SIZE; ++i)
    {
        Point point = getPointFromPiece(pieceTemplate, i, position, rotation);

        if(!isPointInBoard(point) || board[point.x][point.y])
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
    
    return score;
}

bool isPointEqual(const Point& a, const Point& b)
{
    return a.x == b.x && a.y == b.y;
}

bool isPointInBoard(const Point& point)
{
    return (
        point.x >= 0 &&
        point.x < BOARD_WIDTH &&
        point.y >= 0 &&
        point.y < BOARD_HEIGHT
    );
}
