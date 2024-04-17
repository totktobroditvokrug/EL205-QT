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