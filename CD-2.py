class Conversion:
    def __init__(self,capacity):
        self.top=-1
        self.capacity=capacity
        self.array=[]
        self.output=[]
        self.precedence={'+':1, '-':1, '*':2, '/':2, '^':3}


    #To check Whether the stack is empty or not
    def isEmpty(self):
        return True if self.top==-1 else False

    #to return the value of top of stack
    def peek(self):
        return self.array[-1]

    #to pop the element
    def pop(self):
        if not self.isEmpty():
            self.top -= 1
            return self.array.pop()
        else:
            return "$"

    #to push the element
    def push(self,op):
        self.top += 1
        self.array.append(op)

    #to check the element is operand
    def isOperand(self,ch):
        return ch.isalpha()

    #to check precedence
    def notGreater(self,i):
        try:
            a = self.precedence[i]
            b = self.precedence[self.peek()]
            return True if a  <= b else False
        except KeyError:
            return False


    #To convert Infix to postfix

    def infixtopostfix(self,exp):
        for i in exp:
            if self.isOperand(i):
                self.output.append(i)
            elif i == "(":
                self.push(i)

            elif  i == ")":
                while( (not self.isEmpty()) and self.peek() != '('):
                    a = self.pop()
                    self.output.append(a)
                if (not self.isEmpty() and self.peek() != '('):
                    return -1
                else:
                    self.pop()
            else:
                while(not self.isEmpty() and self.notGreater(i)):
                    self.output.append(self.pop())
                self.push(i)

        # pop all the operator from the stack
        while not self.isEmpty():
            self.output.append(self.pop())

        print("".join(self.output))

exp = "a+b*(c^d-e)^(f+g*h)-i"
obj = Conversion(len(exp))
obj.infixtopostfix(exp)
