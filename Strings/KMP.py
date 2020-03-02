def pref(Str):
    Pref  = [0]*len(Str)
    for i in range(1, len(Str)):
        j = i
        while(j > 0):
            j = Pref[j - 1]
            if(Str[j] == Str[i]):
                Pref[i] = j + 1
                break
        if(Str[i] != Str[j]):
            Pref[i] = 0
    return Pref

def KMP(b, a):
    Str = b + "#" + a
    Pref = pref(Str)
    all_intersects = []            #индексы начал вхождений b в строке a
    first = len(b) + 1
    for i in range(first, first + len(a)):
        if(Pref[i] == len(b)):
            all_intersects += [i - 2*len(b)]
    return all_intersects


def main():
    a = "caacabaca"
    b = "ca"
    print(KMP(b, a))

if __name__ == "__main__": main()