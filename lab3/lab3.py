powers_of_two = [str(2**i) for i in range(64, -1, -1)]

with open('VPD_Laba3.txt', 'r', encoding='utf-8') as file:
    data = file.read().strip()

for i in powers_of_two:
    data = data.replace(i, "")

# Сохраняем результат
with open('VPD_Laba3_final_out.txt', 'w', encoding='utf-8') as file:
    file.write(data)

