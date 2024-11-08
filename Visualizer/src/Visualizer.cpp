#include <QFileDialog>
#include <QGridLayout>
#include <QStatusBar>
#include "Visualizer.h"
#include "STLReader.h"
#include "OBJReader.h"
#include "OBJWriter.h"
#include "STLWriter.h"
#include "DataWriter.h"
#include "Transform.h"

using namespace Transformation;

Visualizer::Visualizer(QWidget* parent)
    : QMainWindow(parent)
{
    setupUi();

    connect(loadFile, &QPushButton::clicked, this, &Visualizer::onLoadFileClick);
    connect(translate, &QPushButton::clicked, this, &Visualizer::onTranslateClick);
    connect(exportFile, &QPushButton::clicked, this, &Visualizer::onExportClick);
    connect(transform, &QPushButton::clicked, this, &Visualizer::onTransformClick);

}

Visualizer::~Visualizer()
{
}

void Visualizer::setupUi()
{
    loadFile = new QPushButton("Load File", this);
    translate = new QPushButton("Translate", this);
    exportFile= new QPushButton("Export", this);
    transform = new QPushButton("Transform", this);
    openglWidgetInput = new OpenGlWidget(this);
    openglWidgetOutput = new OpenGlWidget(this);
    progressbar = new QProgressBar(this);
    customStatusBar = new QStatusBar(this);

    graphicsSynchronizer = new GraphicsSynchronizer(openglWidgetInput, openglWidgetOutput);

    QGridLayout* layout = new QGridLayout(this);

    layout->addWidget(loadFile, 0, 0, 1, 2);
    layout->addWidget(translate, 0, 2, 1, 2);
    layout->addWidget(exportFile, 0, 4);
    layout->addWidget(openglWidgetInput, 1, 0, 1, 3);
    layout->addWidget(openglWidgetOutput, 1, 3, 1, 3);
    layout->addWidget(progressbar, 2,0,1,6);
    layout->addWidget(customStatusBar, 3, 0, 1, 6);
    layout->addWidget(transform, 0, 5);


    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setLayout(layout);
    customStatusBar->setFixedHeight(30);

    customStatusBar->showMessage("Welcome to VISUALIZER APP ! Load your file.");
}

void  Visualizer::onLoadFileClick()
{
    customStatusBar->showMessage("Select a file to load...");
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "", tr("files (*.stl *.obj)"));

    if (!fileName.isEmpty())
    {
        inputFilePath = fileName;
        if (inputFilePath.endsWith(".stl", Qt::CaseInsensitive))
        {
            customStatusBar->showMessage("Loading STL file...");
            loadSTLFile(inputFilePath, triangulation, openglWidgetInput);
        }
        else if (inputFilePath.endsWith(".obj", Qt::CaseInsensitive))
        {
            customStatusBar->showMessage("Loading OBJ file...");
            loadOBJFile(inputFilePath, triangulation, openglWidgetInput);
        }
        else
        {
            customStatusBar->showMessage("No file selected.");
        }
    }
}

void Visualizer::onTranslateClick()
{
    customStatusBar->showMessage("Select directory to export temporary file...");
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
        customStatusBar->showMessage("Converting to OBJ File...");
        writer.Write(exportFileName.toStdString(), triangulation,progressbar);
        loadOBJFile(exportFileName, outputTriangulation, openglWidgetOutput);
    }
    else if (inputFilePath.endsWith(".obj", Qt::CaseInsensitive))
    {
        QString exportFileName = dir + "/output.stl";
        STLWriter writer;
        customStatusBar->showMessage("Converting to STL File...");
        writer.Write(exportFileName.toStdString(), triangulation, progressbar);
        loadSTLFile(exportFileName, outputTriangulation, openglWidgetOutput);

    }

}

void Visualizer::onExportClick()
{
    customStatusBar->showMessage("Select a location to save the file...");
    QFileDialog dialog(this);
    progressbar->setValue(0);
    if (inputFilePath.endsWith(".stl", Qt::CaseInsensitive))
    {
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save File"), "", tr("files (*.obj)"));
        ObjWriter writer;
        customStatusBar->showMessage("Exporting OBJ file...");
        writer.Write(fileName.toStdString(), outputTriangulation, progressbar);
        customStatusBar->showMessage("OBJ file exported successfully.");
    }
    else if (inputFilePath.endsWith(".obj", Qt::CaseInsensitive))
    {
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save File"), "", tr("files (*.stl)"));
        STLWriter writer;
        customStatusBar->showMessage("Exporting STL file...");
        writer.Write(fileName.toStdString(), outputTriangulation, progressbar);
        customStatusBar->showMessage("STL file exported successfully.");
    }

}

void Visualizer::onTransformClick()
{
    Transform transform;
    transform.Scale(triangulation, outputTriangulation, 5);
    OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(outputTriangulation);
    openglWidgetOutput->setData(data);
}

OpenGlWidget::Data Visualizer::convertTrianglulationToGraphicsObject(const Triangulation& inTriangulation)
{
    OpenGlWidget::Data data;
    int count = 1;
    for each (const Triangle & triangle in inTriangulation.Triangles)
    {
        for each (const Point & point in triangle.Points())
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
    customStatusBar->showMessage("Reading OBJ File ...");
    objreader.read(filepath.toStdString(), triangulation);

    progressbar->setRange(0, triangulation.Triangles.size());

    OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(triangulation);
    openglWidgetWindow->setData(data);

    customStatusBar->showMessage("OBJ file loaded successfully.");
}

void Visualizer::loadSTLFile(const QString& filepath, Triangulation& triangulation, OpenGlWidget* openglWidgetWindow)
{
    STLReader stlreader;
    customStatusBar->showMessage("Reading STL File ...");
    stlreader.read(filepath.toStdString(), triangulation);

    progressbar->setRange(0, triangulation.Triangles.size());

    OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(triangulation);
    openglWidgetWindow->setData(data);

    customStatusBar->showMessage("STL file loaded successfully.");
}
