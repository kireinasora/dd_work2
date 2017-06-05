#pragma once

#include <QtWidgets/QWidget>
#include <qmessagebox.h>
#include <qdialogbuttonbox.h>
#include 

class ScrollMessageBox : public QDialog
{
  Q_OBJECT

public:
  ScrollMessageBox(QMessageBox::Icon icon, QString const& title, QString const& text,
    QDialogButtonBox::StandardButtons buttons = QDialogButtonBox::Ok, QWidget* parent = 0);

  void setDefaultButton(QDialogButtonBox::StandardButton button);

  static QDialogButtonBox::StandardButton critical(QWidget* parent, QString const& title, QString const& text, QDialogButtonBox::StandardButtons buttons = QDialogButtonBox::Ok, QDialogButtonBox::StandardButton defaultButton = QDialogButtonBox::NoButton);
  static QDialogButtonBox::StandardButton information(QWidget* parent, QString const& title, QString const& text, QDialogButtonBox::StandardButtons buttons = QDialogButtonBox::Ok, QDialogButtonBox::StandardButton defaultButton = QDialogButtonBox::NoButton);
  static QDialogButtonBox::StandardButton question(QWidget* parent, QString const& title, QString const& text, QDialogButtonBox::StandardButtons buttons = QDialogButtonBox::Ok, QDialogButtonBox::StandardButton defaultButton = QDialogButtonBox::NoButton);
  static QDialogButtonBox::StandardButton warning(QWidget* parent, QString const& title, QString const& text, QDialogButtonBox::StandardButtons buttons = QDialogButtonBox::Ok, QDialogButtonBox::StandardButton defaultButton = QDialogButtonBox::NoButton);

  void showEvent ( QShowEvent * event ) override;

private:
  QPixmap standardIcon(QMessageBox::Icon icon);
  void setDefaultButton(QPushButton *button);
  void updateSize();
 
  QLabel *label;
  QDialogButtonBox *buttonBox;

private Q_SLOTS:
  void handle_buttonClicked(QAbstractButton *button);
};

