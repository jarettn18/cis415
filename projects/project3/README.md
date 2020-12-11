PROJECT 3: InstaQuack
CIS 415
Author: Jarett Nishijo

A simple multi-threaded implentation of a back-end for a Social Media Service.
Publisher and Subscriber model
  -Publisher threads push image entries to a registry of circular buffers.
  -Subscriber threads read images from their respective buffer.
  -A Clean up thread is launched to dequeue old entries

Usage:
  An input file is given to the executable to launch publisher and subscriber threads
  
  Input file contains commands:
  
    create: Adds a circular buffer to the registry
        create topic <ID> <Name> <Max Length>
        
    add: Adds a publisher/subscriber thread to be launched
       add <publisher/subscriber> <publisher/subscriber command file>
       
    delta: Changes time the clean up thread uses to dequeue old entries
       delta <time>
    
    start: Launches all threads added
       start
       
    Publisher Commands:
        
      put: adds entry to circular buffer
          put <topic ID> <image URL> <caption>
          
      sleep: sleep for <time> ms
        sleep <time (ms)>
       
      stop: signals end of thread
        stop
    
    Subscriber Commands:
        
      get: reads all unread entries from topic
        get <topic ID>
          
      sleep: sleep for <time> ms
        sleep <time (ms)>
       
      stop: signals end of thread
        stop
       
