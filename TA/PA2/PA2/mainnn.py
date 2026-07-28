#############################
# Name: Gabe Teichroew, Ava Garrison, Ty Mon Harvey
# Course: CSC 2400,
# Assignment: PA 2,
# Description: This program reads user data from a text file and sorts the user by number of posts and number of friends. Then it write the users info into a different text file sorted.
# The program also times how long the sorting takes and writes that to a text file.
##############################

import time

def data(fp):
    # sets an empty hashmap to store the user data in
    user_data = {}
    with open(fp, 'r') as f:
        for line in f:
            line = line.replace('[', '').replace(']', '')
            # breaks the line into parts using the space to split them.
            part = line.split()

            # checking to see if all the data is there
            if len(part) >= 2:
                # the main user
                user = part[0]
                # the number of posts the user has made
                posts = int(part[1])
                # the friends of the user
                friends = part[2:]

                # sets the user in the hashmap with all its attributes
                user_data[user] = {'posts': posts, 'friends': friends}

    return user_data

# ---------- MERGE SORT ----------
def merge_sort(li):
    if len(li) <= 1:
        return li
    else:
        mid = len(li) // 2
        left = merge_sort(li[:mid])
        right = merge_sort(li[mid:])

        return merge(left, right)

def merge(left, right):
    arr = []
    i=j=0

    while i < len(left) and j < len(right):
        if left[i][1] >= right[j][1]:
            arr.append(left[i])
            i += 1
        else:
            arr.append(right[j])
            j += 1

    arr.extend(left[i:])
    arr.extend(right[j:])

    return arr

# ---------------- LOMUTO QUICK SORT ----------------
def quick_sort_lomuto(li):
    quick_sort_lomuto_helper(li, 0, len(li) - 1)
    return li

def quick_sort_lomuto_helper(li, low, high):
    if low < high:
        p = lomuto_partition(li, low, high)
        quick_sort_lomuto_helper(li, low, p - 1)
        quick_sort_lomuto_helper(li, p + 1, high)

def lomuto_partition(li, low, high):
    pivot = li[high][1]   # pivot = post count
    i = low - 1

    for j in range(low, high):
        # descending order
        if li[j][1] >= pivot:
            i += 1
            li[i], li[j] = li[j], li[i]

    li[i + 1], li[high] = li[high], li[i + 1]
    return i + 1

# ---------------- HOARE QUICK SORT ----------------
def quick_sort_hoare(li):
    quick_sort_hoare_helper(li, 0, len(li) - 1)
    return li

def quick_sort_hoare_helper(li, low, high):
    if low < high:
        p = hoare_partition(li, low, high)
        quick_sort_hoare_helper(li, low, p)
        quick_sort_hoare_helper(li, p + 1, high)

def hoare_partition(li, low, high):
    pivot = li[(low + high) // 2][1]
    i = low - 1
    j = high + 1

    while True:
        i += 1
        while li[i][1] > pivot:
            i += 1

        j -= 1
        while li[j][1] < pivot:
            j -= 1

        if i >= j:
            return j

        li[i], li[j] = li[j], li[i]

def main():
    # save the hashmap
    network = data('users.txt')

    # check to see how many users we have in the hashmap
    print(f"Total users loaded: {len(network)}")

    # opens all the files we need to write to and read from
    with open('posts_MerSort.txt', 'w') as file_merg, \
         open('posts_QSortLom.txt', 'w') as file_QSL, \
         open('posts_QSortHoa.txt', 'w') as file_QSH, \
         open('runtimes.txt', 'w') as file_runtime:
        
        file_runtime.write(f"Merge_Sort,Quick_Sort_Lomuto),Quick_Sort_Hoare\n")

        for u_id in network:
            # initialize the total time for each sort time to 0
            total_time_merg = 0
            total_time_qsl = 0
            total_time_qsh = 0

            post_pair = []

            for friend in network[u_id]['friends']:
                if friend in network:
                    f_posts = network[friend]['posts']
                    post_pair.append((friend, f_posts))

            # so we can sort each one without changing the other
            list_merg = post_pair.copy()
            list_qsl = post_pair.copy()
            list_qsh = post_pair.copy()

            # timing the merge sort
            start_time_merg = time.time() * 1000000
            sort_merg = merge_sort(list_merg)
            end_time_merg = time.time() * 1000000
            total_time_merg += end_time_merg - start_time_merg

            # timing the quick sort (Lomuto)
            start_time_qsl = time.time() * 1000000
            sort_qsl = quick_sort_lomuto(list_qsl)
            end_time_qsl = time.time() * 1000000
            total_time_qsl += end_time_qsl - start_time_qsl

            # timing the quick sort (Hoare)
            start_time_qsh = time.time() * 1000000
            sort_qsh = quick_sort_hoare(list_qsh)
            end_time_qsh = time.time() * 1000000
            total_time_qsh += end_time_qsh - start_time_qsh

            # setting the strings to write to the text files
            merg_str = " ".join([f"{friend}({posts})" for friend, posts in sort_merg])
            qsl_str = " ".join([f"{friend}({posts})" for friend, posts in sort_qsl])
            qsh_str = " ".join([f"{friend}({posts})" for friend, posts in sort_qsh])

            # writes the string we looped and puts it in the text file with user id
            file_merg.write(f"{u_id}: {merg_str.strip()}\n")
            file_QSL.write(f"{u_id}: {qsl_str.strip()}\n")
            file_QSH.write(f"{u_id}: {qsh_str.strip()}\n")

            # writes the time for the sorts in a text file
            file_runtime.write(f"{total_time_merg:.3f},{total_time_qsl:.3f},{total_time_qsh:.3f}\n")

main()