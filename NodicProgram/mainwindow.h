#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    char IntToStr(char aChar);
    char StrToInt(char aChar);
    ~MainWindow();
    #define ALL_IN_ONE_CMD_1 "mergehex.exe -m nrf52832_xxaa_s132_bootloader.hex s132_nrf52_2.0.0_softdevice.hex -o temp.hex"
    #define ALL_IN_ONE_CMD_2 "mergehex.exe -m temp.hex nrf52832_xxaa_s132.hex -o ts102.hex"
    #define ERASE_ALL_CMD "nrfjprog.exe -f NRF52 --erasepage 0x1c000-0x80000"
    #define DOWNLOAD_CMD "nrfjprog.exe -f NRF52 --program nrf52832_xxaa_s132.hex --verify"
    #define RESET_CMD "nrfjprog.exe -f NRF52 --reset"
    #define DEL_TEMP "del temp.hex\n\r"
    #define READ_MAC_CMD "nrfjprog.exe --memrd 0x100000a3 --n 7"
    #define APP_NAME "APP烧录器"
    #define HEX_FILE_NAME "nrf52832_xxaa_s132.hex"

    Ui::MainWindow *ui;
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

private:
    bool copy_file_to_path(QString sourceDir ,QString toDir, bool coverFileIfExist);
    bool delete_file(QString filename);
    bool mac_read();
};

#endif // MAINWINDOW_H
