/**
 * @file sketchify.cpp
 * Implementation of the sketchify function.
 */
#pragma GCC diagnostic ignored "-Wuninitialized"
#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>

#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include "busplanner.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;

V2D csvToVector(const std::string & filename){
    V2D rtn;
    
    string str = file_to_string(filename);
    vector<string> split;

    int splitNum = SplitString(str, '\n', split);

    //for (auto e : split) {
    //    std::cout << "Original split: " << e << std::endl;
    //}

    for (int i = 0; i < splitNum; i++) {
        vector<string> splitStrings;
        int splitStringsNum = SplitString(split[i], ',', splitStrings);
        bool add = true;
        for (auto& e : splitStrings) {
            if(e == "")
            {
                add = false;
                break;
            }
            e = Trim(e);
            //std::cout << "Final split num: " << i << " " << e << std::endl;
        }
        if(add)
            rtn.push_back(splitStrings);
    }


    return rtn;
}



void printVector(const std::vector<std::string>& v){
    
    for (size_t i = 0; i < v.size(); i++) {
        if (i != v.size() - 1) {
            std::cout << v[i] << ", ";
        } else {
            std::cout << v[i] << std::endl;
        }
    }
}

void printVector(const std::vector<int>& v){
    
    for (size_t i = 0; i < v.size(); i++) {
        if (i != v.size() - 1) {
            std::cout << v[i] << ", ";
        } else {
            std::cout << v[i] << std::endl;
        }
    }
}

Graph make_adj_list(const V2D &routes) {
    Graph g(true);

    for (size_t i = 0; i < routes.size(); i += 3) {
        string bus = routes[i][0];
        for (size_t j = 1; j < routes[i].size(); j++) {
            Vertex startVertex = routes[i][j];
            int currentDistance = 0;
            
            for (size_t k = j + 1; k < routes[i].size(); k++) {
                Vertex endVertex = routes[i][k];
                currentDistance += std::stoi(routes[i+2][k]);
                
                if (!g.edgeExists(startVertex, endVertex)) {
                    bool b = g.insertEdge(startVertex, endVertex);
                    Edge e = g.setEdgeWeight(startVertex, endVertex, currentDistance);
                    Edge e2 = g.setEdgeLabel(startVertex, endVertex, bus);
                } else {
                    int currWeight = g.getEdgeWeight(startVertex, endVertex);
                    if (currentDistance < currWeight) {
                        Edge e = g.setEdgeWeight(startVertex, endVertex, currentDistance);
                        Edge e2 = g.setEdgeLabel(startVertex, endVertex, bus);
                    } 
                }
                
            }

        }
    }

    return g;
}

auto find_path(Graph g, Vertex start, Vertex end) -> std::vector<Edge>
{
    if(start == end)
    {
        std::vector<Edge> rt;
        return rt;
    }
    std::unordered_set<Vertex> visited;
    visited.insert(start);
    size_t num_vert = g.getVertices().size();
    std::unordered_map<Vertex, std::pair<Vertex, int>> distances; // Vertex, Vertex, int
    distances.insert(std::make_pair(start,std::make_pair(start,0)));
    for(Vertex neighbor : g.getAdjacent(start))
    {
        distances.insert(std::make_pair(neighbor,std::make_pair(start,g.getEdgeWeight(start,neighbor))));
    }
    Vertex current = start;
    while(current != end)
    {
        int minimum_edge = INT_MAX;
        Vertex going_to;
        for(auto pair : distances)
        {
            if(pair.second.second < minimum_edge && visited.find(pair.first) == visited.end())
            {
                going_to = pair.first;
                minimum_edge = pair.second.second;
            }
        }
        visited.insert(going_to);
        for(Vertex neighbor : g.getAdjacent(going_to))
        {
            if(distances.find(neighbor) == distances.end())
                distances.insert(std::make_pair(neighbor,std::make_pair(going_to,distances[going_to].second + g.getEdgeWeight(going_to,neighbor)))); // this node has not yet been in the dictionary
            else
            {
                int current_distance_to_neighbor = distances[neighbor].second;
                int new_distance_to_neighbor = distances[going_to].second + g.getEdgeWeight(going_to,neighbor);
                if(current_distance_to_neighbor > new_distance_to_neighbor)
                    distances[neighbor] = std::make_pair(going_to,new_distance_to_neighbor);
                
            }
        }
        current = going_to;
    }
    // now current has reached end, should have guaranteed found a path (assume one connected component)
    std::vector<Edge> rt;
    while(current != start)
    {
        Vertex backtrack = distances[current].first;
        rt.push_back(g.getEdge(current,backtrack));
        current = backtrack;
    }
    //rt = compress_path(rt);
    return rt;
}

auto compress_path(std::vector<Edge> path) -> std::vector<Edge>
{
    std::vector<Edge> rt;
    if(path.empty())
    {
        return rt;
    }
    std::string previous_bus = path[0].getLabel();
    for(size_t i = 0; i < path.size(); i++)
    {
        int weight = 0;
        Vertex start = path[i].source;
        Vertex end = path[i].dest;
        while(i < path.size() && path[i].getLabel() == previous_bus)
        {
            weight += path[i].getWeight();
            end = path[i].dest;
            i++;
        }
        // path[i-1] is last path with previous bus

        Edge to_add(start, end, weight, previous_bus);
        rt.push_back(to_add);
        if(i < path.size())
            previous_bus = path[i].getLabel();
        i--;
    }
    return rt;
}