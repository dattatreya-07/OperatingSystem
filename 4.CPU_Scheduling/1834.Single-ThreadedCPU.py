import heapq

class Solution(object):
    def getOrder(self, tasks):
        """
        :type tasks: List[List[int]]
        :rtype: List[int]
        """
        # 1. Attach original index to each task so we don't lose track after sorting
        # Format: [enqueueTime, processingTime, originalIndex]
        extended_tasks = []
        for i, task in enumerate(tasks):
            extended_tasks.append([task[0], task[1], i])
            
        # 2. Sort tasks primarily by enqueueTime
        extended_tasks.sort(key=lambda x: x[0])
        
        res = []
        min_heap = []
        curr_time = 0
        task_idx = 0
        n = len(tasks)
        
        # 3. Process until all tasks are executed
        while task_idx < n or min_heap:
            # If the heap is empty, the CPU is idle. 
            # Fast-forward time to the next available task's enqueue time.
            if not min_heap and curr_time < extended_tasks[task_idx][0]:
                curr_time = extended_tasks[task_idx][0]
                
            # Push all tasks that have already arrived by the current time into the heap
            while task_idx < n and extended_tasks[task_idx][0] <= curr_time:
                enqueue, process, idx = extended_tasks[task_idx]
                # Python min-heap automatically sorts tuples by first element, then second
                heapq.heappush(min_heap, (process, idx))
                task_idx += 1
                
            # The CPU picks the best task (shortest process time, then smallest index)
            process_time, idx = heapq.heappop(min_heap)
            curr_time += process_time
            res.append(idx)
            
        return res
