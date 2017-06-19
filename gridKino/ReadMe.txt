Program logic:
"Street" is a line of matrix. It's implemented as a vector filled with unique int's (each is +1 to previous). 

Create a map to store FREE parts of streets. Map is used to conveniently address streets. 
Each street is created as completely free once it is read from configure file for the first time (to create a reel on it). 
Creating a reel means taking difference between a free part of the street and current reel. This dirrerence is written to map, replacing previous value of this free part of the street.
Such, completely free streets are not contained in folder (they are completely free for a short period of time between street initialization and taking defference with the reel on it, resulting difference is what is stored).
Whenever a new reel is read from configure file, the difference is taken with the free part of the street, no matter how many reels are already on that street.

Once all configure file is processed, total amount of free cells is simply a sum of sizes of all free parts of streets in the map + all cells of completely free streets (that have no reels on them).
