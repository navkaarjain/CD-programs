a = input("Enter Your string:")
flag= 0
for i in a:
    if flag == 1:
        print(i)
        flag =0
    if i == '>' or i =='|':
        flag = 1