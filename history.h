#ifndef HISTORY_H
#define HISTORY_H

#include <QDialog>
#include <QSqlTableModel>
#include <QDebug>
#include <QModelIndex>

namespace Ui {
class history;
}

class history : public QDialog
{
    Q_OBJECT

public:
    explicit history(QWidget *parent = 0);
    ~history();
    QSqlTableModel *tabModel;
private:
    Ui::history *ui;
public slots:
    void slotRowDoubleClicked(const QModelIndex index);
signals:
    void history_updata(QModelIndex index);
};

#endif // HISTORY_H
