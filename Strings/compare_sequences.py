def is_pref(pref, Str):
    if(len(pref) > len(Str)):
        return False
    if(pref != Str[0:len(pref)]):
        return False
    return True


def search(a, b):
    queue = []
    reminder_a = set()
    reminder_b = set()
    for i in range(0, len(a)):
        for j in range(0, len(b)):
            if(hash(a[i]) == hash(b[j]) and a[i] == b[j]):
                return [[i], [j]]
            if(is_pref(a[i], b[j])):
                if(b[j][len(a[i]):] not in reminder_b):
                    reminder_b.add(b[j][len(a[i]):])
                    queue.append([False, [i], [j], a[i], b[j], b[j][len(a[i]):]])
                    print(i, j)
            if(is_pref(b[j], a[i])):
                if(a[i][len(b[j]):] not in reminder_a):
                    print(i, j)
                    print(a[i][len(b[j]):])
                    reminder_a.add(a[i][len(b[j]):])
                    queue.append([True, [i], [j], a[i], b[j], a[i][len(b[j]):]])
    print("")
#aaaabbbaabbaaaaabbbabbba abbba
#aaa abb baa bbaaaa abbb aabbb aaabbb bbba

# for q in queue:
# q[0] = True if concatenated a-sequence is longer then concatenated b-sequence
# q[1] = sequence of a-indexes
# q[2] = sequence of b-indexes
# q[3] = concatenated a-sequence
# q[4] = concatenated b-sequence
# q[5] = queue[4] - queue[3] if queue[0] == True

    while(queue != []):
        L = len(queue)
        for i in range(0, L):
            cur = queue.pop(0)
            if(cur[0]):
                for j in range(0, len(b)):
                    if(is_pref(cur[5], b[j])):
                        if(cur[3] == cur[4] + b[j]):
                            return [cur[1], cur[2] + [j]]
                        rem = b[j][len(cur[5]):]
                        if(rem not in reminder_b):
                            reminder_b.add(rem)
                            print(cur[1], cur[2] + [j])
                            queue.append([False, cur[1], cur[2] + [j], cur[3], cur[4] + b[j], rem])

                    if(is_pref(b[j], cur[5])):
                        rem = cur[5][len(b[j]):]
                        if (rem not in reminder_a):
                            reminder_a.add(rem)
                            print(cur[1], cur[2] + [j])
                            queue.append([True, cur[1], cur[2] + [j], cur[3], cur[4] + b[j], rem])
            else:
                for j in range(0, len(a)):
                    if (is_pref(cur[5], a[j])):
                        if (cur[4] == cur[3] + a[j]):
                            return [cur[1] + [j], cur[2]]
                        rem = a[j][len(cur[5]):]
                        if (rem not in reminder_a):
                            reminder_a.add(rem)
                            print(cur[1] + [j], cur[2])
                            queue.append([True, cur[1] + [j], cur[2], cur[3] + a[j], cur[4], rem])

                    if (is_pref(a[j], cur[5])):
                        rem = cur[5][len(a[j]):]
                        if (rem not in reminder_b):
                            reminder_b.add(rem)
                            print(cur[1] + [j], cur[2])
                            queue.append([False, cur[1] + [j], cur[2], cur[3] + a[j], cur[4], rem])
    return [[-1], [-1]]


def main():
    print("Enter strings a_i in one line divided by space")
    a = input().split()

    print("Enter strings b_i in one line divided by space")
    b = input().split()


    answer = search(a, b)

    if(answer[0][0] == -1):
        print("There's no solution")
    else:
        print("a:", end = "   ")
        for i in range(0, len(answer[0])):
            print(answer[0][i] + 1, end=" ")
        print("\nb:", end = "   ")
        for i in range(0, len(answer[1])):
            print(answer[1][i] + 1, end=" ")
        print("")
        for i in range(0, len(answer[0])):
            print(a[answer[0][i]], end = "")
            if(i != len(answer[0]) - 1):
                print(" + ", end = "")
            else:
                print("  =  ", end = "")
        for i in range(0, len(answer[1])):
            print(b[answer[1][i]], end = "")
            if(i != len(answer[1]) - 1):
                print(" + ", end = "")

if __name__ == "__main__": main()