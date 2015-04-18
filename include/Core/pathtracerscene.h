#ifndef PATHTRACERSCENE_H
#define PATHTRACERSCENE_H

/// @class PathTracerScene
/// @date 06/01/15
/// @author Declan Russell
/// @brief A class to manage our OptiX path tracer.
/// @brief This is a singleton class for easy access in other areas of the program

#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_matrix_namespace.h>

#include <QImage>

#include "random.h"
#include "path_tracer.h"
#include "helpers.h"
#include "optixmodel.h"
#include "pinholecamera.h"

using namespace optix;

class PathTracerScene
{
protected:
    typedef optix::float3 float3;
    typedef optix::float4 float4;
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns an instance of our singleton class
    //----------------------------------------------------------------------------------------------------------------------
    static PathTracerScene *getInstance();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  dtor
    //----------------------------------------------------------------------------------------------------------------------
    ~PathTracerScene();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief accessor to our context
    //----------------------------------------------------------------------------------------------------------------------
    inline optix::Context &getContext(){return m_context;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief initialise our class
    //----------------------------------------------------------------------------------------------------------------------
    void init();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our trace function that launches our Optix context
    //----------------------------------------------------------------------------------------------------------------------
    void trace();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns our output buffer
    //----------------------------------------------------------------------------------------------------------------------
    inline optix::Buffer getOutputBuffer(){return m_context["output_buffer"]->getBuffer();}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief set the square root number of samples
    //----------------------------------------------------------------------------------------------------------------------
    inline void setNumSamples( unsigned int sns ){ m_sqrt_num_samples= sns; }
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a mutator to set the width and height of our scene
    //----------------------------------------------------------------------------------------------------------------------
    inline void setSize(unsigned int _width, unsigned int _height){m_width = _width; m_height = _height;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief an accessor to the width of our scene
    //----------------------------------------------------------------------------------------------------------------------
    inline unsigned int getWidth(){return m_width;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief an accessor to the height of our scene
    //----------------------------------------------------------------------------------------------------------------------
    inline unsigned int getHeight(){return m_height;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief resize our scene
    //----------------------------------------------------------------------------------------------------------------------
    void resize(int _width, int _height);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief mutator for our device pixel ratio
    //----------------------------------------------------------------------------------------------------------------------
    inline void setDevicePixelRatio(int _ratio = 1){m_devicePixelRatio = _ratio;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief signals if our camera has changed
    //----------------------------------------------------------------------------------------------------------------------
    inline void signalCameraChanged(){m_cameraChanged=true;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief updates our camera the instance of our camera
    //----------------------------------------------------------------------------------------------------------------------
    void updateCamera();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief accesor to our scenes camera
    /// @return our camera
    //----------------------------------------------------------------------------------------------------------------------
    inline PinholeCamera* getCamera(){return m_camera;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief function to import a mesh to our scene
    /// @param _path - the path to our mesh
    /// @param _id - the id of our mesh
    //----------------------------------------------------------------------------------------------------------------------
    void importMesh(std::string _id, std::string _path);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief transforms a model in our scene
    /// @param _id - the id of our model
    /// @param _trans - the transform we wish to apply to our model
    //----------------------------------------------------------------------------------------------------------------------
    void transformModel(std::string _id, glm::mat4 _trans);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief sets the material of a model in our scene
    /// @param _id - the id of the model in our map
    /// @param _mat - the material that we wish to apply to the object
    //----------------------------------------------------------------------------------------------------------------------
    void setModelMaterial(std::string _id, Material &_mat);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief render scene to image file
    //----------------------------------------------------------------------------------------------------------------------
    QImage saveImage();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Sets a new environment map
    //----------------------------------------------------------------------------------------------------------------------
    void setEnvironmentMap(std::string _environmentMap);
    //----------------------------------------------------------------------------------------------------------------------

protected:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief An instance of the optix engine
    //----------------------------------------------------------------------------------------------------------------------
    optix::Context m_context;
    //----------------------------------------------------------------------------------------------------------------------
private:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our default constructor
    /// @brief these must be private so that we dont have copies of our singleton class created
    //----------------------------------------------------------------------------------------------------------------------
    PathTracerScene();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief remove any access to copy constructor as we dont want copies of our singleton class
    //----------------------------------------------------------------------------------------------------------------------
    PathTracerScene(PathTracerScene const&){}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief remove access to our assignment operator as we dont want copies of our signton class
    //----------------------------------------------------------------------------------------------------------------------
    PathTracerScene& operator=(PathTracerScene const&){}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a pointer to our instance of our singleton class
    //----------------------------------------------------------------------------------------------------------------------
    static PathTracerScene* m_instance;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the top group of our scene
    //----------------------------------------------------------------------------------------------------------------------
    Group m_topGroup;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief an array of all our mesh's
    //----------------------------------------------------------------------------------------------------------------------
    std::map<std::string,OptiXModel*> m_meshArray;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a bool to notify us if the camera has changed and we need to update engine camera paramiters
    //----------------------------------------------------------------------------------------------------------------------
    bool m_cameraChanged;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the camera of our scene
    //----------------------------------------------------------------------------------------------------------------------
    PinholeCamera *m_camera;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our imported model
    //----------------------------------------------------------------------------------------------------------------------
    OptiXModel *m_model;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our imported model
    //----------------------------------------------------------------------------------------------------------------------
    OptiXModel *m_model2;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our imported model
    //----------------------------------------------------------------------------------------------------------------------
    OptiXModel *m_model3;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our device pixel ratio default set to 1 but for mac this could be different
    //----------------------------------------------------------------------------------------------------------------------
    int m_devicePixelRatio;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a function to create the geomtry in our scene, probably not going to be around for long
    //----------------------------------------------------------------------------------------------------------------------
    void createGeometry();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Returns a geometry instance of a parralelogram
    //----------------------------------------------------------------------------------------------------------------------
    optix::GeometryInstance createParallelogram( const float3& anchor, const float3& offset1, const float3& offset2);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns an instance of a parralelogram with an emissive material
    //----------------------------------------------------------------------------------------------------------------------
    optix::GeometryInstance createLightParallelogram( const float3& anchor, const float3& offset1, const float3& offset2, int lgt_instance = -1);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Returns a geometry instance of a sphere
    //----------------------------------------------------------------------------------------------------------------------
    optix::GeometryInstance createSphere( const float4& sphereLoc);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief creates a material and adds it to our geomtry instance
    //----------------------------------------------------------------------------------------------------------------------
    void setMaterial( optix::GeometryInstance& gi, optix::Material material, const std::string& color_name, const float3& color);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our bounding box program
    /// @todo probably doesnt need to be a member but we'll get rid of that later
    //----------------------------------------------------------------------------------------------------------------------
    optix::Program m_pgram_bounding_box;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our intersection program
    /// @todo probably doesn't need to be a member but we'll get rid of that later
    //----------------------------------------------------------------------------------------------------------------------
    optix::Program m_pgram_intersection;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our sphere bounding box program
    //----------------------------------------------------------------------------------------------------------------------
    optix::Program m_pgram_sphereIntersection;
    //----------------------------------------------------------------------------------------------------------------------
    ///  @brief our sphere intersection program
    //----------------------------------------------------------------------------------------------------------------------
    optix::Program m_pgram_bounding_sphere;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the depth of our ray?
    //----------------------------------------------------------------------------------------------------------------------
    unsigned int m_rr_begin_depth;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the sqrt of the number of samples we want
    /// @brief sqrt's are expensive GPU so lets do it on the CPU
    //----------------------------------------------------------------------------------------------------------------------
    unsigned int m_sqrt_num_samples;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief current frame number
    //----------------------------------------------------------------------------------------------------------------------
    unsigned int m_frame;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our sampling strategy, dont think that we need this
    //----------------------------------------------------------------------------------------------------------------------
    unsigned int m_sampling_strategy;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the width of our scene
    //----------------------------------------------------------------------------------------------------------------------
    unsigned int m_width;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the height of our scene
    //----------------------------------------------------------------------------------------------------------------------
    unsigned int m_height;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our output buffer
    //----------------------------------------------------------------------------------------------------------------------
    optix::Buffer m_outputBuffer;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief out lights buffer
    //----------------------------------------------------------------------------------------------------------------------
    optix::Buffer m_lightBuffer;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Our map texture sample
    //----------------------------------------------------------------------------------------------------------------------
    optix::TextureSampler m_mapTexSample;
    //----------------------------------------------------------------------------------------------------------------------
    optix::TextureSampler m_enviSampler;
};

#endif // PATHTRACERSCENE_H
