import os
import random


def random_zero_one(n):
    lst = []
    for i in range(n):
        if random.randint(1, 100) > 50:
            lst.append('0')
        else:
            lst.append('1')
    return ''.join(lst)

def test():
    failed = 0
    all = 100
    for i in range(all):
        a = random.randint(1, 100)
        if a < 50:
            output = random_zero_one(random.randint(1, 20))
            output = output + ''.join((reversed(output)))
            res = os.popen(f'../turing ./palindrome_detector_2tapes.tm {output}')
            res = res.readline().strip()
            if res != "true":
                print(f'failed {output}')
                failed += 1
        else:
            output = random_zero_one(random.randint(1, 20))
            res = os.popen(f'../turing ./palindrome_detector_2tapes.tm {output}')
            res = res.readline().strip()
            is_palim = output == ''.join(reversed(output))
            is_palim_out = res == "true"
            if is_palim_out != is_palim and (res != "true" and res != "false"):
                print(f'failed {output}')
                failed += 1
    print(f'total {all}, failed: {failed}')

test()
