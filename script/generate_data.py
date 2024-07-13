import random
import string

def generate_random_string(length):
    """生成指定长度的随机字符串"""
    return ''.join(random.choices("abc", k=length))

def generate_test_data(num_owners):
    """生成测试数据"""
    data = f"{num_owners}\n**\n"

    for owner_id in range(1, num_owners + 1):
        data += f"{owner_id}\n"
        num_files_per_owner = random.randint(1, 10)
        for file_id in range(1, num_files_per_owner + 1):
            file_id_str = str(file_id)
            random_int = random.randint(1, 10)
            num_keywords_per_file = random.randint(1, 10)
            keywords = ' '.join(generate_random_string(random_int) for _ in range(num_keywords_per_file))
            data += f"{file_id_str} {keywords}\n"
        data += "**\n"

    return data

# 设置参数
num_owners = 5  # 数据拥有者数量

# 生成测试数据
test_data = generate_test_data(num_owners)

# 将生成的数据写入文件
with open("test_data.txt", "w", encoding="utf-8") as file:
    file.write(test_data)

print("数据已写入 test_data.txt 文件")