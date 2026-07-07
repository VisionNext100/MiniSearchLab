import random
import os

def generate_typical_queries():
    """生成历史典型查询（文档中验证过的有效查询）"""
    return [
        "common",
        "stress_test_word",
        "common stress_test_word",
        "this is",
        "line 1",
        "of A00",
        "stress_test_word common",
        "common line 1 of A00"
    ]

def generate_query_keywords():
    """生成符合文件内容的关键词库"""
    common_words = ["common", "stress_test_word", "this", "is", "line", "of"]
    number_words = [str(i) for i in range(1, 99)]  # 1-98
    file_words = [f"A{i:02d}" for i in range(0, 100)]  # A00-A99
    return common_words + number_words + file_words

def generate_random_query(keywords, max_words=10):
    """生成1-10个关键词的随机查询"""
    word_count = random.randint(1, max_words)
    return " ".join(random.sample(keywords, word_count))

def generate_input_file(directory, filename, query_count):
    """生成包含指定数量查询的输入文件"""
    keywords = generate_query_keywords()
    typical_queries = generate_typical_queries()
    
    with open(f"{directory}/{filename}", "w") as f:
        # 写入目录和查询总数
        f.write("./pressure_test/\n")
        f.write(f"{query_count}\n")
        
        # 生成查询
        for i in range(query_count):
            # 每1000条插入1条典型查询
            if i % 1000 == 0:
                query = random.choice(typical_queries)
            else:
                query = generate_random_query(keywords)
            f.write(query + "\n")
    
    print(f"已生成{query_count}条查询到{directory}/{filename}")

if __name__ == "__main__":
    directory = "./"
    filename = "search_queries_100k.txt"
    query_count = 100000
    generate_input_file(directory, filename, query_count)