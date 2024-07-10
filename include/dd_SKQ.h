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

#ifdef _WIN32
#define EXPORT_SYMBOL __declspec(dllexport)
#else
#define EXPORT_SYMBOL __attribute__((visibility("default")))
#endif

using namespace std;
/*----------------------结构体定义--------------------*/
struct char_ptr_hash {
    size_t operator()(const char* s) const {
        return hash<string>()(string(s));
    }
};
// 自定义比较函数
struct char_ptr_equal {
    bool operator()(const char *s1, const char *s2) const {
        return strcmp(s1, s2) == 0;
    }
};
typedef struct data_owner {
    // 当前数据拥有者的编号
    int id;
    // 反向索引是否建立完毕
    int is_back;
    // 正向索引,key是int类型，不需要使用比较函数
    unordered_map<int, void* >* hashmap_forward;
    // 反向索引
    unordered_map<char*, void*, char_ptr_hash, char_ptr_equal>* hashmap_backward;
    // 携带一个vector来存储查找到的id，然后方便求交集
    vector<int> vec;
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
    char * keyword;
}query_unit;

typedef struct query{
    // 查询所占的标签数
    int n;
    // 具体内容
    query_unit ** units;
}query;

/*----------------------JNA定义--------------------*/
extern "C"{

}

/*----------------------方法区域定义--------------------*/
// 初始化数据拥有者的内容
RESULT init_data_set(data_set * set);
RESULT read_data(data_set * set,char * filePath);
//构建反向索引表
RESULT create_backward_index(data_set * set);
// 释放结构的内容
RESULT data_set_free(data_set * set);
// 读取同时初始化查询的内容
query* read_query(char * queryFilePath);
// 进行查询
RESULT notify_dataOwner_2query(data_set * set,query * q);
// 求交集同时写入resFile中
RESULT write_intersection_2file(data_set * set,char * resFile);
// 释放查询的内容
RESULT query_free(query * q);

#endif
