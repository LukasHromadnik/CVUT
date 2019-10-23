#!/usr/bin/python3

import sys
import math
import pycosat

WALL = "B"
SPACE = "W"
LIGHT = "L"
UNDEFINED_INDEX = -1

# input = "WWWBW0BWWWWWWWWWWW12WBWWW"
# input = "WBWWWWWW12WWWWW0BWWWWWWBW"

for line in sys.stdin:
    input = line.strip()
    break

# Get input length
input_length = len(input)

# Compute length of the row
row_length = int(math.sqrt(input_length))

def extend_input(input):

    # Split input to the equal-sized substring
    chunks = [input[i : i + row_length] for i in range(0, len(input), row_length)]

    # Create borders
    border = WALL * row_length

    # Append borders to the splitted input
    chunks = [border] + chunks + [border]

    # Append column borders to the splitted input
    for i in range(len(chunks)):
        chunks[i] = WALL + chunks[i] + WALL

    # Return the extended input
    return ''.join(chunks)

input = extend_input(input)

# Get input length
input_length = len(input)

# Compute length of the row
row_length = int(math.sqrt(input_length))

def linear_index(x, y, swap=False):
    first = x if not swap else y
    second = y if not swap else x
    return first * row_length + second

def transform_input(output):
    if output == "B":
        return "X"
    elif output == "W":
        return "."
    else:
        return output

def print_input(input):
    for i in range(row_length):
        for j in range(row_length):
            index = linear_index(i, j)
            char = input[index] 
            print(transform_input(char), end='')
        print()
    
# print_input(input)

cnf = []

for i in range(row_length):
    for j in range(row_length):
        index = linear_index(i, j)
        if input[index] != SPACE: continue
        row = []
        col = []
        for k in range(1, row_length):
            idx = linear_index(i + k, j)
            if input[idx] == SPACE:
                col.append(idx)
            else: break
        for k in range(1, row_length):
            idx = linear_index(i - k, j)
            if input[idx] == SPACE:
                col.append(idx)
            else: break
        for k in range(1, row_length):
            idx = linear_index(i, j + k)
            if input[idx] == SPACE:
                row.append(idx)
            else: break
        for k in range(1, row_length):
            idx = linear_index(i, j - k)
            if input[idx] == SPACE:
                row.append(idx)
            else: break
        
        # print([index] + row + col)
        cnf.append([index] + row + col)
        for k in range(len(row)):
            cnf.append([-row[k], -index])
            # print([-row[k], -index])
        for k in range(len(col)):
            cnf.append([-col[k], -index])
            # print([-col[k], -index])
        # print("###")







# exit()

# def search_for_spaces(swap=False):
#     spaces = []
#     for row in range(row_length):
#         i = UNDEFINED_INDEX
#         for j in range(row_length):
#             index = linear_index(row, j, swap)
#             if input[index] == SPACE and i == UNDEFINED_INDEX:
#                 i = j
#             elif input[index] != SPACE and i != UNDEFINED_INDEX:
#                 if i !=  j - 1:
#                     spaces.append((i, j - 1, row))
#                 i = UNDEFINED_INDEX

#         if i != UNDEFINED_INDEX:
#             if i != row_length - 1:
#                 spaces.append((i, row_length - 1, row))
#             i = UNDEFINED_INDEX
#     return spaces

# row_spaces = search_for_spaces()
# col_spaces = search_for_spaces(True)

# def space_to_cnf(space, swap=False):
#     cnf = []
#     indexes = []
#     space_length = space[1] - space[0] + 1
#     for i in range(space_length):
#         index = linear_index(space[2], space[0] + i, swap)
#         indexes.append(index)
    
#     return one_of_four(indexes)

def none_of_four(indexes):
    return [[-x] for x in indexes]

def one_of_four(indexes):
    cnf = [indexes]

    for i in range(len(indexes) - 1):
        for j in range(i + 1, len(indexes)):
            clause = [-indexes[i], -indexes[j]]
            cnf.append(clause)
    
    return cnf

def two_of_four(indexes):
    cnf = []
    for i in reversed(range(len(indexes))):
        copy = indexes.copy()
        del copy[i]
        cnf.append(copy)
        cnf.append([-1 * x for x in copy])
    return cnf

def three_of_four(indexes):
    cnf = [[-1 * x for x in indexes]]

    for i in range(len(indexes) - 1):
        for j in range(i + 1, len(indexes)):
            clause = [indexes[i], indexes[j]]
            cnf.append(clause)
    
    return cnf

def four_of_four(indexes):
    return [[x] for x in indexes]

# cnf = []

# for space in row_spaces:
#     result = space_to_cnf(space, swap=False)
#     cnf.extend(result)

# print(cnf)

# exit()

# for space in col_spaces:
#     result = space_to_cnf(space, swap=True)
#     cnf.extend(result)

def neighbours(i, j):
    return [(i + 1, j), (i - 1, j), (i, j + 1), (i, j - 1)]

def neighbours_as_indexes(i, j):
    n = neighbours(i, j)
    return [linear_index(x[0], x[1]) for x in n]

for i in range(row_length):
    for j in range(row_length):
        index = linear_index(i, j)
        indexes = neighbours_as_indexes(i, j)
        if input[index] == "0":
            cnf.extend(none_of_four(indexes))
        if input[index] == "1":
            cnf.extend(one_of_four(indexes))
        elif input[index] == "2":
            cnf.extend(two_of_four(indexes))
        elif input[index] == "3":
            cnf.extend(three_of_four(indexes))
        elif input[index] == "4":
            cnf.extend(four_of_four(indexes))

for i in range(input_length):
    if input[i] != SPACE:
        cnf.append([-i])
        
for i in range(len(cnf)):
    cnf[i] = [x + 1 if x > 0 else x - 1 for x in cnf[i]]

# print(cnf)

result = pycosat.solve(cnf)
if result == "UNSAT":
    print("0")
    exit()

solution = []
for i in result:
    if i < 0:
        continue
    solution.append(i)
solution = [x - 1 for x in solution]
list_input = list(input)
for i in solution:
    list_input[i] = LIGHT
# print()
string_solution = ''.join(list_input)
# print_input(string_solution)

final_solution = ""
for i in range(1, row_length - 1):
    for j in range(1, row_length - 1):
        index = linear_index(i, j)
        final_solution += string_solution[index]
        # print(string_solution[index], end='')
    # print()
print(final_solution)
