#include "calculator.h"
#include "./ui_calculator.h"
#include <QtMath>

double calcVal=0.0;
bool divTrigger=false;
bool multTrigger=false;
bool addTrigger=false;
bool subTrigger=false;
bool modTrigger=false;
bool gcdTrigger=false;
bool lcmTrigger=false;

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);
    ui->display->setText("");
    QPushButton *numButtons[10];
    for (int i=0;i<10;++i){
        QString butName="button"+QString::number(i);
        numButtons[i]=Calculator::findChild<QPushButton *>(butName);
        connect(numButtons[i], SIGNAL(released()), this, SLOT(NumPressed()));
    }
    ui->sqrt->setText(QChar(0x221A));
    ui->power->setText("x\302\262");
    connect(ui->add, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->minus, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->multipluy, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->divide, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->mod, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->gcd, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->lcm, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->button_equal, SIGNAL(released()), this, SLOT(EqualButtonPressed()));
    connect(ui->change_sign, SIGNAL(released()), this, SLOT(ChangeNumberSign()));
    connect(ui->button_ac, SIGNAL(released()), this, SLOT(ClearPressed()));
    connect(ui->sqrt, SIGNAL(released()), this, SLOT(unaryOperatorPressed()));
    connect(ui->power, SIGNAL(released()), this, SLOT(unaryOperatorPressed()));
    connect(ui->reciprocal, SIGNAL(released()), this, SLOT(unaryOperatorPressed()));
    connect(ui->log, SIGNAL(released()), this, SLOT(unaryOperatorPressed()));
    connect(ui->decimal, SIGNAL(released()), this, SLOT(NumPressed()));
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::NumPressed(){
    QPushButton *button=(QPushButton *)sender();
    QString butVal=button->text();
    QString displayVal=ui->display->text();
    QString newVal=displayVal+butVal;
    ui->display->setText(newVal);
}

void Calculator::MathButtonPressed(){
    divTrigger=false;
    multTrigger=false;
    addTrigger=false;
    subTrigger=false;
    modTrigger=false;
    gcdTrigger=false;
    lcmTrigger=false;
    QString displayVal=ui->display->text();
    calcVal=displayVal.toDouble();
    QPushButton *button=(QPushButton *)sender();
    QString butVal=button->text();
    if (QString::compare(butVal, "/", Qt::CaseInsensitive)==0)
        divTrigger=true;
    else if (QString::compare(butVal, "*", Qt::CaseInsensitive)==0)
        multTrigger=true;
    else if (QString::compare(butVal, "+", Qt::CaseInsensitive)==0)
        addTrigger=true;
    else if (QString::compare(butVal, "mod", Qt::CaseInsensitive)==0)
        modTrigger=true;
    else if (QString::compare(butVal, "gcd", Qt::CaseInsensitive)==0)
        gcdTrigger=true;
    else if (QString::compare(butVal, "lcm", Qt::CaseInsensitive)==0)
        lcmTrigger=true;
    else
        subTrigger=true;
    ui->display->setText("");
}

void Calculator::EqualButtonPressed(){
    double solution=0.0;
    QString displayVal=ui->display->text();
    double dblDisplayVal=displayVal.toDouble();
    if (addTrigger || subTrigger || multTrigger || divTrigger || modTrigger || gcdTrigger || lcmTrigger){
        if (addTrigger){
            solution=calcVal+dblDisplayVal;
        }
        else if (subTrigger){
            solution=calcVal-dblDisplayVal;
        }
        else if (multTrigger){
            solution=calcVal*dblDisplayVal;
        }
        else if (modTrigger){
            solution=(int)calcVal%(int)dblDisplayVal;
        }
        else if (gcdTrigger){
            solution=std::__gcd((int)calcVal, (int)dblDisplayVal);
        }
        else if (lcmTrigger){
            solution=calcVal*dblDisplayVal/(std::__gcd((int)calcVal, (int)dblDisplayVal));
        }
        else{
            if (dblDisplayVal==0 || dblDisplayVal==0.0){
                ui->display->setText("Division by 0 is undefined");
                return;
            }
            else{
                solution=calcVal/dblDisplayVal;
            }
        }
    }
    ui->display->setText(QString::number(solution));
}

void Calculator::ChangeNumberSign(){
    QString displayVal = ui->display->text();
    QRegExp reg("[-+]?[0-9.]*");
    if(reg.exactMatch(displayVal)){
        double dblDisplayVal = displayVal.toDouble();
        double dblDisplayValSign = -1 * dblDisplayVal;
        ui->display->setText(QString::number(dblDisplayValSign));
    }

}

void Calculator::ClearPressed(){
    ui->display->setText("");
    calcVal=0.0;
}

void Calculator::unaryOperatorPressed(){
    QPushButton *button=(QPushButton *)sender();
    QString butVal=button->text();
    QString displayVal=ui->display->text();
    if (displayVal.isEmpty()){
        ui->display->setText("Invalid expression");
        return;
    }
    double dblDisplayVal=displayVal.toDouble();
    if (butVal==QChar(0x221A)){
        if (dblDisplayVal<0.0){
            ui->display->setText("Negative number");
            return;
        }
        else{
            ui->display->setText(QString::number(qSqrt(dblDisplayVal)));
        }
    }
    else if (QString::compare(butVal, "1/x", Qt::CaseInsensitive)==0)
    {
        ui->display->setText(QString::number(1/dblDisplayVal));
    }
    else if (QString::compare(butVal, "log", Qt::CaseInsensitive)==0)
    {
        if (dblDisplayVal<=0 || dblDisplayVal<=0.0){
            ui->display->setText("Log is undefined for negative and 0");
            return;
        }
        else{
            ui->display->setText(QString::number(std::log10(dblDisplayVal)));
        }
    }
    else{
        ui->display->setText(QString::number(qPow(dblDisplayVal, 2)));
    }
}
