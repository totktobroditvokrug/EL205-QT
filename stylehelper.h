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

    static QString getSamplesStyle();
    static QString getRegistersStyle();
    static QString getResetTextStyle();

    static QString getRadioButtonSamplesStyle();
    static QString getRadioButtonRegistersStyle();
    static QString getRadioButtonResetTextStyle();

};

#endif // STYLEHELPER_H
