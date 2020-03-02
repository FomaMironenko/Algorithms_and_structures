def Z(str):
    zet = [0]
    l = 0; r = 0
    for i in range(1, len(str)):
        zet += [0]
        if(i > r):
            while(i + zet[i] < len(str) and str[i + zet[i]] == str[zet[i]]):
                zet[i] += 1
            if(zet[i] != 0):
                l = i; r = i + zet[i] - 1
        else:
            zet[i] = min(zet[i - l], r - i + 1)
            while(i + zet[i] < len(str) and str[i + zet[i]] == str[zet[i]]):
                zet[i] += 1
            if(i + zet[i] - 1 > r):
                l = i; r = i + zet[i] - 1
    return zet


def check(b, a):
# a - текст, b - пользовательская строка
    str1 = b + "#" + a
    str2 = b[::-1] + "#" +a[::-1]
    length = len(b) + 1

    pref = Z(str1)
    suf  = Z(str2) #длина максимального совпадающего суффикса
    pref = pref[length:]; suf = suf[length:][::-1]
    length -= 1
    entry = []

    for i in range(len(a) - length + 2):
# правильное написание
        if(i < len(a) and pref[i] == length):
            entry += [i]
            print("Correct: " + str(i))
# пропущеная буква:
        elif(i + length - 2 < len(a) and pref[i] + suf[i + length - 2] >= length - 1):
            entry += [i]
            print("Missed:  " + str(i))
# неправильная буква:
        elif(i + length - 1 < len(a) and pref[i] + suf[i + length - 1] == length - 1):
            entry += [i]
            print("Wrong:   " + str(i))
# лишняя буква:
        elif(i + length < len(a) and pref[i] + suf[i + length] == length):
            entry += [i]
            print("Excess:  " + str(i))
# лишняя удвоенная буква
        elif(i + length < len(a) and pref[i] + suf[i + length] >= length + 1):
            entry += [i]
            print("Doubled: " + str(i))

    return entry



def main():
    print('"mistake" : ', '"I don\'t like making mistakes and there is no misstake in this text"')
    check("mistake", "I don't like making mistakes and there is no misstake in this text")

    print('\n"bb" : ', '"aaabb"')
    check("bb", "aaabb")

    print('\n"bb" : ', '"baaab"')
    check("bb", "baaab")

    print('\n"bbbbb" : ', '"aaabbbbaaaa"')
    check("bbbbb", "aaabbbbaaaa")

if __name__ == "__main__": main()