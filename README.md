# FindingPathsFast
Search using A* in c++ 11

--1.01--
-added unix version FindPath_unix (works on linux, with g++ and valgrind)
-checked for memory leaks with valgrind 
--

Create Point-> Expand point-> check if solved-> repeat

All points contain their move list, id, and esstimated cost

It is possible to easily add some additional features such as different terains, disadvantage to turning and so on.

MapSearch is created so that point's don't have to contain too much information.

I'm using Manhatan instead of eucelidan because it's not allowed to move diagonally, so it's not technically the shortest path

I checked for memory leaks with visual leak detector for vs, it seems to work fine. I wasn't able to use valgrind cause I made this in visual c++ and for some reason g++ cannot read it.
In general performance should be good, I didn't do anything special for multithreaded environment, I think this is light enough for one thread

according to my time tracker it took me 8h to make this. Is this way to much? In my defense I didn't use c++ for a while, so I was a bit slow at the start