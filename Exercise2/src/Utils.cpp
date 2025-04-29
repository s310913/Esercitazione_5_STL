#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;

namespace PolygonalLibrary
{
bool ImportMesh(PolygonalMesh& mesh)
{
	if(!ImportCell0Ds(mesh))
        return false;

    if(!ImportCell1Ds(mesh))
        return false;

    if(!ImportCell2Ds(mesh))
        return false;

    return true;

}

bool ImportCell0Ds(PolygonalMesh& mesh)
{
	ifstream file("./Cell0Ds.csv");
	
	if(file.fail())
    	return false;
    	
    list<string> listLines;

    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front();

    mesh.NumCell0Ds = listLines.size();

    if (mesh.NumCell0Ds == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DsId.reserve(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds);
    	
    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        string val;
        getline(converter, val, ';'); 
        id = stoi(val);
        mesh.Cell0DsId.push_back(id);
        
        getline(converter, val, ';');
        marker = stoi(val);
        
        getline(converter, val, ';');
        mesh.Cell0DsCoordinates(0, id) = stod(val);
        
        getline(converter, val, ';');
        mesh.Cell0DsCoordinates(1, id) = stod(val);
        
        if(marker != 0)
        {
	        const auto it = mesh.MarkerCell0Ds.find(marker);
            if(it == mesh.MarkerCell0Ds.end())
            {
                mesh.MarkerCell0Ds.insert({marker, {id}});
            }
            else
            {
                it->second.push_back(id);
        	}
        }

    }
    return true;
}
    
bool ImportCell1Ds(PolygonalMesh& mesh)
{
	ifstream file("./Cell1Ds.csv");
	if(file.fail())
    	return false;
    	
        list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front();

    mesh.NumCell1Ds = listLines.size();

    if (mesh.NumCell1Ds == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
    mesh.Cell1DsExtrema = Eigen::MatrixXi(2, mesh.NumCell1Ds);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;

        string val;
        getline(converter, val, ';'); 
        id = stoi(val);
        mesh.Cell1DsId.push_back(id);
        
        getline(converter, val, ';');
        marker = stoi(val);
        
        getline(converter, val, ';');
        mesh.Cell1DsExtrema(0, id) = stoi(val);
        
        getline(converter, val, ';');
        mesh.Cell1DsExtrema(1, id) = stoi(val);
        
        if(marker != 0)
        {
            const auto it = mesh.MarkerCell1Ds.find(marker);
            if(it == mesh.MarkerCell1Ds.end())
            {
                mesh.MarkerCell1Ds.insert({marker, {id}});
            }
            else
            {
                it->second.push_back(id);
            }
        }
    }
    
    return true;
}

bool ImportCell2Ds(PolygonalMesh& mesh)
{
	ifstream file("./Cell2Ds.csv");
	if(file.fail())
    	return false;
    	
    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front();

    mesh.NumCell2Ds = listLines.size();

    if (mesh.NumCell2Ds == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsVertices.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsEdges.reserve(mesh.NumCell2Ds);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int nV;
        unsigned int nE;
        vector<unsigned int> vertices;
        vector<unsigned int> edges;
        
        string val;
        getline(converter, val, ';'); 
        id = stoi(val);
        getline(converter, val, ';'); 
        
        getline(converter, val, ';'); 
        nV = stoi(val);

        for(unsigned int i = 0; i < nV; i++)
        {
	        getline(converter, val, ';');
            unsigned int vId = stoi(val);
            vertices.push_back(vId);
            }
            
        getline(converter, val, ';'); 
        nE = stoi(val);

        for(unsigned int i = 0; i < nE; i++)
        {
	        getline(converter, val, ';');
            unsigned int eId = stoi(val);
            edges.push_back(eId);
            }

        mesh.Cell2DsId.push_back(id);
        mesh.Cell2DsVertices.push_back(vertices);
        mesh.Cell2DsEdges.push_back(edges);
    }

    return true;
}
}