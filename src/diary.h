#ifndef BOOKSTORE_DIARY_H
#define BOOKSTORE_DIARY_H
//说明：
#include <iostream>
#include "linkedList.h"
#include "user.h"
#include "book.h"
#include "strScanner.h"
#include "error.cpp"

static linkedList<double> diaryBlock("Diary_storage");
static int Count;

void initDiary();
void writeCount();
void addDiary(const double &num);
void showFinance(strScanner &scanner);
#endif //BOOKSTORE_DIARY_H
