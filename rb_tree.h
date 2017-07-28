#ifndef RB_TREE_H
#define RB_TREE_H
#include <iostream>
#define red 0
#define black 1
#define onright(x) (x->parent->rchild == x)
#define onleft(x) (x->parent->lchild == x)

template <typename T>
struct node{
    int color = 0;/*red for 0 ,black for 1*/
    T data;
    node<T>* parent = NULL;
    node<T>* lchild = NULL;
    node<T>* rchild = NULL;
    node<T>* succ(){
        node<T>* s = this;
        if(lchild){
            s = rchild;
            while(s->lchild!=NULL){s = s->lchild;}
        }
        else{
            while (onright(s)){s = s->parent;}
            s = s->parent;
        }
        return s;
    }
};


template <typename T>
class redblack{
public:
    node<T>* root;
    node<T>* now;
    int size = 0;
    void travIn( node<T>* x) {
        if(!x)return;
        travIn(x->lchild);
        std::cout<<x->data<<std::endl;
        travIn(x->rchild);
         }
    void solve_double_red(node<T>* hot);
    void solve_double_black(node<T>* hot);
    node<T>* removeat(node<T>* x,node<T>* now){
        node<T>* w = x;
        //used to instead
        node<T>* succed = NULL;
        int tag = 0;
        if(onleft(x)){
            tag = 1;
        }
        //if this pointer have no left child
        if(!(x->lchild)){
            succed = x->rchild;
        }
        else if(!(x->rchild)){
            succed = x->lchild;
        }
        else{
            w = w->succ();
            T temp=x->data;
            x->data = w->data;
            w->data = temp;
            node<T>* u = w->parent;
            ((u==x)?u->rchild:u->lchild) = succed = w->rchild;
        }
        now = w->parent;
        if(succed){
            succed->parent = now;
            if(tag){
                now->lchild = succed;
            }
            else{
                now->rchild = succed;
            }
        }
        delete(w);
        return succed;
    }
    node<T>*& FromParentTo(node<T>* hot){
        if(hot == root){
            return root;
        }
        else{
            return (onleft(hot)) ? hot->parent->lchild:hot->parent->rchild;
        }
    }
    node<T>* uncle(node<T>* hot){
        node<T>* grandfather = hot->parent->parent;
        if(grandfather->lchild == hot->parent){ return grandfather->rchild;}
        return grandfather->lchild;
    }
    node<T>* connect34(node<T>* a,node<T>* b,node<T>* c,node<T>* t0,node<T>* t1,node<T>* t2,node<T>* t3){
        a->lchild = t0;
        if(t0 != NULL)t0->parent = a;
        a->rchild = t1;
        if(t1 != NULL)t1->parent = a;
        c->lchild = t2;
        if(t2 != NULL)t2->parent = c;
        c->rchild = t3;
        if(t3 != NULL)t3->parent = c;
        b->lchild = a;
        if(a != NULL)a->parent = b;
        b->rchild = c;
        c->parent = b;
        return b;
    }
    node<T>* rotate(node<T>* v){
        node<T>* p = v->parent;
        node<T>* g = p->parent;
        if(onleft(p)){
            if(onleft(v)){//zig zig
                return connect34(v,p,g,v->lchild,v->rchild,p->rchild,g->rchild);

            }
            else{//zig zag
                v->parent = g->parent;
                return connect34(p,v,g,p->lchild,v->lchild,v->rchild,g->rchild);
            }
        }
        else{
            if(onright(v)){
                //zag zag
                p->parent = g->parent;
                return connect34(g,p,v,g->lchild,p->lchild,v->lchild,v->rchild);
            }
            else{
             v->parent = g->parent;
                return connect34(g,v,p,g->lchild,v->lchild,v->rchild,p->rchild);
            }
        }
    }


public:
    node<T>* insert(const T data);
    bool remove(T data);
    node<T>* search(const T data,node<T>* temp);
};
template <typename T>
bool redblack<T>::remove(T data) {
    node<T>* x = search(data,root);
    if(!x) return false;
    node<T>* r = removeat(x,now);
    if(!(--size))return true;
    if(!now){
        root->color = black;
        return true;
    }
    if(r == NULL){
        return true;
    }
    if(r->color == red){
        r->color = black;
        return  true;
    }
    solve_double_black(r);
    return true;
}

template <typename T>
void redblack<T>::solve_double_black(node<T> *hot) {
    node<T>* parent = hot->parent;
    if(parent == nullptr){
        return;
    }
    node<T>* brother = (hot == parent->lchild)?parent->rchild:parent->lchild;
    //如果兄弟节点为黑色
    if(brother->color == black){
        node<T>* t = nullptr;
        if(brother->rchild->color == red) t = brother->rchild;//借走右侄子
        if(brother->lchild->color == red) t = brother->rchild;//借走左侄子
        if(t){
          //说明有红色的侄子
            int precolor = parent->color;
            //备份其颜色
            node<T>* &b = FromParentTo(parent);
            b = rotate(t);
            if(b->lchild){b->lchild->color = black;}
            if(b->rchild){b->rchild->color = black;}
            b->color = precolor;
        }
        else{
            //借不出红侄子
            brother->color = red;//兄弟染红
            if(parent->color == red){
                parent->color = black;
            }
            else{
                solve_double_black(parent);
            }
        }
    } else{
        brother->color = black;
        parent->color =red;
        node<T>* t = onleft(brother) ? brother->lchild : brother->rchild;
        now = parent;
        node<T>* &temp = FromParentTo(parent);
        temp = rotate(t);
        solve_double_black(t);
    }

}

template <typename T>
void redblack<T>::solve_double_red(node<T> *hot) {
    //如果递归到树根,那么树根染成黑色,黑高度增加
    if(hot == root){
        root->color = black;return;
    }
    //如果点的父亲是黑色的,不存在需要修正的问题
    if(hot->parent->color == black){return;}
    node<T>* father = hot->parent;
    //如果p的父亲存在,而且p是红色的,那么p一定有父亲存在
    node<T>* grand = father->parent;
    node<T>* uncl = uncle(hot);
    //根据uncle的颜色进行处理
    //如果叔叔为black
    int tag = 0;
    if(uncl == NULL){
        tag = 1;
    }
    else if(uncl->color == black){
        tag = 1;
    }
    if(tag){
        //如果节点和父亲同侧
        if((onleft(hot)) == (onleft(father))){
            //染红
            father->color = black;
        }
        else{
            hot->color = black;
            grand->color = red;
            node<T>* gg = grand->parent;
            node<T>* &r = FromParentTo(grand);
            r = rotate(hot);
            r->parent = gg;
        }
    }
        //叔叔为red
    else{
        father->color = black;
        uncl->color = black;
        if(grand != root){
            grand->color = red;
        }
        solve_double_red(grand);
    }


}

template <typename T>
node<T>* redblack<T>::insert(const T data) {
    if(size == 0){
        root = new node<T>();
        root->data = data;
        root->color = black;
        size++;
        return root;
    }
    node<T>* x = search(data,root);
    if(x) return x;
    x = new node<T>;
    x->parent = now;x->color = red;size++;x->data = data;
    if(now->data > data){
        now->lchild = x;
    }
    else{
        now->rchild = x;
    }
    solve_double_red(x);
    return x;
}

template <typename T>
node<T>* redblack<T>::search(T data,node<T>* temp) {
    if(temp == nullptr){ return nullptr;}
    if(data == temp->data){return temp;}
    now = temp;
    if(data<temp->data){ return search(data,temp->lchild);}
    return search(data,temp->rchild);
}


#endif // RB_TREE_H
