#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>

using namespace std;

const int MAX_SHIPS = 10;

class Board;
class GameImpl
{
  public:
    GameImpl(int nRows, int nCols);
    ~GameImpl();
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
    
private:
    struct ShipType{
        int length;
        char symbol;
        string name;
        int ID;
        ShipType(int l, char s, string n, int i){
            length = l;
            symbol = s;
            name = n;
            ID = i;
        }
    };
    int row;
    int col;
    int numShips;
    ShipType* type[MAX_SHIPS];
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols): row(nRows), col(nCols), numShips(0){}

GameImpl::~GameImpl(){
    for(int i=0; i<numShips; i++){
        delete type[i];
    }
};

int GameImpl::rows() const
{
    return row;
}

int GameImpl::cols() const
{
    return col;  
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    if(length>rows() || length>cols() || length<0){
        return false;
    }
    if(symbol == 'X' || symbol == 'o' || symbol == '.' || isprint(symbol)==0){
        return false;
    }
    for(int i=0; i<numShips; i++){
        if(symbol == type[i]->symbol){
            return false;
        }
    }
    if(numShips<MAX_SHIPS){
        type[numShips] = new ShipType(length, symbol, name, numShips);
        numShips++;
    }
    return true;
}

int GameImpl::nShips() const
{
    return numShips;
}

int GameImpl::shipLength(int shipId) const
{
    for(int i=0; i<numShips; i++){
        if(type[i]->ID==shipId){
            return type[i]->length;
        }
    }
    return -1;
}

char GameImpl::shipSymbol(int shipId) const
{
    for(int i=0; i<numShips; i++){
        if(type[i]->ID==shipId){
            return type[i]->symbol;
        }
    }
    return '?';
}

string GameImpl::shipName(int shipId) const
{
    for(int i=0; i<numShips; i++){
        if(type[i]->ID==shipId){
            return type[i]->name;
        }
    }
    return "";
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    if(!p1->placeShips(b1)){ return nullptr; }
    if(!p2->placeShips(b2)){ return nullptr; }
    while(!b1.allShipsDestroyed() && !b2.allShipsDestroyed()){
        bool shotHit2;
        bool shipDestroyed2;
        int shipId2;
        bool validAttack2;
        Point point2;
        cout << p1->name() << "'s turn.  Board for " << p2->name() << ":"<<endl;
        (p1->isHuman() ? b2.display(true) : b2.display(false));
        point2 = p1->recommendAttack();
        validAttack2 = b2.attack(point2, shotHit2, shipDestroyed2, shipId2);
        p1->recordAttackResult(point2, validAttack2, shotHit2, shipDestroyed2, shipId2);
        if(!validAttack2){
            cout << p1->name() << " wasted a shot at " << "("<<point2.r<<","<<point2.c<<")." << endl;
        }
        else if(shipDestroyed2){
            cout << p1->name() << " attacked (" << point2.r << "," << point2.c<<") and " << "destroyed the " << shipName(shipId2) << ", resulting in:" << endl;
            (p1->isHuman() ? b2.display(true) : b2.display(false));
        }
        else{
            cout << p1->name() << " attacked (" << point2.r << "," << point2.c<<") and " << (shotHit2 ? "hit something" : "missed") << ", resulting in:" << endl;
            (p1->isHuman() ? b2.display(true) : b2.display(false));
        }
        
        if(b1.allShipsDestroyed() || b2.allShipsDestroyed()){ break; }
        
        if(shouldPause){ waitForEnter(); }
        
        
        bool shotHit1;
        bool shipDestroyed1;
        int shipId1;
        bool validAttack1;
        Point point1;
        cout << p2->name() << "'s turn.  Board for " << p1->name() << ":"<<endl;
        (p2->isHuman() ? b1.display(true) : b1.display(false));
        point1 = p2->recommendAttack();
        validAttack1 = b1.attack(point1, shotHit1, shipDestroyed1, shipId1);
        p2->recordAttackResult(point1, validAttack1, shotHit1, shipDestroyed1, shipId1);
        if(!validAttack1){
            cout << p2->name() << " wasted a shot at " << "("<<point1.r<<","<<point1.c<<")." << endl;
        }
        else if(shipDestroyed1){
            cout << p2->name() << " attacked (" << point1.r << "," << point1.c<<") and " << "destroyed the " << shipName(shipId1) << ", resulting in:" << endl;
            (p2->isHuman() ? b1.display(true) : b1.display(false));
        }
        else{
            cout << p2->name() << " attacked (" << point1.r << "," << point1.c<<") and " << (shotHit1 ? "hit something" : "missed") << ", resulting in:" << endl;
            (p2->isHuman() ? b1.display(true) : b1.display(false));
        }
        
        if(b1.allShipsDestroyed() || b2.allShipsDestroyed()){ break; }
        if(shouldPause){ waitForEnter(); }
    }
    Player* winner;
    Player* loser;
    winner = b1.allShipsDestroyed() ? p2 : p1;
    loser = (winner==p1 ? p2 : p1);
    if(loser==p1 && p1->isHuman()){
        b2.display(false);
    }
    else if(loser==p2 && p2->isHuman()){
        b1.display(false);
    }
    //NOT SURE ORDERING of displaying board if human
    cout << winner->name() << " wins!" << endl;
    cout << "Saving session..." << endl;
    cout << "...copying shared history..." << endl;
    cout << "...saving history...truncating history files..." << endl;
    cout << "...completed." << endl;
    cout << endl;
    cout << "[Process completed]" <<endl;
    return winner;
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

