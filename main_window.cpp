#include "main_window.h"
#include "ui_main_window.h"

#include <QFileDialog>

#include "scanner_thread.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
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
  ScannerThread* scan_thread = new ScannerThread;
  connect(scan_thread, &ScannerThread::started, this, &MainWindow::scan_started);
  connect(scan_thread, &ScannerThread::started, ui->log_view, &QTextEdit::clear);
  connect(scan_thread, &ScannerThread::finished, this, &MainWindow::scan_finished);
  connect(scan_thread, &ScannerThread::finished, scan_thread, &ScannerThread::deleteLater);
  connect(scan_thread, &ScannerThread::logMessage, ui->log_view, &QTextEdit::append);
  connect(scan_thread, &ScannerThread::updateStat, this, &MainWindow::display_stat);

  scan_thread->setSourceDir(QDir::fromNativeSeparators(ui->src_edit->text()));
  scan_thread->setTargetDir(QDir::fromNativeSeparators(ui->dst_edit->text()));

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
