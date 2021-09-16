#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <qmath.h>

class GameLogic : public QObject
{
Q_OBJECT
private:
    int scores; //количество очков
    QChar ballSymbol; //буфер для запоминания цвета шарика
    QString playField; //игровое поле 9х9, размещённое в QString
    int noneChecker; //количество свободных клеток
    bool gameOver; //состояние игры(окончена или нет)
    QSqlDatabase field; //объект для связи с БД
    int prevIndex;

    bool increaseScores(int increaseValue); //увеличение количества очков на increaseValue, если очков после увеличения больше 9999999, то счётчик обнуляется
    bool scaningFieldForMatches(); //поиск на игровом поле шариков одного цвета составленным по правилам игры
    bool UpdateDatabase(); //сохранение текущего состояния игры в базу данных
    bool generateRandomBallsOnRandomPositoions(); //генераия трёх случайных шариков на случайных позиция, если поле переполнено возврвщает false и ставит флаг GameOver

public:
    explicit GameLogic(QObject *parent = nullptr); //конструктор
    Q_INVOKABLE QChar readField(int index); //получение символа из поля по координатам
    Q_INVOKABLE bool ballMoving(int index); //выбор шарика и установка его на нужное место
    Q_INVOKABLE QString getScoresString(); //получение символьной строки очков
    Q_INVOKABLE bool newGame(); //приводит базу данных и иговое поле в первоначальное состояние
    Q_INVOKABLE bool isGameOver(); //показ состояния игры
    Q_INVOKABLE bool choiseCancel(); //отмена выбора шарика
    Q_INVOKABLE int getActiveIndex(); //передача значения prevIndex
};

#endif // GAMELOGIC_H
