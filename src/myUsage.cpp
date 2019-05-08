#include "myUsage.h"

void MyUsage::report(bool repTime, bool repMem) {
    cout << endl;
    if (repTime) {
        setTimeUsage();
        cout << "Period time used : " << setprecision(4)
             << _periodUsedTime << " seconds" << endl;
        cout << "Total time used  : " << setprecision(4)
             << _totalUsedTime << " seconds" << endl;
    }
    if (repMem) {
        setMemUsage();
        cout << "Total memory used: " << setprecision(4)
             << _currentMem << " M Bytes" << endl;
    }
    cout << endl;
}

double MyUsage::GetTimeUsage() {
    setTimeUsage();
    return _periodUsedTime;
}

double MyUsage::GetMemUsage() {
    setMemUsage();
    return _currentMem;
}
