//类T至少应当支持:< > = 赋值构造 （<<, 如果你需要进行输出调试）
//linkedList类基于文件实现了char[] -> T的键值对存储，允许同一key对应多个值T。目前的对外接口
//std::pair<bool, std::vector<T>> find(char index_[]) ,支持同一char[]对应多个T
//void insert(char index_[], T value_)
//void del(char index_[], T value_)
//std::vector<T> FindAll()

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

    void Split(int pos)//split the node located at pos
    {
        remainder fa_rem, son_rem, next_rem;
        BlockNode<T> fa_node, son_node;

        file.seekg(pos);
        file.read(reinterpret_cast<char *> (&fa_rem), size1);
        file.read(reinterpret_cast<char *> (&fa_node), sizeof (BlockNode<T>));

        int next_pos = fa_rem.next;

        if (next_pos != -1)
        {
            file.seekg(next_pos);
            file.read(reinterpret_cast<char *> (&next_rem), size1);
        }

        fa_rem.next = file_end, son_rem.prev = pos;
        if (next_pos != -1) son_rem.next = next_pos, next_rem.prev = file_end;
        strcpy(fa_rem.maximum, fa_node.array[BLOCK_SIZE / 2 - 1].index);
        strcpy(son_rem.minimum, fa_node.array[BLOCK_SIZE / 2].index);
        strcpy(son_rem.maximum, fa_node.array[BLOCK_SIZE - 1].index);
        fa_node.size = son_node.size = BLOCK_SIZE / 2;
        for (int i = 0; i < fa_node.size; ++i) son_node.array[i] = fa_node.array[i + BLOCK_SIZE / 2];

        file.seekp(pos);
        file.write(reinterpret_cast<char *> (&fa_rem), size1);
        file.write(reinterpret_cast<char *> (&fa_node), sizeof (BlockNode<T>));
        file.seekp(file_end);
        file.write(reinterpret_cast<char *> (&son_rem), size1);
        file.write(reinterpret_cast<char *> (&son_node), sizeof (BlockNode<T>));
        if (next_pos != -1)
        {
            file.seekp(next_pos);
            file.write(reinterpret_cast<char *> (&next_rem), size1);
        }

        if (next_pos == -1)
        {
//            tail += total_size; //update in 12/18
            tail = file_end;
            file.seekp(sizeof(int));
            file.write(reinterpret_cast<char *> (&tail), sizeof(int));
        }

        file_end += (size1 + sizeof (BlockNode<T>));
        file.seekp(2 * sizeof(int));
        file.write(reinterpret_cast<char *> (&file_end), sizeof (int));

    }

    void borrow(int type, int dest_pos, int src_pos)//dest_pos borrow from src_pos, the type 1 is borrow from prev, meanwhile type 2 is borrow from next
    {
        remainder dest_rem, src_rem;
        BlockNode<T> dest_node, src_node;
        file.seekg(dest_pos);
        file.read(reinterpret_cast<char *> (&dest_rem), size1);
        file.read(reinterpret_cast<char *> (&dest_node), sizeof (BlockNode<T>));
        file.seekg(src_pos);
        file.read(reinterpret_cast<char *> (&src_rem), size1);
        file.read(reinterpret_cast<char *> (&src_node), sizeof (BlockNode<T>));

        if (type == 1)
        {
            strcpy(src_rem.maximum, src_node.array[src_node.size - 2].index);
            strcpy(dest_rem.minimum, src_node.array[src_node.size - 1].index);
            for (int i = dest_node.size - 1; i >= 0; --i) dest_node.array[i + 1] = dest_node.array[i];
            dest_node.array[0] = src_node.array[src_node.size - 1];
            src_node.size--;
            dest_node.size++;
        }
        else
        {
            strcpy(src_rem.minimum, src_node.array[1].index);
            strcpy(dest_rem.maximum, src_node.array[0].index);
            dest_node.array[dest_node.size] = src_node.array[0];
            for (int i = 0; i < src_node.size - 1; ++i)
            {
                src_node.array[i] = src_node.array[i + 1];
            }
            src_node.size--;
            dest_node.size++;
        }

        file.seekp(dest_pos);
        file.write(reinterpret_cast<char *> (&dest_rem), size1);
        file.write(reinterpret_cast<char *> (&dest_node), sizeof (BlockNode<T>));
        file.seekp(src_pos);
        file.write(reinterpret_cast<char *> (&src_rem), size1);
        file.write(reinterpret_cast<char *> (&src_node), sizeof (BlockNode<T>));
    }

    void Union_to_prev(int from_pos, int to_pos) //Priority over Union_to_next()
    {
        remainder from_rem, to_rem, next_rem;
        BlockNode<T> from_node, to_node;
        file.seekg(from_pos);
        file.read(reinterpret_cast<char *> (&from_rem), size1);
        file.read(reinterpret_cast<char *> (&from_node), sizeof (BlockNode<T>));
        file.seekg(to_pos);
        file.read(reinterpret_cast<char *> (&to_rem), size1);
        file.read(reinterpret_cast<char *> (&to_node), sizeof (BlockNode<T>));
        int next_pos = from_rem.next;
        if (next_pos != -1)
        {
            file.seekg(next_pos);
            file.read(reinterpret_cast<char *> (&next_rem), size1);
        }

        to_rem.next = next_pos;
        if (next_pos != -1) next_rem.prev = to_pos;
        strcpy(to_rem.maximum, from_rem.maximum);
        for (int i = 0; i < from_node.size; ++i)
        {
            to_node.array[i + to_node.size] = from_node.array[i];
        }
        to_node.size += from_node.size;
        if (from_pos == tail)
        {
            tail = to_pos;
            file.seekp(sizeof(int));
            file.write(reinterpret_cast<char *> (&tail), sizeof(int));
        }

        file.seekp(to_pos);
        file.write(reinterpret_cast<char *> (&to_rem), size1);
        file.write(reinterpret_cast<char *> (&to_node), sizeof (BlockNode<T>));
        if (next_pos != -1)
        {
            file.seekp(next_pos);
            file.write(reinterpret_cast<char *> (&next_rem), size1);
        }
    }

    void Union_to_next(int from_pos, int to_pos)
    {
        remainder from_rem, to_rem, prev_rem;
        BlockNode<T> from_node, to_node;
        file.seekg(from_pos);
        file.read(reinterpret_cast<char *> (&from_rem), size1);
        file.read(reinterpret_cast<char *> (&from_node), sizeof (BlockNode<T>));
        file.seekg(to_pos);
        file.read(reinterpret_cast<char *> (&to_rem), size1);
        file.read(reinterpret_cast<char *> (&to_node), sizeof (BlockNode<T>));
        int prev_pos = from_rem.prev;
        if (prev_pos != -1)
        {
            file.seekg(prev_pos);
            file.read(reinterpret_cast<char *> (&prev_rem), size1);
        }

        //change the linkedList struct, meanwhile from_rem & from_node actually is deleted
        to_rem.prev = prev_pos;
        if (prev_pos != -1) prev_rem.next = to_pos;
        strcpy(to_rem.minimum, from_rem.minimum);

        for (int i = to_node.size - 1; i >= 0; --i) to_node.array[i + from_node.size] = to_node.array[i];
        for (int i = 0; i < from_node.size; ++i)
        {
            to_node.array[i] = from_node.array[i];
        }
        to_node.size += from_node.size;
        if (from_pos == head)
        {
            head = to_pos;//head = from_pos; update in 2022/12/18
            file.seekp(0);//file.seekp(sizeof(int)); update in 2022/12/18
            file.write(reinterpret_cast<char *> (&head), sizeof(int));
        }

        file.seekp(to_pos);
        file.write(reinterpret_cast<char *> (&to_rem), size1);
        file.write(reinterpret_cast<char *> (&to_node), sizeof (BlockNode<T>));
        if (prev_pos != -1)
        {
            file.seekp(prev_pos);
            file.write(reinterpret_cast<char *> (&prev_rem), size1);
        }
    }

    void insert_to_start(char index_[], T value_, int pos)//insert to the node located at pos, and insert to the very start of this node
    {
        remainder cur_rem;
        BlockNode<T> cur_node;
        map_pair<T> to_insert(index_, value_);
        file.seekg(pos);
        file.read(reinterpret_cast<char *> (&cur_rem), size1);
        file.read(reinterpret_cast<char *> (&cur_node), sizeof (BlockNode<T>));
        strcpy(cur_rem.minimum, index_);
        for (int i = cur_node.size - 1; i >= 0; --i) cur_node.array[i + 1] = cur_node.array[i];
        cur_node.array[0] = to_insert;
        cur_node.size++;
        file.seekp(pos);
        file.write(reinterpret_cast<char *> (&cur_rem), size1);
        file.write(reinterpret_cast<char *> (&cur_node), sizeof (BlockNode<T>));
        if (cur_node.size == maxSize) Split(pos);
    }

    void insert_to_end(char index_[], T value_, int pos)//insert to the node located at pos, and insert to the very end of this node
    {
        remainder cur_rem;
        BlockNode<T> cur_node;
        map_pair<T> to_insert(index_, value_);
        file.seekg(pos);
        file.read(reinterpret_cast<char *> (&cur_rem), size1);
        file.read(reinterpret_cast<char *> (&cur_node), sizeof (BlockNode<T>));
        strcpy(cur_rem.maximum, index_);
        cur_node.array[cur_node.size] = to_insert;
        cur_node.size++;
        file.seekp(pos);
        file.write(reinterpret_cast<char *> (&cur_rem), size1);
        file.write(reinterpret_cast<char *> (&cur_node), sizeof (BlockNode<T>));
        if (cur_node.size == maxSize) Split(pos);
    }

public:
    explicit linkedList(const std::string &name)
    {
        std::ifstream infile(name);
        if (!infile)
        {
            std::ofstream outfile(name);
            file.open(name);
            outfile.close();
            int file_head = -1, file_tail = -1, f_end = 3 * sizeof(int);
            file.seekp(0);
            file.write(reinterpret_cast<char *> (&file_head), sizeof(int));
            file.write(reinterpret_cast<char *> (&file_tail), sizeof(int));
            file.write(reinterpret_cast<char *> (&f_end), sizeof(int));
        }
        else file.open(name);

        int file_head, file_tail, f_end;
        file.seekg(0);
        file.read(reinterpret_cast<char *> (&file_head), sizeof(int));
        file.read(reinterpret_cast<char *> (&file_tail), sizeof(int));
        file.read(reinterpret_cast<char *> (&f_end), sizeof(int));

        head = file_head;
        tail = file_tail;
        file_end = f_end;
    }

    std::pair<bool, std::vector<T>> find(char index_[])
    {
        //There are no values to maintain
        int cur_pos;
        file.seekg(0);
        file.read(reinterpret_cast<char *> (&cur_pos), sizeof(int));

        remainder cur_rem;
        BlockNode<T> cur_node;
        bool flag = false;
        std::vector<T> res;
        while(cur_pos != -1)//not out of the linkedList yet
        {
            file.seekg(cur_pos);
            file.read(reinterpret_cast<char *> (&cur_rem), size1);
            if (strcmp(cur_rem.minimum, index_) <= 0 && strcmp(index_, cur_rem.maximum) <= 0)
            {
                file.seekg(cur_pos + size1);
                file.read(reinterpret_cast<char *> (&cur_node), sizeof (BlockNode<T>));
                for (int i = 0; i < cur_node.size; i++)
                {
                    if (strcmp(cur_node.array[i].index, index_) == 0)
                    {
                        flag = true;
                        res.push_back(cur_node.array[i].value);
                    }
                    else if (flag) break;
                }
            }
            else if (flag) break;
            cur_pos = cur_rem.next;
        }
        return std::move(std::make_pair(flag, res));
    }

    void insert(char index_[], T value_)
    {
        //needs to be maintained : (head) (tail) BlocKNode.size BlockNode.Array (new_remainder) (new_BlockNode)
        map_pair<T> to_insert(index_, value_);
        int cur_pos;
        file.seekg(0);
        file.read(reinterpret_cast<char *> (&cur_pos), sizeof(int));
        if (cur_pos == -1)
        {
            head = 3 * sizeof(int);
            tail = 3 * sizeof(int);
            file_end += (size1 + sizeof (BlockNode<T>));
            file.seekp(0);
            file.write(reinterpret_cast<char *> (&head), sizeof(int));
            file.write(reinterpret_cast<char *> (&tail), sizeof(int));
            file.write(reinterpret_cast<char *> (&file_end), sizeof(int));

            remainder first_rem;
            BlockNode<T> first_node;
            strcpy(first_rem.maximum, index_);
            strcpy(first_rem.minimum, index_);
            first_node.size = 1;
            first_node.array[0] = to_insert;

            file.seekp(head);
            file.write(reinterpret_cast<char *> (&first_rem), size1);
            file.write(reinterpret_cast<char *> (&first_node), sizeof (BlockNode<T>));

            return;
        }
        remainder cur_rem;
        BlockNode<T> cur_node;
        bool smaller_flag = false, bigger_flag = false;//smaller_flag is whether there occur a map_pair that is smaller than int_insert

        while(cur_pos != -1)//not out of the linkedList yet
        {
            file.seekg(cur_pos);
            file.read(reinterpret_cast<char *> (&cur_rem), size1);

            if (strcmp(index_, cur_rem.minimum) < 0)
            {
                int prev_pos = cur_rem.prev;
                if (prev_pos == -1)
                {
                    insert_to_start(index_, value_, head);
                    return;
                }
                else
                {
                    insert_to_end(index_, value_, prev_pos);//insert_to_start(index_, value_, prev_pos); update in 2022/12/18
                    return;
                }
            }
            else if (strcmp(index_, cur_rem.maximum) > 0) smaller_flag = true;
            else
            {
                file.seekg(cur_pos + size1);
                file.read(reinterpret_cast<char *> (&cur_node), sizeof (BlockNode<T>));
                for (int i = 0; i < cur_node.size; i++)
                {
                    if (to_insert > cur_node.array[i]) smaller_flag = true;
                    if (to_insert < cur_node.array[i])
                    {
                        if (smaller_flag && !bigger_flag)
                        {
//                            for (int j = i; j < cur_node.size; ++j) cur_node.array[j + 1] = cur_node.array[j];
                            for (int j = cur_node.size - 1; j >= i; --j) cur_node.array[j + 1] = cur_node.array[j];
                            cur_node.array[i] = to_insert;
                            cur_node.size++;
                            if (i == 0) strcpy(cur_rem.minimum, index_);
                            file.seekp(cur_pos);
                            file.write(reinterpret_cast<char *> (&cur_rem), size1);
                            file.write(reinterpret_cast<char *> (&cur_node), sizeof (BlockNode<T>));
                            if (cur_node.size == maxSize)
                            {
                                Split(cur_pos);
                            }
                            return;
                        }
                        bigger_flag = true;
                    }
                }
            }
            cur_pos = cur_rem.next;
        }

        if (!smaller_flag)
        {
            insert_to_start(index_, value_, head);
        }
        else if (!bigger_flag)
        {
            insert_to_end(index_, value_, tail);
        }
    }

    void del(char index_[], T value_)
    {
        //TODO: For cases where the deleted element does not exist, the time complexity can be optimized
        int cur_pos;
        file.seekg(0);
        file.read(reinterpret_cast<char *> (&cur_pos), sizeof(int));

        remainder cur_rem;
        BlockNode<T> cur_node;
        while(cur_pos != -1)//not out of the linkedList yet
        {
            file.seekg(cur_pos);
            file.read(reinterpret_cast<char *> (&cur_rem), size1);
            if (strcmp(cur_rem.minimum, index_) <= 0 && strcmp(index_, cur_rem.maximum) <= 0)
            {
                file.seekg(cur_pos + size1);
                file.read(reinterpret_cast<char *> (&cur_node), sizeof (BlockNode<T>));
                for (int i = 0; i < cur_node.size; i++)
                {
                    if (strcmp(cur_node.array[i].index, index_) == 0 && cur_node.array[i].value == value_)
                    {
                        if (cur_node.size > 1)
                        {
                            if (i == 0) strcpy(cur_rem.minimum, cur_node.array[1].index);
                            else if (i == cur_node.size - 1) strcpy(cur_rem.maximum, cur_node.array[cur_node.size - 2].index);
                        }
                        else
                        {
                            strcpy(cur_rem.minimum, "");
                            strcpy(cur_rem.maximum, "");
                        }
                        for (int j = i; j < cur_node.size - 1; ++j) cur_node.array[j] = cur_node.array[j + 1];
                        cur_node.size--;
                        file.seekp(cur_pos);
                        file.write(reinterpret_cast<char *> (&cur_rem), size1);
                        file.write(reinterpret_cast<char *> (&cur_node), sizeof (BlockNode<T>));
                        if (cur_node.size < minSize)
                        {
                            int prev_pos = cur_rem.prev, next_pos = cur_rem.next;
                            BlockNode<T> prev_node, next_node;
                            bool prev_flag = (prev_pos != -1);
                            bool next_flag = (next_pos != -1);
                            if (prev_flag)
                            {
                                file.seekg(prev_pos + size1);
                                file.read(reinterpret_cast<char *> (&prev_node), sizeof (BlockNode<T>));
                                if (prev_node.size > minSize) {borrow(1, cur_pos, prev_pos); return;}
                            }
                            if (next_flag)
                            {
                                file.seekg(next_pos + size1);
                                file.read(reinterpret_cast<char *> (&next_node), sizeof (BlockNode<T>));
                                if (next_node.size > minSize){borrow(2, cur_pos, next_pos); return;}
                            }
                            if (prev_flag && prev_node.size == minSize) {Union_to_prev(cur_pos, prev_pos); return;}
                            if (next_flag && next_node.size == minSize) {Union_to_next(cur_pos, next_pos); return;}
                        }
                        return;
                    }
                }
            }
            cur_pos = cur_rem.next;
        }
    }

    std::vector<T> FindAll()
    {
        int cur_pos;
        file.seekg(0);
        file.read(reinterpret_cast<char *> (&cur_pos), sizeof(int));

        remainder cur_rem;
        BlockNode<T> cur_node;
        std::vector<T> res;
        while(cur_pos != -1)//not out of the linkedList yet
        {
            file.seekg(cur_pos);
            file.read(reinterpret_cast<char *> (&cur_rem), size1);
            file.seekg(cur_pos + size1);
            file.read(reinterpret_cast<char *> (&cur_node), sizeof (BlockNode<T>));
            for (int i = 0; i < cur_node.size; i++)
            {
                res.push_back(cur_node.array[i].value);
            }
            cur_pos = cur_rem.next;
        }
        return res;
    }
};