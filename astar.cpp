#include "iostream"
#include "vector"
#include "string"
#include "unordered_map"
#include "memory"
#include "cmath"

#define INF 999
#define COMMONDISTANCE 1
#define COLUMNS 10
#define ROWS 10

using namespace std;

class Node;
class Neighbour;

typedef shared_ptr<Node> NodePtr;
typedef shared_ptr<Neighbour> NeighboutPtr;

struct Neighbour
{
    NodePtr node_;
    int distance_;

    Neighbour(NodePtr node)
    {
        node_ = node;
        distance_ = COMMONDISTANCE;
    }
};

class Node
{
    private:
        string name_;
        vector<NeighboutPtr> neighbours_;
        NodePtr comingFrom_;
        int tenativeDistance_;
        bool isStart_;
        bool isDestination_;
        int x_;
        int y_;
        bool isWall_;
        bool isPath_;
        //string drawingSymbol_;

    public:
        Node()
        {
            name_ = "NoName";
            comingFrom_= nullptr;
            tenativeDistance_ = INF;
            isStart_ = false;
            isDestination_ = false;
        }
        Node(string name, int x, int y)
        {
            name_ = name;
            x_ = x;
            y_ = y;
            comingFrom_= nullptr;
            tenativeDistance_ = INF;
            isStart_ = false;
            isDestination_ = false;
            isWall_ = false;
            isPath_ = false;
        }
        ~Node(){}

        NodePtr getNeighbourAtIndex(int index)
        {
            return neighbours_[index]->node_;
        }

        int getDistanceToNeighbourAtIndex(int index)
        {
            return neighbours_[index]->distance_;
        }

        void addNeighbour(NeighboutPtr neighbour)
        {
            neighbours_.push_back(neighbour);
        }

        NodePtr getComingFrom()
        {
            return comingFrom_;
        }

        void setComingFrom(NodePtr node)
        {
            comingFrom_ = node;
        }
        
        int getTenativeDistance()
        {
            return tenativeDistance_;
        }

        void setTenativeDistance(int a)
        {
            tenativeDistance_ = a;
        }

        int getAmountOfNeighbours()
        {
            return neighbours_.size();
        }

        void setIsStart(bool a)
        {
            isStart_ = a;
        }
        
        bool getIsStart()
        {
            return isStart_;
        }

        void setIsDestination(bool a)
        {
            isDestination_ = a;
        }

        bool getIsDestination()
        {
            return isDestination_;
        }

        string getName()
        {
            return name_;
        }

        void setName(string name)
        {
            name_ = name;
        }

        string report()
        {
            string retValue = "";
            retValue += getName() + " " + to_string(getTenativeDistance()) + " " + to_string(int(getIsStart())) + " " + to_string(int(getIsDestination())) + " " /*+ to_string(getAmountOfNeighbours())*/ + "\n";
            return retValue;
        }

        void setX(int x)
        {
            x_ = x;
        }

        int getX()
        {
            return x_;
        }

        void setY(int y)
        {
            y_ = y;
        }

        int getY()
        {
            return y_;
        }   

        bool getIsWall()
        {
            return isWall_;
        }     

        void setIsWall(bool a)
        {
            isWall_ = a;
        }

        void setIsPath(bool a)
        {
            isPath_ = a;
        }

        bool getIsPath()
        {
            return isPath_;
        }

        void draw()
        {
            if(isWall_)
            {
                cout<<" ㅁ ";
            }
            else if(isStart_)
            {
                cout<<" ㅅ ";
            }
            else if(isDestination_)
            {
                cout<<" ㅈ ";
            }
            else if(isPath_)
            {
                cout<<" ㅍ ";
            }
            else
            {
                cout<<" ㅇ ";
            }
        }
};

void makeStart(NodePtr node)
{
    node->setIsStart(true);
    node->setTenativeDistance(0);
}

void makeDestination(NodePtr node)
{
    node->setIsDestination(true);
}

NodePtr returnNodeWithLowestTenativeDistance(vector<NodePtr> &queue)
{
    auto retValue = make_shared<Node>();

    int position = 0;

    if(queue.size() == 1)
    {
        retValue = queue[0];
    }
    else
    {
        for(int i = 0 ; i < queue.size() ; i++)
        {
            if(queue[i]->getTenativeDistance() < retValue->getTenativeDistance())
            {
                retValue = queue[i];
                position = i;
            }        
        }
    }
    queue.erase(queue.begin()+position);
    return retValue;
}

int heuristicFunction(NodePtr destination, NodePtr checked)
{
    int retValue = 0;
    retValue = sqrt(pow((destination->getX() - checked->getX()), 2) + pow((destination->getY() - checked->getY()), 2));
    return retValue;

}

void neighbourChecker(NodePtr base, vector<NodePtr> &queue, NodePtr destination)
{
    for(int i = 0 ; i < base->getAmountOfNeighbours() ; i++)
    {
        if(base->getNeighbourAtIndex(i) == base->getComingFrom())
        {
            continue;
        }
        else if(base->getNeighbourAtIndex(i)->getIsWall())
        {
            continue;
        }
        else
        {
            if(base->getNeighbourAtIndex(i)->getTenativeDistance() <= base->getTenativeDistance() + base->getDistanceToNeighbourAtIndex(i))
            {
                continue;
            }
            else
            {
                base->getNeighbourAtIndex(i)->setComingFrom(base);
                base->getNeighbourAtIndex(i)->setTenativeDistance(base->getTenativeDistance() + base->getDistanceToNeighbourAtIndex(i) + heuristicFunction(destination, base->getNeighbourAtIndex(i)));
                queue.push_back(base->getNeighbourAtIndex(i));
            }
        }
    }
}

int main()
{

    vector<NodePtr> queue;

    NodePtr grid[ROWS][COLUMNS];

    for(int i = 0 ; i < ROWS ; i ++)
    {
        for(int j = 0 ; j < COLUMNS ; j++)
        {
            auto new_node = make_shared<Node>(to_string(j)+"_"+to_string(i), j, i);
            grid[i][j] = new_node;
        }
    }

    //corners
    auto cn1 = make_shared<Neighbour>(grid[0][1]);
    grid[0][0]->addNeighbour(cn1);
    auto cn2 = make_shared<Neighbour>(grid[1][0]);
    grid[0][0]->addNeighbour(cn2);
    auto cn3 = make_shared<Neighbour>(grid[0][COLUMNS-2]);
    grid[0][COLUMNS-1]->addNeighbour(cn3);
    auto cn4 = make_shared<Neighbour>(grid[0][COLUMNS-2]);
    grid[0][COLUMNS-1]->addNeighbour(cn4);
    auto cn5 = make_shared<Neighbour>(grid[ROWS-2][0]);
    grid[ROWS-1][0]->addNeighbour(cn5);
    auto cn6 = make_shared<Neighbour>(grid[ROWS-1][1]);
    grid[ROWS-1][0]->addNeighbour(cn6);
    auto cn7 = make_shared<Neighbour>(grid[ROWS-2][COLUMNS-1]);
    grid[ROWS-1][COLUMNS-1]->addNeighbour(cn7);
    auto cn8 = make_shared<Neighbour>(grid[ROWS-1][COLUMNS-2]);
    grid[ROWS-1][COLUMNS-1]->addNeighbour(cn8);

    //borders up
    for(int j = 1 ; j  < COLUMNS - 1 ; j++)
    {
        auto n1 = make_shared<Neighbour>(grid[0][j-1]);
        auto n2 = make_shared<Neighbour>(grid[0][j+1]);
        auto n3 = make_shared<Neighbour>(grid[1][j]);
        grid[0][j]->addNeighbour(n1);
        grid[0][j]->addNeighbour(n2);
        grid[0][j]->addNeighbour(n3);
    }
    //borders right
    for(int i = 1 ; i < ROWS - 1 ; i++)
    {
        auto n1 = make_shared<Neighbour>(grid[i + 1][COLUMNS - 1]);
        auto n2 = make_shared<Neighbour>(grid[i - 1][COLUMNS - 1]);
        auto n3 = make_shared<Neighbour>(grid[i][COLUMNS - 2]);
        grid[i][COLUMNS - 1]->addNeighbour(n1);
        grid[i][COLUMNS - 1]->addNeighbour(n2);
        grid[i][COLUMNS - 1]->addNeighbour(n3);
    }
    //borders down
    for(int j = 1; j < COLUMNS - 1; j++)
    {
        auto n1 = make_shared<Neighbour>(grid[ROWS - 1][j-1]);
        auto n2 = make_shared<Neighbour>(grid[ROWS - 1][j+1]);
        auto n3 = make_shared<Neighbour>(grid[ROWS - 2][j]);
        grid[ROWS - 1][j]->addNeighbour(n1);
        grid[ROWS - 1][j]->addNeighbour(n2);
        grid[ROWS - 1][j]->addNeighbour(n3);
    }
    //borders left
    for(int i = 1; i < ROWS - 1; i++)
    {
        auto n1 = make_shared<Neighbour>(grid[i + 1][0]);
        auto n2 = make_shared<Neighbour>(grid[i - 1][0]);
        auto n3 = make_shared<Neighbour>(grid[i][1]);
        grid[i][0]->addNeighbour(n1);
        grid[i][0]->addNeighbour(n2);
        grid[i][0]->addNeighbour(n3);
    }
    //center
    for(int i = 1; i < ROWS - 1; i++)
    {
        for(int j = 1; j < COLUMNS - 1; j++)
        {
            auto n1 = make_shared<Neighbour>(grid[i][j + 1]);
            auto n2 = make_shared<Neighbour>(grid[i][j - 1]);
            auto n3 = make_shared<Neighbour>(grid[i + 1][j]);
            auto n4 = make_shared<Neighbour>(grid[i - 1][j]);
            grid[i][j]->addNeighbour(n1);
            grid[i][j]->addNeighbour(n2);
            grid[i][j]->addNeighbour(n3);
            grid[i][j]->addNeighbour(n4);
        }
    }
    //create landscape
    //walls
    //first dimension is number of row, second one is column
    grid[0][1]->setIsWall(true);
    grid[1][1]->setIsWall(true);
    grid[2][1]->setIsWall(true);

    grid[6][3]->setIsWall(true);
    grid[7][3]->setIsWall(true);
    grid[8][3]->setIsWall(true);
    grid[9][3]->setIsWall(true);

    grid[3][4]->setIsWall(true);

    grid[3][5]->setIsWall(true);

    grid[2][6]->setIsWall(true);
    grid[3][6]->setIsWall(true);
    grid[4][6]->setIsWall(true);
    grid[6][6]->setIsWall(true);
    grid[7][6]->setIsWall(true);
    grid[8][6]->setIsWall(true);

    grid[6][7]->setIsWall(true);
    grid[8][7]->setIsWall(true);

    grid[0][8]->setIsWall(true);
    grid[1][8]->setIsWall(true);
    grid[4][8]->setIsWall(true);
    grid[6][8]->setIsWall(true);
    grid[8][8]->setIsWall(true);

    //start
    makeStart(grid[0][0]);
    //destination
    makeDestination(grid[7][7]);

    //draw init
    for(int i = 0 ; i < ROWS ; i++)
    {
        for(int j = 0 ; j < COLUMNS ; j++)
        {
            grid[i][j]->draw();
        }
        cout<<"\n";
    }

    cout<<"\n Solution: \n";

    queue.push_back(grid[0][0]);
    auto current = make_shared<Node>();
    vector<NodePtr> path;
    while(true)
    {
        current = returnNodeWithLowestTenativeDistance(queue);
        if(current->getIsDestination())
        {
            path.push_back(current);
            while(current->getTenativeDistance() != 0)
            {
                path.push_back(current->getComingFrom());
                current = current->getComingFrom();
            }
            break;
        }
        neighbourChecker(current, queue, grid[7][7]);
    }

    //set path
    for(int i = 1 ; i < path.size() - 1 ; i++)
    {
        path[i]->setIsPath(true);
    } 

    //draw solution
    for(int i = 0 ; i < ROWS ; i++)
    {
        for(int j = 0 ; j < COLUMNS ; j++)
        {
            grid[i][j]->draw();
        }
        cout<<"\n";
    }
}
