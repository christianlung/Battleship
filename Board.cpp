#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;

const int MAX_BOARD_SHIPS = 5;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    ~BoardImpl();
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
    bool validPlacement(Point topOrLeft, int shipId, Direction dir);
    const Game& m_game;
    char board[MAXROWS][MAXCOLS];
    int fleet[MAX_BOARD_SHIPS];
    int numShips;
};

BoardImpl::BoardImpl(const Game& g) : m_game(g), numShips(0){
    for(int r=0; r<g.rows(); r++){
        for(int c=0; c<g.cols(); c++){
            board[r][c]='.';
        }
    }
}

BoardImpl::~BoardImpl(){}

void BoardImpl::clear()
{
    for(int r=0; r<MAXROWS; r++){
        for(int c=0; c<MAXCOLS; c++){
            board[r][c]='.';
        }
    }
    numShips = 0;
}

void BoardImpl::block()
{
    int blocked = 0;
    int totalBlock = m_game.rows() * m_game.cols() / 2;
    Point check;
    while(blocked<totalBlock){
        check = m_game.randomPoint();
        if(board[check.r][check.c]!='b'){
            board[check.r][check.c] = 'b';
            blocked++;
        }
    }
}

void BoardImpl::unblock()
{
    for(int r=0; r<MAXROWS; r++){
        for(int c=0; c<MAXCOLS; c++){
            if(board[r][c]=='b'){
                board[r][c] = '.';
            }
        }
    }
}

bool BoardImpl::validPlacement(Point topOrLeft, int shipId, Direction dir){
    if(shipId>=m_game.nShips() || shipId<0){
        return false;
    }
    if(dir==HORIZONTAL){
        if(topOrLeft.c + m_game.shipLength(shipId) > m_game.cols()){
            return false;
        }
    }
    else if(dir==VERTICAL){
        if(topOrLeft.r + m_game.shipLength(shipId) > m_game.rows()){
            return false;
        }
    }
    return true;
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    if(!validPlacement(topOrLeft, shipId, dir)){
        return false;
    }
    for(int i=0; i<numShips; i++){
        if(fleet[i]==shipId){
            return false;
        }
    }
    if(dir==HORIZONTAL){
        for(int d=0; d<m_game.shipLength(shipId); d++){
            if(board[topOrLeft.r][topOrLeft.c + d]!='.'){
                return false;
            }
        }
        for(int i=0; i<m_game.shipLength(shipId); i++){
            board[topOrLeft.r][topOrLeft.c + i] = m_game.shipSymbol(shipId);
        }
    }
    else if(dir==VERTICAL){
        for(int d=0; d<m_game.shipLength(shipId); d++){
            if(board[topOrLeft.r + d][topOrLeft.c]!='.'){
                return false;
            }
        }
        for(int i=0; i<m_game.shipLength(shipId); i++){
            board[topOrLeft.r + i][topOrLeft.c] = m_game.shipSymbol(shipId);
        }
    }
    numShips++;
    fleet[numShips-1] = shipId;
    return true;
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    int index=-1;
    for(int i=0; i<numShips; i++){
        if(fleet[i]==shipId){
            index = i;
            break;
        }
    }
    if(index==-1){
        return false;
    }
    
    if(dir==HORIZONTAL){
        for(int i=0; i<m_game.shipLength(shipId); i++){
            if(board[topOrLeft.r][topOrLeft.c + i] != m_game.shipSymbol(shipId)){
                return false;
            }
        }
        for(int i=0; i<m_game.shipLength(shipId); i++){
            board[topOrLeft.r][topOrLeft.c + i] = '.';
        }
    }
    else if(dir==VERTICAL){
        for(int i=0; i<m_game.shipLength(shipId); i++){
            if(board[topOrLeft.r + i][topOrLeft.c] != m_game.shipSymbol(shipId)){
                return false;
            }
        }
        for(int i=0; i<m_game.shipLength(shipId); i++){
            board[topOrLeft.r + i][topOrLeft.c] = '.';
        }
    }
    for(int i=index; i<numShips; i++){
        fleet[i] = fleet[i+1];
    }
    numShips--;
    return true;
}

void BoardImpl::display(bool shotsOnly) const
{
    cout << "  ";
    for(int i=0; i<m_game.cols(); i++){
        cout << i;
    }
    cout << endl;
    
    for (int r=0; r<m_game.rows(); r++) {
        cout << r << " ";
        for(int c=0; c<m_game.cols(); c++){
            if (shotsOnly) {
                if(board[r][c]!= 'X' && board[r][c]!='.' && board[r][c]!='o'){
                    cout << '.';
                }
                else{
                    cout << board[r][c];
                }
            }
            else{
                cout << board[r][c];
            }
        }
        cout <<endl;
    }
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    //INVALID ATTACK
    if(p.r<0 || p.r >= m_game.rows() || p.c<0 || p.c >=m_game.cols()){
        shotHit = false;
        shipDestroyed = false;
        return false;
    }
    if(board[p.r][p.c]=='X' || board[p.r][p.c]=='o'){
        return false;
    }
    
    //ATTACK HIT
    //--------------------------------------------------------------------
    //UNDESTROYED SHIP
    if(board[p.r][p.c]!='.'){
        shipDestroyed = true;
        shotHit = true;
        char symbol = board[p.r][p.c];
        board[p.r][p.c]='X';
        int length;
        for(int i=0; i<numShips; i++){
            if(m_game.shipSymbol(i)==symbol){
                length = m_game.shipLength(i);
                shipId = i;
            }
        }
        int r = p.r;
        int c = p.c;
        bool survived = false;
        
        for(int i=-length; i<length; i++){
            //HORIZONTAL
            if(c+i>=0 && c+i<m_game.cols()){
                if(board[r][c+i] == symbol){
                    survived = true;
                    shipDestroyed = false;
                    break;
                }
            }
            //VERTICAL
            if(r+i>=0 && r+i<m_game.rows()){
                if(board[r+i][c] == symbol){
                    survived = true;
                    shipDestroyed = false;
                    break;
                }
            }
        }
    }
    //ATTACK NOT HIT
    else{
        shotHit = false;
        shipDestroyed = false;
        board[p.r][p.c]='o';
    }
    return true;
}

bool BoardImpl::allShipsDestroyed() const
{
    for(int r=0; r<m_game.rows(); r++){
        for(int c=0; c<m_game.cols(); c++){
            if(board[r][c]!='X' && board[r][c]!='o' && board[r][c]!='.'){
                return false;
            }
        }
    }
    return true;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
