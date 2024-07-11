#ifndef DD_SKQ_H
#define DD_SKQ_H
/**
 * @author heqi
 * @date 2024/07/10
 * @desc 书写有关分布式数据安全协同的关键字查询的内容，主要和之前的drq差不多的求
 * */

#include "cstdio"
#include "iostream"
#include "vector"
#include "unordered_map"
#include "cstring"
#include "utils.h"
#include "string"
#ifdef _WIN32
#define EXPORT_SYMBOL __declspec(dllexport)
#else
#define EXPORT_SYMBOL __attribute__((visibility("default")))
#endif

using namespace std;
/*----------------------结构体定义--------------------*/
typedef struct data_owner {
    // 当前数据拥有者的编号
    int id;
    // 反向索引是否建立完毕
    bool is_back;
    // 正向索引,key是int类型，不需要使用比较函数
    unordered_map<int, void* >* hashmap_forward;
    // 反向索引
    unordered_map<string, void*>* hashmap_backward;
    // 携带一个vector来存储查找到的id，然后方便求交集
    vector<int> *vec;
}data_owner;
typedef struct data_set{
    // 代表data_owner的数量
    int n;
    // 全体data_owner
    data_owner ** ownerList;
}data_set;

// 查询的结构体
typedef struct query_unit{
    // 目标id
    int target_id;
    // 目标的
    string keyword;
}query_unit;

typedef struct query{
    // 查询所占的标签数
    int n;
    // 具体内容
    query_unit ** units;
}query;

/*----------------------JNA定义--------------------*/
extern "C"{
    EXPORT_SYMBOL int init_algo(char* dataFilePath, data_set * data);
    EXPORT_SYMBOL int query_algo(data_set* data, char* queryFilePath, char* resultFilePath);
    EXPORT_SYMBOL int free_algo(data_set* data);
}

/*----------------------方法区域定义--------------------*/
// 初始化数据拥有者的内容
RESULT read_data(data_set * set,char * filePath);
//构建反向索引表
RESULT create_backward_index(data_set * set);
// 释放结构的内容
RESULT data_set_free(data_set * set);
// 读取同时初始化查询的内容
query* read_query(char * queryFilePath);
// 进行查询
vector<int> *  notify_dataOwner_2query(data_set * set,query * q);
// 求交集同时写入resFile中
RESULT write_intersection_2file(vector<int> * vec,query * q,char * resFile);
// 释放查询的内容
RESULT query_free(query * q);

#endif

