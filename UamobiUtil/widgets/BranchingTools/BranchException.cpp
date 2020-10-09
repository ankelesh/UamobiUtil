#include "BranchException.h"





bool BranchException::canReturnHere(independent_nodes::nodelist currentType)
{
    for (QList<independent_nodes::nodelist>::iterator nindex = acceptableToReturn.begin(); nindex != acceptableToReturn.end(); ++nindex)
    {
        if (currentType == (*nindex))
            return true;
    }
    return false;
}

BranchException::BranchException()
    :QException(), toWhichIndexReturn(ToModeSelection), acceptableToReturn()
{

}

BranchException::BranchException(int toWhereReturn):
    QException(), toWhichIndexReturn(toWhereReturn), acceptableToReturn()
{

}

BranchException::BranchException(const QList<independent_nodes::nodelist> & list)
    : QException(), toWhichIndexReturn(Filtered), acceptableToReturn(list)
{

}
