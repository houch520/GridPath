#include<iostream>
#include<vector>
#include<map>
#include<stdio.h>
#include <algorithm>
#include <math.h>

using namespace std;

class Node;
class Grid;

class Node{
  private:
    int Nodeheight;
  public:
    Node(int height){
      Nodeheight = height;
    }
    int getHeight(){return Nodeheight;}
};

class Grid{
  private:
    int Gridsize;
    map<pair<int,int>,Node> Maze;
  public:
    Grid(int size){
      Gridsize = size;
    }
    void InitNode(map<pair<int,int>,Node> map){
      Maze.insert(map.begin(),map.end());
    }
    void testDataInit(){
      for(int i =0;i<Gridsize;i++){
        for(int u=0;u<Gridsize;u++){
          Node tempNode(i*3+u+1);
          Maze[make_pair(i,u)] = tempNode;  
        }
      }
    }
    Node getNode(pair<int,int> position){return Maze[position];}
    int getSize(){return Gridsize;}
};





int main() {
  Grid playground(3);
  playground.testDataInit();

  
  cout << "Hello World\n";  
}