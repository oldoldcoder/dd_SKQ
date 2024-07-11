#include "cstdio"
#include "dd_SKQ.h"
#include "utils.h"
/**
 * @author heqi
 * @date 2024/07/11
 * @desc 负责实现内容
 * */
int main(){
    data_set  set;
    init_algo("D:\\study\\code\\ClionProject\\dd_SKQ\\data\\data.txt",&set);
    query_algo(&set,"D:\\study\\code\\ClionProject\\dd_SKQ\\data\\query.txt","D:\\study\\code\\ClionProject\\dd_SKQ\\data\\res.txt");
    free_algo(&set);
    return 0;
}