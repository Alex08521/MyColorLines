#include "gamelogic.h"

GameLogic::GameLogic(QObject *parent) : QObject(parent)
{
    field = QSqlDatabase::addDatabase("QSQLITE");
    field.setDatabaseName("./MyColorLines.db");
    field.open();

    QSqlQuery db(field);

    db.exec("SELECT Scores FROM Playfield WHERE id = 1");
    db.first();
    scores = db.value(0).toInt();

    db.exec("SELECT VolOfVoid FROM Playfield WHERE id = 1");
    db.first();
    noneChecker = db.value(0).toInt();

    db.exec("SELECT PrevIndex FROM Playfield WHERE id = 1");
    db.first();
    prevIndex = db.value(0).toInt();

    db.exec("SELECT Field FROM Playfield WHERE id = 1");
    db.first();
    playField = db.value(0).toString();

    ballSymbol = 'N';
    gameOver = false;
}

//************************************************************************************//

QChar GameLogic::readField(int index)
{
    return playField[index];
}

//************************************************************************************//

bool GameLogic::ballMoving(int index)
{
    if(gameOver) return true;
    if((index > -1)&&(index < 81))
    {
        if((playField[index] != 'N') && (ballSymbol == 'N'))
        {
            ballSymbol = playField[index];
            prevIndex = index;
            return true;
        }
        else if((playField[index] == 'N') && (ballSymbol != 'N'))
        {
            playField[index] = ballSymbol;
            ballSymbol = 'N';
            playField[prevIndex] = 'N';
            prevIndex = 82;
            if(!generateRandomBallsOnRandomPositoions()) gameOver = true;
            if(!gameOver)scaningFieldForMatches();
            if(!gameOver) if(!UpdateDatabase()) return false;
            return true;
        }
        else return false;
    }
    else return false;
}

//************************************************************************************//

QString GameLogic::getScoresString()
{
    return QString::number(scores);
}

//************************************************************************************//

bool GameLogic::generateRandomBallsOnRandomPositoions()
{
    if(noneChecker < 2) return false;

    for(int iter = 0; iter < 3; iter++)
    {
        int randomBall = rand()%81;
        QChar symbolTemp = playField[randomBall];
        while(symbolTemp != 'N')
        {
            randomBall = rand()%81;
            symbolTemp = playField[randomBall];
        }

        int randomColor = rand()%4;
        switch (randomColor)
        {
            case 0:
                playField[randomBall] = 'R';
            break;
            case 1:
                playField[randomBall] = 'G';
            break;
            case 2:
                playField[randomBall] = 'B';
            break;
            case 3:
                playField[randomBall] = 'Y';
            break;
        }
    }
    noneChecker -= 3;
    return true;
}

//************************************************************************************//

bool GameLogic::newGame()
{
     scores = 0;
     noneChecker = 80;
     prevIndex = 81;
     playField = "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN";
     gameOver = false;
     ballSymbol = 'N';
     if(!UpdateDatabase()) return false;
     if(!generateRandomBallsOnRandomPositoions()) gameOver = true;

     return true;
}

//************************************************************************************//

bool GameLogic::scaningFieldForMatches()
{
    int matchCheker = 0;
    int indexesForDestroy[81];
    int numOfIndexes = 0;

    for(int y = 0; y < 9; y++)
    {
        for(int x = 0; x < 9; x++)
        {
            if(x == 0)
            {
                if ((playField[y*9+x] == playField[y*9+x+1]) && (playField[y*9+x] != 'N'))
                {
                    matchCheker++;
                    indexesForDestroy[numOfIndexes] = y*9+x;
                    indexesForDestroy[numOfIndexes+1] = y*9+x+1;
                    numOfIndexes++;
                }

            }
            else if(x == 8)
            {
                if((playField[y*9+x] == playField[y*9+x-1]) && (playField[y*9+x] != 'N'))
                {
                    matchCheker++;
                    indexesForDestroy[numOfIndexes] = y*9+x;
                    numOfIndexes++;
                }
            }
            else if((playField[y*9+x] == playField[y*9+x+1]) && (playField[y*9+x] != 'N'))
            {
                matchCheker++;
                indexesForDestroy[numOfIndexes] = y*9+x;
                indexesForDestroy[numOfIndexes+1] = y*9+x+1;
                numOfIndexes++;
            }
            else if((playField[y*9+x] == playField[y*9+x-1]) && (playField[y*9+x] != 'N'))
            {
                matchCheker++;
                indexesForDestroy[numOfIndexes] = y*9+x;
                numOfIndexes++;
                if(matchCheker < 5) numOfIndexes -= matchCheker;
                matchCheker = 0;
            }
            else
            {
                if(matchCheker < 5) numOfIndexes -= matchCheker;
                matchCheker = 0;
            }
        }
        if(matchCheker < 5) numOfIndexes -= matchCheker;
        matchCheker = 0;
    }

    for(int x = 0; x < 9; x++)
    {
        for(int y = 0; y < 9; y++)
        {
            if(y == 0)
            {
                if((playField[y*9+x] == playField[(y+1)*9+x]) && (playField[y*9+x] != 'N'))
                {
                    matchCheker++;
                    indexesForDestroy[numOfIndexes] = y*9+x;
                    indexesForDestroy[numOfIndexes+1] = (y+1)*9+x;
                    numOfIndexes++;
                }
            }
            else if(y == 8)
            {
                if((playField[y*9+x] == playField[(y-1)*9+x]) && (playField[y*9+x] != 'N'))
                {
                    matchCheker++;
                    if(numOfIndexes != 0) indexesForDestroy[numOfIndexes-1] = (y-1)*9+x;
                    indexesForDestroy[numOfIndexes] = y*9+x;
                    numOfIndexes++;
                }
            }
            else if((playField[y*9+x] == playField[(y+1)*9+x]) && (playField[y*9+x] != 'N'))
            {
                matchCheker++;
                indexesForDestroy[numOfIndexes] = y*9+x;
                indexesForDestroy[numOfIndexes+1] = (y+1)*9+x;
                numOfIndexes++;
            }
            else if((playField[y*9+x] == playField[(y-1)*9+x]) && (playField[y*9+x] != 'N'))
            {
                matchCheker++;
                indexesForDestroy[numOfIndexes] = y*9+x;
                numOfIndexes++;
                if(matchCheker < 5) numOfIndexes -= matchCheker;
                matchCheker = 0;
            }
            else
            {
                if(matchCheker < 5) numOfIndexes -= matchCheker;
                matchCheker = 0;
            }
        }
        if(matchCheker < 5) numOfIndexes -= matchCheker;
        matchCheker = 0;
    }

    for(int i = 0; i < numOfIndexes; i++)
    {
        playField[indexesForDestroy[i]] = 'N';

    }

    scores += numOfIndexes/5 * 10;
    noneChecker += numOfIndexes;

    return true;
}

//************************************************************************************//

bool GameLogic::UpdateDatabase()
{
    QSqlQuery db(field);

    if(db.prepare("UPDATE Playfield SET Scores = :scores WHERE id = 1"))
    {
        db.bindValue(":scores", scores);
        db.exec();
    }
    else
    {
        return false;
    }

    if(db.prepare("UPDATE Playfield SET VolOfVoid = :none WHERE id = 1"))
    {
        db.bindValue(":none", noneChecker);
        db.exec();
    }
    else
    {
        return false;
    }

    if(db.prepare("UPDATE Playfield SET PrevIndex = :prev WHERE id = 1"))
    {
        db.bindValue(":prev", prevIndex);
        db.exec();
    }
    else
    {
        return false;
    }

    if(db.prepare("UPDATE Playfield SET Field = :field WHERE id = 1"))
    {
        db.bindValue(":field", playField);
        db.exec();
    }
    else
    {
        return false;
    }

   return true;
}

//************************************************************************************//

bool GameLogic::isGameOver()
{
    return gameOver;
}

//************************************************************************************//

bool GameLogic::choiseCancel()
{
    ballSymbol = 'N';
    prevIndex = 81;
    return true;
}

//************************************************************************************//

int GameLogic::getActiveIndex()
{
    return prevIndex;
}
