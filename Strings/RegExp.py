class automate:

    def __init__(self, arr):
        self.ways = [[[] for j in range(0, 128)] for i in range(0, 2)]
        for i in arr:
            self.ways[0][i] += [1]

    def uni(self, other):
        l1 = len(self.ways); l2 = len(other.ways)
        self.ways.insert(0, [[] for i in range(0, 128)])
        self.ways += other.ways
        self.ways += [[[] for i in range(0, 128)]]
#refilling
        self.ways[0][0] += [1, l1 + 1]
        for i in range(1, l1):
            self.ways[i] = [[x + 1 for x in self.ways[i][j]] for j in range(0, 128)]
        self.ways[l1][0] += [l1 + l2 + 1]
        for i in range(l1 + 1, l1 + l2):
            self.ways[i] = [[x + l1 + 1 for x in self.ways[i][j]] for j in range(0, 128)]
        self.ways[l1 + l2][0] += [l1 + l2 + 1]
        return self

    def star(self):
        l1 = len(self.ways)
        self.ways.insert(0, [[] for i in range(0, 128)])
        self.ways += [[[] for i in range(0, 128)]]
        self.ways[0][0] += [1, l1 + 1]
        for i in range(1, l1):
            self.ways[i] = [[x + 1 for x in self.ways[i][j]] for j in range(0, 128)]
        self.ways[l1][0] += [1, l1 + 1]
        return self

    def conc(self, other):
        l1 = len(self.ways); l2 = len(other.ways)
        self.ways.pop()
        self.ways += other.ways
        for i in range(l1 - 1, l1 + l2 - 2):
            self.ways[i] = [[x + l1 - 1 for x in self.ways[i][j]] for j in range(0, 128)]
        return self

    def dfs(self, vert, exp):
        flag = True
        if(len(exp) == 0):
            flag = False
        if vert == len(self.ways) - 1 and len(exp) == 0:
            return True
        for i in self.ways[vert][0]:
            if self.dfs(i, exp):
                return True
        if flag:
            for i in self.ways[vert][ord(exp[0])]:
                if self.dfs(i, exp[1:]):
                    return True

    def check(self, exp):
        return self.dfs(0, exp)



def uni(l, r):
    return l.uni(r)

def star(l):
    return l.star()

def conc(l, r):
    return l.conc(r)

def parce(text):
    arr = set()
#parcing "-"
    while(True):
        if(len(text) == 0):
            break
        interval = text.find("-")
        if(interval == -1):
            break
        if(interval == len(text) - 1):
            arr.add(ord(text[len(text) - 1]))
            text = text[:len(text) - 1]
            break
        l = ord(text[interval - 1]); r = ord(text[interval + 1])
        arr |= set([i for i in range(min(l, r), max(l, r) + 1)])
        text = text[:interval-1] + text[interval + 2:]

    for i in range(0, len(text)):
        arr.add(ord(text[i]))
    return list(arr)


#BUILDS THE AUTOMATE
def build(exp):
    if(len(exp) == 0):
        return automate([])
    if(exp[0] == '('):
        difference = 1
        sqdifference = 0
        for j in range(1, len(exp)):
            difference += int(exp[j] == "(")
            difference -= int(exp[j] == ")")
            if(difference == 0):
                if(j == len(exp) - 1):
                    return build(exp[1:j])
                if(exp[j + 1] == "|"):
                    return uni(build(exp[1:j]), build(exp[j+2:]))
                if(exp[j + 1] == "*"):
                    if(j == len(exp) - 2):
                        return star(build(exp[1:j]))
                    if(exp[j + 2] == "|"):
                        return uni(star(build(exp[1:j])), build(exp[j+3:]))
                    return conc(star(build(exp[1:j])), build(exp[j+2:]))
                return conc(build(exp[1:j]), build(exp[j+1:]))

    if(exp[0] == "["):
        r = exp.find("]")
        arr = parce(exp[1:r])
        if (r == len(exp) - 1):
            return automate(arr)
        if (exp[r + 1] == "|"):
            return uni(automate(arr), build(exp[r + 2:]))
        if (exp[r + 1] == "*"):
            if (r == len(exp) - 2):
                return star(automate(arr))
            if (exp[r + 2] == "|"):
                return uni(star(automate(arr)), build(exp[r + 3:]))
            return conc(star(automate(arr)), build(exp[r + 2:]))
        return conc(automate(arr), build(exp[r + 1:]))

    if(len(exp) == 1):
        return automate([ord(exp[0])])
    if(exp[1] == "|"):
        return uni(automate([ord(exp[0])]), build(exp[2:]))
    if(exp[1] == "*"):
        if(len(exp) == 2):
            return star(automate([ord(exp[0])]))
        if(exp[2] == "|"):
            return uni(star(automate([ord(exp[0])])), build(exp[3:]))
        return conc(star(automate([ord(exp[0])])), build(exp[2:]))
    return conc(automate([ord(exp[0])]), build(exp[1:]))


def test(RegExp, expression):
    print(RegExp)
    print(expression)
    NSM = build(RegExp)
    b = NSM.check(expression)
    if (b):
        print("it's OK")
    else:
        print("it's NOT OK")
    print("\n", end="")



def main():
    test("[a-zA-Z0-9][a-zA-Z0-9_.+-]*@[a-zA-Z0-9-][a-zA-Z0-9-]*.[a-z]*",
         "fomius2000@yandex.ru")
    test("()", "")
    test("()*", "")
    test("([a-z][0-9])*", "er634wr9u82u3")
    test("([a-z0-9])*", "er634wr9u82u3")
    test("([(])", "(()")

    print("Enter your pattern")
    RegExp = input()
    print("Enter the string, you want to check")
    expression = input()
    test(RegExp, expression)


if __name__ == "__main__": main()