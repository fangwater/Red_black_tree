#include "red_black_tree.h"
#include <queue>
#include <map>
#define cr 30

red_black_tree::red_black_tree(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("red_black_tree_display"));
    resize(800,1000);
    input = new QLineEdit(this);
    input->setGeometry(20,20,60,30);
    insert = new QPushButton(this);
    insert->setText(tr("insert element"));
    insert->setGeometry(220,20,150,30);
    remove = new QPushButton(this);
    remove->setText(tr("remove element"));
    remove->setGeometry(420,20,150,30);
    QObject::connect(insert,SIGNAL(clicked(bool)),this,SLOT(insert_element()));
    QObject::connect(remove,SIGNAL(clicked(bool)),this,SLOT(remove_element()));
    rb_tree = new redblack<int>();
}

void red_black_tree::paintEvent(QPaintEvent*){
    QPainter painter(this);
    node<int>* pRoot = rb_tree->root;
    std::map<node<int>*,Entry*>* nodeinf = new std::map<node<int>*,Entry*>;
    Entry* start = new Entry(1,400,100);
    (*nodeinf)[pRoot] = start;
    painter.drawEllipse(start->xpostion-(cr/2),start->ypostion-(cr/2),cr,cr);
     std::queue<node<int>*> nodeQueue;
     nodeQueue.push(pRoot);
     while(!nodeQueue.empty())
     {
         node<int> *pNode = nodeQueue.front();
         nodeQueue.pop();
         if(pNode)
         {
             Entry* temp = (*nodeinf)[pNode];
             double move = 400;
             for(int m = 0;m<temp->ypostion/100;m++){
                 move = move*0.5;
             }
             if(pNode->lchild){
                 nodeinf->insert(std::make_pair(pNode->lchild,new Entry(pNode->lchild->color,temp->xpostion-move,temp->ypostion+100)));
                 painter.drawLine(temp->xpostion,temp->ypostion,temp->xpostion-move,temp->ypostion+100);
                 nodeQueue.push(pNode->lchild);}
             if(pNode->rchild){
                 nodeQueue.push(pNode->rchild);
                 painter.drawLine(temp->xpostion,temp->ypostion,temp->xpostion+move,temp->ypostion+100);
                 nodeinf->insert(std::make_pair(pNode->rchild,new Entry(pNode->rchild->color,temp->xpostion+move,temp->ypostion+100)));
             }
             Entry* draw = (*nodeinf)[pNode];
             painter.setPen(QColor(0,0,0));
             if(draw->color ==0){
                 painter.setPen(QColor(255,0,0));
             }
             painter.drawEllipse(draw->xpostion-(cr/2),draw->ypostion-(cr/2),cr,cr);
             painter.drawText(draw->xpostion,draw->ypostion,QString::number(pNode->data));
         }
     }
     delete nodeinf;
}
void red_black_tree::insert_element(){
    rb_tree->insert(input->text().toInt());
    update();
}
void red_black_tree::remove_element(){
    rb_tree->remove(input->text().toInt());
    update();
}


