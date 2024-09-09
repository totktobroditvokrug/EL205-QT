#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include<QDebug>
#include<QFile>

#define CONFIG_FILE_NAME "fc_connect.cfg"
#define CONFIG_WORK_DIR "C:/fcconnect/work"
#define CONFIG_SAMPLES_NAME "samples_def.txt"
#define CONFIG_REGISTERS_NAME "registers_def.txt"
#define CONFIG_CONFIG_NAME "config_def.csv"

class ConfigFile
{

public:
    ConfigFile();
    void openConfigFile();  // прочитать или создать файл инициализации
    void rewriteConfig();   // перезаписать путь и имена файлов в файл конфигурации

    // установщики данных из главной формы в файл конфигурации
    void getWorkDir(QString workDir){ _workDir = workDir; }
    void getSamplesName(QString samplesName){ _samplesName = samplesName; }
    void getRegisterName(QString registerName){ _registerName = registerName; }
    void getConfigName(QString configName){ _configName = configName; }

    // сеттеры взаимодействия с главной формой
    QString setWorkDir(){return _workDir; }
    QString setSamplesName(){return _samplesName; }
    QString setRegisterName(){return _registerName; }
    QString setConfigName(){return _configName; }



private:
    QString _workDir;       // путь рабочей директории !!! Вот по этой переменной два обращения!!!
    
    QString _samplesName;   // имя файла сэмплов
    QString _registerName;  // имя файла регистров
    QString _configName;    // имя файла конфигурации инертора
};

#endif // CONFIGFILE_H
