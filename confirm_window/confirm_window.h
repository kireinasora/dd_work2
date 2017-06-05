#ifndef CONFIRM_WINDOW_H
#define CONFIRM_WINDOW_H

#include <QDialog>
#include <windows.h>

namespace Ui {
class confirm_window;
}

class confirm_window : public QDialog
{
    Q_OBJECT

public:
    explicit confirm_window(QWidget *parent = 0);
    ~confirm_window();
	void setData(char files_want_change[500][MAX_PATH], char files_new_name[500][MAX_PATH], int m);

private:
    Ui::confirm_window *ui;

};

#endif // CONFIRM_WINDOW_H
