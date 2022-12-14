<div align = "center">
  
 
  <p align = "center">
    <h1>BusMap</h1>
    <p><h3>Created by Yash Telang, Arthur Wang, and Varun Athreye</h3></p>
  </p>
  
  <p align = "center">
    <a href = "https://drive.google.com/file/d/1CLIBh1Oy-qlJorXU3sJebJuEvW4xdg1Z/view?usp=sharing">Presentation</a>
  </p>
</div>

<details>
  <summary><h2>Table of Contents</h2></summary>
  <ol>
    <li><a href = "#summary">Summary</a></li>
    <li><a href = "#development-process">Development Process</a></li>
    <li>
      <a href = "#github-organization">GitHub Organization</a>
    </li>
    <li>
      <a href = "#running-instructions">Running Instructions</a>
    </li>
  </ol>
</details>



<!--- Summary of presentation introduction --->
## Summary
Our project is a C++ program called BusMap, which takes in 2 bus stops from the Champaign–Urbana Mass Transit District, and displays the most optimal path of buses to take to get from the start input to the destination input. It also displays a map with all of the MTD stops, and highlights the stops in the path between the inputted destinations. To do this, it employs all of our proposed algorithms, which are BFS, Dijkstra's, and the Graphic Output of Graph algorithm. 


<!--- Development Process --->
## Development Process
We first created CSV files with data on the buses, including their locations, start/end times, the stops they go to, etc. Using this data about the time it takes to get from one stop to another, we assigned each connected stop a weight as the time it would take to get from one to the other. For example, if it took 5 minutes to get from Stop A to Stop B (it would take the same amount of time to go from Stop B to Stop A), the edge between the two stops would be weighted as 5. 

We then parsed this data in our program and created an undirected, weighted Graph structure with bus stops as vertices and buses as edges. Each edge had its respective weight as described above. This would then allow us to use our proposed algorithm (Dijkstra's) to find the best path to take between the two inputted stops.

Using our graph containing all of the parsed data about the buses, we then implemented Dijkstra's algorithm to find the shortest path between two inputted stops. We also compressed this path to account for multiple stops on the same bus, and then stored it in a Path object, which contains a vector of the stops and the estimated weight (time) it would take to get from one stop to another.

Next, using the parsed data about the stop locations on a map of the Champaign area, we created a PNG containing this data and highlighted the stops that the most optimal path takes on the map (Graphic Output of Graph algorithm). Finally, we used BFS to find the shortest path between the stops on the map, then highlighting the path that would be taken to get from one to another. This map is then outputted to a PNG that can be displayed to the user.

Finally, throughout this process, we have been testing each component to make sure the data and results are accurate. This includes having assertions for the parsed data, comparing the output of our path finding algorithm to our manually calculated path (using multiple inputs), and confirming that the drawn paths match up on the outputted PNG.


<!--- Github Organization --->
## GitHub Organization

The `Documents` folder contains our project proposal, team contract, and written report.

The `Data` folder contains our created CSVs and the base image of the map that we use to display the path.

The `lib` folder contains provided CS225 classes and methods such as `PNG.h` and `HSLAPixel.h` that are used to read from and write to PNG files.

The `tests` folder contains all of our created test cases as described in our development process.

The `src` folder contains the majority of our code, where we have our graph structure in `Graph.h`, and our functions to parse data, build/compress our path, and output the path to a PNG in `busplanner.h`. This folder also contains the provided `utils.h`, which lets us easily read from files, split by string, and trim strings.

The `entry` folder contains `main.cpp`, which is where we use the functions and structures that we created to parse the data, specify user input, build and compress the path, and write to the outputted PNG.

Our presentation is linked at the top of this README (you must be signed in to your Google @ Illinois account to view the file).

<!--- Provides reproducible installation and running instructions --->
## Running Instructions

To run the program:
1. Download/Clone the repository to your local machine.
2. Open the repository using the standard CS225 Dockerfile.
3. `cd` to the `BusMap` directory.
4. Run `mkdir build`, `cd build`, and `cmake ..` to build the project (same process as CS225 MPs and Labs).
5. Run `make` to compile all files.
6. Run the program by inputting `./main "Start location" "End location" "Output file location"`. If no output file location is specified, the outputted PNG will be in `../Data/MapPath.png`. The possible start and end bus stop locations are displayed in the leftmost column in the `../Data/BusCoordinates.csv` file (they are also displayed when running the program if a location is invalid). If either start/end locations are not valid, the program will terminate and will give you a list of valid stops.
7. Run the tests by running `./test`.

<img src = "./Data/Map.png">
