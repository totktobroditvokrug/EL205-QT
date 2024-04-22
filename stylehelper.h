#ifndef STYLEHELPER_H
#define STYLEHELPER_H

#include <QString>

class StyleHelper
{
public:
    StyleHelper();
    static QString getStartButtonStyle();
    static QString getStartedButtonStyle();
    static QString getWaitButtonStyle();

    static QString getDisableButtonStyle();

    static QString getStopButtonStyle();
    static QString getStopedButtonStyle();
    static QString getAlarmButtonStyle();

    static QString getReadOnlyStyle();
    static QString getReadWriteStyle();

    static QString getSlidersSelectedStyle();
    static QString getRemoveSelectionStyle();

};

#endif // STYLEHELPER_H
