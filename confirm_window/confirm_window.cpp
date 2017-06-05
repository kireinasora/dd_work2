#include "confirm_window.h"
#include "ui_confirm_window.h"
#include "dd_work.h"
#include <QStandardItemModel>

confirm_window::confirm_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::confirm_window)
{
    ui->setupUi(this);
	ui->tableView->verticalHeader()->hide();
	ui->tableView->horizontalHeader()->hide();
	ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

confirm_window::~confirm_window()
{
    delete ui;
}

void confirm_window::setData(char files_want_change[500][MAX_PATH], char files_new_name[500][MAX_PATH], int m) {

	QStandardItemModel *model = new QStandardItemModel();
	
	for (int i = 0; i < m; i++) {
		model->setItem(i, 0, new QStandardItem(files_want_change[i]));
		model->setItem(i, 1, new QStandardItem(files_new_name[i]));	
	}


	ui->tableView->setModel(model);
}