#include "main_window.h"
#include "ui_main_window.h"

#include <QFileDialog>
#include <QFile>

#include "scanner_thread.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  QFile n_file(":/text/notice.html");
  n_file.open(QIODevice::ReadOnly | QIODevice::Text);
  ui->log_view->setHtml(n_file.readAll());
  n_file.close();

  connect(ui->start_stop_btn, &QPushButton::clicked, this, &MainWindow::start_scan);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_browse_src_btn_clicked()
{
  QString src_path = QFileDialog::getExistingDirectory(this, QString(), QDir::homePath());
  if (src_path.isEmpty()) return;
  ui->src_edit->setText(QDir::toNativeSeparators(src_path));
}

void MainWindow::on_browse_dst_btn_clicked()
{
  QString dst_path = QFileDialog::getExistingDirectory(this, QString(), QDir::homePath());
  if (dst_path.isEmpty()) return;
  ui->dst_edit->setText(QDir::toNativeSeparators(dst_path));
}

void MainWindow::start_scan()
{
  QString src_path = QDir::fromNativeSeparators(ui->src_edit->text());
  QString dst_path = QDir::fromNativeSeparators(ui->dst_edit->text());

  ui->log_view->clear();

  QString err_fmt("<span style=\"color:#DC143C;\">%1</span>");
  if (!QFile::exists(src_path)) {
    ui->log_view->append(err_fmt.arg(tr("source path doesn't exists")));
    return;
  }
  if (!QFile::exists(dst_path)) {
    ui->log_view->append(err_fmt.arg(tr("target path doesn't exists")));
    return;
  }
  if (src_path.indexOf(dst_path) == 0 || dst_path.indexOf(src_path) == 0) {
    ui->log_view->append(err_fmt.arg(tr("source and target are subfolders")));
    return;
  }
  if (!QDir(src_path).isReadable()) {
    ui->log_view->append(err_fmt.arg(tr("source path is not readable")));
    return;
  }
  if (!QFileInfo(dst_path).isWritable()) {
    ui->log_view->append(err_fmt.arg(tr("destination path is not writable")));
    return;
  }

  ScannerThread* scan_thread = new ScannerThread;
  connect(scan_thread, &ScannerThread::started, this, &MainWindow::scan_started);
  connect(scan_thread, &ScannerThread::started, ui->log_view, &QTextEdit::clear);
  connect(scan_thread, &ScannerThread::finished, this, &MainWindow::scan_finished);
  connect(scan_thread, &ScannerThread::finished, scan_thread, &ScannerThread::deleteLater);
  connect(scan_thread, &ScannerThread::logMessage, ui->log_view, &QTextEdit::append);
  connect(scan_thread, &ScannerThread::updateStat, this, &MainWindow::display_stat);

  scan_thread->setSourceDir(src_path);
  scan_thread->setTargetDir(dst_path);

  scan_thread->setDeleteProcessed(ui->delete_processed_chb->isChecked());
  scan_thread->setFixExtensions(ui->fix_extensions_chb->isChecked());

  scan_thread->start();

  disconnect(ui->start_stop_btn, &QPushButton::clicked, 0, 0);
  connect(ui->start_stop_btn, &QPushButton::clicked, scan_thread, &ScannerThread::stop);
}

void MainWindow::scan_started()
{
  ui->src_label->setDisabled(true);
  ui->src_edit->setDisabled(true);
  ui->browse_src_btn->setDisabled(true);

  ui->dst_label->setDisabled(true);
  ui->dst_edit->setDisabled(true);
  ui->browse_dst_btn->setDisabled(true);

  ui->delete_processed_chb->setDisabled(true);
  ui->fix_extensions_chb->setDisabled(true);

  ui->start_stop_btn->setText(tr("Cancel"));
}

void MainWindow::scan_finished()
{
  ui->src_label->setEnabled(true);
  ui->src_edit->setEnabled(true);
  ui->browse_src_btn->setEnabled(true);

  ui->dst_label->setEnabled(true);
  ui->dst_edit->setEnabled(true);
  ui->browse_dst_btn->setEnabled(true);

  ui->delete_processed_chb->setEnabled(true);
  ui->fix_extensions_chb->setEnabled(true);

  ui->start_stop_btn->setText(tr("GO!"));

  disconnect(ui->start_stop_btn, &QPushButton::clicked, 0, 0);
  connect(ui->start_stop_btn, &QPushButton::clicked, this, &MainWindow::start_scan);
}

void MainWindow::display_stat(const ScannerThread::ScanStat& st)
{
  ui->statusbar->showMessage(tr("Files found: %1, processed: %2")
                             .arg(st.files_found)
                             .arg(st.files_procesed));
}
