# FindingPathsFast
Search using A* in c++ 11 in visual studio

1.02
now 10x faster with the same results, also added map generation, and console map drawing with path

Search using A*

Create Point-> Expand point-> check if solved-> repeat

All points contain their move list, id, and esstimated cost

It is possible to easily add some additional features such as different terains, disadvantage to turning and so on.

MapSearch is created so that points don't have to contain too much information.

I'm using Manhatan instead of eucelidan because it's not allowed to move diagonally, so path length should be the same
There aren't any memory leaks.
In general performance is decent, but still not good enough for real time, on my laptop 100x100 map with worst case calculates 0.5s. if non perfect solution is ok, it could work 10x faster in some cases.
Addidng multithreading could make it couple times faster, but I haven't really considered it at the beggining, and now it's a bit difficult to fit it to this solution.

I tried using list instead of vector but performance was 30% worse
Main bottleneck is inserting elements into my expanding vector, to avoid sorting I have to insert objects in the middle of the vector