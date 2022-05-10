#include <iostream>
#include <vector>
#include <map>
#include <stdio.h>
#include <algorithm>
#include <math.h>
#include <chrono>


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
    void printHeight(){
      for(int z=0;z<Gridsize;z++){printf("----");}
      printf("-");
      for(int i=0;i<Gridsize;i++){
        cout<<endl<<"|";
        for(int u=0;u<Gridsize;u++){
          printf("%3d|", Maze[make_pair(i,u)].getHeight());
        }
        cout<<endl;
        for(int z=0;z<Gridsize;z++){printf("----");}
        printf("-");
        cout<<endl;
      }
    }
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
    void printInfo(){
      this->printSad();
      this->printPath();
    }
    void printPos(){
      cout<<currentPosition.first<<":"<<currentPosition.second<<endl;
    }
    void printSad(){
      cout<<"SadValue:"<<SadValue<<endl;
    }
    void printPath(){
      cout<< "Path:\n";
      for(int i=0;i<Path.size();i++){
        printf("x:%d,y:%d\n",Path[i].first,Path[i].second);
      }
    }
    int getPathSize(){return Path.size();}
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
      if(find(Path.begin(),Path.end(),destination)!=Path.end()){return false;}
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
      SetPosition(destination);
      return;
    }  
};

class PathFinder: public Player{
  public:
    PathFinder(pair<int,int> startPoint,Grid *Playground):Player(Playground){
      SetPosition(startPoint);
    }
    PathFinder(PathFinder * cloneTarget){
      currentPosition = cloneTarget->currentPosition;
      currentPlayground = cloneTarget->currentPlayground;
      Path = cloneTarget->Path;
      SadValue = cloneTarget->SadValue;
    }
    
    
    PathFinder PathFind(){
      if(Path.size()>=pow(currentPlayground->getSize(),2)){
        return *this;
      }
      int minSadValue = pow(currentPlayground->getSize(),2)+1;
      PathFinder * minSadptr = new PathFinder(*this);
      minSadptr->SadValue = minSadValue;
      PathFinder up(this->CloneMove(make_pair(currentPosition.first,currentPosition.second+1))),
      down(this->CloneMove(make_pair(currentPosition.first,currentPosition.second-1))),
      left(this->CloneMove(make_pair(currentPosition.first-1,currentPosition.second))),
      right(this->CloneMove(make_pair(currentPosition.first+1,currentPosition.second)));
      if(CheckValidMove(make_pair(currentPosition.first,currentPosition.second+1))){
        up = up.PathFind();
        if(up.SadValue<minSadValue && up.currentPosition!= currentPosition){minSadValue =up.SadValue;minSadptr = &up;}
      }
      if(CheckValidMove(make_pair(currentPosition.first,currentPosition.second-1))){
        down= down.PathFind();
        if(down.SadValue<minSadValue && down.currentPosition!= currentPosition){minSadValue =down.SadValue;minSadptr = &down;}
      }
      if(CheckValidMove(make_pair(currentPosition.first-1,currentPosition.second))){
        left=left.PathFind();

        if(left.SadValue<minSadValue&& left.currentPosition!= currentPosition){minSadValue =left.SadValue;minSadptr = &left;}
      }
      if(CheckValidMove(make_pair(currentPosition.first+1,currentPosition.second))){
        right = right.PathFind();

        if(right.SadValue<minSadValue && right.currentPosition!= currentPosition){minSadValue =right.SadValue;minSadptr = &right;}
      }
      return new PathFinder(*minSadptr);
    }
    PathFinder CloneMove(pair<int,int> dest){
      PathFinder returnTemp(*this);
      returnTemp.move(dest);
      return returnTemp;
    }
};
int main() {
  auto start = std::chrono::high_resolution_clock::now();
  //size nxn 
  Grid playground(3);
  playground.testDataInit();
  PathFinder test(make_pair(0,0),&playground);
  test.PathFind();
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  printf("runtime(in mileseconds): %ld\n",duration.count()); 
}