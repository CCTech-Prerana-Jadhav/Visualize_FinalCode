#include <QFileDialog>
#include <QGridLayout>
#include "Visualizer.h"
#include "STLReader.h"
#include "OBJReader.h"
#include "OBJWriter.h"
#include "STLWriter.h"


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

}

void Visualizer::setupUi()
{
    loadFile = new QPushButton("Load File", this);
    translate = new QPushButton("Translate", this);
    exportFile= new QPushButton("Export", this);
    openglWidgetInput = new OpenGlWidget(this);
    openglWidgetOutput = new OpenGlWidget(this);
    progressbar = new QProgressBar(this);
    graphicsSynchronizer = new GraphicsSynchronizer(openglWidgetInput, openglWidgetOutput);

    QGridLayout* layout = new QGridLayout(this);

    layout->addWidget(loadFile, 0, 0);
    layout->addWidget(translate, 0, 2);
    layout->addWidget(openglWidgetInput, 1, 0);
    layout->addWidget(openglWidgetOutput, 1, 2, 1, 2);
    layout->addWidget(exportFile, 0, 3);
    layout->addWidget(progressbar, 2, 0);

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
            loadSTLFile(inputFilePath, triangulation, openglWidgetInput);
        }
        else if (inputFilePath.endsWith(".obj", Qt::CaseInsensitive))
        {
            loadOBJFile(inputFilePath, triangulation, openglWidgetInput);
        }
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

    progressbar->setValue(0);

    
    if (inputFilePath.endsWith(".stl", Qt::CaseInsensitive))
    {
        QString exportFileName = dir + "/output.obj";
        ObjWriter writer;
        writer.Write(exportFileName.toStdString(), triangulation);

        

        loadOBJFile(exportFileName, outputTriangulation, openglWidgetOutput);
        QFile::remove(exportFileName);
    }
    else if (inputFilePath.endsWith(".obj", Qt::CaseInsensitive))
    {
        QString exportFileName = dir + "/output.stl";
        STLWriter writer;
        writer.Write(exportFileName.toStdString(), triangulation);

        loadSTLFile(exportFileName, outputTriangulation, openglWidgetOutput);
        QFile::remove(exportFileName);
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
    int count = 1;
    for each (const Triangle& triangle in inTriangulation.Triangles)
    {
        for each (const Point& point in triangle.Points())
        {
            data.vertices.push_back(inTriangulation.UniqueNumbers[point.X()]);
            data.vertices.push_back(inTriangulation.UniqueNumbers[point.Y()]);
            data.vertices.push_back(inTriangulation.UniqueNumbers[point.Z()]);
        }

        const Point& normal = triangle.Normal();

        for (size_t i = 0; i < 3; i++)
        {
            data.normals.push_back(inTriangulation.UniqueNumbers[normal.X()]);
            data.normals.push_back(inTriangulation.UniqueNumbers[normal.Y()]);
            data.normals.push_back(inTriangulation.UniqueNumbers[normal.Z()]);
        }

        progressbar->setValue(count);
        count++;
    }

    return data;
}

void Visualizer::loadOBJFile(const QString& filepath, Triangulation& triangulation, OpenGlWidget* openglWidgetWindow)
{
    OBJReader objreader;
    objreader.read(filepath.toStdString(), triangulation);

    progressbar->setRange(0, triangulation.Triangles.size());

    OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(triangulation);
    openglWidgetWindow->setData(data);
}

void Visualizer::loadSTLFile(const QString& filepath, Triangulation& triangulation, OpenGlWidget* openglWidgetWindow)
{
    STLReader stlreader;
    stlreader.read(filepath.toStdString(), triangulation);

    progressbar->setRange(0, triangulation.Triangles.size());

    OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(triangulation);
    openglWidgetWindow->setData(data);
}