#include <iostream>
#include <fstream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"
#include <iomanip>
#include <list>

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;

int main()
{
    PolygonalMesh mesh;

    if(!ImportMesh(mesh))
    {
        cerr << "file not found" << endl;
        return 1;
    }
    
    Gedim::UCDUtilities utilities;
    {
        vector<Gedim::UCDProperty<double>> cell0Ds_properties(1);

        cell0Ds_properties[0].Label = "Marker";
        cell0Ds_properties[0].UnitLabel = "-";
        cell0Ds_properties[0].NumComponents = 1;

        vector<double> cell0Ds_marker(mesh.NumCell0Ds, 0.0);
        for(const auto &m : mesh.MarkerCell0Ds)
            for(const unsigned int id: m.second)
                cell0Ds_marker.at(id) = m.first;

        cell0Ds_properties[0].Data = cell0Ds_marker.data();

        utilities.ExportPoints("./Cell0Ds.inp",
                               mesh.Cell0DsCoordinates,
                               cell0Ds_properties);
    }

    {

        vector<Gedim::UCDProperty<double>> cell1Ds_properties(1);

        cell1Ds_properties[0].Label = "Marker";
        cell1Ds_properties[0].UnitLabel = "-";
        cell1Ds_properties[0].NumComponents = 1;

        vector<double> cell1Ds_marker(mesh.NumCell1Ds, 0.0);
        for(const auto &m : mesh.MarkerCell1Ds)
            for(const unsigned int id: m.second)
                cell1Ds_marker.at(id) = m.first;

        cell1Ds_properties[0].Data = cell1Ds_marker.data();

        utilities.ExportSegments("./Cell1Ds.inp",
                                 mesh.Cell0DsCoordinates,
                                 mesh.Cell1DsExtrema,
                                 {},
                                 cell1Ds_properties);
    }
     
     //verifica marker
     cout << "Verifica markers Cell0Ds" << endl;
     for (const auto& coppia : mesh.MarkerCell0Ds) 
     {
        int marker = coppia.first;
        list<unsigned int> listaId = coppia.second;
        
        cout << "Id | Marker" << endl;
        
        for(list<unsigned int>::iterator it = listaId.begin(); it != listaId.end(); it++)
        {
	        cout << *it << "    " << marker << endl;
        }
        
        cout << endl;
    }
    
     cout << "Verifica markers Cell1Ds" << endl;
     for (const auto& coppia : mesh.MarkerCell1Ds) 
     {
        int marker = coppia.first;
        list<unsigned int> listaId = coppia.second;
        
        cout << "Id | Marker" << endl;
        
        for(list<unsigned int>::iterator it = listaId.begin(); it != listaId.end(); it++)
        {
	        cout << *it << "    " << marker << endl;
        }
    }
    
    
    //edges non-zero length
    for (unsigned int i = 0; i < mesh.Cell1DsExtrema.cols(); i++) 
    {
	    int length = mesh.Cell1DsExtrema(1, i) - mesh.Cell1DsExtrema(0, i);
    if (abs(length) < 1e-16) 
    {
        cout << "Edge " << mesh.Cell1DsId[i] << " is zero" << endl;
        }
        
    }
    
    //non-zero area
    for (unsigned int i = 0; i < mesh.Cell2DsVertices.size(); i++)
    {
	    double area = 0.0;
	    vector<double> coordX;
	    vector<double> coordY;
	    for (unsigned int j = 0; j < mesh.Cell2DsVertices[i].size(); j++)
	   {
		   int nodo = mesh.Cell2DsVertices[i][j];
		   double X = mesh.Cell0DsCoordinates(0, nodo);
		   double Y = mesh.Cell0DsCoordinates(1, nodo);
		   coordX.push_back(X);
		   coordY.push_back(Y);
	   }
	   
	   unsigned int n = coordX.size();
	   for(unsigned int k = 0; k < n; k++)
	   {
		   unsigned int next = (k+1) % n;
		   area += coordX[k]*coordY[next] - coordX[next]*coordY[k];
	   }
	   area = area * 0.5;
	   
	   if(abs(area) < 1e-16)
	   {
		   cout << "Poligono " << mesh.Cell2DsId[i] << " has area zero" << endl; 
	   }
    }


    return 0;
}
