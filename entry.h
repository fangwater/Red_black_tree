#ifndef ENTRY_H
#define ENTRY_H
#define red 0
#define black 1

struct Entry{
    int xpostion;
    int ypostion;
    int color;
    Entry(int co,int x,int y):color(co),xpostion(x),ypostion(y){};
};
#endif // ENTRY_H
