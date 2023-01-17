#include "Game.h"
#include "Player.h"
#include "globals.h"
#include "Board.h"
#include <iostream>
#include <string>
#include <cassert>

using namespace std;

bool addStandardShips(Game& g)
{
    return g.addShip(5, 'A', "aircraft carrier")  &&
           g.addShip(4, 'B', "battleship")  &&
           g.addShip(3, 'D', "destroyer")  &&
           g.addShip(3, 'S', "submarine")  &&
           g.addShip(2, 'P', "patrol boat");
}

bool addOtherShips(Game& g)
{
    return //g.addShip(5, 'A', "aircraft carrier")  &&
           //g.addShip(4, 'B', "battleship")  &&
           g.addShip(3, 'D', "destroyer")  &&
           g.addShip(3, 'S', "submarine")  &&
           g.addShip(2, 'P', "patrol boat"); //remember to put semicolon
//           g.addShip(3, 'T', "test1 boat") &&
//           g.addShip(2, 'Y', "test2 boat") &&
//           g.addShip(3, 'V', "test3 boat") &&
//           g.addShip(3, 'L', "test4 boat") &&
//           g.addShip(3, 'W', "test5 boat") &&
//           g.addShip(3, 'U', "test6 boat");
    
}


int main()
{
    const int NTRIALS = 100;

    cout << "Select one of these choices for an example of the game:" << endl;
    cout << "  1.  A mini-game between two mediocre players" << endl;
    cout << "  2.  A mediocre player against a human player" << endl;
    cout << "  3.  A " << NTRIALS
         << "-game match between a mediocre and an awful player, with no pauses"
         << endl;
    cout << "Enter your choice: ";
    string line;
    getline(cin,line);
    if (line.empty())
    {
        cout << "You did not enter a choice" << endl;
    }
    else if (line[0] == '1')
    {
        Game g(2, 3);
        g.addShip(2, 'R', "rowboat");
        Player* p1 = createPlayer("mediocre", "Popeye", g);
        Player* p2 = createPlayer("mediocre", "Bluto", g);
        cout << "This mini-game has one ship, a 2-segment rowboat." << endl;
        g.play(p1, p2);
        delete p1;
        delete p2;
    }
    else if (line[0] == '2')
    {
        Game g(10, 10);
        addStandardShips(g);
        Player* p1 = createPlayer("mediocre", "Mediocre Midori", g);
        Player* p2 = createPlayer("human", "Shuman the Human", g);
        g.play(p1, p2);
        delete p1;
        delete p2;
    }
    else if (line[0] == '3')
    {
        int nMediocreWins = 0;

        for (int k = 1; k <= NTRIALS; k++)
        {
            cout << "============================= Game " << k
                 << " =============================" << endl;
            Game g(10, 10);
            addStandardShips(g);
            Player* p1 = createPlayer("awful", "Awful Aubrey", g);
            Player* p2 = createPlayer("mediocre", "Mediocre Mimi", g);
            Player* winner = (k % 2 == 1 ?
                                g.play(p1, p2, false) : g.play(p2, p1, false));
            if (winner == p2)
                nMediocreWins++;
            delete p1;
            delete p2;
        }
        cout << "The mediocre player won " << nMediocreWins << " out of "
             << NTRIALS << " games." << endl;
          // We'd expect a mediocre player to win most of the games against
          // an awful player.  Similarly, a good player should outperform
          // a mediocre player.
    }
    else
    {
       cout << "That's not one of the choices." << endl;
    }
}

//int main(){
//    Game g1(10,10);
//    addStandardShips(g1);
//    assert(g1.rows()==10);
//    assert(g1.cols()==10);
//    assert(g1.isValid(Point(0,0)));
//    assert(g1.isValid(Point(1,9)));
//    assert(g1.isValid(Point(-1,0))==false);
//    assert(g1.isValid(Point(3,3)));
//    assert(g1.isValid(Point(3,10))==false);
//    assert(g1.addShip(11, 'A', "aircraft carrier")==false);
//    assert(g1.addShip(9, 'X', "aircraft")==false);
//    assert(g1.addShip(9, 'o', "aircraft")==false);
//    assert(g1.addShip(9, '.', "aircraft")==false);
//    assert(g1.addShip(-1, 'a', "aircraft")==false);
//    assert(g1.nShips()==0);
//    assert(g1.addShip(5, 'A', "aircraft carrier"));
//    assert(g1.nShips()==1);
//    assert(g1.addShip(4, 'B', "battleship"));
//    assert(g1.nShips()==2);
//    assert(g1.addShip(3, 'D', "destroyer"));
//    assert(g1.nShips()==3);
//    assert(g1.addShip(3, 'S', "submarine"));
//    assert(g1.nShips()==4);
//    assert(g1.addShip(2, 'P', "patrol boat"));
//    assert(g1.nShips()==5);
//    assert(g1.shipLength(0)==5);
//    assert(g1.shipLength(1)==4);
//    assert(g1.shipLength(2)==3);
//    assert(g1.shipLength(3)==3);
//    assert(g1.shipLength(4)==2);
//    assert(g1.shipLength(-1)==-1);
//    assert(g1.shipLength(5)==-1);
//    assert(g1.shipSymbol(0)=='A');
//    assert(g1.shipSymbol(1)=='B');
//    assert(g1.shipSymbol(2)=='D');
//    assert(g1.shipSymbol(3)=='S');
//    assert(g1.shipSymbol(4)=='P');
//    assert(g1.shipSymbol(-1)=='?');
//    assert(g1.shipSymbol(5)=='?');
//    assert(g1.shipName(0)=="aircraft carrier");
//    assert(g1.shipName(1)=="battleship");
//    assert(g1.shipName(2)=="destroyer");
//    assert(g1.shipName(3)=="submarine");
//    assert(g1.shipName(4)=="patrol boat");
//    assert(g1.shipName(-1)=="");
//    assert(g1.shipName(5)=="");
//    cout << "All game test cases passed!";
    
//    Board b1(g1);
//    b1.display(false);
//    assert(b1.placeShip(Point(0,0), 0, HORIZONTAL));
//    assert(b1.placeShip(Point(0,4), 1, HORIZONTAL)==false);
//    assert(b1.placeShip(Point(0,7), 1, HORIZONTAL)==false);
//    assert(b1.placeShip(Point(10,0), 0, HORIZONTAL)==false);
//    assert(b1.placeShip(Point(0,0), 4, VERTICAL)==false);
//    assert(b1.placeShip(Point(7,0), 4, VERTICAL));
//    assert(b1.placeShip(Point(3,4), 1, VERTICAL));
//    assert(b1.placeShip(Point(6,7), 2, HORIZONTAL));
//    assert(b1.placeShip(Point(4,6), 3, VERTICAL));
//    assert(b1.allShipsDestroyed()==false);
//    b1.display(false);
//    cout << endl;
    
//    assert(b1.unplaceShip(Point(0,1), 0, HORIZONTAL)==false);
//    assert(b1.unplaceShip(Point(0,4), 1, HORIZONTAL)==false);
//    assert(b1.unplaceShip(Point(0,7), 1, HORIZONTAL)==false);
//    assert(b1.unplaceShip(Point(10,0), 0, HORIZONTAL)==false);
//    assert(b1.unplaceShip(Point(0,0), 4, VERTICAL)==false);
//    assert(b1.unplaceShip(Point(0,0), 0, HORIZONTAL));
//    assert(b1.allShipsDestroyed()==false);
//    assert(b1.unplaceShip(Point(7,0), 4, VERTICAL));
//    assert(b1.unplaceShip(Point(3,4), 1, VERTICAL));
//    assert(b1.unplaceShip(Point(6,7), 2, HORIZONTAL));
//    assert(b1.unplaceShip(Point(4,6), 3, VERTICAL));
//    assert(b1.allShipsDestroyed());
//    b1.display(false);
//    cout << endl;
    
//    bool shotHit;
//    bool shipDestroyed;
//    int shipId;
//    assert(b1.attack(Point(-1,-1), shotHit, shipDestroyed, shipId)==false);
//    assert(b1.attack(Point(0,10), shotHit, shipDestroyed, shipId)==false);
//    assert(b1.attack(Point(10,0), shotHit, shipDestroyed, shipId)==false);
//
//
//
//    assert(b1.attack(Point(7,0), shotHit, shipDestroyed, shipId));
//    assert(shotHit == true);
//    b1.display(false);
//    b1.display(true);
//
//    assert(b1.attack(Point(10,0), shotHit, shipDestroyed, shipId)==false);
//    assert(shotHit == false);
//
//    assert(b1.attack(Point(8,0), shotHit, shipDestroyed, shipId));
//    assert(shotHit == true && shipDestroyed == true && shipId == 4);
//    b1.display(false);
//    b1.display(true);
//
//    assert(b1.attack(Point(7,0), shotHit, shipDestroyed, shipId)==false);
//    cout << "All board test cases passed!" << endl;
    
//    Game g(10,10);
//    addOtherShips(g);
//    Board b(g);
//    Player* p1 = createPlayer("good", "Chris", g);
//    p1->placeShips(b);
//
//    bool shotHit;
//    bool shipDestroyed;
//    int shipId;
//
//
//    Point rec = p1->recommendAttack();
//
//    bool valid = b.attack(rec, shotHit, shipDestroyed, shipId);
//
//    p1->recordAttackResult(rec, valid, shotHit, shipDestroyed, shipId);
//    b.display(true);
//
//    while(!b.allShipsDestroyed()){
//        rec = p1->recommendAttack();
//        valid = b.attack(rec, shotHit, shipDestroyed, shipId);
//        p1->recordAttackResult(rec, valid, shotHit, shipDestroyed, shipId);
//        b.display(true);
//    }
//}
