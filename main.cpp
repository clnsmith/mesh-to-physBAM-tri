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

    std::istream* input=FILE_UTILITIES::Safe_Open_Input("femur.asc",false);
    while(1){
    std::string token;
    *input>>token;
    float a, b, c, d, e, f;

   	 if(*input>>a>>b>>c>>d>>e>>f){
            VECTOR<T,3> vertex;
            *input>>vertex.x>>vertex.y>>vertex.z;
            LOG::cout<<"Read vertex "<<vertex<<std::endl;
            int p=triangulated_surface->particles.array_collection->Add_Element();
            triangulated_surface->particles.X(p)=vertex;
            continue;
        }
        /*if(token == "f"){
            VECTOR<int,3> face;
            *input>>face.x>>face.y>>face.z;
            LOG::cout<<"Read face "<<face<<std::endl;
            triangulated_surface->mesh.elements.Append(face);
            continue;
        }*/
        break;
    }
    triangulated_surface->Update_Number_Nodes();
    FILE_UTILITIES::Write_To_File(stream_type,"leg.tri",*triangulated_surface);

    delete input;

    LOG::Finish_Logging();

    return 0;
} 
