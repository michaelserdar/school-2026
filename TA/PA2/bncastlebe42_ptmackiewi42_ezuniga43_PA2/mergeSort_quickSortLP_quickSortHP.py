"""
Credit Statement:
We, Bryler, Emmanuel, and Paul worked within our group for this assignment and did not receive outside help from TAs or the instructor.




AI Statement:
We acknowledge the utilization of Gemini, a language model developed by Google, in the preparation of this assignment. Gemini was employed for the following manner: How to measure the clock-time taken by the code, and reviewing the assignment instructions for completeness and formatting suggestions.
"""





import time
import matplotlib.pyplot as plt




def lomuto_partition(arr, low, high):
    pivot = arr[low][1]
    l = low + 1
    for r in range(low+1, high+1):
        if arr[r][1] > pivot:
            arr[l], arr[r] = arr[r], arr[l]
            l+=1
    arr[low], arr[l-1] = arr[l-1], arr[low]
   
    return l - 1
           
     




def quick_sort_lomuto(arr, low=0, high=None):
    if high is None:
        high = len(arr) - 1
       
    if low < high:
        pivot_index = lomuto_partition(arr, low, high)
       
        quick_sort_lomuto(arr, low, pivot_index-1)
        quick_sort_lomuto(arr, pivot_index+1, high)
   
    return arr








def hoare_partition(arr, low, high):
    pivot = arr[low][1]
    l =  low - 1
    r = high + 1




   
    while (True):
        l+=1
        while(arr[l][1] > pivot):
            l+=1
        r-=1
        while(arr[r][1] < pivot):
            r-=1
       
        if l >= r:
            return r
        arr[l],arr[r] = arr[r],arr[l]
   




def quick_sort_hoare(arr, low=0, high=None):
    if high is None:
        high = len(arr) - 1
    if low < high:
        pivot_index = hoare_partition(arr, low, high)
       
        quick_sort_hoare(arr, low, pivot_index)
        quick_sort_hoare(arr, pivot_index+1, high)
    return arr




def merge(A,B):
    C = []
    i=j=0
   
    while i < len(A) and j < len(B):
        if A[i][1] >= B[j][1]:
            C.append(A[i])
            i+=1
        else:
            C.append(B[j])
            j+=1
           
    C.extend(A[i:])
    C.extend(B[j:])
   
    return C








def mergeSort(arr):
    if len(arr) <= 1:
        return arr
    mid = len(arr) // 2
    L = mergeSort(arr[:mid])
    R = mergeSort(arr[mid:])
    B = merge(L,R)
    return B








# Get the data (basically the same from the last checkpoint)
user_posts = {}    # {user id: num_posts}
user_friends = {}  # {user id: [list_of_friend_ids]}








with open('users.txt', 'r') as f:
    for line in f:
        data = line.split()
        if len(data) == 0: continue








        user_id = str(data[0])
        posts = int(data[1])








        raw_friends_string = " ".join(data[2:])
        friends = raw_friends_string.strip("[]").split()








        user_posts[user_id] = posts
        user_friends[user_id] = friends
















def get_string_data(user_id, data):
    pairs = []
    for d in data:
        pair = f"({d[0]}, {d[1]})"
        pairs.append(pair)








    return f"{user_id} {' '.join(pairs)}\n"
















# Write results to file
sorted_uids = list(user_friends.keys())
runtimes = []








merge_sort_file = open('posts_MerSort.txt', 'w')
lomuto_sort_file = open('posts_QSortLom.txt', 'w')
hoare_sort_file = open('posts_QSortHoa.txt', 'w')








for user_id in sorted_uids:
    friends = user_friends[user_id]
    # List of (friend_id, post_count)
    base_data = []








    for f_id in friends:
        base_data.append((f_id, user_posts[f_id]))








    # Merge Sort
    merge_data = list(base_data)
    start = time.perf_counter_ns()
    sorted_mer = mergeSort(merge_data)
    merge_microseconds = (time.perf_counter_ns() - start) / 1000








    # Quick Sort with Lomuto partition
    lomuto_data = list(base_data)
    start = time.perf_counter_ns()
    quick_sort_lomuto(lomuto_data, 0, len(lomuto_data) - 1)
    lomuto_microseconds = (time.perf_counter_ns() - start) / 1000








    # Quick Sort with Hoare partition
    hoare_data = list(base_data)
    start = time.perf_counter_ns()
    quick_sort_hoare(hoare_data, 0, len(hoare_data) - 1)
    hoare_microseconds = (time.perf_counter_ns() - start) / 1000








    runtimes.append((merge_microseconds, lomuto_microseconds, hoare_microseconds))








    merge_sort_file.write(get_string_data(user_id, sorted_mer))
    lomuto_sort_file.write(get_string_data(user_id, lomuto_data))
    hoare_sort_file.write(get_string_data(user_id, hoare_data))








merge_sort_file.close()
lomuto_sort_file.close()
hoare_sort_file.close()




x = list(range(1,101))
y1, y2, y3 = [], [], []




for i in range(len(runtimes)):
   y1.append(runtimes[i][0])
   y2.append(runtimes[i][1])
   y3.append(runtimes[i][2])




fig, ax = plt.subplots()
plt.subplots_adjust(left=0.1, right=0.9, top=0.9, bottom=0.1)




width = 0.4




ax.plot(x, y1, label="Merge Sort")
ax.plot(x, y2, label="Lomuto Sort")
ax.plot(x, y3, label="Hoare Sort")




ax.set_title("Different Sort Methods' Runtimes User Data")
ax.set_xlim(0, 100)




plt.legend()
plt.xlabel("User ID:")
plt.ylabel("Runtime (Micoseconds)")




plt.show()








with open('runtimes.txt', 'w') as f:
    for r in runtimes:
        f.write(f"({r[0]:.2f}, {r[1]:.2f}, {r[2]:.2f})\n")


