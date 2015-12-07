
class EnitityObject
{
    public:
        EnitityObject();
        EnitityObject(int row , int col){rowPosition = row;columnPosition = col;}
        int GetrowPosition() { return rowPosition; }
        void SetrowPosition(int val) { rowPosition = val; }
        int GetcolumnPosition() { return columnPosition; }
        void SetcolumnPosition(int val) { columnPosition = val; }
        int GetoldRowPosition() { return oldRowPosition; }
        void SetoldRowPosition(int val) { oldRowPosition = val; }
        int GetoldColumnPosition() { return oldColumnPosition; }
        void SetoldColumnPosition(int val) { oldColumnPosition = val; }
    protected:
    private:
        int rowPosition;
        int columnPosition;
        int oldRowPosition;
        int oldColumnPosition;
};

#include <iostream>
#include <fstream>
#include <list>
#include <cmath>

using namespace std;

const char pirateShipSymbol = 'P', iceSymbol = 'I', exitSymbol = 'X', sharkSymbol = 'S', britSymbol = 'B';
const int ROWS = 15, COLUMNS = 30;

EnitityObject* pirateShip;
list<EnitityObject*> sharkList;
list<EnitityObject*> britList;

void briefing();
void readMap(char ocean[ROWS][COLUMNS]);
void printMap(const char ocean[ROWS][COLUMNS]);
bool checkCollisions(char ocean[ROWS][COLUMNS]);
void findbestPathToTravel(char ocean[ROWS][COLUMNS],int &bestPathColumn, int &bestPathRow, bool checkForIce);
int distanceBetween(int x1, int y1, int x2,int y2);
void updateMapPosition(char ocean[ROWS][COLUMNS], EnitityObject *enemy);
void moveEnemy(char ocean[ROWS][COLUMNS], EnitityObject *entity, bool checkForIce);
void updateMapPosition(char ocean[ROWS][COLUMNS],int shipColumnPosition, int shipRowPosition);
void pirateMove(char ocean[ROWS][COLUMNS]);
bool isCorrectMoveKey(char c);
void updateEnemyPosition(char ocean[ROWS][COLUMNS], list<EnitityObject*> enemy, bool checkIceCollision);
void updateEnemyMapPosition(char ocean[ROWS][COLUMNS], list<EnitityObject*> enemy);

int main(){
    char ocean[ROWS][COLUMNS];
    briefing();
    readMap(ocean);
    printMap(ocean);

    bool endOfGame = false;
    while(!endOfGame){

        pirateMove(ocean);
        endOfGame = checkCollisions(ocean);

        if(!endOfGame){
            updateEnemyPosition(ocean,sharkList,true);
            updateEnemyPosition(ocean,britList,false);
            endOfGame = checkCollisions(ocean);

            if(!endOfGame){
                updateMapPosition(ocean, pirateShip);
                updateEnemyMapPosition(ocean, sharkList);
                updateEnemyMapPosition(ocean, britList);
                printMap(ocean);
            }
        }

    }
    return 0;
}

void updateEnemyPosition(char ocean[ROWS][COLUMNS], list<EnitityObject*> enemy, bool checkIceCollision){
    list <EnitityObject*>::iterator iEnemy;
    for (iEnemy = enemy.begin(); iEnemy != enemy.end(); ++iEnemy)
    {
        EnitityObject *dog = dynamic_cast<EnitityObject*>(*iEnemy);
        moveEnemy(ocean, dog , checkIceCollision);
    }
}
void updateEnemyMapPosition(char ocean[ROWS][COLUMNS], list<EnitityObject*> enemy){
    list <EnitityObject*>::iterator iEnemy;
    for (iEnemy = enemy.begin(); iEnemy != enemy.end(); ++iEnemy)
    {
        EnitityObject *dog = dynamic_cast<EnitityObject*>(*iEnemy);
        updateMapPosition(ocean, dog);
    }
}
bool checkCollisions(char ocean[ROWS][COLUMNS]){
    int shipRowPosition = pirateShip->GetrowPosition();
    int shipColumnPosition = pirateShip->GetcolumnPosition();

    if(ocean[shipRowPosition][shipColumnPosition] == iceSymbol){
        cout << "\n\n\n\n\n\n";
        cout << "You have hit ice!" << endl;
        return true;
    }
    if(ocean[shipRowPosition][shipColumnPosition] == exitSymbol){
        cout << "\n\n\n\n\n\n";
        cout << "You win!!!!!!!!!!!!!!!" << endl;
        return true;
    }
    list <EnitityObject*>::iterator iShark;
    for (iShark = sharkList.begin(); iShark != sharkList.end(); ++iShark)
    {
        EnitityObject *shark = dynamic_cast<EnitityObject*>(*iShark);
        if(shark->GetcolumnPosition() == shipColumnPosition && shark->GetrowPosition() == shipRowPosition){
            cout << "\n\n\n\n\n\n";
            cout << "You have been eaten by a shark!" << endl;
            return true;
        }
    }

    list <EnitityObject*>::iterator iBrits;
    for (iBrits = britList.begin(); iBrits != britList.end(); ++iBrits)
    {
        EnitityObject *brit = dynamic_cast<EnitityObject*>(*iBrits);
        if(distanceBetween(brit->GetrowPosition(),brit->GetcolumnPosition(), shipRowPosition,shipColumnPosition) <= 2){
            cout << "\n\n\n\n\n\n";
            cout << "You have been killed by the brits!" << endl;
            return true;
        }
    }
    return false;
}
void briefing(){
    cout << "blah blah blah rules and shit" << endl;

}
void readMap(char ocean[ROWS][COLUMNS]){
    ifstream inFile("map.txt");
    if(!inFile){
        cout << "error" << endl;
    }else
        for(int i = 0; i < ROWS; i++){
            for(int j = 0; j < COLUMNS; j++){
                char current;
                inFile.get(current);
                while(isspace(current)){
                    inFile.get(current);
                }
               unsigned char c;
                switch(current){
                    case '5':
                        c = pirateShipSymbol;
                        pirateShip = new EnitityObject(i,j);
                        break;
                    case '4':
                        c = exitSymbol;
                        break;
                    case '3':
                        c = britSymbol;
                        britList.push_back(new EnitityObject(i,j));
                        break;
                    case '2':
                        c = sharkSymbol;
                        sharkList.push_back(new EnitityObject(i,j));
                        break;
                    case '1':
                        c = iceSymbol;
                        break;
                    case '0':
                        c = ' ';
                        break;
                }
                ocean[i][j] = c;
            }
        }
    }
void printMap(const char ocean[ROWS][COLUMNS]){
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS; j++){
            cout << ocean[i][j];
        }
        cout << endl;
    }
}
int distanceBetween(int x1, int y1, int x2,int y2){
    int xDistance = abs(x2-x1);
    int yDistance = abs(y2-y1);
    return xDistance + yDistance;
}
void updateMapPosition(char ocean[ROWS][COLUMNS], EnitityObject *brit){
    ocean[brit->GetrowPosition()][brit->GetcolumnPosition()] = ocean[brit->GetoldRowPosition()][brit->GetoldColumnPosition()];
    ocean[brit->GetoldRowPosition()][brit->GetoldColumnPosition()] = ' ';
}
void findbestPathToTravel(char ocean[ROWS][COLUMNS],int &bestPathColumn, int &bestPathRow, bool checkForIce){
    int const RCCI = 2, RCCO = 9;
    int rowColumnAdditions[RCCO][RCCI] = {{0,0},{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
    int smallestDistance = 99999;
    int tempDistance = 0;
    int returningRow = bestPathRow;
    int returningColumn = bestPathColumn;
    int tempRow = bestPathRow;
    int tempColumn = bestPathColumn;
    for(int i = 1; i < RCCO; i++){
            tempRow = bestPathRow + rowColumnAdditions[i][0];
            tempColumn = bestPathColumn + rowColumnAdditions[i][1];
            tempDistance = distanceBetween(tempColumn, tempRow, pirateShip->GetcolumnPosition(), pirateShip->GetrowPosition());
            if(tempRow != 0 && tempRow != ROWS-1 && tempColumn != 0 && tempColumn != COLUMNS-1){
                bool isIce = checkForIce ? (ocean[tempRow][tempColumn] == iceSymbol) : false;
                if(tempDistance < smallestDistance && !isIce){
                    returningColumn = tempColumn;
                    returningRow = tempRow;
                    smallestDistance = tempDistance;
                }
            }
    }

    bestPathRow = returningRow;
    bestPathColumn = returningColumn;
}
void moveEnemy(char ocean[ROWS][COLUMNS], EnitityObject *entity, bool checkForIce){
        //possibly add funcationality to remove sharks from list when they collide
       int columnPosition = entity->GetcolumnPosition();
       int rowPosition = entity->GetrowPosition();
       int newColumnPosition = columnPosition;
       int newRowPosition = rowPosition;
       findbestPathToTravel(ocean, newColumnPosition, newRowPosition, checkForIce);
       entity->SetoldColumnPosition(columnPosition);
       entity->SetoldRowPosition(rowPosition);
       entity->SetrowPosition(newRowPosition);
       entity->SetcolumnPosition(newColumnPosition);
}
void pirateMove(char ocean[ROWS][COLUMNS]){

    int newColumn = pirateShip->GetcolumnPosition(), newRow = pirateShip->GetrowPosition();
    char movementDiretion = 'h';
    while(!isCorrectMoveKey(movementDiretion)){
        cout << "Select direction to move: ";
        cin >>  movementDiretion;
    }
    movementDiretion = tolower(movementDiretion);

    switch (movementDiretion){
    case 'q':
        newColumn--;
        newRow--;
        break;
    case 'w':
        newRow--;
        break;
    case 'e':
        newColumn++;
        newRow--;
        break;
    case 'a':
        newColumn--;
        break;
    case 'd':
        newColumn++;
        break;
    case 'z':
        newColumn--;
        newRow++;
        break;
    case 'x':
        newRow++;
        break;
    case 'c':
        newColumn++;
        newRow++;
        break;
    }
    pirateShip->SetoldColumnPosition(pirateShip->GetcolumnPosition());
    pirateShip->SetoldRowPosition(pirateShip->GetrowPosition());
    pirateShip->SetcolumnPosition(newColumn);
    pirateShip->SetrowPosition(newRow);
//    oldShipColumnPosition = shipColumnPosition;
//    oldShipRowPosition = shipRowPosition;
//    shipRowPosition = newRow;
//    shipColumnPosition = newColumn;

}
bool isCorrectMoveKey(char c){
    char allowed[8] = {'q','w','e','a','d','z','x','c'};
    for(int i = 0; i < 8; i++){
        if(tolower(c) == allowed[i]){
            return true;
        }
    }
    return false;
}
