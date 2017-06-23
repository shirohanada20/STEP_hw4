#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <map>

struct Node {
  int number;
  double score, edgesscore; //score is pagerank's score.
  std::string name;
  std::vector<int>* targets;

  Node(){
    score = 1.0;
    edgesscore = 0;
    targets = new std::vector<int>;
  }

  ~Node(){
    delete(targets);
  }
};

class Graph{

private:
  void evaluateranking(int num, float score);

public:
  std::map<int, Node*>* graph;
  void addpages(std::string filename);
  void addlinks(std::string filename);
  void evaluatepagerank();
  std::vector<Node*> ranking;

  Graph(){
    graph = new std::map<int, Node*>;
    Node* provision;
    provision = new Node;
    provision -> score = 0;
    for (int i = 0; i < 10; i++) {
      ranking.push_back(provision);
    }
  }

  ~Graph(){
    delete(graph);
  }
};

void Graph::evaluateranking(int num, float score){
  for (int i = 0; i < 10; i++) {
    if (score < ranking.at(i) -> score) {
      if (i != 0) {
        ranking.erase(ranking.begin() + i-1);
        ranking.insert(ranking.begin() + i-1, graph -> at(num));
        break;
      }
      else {
        break;
      }
    }
    else if (i == 9){
      ranking.erase(ranking.begin() + i);
      ranking.insert(ranking.begin() + i, graph -> at(num));
      break;
    }
  }
}

void Graph::addpages(std::string filename){
  int num;
  std::string str;
  char name[512];
  std::ifstream ifs(filename);
  while (getline(ifs, str)) {
    Node* node = new Node;
    sscanf(str.data(), "%d\t%s", &num, name);
    node -> number = num;
    node -> name = name;
    graph -> insert(std::make_pair(num, node));
    //std::cout << node -> number << '\n';
    //std::cout << node -> name << '\n';
  }
  int size = graph -> size();
  for (int i = 0; i < size; i++) {
    graph -> at(i) -> score = 1.0;
  }
}

void Graph::addlinks(std::string filename){
  int num, target;
  std::string str;
  std::ifstream ifs(filename);
  while (getline(ifs, str)){
    sscanf(str.data(), "%d\t%d", &num, &target);
    graph -> at(num) -> targets -> push_back(target);
    //std::cout << target << '\n';
  }
}

void Graph::evaluatepagerank(){
  int links, score, a, b;
  int size = graph -> size();
  for (int i = 0; i < size; i++) {
    links = graph -> at(i) -> targets -> size();
    std::cout << "links" << '\n';
    std::cout << links << '\n';
    std::cout << "score" << '\n';
    std::cout << score << '\n';
    a = graph -> at(i) -> score;
    score = a/b;
    std::cout << score << '\n';
    for (int j = 0; j < links; j++) {
      graph -> at(graph -> at(i) -> targets -> at(j)) -> edgesscore += score;
    }
  }
  for (int l = 0; l < size; l++) {
    graph -> at(l) -> score = graph -> at(l) -> edgesscore;
    graph -> at(l) -> edgesscore = 0;
    evaluateranking(l, graph -> at(l) -> score);
  }
}




int main(int argc, char const *argv[]) {
  Graph* graph = new Graph;
  Node* node;
  int a = 10;
  float b = 1.0;
  std::cout << b/a << '\n';
  graph -> addpages("pages.txt");
  graph -> addlinks("links.txt");
  graph -> evaluatepagerank();
  for (int i = 0; i < 10; i++) {
    std::cout << graph -> ranking.at(i) -> number << '\n';
    std::cout << graph -> ranking.at(i) -> name << '\n';
    std::cout << graph -> ranking.at(i) -> score << '\n';
  }
  /*node = graph -> graph -> at(0);
  std::cout << node -> name << '\n';
  std::cout << node -> targets -> front() << '\n';*/
  delete(graph);
  return 0;
}
