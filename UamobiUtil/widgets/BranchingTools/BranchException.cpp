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
    :
      #ifdef QT_VERSION5X
      QException(),
      #else
      std::exception(),
      #endif

      toWhichIndexReturn(ToModeSelection), acceptableToReturn()
{

}

BranchException::BranchException(int toWhereReturn):

    #ifdef QT_VERSION5X
    QException(),
    #else
    std::exception(),
    #endif
    toWhichIndexReturn(toWhereReturn), acceptableToReturn()
{

}

BranchException::BranchException(const QList<independent_nodes::nodelist> & list)
    :
      #ifdef QT_VERSION5X
      QException(),
      #else
      std::exception(),
      #endif
      toWhichIndexReturn(Filtered), acceptableToReturn(list)
{

}
