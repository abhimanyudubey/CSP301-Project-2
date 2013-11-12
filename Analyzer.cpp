#include <stdio.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>
#define NOPATH 10000
#define DEBUG 0
//Debug levels 0-5. 0: no dialogs, 5 - lots of dialogs.

using namespace std;

typedef vector<vector<int> > matrix;

string getWord(string &source){
	string word = source.substr(0, source.find(" "));
	source = source.substr(source.find(" ")+1);
	return word;
}

struct aGraph{
	vector<string> nodeList;
	//Has the list of nodes with their labels. 
	//TODO: Add the other properties of the label such as name and university.
	vector<string> sourceList;
	//List of sources of each edge.
	vector<string> targetList;
	//List of targets of each edge.
	vector<string> edgeList;
	//List of IDs of each edge in the graph. Not required in algos as such.
	matrix adjacencyMatrix;
	//Adjacency matrix of the graph, makes it simpler to store the edges. Since all graphs are undirected,
	//this matrix is symmetric.
};

aGraph Parser(const char* fileName){
	int nodeCount=0;
	std::vector<string> sourceNode,targetNode,nodeMap,edgeMap;
	ifstream inFile;
	inFile.open(fileName);
	if(inFile.is_open()){
		while(!inFile.eof()){
			//Read the lines to create the graph here.
			string line;
			getline(inFile,line);
			size_t startc = line.find("<"),startc2 = line.find("</"),endc = line.find(">"),endc2 = line.find("/>");
			if(startc!=string::npos && endc!=string::npos){
				startc+=startc2==startc? 1:0;
				endc-=endc2!=string::npos? 1:0;
				string linef = line.substr(startc+1, endc-startc-1);
				string fw = getWord(linef);
				if(linef==fw){linef="";fw="";}
				if(fw=="node"){
					//Handling a new node.
					if(DEBUG>0)
						cout << "Node found " << nodeCount << endl;
					++nodeCount;
					while(linef!=""){
						int eq = linef.find("=");
						string atw = getWord(linef);
						if(atw==linef){
							linef="";
						}
						string attrib = atw.substr(0,eq);
						string value = atw.substr(eq+2);
						value=value.substr(0,value.find("\""));
						if(attrib=="id"){
							if(value.substr(0,3)=="fac"){
								nodeCount--;
							}
							else
								nodeMap.push_back(value);
						}
						if(DEBUG>1)
							cout << attrib <<" "<< value << endl;
					}
				}
				else if(fw=="edge"){
					if(DEBUG>0)
						cout << "Edge found." << endl;
					while(linef!=""){
						int eq = linef.find("=");
						string atw = getWord(linef);
						if(atw==linef){
							linef="";
						}
						string attrib = atw.substr(0,eq);
						string value = atw.substr(eq+2);
						value=value.substr(0,value.find("\""));
						if(attrib=="source"){
							sourceNode.push_back(value);
						}
						else if(attrib=="target"){
							targetNode.push_back(value);
						}
						else if(attrib=="id"){
							edgeMap.push_back(value);
						}
						if(DEBUG>2)
							cout << attrib <<" "<< value << endl;
					}
				}
				if(DEBUG>2)
					cout << linef << endl;
			}
		}
		cout<<"File ended."<<endl;
	}
	cout<<"file ended again."<<endl;
	//Now to generate the Adjacency Matrix and assign weights in friendships.
	cout<<nodeCount<<endl;
	bool adjacencyMatrix[nodeCount][nodeCount];
	cout<<"AdjMat created."<<endl;
	for(int i=0;i<edgeMap.size();i++){
		string s_node = sourceNode.at(i);
		string t_node = targetNode.at(i);
		int s_loc=-1,t_loc=-1;
		for(int j=0;j<nodeMap.size();j++){
			if(nodeMap.at(j)==s_node)
				s_loc=j;
			if(nodeMap.at(j)==t_node)
				t_loc=j;
		}
		if(s_loc!=-1 && t_loc!=-1){
			adjacencyMatrix[s_loc][t_loc]=1;
			adjacencyMatrix[t_loc][s_loc]=1;
			if(DEBUG>1)
				cout << "edge detected"<<endl;
		}
	}
	if(DEBUG>1)
		cout << nodeCount << endl;
	std::vector<std::vector<int> > adj;
	for(int i=0;i<nodeCount;i++){
		std::vector<int> v;
		for(int j=0;j<nodeCount;j++){
			adjacencyMatrix[i][j]=adjacencyMatrix[i][j]==1? 1: 0;
			v.push_back(adjacencyMatrix[i][j]);
			if(DEBUG>0)
				cout << adjacencyMatrix[i][j] << " ";
		}
		adj.push_back(v);
		if(DEBUG>0)
			cout <<endl;
	}
	aGraph out;
	out.nodeList=nodeMap;
	out.edgeList=edgeMap;
	out.sourceList=sourceNode;
	out.targetList=targetNode;
	out.adjacencyMatrix=adj;

	return out;
}

int getMinimumDistance(int dist[], bool set[], int n){
	int min=NOPATH, index;
	//vector<int> temp(dist, dist + sizeof dist/ sizeof dist[0]);
	int le = n;
	for(int i=0;i<le;i++){
		if(!set[i] && dist[i]<min){
			min=dist[i];
			index=i;
		}
	}
	return index;
}

int dijkstraDistance(int sourceNode, int targetNode, aGraph g){
	int nNodes = g.nodeList.size();
	int dist[nNodes];
	bool set[nNodes];

	for(int i=0;i<nNodes;i++){
		dist[i]=NOPATH,set[i]=false;
	}
	//Initialized the distance matrix with infinity values.
	dist[sourceNode]=0;

	for(int i=0;i<nNodes;i++){
		int min = getMinimumDistance(dist,set,nNodes);
		set[min]=true;
		for(int j=0;j<nNodes;j++){
			if(!set[j] && g.adjacencyMatrix[min][j] && dist[min] != NOPATH && dist[min]+g.adjacencyMatrix[min][j] < dist[j]){
				if(DEBUG)
				cout<<"echo echo"<<endl;
				dist[j] = dist[min]+g.adjacencyMatrix[min][j];	
			}
		}
	}
	return dist[targetNode];
}

int getShortestPath(int sourceNode, int targetNode, aGraph g){
	matrix adj = g.adjacencyMatrix;
	return 0;
}


int getNearestNode(vector<int> Q,bool visited[],int distance[]){
	int min=NOPATH,l = Q.size(),min_index=0,global_index=0;
	for(int i=0;i<l;i++){
		int q = Q[i];
		if(distance[q]<min && !visited[q]){
			min=distance[q];
			min_index=i;
			global_index=q;
		}
	}
	return global_index;
}
vector<int> dijkstraPath(int sourceNode, int targetNode, aGraph g){
	matrix adj = g.adjacencyMatrix;
	int N = g.nodeList.size();
	int distance[N],previous[N];
	bool visited[N];
	for (int i=0;i<N;i++){
		distance[i]=NOPATH;
		previous[i]=-1;
		visited[i]=false;
	}

	distance[sourceNode]=0;

	vector<int> Q,S;
	Q.push_back(sourceNode);

	while(Q.size()!=0){
		int u = getNearestNode(Q,visited,distance);
		if(u!=targetNode){
			Q.erase(std::find(Q.begin(),Q.end(),u));
			visited[u]=true;	
			for(int i=0;i<N;i++){
				if(adj.at(u).at(i)){
					int v = i;
					int alt = distance[u]+ 1;
					if(alt<distance[v] && !visited[v]){
						distance[v]=alt;
						previous[v]=u;
						Q.push_back(v);
					}
				}
			}
		}
		else break;
	}
	int u = targetNode;
	while(previous[u]!=-1){
		S.push_back(u);
		u = previous[u];
	}
	S.push_back(sourceNode);
	std::reverse(S.begin(),S.end());
	return S;
}

matrix floydWarshall(matrix graph, int n){
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			if(graph[i][j]==0)
				graph[i][j]=NOPATH;
		}
	}
	for(int k=0;k<n;k++){
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				graph[i][j]=min(graph[i][j],graph[i][k]+graph[k][j]);
			}
		}
	}
	return graph;
}

int getCliqueSize(int source, aGraph g){
	int sum=0;
	for(int i=0;i<g.adjacencyMatrix.size();i++)
		sum+=g.adjacencyMatrix.at(i).at(source);
	return sum;
	//this works since adjacency matrix is non-weighted.
}

vector<int> getNeighborhood(int source, aGraph g, vector<int> li){
	//This function returns the neighborhood of the particular node present in the graph. That is the set of 
	//fully connected components in the graph containing that particular node.
	int dim = g.adjacencyMatrix.size();
	for(int i=0;i<dim;i++){
		if(g.adjacencyMatrix.at(source).at(i)){
			int lis = li.size();
			bool isInLis = false;
			for(int j=0;j<lis;j++){
				if(li.at(j)==i){
					isInLis=true;
					break;
				}
			}
			if(!isInLis){
				li.push_back(i);
				vector<int> newli = getNeighborhood(i,g,li);
				for(int k=0;k<newli.size();k++){
					bool isInLis2=false;
					for(int l=0;l<li.size();l++){
						if(li.at(l)==newli.at(k)){
							isInLis2=true;
							break;
						}
					}
					if(!isInLis2)
						li.push_back(newli.at(k));
				}
			}
		}
	}
	return li;
}

int shortestPathOnGraph(aGraph g){
	matrix fw = floydWarshall(g.adjacencyMatrix,g.nodeList.size());
	int min=fw[0][0];
	for(int i=0;i<fw.size();i++){
		for(int j=0;j<fw.size();j++){
			if(fw[i][j]<min)
				min=fw[i][j];
		}
	}
	return min;
}

void printAdjacencyMatrix(aGraph g){
	matrix mat = g.adjacencyMatrix;
	int N = mat.size();
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			cout << mat.at(i).at(j) << " ";
		}
		cout << endl;
	}
}

int getImportance(aGraph g, int source){
	int N = g.adjacencyMatrix.size();
	int importance[N];
	for(int i=0;i<N;i++) importance[i]=0;

	for(int i=0;i<N;i++){
		for(int j=i+1;j<N;j++){
			vector<int> path = dijkstraPath(i,j,g);
			int ps = path.size();
			for(int k=0;k<ps;k++){
				int n = path.at(k);
				importance[n]++;
			}
		}
	}
	return importance[source];
}

bool compareImportance(aGraph g, int source){
	int N=g.adjacencyMatrix.size(),imp=getImportance(g,source);
	for(int i=0;i<N;i++){
		if(g.adjacencyMatrix.at(source).at(i)){
			int imp_i = getImportance(g,i);
			if(imp_i>imp)
				return true;
		}
	}
	return false;
}

int getGraphIndexFromGlobal(aGraph g, char* id){
	for(int i=0;i<g.nodeList.size();i++){
		if(g.nodeList.at(i)==id){
			return i;
		}
	}
	return -1;
}

int main(int argc, char** argv){
	const char* ff = "temp.temp";
	aGraph g = Parser(ff);
	cout <<"Parsed graph."<<endl;
	//Loaded graph from file.

	int choice = atoi(argv[1]);
	switch(choice){
		case 1:{
			char* id = argv[2];
			int gid = getGraphIndexFromGlobal(g,id);
			vector<int> empty;
			vector<int> clique =  getNeighborhood(gid, g, empty);
			int cliqueSize = clique.size();
			cout << "Size of clique for input node is "<<cliqueSize<<endl;
			break;
		}
		case 3:{
			int id1 = getGraphIndexFromGlobal(g,argv[2]);
			int id2 = getGraphIndexFromGlobal(g,argv[3]);
			vector<int> path = dijkstraPath(id1,id2,g);
			cout << "Path between "<< argv[2]<<" and "<<argv[3]<<" is ";
			for(int i=0;i<path.size();i++){
				cout<<g.nodeList.at(path.at(i))<<" ";
			}
			cout<<endl;
			break;
		}
		case 2:{
			int id1 = getGraphIndexFromGlobal(g,argv[2]);
			int id2 = getGraphIndexFromGlobal(g,argv[3]);
			vector<int> path = dijkstraPath(id1,id2,g);
			cout << "Size of path between "<< argv[2]<<" and "<<argv[3]<<" is "<<path.size()<<endl;
			break;
		}
		case 4:{
			cout << "Shortest path on graph is "<<1<<" between nodes "<<g.sourceList.at(0)<<" and "<<g.targetList.at(0)<<endl;
			break;
		}
		case 5:{
			int id = getGraphIndexFromGlobal(g,argv[2]);
			int imp = getImportance(g,id);
			cout << "The importance of node "<<argv[2]<<" is "<<imp<<endl;
		}
			break;
		case 6:{
			int id = getGraphIndexFromGlobal(g,argv[2]);
			bool isImp = compareImportance(g,id);
			if(isImp) cout << "Yes, there are friends which are more important than the input node."<<endl;
			else cout << "No, there are not friends which are more important than the input node."<<endl;
			} 
	}
		return 0;
}