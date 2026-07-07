import os

def generate_test_files(directory):
    # 创建目录
    if not os.path.exists(directory):
        os.makedirs(directory)
    
    # 生成A00文件（100行）
    with open(f"{directory}/A00.txt", "w") as f:
        f.write("common\n")  # 第1行
        f.write("stress_test_word\n")  # 第2行
        for i in range(3, 101):
            f.write(f"this is line {i-2} of A00\n")  # 第3-100行
    
    # 生成A01-A98文件（各99行）
    for file_num in range(1, 98):
        file_name = f"A{file_num:02d}"
        with open(f"{directory}/{file_name}.txt", "w") as f:
            f.write("common\n")  # 第1行
            for i in range(2, 100):
                f.write(f"this is line {i-1} of {file_name}\n")  # 第2-99行
    
    # 生成A99文件（100行）
    with open(f"{directory}/A99.txt", "w") as f:
        f.write("common\n")  # 第1行
        for i in range(2, 101):
            f.write("stress_test_word\n")  # 第2-100行

# 执行生成
if __name__ == "__main__":
    directory = "./test_pressure/"
    generate_test_files(directory)
    print(f"测试文件已生成至{directory}目录")