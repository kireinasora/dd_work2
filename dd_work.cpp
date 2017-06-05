#include "dd_work.h"
#include "confirm_window\confirm_window.h" 
#include <windows.h>
#include <string.h>
#include "qfiledialog.h"
#include "qmessagebox.h"
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QTableView>



int FindFiles(char oldnames[500][MAX_PATH], char dir[MAX_PATH], char chose_extension[10]);
void FindFormat(char input[20],char input_2[20], char input_3[20], char oldnames[500][MAX_PATH], char newnames_without_extension[500][MAX_PATH], char newnames[500][MAX_PATH]);

int number_of_files;
int m;

dd_work::dd_work(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	find_current_directory();

	setlocale(LC_ALL, "");
}


void dd_work::on_current_folder_clicked(void) {
	find_current_directory();
	ui.selected_folder->setText(this->dir);
}

void dd_work::on_browse_folder_clicked(void) {

	//Choose Directory
	QFileDialog *filedialog = new QFileDialog(this);
	filedialog->setWindowTitle("請選擇資料夾");
	filedialog->setFileMode(QFileDialog::Directory);
	filedialog->setOption(QFileDialog::ShowDirsOnly);
	if (filedialog->exec() == QDialog::Accepted) {
		strcpy_s(this->dir, ((filedialog->selectedFiles()[0]).toUtf8()).data());
	}
	for (int i = 0; i < strlen(this->dir); i++) {
		if (this->dir[i] == '/') this->dir[i] = '\\';
	}
	if(this->dir[strlen(this->dir)-1]!='\\')
		strcat(this->dir, "\\");
	ui.selected_folder->setText(this->dir);
}

void dd_work::on_pushButton_clicked(void) {
	//Choose extension
	if(this->dir[strlen(this->dir)-1]!='\\')
		strcat(this->dir, "\\");
	char chose_extension[10] = "*";
	if (ui.radioButton->isChecked()) strcpy(chose_extension, "*.pdf");
	else if (ui.radioButton_2->isChecked())strcpy(chose_extension, "*.*");
	else if (ui.radioButton_3->isChecked()) {
		char extension_tmp[10];
		strcpy(extension_tmp, ((ui.extension->toPlainText()).toUtf8()).data());
		strcat_s(chose_extension, extension_tmp);
	}
	
	//initizelize
	for (int i = 0; i < 500; i++) strcpy(this->oldnames[i]," ");
	for (int i = 0; i < 500; i++) strcpy(this->files_new_name[i]," ");
	for (int i = 0; i < 500; i++) strcpy(this->files_want_change[i]," ");


	if (!(FindFiles(this->oldnames, this->dir, chose_extension))) {
		char input_1[20];
		char input_2[20];
		char input_3[20];
		strcpy(input_1, ((ui.first_input->toPlainText()).toUtf8()).data());
		strcpy(input_2, ((ui.sec_input->toPlainText()).toUtf8()).data());
		strcpy(input_3, ((ui.third_input->toPlainText()).toUtf8()).data());
		
		FindFormat(input_1, input_2, input_3, this->oldnames, this->files_new_name, this->files_want_change);
		
		//rename

		char buff[MAX_PATH];
		strcpy_s(buff, this->dir);
		strcat_s(buff, this->files_want_change[0]);


		confirm_window window;
		window.setData(this->files_want_change,this->files_new_name, m);
		window.setModal(true);

		if (window.exec() == QDialog::Accepted) {
			for (int i = 0; i < m; i++) {
				char buff[MAX_PATH];
				char buff1[MAX_PATH];
				strcpy_s(buff, this->dir);
				strcpy_s(buff1, this->dir);
				strcat_s(buff, this->files_want_change[i]);
				strcat_s(buff1, this->files_new_name[i]);
				rename(buff, buff1);
			}
		}
	}
}

void dd_work::on_pushButton_2_clicked(void) {
	HANDLE hFile;
	WIN32_FIND_DATAA FileData;
	char path_with_extension[MAX_PATH];

	if(this->dir[strlen(this->dir)-1]!='\\')
		strcat(this->dir, "\\");
	strcpy_s(path_with_extension, this->dir);
	strcat_s(path_with_extension, "*.*");

	hFile = FindFirstFileA(path_with_extension, &FileData);
	
	number_of_files = 0;
	
	if (hFile == INVALID_HANDLE_VALUE) {
		QMessageBox::information(0, QStringLiteral("搵唔到啊!"), "Cannot find any files!");
	}
	else {
		while(FindNextFileA(hFile, &FileData)){
			if (FileData.cFileName[0] == '.')
				continue;
			strcpy_s(this->oldnames[number_of_files], FileData.cFileName);
			number_of_files++;
		}
	}
	
	//EXEC
	bool check;
	m = 0;
	int a = 0;
	int i, j, k, n, y;
	for (i = 0; i < number_of_files; i++) {
        check=FALSE;
		for (j = 0; j < strlen(this->oldnames[i]); j++){
			if ((this->oldnames[i][j] == '_' || this->oldnames[i][j] == '.' || this->oldnames[i][j] == ' ')&& !isdigit(this->oldnames[i][j+1])) {
				for (k = j + 1; k < strlen(this->oldnames[i] - j); k++) {
					if (this->oldnames[i][k] == '-' && isdigit(this->oldnames[i][k + 1]) && isdigit(this->oldnames[i][k + 2])) {
						check = TRUE;
						break;
					}
				}
			}
			if (check == TRUE) {
				strcpy(this->files_want_change[a], this->oldnames[i]);

				j++;
				int tmp = j;
				for (n = 0; n < (k+2-tmp+1); j++, n++) {
					this->files_new_name[m][n] = this->oldnames[i][j];
				}
				this->files_new_name[m][n] = '\0';

				//extension
				char extension[10];
				n = strlen(this->oldnames[i])-1;
				while (this->oldnames[i][n] != '.') n--;
				y = 0;
				while (n<strlen(this->oldnames[i])) {
					extension[y] = this->oldnames[i][n];
					n++;
					y++;
				}
				extension[y] = '\0';
				strcat(this->files_new_name[m], extension);

				m++;
				a++;
				break;
			}
		}
	}

	//rename

	char buff[MAX_PATH];
	strcpy_s(buff, this->dir);
	strcat_s(buff, this->files_want_change[0]);


	confirm_window window;
	window.setData(this->files_want_change,this->files_new_name, m);
	window.setModal(true);

	if (window.exec() == QDialog::Accepted) {
		for (int i = 0; i < m; i++) {
			char buff[MAX_PATH];
			char buff1[MAX_PATH];
			strcpy_s(buff, this->dir);
			strcpy_s(buff1, this->dir);
			strcat_s(buff, this->files_want_change[i]);
			strcat_s(buff1, this->files_new_name[i]);
			rename(buff, buff1);
		}
	}
}


int FindFiles(char oldnames[500][MAX_PATH], char dir[MAX_PATH], char chose_extension[10]) {
	/*find files in directory*/
	HANDLE hFile;
	WIN32_FIND_DATAA FileData;
	char path_with_extension[MAX_PATH];

	strcpy_s(path_with_extension, dir);
	strcat_s(path_with_extension, chose_extension);



	hFile = FindFirstFileA(path_with_extension, &FileData);
	
	number_of_files = 0;
	
	if (hFile == INVALID_HANDLE_VALUE) {
		QMessageBox::information(0, QStringLiteral("搵唔到啊!"), "Cannot find any files!");
		return 1;
	}
	else {
		if (strcmp(chose_extension, "*.*")) {				
			strcpy_s(oldnames[number_of_files], FileData.cFileName);
			number_of_files++;
		}
		while(FindNextFileA(hFile, &FileData)){
			if (FileData.cFileName[0] == '.')
				continue;
			strcpy_s(oldnames[number_of_files], FileData.cFileName);
			number_of_files++;
		}
	}
	return 0;
}

void FindFormat(char input[20],char input_2[20], char input_3[20], char oldnames[500][MAX_PATH], char files_new_name[500][MAX_PATH],char files_want_change[500][MAX_PATH]) {
	
	bool check;

	int j, k, a;
	m = 0;
	a = 0;
	int y, n;

    for (int i =0; i<number_of_files; i++){
        check=FALSE;
        for(j =0; j<strlen(oldnames[i])-strlen(input); j++){
            for(k =0; k<strlen(input); k++){
                if(input[k]== oldnames[i][j+k]){
                    check=TRUE;
                    continue;
                }
                else if(input[k]=='#' && oldnames[i][j+k]>='0' && oldnames[i][j+k]<='9'){
                    check=TRUE;
                    continue;
                }
                else if(input[k]=='@'&& (oldnames[i][j+k]>=' ' && oldnames[i][j+k]<='/' || (oldnames[i][j+k]>=':'&& oldnames[i][j+k]<='~'))){
                    check=TRUE;
                    continue;
                }
                else if(input[k]=='?'&& oldnames[i][j+k]>=' ' && oldnames[i][j+k]<='~'){
                    check=TRUE;
                    continue;
                }
                else {
                    check=FALSE;
                    break;
                }
            }
			if (check == TRUE) {

				strcpy(files_want_change[a], oldnames[i]);

				for (n = 0; n < k; j++, n++) {
					files_new_name[m][n] = oldnames[i][j];
				}
				files_new_name[m][n] = '\0';

				//extension
				char extension[10];
				n = strlen(oldnames[i])-1;
				while (oldnames[i][n] != '.') n--;
				y = 0;
				while (n<strlen(oldnames[i])) {
					extension[y] = oldnames[i][n];
					n++;
					y++;
				}
				extension[y] = '\0';
				strcat(files_new_name[m], extension);

				m++;
				a++;
				break;
            }
        }
        if(check==TRUE) continue;

        if(strlen(input_2)!=0){
            for(j =0; j<strlen(oldnames[i])-strlen(input_2); j++){
                for(k =0; k<strlen(input_2); k++){
                    if(input_2[k]== oldnames[i][j+k]){
                        check=TRUE;
                        continue;
                    }
                    else if(input_2[k]=='#' && oldnames[i][j+k]>='0' && oldnames[i][j+k]<='9'){
                        check=TRUE;
                        continue;
                    }
                    else if(input_2[k]=='@'&& (oldnames[i][j+k]>=' ' && oldnames[i][j+k]<='/' || (oldnames[i][j+k]>=':'&& oldnames[i][j+k]<='~'))){
                        check=TRUE;
                        continue;
                    }
                    else if(input_2[k]=='?'&& oldnames[i][j+k]>=' ' && oldnames[i][j+k]<='~'){
                        check=TRUE;
                        continue;
                    }
                    else {
                        check=FALSE;
                        break;
                    }
                }
				if (check == TRUE) {

					strcpy(files_want_change[a], oldnames[i]);

					for (n = 0; n < k; j++, n++) {
						files_new_name[m][n] = oldnames[i][j];
					}
					files_new_name[m][n] = '\0';

					//extension
					char extension[10];
					n = strlen(oldnames[i])-1;
					while (oldnames[i][n] != '.') n--;
					int y = 0;
					while (n<strlen(oldnames[i])) {
						extension[y] = oldnames[i][n];
						n++;
						y++;
					}
					extension[y] = '\0';
					strcat(files_new_name[m], extension);

					m++;
					a++;
					break;
				}	
            }
        }
        if(check==TRUE) continue;

        if(strlen(input_3)!=0){
            for(j =0; j<strlen(oldnames[i])-strlen(input_3); j++){
                for(k =0; k<strlen(input_3); k++){
                    if(input_3[k]== oldnames[i][j+k]){
                        check=TRUE;
                        continue;
                    }
                    else if(input_3[k]=='#' && oldnames[i][j+k]>='0' && oldnames[i][j+k]<='9'){
                        check=TRUE;
                        continue;
                    }
                    else if(input_3[k]=='@'&& (oldnames[i][j+k]>=' ' && oldnames[i][j+k]<='/' || (oldnames[i][j+k]>=':'&& oldnames[i][j+k]<='~'))){
                        check=TRUE;
                        continue;
                    }
                    else if(input_3[k]=='?'&& oldnames[i][j+k]>=' ' && oldnames[i][j+k]<='~'){
                        check=TRUE;
                        continue;
                    }
                    else {
                        check=FALSE;
                        break;
                    }
                }
				if (check == TRUE) {
					strcpy(files_want_change[a], oldnames[i]);

					for (n = 0; n < k; j++, n++) {
						files_new_name[m][n] = oldnames[i][j];
					}
					files_new_name[m][n] = '\0';

					//extension
					char extension[10];
					n = strlen(oldnames[i])-1;
					while (oldnames[i][n] != '.') n--;
					y = 0;
					while (n<strlen(oldnames[i])) {
						extension[y] = oldnames[i][n];
						n++;
						y++;
					}
					extension[y] = '\0';
					strcat(files_new_name[m], extension);

					m++;
					a++;
					break;
				}
            }
        }
    }
}