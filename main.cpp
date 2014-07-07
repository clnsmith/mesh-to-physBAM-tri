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
    if (argc < 3) {
        LOG::cerr<<"Usage: " << (argc > 0 ? "obj2tri" : argv[0]) << " infile outfile"<<std::endl;
        return 1;
    }
    typedef float T;typedef float RW;
    RW rw=RW();STREAM_TYPE stream_type(rw);

    Initialize_Read_Write_Structures();

    LOG::Initialize_Logging();

    TRIANGULATED_SURFACE<T>* triangulated_surface=TRIANGULATED_SURFACE<T>::Create();

    int vertCount = 0;
    int faceCount = 0;
    std::istream* input=FILE_UTILITIES::Safe_Open_Input(argv[1], false);
    while(!input->eof()){
        std::string token;
        *input>>token;
        if(token == "v"){
            VECTOR<T,3> vertex;
            *input>>vertex.x>>vertex.y>>vertex.z;
            LOG::cout<<"Read vertex "<<vertex<<std::endl;
            int p=triangulated_surface->particles.array_collection->Add_Element();
            triangulated_surface->particles.X(p)=vertex;
            vertCount++;
            continue;
        } else if (token == "f"){
            VECTOR<int,3> face;
            *input>>face.x>>face.y>>face.z;
            LOG::cout<<"Read face "<<face<<std::endl;
            triangulated_surface->mesh.elements.Append(face);
            faceCount++;
            continue;
        } else {
            if (!input->eof()) {
                LOG::cerr<<"Warning: unrecognized token "<<token<<" !"<<std::endl;
            }
        }
    }

    triangulated_surface->Update_Number_Nodes();
    FILE_UTILITIES::Write_To_File(stream_type, argv[2], *triangulated_surface);

    if (vertCount == 0) {
        LOG::cout<<"Warning: no vertices read. Please verify input file.\n"<<std::endl;
    }
    if (faceCount == 0) {
        LOG::cout<<"Warning: no faces read. Please verify input file.\n"<<std::endl;
    }

    LOG::cout<<"Done reading file."<<std::endl;
    LOG::cout<<"Vertices read: "<<vertCount<<std::endl;
    LOG::cout<<"Faces read: "<<faceCount<<std::endl;

    delete input;

    LOG::Finish_Logging();

    return 0;
} 
