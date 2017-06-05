#pragma once

#include <string.h>
#include <windows.h>
#include <QtWidgets/QWidget>
#include "ui_dd_work.h"
#include "qmessagebox.h"

class dd_work : public QWidget
{
	Q_OBJECT

public:
	dd_work(QWidget *parent = Q_NULLPTR);
    char oldnames[500][MAX_PATH];
	char files_new_name[500][MAX_PATH];
	char files_want_change[500][MAX_PATH];
    char dir[MAX_PATH];
	
	void find_current_directory(void) {
		strcpy(this->dir, ((qApp->applicationDirPath()).toUtf8()).data());
		int i;
		for (i = 0; i < strlen(this->dir); i++) {
			if (this->dir[i] == '/') {
				this->dir[i] = '\\';
			}
		}
		this->dir[i] = '\\';
		this->dir[i + 1] = '\0';
		
		//GetModuleFileNameA(NULL, this->dir, MAX_PATH);
		//int i;
		//for (i = strlen(this->dir); this->dir[i] != '\\'; i--);
		//this->dir[i+1] = '\0';

	}



private:
	Ui::dd_workClass ui;

public slots:
	void on_current_folder_clicked(void);
	void on_browse_folder_clicked(void);
	void on_pushButton_clicked(void);
	void on_pushButton_2_clicked(void);
};

