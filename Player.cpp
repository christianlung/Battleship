#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <ctype.h>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

class HumanPlayer: public Player{
public:
    HumanPlayer(string nm, const Game& g);
    //might need virtual ~HumanPlayer();
    virtual bool isHuman() const { return true; }
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    
private:
};

HumanPlayer::HumanPlayer(string nm, const Game& g) : Player(nm, g){}

bool HumanPlayer::placeShips(Board& b){
    cout << name() << " must place 5 ships." << endl;
    b.display(false);
    int r,c;
    char c_dir;
    Direction d_dir;
    for(int k = 0; k<game().nShips(); k++){
        cout << "Enter h or v for direction of " << game().shipName(k) << " (length " << game().shipLength(k) << "): ";
        cin >> c_dir;
        while(c_dir != 'h' && c_dir != 'v'){
            cout << "Direction must be h or v." << endl;
            cout << "Enter h or v for direction of " << game().shipName(k) << " (length " << game().shipLength(k) << "): ";
            cin >> c_dir;
        }
        
        d_dir = (c_dir == 'h') ? HORIZONTAL : VERTICAL;
        cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
        bool valid;
        while((valid = getLineWithTwoIntegers(r, c)) || !valid){
                    if(!valid){
                        cout << "You must enter two integers." << endl;
                        cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
                    }
                    else{
                        if(!b.placeShip(Point(r,c), k, d_dir)){
                            cout << "The ship can not be placed there." << endl;
                            cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
                            continue;
                        }
                        else{
                            break;
                        }
                    }
                }
        b.display(false);
    }
    return true;
}

Point HumanPlayer::recommendAttack(){
    cout << "Enter the row and column to attack (e.g., 3 5): ";
    int r,c;
    while(!getLineWithTwoIntegers(r, c)){
        cout << "You must enter two integers." << endl;
    }
    return Point(r,c);
}

void HumanPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */) {}

void HumanPlayer::recordAttackByOpponent(Point /* p */){}




//*********************************************************************
//  MediocrePlayer
//*********************************************************************

class MediocrePlayer: public Player{
public:
    MediocrePlayer(string nm, const Game& g);
    //might need virtual ~MediocrePlayer();
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    
private:
    bool fitShips(Board& b, int k);
    bool spotsLeft();
    Point refHit; //reference point that first hit the ship
    bool isDestroyed; //is it destroyed
    char board[MAXROWS][MAXCOLS];
};

MediocrePlayer::MediocrePlayer(string nm, const Game& g) : Player(nm, g), refHit(Point(-1,-1)), isDestroyed(false){
    for(int r=0; r<g.rows(); r++){
        for(int c=0; c<g.cols(); c++){
            board[r][c] = '.';
        }
    }
}

bool MediocrePlayer::placeShips(Board& b){
    for(int trial = 0 ; trial<50; trial++){
        b.block();
        bool success = fitShips(b,0);
        b.unblock();
        if(success){
            return true;
        }
    }
    return false;
}

bool MediocrePlayer::fitShips(Board& b, int k){
    if(k==game().nShips()){
        return true;
    }
    for(int r=0; r<game().rows(); r++){
        for(int c=0; c<game().cols(); c++){
            if(b.placeShip(Point(r,c), k, HORIZONTAL)){
                if(!fitShips(b, k+1)){
                    b.unplaceShip(Point(r,c), k, HORIZONTAL);
                }
                return true;
            }
            else if (b.placeShip(Point(r,c), k, VERTICAL)){
                if(!fitShips(b, k+1)){
                    b.unplaceShip(Point(r,c), k, VERTICAL);
                }
                return true;
            }
        }
    }
    return false;
}

bool MediocrePlayer::spotsLeft(){
    //CHECKS HORIZONTALLY FIRST
    for(int col = refHit.c -4; col<= refHit.c+4; col++){
        if(col >= 0 && col <game().cols()){
            if(board[refHit.r][col]=='.'){
                return true;
            }
        }
    }
    //CHECKS VERTICALLY
    for(int row = refHit.r -4; row <=refHit.r +4; row++){
        if(row >= 0 && row<game().rows()){
            if(board[row][refHit.c]=='.'){
                return true;
            }
        }
    }
    return false;
}

Point MediocrePlayer::recommendAttack(){
    int halfChance; //50% chance it is vertical or horizontal
    int randRange;  //make a random number from -4 to 4
    //does have a reference point not yet destroyed
    if((refHit.r!=-1 && refHit.c!=-1) && isDestroyed == false){
        do{
            halfChance = rand()%2;
            randRange = rand()%9 -4;
            //CHECKS HORIZONTALLY
            if(halfChance==0){
                if(board[refHit.r][refHit.c+randRange] == '.' && refHit.c+randRange>=0 && refHit.c+randRange <game().cols()){
                    return Point(refHit.r,refHit.c + randRange);

                }
            }
            //CHECKS VERTICALLY
            else{
                if(board[refHit.r+randRange][refHit.c] == '.' && refHit.r+randRange>=0 && refHit.r+randRange <game().rows()){
                    return Point(refHit.r + randRange, refHit.c);
                }
            }
        } while(spotsLeft());
    }
    
    //if you go through the cross without finding an untouched point, then select random point
    Point recAttack;
    do{
        recAttack = game().randomPoint();
    } while(board[recAttack.r][recAttack.c]!='.');
    return recAttack;
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId){
    //if point p is in range of the cross of refHit, don't update refHit
    if(validShot){
        if(shotHit){
            board[p.r][p.c] = 'X';
            
            //if outside of the cross, then make that the new reference point
            bool insideCross = false;
            //CHECKS HORIZONTAL
            if(p.r == refHit.r){
                if(p.c>=refHit.c-4 && p.c<=refHit.c+4){
                    insideCross = true;
                }
            }
            //CHECKS VERTICAL
            if(p.c == refHit.c){
                if(p.r>=refHit.r-4 && p.r<=refHit.r+4){
                    insideCross = true;
                }
            }
            if(!insideCross){
                refHit = p;
                isDestroyed = false;
            }
            if(shipDestroyed){
                refHit = Point(-1,-1);
                isDestroyed = true;
            }
        }
        else{
            board[p.r][p.c] = 'o';
        }
    }
}

void MediocrePlayer::recordAttackByOpponent(Point p){}

//*********************************************************************
//  GoodPlayer
//*********************************************************************

class GoodPlayer : public Player{
public:
    GoodPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    
private:
    Point refHit;
    bool isDestroyed;
    int searchDirection; //0 for north, 1 for east, 2 for south, 3 for west, -1 for unset
    int attackDirection; //0 for north, 1 for east, 2 for south, 3 for west, -1 for unset
    int attackLength;
    char attackBoard[MAXROWS][MAXCOLS];
    bool placeBoard[MAXROWS][MAXCOLS];
    bool validPlacement(Point p, int shipId, Direction dir);
    void placeSides(Point p, int shipId, Direction dir, bool placed); //to place and unplace blocks on sides
    bool fitShips(Board& b, int k, bool oneApart);
    void switchOppositeDirection(int& direction);
    };

GoodPlayer::GoodPlayer(string nm, const Game& g): Player(nm, g), refHit(Point(-1,-1)), isDestroyed(false), searchDirection(0), attackDirection(-1), attackLength(1){
    for(int r=0; r<game().rows(); r++){
        for(int c=0; c<game().cols(); c++){
            attackBoard[r][c] = '.';
            placeBoard[r][c] = true;
        }
    }
}

bool GoodPlayer::validPlacement(Point p, int shipId, Direction dir){
    int length = game().shipLength(shipId);
    if (dir==VERTICAL) {
        for(int r=0; r<length; r++){
            if(placeBoard[p.r+r][p.c] == false){
                return false;
            }
        }
        return true;
    }
    else{
        for(int c=0; c<length; c++){
            if(placeBoard[p.r][p.c+c] == false){
                return false;
            }
        }
        return true;
    }
}

void GoodPlayer::placeSides(Point p, int shipId, Direction dir, bool placed){
    int length = game().shipLength(shipId);
    if (dir == VERTICAL) {
        for(int r=0; r<length; r++){
            if(p.c-1>=0){
                placeBoard[p.r+r][p.c-1] = !placed;
            }
            if(p.c+1<game().cols()){
                placeBoard[p.r+r][p.c+1] = !placed;
            }
            placeBoard[p.r+r][p.c] = !placed;
        }
        if(p.r-1>=0){
            placeBoard[p.r-1][p.c] = !placed;
        }
        if(p.r+length<game().rows()){
            placeBoard[p.r+length][p.c] = !placed;
        }
    }
    else if(dir == HORIZONTAL){
        for(int c=0; c<length; c++){
            if(p.r-1>=0){
                placeBoard[p.r-1][p.c+c] = !placed;
            }
            if(p.r+1<game().rows()){
                placeBoard[p.r+1][p.c+c] = !placed;
            }
            placeBoard[p.r][p.c+c] = !placed;
        }
        if(p.c-1>=0){
            placeBoard[p.r][p.c-1] = !placed;
        }
        if(p.c+length<game().cols()){
            placeBoard[p.r][p.c+length] = !placed;
        }
    }
}

bool GoodPlayer::fitShips(Board& b, int k, bool oneApart){
    if(k==game().nShips()){
        return true;
    }
    for(int r=0; r<game().rows(); r++){
        for(int c=0; c<game().cols(); c++){
            if(validPlacement(Point(r,c),k,HORIZONTAL) && b.placeShip(Point(r,c), k, HORIZONTAL)){
                //if can place, then block out sides
                if(oneApart){
                    placeSides(Point(r,c), k, HORIZONTAL, true);
                }
                if(!fitShips(b, k+1, oneApart)){
                    b.unplaceShip(Point(r,c), k, HORIZONTAL);
                    //if can't place, then unblock out sides
                    if(oneApart){
                        placeSides(Point(r,c), k, HORIZONTAL, false);

                    }
                }
                return true;
            }
            else if (validPlacement(Point(r,c), k, VERTICAL) && b.placeShip(Point(r,c), k, VERTICAL)){
                //if can place, then block out sides
                if(oneApart){
                    placeSides(Point(r,c), k, VERTICAL, true);
                }
                if(!fitShips(b, k+1, oneApart)){
                    b.unplaceShip(Point(r,c), k, VERTICAL);
                    //if can't place, then unblock out sides
                    if(oneApart){
                        placeSides(Point(r,c), k, VERTICAL, false);
                    }
                }
                return true;
            }
        }
    }
    return false;
}

bool GoodPlayer::placeShips(Board& b){
    for(int trial = 0 ; trial<50; trial++){
        b.block();
        b.display(false);
        bool success = fitShips(b,0,true);
        b.unblock();
        if(success){
            b.display(false); //see if need to comment this out later
            return true;
        }
    }
    for(int trial = 0 ; trial<50; trial++){
        b.block();
        bool success = fitShips(b,0,false);
        b.unblock();
        if(success){
            b.display(false); //see if need to comment this out later
            return true;
        }
    }
    return false;
}

void GoodPlayer::switchOppositeDirection(int& direction){
    switch (direction) {
        case 0:
            direction = 2;
            break;
        case 1:
            direction = 3;
            break;
        case 2:
            direction = 0;
            break;
        case 3:
            direction = 1;
            break;
    }
}

Point GoodPlayer::recommendAttack(){
    //does have a reference point not yet destroyed
    if((refHit.r!=-1 && refHit.c!=-1) && isDestroyed == false){
        //SEARCHING
        if(attackDirection==-1){
            //if can't check one direction, check next direction
            if(searchDirection == 0 && (refHit.r-1<0 || attackBoard[refHit.r-1][refHit.c]!='.')){
                searchDirection++;
            }
            if(searchDirection == 1 && (refHit.c+1>=game().cols() || attackBoard[refHit.r][refHit.c+1]!='.')){
                searchDirection++;
            }
            if(searchDirection == 2 && (refHit.r+1>=game().rows() || attackBoard[refHit.r+1][refHit.c]!='.')){
                searchDirection++;
            }
            if(searchDirection == 3 && (refHit.c-1<0 || attackBoard[refHit.r][refHit.c-1]!='.')){
                searchDirection++;
            }

            switch(searchDirection){
                case 0:
                    return Point(refHit.r-1, refHit.c);
                case 1:
                    return Point(refHit.r, refHit.c+1);
                case 2:
                    return Point(refHit.r+1, refHit.c);
                case 3:
                    return Point(refHit.r, refHit.c-1);
            }
        }
        //ATTACKING
        else{
            
            //if direction is not available or occupied, switch direction
            if((attackDirection == 0 && (refHit.r-attackLength<0 || attackBoard[refHit.r-attackLength][refHit.c]!='.')) ||
              (attackDirection == 1 && (refHit.c+attackLength>=game().cols() || attackBoard[refHit.r][refHit.c+attackLength]!='.')) ||
               (searchDirection == 2 && (refHit.r+attackLength>=game().rows() || attackBoard[refHit.r+attackLength][refHit.c]!='.')) ||
               (searchDirection == 3 && (refHit.c-attackLength<0 || attackBoard[refHit.r][refHit.c-attackLength]!='.'))){
                searchDirection = attackDirection+1;
                attackDirection=-1;
                attackLength = 1;
                switch(searchDirection){
                    case 0:
                        return Point(refHit.r-1, refHit.c);
                    case 1:
                        return Point(refHit.r, refHit.c+1);
                    case 2:
                        return Point(refHit.r+1, refHit.c);
                    case 3:
                        return Point(refHit.r, refHit.c-1);
                }
            }
            
            //attack in specified direction
            switch (attackDirection){
                case 0:
                    return Point(refHit.r-attackLength, refHit.c);
                case 1:
                    return Point(refHit.r, refHit.c+attackLength);
                case 2:
                    return Point(refHit.r+attackLength, refHit.c);
                case 3:
                    return Point(refHit.r, refHit.c-attackLength);
            }
        }
        
    }
    //if you go through the cross without finding an untouched point, then select random point
    Point recAttack;
    do{
        recAttack = game().randomPoint();
    } while(attackBoard[recAttack.r][recAttack.c]!='.');
    return recAttack;
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId){
    //if point p is in range of the cross of refHit, don't update refHit
    if(validShot){
        if(shotHit){
            attackBoard[p.r][p.c] = 'X';
            
            //if outside of the cross, then make that the new reference point
            bool insideCross = false;
            //CHECKS HORIZONTAL
            if(p.r == refHit.r){
                if(p.c>=refHit.c-4 && p.c<=refHit.c+4){
                    insideCross = true;
                }
            }
            //CHECKS VERTICAL
            if(p.c == refHit.c){
                if(p.r>=refHit.r-4 && p.r<=refHit.r+4){
                    insideCross = true;
                }
            }
            if(insideCross){
                if(attackDirection ==-1){
                    attackDirection = searchDirection;
                    attackLength = 1;
                }
                attackLength++;
                //if it's not 4 in one direction, check the other side
                if (attackLength>4) {
                    attackLength = 1;
                    switchOppositeDirection(attackDirection);
                }
            }
            if(!insideCross){
                refHit = p;
                isDestroyed = false;
                attackLength = 1;
                searchDirection = 0;
                attackDirection = -1;
            }
            if(shipDestroyed){
                refHit = Point(-1,-1);
                isDestroyed = false;
                attackLength = 1;
                searchDirection = 0;
                attackDirection = -1;
            }
        }
        else{
            attackBoard[p.r][p.c] = 'o';
            if(attackDirection!=-1){
                switchOppositeDirection(attackDirection);
                attackLength = 1;
            }
            else{
                searchDirection=0;
            }
        }
    }
}

void GoodPlayer::recordAttackByOpponent(Point p){}

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}
