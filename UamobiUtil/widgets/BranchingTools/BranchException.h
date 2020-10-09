#pragma once
#include <QException>
#include "widgets/parents/IndependentBranchNode.h"
#include <QList>


class BranchException : public QException
{
protected:
    int toWhichIndexReturn;
    QList<independent_nodes::nodelist> acceptableToReturn;
public:
    enum ReservedIndexes {Filtered = -1024, ToModeSelection = -2, ToLogin = -1};
    void raise() const override {throw *this;}
    BranchException* clone()const override {return new BranchException(*this);}
    bool isDirectioned() {return toWhichIndexReturn >= 0;};
    bool isDirectionedOutside() {return toWhichIndexReturn < 0;}
    int whereToReturn() {return (toWhichIndexReturn > 0) ? toWhichIndexReturn : 0;};
    bool canReturnHere(independent_nodes::nodelist);
    BranchException();
    explicit BranchException(int);
    explicit BranchException(const QList<independent_nodes::nodelist>&);

};




