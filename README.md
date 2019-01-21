# smart_locks

A smart lock datastructure that prevents thread deadlock. 
This is implemented by using a undirected graph representation of resources and threads. 
Vertices are threads and resources and edges are resource-thread assignments. 
If a cycle in the graph is detected upon resource request, the request is denied to prevent cycledetection. 
