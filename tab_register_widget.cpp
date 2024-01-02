#include "mainwindow.h"
#include "ui_mainwindow.h"

//------------------Вывод значений регистров на дисплей-----------------
void MainWindow::regDisplay() //
{
    ui->textEdit_regDisplay->clear(); // временная заглушка для вывода данных. Заменить на генерацию item
    for(int i=0; i<IREG_INV_ALL_END_REGISTERS; i++){
        if(regDataArray[i].displayed){
            ui->textEdit_regDisplay->append(regDataArray[i].displayString);
        }
    }
}
