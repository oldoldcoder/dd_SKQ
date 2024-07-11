#include "dd_SKQ.h"
#include "fstream"
#include "sstream"
#include "iostream"
#include "string"
#include "algorithm"
using namespace std;
// 具体方法的实现
/**
 * 文件格式如下：
 * n(数据拥有者的数量)
 * id0 关键字x1 关键字x2 关键字x3
 * id1 关键字x1 关键字x2 关键字x3
 * */
/**-------------------JNA接口实现---------------------*/
EXPORT_SYMBOL int init_algo(char* dataFilePath, data_set * data){

	if (read_data(data, dataFilePath) != SUCCESS) {
		return ERROR;
	}
	if (create_backward_index(data) != SUCCESS) {
		return ERROR;
	}

	return SUCCESS;
}
EXPORT_SYMBOL int query_algo(data_set * data, char* queryFilePath, char* resultFilePath){
    query * q = read_query(queryFilePath);
    auto inter_section = notify_dataOwner_2query(data,q);
    // 打印结果
    if(write_intersection_2file(inter_section,q,resultFilePath)!= SUCCESS) {
        printf("error:can't write content to resultFile");
        return ERROR;
    }
    query_free(q);
    return SUCCESS;
}
EXPORT_SYMBOL int free_algo(data_set * data){
    data_set_free(data);
    return SUCCESS;
}

RESULT read_data(data_set * set,char * filePath){
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filePath << endl;
        return ERROR;
    }

    string line;

    // 读取数据拥有者的数量
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> set->n;
    } else {
        std::cerr << "Error: Could not read the number of data owners." << std::endl;
        return ERROR;
    }
    int idx  = 0;
    // 创建内容
    set->ownerList = (data_owner **)malloc(sizeof (data_owner *) * set->n);
    // 读取每个数据拥有者的id和关键字
    while (getline(file, line)) {
        if (line == "**") { // 遇到分割符
            set->ownerList[idx] = (data_owner *)malloc(sizeof (data_owner));
            getline(file, line); // 读取id行
            set->ownerList[idx]->id = stoi(line);
            // 是否完成反向索引表的转换
            set->ownerList[idx]->is_back = false;
            set->ownerList[idx]->hashmap_forward = new unordered_map<int, void*>();
            set->ownerList[idx]->hashmap_backward = new unordered_map<string, void*>();
            // 初始化vector
            set->ownerList[idx]->vec = new vector<int>();
            // 读不到**的时候就完成创建的内容
            while (getline(file, line) && line != "**") {
                istringstream iss(line);
                string file_id;
                string keyword;
                iss >> file_id; // 读取fileid
                vector<string> *keywords = new vector<string>;
                // 读取关键字
                while (iss >> keyword) {
                    keywords->push_back(keyword);
                }

                set->ownerList[idx]->hashmap_forward->insert(make_pair(stoi(file_id),keywords));
            }
            idx++;
        }
        if (idx != set->n) {
            cerr << "Error: The number of data owners read does not match the specified number." << endl;
            return ERROR;
        }
    }
    // 初始化完毕
    return SUCCESS;
}
//构建反向索引表
RESULT create_backward_index(data_set * set){
    // 使用遍历来创建反向索引表
    int n = set->n;
    for(int i = 0 ; i < n ; ++i){
        data_owner *tmp = set->ownerList[i];
        for (auto it = tmp->hashmap_forward->begin(); it != tmp->hashmap_forward->end(); ++it) {

            vector<string>* data = (vector<string> *)it->second;
            // 拿到我们的file_id
            int file_id = it->first;
            // 遍历关键字
            int len = data->size();
            for (int j = 0; j < len; ++j) {
                string c = data->at(j);
                auto itt = tmp->hashmap_backward->find(c);
                if (itt == tmp->hashmap_backward->end()) {
                    // 创建vector放入到反向索引表格中去
                    vector<int> * fileIds = new vector<int>();
                    fileIds->push_back(file_id);
                    tmp->hashmap_backward->insert(std::make_pair(c, reinterpret_cast<void*>(fileIds)));
                }
                else {
                    vector<int>* fileIds = (vector<int>*)it->second;
                    fileIds->push_back(file_id);
                }
            }
        }
        tmp->is_back = TRUE;
    }
    return SUCCESS;
}
// 释放结构的内容
RESULT data_set_free(data_set * set){
    // 反向结构--这会好累，呼吸也好累，明明吃了饭了，为什么这会这么累，手指头打的邦疼
    int n = set->n;
    for(int i = 0; i < n ; ++i){
        delete(set->ownerList[i]->hashmap_backward);
        delete(set->ownerList[i]->hashmap_forward);
        delete(set->ownerList[i]->vec);
        free(set->ownerList[i]);
    }
    free(set->ownerList);
    return SUCCESS;
}

std::string trim(const std::string& str) {
    const std::string whitespace = " \t\n\r";
    const auto start = str.find_first_not_of(whitespace);
    if (start == std::string::npos) {
        return ""; // 如果没有非空白字符，返回空字符串
    }
    const auto end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

// 读取同时初始化查询的内容
query* read_query(char * queryFilePath){
    // 通过文件路径初始化查询的内容
    std::ifstream infile(queryFilePath);
    if (!infile) {
        std::cerr << "Unable to open file: " << queryFilePath << std::endl;
        return nullptr;
    }

    query* q = (query *)malloc(sizeof (query)) ;
    string li;

    // 读取数据拥有者的数量
    if (std::getline(infile, li)) {
        std::istringstream iss(li);
        iss >> q->n;
    } else {
        std::cerr << "Error: Could not read the number of data owners." << std::endl;
        return ERROR;
    }

    int n = q->n;
    q->units = (query_unit **) malloc(sizeof (query_unit *) * n);

    for (int i = 0; i < q->n; ++i) {
        std::string line;
        if (std::getline(infile, line)) {
            std::istringstream iss(line);
            iss >> q->units[i]->target_id;
            std::getline(iss, q->units[i]->keyword);
            trim(q->units[i]->keyword);
        }
    }

    return q;
}

void intersectVectors(std::vector<int>& vector1, std::vector<int>& vector2) {
    // 临时向量存储交集
    std::vector<int> tempVector;

    // 需要对向量进行排序才能使用 std::set_intersection
    sort(vector1.begin(), vector1.end());
    sort(vector2.begin(), vector2.end());

    // 求交集
    std::set_intersection(
            vector1.begin(), vector1.end(),
            vector2.begin(), vector2.end(),
            std::back_inserter(tempVector)
    );

    // 将交集结果赋值回 vector1
    vector1 = tempVector;
}

// 进行查询
vector<int> *  notify_dataOwner_2query(data_set * set,query * q){
    // 通知进行查询
    int sn = set->n;
    int qn = q->n;
    // 使用 lambda 表达式对 ownerList 进行排序
    std::sort(set->ownerList, set->ownerList + set->n, [](const data_owner* a, const data_owner* b) {
        return a->id < b->id;
    });
    std::sort(q->units,q->units + q->n, [](const query_unit* a, const query_unit* b) {
        return a->target_id < b->target_id;
    });
    // 两个组的id都是顺序排列
    int idxI = 0;
    int idxJ = 0;
    vector<int> * inter_section = nullptr;
    while(idxI < sn && idxJ < qn){
        if(set->ownerList[idxI]->id < q->units[idxJ]->target_id){
            idxI++;
        }else if(set->ownerList[idxI]->id == q->units[idxJ]->target_id){
            // 查询，然后下一个
            auto it= set->ownerList[idxI]->hashmap_backward->find(q->units[idxJ]->keyword);
            if(it != set->ownerList[idxI]->hashmap_backward->end()){
                if(inter_section == nullptr){
                    inter_section = new vector<int>;
                    // 深copy
                    inter_section = (vector<int> *)it->second;
                }else{
                    intersectVectors(*inter_section, *(vector<int> *)it->second);
                }
            }
        }else{
            idxJ++;
        }
    }
    return inter_section;
}
// 求交集同时写入resFile中
RESULT write_intersection_2file(vector<int> * vec,query * q,char * resFile){
    FILE *file = fopen(resFile, "w");
    if (file == nullptr) {
        perror("Error opening file");
        return ERROR;
    }
    // 写入关键字
    int qn = q->n;
    fprintf(file, "keywordList: ");
    for(int i = 0 ; i < qn ; ++i){
        fprintf(file, "%s ",q->units[i]->keyword.c_str());
    }
    // 写入查询的交集的内容
    int sn = vec->size();

    fprintf(file, "\nList of included files: ");
    for(int i = 0 ; i < sn ; ++i){
        fprintf(file, "%d ",(*vec)[i]);
        if((i + 1) % 5 == 0){
            fprintf(file, "\n");
        }
    }
    fprintf(file,"\n");
    fclose(file);
    return SUCCESS;
}
// 释放查询的内容
RESULT query_free(query * q){
    // 貌似不用手动释放string，那这样就完了？
    free(q->units);
}