print("The String is : a=b+c*23-56^2")
a="b+c*23-56^2"
counta=0
countn=0
countc=0
counto=0
for i in a:
    if(i.isalpha()):
        counta+=1

    if(i=="=" or i=="*" or i=="-" or i=="^" or i=="/" or i=="+"):
        counto+=1


for i in range(0,len(a)-1):

    if(a[i].isnumeric() and a[i+1].isnumeric()):
        countn+=1

    elif(a[i].isdigit()):
        countn+=1



print('The no. of alphabets is',counta)
print('The no. of operator is', counto)
print('The no. of number is', countn)
