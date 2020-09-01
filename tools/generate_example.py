#!/usr/bin/env python3

import sys
import random
import string


def rand_str(max_let_len=6, max_int=1000000):
    let_len = random.randint(1, max_let_len)
    letters = ''.join(random.choice(string.ascii_uppercase) for _ in range(let_len))
    numbers = str(random.randint(1, max_int))
    return letters + numbers


def gen_rand_name(cells):
    name = rand_str()
    while name in cells:
        name = rand_str()
    return name


if __name__ == "__main__":
    cells_count = int(sys.argv[1]) if len(sys.argv) > 1 else 1000
    value_cells_ratio = (int(sys.argv[2]) if len(sys.argv) > 2 else 10) / 100
    max_branches = int(sys.argv[3]) if len(sys.argv) > 3 else 10
    cells = dict()
    for i in range(cells_count):
        name = gen_rand_name(cells)
        is_value = (random.random() < value_cells_ratio) or len(cells) < 3
        if is_value:
            cells[name] = [str(random.randint(0, 25))]
        else:
            cnt = random.randint(1, max_branches)
            to_choose_from = list(cells.keys())
            cells[name] = list(set([random.choice(to_choose_from) for _ in range(cnt)]))
    for name in sorted(cells.keys()):
        print(f"{name} = " + "+".join(cells[name]))
