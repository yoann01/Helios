#include "NodeGraph/OSLNodesEditor.h"
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include <QDate>
#include <QGraphicsScene>
#include <algorithm>
#include "NodeGraph/qneconnection.h"
#include "NodeGraph/OSLAbstractVarBlock.h"
#include "Core/pathtracerscene.h"

#define stringify( x ) stringify_literal( x )
#define stringify_literal( x ) # x

//------------------------------------------------------------------------------------------------------------------------------------
OSLNodesEditor::OSLNodesEditor(QObject *parent) :
    QNodesEditor(parent)
{
    //set our material destination
    m_optixMatDest = "OptixMaterials/tempMat.cu";
    m_materialName = "tempMat";

    int v;
    cudaRuntimeGetVersion(&v);
    int vMajor = floor(v/1000);
    int vMinor = (v - (floor(v/1000)*1000))/10;
    m_cudaVersion= std::to_string(vMajor)+"."+std::to_string(vMinor);
    std::cout<<"Compiling with CUDA Version "<<m_cudaVersion<<std::endl;

    m_cudaDir = stringify(CUDA_DIRECTORY);
    m_optixDir = stringify(OPTIX_DIRECTORY);
    std::cout<<"Cuda Dir"<<m_cudaDir<<std::endl;
    std::cout<<"Optix Dir"<<m_optixDir<<std::endl;

}
//------------------------------------------------------------------------------------------------------------------------------------
QString OSLNodesEditor::portTypeToString(QNEPort::variableType _type)
{
    switch(_type)
    {
        case(QNEPort::TypeFloat): return QString("float"); break;
        case(QNEPort::TypeInt): return QString("int"); break;
        case(QNEPort::TypeNormal): return QString("float3"); break;
        case(QNEPort::TypeColour): return QString("float3"); break;
        case(QNEPort::TypePoint): return QString("float3"); break;
        case(QNEPort::TypeMatrix): return QString("float*"); break;
        default: break;
    }
    return "";
}
//------------------------------------------------------------------------------------------------------------------------------------
QNEBlock *OSLNodesEditor::getLastBlock()
{
    //iterate through our graphics items in our scene
    foreach(QGraphicsItem *item, getScene()->items())
    {
        //if its a shader block then lets see if its got Ci in it
        if (item->type() == OSLShaderBlock::Type)
        {
            //iterate through all our ports of the block
            QVector<QNEPort*> blockPorts = ((OSLShaderBlock*)item)->ports();
            foreach(QNEPort* port,blockPorts)
            {
                //if the port is an output and its name is Ci then we have a last block
                if(port->isOutput() && (port->getName() == "Ci"))
                {
                    return (QNEBlock*)item;
                }
            }
        }
    }
    //if we get to here then we have not found the last block
    return 0;

}
//------------------------------------------------------------------------------------------------------------------------------------
void OSLNodesEditor::evaluateBlock(QNEBlock *_block, std::vector<QNEBlock *> &_blockVector)
{
    //check if our block has already been evaluated
    std::vector<QNEBlock*>::iterator it = std::find(_blockVector.begin(),_blockVector.end(),_block);
    if(it != _blockVector.end())
    {
        return;
    }
    else
    {
        //if its not been evaluated check all of its imputs to see if they have been evaluated
        QVector<QNEPort*> blockPorts = ((OSLShaderBlock*)_block)->ports();
        foreach(QNEPort* port,blockPorts){
            //if the port is an input check to see if its connected to another shader
            if(!port->isOutput())
            {
                //get all the connections of our port
                QVector<QNEConnection*> connections = port->connections();
                foreach(QNEConnection* con, connections){
                    //check which port of the connection is the output of the previous node
                    if(con->port1()->isOutput())
                    {
                        evaluateBlock(con->port1()->block(),_blockVector);
                    }
                    else
                    {
                        evaluateBlock(con->port2()->block(),_blockVector);
                    }
                }
            }
        }
        _blockVector.push_back(_block);
        return;
    }
}

//------------------------------------------------------------------------------------------------------------------------------------
std::string OSLNodesEditor::compileMaterial(optix::Material &_mat)
{
    QFileInfo fileInfo(m_optixMatDest.c_str());
    if(!QDir(fileInfo.absoluteDir()).exists())
    {
        QDir().mkdir(fileInfo.absolutePath());
    }
    QFile myfile(m_optixMatDest.c_str());

    if(myfile.open(QIODevice::WriteOnly))
    {

        percentCompiled(0);

        QTextStream stream(&myfile);
        QDate date;
        stream<<"//This file has been automatically generated by Declan Russell's xX_OSL Node Editor Hyper_Xx #NoScopez on the "<<date.currentDate().toString("dd.MM.yyyy")<<endl;

        //add our includes and namespaces
        stream<<"#include <optix.h>"<<endl;
        stream<<"#include <optixu/optixu_math_namespace.h>"<<endl;
        stream<<"#include <optixu/optixu_matrix_namespace.h>"<<endl;
        stream<<"#include \"Core/path_tracer.h\""<<endl;
        stream<<"#include \"Core/random.h\""<<endl;
        stream<<"#include \"BRDFUtils.h\""<<endl;
        stream<<"#include \"helpers.h\""<<endl;
        stream<<"using namespace optix;"<<endl;
        stream<<"\n\n";

        //Declare our path tracer variables
        stream<<"rtDeclareVariable(float,         scene_epsilon, , );\n"<<endl;
        stream<<"rtDeclareVariable(rtObject,      top_object, , );\n"<<endl;
        stream<<"rtBuffer<ParallelogramLight>     lights;\n"<<endl;
        stream<<"rtDeclareVariable(unsigned int,  maxDepth      , , );"<<endl;
        stream<<"rtDeclareVariable(unsigned int,  pathtrace_shadow_ray_type, , );\n"<<endl;
        stream<<"// Camera Variables\n"<<endl;
        stream<<"rtDeclareVariable(float3,        eye, , );"<<endl;
        stream<<"// Geometry Variables "<<endl;
        stream<<"rtDeclareVariable(float3, geometric_normal, attribute geometric_normal, ); "<<endl;
        stream<<"rtDeclareVariable(float3, shading_normal,   attribute shading_normal, );"<<endl;
        stream<<"rtDeclareVariable(float3, texcoord, attribute texcoord, );"<<endl;
        stream<<"rtDeclareVariable(float3, tangent, attribute tangent, );"<<endl;
        stream<<"rtDeclareVariable(float3, bitangent, attribute bitangent, );"<<endl;
        stream<<"// Our current ray and payload variables"<<endl;
        stream<<"rtDeclareVariable(optix::Ray, ray,          rtCurrentRay, );"<<endl;
        stream<<"rtDeclareVariable(float,      t_hit,        rtIntersectionDistance, );"<<endl;
        stream<<"rtDeclareVariable(PerRayData_pathtrace, current_prd, rtPayload, );"<<endl;
        stream<<"\n\n";

        percentCompiled(10);
        stream<<"//Dynamic Input Variables"<<endl;
        //run through all our ports to find if we have any input variables to declare
        foreach(QGraphicsItem *item, getScene()->items()){
            if (item->type() == QNEPort::Type)
            {

                //check to see if we have an input port
                if(!((QNEPort*) item)->isOutput())
                {
                    QVector<QNEConnection*> connection = ((QNEPort*) item)->connections();
                    if(connection.size()>1)
                    {
                        return "Port " + ((QNEPort*) item)->getName().toStdString() + " has multiple inputs";
                    }
                    if(connection.size()>0)
                    {
                        //make sure our input is not comming from another shader block
                        if(connection[0]->port1()->isOutput() && connection[0]->port1()->block()->type()==OSLShaderBlock::Type) continue;
                        if(connection[0]->port2()->isOutput() && connection[0]->port2()->block()->type()==OSLShaderBlock::Type) continue;
                    }
                    //if its a texture we need to write something different
                    if(((QNEPort*) item)->getVaribleType()==QNEPort::TypeString)
                    {
                        OSLShaderBlock *block = (OSLShaderBlock*) ((QNEPort*) item)->block();
                        stream<<"rtTextureSampler<float4,2> "<<block->getBlockName().c_str()<<((QNEPort*) item)->getName()<<";"<<endl;
                    }
                    else
                    {
                        //Convert our input type into a string
                        QString type = portTypeToString(((QNEPort*) item)->getVaribleType());
                        //if we have a type and our port belongs to a node that is not a shader node
                        //lets add it to our material variables
                        if(type.size()>0)
                        {
                            //node: if multiple shaders have the same variable name then they will clash in the program
                            //to solve this we put the shader name on the start of the variable
                            OSLShaderBlock *block = (OSLShaderBlock*) ((QNEPort*) item)->block();
                            stream<<"rtDeclareVariable("<<type<<","<<block->getBlockName().c_str()<<((QNEPort*) item)->getName()<<",,);"<<endl;
                        }
                    }
                }
            }
        }
        stream<<"\n\n";
        percentCompiled(30);
        //our BRDF functions
        QFile brdfsLib("brdfs/brdfs");
        if(brdfsLib.open(QIODevice::ReadOnly))
        {
            QTextStream in(&brdfsLib);
            while(!in.atEnd())
            {
                stream<<in.readLine()<<endl;
            }
            brdfsLib.close();
        }
        percentCompiled(35);

        //add all the device functions that we need in our material program
        stream<<"//Our OSL device functions"<<endl;
        foreach(QGraphicsItem *item, getScene()->items())
        {
            if (item->type() == OSLShaderBlock::Type)
            {
                stream<<((OSLShaderBlock*)item)->getDevicefunction().c_str()<<endl;
            }
        }
        stream<<"\n\n";

        percentCompiled(40);

        //add our main material program funcion
        stream<<"//-------Main Material Program-----------"<<endl;
        stream<<"RT_PROGRAM void "<<m_materialName.c_str()<<"(){"<<endl;

        stream<<"if (current_prd.depth > maxDepth){"<<endl;
        stream<<"   current_prd.done = true;"<<endl;
        stream<<"   return;"<<endl;
        stream<<"}"<<endl;

        //set up our shader globals, These are the equivilent to globals about our surface in OSL
        stream<<"ShaderGlobals sg;"<<endl;
        stream<<"// Calcualte the shading and geometric normals for use with our OSL shaders"<<endl;
        stream<<"sg.N = normalize( rtTransformNormal(RT_OBJECT_TO_WORLD, shading_normal));"<<endl;
        stream<<"sg.Ng = normalize( rtTransformNormal( RT_OBJECT_TO_WORLD, geometric_normal ) );"<<endl;
        stream<<"sg.P = ray.origin + t_hit * ray.direction;"<<endl;
        stream<<"sg.I = normalize(eye - sg.P);"<<endl;
        stream<<"// The shading position"<<endl;
        //stream<<"current_prd.origin = ray.origin + t_hit * ray.direction;"<<endl;
        stream<<"// Texture coordinates"<<endl;
        stream<<"sg.u = texcoord.x;"<<endl;
        stream<<"sg.v = texcoord.y;"<<endl;
        stream<<"sg.dPdu = rtTransformNormal( RT_OBJECT_TO_WORLD, tangent);"<<endl;
        stream<<"sg.dPdv = rtTransformNormal( RT_OBJECT_TO_WORLD, bitangent);"<<endl;


        //Retrieve and print out any variables that we need for our kernal functions
        foreach(QGraphicsItem *item, getScene()->items())
        {
            if (item->type() == OSLShaderBlock::Type)
            {
                //get the ports of our block
                QVector<QNEPort*> ports= ((OSLShaderBlock*)item)->ports();
                foreach(QNEPort* p, ports)
                {
                    if(p->isOutput()){
                        stream<<portTypeToString(p->getVaribleType())<<" ";
                        //note: if 2 shaders have the same variable name we need to
                        //distinguish between them so to solve this we will have the
                        //name of the shader concatinated with the varibale name
                        stream<<((OSLShaderBlock*)item)->getBlockName().c_str()<<p->getName();
                        //lets also set its default value
                        std::vector<std::string> initParams = p->getInitParams();
                        if(initParams.size()==0)
                        {
                            stream<<";";
                            continue;
                        }
                        if(initParams.size()==3)
                        {
                            stream<<" = make_float3(";
                            for(int i=0;i<3;i++)
                            {
                                stream<<initParams[i].c_str();
                                if(i!=2) stream<<",";
                            }
                            stream<<");"<<endl;
                        }
                        else
                        {
                            stream<<" = "<<initParams[0].c_str()<<";"<<endl;
                        }
                    }
                }
            }
        }

        stream<<"\n\n";

        percentCompiled(60);

        //get our last shader block;
        QNEBlock *lastBlock = getLastBlock();
        //check that we have found a last block
        if(!lastBlock)
        {
            return "Cannot find end block";
        }
        //evaluate our blocks so we know the order to call our
        //device functions
        std::vector<QNEBlock*> orderedBlocks;
        evaluateBlock(lastBlock,orderedBlocks);

        //write our code
        for(unsigned int i=0;i<orderedBlocks.size();i++)
        {
            if(orderedBlocks[i]->type() == OSLShaderBlock::Type)
            {
                //note the shader name is always the same as the device function we need to call
                stream<<((OSLShaderBlock*)orderedBlocks[i])->getBlockName().c_str()<<"(";
                //put in our shader globals
                stream<<"sg";
                //now lets print out our the paramiters we need in the function
                QVector<QNEPort*> ports = ((OSLShaderBlock*)orderedBlocks[i])->ports();
                for(int i=0; i<ports.size(); i++)
                {
                    QNEPort* p = ports[i];

                    //if our port is not a variable then lets skip
                    if(p->getVaribleType() == QNEPort::TypeVoid)
                    {
                        continue;
                    }
                    //if its our output Ci continue
                    if(p->getName()=="Ci")continue;


                    //if there is nothing connected we just stick in our default paramiter
                    if(p->connections().size()==0)
                    {
                        if(i!=0)
                        {
                            stream<<",";
                        }
                        stream<<((OSLShaderBlock*)p->block())->getBlockName().c_str();
                        stream<<p->getName();
                    }
                    else
                    {
                        QVector<QNEConnection*> con = p->connections();
                        if(con.size()>1)
                        {
                            return "Error: Input to shader has multiple input connections";
                        }
                        else
                        {
                            if(i!=0)
                            {
                                stream<<",";
                            }
                            //find which port of the connection is the input and print the variable name
                            if(con[0]->port1()->isOutput()){
                                if(con[0]->port1()->block()->type() == OSLShaderBlock::Type){
                                    QNEBlock* b = (QNEBlock*)con[0]->port1()->block();
                                    stream<<b->getBlockName().c_str()<<con[0]->port1()->getName();
                                }
                                else if(con[0]->port1()->block()->type() == OSLAbstractVarBlock::Type){
                                    QNEBlock* b = (QNEBlock*)con[0]->port2()->block();
                                    stream<<b->getBlockName().c_str()<<con[0]->port2()->getName();
                                }
                            }
                            else{
                                if(con[0]->port2()->block()->type() == OSLShaderBlock::Type){
                                    QNEBlock* b = (QNEBlock*)con[0]->port2()->block();
                                    stream<<b->getBlockName().c_str()<<con[0]->port2()->getName();
                                }
                                else if(con[0]->port2()->block()->type() == OSLAbstractVarBlock::Type){
                                    QNEBlock* b = (QNEBlock*)con[0]->port1()->block();
                                    stream<<b->getBlockName().c_str()<<con[0]->port1()->getName();
                                }
                            }
                        }
                    }

                }

                //finish our kernal call
                stream<<");"<<endl;
            }
        }

/*
        //calculate our shadows
        stream<<"// Compute our shadows\n"<<endl;
        stream<<"unsigned int num_lights = lights.size();\n"<<endl;
        stream<<"printf(\" number of lights: %u\", num_lights);"<<endl;
        stream<<"float3 result = make_float3(0.0f);\n"<<endl;

        stream<<"for(int i = 0; i < num_lights; ++i) {\n"<<endl;
        stream<<"    ParallelogramLight light = lights[i];\n"<<endl;
        stream<<"    float z1 = rnd(current_prd.seed);\n"<<endl;
        stream<<"    float z2 = rnd(current_prd.seed);\n"<<endl;
        stream<<"    float3 light_pos = light.corner + light.v1 * z1 + light.v2 * z2; \n"<<endl;
        stream<<"    float Ldist = length(light_pos - sg.P);\n"<<endl;
        stream<<"    float3 L = normalize(light_pos - sg.P);\n"<<endl;
        stream<<"    float nDl = dot( sg.N, L );\n"<<endl;
        stream<<"    float LnDl = dot( light.normal, L );\n"<<endl;
        stream<<"    float A = length(cross(light.v1, light.v2));\n"<<endl;
        stream<<"    // cast shadow ray\n"<<endl;
        stream<<"    if ( nDl > 0.0f && LnDl > 0.0f ) {\n"<<endl;
        stream<<"       PerRayData_pathtrace_shadow shadow_prd;\n"<<endl;
        stream<<"        shadow_prd.inShadow = false;\n"<<endl;
        stream<<"        shadow_prd.type = shadowRay;\n"<<endl;
        stream<<"        Ray shadow_ray = make_Ray( sg.P, L, pathtrace_shadow_ray_type, scene_epsilon, Ldist );\n"<<endl;
        stream<<"        rtTrace(top_object, shadow_ray, shadow_prd);\n"<<endl;

        stream<<"        if(!shadow_prd.inShadow){\n"<<endl;
        stream<<"            float weight=nDl * LnDl * A / (M_PIf*Ldist*Ldist);\n"<<endl;
        stream<<"            result += light.emission * weight;\n"<<endl;
        stream<<"        }\n"<<endl;
        stream<<"    }\n"<<endl;
        stream<<"}\n"<<endl;


        stream<<"current_prd.result = result;"<<endl;
*/

        stream<<"current_prd.done = true;"<<endl;

        //end of our material program
        stream<<"}"<<endl;

        percentCompiled(80);
        //close our file
        myfile.close();

        //Compile our shader program to our material

        //get the path to the file
        std::string path = m_optixMatDest;

        cudaDeviceProp prop;
        cudaGetDeviceProperties(&prop, 0);
        std::string gencodeFlag = " -gencode arch=compute_"+std::to_string(prop.major)+"0,code=sm_"+std::to_string(prop.major)+"0";

        std::string cudaSDKDir;
        cudaSDKDir = m_cudaDir + "/samples";
        std::string includePaths = " -I"+m_optixDir+"/SDK"+" -I"+m_optixDir+"/SDK/sutil"+
                                   " -I"+m_optixDir+"/include"+" -I"+m_cudaDir+"/include"+
                                   " -I"+m_cudaDir+"/common/inc"+" -I"+m_cudaDir+"/../shared/inc"+
                                   " -I./include";
        //std::cout<<"includePaths "<<includePaths<<std::endl;

        std::string libDirs = " -L"+m_cudaDir+"/lib64"+" -L"+m_cudaDir+"/lib"+" -L"+cudaSDKDir+"/common/lib"+
                              " -L"+m_optixDir+"/lib64";
        std::string libs = " -lcudart -loptix -loptixu";
        std::string nvcc = "nvcc ";
#ifdef DARWIN
        std::string nvccFlags =" -m64"+gencodeFlag+" -maxrregcount 20 -w --compiler-options -fno-strict-aliasing -use_fast_math --ptxas-options=-v -ptx";
#else
        std::string nvccFlags =" -m64"+gencodeFlag+" -w --compiler-options -fno-strict-aliasing -use_fast_math --ptxas-options=-v -ptx";
#endif
        QFileInfo file = QString(path.c_str());
        std::string output = "./ptx/"+file.fileName().toStdString()+".ptx";
        //std::cout<<"output "<<output<<std::endl;
        std::string nvccCallString = nvcc+nvccFlags+includePaths+libDirs+libs+" ./"+path+" -o "+output;
        //std::cout<<"calling nvcc with: "<<nvccCallString<<std::endl;

        if(system(nvccCallString.c_str())==NULL){
            optix::Context optiXEngine = PathTracerScene::getInstance()->getContext();
            optix::Program closestHitProgram = optiXEngine->createProgramFromPTXFile(output,m_materialName);
            optix::Program anyHitProgram = optiXEngine->createProgramFromPTXFile( "ptx/path_tracer.cu.ptx", "shadow" );
            _mat->setClosestHitProgram(0,closestHitProgram);
            _mat->setAnyHitProgram(1,anyHitProgram);
            _mat->validate();


            //initialize our input params
            foreach(QGraphicsItem *item, getScene()->items()){
                if (item->type() == QNEPort::Type)
                {

                    //check to see if we have an input port
                    if(!((QNEPort*) item)->isOutput()){
                        QVector<QNEConnection*> connection = ((QNEPort*) item)->connections();
                        if(connection.size()>0){
                            //make sure our input is not comming from another shader block
                            if(connection[0]->port1()->isOutput() && connection[0]->port1()->block()->type()==OSLShaderBlock::Type) continue;
                            if(connection[0]->port2()->isOutput() && connection[0]->port2()->block()->type()==OSLShaderBlock::Type) continue;
                        }
                        //Convert our input type into a string
                        QString type = portTypeToString(((QNEPort*) item)->getVaribleType());
                        //if we have a type and our port belongs to a node that is not a shader node
                        //lets add it to our material variables
                        if(type.size()>0){
                            //node: if multiple shaders have the same variable name then they will clash in the program
                            //to solve this we put the shader name on the start of the variable
                            OSLShaderBlock *block = (OSLShaderBlock*) ((QNEPort*) item)->block();
                            std::string varName = block->getBlockName() + ((QNEPort*) item)->getName().toStdString();
                            std::vector<std::string> initParams = ((QNEPort*) item)->getInitParams();
                            if(((QNEPort*) item)->getVaribleType()==QNEPort::TypeInt){
                                _mat[varName.c_str()]->setInt(std::stoi(initParams[0]));
                            }
                            else if(((QNEPort*) item)->getVaribleType()==QNEPort::TypeFloat){
                                _mat[varName.c_str()]->setFloat(std::stof(initParams[0]));
                            }
                            else{
                                _mat[varName.c_str()]->setFloat(std::stof(initParams[0]),std::stof(initParams[1]),std::stof(initParams[2]));
                            }
                        }
                    }
                }
            }
            //initialize the blocks connected
            foreach(QGraphicsItem *item, getScene()->items()){
                if (item->type() == OSLAbstractVarBlock::Type)
                {
                    ((OSLAbstractVarBlock*)item)->setLinkedVar();
                }
            }

            percentCompiled(100);
            std::cerr<<"Material Compiled!"<<std::endl;

            return "Material Compiled";
        }
        else{
            return "NVCC could not compile OptiX Material. This could be incorrect set up or its our fault (in that case sorry)";
        }
    }
    else{
        return "Cannot create material file in location " + m_optixMatDest;
    }

}
//------------------------------------------------------------------------------------------------------------------------------------
