#include "stylehelper.h"

StyleHelper::StyleHelper()
{

}

QString StyleHelper::getStartButtonStyle()
{
    return "QPushButton{"
            "color: #008000;"
            "background: none;"
           "}";
}

QString StyleHelper::getStartedButtonStyle()
{
    return "QPushButton{"
            "color: #000000;"
            "background-color: #00dd00;"
            "border: 1px solid #808080;"
           "}"
           "QPushButton::hover{"

                       "background-color: #00ff00;"
                       "border: 1px solid #c0c0c0;"
           "}";
}

QString StyleHelper::getWaitButtonStyle()
{
    return "QPushButton{"
            "color: #000000;"
            "background-color: #ffff7f;"
            "border: 1px solid #808080;"
           "}"
           "QPushButton::hover{"

                       "background-color: #ffff00;"
                       "border: 1px solid #c0c0c0;"
           "}";
}

QString StyleHelper::getDisableButtonStyle()
{
    return "QPushButton{"
            "color: #808080;"
            "background-color: #c0c0c0;"
            "border: 1px solid #808080;"
           "}";
}

QString StyleHelper::getStopButtonStyle()
{
    return "QPushButton{"
            "color: #800000;"
            "background: none;"
           "}";
}

QString StyleHelper::getStopedButtonStyle()
{
    return "QPushButton{"
            "color: #000000;"
            "background-color: #dd0000;"
            "border: 1px solid #808080;"
           "}"
           "QPushButton::hover{"

                       "background-color: #ff0000;"
                       "border: 1px solid #c0c0c0;"
           "}";
}

QString StyleHelper::getAlarmButtonStyle()
{
    return "QPushButton{"
           "color: #808080;"
           "background-color: #c0c0c0;"
           "border: 1px solid #808080;"
           "border-radius: 20%;"
           "}";
}

QString StyleHelper::getReadOnlyStyle()
{
    return  "QLineEdit{"
            "color: rgb(255, 0, 0);"
            "}";
}

QString StyleHelper::getReadWriteStyle()
{
    return  "QLineEdit{"
            "color: rgb(0, 0, 255);"
            "}";
}

QString StyleHelper::getSlidersSelectedStyle()
{
    return  "QLineEdit{"
            "color: rgb(0, 100, 0);"
            "}";
}

QString StyleHelper::getRemoveSelectionStyle()
{
    return "QLineEdit {background-color: none;}";
}

QString StyleHelper::getSamplesStyle()
{
    return  "QLabel{"
            "font-weight:600;"
            "font-size:12pt;"
            "color: rgb(0, 100, 0);"
            "}";
}

QString StyleHelper::getRegistersStyle()
{    return  "QLabel{"
             "font-weight:600;"
             "font-size:12pt;"
             "color: rgb(100, 0, 0);"
             "}";

}

QString StyleHelper::getResetTextStyle()
{    return  "QLabel{"
             "color: none;"
             "}";

}


QString StyleHelper::getRadioButtonSamplesStyle()
{
    return  "QRadioButton{"
            "font-weight:600;"
            "font-size:12pt;"
            "color: rgb(0, 100, 0);"
            "}";
}

QString StyleHelper::getRadioButtonRegistersStyle()
{    return  "QRadioButton{"
            "font-weight:600;"
            "font-size:12pt;"
             "color: rgb(100, 0, 0);"
             "}";

}

QString StyleHelper::getRadioButtonResetTextStyle()
{    return  "QRadioButton{"
            "font-weight:400;"
            "font-size:8pt;"
             "color: none;"
             "}";

}
