#pragma once
#ifdef QT_VERSION5X
#include <QException>
#else
#include <exception>
#endif
#include "widgets/parents/IndependentBranchNode.h"
#include <QList>


class BranchException : public

        #ifdef QT_VERSION5X
        QException
        #else
        std::exception
        #endif
{
protected:
    int toWhichIndexReturn;
    QList<independent_nodes::nodelist> acceptableToReturn;
public:
    enum ReservedIndexes {Filtered = -1024, ToModeSelection = -2, ToLogin = -1};
    void raise() const
#ifdef QT_VERSION5X
    override
#endif
    {throw *this;}
    BranchException* clone()const
#ifdef QT_VERSION5X
    override
#endif
    {return new BranchException(*this);}
    bool isDirectioned() {return toWhichIndexReturn >= 0;}
    bool isDirectionedOutside() {return toWhichIndexReturn < 0;}
    int whereToReturn() {return (toWhichIndexReturn > 0) ? toWhichIndexReturn : 0;}
    int specialDestination() { return toWhichIndexReturn; }
    bool canReturnHere(independent_nodes::nodelist);
    BranchException();
    explicit BranchException(int);
    explicit BranchException(const QList<independent_nodes::nodelist>&);

};




