<h1>movesToSort</h1>

A project to compare the BFS, A*, and IDS sorting algorithms, outputting the following:
<ul>
  <li>Steps taken to sort</li>
  <li>Queue size (if applicable)</li>
  <li>The number of visited states</li>
</ul>

<h3>To Build</h3>

****
Windows:
```
g++ -o movesToSort.exe movesToSort.cpp
```
Linux/MacOS
```
g++ -o movesToSort.out movesToSort.cpp
```

<h3>To Run</h3>

***
Run the program with:
```
./movesToSort.exe
```
The program will then prompt for input. Valid inputs are a string of space-delimited integers (no more than 10) in any order.
<br>
<br>
Examples:
```
1 3 5 7 9 2 4 6 8 10
```
```
9 3 4 6 2 7 8 10 1 5
```
