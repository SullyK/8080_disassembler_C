import re 
my_output = []
external_output = []

with open('output.txt', 'r') as file:
    for line in file:
        my_output.append(line.strip().upper())

with open('github_data.txt', 'r') as file:
    for line in file:
       external_output.append(line.strip().upper())

#ugly way but it works for nowa 
for index, line in enumerate(my_output):
    x = re.split(r'[\t,]+',external_output[index])
    if x[0].isdigit():
        continue
    for token in x:
        if str(token) in str(my_output[index]):
            str(my_output[index]).replace(str(token), '')
        else:
            print(f"On lines of your file: {index}, inequal")
            print(f"yours: {my_output[index]}, theirs {x}")
            break;
         
        
