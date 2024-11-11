#include <QFileDialog>
#include <QGridLayout>
#include <QFile>
#include "Visualizer.h"
#include "STLReader.h"
#include "OBJReader.h"
#include "OBJWriter.h"
#include "STLWriter.h"
#include "DataWriter.h"

Visualizer::Visualizer(QWidget* parent)
    : QMainWindow(parent)
{
    setupUi();
    connect(loadFile, &QPushButton::clicked, this, &Visualizer::onLoadFileClick);
    connect(translate, &QPushButton::clicked, this, &Visualizer::onTranslateClick);
    connect(exportFile, &QPushButton::clicked, this, &Visualizer::onExportClick);
}

Visualizer::~Visualizer()
{
    if (QFile::exists(inputFilePath))
    {
        QFile::remove(inputFilePath);
        qDebug() << "File Deleted";
    }
}

void Visualizer::setupUi()
{
    loadFile = new QPushButton("Load File", this);
    translate = new QPushButton("Translate", this);
    exportFile= new QPushButton("Export", this);
    openglWidgetInput = new OpenGlWidget(this);
    openglWidgetOutput = new OpenGlWidget(this);
    graphicsSynchronizer = new GraphicsSynchronizer(openglWidgetInput, openglWidgetOutput);

    QGridLayout* layout = new QGridLayout(this);

    layout->addWidget(loadFile, 0, 0);
    layout->addWidget(translate, 0, 1);
    layout->addWidget(openglWidgetInput, 1, 0);
    layout->addWidget(openglWidgetOutput, 1, 1, 1, 2);
    layout->addWidget(exportFile, 0, 2);


    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setLayout(layout);
}

void  Visualizer::onLoadFileClick()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "", tr("files (*.stl *.obj)"));

    if (!fileName.isEmpty())
    {
        inputFilePath = fileName;
        if(inputFilePath.endsWith(".stl",Qt::CaseInsensitive))
        {
            STLReader reader;
            reader.read(inputFilePath.toStdString(), triangulation);
        }
        else if (inputFilePath.endsWith(".obj", Qt::CaseInsensitive))
        {
            OBJReader reader;
            reader.read(inputFilePath.toStdString(), triangulation);
        }
        OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(triangulation);
        openglWidgetInput->setData(data);
    }
}

void Visualizer::onTranslateClick()
{
    QFileDialog dialog(this);

    dialog.setFileMode(QFileDialog::Directory);

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
        "/home",
        QFileDialog::ShowDirsOnly
        | QFileDialog::DontResolveSymlinks);

    
    if (inputFilePath.endsWith(".stl", Qt::CaseInsensitive))
    {
        QString exportFileName = dir + "/output.obj";
        inputFilePath = exportFileName;
        Geometry::Matrix4x4 mat;
        Transformation::Transformation t;
        outputTriangulation =   t.scaling(triangulation,mat);
        for (auto i : outputTriangulation.Triangles)
        {
                qDebug() << i.P1().X() << i.P1().Y() << i.P1().Z() << "|" << i.P2().X() << i.P2().Y() << i.P2().Z() << "|" << i.P3().X() << i.P3().Y() << i.P3().Z();
        }
        qDebug() << outputTriangulation.Triangles[0].P1().Z();

        ObjWriter writer;
        writer.Write(exportFileName.toStdString(), outputTriangulation);
        // reload file to check and load in output renderer
        OBJReader reader;
        reader.read(exportFileName.toStdString(), outputTriangulation);

        OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(outputTriangulation);
        openglWidgetOutput->setData(data);
        

    }
    else if (inputFilePath.endsWith(".obj", Qt::CaseInsensitive))
    {
        QString exportFileName = dir + "/output.stl";
        inputFilePath = exportFileName;
        Geometry::Matrix4x4 mat;
        Transformation::Transformation t;
        outputTriangulation = t.scaling(triangulation, mat);
        STLWriter writer;
        writer.Write(exportFileName.toStdString(), outputTriangulation);


        // reload file to check and load in output renderer
        STLReader reader;
        reader.read(exportFileName.toStdString(), outputTriangulation);

        OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(outputTriangulation);
        openglWidgetOutput->setData(data);
    }

}

void Visualizer::onExportClick()
{
    QFileDialog dialog(this);

    if (inputFilePath.endsWith(".stl", Qt::CaseInsensitive))
    {
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save File"), "", tr("files (*.obj)"));
        ObjWriter writer;
        writer.Write(fileName.toStdString(), outputTriangulation);
    }
    else if (inputFilePath.endsWith(".obj", Qt::CaseInsensitive))
    {
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save File"), "", tr("files (*.stl)"));
        STLWriter writer;
        writer.Write(fileName.toStdString(), outputTriangulation);
    }
}

OpenGlWidget::Data Visualizer::convertTrianglulationToGraphicsObject(const Triangulation& inTriangulation)
{
    OpenGlWidget::Data data;
    for each (Triangle triangle in inTriangulation.Triangles)
    {
        for each (Point point in triangle.Points())
        {
            data.vertices.push_back(inTriangulation.UniqueNumbers[point.X()]);
            data.vertices.push_back(inTriangulation.UniqueNumbers[point.Y()]);
            data.vertices.push_back(inTriangulation.UniqueNumbers[point.Z()]);
        }
        
        Point normal = triangle.Normal();

        for (size_t i = 0; i < 3; i++)
        {
            data.normals.push_back(inTriangulation.UniqueNumbers[normal.X()]);
            data.normals.push_back(inTriangulation.UniqueNumbers[normal.Y()]);
            data.normals.push_back(inTriangulation.UniqueNumbers[normal.Z()]);
        }
    }
    return data;
}

    