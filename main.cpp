
class EnitityObject
{
    //class to use for initializing each entity as
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
#include <cstdlib>
#include <ctime>
#include <vector>

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
void pirateMove(char ocean[ROWS][COLUMNS]);
bool isCorrectMoveKey(char c);
void updateMapPosition(char ocean[ROWS][COLUMNS], EnitityObject *enemy);

void findbestPathToTravel(char ocean[ROWS][COLUMNS],int &bestPathColumn, int &bestPathRow, bool checkForIce);
int distanceBetween(int x1, int y1, int x2,int y2);
void moveEnemy(char ocean[ROWS][COLUMNS], EnitityObject *entity, bool checkForIce);
void updateEnemyPosition(char ocean[ROWS][COLUMNS], list<EnitityObject*> enemy, bool checkIceCollision);
void updateEnemyMapPosition(char ocean[ROWS][COLUMNS], list<EnitityObject*> enemy);
void removeSharksCollision(char ocean[ROWS][COLUMNS],list<EnitityObject*> removeSharks);
void mapRandomizer( char randOcean[ROWS][COLUMNS]);
int main(){
    //main function where all the looping of the game is done

    //mapRandomizer();

    char ocean[ROWS][COLUMNS];
    briefing();
    //readMap(ocean);
    mapRandomizer(ocean);
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

void randomSymbolsOnMap( vector<int> &row, vector<int> &column, char ocean[ROWS][COLUMNS],int numElements, char symbol){
    srand(static_cast <unsigned int> (time(0)));
    for(int i = 0; i < numElements; i++){
        int randomIndexRow = rand() % row.size();
        ocean[row[randomIndexRow]][column[randomIndexRow]] = symbol;
        row.erase(row.begin() + randomIndexRow);
        column.erase(column.begin() + randomIndexRow);
    }
}

void mapRandomizer(char randOcean[ROWS][COLUMNS]){
    int sharkNum = 2;
    int britnum = 1;
    int iceNum = 12;
    vector<int> openRows;
    vector<int> openColums;
    vector<int> borderRows;
    vector<int> borderColumns;
    srand(static_cast <unsigned int> (time(0)));

    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS; j++){
            if(i == 0 || i == (ROWS-1) || j == 0 || j == (COLUMNS-1)){
                randOcean[i][j] = iceSymbol;
                borderColumns.push_back(j);
                borderRows.push_back(i);
            }else{
                randOcean[i][j] = ' ';
                openColums.push_back(j);
                openRows.push_back(i);
            }
        }
    }

    for(int i = 0; i < britnum; i++){
        int randomIndexRow = rand() % openRows.size();
        randOcean[openRows[randomIndexRow]][openColums[randomIndexRow]] = britSymbol;
         britList.push_back(new EnitityObject(openRows[randomIndexRow],openColums[randomIndexRow]));
        openRows.erase(openRows.begin() + randomIndexRow);
        openColums.erase(openColums.begin() + randomIndexRow);
    }
     for(int i = 0; i < sharkNum; i++){
        int randomIndexRow = rand() % openRows.size();
        randOcean[openRows[randomIndexRow]][openColums[randomIndexRow]] = sharkSymbol;
        sharkList.push_back(new EnitityObject(openRows[randomIndexRow],openColums[randomIndexRow]));
        openRows.erase(openRows.begin() + randomIndexRow);
        openColums.erase(openColums.begin() + randomIndexRow);
    }
    for(int i = 0; i < iceNum; i++){
        int randomIndexRow = rand() % openRows.size();
        randOcean[openRows[randomIndexRow]][openColums[randomIndexRow]] = iceSymbol;
        openRows.erase(openRows.begin() + randomIndexRow);
        openColums.erase(openColums.begin() + randomIndexRow);
    }
    int randomIndexRow = rand() % openRows.size();
    randOcean[openRows[randomIndexRow]][openColums[randomIndexRow]] = pirateShipSymbol;
    pirateShip = new EnitityObject(openRows[randomIndexRow],openColums[randomIndexRow]);
    openRows.erase(openRows.begin() + randomIndexRow);
    openColums.erase(openColums.begin() + randomIndexRow);

    randomIndexRow = rand() % borderRows.size();
    randOcean[borderRows[randomIndexRow]][borderColumns[randomIndexRow]] = exitSymbol;
    borderRows.erase(borderRows.begin() + randomIndexRow);
    borderColumns.erase(borderColumns.begin() + randomIndexRow);

    for(int i = 0; i < openRows.size(); i++){
         randOcean[openRows[i]][openColums[i]] = ' ';
    }
}


void briefing(){
    //lay out the rules
    cout << "blah blah blah rules" << endl;
}

void readMap(char ocean[ROWS][COLUMNS]){
    //read map into char array and set the symbols
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
    //print map to screen
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS; j++){
            cout << ocean[i][j];
        }
        cout << endl;
    }
}
int distanceBetween(int x1, int y1, int x2,int y2){
    //calculate the distance between two points
    int xDistance = abs(x2-x1);
    int yDistance = abs(y2-y1);
    return xDistance + yDistance;
}

void updateMapPosition(char ocean[ROWS][COLUMNS], EnitityObject *entity){
    //update map
    ocean[entity->GetrowPosition()][entity->GetcolumnPosition()] = ocean[entity->GetoldRowPosition()][entity->GetoldColumnPosition()];
    ocean[entity->GetoldRowPosition()][entity->GetoldColumnPosition()] = ' ';
}
void findbestPathToTravel(char ocean[ROWS][COLUMNS],int &bestPathColumn, int &bestPathRow, bool checkForIce){
    //the logic to determine what way the enemy should travel to get to the player in the fastest route
    // the bool check for ice is to allow me to check for ice if need be but if its false the enemy
    //just goes through the ice
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
   //move the enemy and set its new position
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
    //move the pirates position, by checking inputted check then updateing accordingly
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
}
bool isCorrectMoveKey(char c){
    //check to see if inputted char is allowed
    char allowed[8] = {'q','w','e','a','d','z','x','c'};
    for(int i = 0; i < 8; i++){
        if(tolower(c) == allowed[i]){
            return true;
        }
    }
    return false;
}
void updateEnemyPosition(char ocean[ROWS][COLUMNS], list<EnitityObject*> enemy, bool checkIceCollision){
    //loop over the list to update the position (before render)
    list <EnitityObject*>::iterator iEnemy;
    for (iEnemy = enemy.begin(); iEnemy != enemy.end(); ++iEnemy)
    {
        EnitityObject *dog = dynamic_cast<EnitityObject*>(*iEnemy);
        moveEnemy(ocean, dog , checkIceCollision);
    }
}
void updateEnemyMapPosition(char ocean[ROWS][COLUMNS], list<EnitityObject*> enemy){
    //update the position on the map of the enemys
    //need to loop over the list so each element can be updated
    list <EnitityObject*>::iterator iEnemy;
    for (iEnemy = enemy.begin(); iEnemy != enemy.end(); ++iEnemy)
    {
        EnitityObject *dog = dynamic_cast<EnitityObject*>(*iEnemy);
        updateMapPosition(ocean, dog);
    }
}
bool checkCollisions(char ocean[ROWS][COLUMNS]){
    //cehck collisions with all objetcs on the screen to determine what to do
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
    list <EnitityObject*>::iterator iBrits;
    list<EnitityObject*> tempSharks;
    //loop over brits check for any collisions
    for (iBrits = britList.begin(); iBrits != britList.end(); ++iBrits)
    {
        EnitityObject *brit = dynamic_cast<EnitityObject*>(*iBrits);
        if(distanceBetween(brit->GetrowPosition(),brit->GetcolumnPosition(), shipRowPosition,shipColumnPosition) <= 2){
            cout << "\n\n\n\n\n\n";
            cout << "You have been killed by the brits!" << endl;
            return true;
        }
    }

    //loop over sharks check for any collisions
    for (iShark = sharkList.begin(); iShark != sharkList.end(); ++iShark)
    {
        EnitityObject *shark = dynamic_cast<EnitityObject*>(*iShark);
        if(shark->GetcolumnPosition() == shipColumnPosition && shark->GetrowPosition() == shipRowPosition){
            cout << "\n\n\n\n\n\n";
            cout << "You have been eaten by a shark!" << endl;
            return true;
        }

        for (iBrits = britList.begin(); iBrits != britList.end(); ++iBrits){
             EnitityObject *brit = dynamic_cast<EnitityObject*>(*iBrits);
            if(shark->GetcolumnPosition() == brit->GetcolumnPosition() && shark->GetrowPosition() == brit->GetrowPosition()){
                tempSharks.push_back(shark);
            }

        }
    }

    removeSharksCollision(ocean, tempSharks);
    return false;
}
void removeSharksCollision(char ocean[ROWS][COLUMNS], list<EnitityObject*> removeSharks){
    //remove sharks from shark list in case of collision
    list <EnitityObject*>::iterator itempShark;
    for (itempShark = removeSharks.begin(); itempShark != removeSharks.end(); ++itempShark)
    {
        EnitityObject *shark = dynamic_cast<EnitityObject*>(*itempShark);
        sharkList.remove(shark);
        if(ocean[shark->GetoldRowPosition()][shark->GetoldColumnPosition()] != britSymbol)
        ocean[shark->GetoldRowPosition()][shark->GetoldColumnPosition()] = ' ';
    }
}
