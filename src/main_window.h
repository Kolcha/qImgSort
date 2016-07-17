#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

#include "scanner_thread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_browse_src_btn_clicked();
  void on_browse_dst_btn_clicked();

  void start_scan();

  void scan_started();
  void scan_finished();

  void display_stat(const ScannerThread::ScanStat& st);

private:
  Ui::MainWindow *ui;
};

#endif // MAIN_WINDOW_H