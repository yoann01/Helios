#include "UI/AbstractMaterialWidget.h"
#include "Core/pathtracerscene.h"
#include "NodeGraph/OSLShaderBlock.h"
#include "NodeGraph/OSLVarFloatBlock.h"
#include "NodeGraph/OSLVarFloatThreeBlock.h"
#include "NodeGraph/OSLVarColorBlock.h"
#include "NodeGraph/OSLVarNormalBlock.h"
#include "NodeGraph/OSLVarPointBlock.h"
#include "NodeGraph/OSLVarIntBlock.h"
#include <QMenu>
#include <QPoint>
#include <QFileDialog>
#include <QFileInfo>
#include <QPushButton>
#include <QMessageBox>
#include <iostream>

//declare our static class instance
AbstractMaterialWidget* AbstractMaterialWidget::m_instance;

AbstractMaterialWidget::AbstractMaterialWidget(QWidget *parent) :
    QDockWidget(parent,Qt::Window)
{
    this->setMinimumHeight(500);
    this->setMinimumWidth(700);
    this->setWindowTitle("OSL Hypershader 3000");
    m_matCreated = false;
    //add our groupbox
    m_widgetGroupBox = new QGroupBox(this);
    this->setWidget(m_widgetGroupBox);
    m_groupBoxLayout = new QGridLayout(this);
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
    m_nodeEditor->install(m_nodeInterfaceScene);

    //add a button to to launch the creation of our shader
    QPushButton *createShaderBtn = new QPushButton("Create Shader",this);
    m_groupBoxLayout->addWidget(createShaderBtn,0,1,1,1);
    connect(createShaderBtn,SIGNAL(clicked()),this,SLOT(createOptixMaterial()));


    //Set up our menu for if you right click in our widget
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showContextMenu(QPoint)));

    //create a material in our context
    m_material = PathTracerScene::getInstance()->getContext()->createMaterial();
}
//------------------------------------------------------------------------------------------------------------------------------------
AbstractMaterialWidget::~AbstractMaterialWidget(){
    for(unsigned int i=0; i<m_nodes.size();i++){
        delete m_nodes[i];
    }
}

AbstractMaterialWidget* AbstractMaterialWidget::getInstance(QWidget *parent)
{
    if(m_instance){
        if(parent){
            if(m_instance->parent()){
                std::cerr<<"AbstractMaterialWidget already has a parent"<<std::endl;
            }
            else{
                m_instance->setParent(parent);
            }
        }
    }
    else{
        m_instance = new AbstractMaterialWidget(parent);
    }
    return m_instance;
}
//------------------------------------------------------------------------------------------------------------------------------------
void AbstractMaterialWidget::createOptixMaterial(){
    //turn our graph editor into an optix material program
    std::string status = m_nodeEditor->compileMaterial(m_material);
    if(status=="Material Compiled"){
        m_matCreated = true;
    }
    else{
        QMessageBox::warning(this,tr("Shader Compilation Failed"),tr(status.c_str()));
        m_matCreated=false;
    }

}
//------------------------------------------------------------------------------------------------------------------------------------
void AbstractMaterialWidget::applyMaterialToMesh(std::string _mesh)
{
    if(m_matCreated){
        std::cerr<<"Adding material "<<m_materialName<<" to mesh "<<_mesh<<std::endl;
        PathTracerScene::getInstance()->setModelMaterial(_mesh,m_material);
    }
    else{
        QMessageBox::warning(this,"Add Material","No OSL shader created");
    }
}
//------------------------------------------------------------------------------------------------------------------------------------
void AbstractMaterialWidget::showContextMenu(const QPoint &pos){

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

    //find out if something has been clicked
    QAction* selectedItem = myMenu.exec(globalPos);
    if(selectedItem){
        switch(selectedItem->data().toInt())
        {
        case(0): addShaderNode(); break;
        case(1):{
            //create a float block
            OSLVarFloatBlock *f = new OSLVarFloatBlock(m_nodeInterfaceScene, m_material);
            m_nodes.push_back(f);
        }
        break;
        case(2):{
            OSLVarIntBlock *i = new OSLVarIntBlock(m_nodeInterfaceScene, m_material);
            m_nodes.push_back(i);
        }
        break;
        case(3):{
            OSLVarFloatThreeBlock *v = new OSLVarFloatThreeBlock(m_nodeInterfaceScene,m_material);
            m_nodes.push_back(v);
        }
        break;
        case(4):{
            OSLVarColorBlock *c = new OSLVarColorBlock(m_nodeInterfaceScene,m_material);
            m_nodes.push_back(c);
        }
        break;
        case(5):{
            OSLVarNormalBlock *n = new OSLVarNormalBlock(m_nodeInterfaceScene,m_material);
            m_nodes.push_back(n);
        }
        break;
        case(6):{
            OSLVarPointBlock *p = new OSLVarPointBlock(m_nodeInterfaceScene,m_material);
            m_nodes.push_back(p);
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
    QString location = QFileDialog::getOpenFileName(0,QString("Import Shader"), QString("shaders/"), QString("OSL files (*.osl)"));
    //if nothing selected then we dont want to do anything
    if(location.isEmpty()) return;

    //create a new shader node in our ui
    //add it to out interface. This needs to be don before we add any
    //ports or it will not work, should probably do something about this
    OSLShaderBlock *b = new OSLShaderBlock();
    m_nodeInterfaceScene->addItem(b);
    if(!b->loadShader(location)){
        QMessageBox::warning(this,"Compile Error","OSL Shader could not be compiled!");
        m_nodeInterfaceScene->removeItem(b);
        delete b;
    }
    else{
        //add it to our list of nodes
        m_nodes.push_back(b);
    }
}
//------------------------------------------------------------------------------------------------------------------------------------
