#include "calc.h"
#include <QDebug>
using namespace std;

CALC::CALC()
{

    p[0].lvl = 1;
    p[1].lvl = 2;

    qDebug() << "CALC:" << size << p[0].lvl << p[1].lvl << p[2].lvl << p[3].lvl;
}

CALC::~CALC()
{
    delete [] p;
}
