from termcolor import colored

def same(str1, str2):
    if hash(str1) == hash(str2) and len(str1) == len(str2) and str1 == str2:
        return True
    return False

#################FIND A GENERAL SUBSTRATE

#####DYNAMIC#####
def com_subseq(ver1, ver2):
    L = [[0 for j in range(0, len(ver2) + 1)] for i in range(0, len(ver1) + 1)]
    for i in range(1, len(ver1) + 1):
        for j in range(1, len(ver2) + 1):
            if(same(ver1[i - 1], ver2[j - 1])):
                L[i][j] = L[i-1][j-1] + 1
            else:
                L[i][j] = max(L[i][j-1], L[i-1][j])
    i = len(ver1)
    j = len(ver2)
    sub = []
    while i != 0 and j != 0:
        if(same(ver1[i - 1], ver2[j - 1])):
            sub += [[ver1[i-1], i - 1, j - 1]]
            i -= 1; j -= 1
        elif(L[i][j - 1] > L[i - 1][j]):
            j -= 1
        else:
            i -= 1
    return sub[::-1]

#####IMPROOVED SEARCH#####
def Myers(ver1, ver2):
    print(ver1)
    print(ver2)
    parent = {}
    queue = [(0, 0)]
    sub = []
    while(True):
        cur = queue.pop(0)
        if(cur[0] >= len(ver1) or cur[1] >= len(ver2)):
            continue
        if cur == (len(ver1) - 1, len(ver2) - 1):
            i = len(ver1) - 1
            j = len(ver2) - 1
            while(i != 0 or j != 0):
                print(i, j)
                if same(ver1[i], ver2[j]):
                    sub += [[ver1[i], i, j]]
                [i, j] = parent[tuple([i, j])]
            break

        if same(ver1[cur[0]], ver2[cur[1]]) and (cur[0] + 1, cur[1] + 1) not in parent.keys():
            queue.insert(0, (cur[0] + 1, cur[1] + 1))
            parent[(cur[0] + 1, cur[1] + 1)] = (cur[0], cur[1])
        else:
            if((cur[0] + 1, cur[1]) not in parent.keys()):
                queue.append((cur[0] + 1, cur[1]))
                parent[(cur[0] + 1, cur[1])] = (cur[0], cur[1])
            if ((cur[0], cur[1] + 1) not in parent.keys()):
                queue.append((cur[0], cur[1] + 1))
                parent[(cur[0], cur[1] + 1)] = (cur[0], cur[1])
    return sub[::-1]
##################################


def diff(ver1, ver2):
    #sub = com_subseq(ver1, ver2)
    sub = Myers(ver1, ver2)
    iter1 = 0; iter2 = 0
    print("\n")
    for i in sub:
        while(iter1 < i[1]):
            print(colored("-"+ver1[iter1].replace("\n", " "), "red"))
            iter1 += 1
        iter1 += 1
        while(iter2 < i[2]):
            print(colored("+"+ver2[iter2].replace("\n", " "), "green"))
            iter2 += 1
        iter2 += 1
        print(i[0].replace("\n", " "))

    while(iter1 < len(ver1)):
        print(colored("-" + ver1[iter1].replace("\n", " "), "red"))
        iter1 += 1
    while (iter2 < len(ver2)):
        print(colored("+" + ver2[iter2].replace("\n", " "), "green"))
        iter2 += 1


####### INPUT #######
def cons_input(ver1, ver2):
    print("Enter the first file, line by line. In the end enter 0")
    while (True):
        ver1 += [input()]
        if (ver1[len(ver1) - 1] == "0"):
            ver1.pop(len(ver1) - 1)
            break
    print("Enter the first file, line by line. In the end enter 0")
    while (True):
        ver2 += [input()]
        if (ver2[len(ver2) - 1] == "0"):
            ver2.pop(len(ver2) - 1)
            break

def file_input(ver1, ver2, file1, file2):
    with open(file1, "r", encoding = "utf-8") as f1:
        while (True):
            tmp = f1.readline()
            if (tmp == ""):
                break
            ver1 += [tmp]
        f1.close()
    with open(file2, "r", encoding = "utf-8") as f2:
        while (True):
            tmp = f2.readline()
            if (tmp == ""):
                break
            ver2 += [tmp]
        f2.close()
#####################

def main():
    ver1 = []
    ver2 = []
    file1 = "Enter your file1"
    file2 = "Enter your file2"
    #cons_input(ver1, ver2)
    file_input(ver1, ver2, file1, file2)
    diff(ver1, ver2)

if __name__ == "__main__": main()