#include "cstdio"
#include "dd_SKQ.h"
#include "utils.h"
/**
 * @author heqi
 * @date 2024/07/11
 * @desc 负责实现内容
 * */

void printfTime(char* desc,clock_t start) {
    double cpu_time_used = ((double)(clock() - start)) / CLOCKS_PER_SEC;

    printf("%s used: %f seconds\n",desc, cpu_time_used);
}

int main(){
    data_set  set;
    clock_t start, end;
    start = clock();
    init_algo("/root/heqi/encryption_algorithm/dd_skq/data/data.txt",&set);
    printfTime("init",start);
    query_algo(&set,"/root/heqi/encryption_algorithm/dd_skq/data/query.txt","/root/heqi/encryption_algorithm/dd_skq/data/res.txt");
    start = clock();
    printfTime("init",start);
    free_algo(&set);
    return 0;
}