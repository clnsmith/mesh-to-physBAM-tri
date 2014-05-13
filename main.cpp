// Copyright (c) 2011, Eftychios Sifakis.
// Distributed under the FreeBSD license (see license.txt)

#include <PhysBAM_Geometry/Topology_Based_Geometry/TRIANGULATED_SURFACE.h>
#include <PhysBAM_Geometry/Read_Write/Geometry/READ_WRITE_TRIANGULATED_SURFACE.h>
#include <PhysBAM_Tools/Read_Write/Utilities/FILE_UTILITIES.h>
#include <PhysBAM_Tools/Log/LOG.h>

using namespace PhysBAM;
using namespace FILE_UTILITIES;

int main(int argc,char* argv[])
{
    typedef float T;typedef float RW;
    RW rw=RW();STREAM_TYPE stream_type(rw);

    Initialize_Read_Write_Structures();

    LOG::Initialize_Logging();

    TRIANGULATED_SURFACE<T>* triangulated_surface=TRIANGULATED_SURFACE<T>::Create();

    //Import File
    std::istream* input=FILE_UTILITIES::Safe_Open_Input("femur.asc",false);

    //Skip First Line
    std::string dummyLine;
    getline(*input, dummyLine);

    //Read Second Line to get # of vertices and triangles
    int numVertex=0, numTriangle=0;
    *input>>numVertex>>numTriangle;
    LOG::cout<<numVertex<<numTriangle<<std::endl;
    
    //Read Third Line to get bounding box dimensions (not needed for physBAM)
    float xMin, xMax, yMin, yMax, zMin, zMax;
    *input>>xMin>>xMax>>yMin>>yMax>>zMin>>zMax;
    LOG::cout<<"Bounding Box "<<xMin<<' '<<xMax<<' '<<yMin<<' '<<yMax<<' '<<zMin<<' '<<zMax<<std::endl;

    //Read verticies and assign them to physBAM particles
    VECTOR<T,3> vertex;
    VECTOR<T,3> normal; //used as a place holder, physBAM doesn't need these

    for(int i=0; i < numVertex; i++){
        *input>>vertex.x>>vertex.y>>vertex.z>>normal.x>>normal.y>>normal.z; 
        LOG::cout<<"Read vertex "<<vertex<<std::endl;
        int p=triangulated_surface->particles.array_collection->Add_Element();
        triangulated_surface->particles.X(p)=vertex;
    }


    //Read triangles and assign them to physBAM mesh.elements
    float dimension; //used as place holder, describes triangle mesh in .asc file
    VECTOR<int,3> triangle;

    for(int j=0; j < numTriangle; j++){
        *input>>dimension>>triangle.x>>triangle.y>>triangle.z;
            LOG::cout<<"Read triangle "<<triangle<<std::endl;
            triangulated_surface->mesh.elements.Append(triangle);
    }        
    
    triangulated_surface->Update_Number_Nodes();
    FILE_UTILITIES::Write_To_File(stream_type,"femur.tri",*triangulated_surface);

    delete input;

    LOG::Finish_Logging();

    return 0;
} 
