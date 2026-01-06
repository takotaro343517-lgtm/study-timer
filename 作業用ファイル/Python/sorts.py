import random
import matplotlib as mlp
import copy

def bubble_sort(list):
    for i in range(len(list) - 1):
        for j in range(len(list) - i - 1):
            if list[j] > list[j+1]:
                list[j], list[j+1] = list[j+1], list[j]
    return list

def abe_sort(list):
    for i in range(len(list) - 1):    
        if list[i] > list[i+1]:
            list[i+1] = list[i]
    return list

def stalin_sort(list):
    for i in range(len(list) - 1):
        max = list[0]
        if max > list[i]:
            list[i] = 0
        elif max <= list[i]:
            max = list[i]
    return list
        

lists = [random.randint(0,100) for i in range(10)]

print(lists)

copy_list = copy.copy(lists)
copy2_list = copy.copy(lists)
bubble_sort(lists)
abe_sort(copy_list)
stalin_sort(copy2_list)
print(lists)
print(copy_list)
print(copy2_list)