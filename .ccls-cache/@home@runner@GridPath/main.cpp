#include <iostream>
#include <vector>
#include <map>
#include <stdio.h>
#include <algorithm>
#include <math.h>

using namespace std;

class Node;
class Grid;
class Player;
class PathFinder;
class Node{
  private:
    int Nodeheight;
  public:
    Node(int height){
      Nodeheight = height;
    }
    Node(){
      Nodeheight =0;
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
    Grid(){
      Gridsize =0;
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

class Player{
  protected:
    pair<int,int> currentPosition;
    Grid * currentPlayground;
    vector<pair<int,int>> Path;
    int SadValue = 0;
  public:
    Player(Grid * Playground){
      currentPlayground = Playground;
    }
    Player(Player *target){
      currentPosition =target->currentPosition;
      currentPlayground = target->currentPlayground;
      Path = target->Path;
    }
    Player(){
      init();
    }
    void init(){
      currentPosition = make_pair(-1,-1);
      currentPlayground = new Grid();
    }
    bool CheckValidMove(pair<int,int> destination){
      //out of range
      if(!(Checkcoordinate(destination.first)&& Checkcoordinate(destination.second))){
        return false;
      }
      //move more than one step
      if(CountNumofStep(destination)>1){return false;}
      //stepped before
      if(find(Path.begin(),Path.end(),destination)!=Path.end()){return true;}
      return true;
    }
    bool Checkcoordinate(int destination){
      if(destination<0||destination>=currentPlayground->getSize()){
        return false;
      }
      return true;
    }
    int CountNumofStep(pair<int,int> destination){
      return (abs(destination.first-currentPosition.first)+abs(destination.second-currentPosition.second));
    }
    void SetPosition(pair<int,int> destination){
      currentPosition = destination;
      Path.push_back(currentPosition);
      return;
    }
    int checkSadValue(pair<int,int> destination){
      if(currentPlayground->getNode(currentPosition).getHeight()<currentPlayground->getNode(destination).getHeight()){
        return 1;
      }else{
        return 0;
      }
    }
    void move(pair<int,int> destination){
      if(!CheckValidMove(destination)){
        return ; 
      }
      SadValue+=checkSadValue(destination);
      Path.push_back(destination);
      SetPosition(destination);
      return;
    }  
};

class PathFinder: public Player{
  public:
    PathFinder(pair<int,int> startPoint,Grid *Playground):Player(Playground){
      SetPosition(startPoint);
    }
    void printPos(){
      cout<<currentPosition.first<<":"<<currentPosition.second<<endl;
    }
  
};
int main() {
  Grid playground(3);
  playground.testDataInit();
  PathFinder test(make_pair(0,0),&playground);
  test.move(make_pair(1,0));
  test.printPos();
  test.SetPosition(make_pair(2,0));
  test.printPos();
  
  cout << "Hello World\n";  
}