class Solution(object):
    def assignTasks(self, servers, tasks):
        import heapq
        available = [(weight, i) for i, weight in enumerate(servers)]
        heapq.heapify(available)
        busy = []
    
        ans = []
        curr_time = 0
    
        for j, task_time in enumerate(tasks):
        # The current time cannot be behind the task arrival time j
            curr_time = max(curr_time, j)
        
        # If no servers are available, jump time forward to when the next server frees up
            if not available:
                curr_time = max(curr_time, busy[0][0])
            
        # Free all servers that have finished their tasks by curr_time
            while busy and busy[0][0] <= curr_time:
                free_time, weight, idx = heapq.heappop(busy)
                heapq.heappush(available, (weight, idx))
            
        # Assign the task to the best available server
            weight, idx = heapq.heappop(available)
            ans.append(idx)
        
        # Put the server into the busy heap
            heapq.heappush(busy, (curr_time + task_time, weight, idx))
        
        return ans
