#include "configfile.h"
#include <QMessageBox>

ConfigFile::ConfigFile() // необязательная инициализация в конструкторе
{
    _workDir = CONFIG_WORK_DIR;            // путь рабочей директории
    _samplesName = CONFIG_SAMPLES_NAME;    // имя файла сэмплов
    _registerName = CONFIG_REGISTERS_NAME; // имя файла регистров
    _configName = CONFIG_CONFIG_NAME;      // имя файла конфигурации инертора
}

void ConfigFile::openConfigFile()
{
    QFile file(CONFIG_FILE_NAME);

    if ((file.exists())&&(file.open(QIODevice::ReadOnly))){
        qDebug() << "файл инициализации существует:";
        QTextStream stream(&file);
        QString readStr = stream.readAll();
        QStringList stringArray = readStr.split("\n");
        int length = stringArray.size();

        for (int i = 0; i < length; i++) {
            QStringList strCfgArr = stringArray[i].split(" ");
            int lengthStr = strCfgArr.size();
            if(lengthStr == 2){
               qDebug() << "position:" << strCfgArr[0] << "value:" << strCfgArr[1];
               if(strCfgArr[0] == "CONFIG_WORK_DIR") _workDir = strCfgArr[1];
               if(strCfgArr[0] == "CONFIG_SAMPLES_NAME") _samplesName = strCfgArr[1];
               if(strCfgArr[0] == "CONFIG_REGISTERS_NAME") _registerName = strCfgArr[1];
               if(strCfgArr[0] == "CONFIG_CONFIG_NAME") _configName = strCfgArr[1];
            }
        }
        qDebug() << "прочитанные значения" << _workDir << _samplesName << _registerName << _configName;

        file.close();
    }
    else{
        if((file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))){
            qDebug() << "создаем файл инициализации!";
            QTextStream stream(&file);
            stream << "CONFIG_WORK_DIR " << CONFIG_WORK_DIR << "\n" << "CONFIG_SAMPLES_NAME " << CONFIG_SAMPLES_NAME <<
                      "\n" << "CONFIG_REGISTERS_NAME " << CONFIG_REGISTERS_NAME << "\n" << "CONFIG_CONFIG_NAME " << CONFIG_CONFIG_NAME;           

            _workDir = CONFIG_WORK_DIR;            // путь рабочей директории
            _samplesName = CONFIG_SAMPLES_NAME;    // имя файла сэмплов
            _registerName = CONFIG_REGISTERS_NAME; // имя файла регистров
            _configName = CONFIG_CONFIG_NAME;      // имя файла конфигурации инертора
            file.close();
        }
        else QMessageBox::warning(nullptr, "Внимание","Файл конфигурации не существует.");
    }

}

void ConfigFile::rewriteConfig()  // перезаписать путь и имена файлов в файл конфигурации
{

    QFile file(CONFIG_FILE_NAME);

    if ((file.exists())&&(file.open(QIODevice::ReadWrite | QIODevice::Truncate))){
        QTextStream stream(&file);
        qDebug() << "перезаписываем файл инициализации!" << _workDir << _samplesName << _registerName << _configName;
        stream << "CONFIG_WORK_DIR " << _workDir << "\n" << "CONFIG_SAMPLES_NAME " << _samplesName <<
                  "\n" << "CONFIG_REGISTERS_NAME " << _registerName << "\n" << "CONFIG_CONFIG_NAME " << _configName;
        file.close();
    }
    else QMessageBox::warning(nullptr, "Внимание","Файл конфигурации не найден.");
}
