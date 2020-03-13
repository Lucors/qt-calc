#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QVector>
#include <QString>
#include <QDebug>
#include <cstdlib>
#include <ctime>

class Calculator : public QWidget {
    Q_OBJECT
private:
    QLabel *calcScreen;
    QLabel *calcHistoryScreen;
    QVector <QPushButton *> buttons;
    QGridLayout *layoutGrid;
    QVector <QString> screenValues;
    QString tmpNumber = "";
    bool isNumberDef = false;
    bool isCalculating = true;
    bool isDotted = false;
    bool firstTime = true;
public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();
    void printValues();
    bool checkFinalScreenValue();
    void calcByOperator(QVector <QString> *, QChar);
    QString calcIt();
    void addToCalcScreen(QChar);
    void addToScreenValue(QChar);
};

#endif // CALCULATOR_H
