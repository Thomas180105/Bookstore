#ifndef BOOKSTORE_LINKEDLIST_H
#define BOOKSTORE_LINKEDLIST_H

//类T至少应当支持:< > = 赋值构造 （<<, 如果你需要进行输出调试）
//linkedList类基于文件实现了char[] -> T的键值对存储，允许同一key对应多个值T。目前的对外接口
//std::pair<bool, std::vector<T>> find(char index_[]) ,支持同一char[]对应多个T
//void insert(char index_[], T value_)
//void del(char index_[], T value_)
//std::vector<T> findAll()
//void modify(char index_[], T his_value, T new_value) 使用条件：保证当前模式是one key - one value

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

const int BLOCK_SIZE = 300;
const int maxSize = 300;
const int minSize = 120;

template<class T>
struct map_pair{
    char index[65];
    T value;
    map_pair() : index("") {}
    map_pair(const char index_[], const T value_) : value(value_)
    {
        strcpy(index, index_);
    }
    bool operator<(const map_pair &obj)
    {
        if (strcmp(index, obj.index) < 0) return true;
        if (strcmp(index, obj.index) > 0) return false;
        return value < obj.value;
    }
    bool operator>(const map_pair &obj)
    {
        if (strcmp(index, obj.index) > 0) return true;
        if (strcmp(index, obj.index) < 0) return false;
        return value > obj.value;
    }
    map_pair &operator=(const map_pair &obj)
    {
        if (this == &obj) return *this;
        value = obj.value;
        strcpy(index, obj.index);
        return *this;
    }
};
struct remainder{
    char maximum[65];
    char minimum[65];
    int prev;//The starting position of the prev block in the file
    int next;
    remainder(int x = -1, int y = -1) : prev(x), next(y)
    {
        strcpy(maximum, "");
        strcpy(minimum, "");
    }
};
const int size1 = sizeof (remainder);
template<class T>
struct BlockNode{
    int size;//The number of elements stored in the array currently
    map_pair<T> array[BLOCK_SIZE];

    BlockNode() : size(0)
    {
        for (int i = 0; i < BLOCK_SIZE; ++i)
        {
            strcpy(array[i].index, "");
            array[i].value = T();
        }
    }

    BlockNode &operator=(const BlockNode &obj)
    {
        if (this == &obj) return *this;
        size = obj.size;
        for (int i = 0; i < size; ++i) array[i] = obj.array[i];
        return *this;
    }
};
//const int size2 = sizeof (BlockNode<T>);
//const int total_size = size1 + size2;

template<class T>
class linkedList {
public:
    int head;//the starting pos of the first remainder in the file
    int tail;//the starting pos of the last remainder in the file
    int file_end;
    std::fstream file;

private:

    void Split(int pos);//split the node located at pos

    void borrow(int type, int dest_pos, int src_pos);//dest_pos borrow from src_pos, the type 1 is borrow from prev, meanwhile type 2 is borrow from next

    void Union_to_prev(int from_pos, int to_pos); //Priority over Union_to_next()

    void Union_to_next(int from_pos, int to_pos);

    void insert_to_start(char index_[], T value_, int pos);//insert to the node located at pos, and insert to the very start of this node

    void insert_to_end(char index_[], T value_, int pos);//insert to the node located at pos, and insert to the very end of this node

public:
    explicit linkedList(const std::string &name);

    std::pair<bool, std::vector<T>> find(const char *index_);

    void insert(const char *index_, const T &value_);

    void del(const char *index_, const T &value_);

    std::vector<T> findAll();

    void modify(const char *index_, const T &his_value, const T &new_value);
};

#endif //BOOKSTORE_LINKEDLIST_H
