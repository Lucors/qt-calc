#include "calculator.h"

Calculator::Calculator(QWidget *parent) : QWidget(parent){
    this->setWindowIcon(QIcon("ico.png"));
    this->setStyleSheet("background-color: #284061; color: black;");
    this->setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);
    this->setFixedSize(0, 0);
    this->setWindowTitle("Calculate It!");

    this->calcScreen = new QLabel;
    this->calcScreen->setAlignment(Qt::AlignRight);
    this->calcScreen->setFont(QFont("sans-serif", 15, QFont::Bold));
//        this->calcScreen->setMinimumSize(50, 30);
    this->calcScreen->setStyleSheet("border-radius: 5px; background-color: #BABCBE; padding-right: 5px; padding-left: 5px;");

    this->calcHistoryScreen = new QLabel;
    this->calcHistoryScreen->setAlignment(Qt::AlignRight);
    this->calcHistoryScreen->setFont(QFont("sans-serif", 9));
//        this->calcScreen->setMinimumSize(50, 30);
    this->calcHistoryScreen->setStyleSheet("border-radius: 5px; background-color: #BABCBE; padding-right: 5px; padding-left: 5px;");

    this->layoutGrid = new QGridLayout(this);

    QString buttonsValues = "C+789/456-123*0.=";
    layoutGrid->addWidget(calcHistoryScreen, 0, 0, 1, 4);
    layoutGrid->addWidget(calcScreen, 1, 0, 1, 4);

    int globalIter = 0;
    for (int i = 2; i < 7; i++){
        for (int j = 0; j < 4; j++, globalIter++){
            QPushButton *buttonIJ = new QPushButton(static_cast<QString>(buttonsValues[globalIter]), this);
            buttonIJ->setFlat(true);
            buttonIJ->setMinimumSize(50, 30);
            if (buttonsValues[globalIter].isDigit()){
                buttonIJ->setStyleSheet("QPushButton {border-radius: 5px; background-color: #365684; color: white;}"
                                        "QPushButton:hover {background-color: #5182C6; color: black;}");
            }
            else {
                if (buttonsValues[globalIter] != QChar('=')){
                    buttonIJ->setStyleSheet("QPushButton {border-radius: 5px; background-color: #BABCBE;}"
                                            "QPushButton:hover {background-color: #F4F7F9;}");
                }
                else {
                    buttonIJ->setStyleSheet("QPushButton {border-radius: 5px; background-color: #848587;}"
                                            "QPushButton:hover {background-color: #F4F7F9;}");
                }
            }
            buttonIJ->setFont(QFont("sans-serif", 10, QFont::Bold));
            buttonIJ->setCursor(Qt::PointingHandCursor);
            this->buttons.push_back(buttonIJ);
            this->connect(buttonIJ, &QPushButton::clicked, [=](){
                this->addToScreenValue(buttonsValues[globalIter]);
             } );
            if (i == 6 && j == 2){
                layoutGrid->addWidget(buttonIJ, i, j, 1, 2);
                break;
            }
            if (i == 2){
                layoutGrid->addWidget(buttonIJ, i, j++, 1, 2);
                if (j == 2){
                    break;
                }
            }
            else {
                layoutGrid->addWidget(buttonIJ, i, j);
            }
        }
    }
    this->setLayout(layoutGrid);
}
Calculator::~Calculator(){}
void Calculator::printValues(){
    QString debugOutput;
    for (int i = 0; i < screenValues.size(); i++){
        debugOutput += " " + screenValues[i] + " ";
    }
    qDebug() << "---------screenValues:";
    qDebug() << debugOutput;
    qDebug() << "------------------end";
}
bool Calculator::checkFinalScreenValue(){
    if (screenValues.size() > 0){
        QString operators = "-+*/";
        for (QChar iter : operators){
            if (screenValues[screenValues.size()-1][0] == iter){
                return true;
            }
        }
    }
    return false;
}
void Calculator::calcByOperator(QVector <QString> *inputValues, QChar charOperator){
    if (!(inputValues->size() == 1)){
        for (int i = 0; i < inputValues->size(); i++){
            if (inputValues->at(i)[0] == charOperator){
                double num1, num2, result = 0.0;
                num1 = atof(inputValues->at(i-1).toStdString().c_str());
                num2 = atof(inputValues->at(i+1).toStdString().c_str());
                switch (charOperator.unicode()) {
                    case '+': {
                        result = num1 + num2;
                        break;
                    }
                    case '-': {
                        result = num1 - num2;
                        break;
                    }
                    case '/': {
                        result = num1 / num2;
                        break;
                    }
                    case '*': {
                        result = num1 * num2;
                        break;
                    }
                    default:{
                        qDebug() << "Undefined operator '"+static_cast<QString>(charOperator)+"'";
                        break;
                    }
                }
                inputValues->erase(inputValues->begin()+(i-1), inputValues->begin()+(i+2));
                QString strResult = QString::number(result, 'f', 5);
                inputValues->insert(i-1, strResult);
                i--;
            }
        }
    }
}
QString Calculator::calcIt(){
    QString tmpResultValue = "";
    if (this->isNumberDef){
        this->isNumberDef = false;
        this->isDotted = false;
        if (this->tmpNumber != ""){
            this->screenValues.push_back(tmpNumber);
            tmpNumber = "";
        }
    }
    if (this->checkFinalScreenValue()){
        screenValues.erase(screenValues.end()-1, screenValues.end());
    }
    printValues();
    if (this->screenValues.size() > 0){
        if (this->screenValues.size() != 1){
            calcByOperator(&this->screenValues, '*');
//                printValues();
            calcByOperator(&this->screenValues, '/');
//                printValues();
            calcByOperator(&this->screenValues, '-');
//                printValues();
            calcByOperator(&this->screenValues, '+');
//                printValues();
        }
        double num1 = atof(screenValues[0].toStdString().c_str());
        tmpResultValue = QString::number(num1, 'f', 5);
    }
    this->screenValues.clear();
    this->screenValues.push_back(tmpResultValue);
    this->isCalculating = false;
    return tmpResultValue;
}
void Calculator::addToCalcScreen(QChar someChar){
    QString tmpScreenValue = this->calcScreen->text();
    tmpScreenValue += someChar;
    this->calcScreen->setText(tmpScreenValue);
}
void Calculator::addToScreenValue(QChar someChar){
    if (!someChar.isDigit() && someChar != static_cast<QChar>("."[0])){
        if (!this->firstTime){
            if (someChar == QChar('C')){
                this->isCalculating = true;
                this->isNumberDef = false;
                this->isDotted = false;
                this->firstTime = true;
                if (this->tmpNumber != ""){
                    tmpNumber = "";
                }
                this->screenValues.clear();
                this->calcHistoryScreen->setText("");
                this->calcScreen->setText("");
                return;
            }

            if (someChar == QChar('=')){
                if (this->isCalculating){
                    this->calcHistoryScreen->setText(calcScreen->text());
                    this->calcScreen->setText(calcIt());
                }
                return;
            }


            if (!isCalculating){
                isCalculating = true;
            }
            if (this->isNumberDef){
                this->isNumberDef = false;
                this->isDotted = false;
                if (this->tmpNumber != ""){
                    this->screenValues.push_back(tmpNumber);
                    tmpNumber = "";
                }
            }
            if (!this->checkFinalScreenValue()){
                this->screenValues.push_back(someChar);
                addToCalcScreen(someChar);
                return;
            }
        }
    }
    else {
        if (firstTime){
            firstTime = false;
        }
        if (!this->isNumberDef){
            this->isNumberDef = true;
        }
        if (!isCalculating){
            this->calcScreen->setText("");
            this->screenValues.clear();
            isCalculating = true;
        }
        if (someChar == QChar('.')){
            if (!this->isDotted){
                this->isDotted = true;
                this->tmpNumber += someChar;
                addToCalcScreen(someChar);
            }
        }
        else {
            this->tmpNumber += someChar;
            addToCalcScreen(someChar);
        }
    }
}
