#include "UI/AbstractMaterialWidget.h"
#include "Core/pathtracerscene.h"
#include "NodeGraph/OSLShaderBlock.h"
#include "NodeGraph/OSLVarFloatBlock.h"
#include "NodeGraph/OSLVarFloatThreeBlock.h"
#include "NodeGraph/OSLVarColorBlock.h"
#include "NodeGraph/OSLVarNormalBlock.h"
#include "NodeGraph/OSLVarPointBlock.h"
#include "NodeGraph/OSLVarIntBlock.h"
#include "NodeGraph/OSLVarImageBlock.h"
#include "Core/MaterialLibrary.h"
#include "UI/MeshWidget.h"
#include <QMenu>
#include <QPoint>
#include <QFileDialog>
#include <QInputDialog>
#include <QFileInfo>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <iostream>
#include <QProgressBar>

//declare our static class instance
AbstractMaterialWidget* AbstractMaterialWidget::m_instance;

AbstractMaterialWidget::AbstractMaterialWidget(QWidget *parent) :
    QDockWidget(parent,Qt::Window)
{
    this->setMinimumHeight(500);
    this->setMinimumWidth(700);
    this->setWindowTitle("OSL Hypershader 3000");
    m_matCreated = false;
    m_matAddedToLib = false;
    m_curNGPath.clear();
    //add our groupbox
    m_widgetGroupBox = new QGroupBox(this);
    this->setWidget(m_widgetGroupBox);
    m_groupBoxLayout = new QGridLayout(m_widgetGroupBox);
    m_widgetGroupBox->setLayout(m_groupBoxLayout);
    //create our graphics view to hold our node interface scene
    m_graphicsView = new QGraphicsView(this);
    //add this widget to our group box
    m_groupBoxLayout->addWidget(m_graphicsView,0,0,1,1);
    //create our node interface scene
    m_nodeInterfaceScene = new QGraphicsScene(this);
    //set our graphics view scene to that of our node interface scene
    m_graphicsView->setScene(m_nodeInterfaceScene);
    //some stuff to make it look nicer
    m_graphicsView->setRenderHint(QPainter::Antialiasing, true);
    //create our node editor, could become useful in the future
    m_nodeEditor = new OSLNodesEditor(this);
    connect(m_nodeEditor,SIGNAL(matChanged()),this,SLOT(signalMatChanged()));
    m_nodeEditor->install(m_nodeInterfaceScene);

    //group box for our buttons
    QGroupBox *toolGrbBox = new QGroupBox(this);
    toolGrbBox->setMaximumWidth(200);
    m_groupBoxLayout->addWidget(toolGrbBox,0,1,1,1);
    QGridLayout *toolLayout = new QGridLayout(toolGrbBox);
    toolGrbBox->setLayout(toolLayout);

    //add a button to to launch the creation of our shader
    QPushButton *createShaderBtn = new QPushButton("Create Shader",toolGrbBox);
    toolLayout->addWidget(createShaderBtn,0,0,1,1);
    connect(createShaderBtn,SIGNAL(clicked()),this,SLOT(createOptixMaterial()));

    QPushButton *newMatBtn = new QPushButton("New Material",toolGrbBox);
    toolLayout->addWidget(newMatBtn,1,0,1,1);
    connect(newMatBtn,SIGNAL(pressed()),this,SLOT(newMaterial()));

    QPushButton *addMatToLibBtn = new QPushButton("Add Material to Library", toolGrbBox);
    toolLayout->addWidget(addMatToLibBtn,2,0,1,1);
    connect(addMatToLibBtn,SIGNAL(pressed()),this,SLOT(addMaterialToLib()));

    QPushButton *loadFromLibBtn = new QPushButton("Load Material from Library",toolGrbBox);
    toolLayout->addWidget(loadFromLibBtn,3,0,1,1);
    connect(loadFromLibBtn,SIGNAL(pressed()),MaterialLibrary::getInstance(),SLOT(loadMatToHyperShader()));

    QPushButton *saveBtn = new QPushButton("Save",toolGrbBox);
    toolLayout->addWidget(saveBtn,4,0,1,1);
    connect(saveBtn,SIGNAL(pressed()),this,SLOT(saveNodeGraph()));

    QPushButton *loadNgBtn = new QPushButton("Import External Node Graph",toolGrbBox);
    toolLayout->addWidget(loadNgBtn,5,0,1,1);
    connect(loadNgBtn,SIGNAL(pressed()),this,SLOT(importNodeGraph()));

    QPushButton *applyMatBtn = new QPushButton("Apply Material",toolGrbBox);
    toolLayout->addWidget(applyMatBtn,6,0,1,1);
    connect(applyMatBtn,SIGNAL(pressed()),this,SLOT(applyMaterialToMesh()));

    //push our buttons together
    QSpacerItem *spacer = new QSpacerItem(1, 1, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    toolLayout->addItem(spacer,toolGrbBox->children().size(),0,1,1);

    //Set up our menu for if you right click in our widget
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showContextMenu(QPoint)));

    //create a material in our context
    m_material = PathTracerScene::getInstance()->getContext()->createMaterial();

}
//------------------------------------------------------------------------------------------------------------------------------------
void AbstractMaterialWidget::compileAndAddToLib(QString _path)
{
    QFileInfo fileInfo(_path);
    std::cerr<<"Loading Material "<<fileInfo.baseName().toStdString()<<" from "<<_path.toStdString()<<std::endl;
    loadNodeGraph(_path);
    std::cerr<<"Compiling Material "<<fileInfo.baseName().toStdString()<<std::endl;
    m_matCreated = false;
    createOptixMaterial();
    if(m_matCreated)
    {
        MaterialLibrary::getInstance()->addMaterialToLibrary(fileInfo.baseName().toStdString(),m_material);
    }
    else
    {
        std::cerr<<"Compilation of material "<<fileInfo.baseName().toStdString()<<" from "<<_path.toStdString()<<" has failed and not been added to the library."<<std::endl;
    }
    newMaterial();
}

//------------------------------------------------------------------------------------------------------------------------------------
AbstractMaterialWidget::~AbstractMaterialWidget()
{
    //remove any objects still left in our scene
    QObjectList sceneItems = m_graphicsView->children();
    for(int i=0;i<sceneItems.size();i++){
        delete sceneItems[i];
    }
}
//------------------------------------------------------------------------------------------------------------------------------------
AbstractMaterialWidget* AbstractMaterialWidget::getInstance(QWidget *parent)
{
    if(m_instance)
    {
        if(parent)
        {
            if(m_instance->parent())
            {
                std::cerr<<"AbstractMaterialWidget already has a parent"<<std::endl;
            }
            else
            {
                m_instance->setParent(parent,Qt::Window);
            }
        }
    }
    else
    {
        m_instance = new AbstractMaterialWidget(parent);
    }
    return m_instance;
}
//------------------------------------------------------------------------------------------------------------------------------------
void AbstractMaterialWidget::createOptixMaterial()
{
    //Progress bar to entertain/distract the user
    QProgressBar progressBar(this);
    progressBar.setRange(0,100);
    progressBar.show();
    connect(m_nodeEditor,SIGNAL(percentCompiled(int)),&progressBar,SLOT(setValue(int)));
    //turn our graph editor into an optix material program
    std::string status = m_nodeEditor->compileMaterial(m_material);
    if(status=="Material Compiled")
    {
        m_matCreated = true;
    }
    else{
        QMessageBox::warning(this,tr("Shader Compilation Failed"),tr(status.c_str()));
        m_matCreated=false;
    }
}
//------------------------------------------------------------------------------------------------------------------------------------
void AbstractMaterialWidget::applyMaterialToMesh()
{
    if(m_matCreated)
    {
        if(m_matAddedToLib)
        {
            MeshWidget::getInstance()->applyOSLMaterial(m_material,m_curMatName);
        }
        else
        {
            if(addMaterialToLib())
            {
                MeshWidget::getInstance()->applyOSLMaterial(m_material,m_curMatName);
            }

        }
    }
    else
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this,"Add Material","No OSL shader created. Would you like to compile it now?",QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            createOptixMaterial();
            applyMaterialToMesh();
        }
        else{
            return;
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------------------
void AbstractMaterialWidget::applyMaterialToMesh(std::string _mesh)
{
    if(m_matCreated)
    {
        std::cerr<<"Adding material "<<m_curMatName<<" to mesh "<<_mesh<<std::endl;
        PathTracerScene::getInstance()->setModelMaterial(_mesh,m_material);
    }
    else
    {
        QMessageBox::warning(this,"Add Material","No OSL shader created. Please Create Material First");
    }
}
//------------------------------------------------------------------------------------------------------------------------------------
void AbstractMaterialWidget::showContextMenu(const QPoint &pos)
{

    QPoint globalPos = this->mapToGlobal(pos);

    //our drop down menu
    QMenu myMenu;
    QAction *addShaderNodeBtn = new QAction(&myMenu);
    addShaderNodeBtn->setText("Add Shader Node");
    addShaderNodeBtn->setData(QVariant(0));
    myMenu.addAction(addShaderNodeBtn);

    QAction *addFloatNodeBtn = new QAction(&myMenu);
    addFloatNodeBtn->setText("Add Float Node");
    addFloatNodeBtn->setData(QVariant(1));
    myMenu.addAction(addFloatNodeBtn);

    QAction *addIntNodeBtn = new QAction(&myMenu);
    addIntNodeBtn->setText("Add Int Node");
    addIntNodeBtn->setData(QVariant(2));
    myMenu.addAction(addIntNodeBtn);

    QAction *addVectorNodeBtn = new QAction(&myMenu);
    addVectorNodeBtn->setText("Add Vector Node");
    addVectorNodeBtn->setData(QVariant(3));
    myMenu.addAction(addVectorNodeBtn);

    QAction *addColorNodeBtn = new QAction(&myMenu);
    addColorNodeBtn->setText("Add Color Node");
    addColorNodeBtn->setData(QVariant(4));
    myMenu.addAction(addColorNodeBtn);

    QAction *addNormalNodeBtn = new QAction(&myMenu);
    addNormalNodeBtn->setText("Add Normal Node");
    addNormalNodeBtn->setData(QVariant(5));
    myMenu.addAction(addNormalNodeBtn);

    QAction *addPointNodeBtn = new QAction(&myMenu);
    addPointNodeBtn->setText("Add Point Node");
    addPointNodeBtn->setData(QVariant(6));
    myMenu.addAction(addPointNodeBtn);

    QAction *addImageNodeBtn = new QAction(&myMenu);
    addImageNodeBtn->setText("Add Image Node");
    addImageNodeBtn->setData(QVariant(7));
    myMenu.addAction(addImageNodeBtn);

    //find out if something has been clicked
    QAction* selectedItem = myMenu.exec(globalPos);

    //Its important to note that we do not need to keep a pointer to these peices
    //of memory as we can simply query our scenes children and delete them from there
    if(selectedItem){
        switch(selectedItem->data().toInt())
        {
        case(0): addShaderNode(); break;
        case(1):
        {
            //create a float block
            OSLVarFloatBlock *f = new OSLVarFloatBlock(m_nodeInterfaceScene, m_material);
            connect(f->m_widgetProxy,SIGNAL(attributeChanged()),this,SLOT(signalMatChanged()));
        }
        break;
        case(2):
        {
            //create an int block
            OSLVarIntBlock *i = new OSLVarIntBlock(m_nodeInterfaceScene, m_material);
            connect(i->m_widgetProxy,SIGNAL(attributeChanged()),this,SLOT(signalMatChanged()));
        }
        break;
        case(3):
        {
            //create a float 3 block
            OSLVarFloatThreeBlock *f = new OSLVarFloatThreeBlock(m_nodeInterfaceScene,m_material);
            connect(f->m_widgetProxy,SIGNAL(attributeChanged()),this,SLOT(signalMatChanged()));
        }
        break;
        case(4):
        {
            //create a color block
            OSLVarColorBlock *c = new OSLVarColorBlock(m_nodeInterfaceScene,m_material);
            connect(c->m_widgetProxy,SIGNAL(attributeChanged()),this,SLOT(signalMatChanged()));
        }
        break;
        case(5):
        {
            //create a normal block
            OSLVarNormalBlock *n = new OSLVarNormalBlock(m_nodeInterfaceScene,m_material);
            connect(n->m_widgetProxy,SIGNAL(attributeChanged()),this,SLOT(signalMatChanged()));
        }
        break;
        case(6):
        {
            //create a point block
            OSLVarPointBlock *p = new OSLVarPointBlock(m_nodeInterfaceScene,m_material);
            connect(p->m_widgetProxy,SIGNAL(attributeChanged()),this,SLOT(signalMatChanged()));
        }
        break;
        case(7):
        {
            //create a var image block
            OSLVarImageBlock *i = new OSLVarImageBlock(m_nodeInterfaceScene,m_material);
            connect(i->m_widgetProxy,SIGNAL(attributeChanged()),this,SLOT(signalMatChanged()));
        }
        break;
        //if nothing do nothing
        default: break;
        }
    }


}

//------------------------------------------------------------------------------------------------------------------------------------
void AbstractMaterialWidget::addShaderNode()
{
    //let the user select a shader to load in
    QStringList locations = QFileDialog::getOpenFileNames(0,QString("Import Shader"), QString("shaders/"), QString("OSL files (*.osl)"));

    for(int i=0;i<locations.size();i++)
    {
        //if nothing selected then we dont want to do anything
        if(locations[i].isEmpty()) return;

        //create a new shader node in our ui
        //add it to out interface. This needs to be don before we add any
        //ports or it will not work, should probably do something about this
        OSLShaderBlock *b = new OSLShaderBlock();
        m_nodeInterfaceScene->addItem(b);
        if(!b->loadShader(locations[i]))
        {
            QMessageBox::warning(this,"Compile Error","OSL Shader could not be compiled!");
            m_nodeInterfaceScene->removeItem(b);
            delete b;
        }
    }
}
//------------------------------------------------------------------------------------------------------------------------------------
void AbstractMaterialWidget::newMaterial()
{
    m_nodeEditor->getScene()->clear();
    m_material = PathTracerScene::getInstance()->getContext()->createMaterial();
    m_matCreated=false;
    m_matAddedToLib = false;
    m_curNGPath.clear();
}

//------------------------------------------------------------------------------------------------------------------------------------
bool AbstractMaterialWidget::addMaterialToLib(){
    if(!m_matCreated)
    {
        QMessageBox::warning(this,"Add Material to Library","No material created");
        return false;
    }
    if(m_matAddedToLib)
    {
        QMessageBox::warning(this,"Add Material to Library","Material already added to library");
        return false;
    }
    std::string matName = QInputDialog::getText(this,"Add Material to Library","Material Name",QLineEdit::Normal).toStdString();
    if(matName.length()==0)
    {
        QMessageBox::warning(this,"Add Material to Library","You must give your material a name to add it to the library");
        return false;
    }

    if(!QDir("NodeGraphs").exists()) QDir().mkdir("NodeGraphs");
    QFile f(("NodeGraphs/" + matName + ".hel").c_str());
    f.open(QFile::WriteOnly);
    QDataStream ds(&f);
    m_nodeEditor->save(ds);
    f.close();
    if(MaterialLibrary::getInstance()->addMaterialToLibrary(matName,m_material))
    {
        m_matAddedToLib = true;
        m_curNGPath = ("NodeGraphs/" + matName + ".hel").c_str();
        m_curMatName = matName;
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------------------------------------------------------------
void AbstractMaterialWidget::importNodeGraph()
{
    QString location = QFileDialog::getOpenFileName(this,tr("Import Node Graph"),"", tr("Mesh Files (*.hel)"));
    QFile f(location);
    if(f.open(QFile::ReadOnly))
    {
        QDataStream ds(&f);
        m_nodeEditor->load(ds);
    }
    else
    {
        QMessageBox::warning(this,"Import Node Graph","Cannot load node graph");
    }
}
//------------------------------------------------------------------------------------------------------------------------------------
void AbstractMaterialWidget::loadNodeGraph(QString _path)
{
    QFile f(_path);
    if(f.open(QFile::ReadOnly))
    {
        QDataStream ds(&f);
        m_nodeEditor->load(ds);
        m_curNGPath = _path;
    }
    else
    {
        QMessageBox::warning(this,"Import Node Graph","Cannot load node graph");
    }
}
//------------------------------------------------------------------------------------------------------------------------------------
void AbstractMaterialWidget::saveNodeGraph()
{
    if(m_curNGPath.isEmpty())
    {
        addMaterialToLib();
    }
    else
    {
        QFile f(m_curNGPath);
        f.open(QFile::WriteOnly);
        QDataStream ds(&f);
        m_nodeEditor->save(ds);
    }
}
//------------------------------------------------------------------------------------------------------------------------------------
