#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QPainter>
#include <map>
#include "entry.h"
#include "rb_tree.h"

class red_black_tree : public QWidget
{
    Q_OBJECT

public:
    red_black_tree(QWidget *parent = 0);
    QLineEdit* input;
    QPushButton* insert;
    QPushButton* remove;
    redblack<int>* rb_tree;
public slots:
    void insert_element();
    void remove_element();
protected:
    void paintEvent(QPaintEvent*);
};

#endif // RED_BLACK_TREE_H
