#include "dd_SKQ.h"

using namespace std;

// 具体方法的实现
RESULT init_data_set(data_set * set){

}
RESULT read_data(data_set * set,char * filePath){

}
//构建反向索引表
RESULT create_backward_index(data_set * set){

}
// 释放结构的内容
RESULT data_set_free(data_set * set){

}
// 读取同时初始化查询的内容
query* read_query(char * queryFilePath){

}
// 进行查询
RESULT notify_dataOwner_2query(data_set * set,query * q){

}
// 求交集同时写入resFile中
RESULT write_intersection_2file(data_set * set,char * resFile){

}
// 释放查询的内容
RESULT query_free(query * q){

}