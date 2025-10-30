
with open("output_2025-10-30", "r") as file:
    data = file.readlines()
new_data = []

for i in range(0,len(data),4):
    new_data.append({data[i][:len(data[i])-2]:(data[i+1][:len(data[i+1])-1]+data[i+3][:len(data[i+3])-1]).replace(",","")})

print(*new_data)
for time, value in new_data.items():
    print(f"time: {time}, value: {value}")